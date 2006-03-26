; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (s48-disable-interrupts!)
  (disable-interrupts!))

(define (s48-enable-interrupts!)
  (enable-interrupts!))

					; used for raising exceptions in external code
(define (s48-push x)
  (push x))

(define (s48-stack-ref i)
  (stack-ref i))

(define (s48-stack-set! x v)
  (stack-set! x v))

(define (s48-enter-integer x)
  (enter-integer x (ensure-space long-as-integer-size)))

					; arguments must either both be intergers or both floanums
(define (s48-integer-or-floanum-add x y)
  (if (double? x)
      (flonum-add x y)
      (integer-add x y)))
(define (s48-integer-or-floanum-sub x y)
  (if (double? x)
      (flonum-subtract x y)
      (integer-subtract x y)))

(define (s48-integer-or-floanum-mul x y)
  (if (double? x)
      (flonum-multiply x y)
      (integer-multiply x y)))

(define (s48-integer-remainder x y)
  (integer-remainder x y))

(define (s48-integer-quotient x y)
  (integer-quotient x y))

(define (s48-integer-divide x y)
  (integer-divide x y))

(define-syntax define-integer-or-floanum-comparison
  (syntax-rules ()
    ((define-integer-or-floanum-comparison 
       s48-integer-or-floanum-proc integer-op flonum-op)
     (define (s48-integer-or-floanum-proc x y)
       (enter-boolean
	(if (double? x)
	    (flonum-op x y)
	    (integer-op x y)))))))

(define-integer-or-floanum-comparison s48-integer-or-floanum-= integer= flonum=)
(define-integer-or-floanum-comparison s48-integer-or-floanum-< integer< flonum<)
(define-integer-or-floanum-comparison s48-integer-or-floanum-> integer> flonum>)
(define-integer-or-floanum-comparison s48-integer-or-floanum-<= integer<= flonum<=)
(define-integer-or-floanum-comparison s48-integer-or-floanum->= integer>= flonum>=)

(define (s48-integer-bitwise-not x)
  (integer-bitwise-not x))
(define (s48-integer-bit-count x)
  (integer-bit-count x))
(define (s48-integer-bitwise-and x y)
  (integer-bitwise-and x y))
(define (s48-integer-bitwise-ior x y)
  (integer-bitwise-ior x y))
(define (s48-integer-bitwise-xor x y)
  (integer-bitwise-xor x y))

(define (s48-string-set s i c)
  (vm-string-set! s i c))

(define (s48-string-ref s i)
  (vm-string-ref s i))

(define (s48-string-length s)
  (vm-string-length s))

(define (s48-allocate-string len)
  (vm-make-string+gc len))

(define (s48-enter-string-latin-1 s)
  (enter-string+gc s))

(define (s48-copy-string-to-scheme-string-latin-1 string len vm-string)
  (copy-string-to-vm-string/latin-1! string len vm-string))

(define (s48-copy-scheme-string-to-string-latin-1 vm-string string)
  (copy-vm-string-to-string/latin-1! vm-string string))
