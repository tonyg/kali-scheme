; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


; Inferencer needs to recognize predicates as such.
; Also accessors if we want to get pattern matching working properly.
; So we make a (predicate <record>) type.
;
; The inferencer needs to come up with true and false assertions for
; each expression.  Assertion is either bottom or a list of
; (<predicate> <exp> <#t-or-#f>) indicating predicates that are true
; or false for that value of the expression.
;
; No.  For each expression we know its type, so we know exactly what
; variants are possible.  All we need to keep track of is (<exp> <variants>).
; The variants can be encoded as a bitset.

; #t -> ()
;       #f
; #f -> #f
;       ()
; (<predicate> <exp>) -> ((<predicate> <exp> #t))
;                        ((<predicate> <exp> #f))
; (IF <test> <cons> <alt>) -> (or (and <test>t <cons>t)
;                                 (and <test>f <alt>t))
;                             (or (and <test>t <cons>f)
;                                 (and <test>f <alt>f))
;      (and #f S) -> #f
;      (and S1 S2) -> (union S1 S2)
;      (and ((e set1)) ((e set2))) -> #f if set3 = 0
;                                     ((e set3)) otherwise
;                                     where set3 = (bitwise-and set1 set2)
;      (or S1 S2) -> (intersection S1 S2)
;      (or ((e set1)) ((e set2))) -> ((e (bitwise-ior set1 set2)))
;
; (begin ... <last>) -> <last>t
;                       <last>f
; similarly for LETREC etc.
;
; NOT won't be inlined yet, so it will have to be known to the inferencer.
; The usual OR macro won't work either, because the LET won't be understood.
; That too will have to be primitive, and thus primitive in the interpreter
; as well.  AND should work find as-is.
;
; NOT and OR can be ignored for the moment.
;
; (IF <test> <cons> <alt>) asserts <test>t values while checking <cons>
; and <test>f while checking <alt>.
;
; There are so few rules that there is no need to have two separate
; sets of inference rules.  Just have an entry point that drops the
; assertions for those places that don't care.
;
; As a first cut the expressions in assertions can only be variables.
; Also, since the assertions are type assertions, an assertion list can
; have only one true assertion for any given expression.

(define-record-type assertion
  (value    ; variable (or, later, slot) whose value is being asserted
   type     ; the union type we are making assertions about
   variants ; bitset of allowable variants
   ; slots  ; for assertions about particular slots, should we ever get that far
   )
  ())

(define make-assertion assertion-maker)

(define (copy-assertion assert new-variants)
  (assertion-maker (assertion-value assert)
		   (assertion-type assert)
		   new-variants))

; To make merging simpler, assertion lists are sorted by variable-id.

; Union of two assertion lists.

(define (assertion-and l1 l2)
  (if (and l1 l2)
      (real-assertion-and l1 l2)
      #f))

(define (real-assertion-and l1 l2)
  (let loop ((l1 l1) (l2 l2) (res '()))
    (cond ((null? l1)
	   (append (reverse res) l2))
	  ((null? l2)
	   (append (reverse res) l1))
	  (else
	   (let ((i1 (variable-id (assertion-value (car l1))))
		 (i2 (variable-id (assertion-value (car l2)))))
	     (cond ((< i1 i2)
		    (loop (cdr l1) l2 (cons (car l1) res)))
		   ((< i2 i1)
		    (loop l1 (cdr l2) (cons (car l2) res)))
		   (else
		    (let ((variants (set-and (assertion-variants (car l1))
					     (assertion-variants (car l2)))))
		      (if (null-set? variants)
			  #f
			  (loop (cdr l1)
				(cdr l2)
				(cons (copy-assertion (car l1) variants)
				      res)))))))))))

; Intersection of two assertion lists.

(define (assertion-or l1 l2)
  (if l1
      (if l2
	  (real-assertion-or l1 l2)
	  l1)
      #f))

(define (real-assertion-or l1 l2)
  (let loop ((l1 l1) (l2 l2) (res '()))
    (if (or (null? l1)
	    (null? l2))
	(reverse res)
	(let ((i1 (variable-id (assertion-value (car l1))))
	      (i2 (variable-id (assertion-value (car l2)))))
	  (cond ((< i1 i2)
		 (loop (cdr l1) l2 res))
		((< i2 i1)
		 (loop l1 (cdr l2) res))
		(else
		 (loop (cdr l1)
		       (cdr l2)
		       (let ((variants (set-ior (assertion-variants (car l1))
						(assertion-variants (car l2)))))
			 (if (set-eq? variants
				      (full-variant-set (assertion-type (car l1))))
			     (cons (copy-assertion (car l1) variants)
				   res)
			     res)))))))))

;----------------
; Use shallow binding to install the types provided by ASSERTIONS while
; THUNK is evaluated.

(define (with-assertions assertions thunk)
  (let ((old-types (map install-assertion! assertions)))
    (thunk)
    (for-each (lambda (assert type)
		(set-variable-type! (assertion-value assert) type))
	      assertions
	      old-types)))

(define (install-assertion! assert)
  (let ((type (variable-type (assertion-value assert))))
    (set-variable-type! (assertion-value assert)
			(get-union-subtype (assertion-type assert)
					   (assertion-variants assert)))
    type))
