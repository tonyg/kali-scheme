; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

;----------------

(define (add-space size0 size1)
  (bignum-digits->size (+ (max size0 size1) 1)))

; These take care of the extra space needed for fixnum arguments (they need
; to be converted to bignums).  SPACE-PROC takes the length of the two bignums
; and returns the space needed for the computation and results.

(define (binary-space-proc space-proc)
  (lambda (x y)
    (receive (length0 extra0)
	(integer-bignum-digits x)
      (receive (length1 extra1)
	  (integer-bignum-digits y)
	(+ (space-proc length0 length1)
	   extra0
	   extra1)))))

(define (unary-space-proc space-proc)
  (lambda (x)
    (receive (length extra)
	(integer-bignum-digits x)
      (+ (space-proc length)
	 extra))))

; While checking for space, which may cause a GC, we have to save the two
; arguments where they will be traced.

(define (binary-bignum-op space-proc proc)
  (let ((space-proc (binary-space-proc space-proc)))
    (lambda (x y)
      (let ((needed (space-proc x y)))
	(save-temp0! x)
	(save-temp1! y)
	(ensure-bignum-space! needed)
	(let ((x (integer->external-bignum (recover-temp0!)))
	      (y (integer->external-bignum (recover-temp1!))))
	  (external-bignum->integer (proc x y)))))))

; Same again for unary procedures.

(define (unary-bignum-op space-proc proc)
  (let ((space-proc (unary-space-proc space-proc)))
    (lambda (x)
      (let ((needed (space-proc x)))
	(save-temp0! x)
	(ensure-bignum-space! needed)
	(external-bignum->integer 
	  (proc (integer->external-bignum (recover-temp0!))))))))


(define bignum-add (binary-bignum-op add-space external-bignum-add))
(define bignum-subtract (binary-bignum-op add-space external-bignum-subtract))

(define bignum-multiply (binary-bignum-op (lambda (size0 size1)
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

(define (bignum-divide x y)
  (let ((needed ((binary-space-proc divide-space) x y)))
    (save-temp0! x)
    (save-temp1! y)
    (ensure-bignum-space! needed)
    (let ((x (integer->external-bignum (recover-temp0!)))
	  (y (integer->external-bignum (recover-temp1!))))
      (receive (div-by-zero? quot rem)
	  (external-bignum-divide x y)
	(if div-by-zero?
	    (values #t
		    (enter-fixnum 0)   ;just to have a descriptor
		    (enter-fixnum 0))
	    (values #f
		    (external-bignum->integer quot)
		    (external-bignum->integer rem)))))))

(define (shift-space x n)
  (receive (x-size extra)
	   (integer-bignum-digits x)
    (+ extra
       (if (>= n 0)
	   (+ (bignum-digits->size x-size) 
	      (bignum-digits->size (quotient n bignum-digit-bits))
	      1)
	   (+ (* 2 (not-space x-size))
	      (+ (bignum-digits->size x-size) 1))))))

(define (bignum-arithmetic-shift x y)
  (let* ((y (extract-fixnum y))
	 (needed (shift-space x y)))
    (save-temp0! x)
    (ensure-bignum-space! needed)
    (let ((x (integer->external-bignum (recover-temp0!))))
      (external-bignum->integer (external-bignum-arithmetic-shift x y)))))

;;; bitwise-not x == (- -1 x)
;;; ignore that -1 is cached...
(define (not-space size0)
  (add-space size0 fixnum-as-bignum-digits))

(define bignum-bitwise-not 
  (unary-bignum-op not-space external-bignum-bitwise-not))

(define bignum-bit-count
  (let ((space-proc (unary-space-proc not-space)))
    (lambda (x)
      (let ((needed (space-proc x)))
	(save-temp0! x)
	(ensure-bignum-space! needed)
	(enter-fixnum
	  (external-bignum-bit-count
	    (integer->external-bignum (recover-temp0!))))))))

(define (bitwise-space size0 size1)
  (bignum-digits->size (+ (max size0 size1) 1)))

(define bignum-bitwise-and 
  (binary-bignum-op bitwise-space external-bignum-bitwise-and))
(define bignum-bitwise-ior
  (binary-bignum-op bitwise-space external-bignum-bitwise-ior))
(define bignum-bitwise-xor 
  (binary-bignum-op bitwise-space external-bignum-bitwise-xor))

; These are not applied to fixnums.

(define (bignum= x y)
  (external-bignum-equal? (extract-bignum x)
			  (extract-bignum y)))

(define (bignum< x y)
  (= -1 (external-bignum-compare (extract-bignum x)
				 (extract-bignum y))))

(define bignum-abs (unary-bignum-op 
		    (lambda (size) size)
		    (lambda (x)
		      (if (= (external-bignum-test x)
			     -1)
			  (external-bignum-negate x)
			  x))))

(define (bignum-positive? x)
  (= (external-bignum-test (extract-bignum x)) 1))

(define (bignum-nonnegative? x)
  (not (= (external-bignum-test (extract-bignum x)) -1)))

;----------------
; Return the number of bignum digits in an integer.  For fixnums this is a
; fixed amount.  The second return value is the amount of space needed to
; convert the argument into a bignum.

(define (integer-bignum-digits x)
  (if (fixnum? x)
      (values fixnum-as-bignum-digits
	      fixnum-as-bignum-size)
      (values (bignum-digits x)
	      0)))

; Converting back and forth between Scheme 48 integers and external bignums.

(define (integer->external-bignum desc)
  (if (fixnum? desc)
      (long->external-bignum (extract-fixnum desc))
      (extract-bignum desc)))

(define (long->external-bignum x)
  (external-bignum-from-long x))

; Converting between longs and bignums

(define (long->bignum x key)
  (set-bignum-preallocation-key! key)
  (enter-bignum (long->external-bignum x)))

(define (external-bignum->integer external-bignum)
  (if (external-bignum-fits-in-word? external-bignum
				     bits-per-fixnum
				     #t)
      (enter-fixnum (external-bignum->long external-bignum))
      (enter-bignum external-bignum)))
