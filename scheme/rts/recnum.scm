; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Rectangular complex arithmetic built on real arithmetic.

(define-extended-number-type :recnum (:complex)
  (make-recnum real imag)
  recnum?
  (real recnum-real-part)
  (imag recnum-imag-part))

(define (rectangulate x y)    ; Assumes (eq? (exact? x) (exact? y))
  (if (= y 0)
      x
      (make-recnum x y)))

(define (rectangular-real-part z)
  (if (recnum? z)
      (recnum-real-part z)
      (real-part z)))

(define (rectangular-imag-part z)
  (if (recnum? z)
      (recnum-imag-part z)
      (imag-part z)))

(define (rectangular+ a b)
  (rectangulate (+ (rectangular-real-part a) (rectangular-real-part b))
		(+ (rectangular-imag-part a) (rectangular-imag-part b))))

(define (rectangular- a b)
  (rectangulate (- (rectangular-real-part a) (rectangular-real-part b))
		(- (rectangular-imag-part a) (rectangular-imag-part b))))

(define (rectangular* a b)
  (let ((a1 (rectangular-real-part a))
	(a2 (rectangular-imag-part a))
	(b1 (rectangular-real-part b))
	(b2 (rectangular-imag-part b)))
    (rectangulate (- (* a1 b1) (* a2 b2))
		  (+ (* a1 b2) (* a2 b1)))))

(define (rectangular/ a b)
  (let ((a1 (rectangular-real-part a))
	(a2 (rectangular-imag-part a))
	(b1 (rectangular-real-part b))
	(b2 (rectangular-imag-part b)))
    (let ((d (+ (* b1 b1) (* b2 b2))))
      (rectangulate (/ (+ (* a1 b1) (* a2 b2)) d)
		    (/ (- (* a2 b1) (* a1 b2)) d)))))

(define (rectangular= a b)
  (let ((a1 (rectangular-real-part a))
	(a2 (rectangular-imag-part a))
	(b1 (rectangular-real-part b))
	(b2 (rectangular-imag-part b)))
    (and (= a1 b1) (= a2 b2))))


; Methods

(define-method &complex? ((z :recnum)) #t)

(define-method &real-part ((z :recnum)) (recnum-real-part z))
(define-method &imag-part ((z :recnum)) (recnum-imag-part z))

(define-method &magnitude ((z :recnum))
  (let ((r (recnum-real-part z))
	(i (recnum-imag-part z)))
    (sqrt (+ (* r r) (* i i)))))

(define-method &angle ((z :recnum))
  (atan (recnum-imag-part z)
	(recnum-real-part z)))

; Methods on complexes in terms of real-part and imag-part

(define-method &exact? ((z :recnum))
  (exact? (recnum-real-part z)))

(define-method &inexact->exact ((z :recnum))
  (make-recnum (inexact->exact (recnum-real-part z))
	       (inexact->exact (recnum-imag-part z))))

(define-method &exact->inexact ((z :recnum))
  (make-recnum (exact->inexact (recnum-real-part z))
	       (exact->inexact (recnum-imag-part z))))

(define (define-recnum-method mtable proc)
  (define-method mtable ((m :recnum) (n :complex)) (proc m n))
  (define-method mtable ((m :complex) (n :recnum)) (proc m n)))

(define-recnum-method &+ rectangular+)
(define-recnum-method &- rectangular-)
(define-recnum-method &* rectangular*)
(define-recnum-method &/ rectangular/)
(define-recnum-method &= rectangular=)

(define-method &sqrt ((n :real))
  (if (< n 0)
      (make-rectangular 0 (sqrt (- 0 n)))
      (next-method)))			; not that we have to

(define-method &sqrt ((z :recnum))
  (exp (/ (log z) 2)))

(define plus-i (make-recnum 0 1)) ; we can't read +i yet
(define minus-i (make-recnum 0 -1))

(define-method &exp ((z :recnum))
  (let ((i (imag-part z)))
    (* (exp (real-part z))
       (+ (cos i) (* plus-i (sin i))))))

(define-method &log ((z :recnum))
  (+ (log (magnitude z)) (* plus-i (angle z))))

(define pi (delay (* 2 (asin 1)))) ; can't compute at build time

(define-method &log ((n :real))
  (if (< n 0)
      (make-rectangular (log (- 0 n)) (force pi))
      (next-method)))

(define-method &sin ((c :recnum))
  (let ((i-c (* c plus-i)))
    (/ (- (exp i-c)
	  (exp (- 0 i-c)))
       (* 2 plus-i))))

(define-method &cos ((c :recnum))
  (let ((i-c (* c plus-i)))
    (/ (+ (exp i-c)
	  (exp (- 0 i-c)))
       2)))

(define-method &tan ((c :recnum))
  (/ (sin c) (cos c)))

(define-method &asin ((c :recnum))
  (* minus-i
     (log (+ (* c plus-i)
	     (sqrt (- 1 (* c c)))))))

(define-method &acos ((c :recnum))
  (* minus-i
     (log (+ c
	     (* plus-i (sqrt (- 1 (* c c))))))))

; kludge; we can't read floating point yet
(define infinity (delay (expt (exact->inexact 2) (exact->inexact 1500))))

(define-method &atan1 ((c :recnum))
  (if (or (= c plus-i)
	  (= c minus-i))
      (- 0 (force infinity))
      (* plus-i
	 (/ (log (/ (+ plus-i c)
		    (+ plus-i (- 0 c))))
	    2))))

; Gleep!  Can we do quotient and remainder on Gaussian integers?
; Can we do numerator and denominator on complex rationals?

(define-method &number->string ((z :recnum) radix)
  (let ((x (real-part z))
	(y (imag-part z)))
    (let ((r (number->string x radix))
	  (i (number->string (abs y) radix))
	  (& (if (< y 0) "-" "+")))
      (if (and (inexact? y)		;gross
	       (char=? (string-ref i 0) #\#))
	  (string-append (if (char=? (string-ref r 0) #\#)
			     ""
			     "#i")
			 r &
			 (substring i 2 (string-length i))
			 "i")
	  (string-append r & i "i")))))

(define-method &make-rectangular ((x :real) (y :real))
  (if (eq? (exact? x) (exact? y))
      (rectangulate x y)
      (rectangulate (if (exact? x) (exact->inexact x) x)
		    (if (exact? y) (exact->inexact y) y))))
