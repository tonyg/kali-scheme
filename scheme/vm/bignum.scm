; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; These need to operate on both bignums and fixnums.
;bignum-add
;bignum-subtract
;bignum-multiply
;bignum-quotient
;bignum-remainder
;bignum-abs
;
; These only see bignums.
;bignum=
;bignum<

; This only sees fixnums.
;fixnum->bignum

; fixnum-as-bignum-length - the maximum bignum digits required to hold a fixnum
; From struct.scm:
;   bignum-length - usual length operator, gives number of descriptors
;   bignum-size - space to hold a bignum of N descriptors
; Defined here:
;   bignum-digits - number of digits in a bignum
;   bignum-digits->size - space to hold a bignum of N digits
;   fixnum-as-bignum-digits - number of digits to hold a fixnum

; The first word in a bignum is used as a header by the C code.

(define (bignum-digits bignum)
  (- (bignum-length bignum) 1))

(define (bignum-digits->size n)
  (bignum-size (+ n 1)))

;----------------

(define (add-space size0 size1)
  (bignum-digits->size (+ (max size0 size1) 1)))

(define bignum-add      (binary-bignum-op add-space external-bignum-add))
(define bignum-subtract (binary-bignum-op add-space external-bignum-subtract))

(define bignum-multiply
  (binary-bignum-op (lambda (size0 size1)
		      (bignum-digits->size (+ size0 size1)))
		    external-bignum-multiply))

; Three bignums whose total length is twice the numerator plus two.

(define (divide-space numerator-size denominator-size)
  (+ (* 2 (bignum-digits->size numerator-size))
     (bignum-digits->size 2)))

(define bignum-quotient
  (binary-bignum-op divide-space external-bignum-quotient))

(define bignum-remainder
  (binary-bignum-op divide-space external-bignum-remainder))

; These are not applied to fixnums.

(define (bignum= x y)
  (external-bignum-equal? (extract-bignum x)
			  (extract-bignum y)))

(define (bignum< x y)
  (= -1 (external-bignum-compare (extract-bignum x)
				 (extract-bignum y))))

(define (bignum-abs x)
  (cond ((bignum? x)
	 (if (= (external-bignum-test (extract-bignum x))
		-1)
	     (external-bignum-negate (extract-bignum x))
	     x))
	((fx= x least-bignum)
	 (long->external-bignum (abs (extract-fixnum x)) key))
	(else
	 (enter-fixnum (abs (extract-fixnum x))))))

(define bignum-abs
  (unary-bignum-op (lambda (size) size)
		   (lambda (x)
		     (if (= (external-bignum-test x)
			    -1)
			 (external-bignum-negate x)
			 x))))

;----------------

; While checking for space, which may cause a GC, we have to save the two
; arguments where they will be traced.

(define (binary-bignum-op space-proc proc)
  (let ((space-proc (binary-space-proc space-proc)))
    (lambda (x y)
      (let ((needed (space-proc x y)))
	(receive (key x y)
	    (ensure-space-saving-temps needed x y)
	  (external-bignum->integer (proc (integer->external-bignum x)
					  (integer->external-bignum y))))))))

; Same again for unary procedures.

(define (unary-bignum-op space-proc proc)
  (let ((space-proc (unary-space-proc space-proc)))
    (lambda (x)
      (let ((needed (space-proc x)))
	(receive (key x)
	    (ensure-space-saving-temp needed x)
	  (external-bignum->integer (proc (integer->external-bignum x))))))))

; These take care of the extra space needed for fixnum arguments (they need
; to be converted to bignums).  SPACE-PROC takes the length of the two bignums
; and returns the space needed for the computation and results.

(define (binary-space-proc space-proc)
  (lambda (x y)
    (receive (length0 extra0)
	(integer-bignum-digits x)
      (receive (length1 extra1)
	  (integer-bignum-digits x)
	(+ (space-proc length0 length1)
	   extra0
	   extra1)))))

(define (unary-space-proc space-proc)
  (lambda (x)
    (receive (length extra)
	(integer-bignum-digits x)
      (+ (space-proc length)
	 extra))))

; Return the number of bignum digits in an integer.  For fixnums this is a
; fixed amount.  The second return value is the amount of space needed to
; convert the argument into a bignum.

(define (integer-bignum-digits x)
  (if (fixnum? x)
      (values fixnum-as-bignum-digits
	      (bignum-digits->size fixnum-as-bignum-digits))
      (values (bignum-digits x)
	      0)))

; Converting back and forth between Scheme 48 integers and external bignums.

(define (integer->external-bignum x)
  (if (fixnum? x)
      (long->external-bignum (extract-fixnum x))
      (extract-bignum x)))

(define (external-bignum->integer external-bignum)
  (if (external-bignum-fits-in-word? external-bignum 30 #t)
      (enter-fixnum (external-bignum->long external-bignum))
      (enter-bignum external-bignum)))
