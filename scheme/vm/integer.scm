; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Operations on integers.

; This expects its arguments to be integers.

(define (integer-op fixnum-op bignum-op)
  (if (and (fixnum? x)
	   (fixnum? y))
      (fixnum-op x y (lambda (r) r) bignum-op)
      (bignum-op x y)))
  
(define integer-add       (integer-op add-carefully       bignum-add))
(define integer-subtract  (integer-op subtract-carefully  bignum-subtract))
(define integer-multiply  (integer-op multiply-carefully  bignum-multiply))
(define integer-quotient  (integer-op quotient-carefully  bignum-quotient))
(define integer-remainder (integer-op remainder-carefully bignum-remainder))

(define (integer-abs x)
  (if (fixnum? x)
      (abs-carefully x (lambda (r) r) bignum-abs)
      (bignum-abs x)))

; What about shifts and bitwise operations?

; Fixnums and bignums are disjoint.

(define (integer= x y)
  (cond ((and (fixnum? x)
	      (fixnum? y))
	 (fixnum= x y))
	((and (bignum? x)
	      (bignum? y))
	 (bignum= x y))
	(else
	 #f)))

; Positive bignums are greater than all fixnums, negative bignums are less
; than all fixnums.

(define (integer< x y)
  (cond ((fixnum? x)
	 (if (fixnum? y)
	     (fixnum< x y)
	     (bignum-positive? y)))
	((fixnum? y)
	 (not (bignum-positive? y)))
	(else
	 (bignum< x y))))

(define (integer<= x y)
  (not (integer< y x)))

(define (integer> x y)
  (integer< y x))

(define (integer>= x y)
  (not (integer< x y)))


	 
		
