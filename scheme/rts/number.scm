; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file number.scm.


;;;; Numbers

(define (inexact? n) (not (exact? n)))

(define (modulo x y)
   (let ((r (remainder x y)))
      (if (> y 0)
	  (if (< r 0)
	      (+ r y)
	      r)
	  (if (> r 0)
	      (+ r y)
	      r))))

(define (ceiling x)
  (- 0 (floor (- 0 x))))		;floor is primitive

(define (truncate x)
  (if (< x 0)
      (ceiling x)
      (floor x)))

(define (round x)
  (let* ((x+1/2 (+ x (/ 1 2)))
	 (r (floor x+1/2)))
    (if (and (= r x+1/2)
	     (odd? r))
	(- r 1)
	r)))
	
; GCD

(define (gcd . integers)
  (reduce (lambda (x y)
	    (cond ((< x 0) (gcd (- 0 x) y))
		  ((< y 0) (gcd x (- 0 y)))
		  ((< x y) (euclid y x))
		  (else (euclid x y))))
	  0
	  integers))

(define (euclid x y)
  (if (= y 0)
      (if (and (inexact? y)
	       (exact? x))
	  (exact->inexact x)
	  x)
      (euclid y (remainder x y))))

; LCM

(define (lcm . integers)
  (reduce (lambda (x y)
	    (let ((g (gcd x y)))
	      (cond ((= g 0) g)
		    (else (* (quotient (abs x) g) (abs y))))))
	  1
	  integers))

; Exponentiation.

(define (expt x n)
  (if (and (integer? n) (exact? n))
      (if (>= n 0)
	  (raise-to-integer-power x n)
	  (/ 1 (raise-to-integer-power x (- 0 n))))
      (exp (* n (log x)))))

(define (raise-to-integer-power x n)
  (if (= n 0)
      1
      (let loop ((s x) (i n) (a 1))	;invariant: a * s^i = x^n
	(let ((a (if (odd? i) (* a s) a))
	      (i (quotient i 2)))
	  (if (= i 0)
	      a
	      (loop (* s s) i a))))))
