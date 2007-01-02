; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Type lattice.
; Sorry this is so hairy, but before it was written, type checking 
; consumed 15% of compile time.

(define-record-type meta-type :meta-type
  (really-make-type mask more info)
  meta-type?
  (mask type-mask)
  (more type-more)
  (info type-info))

; MASK is a bit set.  The current bits are:
;
; Non values:
;   syntax
;   other static type
;   no values - indicates an optional type; the type with only this bit set
;     is the empty rail type.
;   two or more - indicates a rail-type with at least two elements
;     
; Values:
;   exact integer
;   integer
;   exact rational
;   rational
;   exact real
;   real
;   exact complex
;   complex
;   other exact number
;   other number
;   boolean
;   pair
;   null
;   record
;   procedure
;   other
;
; The MORE field is only used for rail types, which are like ML's tuples.
; If the TWO-OR-MORE? bit is set, then 
;  more = (head . tail).
; Otherwise, more = #f.
;
; For procedure types, the PROCEDURE bit is set and the INFO field is a three
; element list: (domain codomain restrictive?)
;  If INFO field for the type of F is (t1 t2 #t), then
;    if x : t1 then (f x) : t2 (possible error!), else (f x) : error.
;  If INFO field for the type of F is (t1 t2 #f), then
;    there exists an x : t1 such that (f x) : t2.
;
; For types which do not have bits, the OTHER bit is set and the INFO field is
; a symbol naming some type that doesn't have its own bit in the mask.  The
; other types defined in this file are:
;
;   :char
;   :unspecific
;   :string
;   :symbol
;   :vector
;   :escape
;   :structure
;
; More are constructed later by using SEXP->TYPE.

(define-record-discloser :meta-type
  (lambda (t)
    `(type ,(let ((m (type-mask t)))
	      (or (table-ref mask->name-table m)
		  m))
	   ,(let ((more (type-more t)))
	      (if (and (pair? more) (eq? (cdr more) t))
		  '*
		  more))
	   ,(type-info t))))

(define (make-type mask more info)
  (make-immutable!
   (really-make-type mask more info)))

(define name->type-table (make-table))
(define mask->name-table (make-table))

(define (name->type x)
  (or (table-ref name->type-table x)
      (make-other-type x)))

(define (set-type-name! type name)
  (table-set! name->type-table name type)
  (if (not (or (type-info type)
	       (type-more type)))
      (table-set! mask->name-table (type-mask type) name)))
	     
; Masks
; Top of lattice has mask = -1, bottom has mask = 0.

(define *mask* 1)

(define (new-type-bit)
  (let ((m *mask*))
    (set! *mask* (arithmetic-shift *mask* 1))
    m))

(define (mask->type mask)
  (make-type mask #f #f))

(define bottom-type (mask->type 0))
(define error-type bottom-type)

(define (bottom-type? t)
  (= (type-mask t) 0))

(set-type-name! bottom-type ':error)

(define (new-atomic-type)
  (mask->type (new-type-bit)))

(define (named-atomic-type name)
  (let ((t (new-atomic-type)))
    (set-type-name! t name)
    t))

; --------------------
; Top of the lattice.

(define syntax-type (named-atomic-type ':syntax))
(define other-static-type (new-atomic-type))

; --------------------

; "Rails" are argument sequence or return value sequences.
; Four constructors:
;   empty-rail-type
;   (rail-type t1 t2)
;   (optional-rail-type t1 t2)
;   (make-rest-type t)

; If a type's two-or-more? bit is set, then
;  more = (head . tail).
; Otherwise, more = #f.

(define empty-rail-type (new-atomic-type))

(define (rail-type t1 t2)		;CONS analog
  (cond ((empty-rail-type? t2) t1)
	((bottom-type? t1) t1)
	((bottom-type? t2) t2)
	((and (optional-type? t1)
	      (rest-type? t2)
	      (same-type? t1 (head-type t2)))
	 ;; Turn (&opt t &rest t) into (&rest t)
	 t2)
	((or (optional-type? t1)
	     (optional-type? t2))
	 (make-type (bitwise-ior (type-mask t1) mask/two-or-more)
		    (make-immutable! (cons t1 t2))
		    #f))
	(else
	 (make-type mask/two-or-more
		    (make-immutable! (cons t1 t2))
		    (type-info t1)))))

(define (make-optional-type t)
  (if (type-more t)
      (warn "peculiar type in make-optional-type" t))
  (make-type (bitwise-ior (type-mask t) mask/no-values)
	     #f
	     (type-info t)))

; A rest type is an infinite rail type with both the no-values and the
; two-or-more bits set.

(define (make-rest-type t)
  (if (bottom-type? t)
      t
      (let* ((z (cons (make-optional-type t) #f))
	     (t (make-type (bitwise-ior (type-mask t) mask/&rest)
			   z
			   (type-info t))))
	(set-cdr! z t)
	(make-immutable! z)
	t)))

(define (head-type t)			;Can return an &opt type
  (let ((more (type-more t)))
    (if more
	(car more)
	t)))

(define (head-type-really t)		;Always returns a value type
  (let ((h (head-type t)))
    (if (optional-type? h)
	(make-type (bitwise-and (type-mask h) (bitwise-not mask/no-values))
		   #f
		   (type-info h))
	h)))
	  
(define (tail-type t)
  (if (empty-rail-type? t)
      ;; bottom-type   ?
      (warn "rail-type of empty rail" t))
  (let ((more (type-more t)))
    (if more
	(cdr more)
	empty-rail-type)))

(define (empty-rail-type? t)
  (= (bitwise-and (type-mask t) mask/one-or-more) 0))

(define (rest-type? t)			;For terminating recursions
  (let ((more (type-more t)))
    (and more
	 (eq? (cdr more) t))))

(define (optional-type? t)
  (> (bitwise-and (type-mask t) mask/no-values) 0))


; The no-values type has one element, the rail of length zero.
; The two-or-more type consists of all rails of length two
; or more.

(define mask/no-values (type-mask empty-rail-type))
(define mask/two-or-more (new-type-bit))
(define mask/&rest (bitwise-ior (type-mask empty-rail-type)
				mask/two-or-more))

(table-set! mask->name-table mask/no-values ':no-values)

(define value-type (mask->type (bitwise-not (- *mask* 1))))
(set-type-name! value-type ':value)
(define mask/value (type-mask value-type))

(define (value-type? t)
  (let ((m (type-mask t)))
    (= (bitwise-and m mask/value) m)))

(define any-values-type
  (make-rest-type value-type))
(set-type-name! any-values-type ':values)
				   
(define any-arguments-type any-values-type)

(define mask/one-or-more
  (bitwise-ior mask/value mask/two-or-more))

; --------------------
; Lattice operations.

; Equivalence

(define (same-type? t1 t2)
  (or (eq? t1 t2)
      (and (= (type-mask t1) (type-mask t2))
	   (let ((more1 (type-more t1))
		 (more2 (type-more t2)))
	     (if more1
		 (and more2
		      (if (eq? (cdr more1) t1)
			  (eq? (cdr more2) t2)
			  (if (eq? (cdr more2) t2)
			      #f
			      (and (same-type? (car more1) (car more2))
				   (same-type? (cdr more1) (cdr more2))))))
		 (not more2)))
	   (let ((info1 (type-info t1))
		 (info2 (type-info t2)))
	     (or (eq? info1 info2)		; takes care of OTHER types
		 (and (pair? info1)		; check for same procedure types
		      (pair? info2)
		      (same-type? (car info1) (car info2))
		      (same-type? (cadr info1) (cadr info2))
		      (eq? (caddr info1) (caddr info2))))))))

(define (subtype? t1 t2)		;*** optimize later
  (same-type? t1 (meet-type t1 t2)))

; (mask->type mask/procedure) represents the TOP of the procedure
; subhierarchy.

(define (meet-type t1 t2)
  (if (same-type? t1 t2)
      t1
      (let ((m (bitwise-and (type-mask t1) (type-mask t2))))
	(cond ((> (bitwise-and m mask/two-or-more) 0)
	       (meet-rail t1 t2))
	      ((eq? (type-info t1) (type-info t2))
	       (make-type m #f (type-info t1)))
	      ((> (bitwise-and m mask/other) 0)
	       (let ((i1 (other-type-info t1))
		     (i2 (other-type-info t2)))
		 (if (and i1 i2)
		     (mask->type (bitwise-and m (bitwise-not mask/other)))
		     (make-type m
				#f
				(or i1 i2)))))
	      ((> (bitwise-and m mask/procedure) 0)
	       (meet-procedure m t1 t2))
	      (else
	       (mask->type m))))))

(define (other-type-info t)
  (let ((i (type-info t)))
    (if (pair? i) #f i)))

;(define (p name x) (write `(,name ,x)) (newline) x)

(define (meet-rail t1 t2)
  (let ((t (meet-type (head-type t1)
		      (head-type t2))))
    (if (and (rest-type? t1)
	     (rest-type? t2))
	(make-rest-type t)
	(rail-type t (meet-type (tail-type t1)
				(tail-type t2))))))
	       
; Start with these assumptions:
;
;  . (meet? t1 t2) == (not (bottom-type? (meet-type t1 t2)))
;  . (subtype? t1 t2) == (same-type? t1 (meet-type t1 t2))
;  . (subtype? t1 t2) == (same-type? t2 (join-type t1 t2))
;  . We signal a type error if not (intersect? have want).
;  . We infer the type of a parameter by intersecting the want-types
;    of all definitely-reached points of use.
;
; 1. If both types are nonrestrictive, we have to JOIN both domains
; and codomains (if we are to avoid conjunctive types).
;
; (+ (f 1) (car (f 'a)))    [reconstructing type of f by computing meet of all contexts]
;   => meet (proc (:integer) :number nonr) (proc (:symbol) :pair nonr)
;   => (proc ((join :integer :symbol) (join :number :pair)) nonr), yes?
;
; 2. If both types are restrictive, we need to MEET both domains and
; codomains.
;
; (define (foo) 3),   (export (foo (proc (:value) :value)))
;  Error - disjoint domains.
;
; (define (foo) 'baz),   (export (foo (proc () :number)))
;  Error - disjoint codomains.
;
; 3. If one is restrictive and the other isn't then we still need to
; MEET on both sides.
;
; (with-output-to-file "foo" car)
;   => meet (proc () :any nonr), (proc (:pair) :value restr)
;   => Error - disjoint domains.
;
; (frob (lambda () 'a))  where (define (frob f) (+ (f) 1))
;   => meet (proc () :symbol restr), (proc () :number nonr)
;   => Error - disjoint codomains.
;
; Does export checking look for (intersect? want have), or for
; (subtype? want have) ?  We should be able to narrow something as we
; export it, but not widen it.
;
; (define (foo . x) 3),   (export (foo (proc (value) value)))
;  No problem, since the domain of the first contains the domain of the second.
;
; (define (foo x . y) (+ x 3)),   (export (foo (proc (value) value)))
;  Dubious; the domains intersect but are incomparable.  The meet
;  should be (proc (number) number).
;
; (define (foo x) (numerator x)),   (export (foo (proc (real) integer)))
;  This is dubious, since the stated domain certainly contains values
;  that will be rejected.  (But then, what about divide by zero, or
;  vector indexing?)
;
; (define (foo x) (numerator x)),   (export (foo (proc (integer) integer)))
;  This should definitely be OK.

(define (meet-procedure m t1 t2)
  (let ((dom1 (procedure-type-domain t1))
	(dom2 (procedure-type-domain t2))
	(cod1 (procedure-type-codomain t1))
	(cod2 (procedure-type-codomain t2)))
    (cond ((or (restrictive? t1)
	       (restrictive? t2))
	   (let ((dom (meet-type dom1 dom2))
		 (cod (meet-type cod1 cod2)))
	     (if (or (bottom-type? dom)
		     (and (bottom-type? cod)
			  (not (bottom-type? cod1)) ;uck
			  (not (bottom-type? cod2))))
		 (mask->type (bitwise-and m (bitwise-not mask/procedure)))
		 (make-procedure-type m
				      dom
				      cod
				      #t))))
	  ((and (subtype? dom2 dom1)
		(subtype? cod2 cod1))
	   ;; exists x : dom1 s.t. (f x) : cod1   adds no info
	   (make-procedure-type m dom2 cod2 #f))
	  (else
	   ;; Arbitrary choice.
	   (make-procedure-type m dom1 cod1 #f)))))

; MEET? is the operation used all the time by the compiler.  We want
; getting a yes answer to be as fast as possible.  We could do
;
;   (define (meet? t1 t2) (not (bottom-type? (meet-type t1 t2))))
;
; but that would be too slow.

(define (meet? t1 t2)
  (or (eq? t1 t2)
      (let ((m (bitwise-and (type-mask t1)
			    (type-mask t2))))
	(cond ((= m mask/two-or-more)
	       (and (meet? (head-type t1)
			   (head-type t2))
		    (meet? (tail-type t1)
			   (tail-type t2))))
	      ((= m 0)
	       #f)
	      ((eq? (type-info t1)
		    (type-info t2))
	       #t)
	      ((= m mask/other)
	       (not (and (other-type-info t1)
			 (other-type-info t2))))
	      ((= m mask/procedure)
	       (meet-procedure? t1 t2))
	      (else
	       #t)))))

(define (meet-procedure? t1 t2)
  (if (or (restrictive? t1)
	  (restrictive? t2))
      (and (meet? (procedure-type-domain t1)
		  (procedure-type-domain t2))
	   (meet? (procedure-type-codomain t1)
		  (procedure-type-codomain t2)))
      #t))


; Join

(define (join-type t1 t2)
  (if (same-type? t1 t2)
      t1
      (let ((m (bitwise-ior (type-mask t1)
			    (type-mask t2))))
	(if (> (bitwise-and m mask/two-or-more) 0)
	    (join-rail t1 t2)
	    (let ((info1 (type-info t1))
		  (info2 (type-info t2)))
	      (cond ((equal? info1 info2)
		     (make-type m #f (type-info t1)))
		    ((> (bitwise-and m mask/other) 0)
		     (make-type m #f #f))
		    ((> (bitwise-and m mask/procedure) 0)
		     (join-procedure m t1 t2))
		    (else
		     (error "This shouldn't happen" t1 t2))))))))

(define (join-rail t1 t2)
  (let ((t (join-type (head-type t1) (head-type t2))))
    (if (and (rest-type? t1)
	     (rest-type? t2))
	(make-rest-type t)
	(rail-type t
		   (if (type-more t1)
		       (if (type-more t2)
			   (join-type (tail-type t1)
				      (tail-type t2))
			   (tail-type t1))
		       (tail-type t2))))))
	
; This is pretty gross.

(define (join-procedure m t1 t2)
  (if (procedure-type? t1)
      (if (procedure-type? t2)
	  (let ((dom1 (procedure-type-domain t1))
		(dom2 (procedure-type-domain t2))
		(cod1 (procedure-type-codomain t1))
		(cod2 (procedure-type-codomain t2)))
	    (make-procedure-type m
				 (join-type dom1 dom2) ;Error when outside here
				 (join-type cod1 cod2)
				 (and (restrictive? t1)
				      (restrictive? t2))))
	  (make-type m #f (type-info t1)))
      (make-type m #f (type-info t2))))


; --------------------
; Value types.

; First, the ten indivisible number types.

(define number-hierarchy
  '(:integer :rational :real :complex :number))

(let loop ((names number-hierarchy)
	   (exact bottom-type)
	   (inexact bottom-type))
  (if (null? names)
      (begin (set-type-name! exact ':exact)
	     (set-type-name! inexact ':inexact))
      (let* ((exact (join-type exact (new-atomic-type)))
	     (inexact (join-type inexact (new-atomic-type))))
	(set-type-name! (join-type exact inexact)
			(car names))
	(loop (cdr names)
	      exact
	      inexact))))

(define integer-type  (name->type ':integer))
(define rational-type (name->type ':rational))
(define real-type     (name->type ':real))
(define complex-type  (name->type ':complex))
(define number-type   (name->type ':number))
(define exact-type    (name->type ':exact))
(define inexact-type  (name->type ':inexact))

(define exact-integer-type (meet-type integer-type exact-type))
(set-type-name! exact-integer-type ':exact-integer)

(define inexact-real-type (meet-type real-type inexact-type))
(set-type-name! inexact-real-type ':inexact-real)

; Next, all the others.

(define boolean-type (named-atomic-type ':boolean))
(define pair-type (named-atomic-type ':pair))
(define null-type (named-atomic-type ':null))
(define record-type (named-atomic-type ':record))

(define any-procedure-type (named-atomic-type ':procedure))

; ???
; (define procedure-nonbottom-type (new-atomic-type))
; (define procedure-bottom-type (new-atomic-type))
; (define mask/procedure (meet procedure-nonbottom-type procedure-bottom-type))

; OTHER-VALUE-TYPE is a catchall for all the other ones we don't
; anticipate (for now including string, vector, char, etc.).

(define other-value-type (named-atomic-type ':other))
(define mask/other (type-mask other-value-type))

(define (make-other-type id)
  (let ((t (make-type mask/other #f id)))
    (set-type-name! t id)
    t))

(define char-type	(make-other-type ':char))
(define unspecific-type (make-other-type ':unspecific))
(define string-type	(make-other-type ':string))
(define symbol-type	(make-other-type ':symbol))
(define vector-type	(make-other-type ':vector))
(define escape-type	(make-other-type ':escape))
(define structure-type  (make-other-type ':structure))


; --------------------
; Procedures.

(define mask/procedure (type-mask any-procedure-type))

(define (procedure-type dom cod r?)
  (make-procedure-type mask/procedure dom cod r?))

(define (make-procedure-type m dom cod r?)
  (make-type m
	     #f
	     (if (and (not r?)
		      (same-type? dom value-type)
		      (same-type? cod value-type))
		 #f	;LUB of all procedure types
		 (list dom cod r?))))

(define (procedure-type-domain t)
  (let ((info (type-info t)))
    (if (pair? info)
	(car info)
	any-values-type)))

(define (procedure-type-codomain t)
  (let ((info (type-info t)))
    (if (pair? info)
	(cadr info)
	any-values-type)))

(define (restrictive? t)
  (let ((info (type-info t)))
    (if (pair? info)
	(caddr info)
	#f)))

; --------------------
; Conversion to and from S-expression.

(define (sexp->type x r?)
  (cond ((symbol? x)
	 (name->type x))
	((pair? x)
	 (case (car x)
	   ((some-values)
	    (sexp->values-type (cdr x) #t r?))
	   ((proc procedure-type)
	    (let ((r? (if (or (null? (cdddr x))
			      (eq? (cadddr x) r?))
			  r?
			  (not r?))))
	      (procedure-type (sexp->values-type (cadr x) #t (not r?))
			      (sexp->type (caddr x) r?)
			      r?)))
	   ((meet)
	    (if (null? (cdr x))
		bottom-type
		(let ((l (map (lambda (x) (sexp->type x r?)) (cdr x))))
		  (reduce meet-type (car l) (cdr l)))))
	   ((join)
	    (let ((l (map (lambda (x) (sexp->type x r?)) (cdr x))))
	      (reduce join-type (car l) (cdr l))))
	   ((mask->type)
	    (mask->type (cadr x)))
           ((variable)
            (variable-type (sexp->type (cadr x) r?)))
	   (else (error "unrecognized type" x))))
	(else (error "unrecognized type" x))))

(define (sexp->values-type l req? r?)
  (cond ((null? l)
	 empty-rail-type)
	((eq? (car l) '&rest)
	 (make-rest-type (sexp->type (cadr l) r?)))
	((eq? (car l) '&opt)
	 (sexp->values-type (cdr l) #f r?))
 	((eq? (car l) 'rail-type)
 	 (sexp->values-type (cdr l) req? r?))
	(else
	 (let ((t (sexp->type (car l) r?)))
	   (rail-type (if req? t (make-optional-type t))
		      (sexp->values-type (cdr l)
					 req?
					 r?))))))

; Convert type to S-expression

(define (type->sexp t r?)
  (if (variable-type? t)
      `(variable ,(type->sexp (variable-value-type t) r?))
      (if (> (bitwise-and (type-mask t) mask/&rest) 0)
          (if (same-type? t any-values-type)
              ':values
              `(some-values ,@(rail-type->sexp t r?)))
          (let ((j (disjoin-type t)))
            (cond ((null? j) ':error)
                  ((null? (cdr j))
                   (atomic-type->sexp (car j) r?))
                  (else
                   `(join ,@(map (lambda (t)
                                   (atomic-type->sexp t r?))
                                 j))))))))
	 
(define (atomic-type->sexp t r?)
  (let ((m (type-mask t)))
    (cond ((and (not (type-info t))
		(table-ref mask->name-table m)))
	  ((= m mask/other) 
	   (or (type-info t) ':value))	;not quite
	  ((= m mask/procedure)
	   (let ((r (restrictive? t)))
	     `(proc ,(rail-type->sexp (procedure-type-domain t)
				      (not r))
		    ,(type->sexp (procedure-type-codomain t) r)
		    ,@(if (eq? r r?)
			  '()
			  `(,r)))))
	  ((type-info t)
	   `(ill-formed ,(type-mask t) ,(type-info t)))
	  ((subtype? t exact-type)
	   `(meet :exact
		  ,(type->sexp (mask->type (let ((m (type-mask t)))
					     (bitwise-ior m (arithmetic-shift m 1))))
			       #t)))
	  ((subtype? t inexact-type)
	   `(meet :inexact
		  ,(type->sexp (mask->type (let ((m (type-mask t)))
					     (bitwise-ior m (arithmetic-shift m -1))))
			       #t)))
	  ;; ((meet? t number-type) ...)
	  (else
	   `(mask->type ,(type-mask t))))))

(define (rail-type->sexp t r?)
  (let recur ((t t) (prev-req? #t) (r? r?))
    (cond ((empty-rail-type? t) '())
	  ((rest-type? t)
	   `(&rest ,(type->sexp (head-type-really t) r?)))
	  ((optional-type? t)
	   (let ((tail (cons (type->sexp (head-type-really t) r?)
			     (recur (tail-type t) #f r?))))
	     (if prev-req?
		 `(&opt ,@tail)
		 tail)))
	  (else
	   (cons (type->sexp (head-type t) r?)
		 (recur (tail-type t) #t r?))))))

; Decompose a type into components

(define (disjoin-type t)
  (cond ((bottom-type? t) '())
	((and (not (type-info t))
	      (table-ref mask->name-table (type-mask t)))
	 (list t))
	((meet? t other-value-type)
	 (cons (meet-type t other-value-type)
	       (disjoin-rest t mask/other)))
	((meet? t any-procedure-type)
	 (cons (meet-type t any-procedure-type)
	       (disjoin-rest t mask/procedure)))
	((meet? t number-type)
	 (cons (meet-type t number-type)
	       (disjoin-rest t mask/number)))
	(else
	 (do ((i 1 (arithmetic-shift i 1)))
	     ((> (bitwise-and (type-mask t) i) 0)
	      (cons (mask->type i)
		    (disjoin-rest t i)))))))

(define (disjoin-rest t mask)
  (disjoin-type (mask->type (bitwise-and (type-mask t)
					 (bitwise-not mask)))))

(define mask/number (type-mask number-type))

; --------------------
; obsolescent?  see lambda and values reconstructors in recon.scm

(define (make-some-values-type types)
  (if (null? types)
      empty-rail-type
      (rail-type (car types) (make-some-values-type (cdr types)))))

(define-syntax proc
  (syntax-rules ()
    ((proc (?type ...) ?cod)
     (procedure-type (some-values ?type ...) ?cod #t))
    ((proc (?type ...) ?cod ?r)
     (procedure-type (some-values ?type ...) ?cod ?r))))

(define-syntax some-values
  (syntax-rules (&opt &rest)
    ((some-values) empty-rail-type)
    ((some-values &opt) empty-rail-type)
    ((some-values ?t) ?t)
    ((some-values &rest ?t) (make-rest-type ?t))
    ((some-values &opt &rest ?t) (make-rest-type ?t))
    ((some-values &opt ?t1 . ?ts)
     (rail-type (make-optional-type ?t1)
                (some-values &opt . ?ts)))
    ((some-values ?t1 . ?ts)
     (rail-type ?t1 (some-values . ?ts)))))

(define (procedure-type? t)
  (= (type-mask t) mask/procedure))

(define (fixed-arity-procedure-type? t)
  (and (procedure-type? t)
       (let loop ((d (procedure-type-domain t)))
	 (cond ((empty-rail-type? d) #t)
	       ((optional-type? d) #f)
	       (else (loop (tail-type d)))))))

(define (procedure-type-arity t)
  (do ((d (procedure-type-domain t) (tail-type d))
       (i 0 (+ i 1)))
      ((empty-rail-type? d) i)
    (if (optional-type? d)
	(error "this shouldn't happen" t d))))

(define (procedure-type-argument-types t)
  (let recur ((d (procedure-type-domain t)))
    (cond ((empty-rail-type? d) '())
	  ((optional-type? d)
	   (call-error "lossage" procedure-type-argument-types t))
	  (else
	   (cons (head-type d)
		 (recur (tail-type d)))))))

;----------------
; Odd types - variable types and the undeclared type
;
; These were elsewhere (syntax.scm) and should be here.  If I could understand
; the above code I could make these be `real' types.

(define (variable-type type)
  (list 'variable type))

(define (variable-type? type)
  (and (pair? type) (eq? (car type) 'variable)))

(define variable-value-type cadr)

; Usual type for Scheme variables.

(define usual-variable-type (variable-type value-type))

; cf. EXPORT macro

(define undeclared-type ':undeclared)

;----------------
; Used in two places:
; 1. GET-LOCATION checks to see if the context of use (either variable
;    reference or assignment) is compatible with the declared type.
; 2. CHECK-STRUCTURE checks to see if the reconstructed type is compatible
;    with any type declared in the interface.

(define (compatible-types? have-type want-type)
  (if (variable-type? want-type)
      (and (variable-type? have-type)
	   (same-type? (variable-value-type have-type)
		       (variable-value-type want-type)))
      (meet? (if (variable-type? have-type)
		 (variable-value-type have-type)
		 have-type)
	     want-type)))


