; Unions

; Todo:
;  - finish the code below
;  - add this file to the packages
;  - add a separate field and member resolution phase.
;  - figure out how union literals are going to work
;  - to-cps method for type-case
;  - C generation methods for unions, union literals, type-case, and
;    x->type (which is a cast).

; Plan - datatypes with the initial restriction that a datatype
; cannot be polymorphic (does this matter?) and cannot have more than one
; non-nullary constructor.  What is needed is a new expander and a couple
; of primitives:
;
; (x->union value 'union-type 'variant)
; (type-case value 'union-type '(variant ...) . conts)
;
; X->UNION does type checking and is a no-op in C.
; TYPE-CASE does type checking (VALUE must have type UNION-TYPE and the
; continuations receive arguments of the type of the corresponding variant).
; The C code does a switch for the nullary constructors and a second switch
; using the type field of the non-nullary constructors.  Not every type
; needs both switches.

; Representation of union types.

(define-record-type union-type :union-type
  (really-make-union-type name)
  union-type?
  (name union-type-name)
  ; MEMBERS are filled in later because of circularity
  (members union-type-members set-union-type-members))

(define-union-discloser :union-type
  (lambda (rtype)
    (list 'union-type (union-type-name rtype))))

; Members of union types.

(define-record-type union-member :union-member
  (make-union-member union-type name type)
  union-member?
  (union-type	union-member-union-type)
  (name		union-member-name)
  (type		union-member-type))

; Global table of union types.  Since we compile to a single C file the
; union types used within a single computation must have distinct names.
; (This should really be a fluid.)

(define *union-type-table* (make-symbol-table))

(define (reset-union-data!)
  (set! *union-type-table* (make-symbol-table)))

(define (get-union-type id)
  (cond ((table-ref *union-type-table* id)
	 => identity)
	(else
	 (error "no union type ~S" id))))

(define (lookup-union-type id)
  (table-ref *union-type-table* id))

(define (all-union-types)
  (table->entry-list *union-type-table*))

; Construction a union type.  This gets the name and the member specifications.
; Each member specification consists of a name and a type.

; Aargh.  What to do?  The simplest thing might be to expand define-data-type
; into define-record-types and and a define-union-type.  Except that we hit
; the circularity problem again.  We need to create record field type after
; all macro expansion has been completed.

(define (make-union-type id specs)
  (let ((ut (really-make-union-type id)))
    (if (table-ref *union-type-table* id)
	(user-error "multiple definitions of union type ~S" id))
    (table-set! *union-type-table* id ut)
    (set-union-type-members! ut (map (lambda (spec)
				       (make-union-member
					  ut
					  (car spec)
					  (expand-type-spec (cadr spec))))
				     specs))
    (set-union-type-constructor-args! ut
				       (map (lambda (name)
					      (get-union-type-member id name))
					    constructor-args))
    ut))

; Return the member record for MEMBER-ID in union-type TYPE-ID.

(define (get-union-type-member type-id member-id)
  (let ((utype (get-union-type type-id)))
    (cond ((any (lambda (member)
		  (eq? member-id (union-member-name member)))
		(union-type-members utype))
	   => identity)
	  (else
	   (user-error "~S is not a member of ~S" member-id utype)))))

; Returns a list of the expected continuation types (each of which returns
; UVAR) as its result.

(define (get-union-deconstruction-types type-id uvar)
  ...)

;(define-data-type list delistifier
;  (pair :pair pair->list
;        (make-pair car cdr)
;        (car integer car set-car!)
;        (cdr list    cdr set-cdr!))
;  (null))
;
; ->
;
; (define-record-type pair :pair
;   (make-pair car cdr)
;   (car integer car set-car!)
;   (cdr list    cdr set-cdr!))
; (define-union-type list delistifier
;   (pair :pair pair->list)		; must be a record type
;   (null))				; implied unit type

; (define-union-type list delistifier
;   (pair :pair)
;   null)		; implied unit type
; ->
; (define (pair->list x) (x->union x 'list 'pair))
; (define null (x->union 0 'list 'null))	; needs compile-time objects
; (define (delistifier x pair-cont null-cont)
;   (type-case x 'list '(pair null) pair-cont null-cont))

(define (expand-union-type exp r c)
  (let ((id (cadr exp))
	(deconstructor (caddr exp))
	(members (cdddr exp)))
    (let* ((ut (make-union-type id members))
	   (conts (make-names members r)))
      `(,(r 'begin)
	(,(r 'define) (,deconstructor ,(r 'x) . ,conts)
	   (,(r 'type-case) ,(r 'x) ',id . ,conts))
	. ,(apply append
		  (do ((members members (cdr members))
		       (i 0 (+ i 1))
		       (res '() (cons (expand-member (car members) i id r)
				      res)))
		      ((null? members)
		       (apply append (reverse res)))))))))

; MEMBER is either (tag-name type injector-name) or just the tag-name.

(define (expand-member member i type-id r)
  (if (pair? member)
      `((,(r 'define) (,(caddr member) ,(r 'x))
	    (,(r 'x->union) ,(r 'x) ',type-id ',(car member))))
      `((,(r 'define) ,member
	    (,(r 'x->union) ,i ',type-id ',member)))))

(define (make-names things r)
  (do ((i 0 (+ i 1))
       (things things (cdr things))
       (names '() (cons (r (build-symbol 'c i))
			names)))
      (reverse names)))

(define (build-symbol . args)
  (string->symbol
   (string-append (map (lambda (x)
			 (cond ((string? x)
				x)
			       ((symbol? x)
				(symbol->string x))
			       ((number? x)
				(number->string x))
			       (else
				(error "can't coerce to string" x))))
		       args))))
			     
;(define (expand-define-data-type exp r c)
;  (let ((id (cadr exp))
;        (deconstructor (caddr exp))
;        (options (cdddr exp)))
;    (let* ((specs (map (lambda (option)
;                                     (if (null? (cdr option))
;                                         option
;                                         (cons (car option)
;                                               (map car (cddddr option)))))
;                                     options))
;           (dt (make-union-type id specs))
;           (conts (make-names specs r)))
;      `(,(r 'begin)
;        (,(r 'define) (,deconstructor ,(r 'x) . ,conts)
;           (,(r 'type-case) ,(r 'x) ',id . ,conts))
;        . ,(apply append
;                  (do ((options options (cdr options))
;                       (i 0 (+ i 1))
;                       (res '() (cons (expand-option (car options) i id r)
;                                      res)))
;                      ((null? options)
;                       (apply append (reverse res)))))))))
;                             
;(define (expand-option option i type-id r)
;  (if (null? (cdr option))
;      `((,(r 'define) (,(car option))
;            (,(r 'x->union) ,i ',type-id ',(car option))))
;      `((,(r 'define-record-type) ,(car option)
;                                  ,(cadr option)
;                                  . ,(cdddr option))
;        (,(r 'define) (,(caddr option) ,(r 'x))
;            (,(r 'x->union) ,(r 'x) ',type-id ',(car option))))))

; I'll live without this for now.
;
;(define-syntax delistify
;  (syntax-rules ()
;    ((delistify value . clauses)
;     (really-delistify value clauses #f match-error #f match-error))))
;
;(define-syntax really-delistify
;  (syntax-rules ()
;    ((really-delistify value () pair? pair-cont null? null-cont)
;     (delistifier value pair-cont null-cont))
;    ((really-delistify value
;                       (((pair car-var cdr-var) code . more-code)
;                        . more-clauses)
;                        #f pair-cont null? null-cont)
;     (really-delistify value
;                       more-clauses
;                       #t
;                       (lambda (a-pair)
;                         (let ((car-var (car a-pair))   ; could check for #F
;                               (cdr-var (cdr a-pair)))
;                           code . more-code))
;                       null?
;                       null-cont))
;    ((really-delistify value
;                       (((pair . pair-var) code . more-code)
;                        . more-clauses)
;                        #f pair-cont null? null-cont)
;     (really-delistify value
;                       more-clauses
;                       #t
;                       (lambda (pair-var)
;                           code . more-code)
;                       null?
;                       null-cont))
;    ((really-delistify value
;                       (((null) code . more-code)
;                        . more-clauses)
;                        pair? pair-cont #f null-cont)
;     (really-delistify value
;                       more-clauses
;                       pair?
;                       pair-cont
;                       #t
;                       (lambda ()
;                         code . more-code)))
;    (error)))
