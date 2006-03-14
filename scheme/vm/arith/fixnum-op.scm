; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Fixnum-only primitive operations

; These predicates are used to characterize the numeric representations that
; are implemented in the VM.

(define (unary-lose x)
  (raise-exception wrong-type-argument 0 x))

(define (binary-lose x y)
  (raise-exception wrong-type-argument 0 x y))

(define-primitive number? (any->)
  (lambda (x)
    (or (fixnum? x)
	(bignum? x)
	(ratnum? x)
	(double? x)
	(extended-number? x)))
  return-boolean)

(define-primitive integer?  (any->)
  (lambda (n)
    (cond ((or (fixnum? n)
	       (bignum? n))
	       (goto return-boolean #t))
	  ((or (extended-number? n)
	       (double? n))
	   (unary-lose n))
	  (else
	   (goto return-boolean #f)))))

(define vm-number-predicate
  (lambda (n)
    (cond ((or (fixnum? n)
	       (bignum? n)
	       (and rationals? (ratnum? n))
	       (and doubles? (double? n)))
	   (goto return-boolean #t))
	  ((extended-number? n)
	   (unary-lose n))
	  (else
	   (goto return-boolean #f)))))

(define-primitive rational? (any->) vm-number-predicate)
(define-primitive real?     (any->) vm-number-predicate)
(define-primitive complex?  (any->) vm-number-predicate)

;----------------
; A macro for defining primitives that only operate on fixnums.

(define-syntax define-fixnum-only
  (syntax-rules ()
    ((define-fixnum-only (opcode arg) value)
     (define-primitive opcode (any->)
       (lambda (arg)
	 (if (fixnum? arg)
	     (goto return value)
	     (unary-lose arg)))))
    ((define-fixnum-only (opcode arg0 arg1) value)
     (define-primitive opcode (any-> any->)
       (lambda (arg0 arg1)
	 (if (and (fixnum? arg0)
		  (fixnum? arg1))
	     (goto return value)
	     (binary-lose arg0 arg1)))))))

; These primitives have a simple answer in the case of fixnums; for all others
; they punt to the run-time system.

(define-fixnum-only (exact?      n) true)
(define-fixnum-only (real-part   n) n)
(define-fixnum-only (imag-part   n) (enter-fixnum 0))
(define-fixnum-only (floor       n) n)
(define-fixnum-only (numerator   n) n)
(define-fixnum-only (denominator n) (enter-fixnum 1))

(define-primitive angle (any->)
  (lambda (n)
    (if (and (fixnum? n)
	     (>= n 0))
	(goto return (enter-fixnum 0))
	(unary-lose n))))

(define-primitive magnitude (any->)
  (lambda (n)
    (if (fixnum? n)
	(abs-carefully n
		       (lambda (r)
			 (goto return r))
		       unary-lose)
	(unary-lose n))))

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

(define-punter  atan1)
(define-punter2 atan2)
(define-punter2 make-polar)
(define-punter2 make-rectangular)

(define-syntax define-binop
  (syntax-rules ()
    ((define-binop opcode careful-op)
     (define-primitive opcode (any-> any->)
       (lambda (x y)
	 (if (and (fixnum? x)
		  (fixnum? y))
	     (goto careful-op x y return binary-lose)
	     (binary-lose x y)))))))

(define-binop + add-carefully)
(define-binop - subtract-carefully)
(define-binop * multiply-carefully)
(define-binop / divide-carefully)
(define-binop quotient         quotient-carefully)
(define-binop remainder        remainder-carefully)
(define-binop arithmetic-shift shift-carefully)

(define-fixnum-only (=  x y) (enter-boolean (fixnum=  x y)))
(define-fixnum-only (<  x y) (enter-boolean (fixnum<  x y)))
(define-fixnum-only (>  x y) (enter-boolean (fixnum>  x y)))
(define-fixnum-only (<= x y) (enter-boolean (fixnum<= x y)))
(define-fixnum-only (>= x y) (enter-boolean (fixnum>= x y)))

(define-fixnum-only (bitwise-not x)   (fixnum-bitwise-not x))
(define-fixnum-only (bitwise-and x y) (fixnum-bitwise-and x y))
(define-fixnum-only (bitwise-ior x y) (fixnum-bitwise-ior x y))
(define-fixnum-only (bitwise-xor x y) (fixnum-bitwise-xor x y))
