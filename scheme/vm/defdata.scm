; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Macros for defining data types.

; An ugly and unsafe macro for defining VM data structures.
;
; (DEFINE-PRIMITIVE-DATA-TYPE <name> <type> <immutable?> <constructor-name>
;                             <slot>*)
; <slot> ::= (<accessor-name>) | (<accessor-name> <modifier-name>)
;
; (define-primitive-data-type pair N #f cons (car set-car!) (cdr))
; =>
; (begin
;  (define (cons a b) (d-vector N ...))
;  (define pair? (stob-predicate ...))
;  (define pair-size 3)
;  (define (car x) (d-vector-ref x 0))
;  (define (set-car! x val) (d-vector-set! x 0 val))
;  (define (cdr x) (d-vector-ref x 1))

(define-syntax define-primitive-data-type
  (lambda (exp rename compare)
    (destructure (((d-p-d-t name type immutable? make . body) exp))
      (define (concatenate-symbol . syms)
	(string->symbol (apply string-append (map symbol->string syms))))
      (let* ((pred (concatenate-symbol name '?))
	     (size (concatenate-symbol name '- 'size))
	     (shorten (lambda (l1 l2) (map (lambda (x1 x2) x2 x1) l1 l2)))
	     (vars (shorten `(a b c d e f g h i j) body)))
	`(begin ,@(if make
		      `((define ,make
			  (let ((type (enum stob ,type)))
			    (lambda (,@vars key)
			      ,(if immutable?
				   `(immutable-d-vector type key ,@vars)
				   `(d-vector type key ,@vars))))))
		      '())
		(define ,pred (stob-predicate (enum stob ,type)))
		(define ,size (+ ,(length body) stob-overhead))
		,@(do ((s body (cdr s))
		       (i 0 (+ i 1))
		       (d '() (let* ((slot (car s))
				     (d (cons `(define (,(car slot) x)
						 (d-vector-ref x ,i))
					      d)))
				(if (null? (cdr slot))
				    d
				    (cons `(define (,(cadr slot) x val)
					     (d-vector-set! x ,i val))
					  d)))))
		      ((null? s) (reverse d))))))))

; This is a front for DEFINE-PRIMITIVE-DATA-TYPE that gets the names from
; STOB-DATA (which is defined in arch.scm).  This ensures that the run-time
; code, the VM, and the linker agree on what these structures look like.
;
; SCHEME? is #T if the data structure is a Scheme structure, in which case
; the names defined by the form all have VM- prepended.

(define-syntax define-shared-primitive-data-type
  (lambda (exp rename compare)
    (let* ((name (cadr exp))
	   (scheme? (if (null? (cddr exp)) #f (car (cddr exp))))
	   (immutable? (if (or (null? (cddr exp))
			       (null? (cdddr exp)))
			   #f
			   (cadr (cddr exp))))
	   (rest (if (or (null? (cddr exp))
			 (null? (cdddr exp)))
		     '()
		     (cddddr exp)))
	   (extra-maker (if (null? rest) #f (car rest)))
	   (extra-setters (if (or (null? rest)
				  (null? (cdr rest)))
			      '()
			      (cadr rest)))
	   (extra-fields (if (or (null? rest)
				 (null? (cdr rest)))
			     '()
			     (cddr rest))))
      (define (concatenate-symbol . syms)
	(string->symbol (apply string-append (map symbol->string syms))))
      (let ((data (cddr (assq name stob-data)))
	    (fixup (lambda (n)
		     (if scheme? (concatenate-symbol 'vm- n) n))))
	`(define-primitive-data-type
	   ,(fixup name)
	   ,name
	   ,immutable?
	   ,(fixup (if (car data) (car data) extra-maker))
	   . ,(map (lambda (p)
		     (cons (fixup (car p))
			   (cond ((and (not (null? (cdr p)))
				       (cadr p))
				  (list (fixup (cadr p))))
				 ((assq (car p) extra-setters)
				  => cdr)
				 (else '()))))
		   (append (cdr data) extra-fields)))))))

; A d-vector macro version of the VECTOR procedure.
; This is only used in the expansion of DEFINE-PRIMITIVE-DATA-TYPE.

(define-syntax d-vector
  (lambda (exp rename compare)
    (destructure (((d-v type key . args) exp))
      `(let ((v (make-d-vector ,type ,(length args) key)))
	 ,@(do ((a args (cdr a))
		(i 0 (+ i 1))
		(z '() (cons `(d-vector-init! v ,i ,(car a)) z)))
	       ((null? a) (reverse z)))
	 v))))

(define-syntax immutable-d-vector
  (syntax-rules ()
   ((immutable-d-vector stuff ...)
    (let ((vec (d-vector stuff ...)))
      (make-immutable! vec)
      vec))))

; A simpler macro for defining types of vectors.  Again SCHEME? being #T
; causes VM- to be prepended to the defined names.

(define-syntax define-vector-data-type
  (lambda (exp rename compare)
    (let ((name (cadr exp))
	  (scheme? (cddr exp)))
      (define (concatenate-symbol . syms)
	(string->symbol (apply string-append (map symbol->string syms))))
      (let* ((type `(enum stob ,name))
	     (fix (if (not (null? scheme?))
		      'vm-
		      (string->symbol "")))
	     (pred (concatenate-symbol fix name '?))
	     (make (concatenate-symbol fix 'make- name))
	     (size (concatenate-symbol fix name '- 'size))
	     (length (concatenate-symbol fix name '- 'length))
	     (ref (concatenate-symbol fix name '- 'ref))
	     (set (concatenate-symbol fix name '- 'set!)))
	`(begin (define ,make (stob-maker ,type make-d-vector))
		(define ,pred (stob-predicate ,type))
		(define (,size len) (+ len stob-overhead))
		(define ,length d-vector-length)
		(define ,ref d-vector-ref)
		(define ,set d-vector-set!))))))
