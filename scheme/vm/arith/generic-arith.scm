; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Generic arithmetic.

; The different kinds of numbers.

(define-enumeration numbers
  (fixnum
   bignum
   rational
   float
   complex
   not-a-number))

; Mapping numbers to their representation.

(define stob-numbers
  (make-vector stob-count (enum numbers not-a-number)))

; For now all we have are bignums (and fixnums, of course).
(vector-set! stob-numbers (enum stob bignum) (enum numbers bignum))

(define (number->representation x)
  (cond ((fixnum? x)
	 (enum numbers fixnum))
	((stob? x)
	 (vector-ref stob-numbers (header-type (stob-header x))))
	(else
	 (enum numbers not-a-number))))

;----------------
; Tables for unary and binary operations.  All entries initially return DEFAULT.

(define (make-unary-table default)
  (make-vector numbers-count
	       (lambda (x)
		 default)))
	
; (unary-table-set! <table> <type> <value>)
; (unary-table-set! <table> (<type> ...) <value>)

(define-syntax unary-table-set!
  (syntax-rules ()
    ((unary-table-set! ?table (?kind ...) ?value)
     (real-unary-table-set! ?table (list (enum numbers ?kind) ...) ?value))
    ((unary-table-set! ?table ?kind ?value)
     (real-unary-table-set! ?table (list (enum numbers ?kind)) ?value))))

(define (real-unary-table-set! table kinds value)
  (for-each (lambda (kind)
	      (vector-set! table kind value))
	    kinds))

(define (unary-dispatch table x)
  ((vector-ref table
	       (number->representation x))
     x))

(define (make-binary-table default)
  (make-vector (* numbers-count numbers-count)
	       (lambda (x y)
		 default)))

; Same as for unary tables, except that we have two indexes or lists of indexes.

(define-syntax binary-table-set!
  (syntax-rules ()
    ((binary-table-set! ?table (?kind0 ...) (?kind1 ...) ?value)
     (real-binary-table-set! ?table
			     (list (enum numbers ?kind0) ...)
			     (list (enum numbers ?kind1) ...)
			     ?value))
    ((binary-table-set! ?table (?kind0 ...) ?kind1 ?value)
     (real-binary-table-set! ?table
			     (list (enum numbers ?kind0) ...)
			     (list (enum numbers ?kind1))
			     ?value))
    ((binary-table-set! ?table ?kind0 (?kind1 ...) ?value)
     (real-binary-table-set! ?table
			     (list (enum numbers ?kind0))
			     (list (enum numbers ?kind1) ...)
			     ?value))
    ((binary-table-set! ?table ?kind0 ?kind1 ?value)
     (real-binary-table-set! ?table
			     (list (enum numbers ?kind0))
			     (list (enum numbers ?kind1))
			     ?value))))

(define (real-binary-table-set! table kinds0 kinds1 value)
  (for-each (lambda (kind0)
	      (for-each (lambda (kind1)
			  (vector-set! table
				       (+ (* kind0 numbers-count)
					  kind1)
				       value))
			kinds1))
	    kinds0))

; Does this need to be changed to get a computed goto?

(define (binary-dispatch table x y)
  ((vector-ref table
	       (+ (* (number->representation x)
		     numbers-count)
		  (number->representation y)))
     x
     y))

(define (binary-lose x y)
  unspecific-value)

;----------------
; The actual opcodes

; Predicates

(define-primitive number? (any->)
  (lambda (x)
    (not (= (number->representation x)
	    (enum numbers not-a-number))))
  return-boolean)

(define-primitive integer? (any->)
  (lambda (x)
    (let ((type (number->representation x)))
      (or (= type (enum numbers fixnum))
	  (= type (enum numbers bignum)))))
  return-boolean)

(define-primitive rational? (any->)
  (lambda (x)
    (let ((type (number->representation x)))
      (or (= type (enum numbers fixnum))
	  (= type (enum numbers bignum))
	  (= type (enum numbers rational)))))
  return-boolean)

(define-primitive real? (any->)
  (lambda (x)
    (let ((type (number->representation x)))
      (not (or (= type (enum numbers complex))
	       (= type (enum numbers not-a-number))))))
  return-boolean)

(define-primitive complex? (any->)
  (lambda (x)
    (not (= (number->representation x)
	    (enum numbers not-a-number))))
  return-boolean)

(define-primitive exact? (any->)
  (lambda (x)
    (enum-case number (number->representation x)
	       ((float)
		(goto return-boolean #f))
	       ((complex)
		(goto return-boolean (not (float? (complex-real-part x)))))
	       ((not-a-number)
		(raise-exception wrong-type-argument 0 x))
	       (else
		(goto return-boolean #t)))))

;----------------
; Arithmetic

(define-syntax define-binary-primitive
  (syntax-rules ()
    ((define-binary-primitive id table careful integer)
     (define table (make-binary-table binary-lose))
     (define-primitive id (any-> any->)
       (lambda (x y)
	 (if (and (fixnum? x)
		  (fixnum? y))
	     (goto careful
		   x
		   y
		   return
		   (lambda (x y)
		     (goto return (integer x y))))
	     (let ((r (binary-dispatch table x y)))
	       (if (vm-eq? r unspecific-value)
		   (raise-exception wrong-type-argument 0 x y)
		   (goto return r))))))
     (binary-table-set! table (fixnum bignum) (fixnum bignum) integer))))

(define-binary-primitive + add-table add-carefully integer-add)
(define-binary-primitive - subtract-table subtract-carefully integer-subtract)
(define-binary-primitive * multiply-table multiply-carefully integer-multiply)
(define-binary-primitive quotient quotient-table quotient-carefully integer-quotient)
(define-binary-primitive remainder remainder-table remainder-carefully integer-remainder)
(define-binary-primitive arithmetic-shift shift-table shift-carefully integer-shift)

; Hm.  There is no integer-divide (obviously)

(define-binary-primitive / divide-table divide-carefully integer-)












****************************************************************

How to structure all this?  It would be nice if the interpreter could be
broken into several modules.  The registers and define-primitive would
need to be separated out.

;----------------
; Tower predicates.
; These need to be changed.

(define-unary-opcode-extension integer?       &integer?  #f)
(define-unary-opcode-extension rational?      &rational? #f)
(define-unary-opcode-extension real?          &real?     #f)
(define-unary-opcode-extension complex?       &complex?  #f)
(define-unary-opcode-extension number?        &number?   #f)
(define-unary-opcode-extension exact?         &exact?    #f)

(let ((true (lambda (x) #t)))
  (unary-table-set! &integer?  (fixnum bignum)                        true)
  (unary-table-set! &rational? (fixnum bignum rational)               true)
  (unary-table-set! &real?     (fixnum bignum rational float)         true)
  (unary-table-set! &complex?  (fixnum bignum rational float complex) true)
  (unary-table-set! &number?   (fixnum bignum rational float complex) true)
  (unary-table-set! &exact?    (fixnum bignum rational)               true))

; The two parts of a complex number must have the same exactness.

(unary-table-set! &exact? (complex)
		  (lambda (z)
		    (real-part z)))

;----------------
; Imaginary operations.

(define-unary-opcode-extension real-part      &real-part (lambda (x) x))
(define-unary-opcode-extension imag-part      &imag-part (lambda (x) 0))

(unary-table-set! &real-part (complex not-a-number)
		  (lambda (x) unimplemented))

(unary-table-set! &imag-part (complex not-a-number)
		  (lambda (x) unimplemented))

;----------------
; Fractions

(define-unary-opcode-extension floor          &floor)
(define-unary-opcode-extension numerator      &numerator)
(define-unary-opcode-extension denominator    &denominator)

(define (identity x) x)

(unary-table-set! &floor       (fixnum bignum) identity)
(unary-table-set! &numerator   (fixnum bignum) identity)
(unary-table-set! &denominator (fixnum bignum) (lambda (x) 1))

;----------------
; Square root.

(define-unary-opcode-extension sqrt &sqrt)

; The bignum code could whack this.
; The VM doesn't do sqrt for positive fixnums.  I wonder why?

; For negative N, we lose if MAKE-RECTANGULAR loses.

(unary-table-set! &sqrt (fixnum bignum)
  (lambda (n)
    (if (>= n 0)
	(non-negative-integer-sqrt n)	;Dubious (JAR)
	(let ((s (non-negative-integer-sqrt (- n))))
	  (if (eq? s unimplemented)
	      s
	      (binary-dispatch &make-rectangular
			       0
			       s))))))

; Courtesy of Mr. Newton.

(define (non-negative-integer-sqrt n)
  (if (<= n 1)    ; for both 0 and 1
      n
      (let loop ((m (quotient n 2)))
	(let ((m1 (quotient n m)))
	  (cond ((< m1 m)
		 (loop (quotient (+ m m1) 2)))
		((= n (* m m))
		 m)
		(else
		 unimplemented))))))

;----------------
; Make sure this has very low priority, so that it's only tried as a
; last resort.
;
; In fact, I'll comment it out completely. -RK

;(define-method &/ (m n)
;  (if (and (integer? m) (integer? n))
;      (if (= 0 (remainder m n))
;          (quotient m n)
;          (let ((z (abs (quotient n 2))))
;            (set-exactness (quotient (if (< m 0)
;                                         (- m z)
;                                         (+ m z))
;                                     n)
;                           #f)))
;      (next-method)))

;----------------
; The rest have no useful defaults.

(define-unary-opcode-extension exact->inexact &exact->inexact)
(define-unary-opcode-extension inexact->exact &inexact->exact)

(define-binary-opcode-extension +              &+)
(define-binary-opcode-extension -              &-)
(define-binary-opcode-extension *              &*)
(define-binary-opcode-extension /              &/)
(define-binary-opcode-extension =              &=)
(define-binary-opcode-extension <              &<)
(define-binary-opcode-extension quotient       &quotient)
(define-binary-opcode-extension remainder      &remainder)
  
(define-binary-opcode-extension make-rectangular &make-rectangular)

(define-unary-opcode-extension exp  &exp)
(define-unary-opcode-extension log  &log)
(define-unary-opcode-extension sin  &sin)
(define-unary-opcode-extension cos  &cos)
(define-unary-opcode-extension tan  &tan)
(define-unary-opcode-extension asin &asin)
(define-unary-opcode-extension acos &acos)
(define-unary-opcode-extension atan &atan)

; >, <=, and >= are all extended using the table for <.

(extend-opcode! (enum op >)
		(lambda (lose)
		  (lambda (reason arg0 arg1)
		    (let ((res (binary-dispatch &< arg1 arg0)))
		      (if (eq? res unimplemented)
			  (lose reason arg0 arg1)
			  res)))))
(extend-opcode! (enum op <=)
		(lambda (lose)
		  (lambda (reason arg0 arg1)
		    (let ((res (binary-dispatch &< arg1 arg0)))
		      (if (eq? res unimplemented)
			  (lose reason arg0 arg1)
			  (not res))))))
(extend-opcode! (enum op >=)
		(lambda (lose)
		  (lambda (reason arg0 arg1)
		    (let ((res (binary-dispatch &< arg0 arg1)))
		      (if (eq? res unimplemented)
			  (lose reason arg0 arg1)
			  (not res))))))

