; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Integer-only primitive operations

; These predicates are used to characterize the numeric representations that
; are implemented in the VM.

(define (unary-lose x)
  (raise-exception wrong-type-argument 0 x))

(define (binary-lose x y)
  (raise-exception wrong-type-argument 0 x y))

; They're all numbers, even if we can't handle them.

(define-primitive number? (any->)
  (lambda (x)
    (or (fixnum? x)
	(bignum? x)
	(ratnum? x)
	(double? x)
	(extended-number? x)))
  return-boolean)
  
(define (vm-integer? n)
  (cond ((or (fixnum? n)
	     (bignum? n))
	 (goto return-boolean #t))
	((extended-number? n)
	 (unary-lose n))
	(else
	 (goto return-boolean #f))))

; These assume that ratnums and doubles aren't being used.

(define-primitive integer?  (any->) vm-integer?)
(define-primitive rational? (any->) vm-integer?)
(define-primitive real?     (any->) vm-integer?)
(define-primitive complex?  (any->) vm-integer?)

;----------------
; A macro for defining primitives that only operate on fixnums.

(define-syntax define-integer-only
  (syntax-rules ()
    ((define-integer-only (opcode arg) value)
     (define-primitive opcode (vm-integer->)
       (lambda (arg)
	 (goto return value))))
    ((define-integer-only (opcode arg0 arg1) value)
     (define-primitive opcode (vm-integer-> vm-integer->)
       (lambda (arg0 arg1)
	 (goto return value))))))

; These primitives have a simple answer in the case of fixnums; for all others
; they punt to the run-time system.

(define-integer-only (exact?      n) true)
(define-integer-only (real-part   n) n)
(define-integer-only (imag-part   n) (enter-fixnum 0))
(define-integer-only (floor       n) n)
(define-integer-only (numerator   n) n)
(define-integer-only (denominator n) (enter-fixnum 1))

(define-primitive angle (vm-integer->)
  (lambda (n)
    (if (if (fixnum? n)
	    (fx>= n (enter-fixnum 0))
	    (bignum-nonnegative? n))
	(goto return (enter-fixnum 0))
	(unary-lose n))))

(define-primitive magnitude (vm-integer->) integer-abs)

; These all just raise an exception and let the run-time system do the work.

(define-syntax define-punter
  (syntax-rules ()
    ((define-punter opcode)
     (define-primitive opcode (any->) unary-lose))))

(define-punter exact->inexact)
(define-punter inexact->exact)
(define-punter exp)
(define-punter log)
(define-punter sin)
(define-punter cos)
(define-punter tan)
(define-punter asin)
(define-punter acos)
(define-punter sqrt)

(define-syntax define-punter2
  (syntax-rules ()
    ((define-punter2 opcode)
     (define-primitive opcode (any-> any->) binary-lose))))

(define-punter2 atan)
(define-punter2 make-polar)
(define-punter2 make-rectangular)

(define-syntax define-binop
  (syntax-rules ()
    ((define-binop opcode careful-op integer-op)
     (define-primitive opcode (any-> any->)
       (lambda (x y)
	 (cond ((and (fixnum? x)
		     (fixnum? y))
		(goto careful-op
		      x
		      y
		      return
		      (lambda (x y)
			(goto return (integer-op x y)))))
	       ((and (vm-integer? x)
		     (vm-integer? y))
		(goto return (integer-op x y)))
	       (else
		(binary-lose x y))))))))

(define-binop + add-carefully integer-add)
(define-binop - subtract-carefully integer-subtract)
(define-binop * multiply-carefully integer-multiply)
(define-binop / divide-carefully integer-divide)
(define-binop quotient         quotient-carefully integer-quotient)
(define-binop remainder        remainder-carefully integer-remainder)
;(define-binop arithmetic-shift shift-carefully)

(define-syntax define-comparison
  (syntax-rules ()
    ((define-comparison opcode fixnum-op integer-op)
     (define-primitive opcode (any-> any->)
       (lambda (x y)
	 (cond ((and (fixnum? x)
		     (fixnum? y))
		(goto careful-op
		      x
		      y
		      return
		      (lambda (x y)
			(goto return (integer-op x y)))))
	       ((and (vm-integer? x)
		     (vm-integer? y))
		(goto return (integer-op x y)))
	       (else
		(binary-lose x y))))))))

(define-primitive = (any-> any->)
  (lambda (x y)
    (cond ((fixnum? x)
	   (if (fixnum? y)
	       (goto return-boolean (fixnum= x y))
	       (if (bignum? y)
		   (goto return-boolean #f)
		   (binary-lose x y))))
	  ((fixnum? y)
	   (if (bignum? x)
	       (goto return-boolean #f)
	       (binary-lose x y)))
	  ((and (bignum? x)
		(bignum? y))
	   (goto return-boolean (bignum= x y)))
	  (else
	   (binary-lose x y)))))

(define-primitive < (any-> any->)
  (lambda (x y)
    (cond ((fixnum? x)
	   (if (fixnum? y)
	       (goto return-boolean (fixnum< x y))
	       (if (bignum? y)
		   (goto return-boolean (bignum-positive? y))
		   (binary-lose x y))))
	  ((fixnum? y)
	   (if (bignum? x)
	       (goto return-boolean (bignum-negative? x))
	       (binary-lose x y)))
	  ((and (bignum? x)
		(bignum? y))
	   (goto return-boolean (bignum< x y)))
	  (else
	   (binary-lose x y)))))

(define-integer-only (<  x y) (enter-boolean (fixnum<  x y)))
(define-integer-only (>  x y) (enter-boolean (fixnum>  x y)))
(define-integer-only (<= x y) (enter-boolean (fixnum<= x y)))
(define-integer-only (>= x y) (enter-boolean (fixnum>= x y)))

(define-integer-only (bitwise-not x)   (fixnum-bitwise-not x))
(define-integer-only (bitwise-and x y) (fixnum-bitwise-and x y))
(define-integer-only (bitwise-ior x y) (fixnum-bitwise-ior x y))
(define-integer-only (bitwise-xor x y) (fixnum-bitwise-xor x y))
