; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Complex arithmetic, built on real arithmetic.

(define recnum-type
  (make-extended-number-type "Recnum" '(real-part imag-part)))

(define recnum? (extended-number-predicate recnum-type))
(define make-recnum
  (extended-number-constructor recnum-type '(real-part imag-part)))

(define recnum-real-part (extended-number-accessor recnum-type 'real-part))
(define recnum-imag-part (extended-number-accessor recnum-type 'imag-part))

(define (rectangular-real-part z)
  (if (recnum? z)
      (recnum-real-part z)
      (real-part z)))

(define (rectangular-imag-part z)
  (if (recnum? z)
      (recnum-imag-part z)
      (imag-part z)))

(define (rectangular+ a b)
  (make-rectangular (+ (rectangular-real-part a) (rectangular-real-part b))
		    (+ (rectangular-imag-part a) (rectangular-imag-part b))))

(define (rectangular- a b)
  (make-rectangular (- (rectangular-real-part a) (rectangular-real-part b))
		    (- (rectangular-imag-part a) (rectangular-imag-part b))))

(define (rectangular* a b)
  (let ((a1 (rectangular-real-part a))
	(a2 (rectangular-imag-part a))
	(b1 (rectangular-real-part b))
	(b2 (rectangular-imag-part b)))
    (make-rectangular (- (* a1 b1) (* a2 b2))
		      (+ (* a1 b2) (* a2 b1)))))

(define (rectangular/ a b)
  (let ((a1 (rectangular-real-part a))
	(a2 (rectangular-imag-part a))
	(b1 (rectangular-real-part b))
	(b2 (rectangular-imag-part b)))
    (let ((d (+ (* b1 b1) (* b2 b2))))
      (make-rectangular (/ (+ (* a1 b1) (* a2 b2)) d)
			(/ (- (* a2 b1) (* a1 b2)) d)))))

(define (rectangular= a b)
  (let ((a1 (rectangular-real-part a))
	(a2 (rectangular-imag-part a))
	(b1 (rectangular-real-part b))
	(b2 (rectangular-imag-part b)))
    (and (= a1 b1) (= a2 b2))))


; Methods

(define recnum-family (make-family 'recnum 6))

(define (define-rectangular-method table proc)
  (define-method table recnum-family proc))

; Methods on recnums

(define (true-for-recnums n)
  (if (recnum? n) #t (fail)))

(define-rectangular-method complex?-table  true-for-recnums)
(define-rectangular-method number?-table   true-for-recnums)

(define (when-recnum proc)
  (lambda (n) (if (recnum? n) (proc n) (fail))))

(define-rectangular-method real-part-table (when-recnum recnum-real-part))
(define-rectangular-method imag-part-table (when-recnum recnum-imag-part))

; Methods on complexes in terms of real-part and imag-part

(define-rectangular-method exact?-table
  (lambda (z)
    (if (and (complex? z) (not (real? z)))
	(exact? (real-part z))
	(fail))))

(define-rectangular-method inexact->exact-table
  (lambda (z)
    (if (and (complex? z) (not (real? z)))
	(make-rectangular (inexact->exact (real-part z))
			  (inexact->exact (imag-part z)))
	(fail))))

(define-rectangular-method exact->inexact-table
  (lambda (z)
    (if (and (complex? z) (not (real? z)))
	(make-rectangular (exact->inexact (real-part z))
			  (exact->inexact (imag-part z)))
	(fail))))

(define (when-complexes proc)
  (lambda (m n)
    (if (and (complex? m) (complex? n)
	     (not (and (real? m) (real? n))))
	(proc m n)
	(fail))))

(define-rectangular-method plus-table  (when-complexes rectangular+))
(define-rectangular-method minus-table  (when-complexes rectangular-))
(define-rectangular-method *-table  (when-complexes rectangular*))
(define-rectangular-method /-table  (when-complexes rectangular/))
(define-rectangular-method =-table  (when-complexes rectangular=))

(define-rectangular-method sqrt-table
  (lambda (n)
    (if (and (integer? n)
             (< n 0))
        (make-rectangular 0 (sqrt (- n)))
        (fail))))  ; not that we have to

; Gleep!  Can we do quotient and remainder on Gaussian integers?
; Can we do numerator and denominator on complex rationals?

(define-rectangular-method number->string-table
  (lambda (n radix)
    (if (and (complex? n) (not (real? n)) (exact? n))
	(let ((x (real-part n))
	      (y (imag-part n)))
	  (string-append (really-number->string x radix)
			 (if (< y 0) "-" "+")
			 (really-number->string (abs y) radix)
			 "i"))
	(fail))))

(define-rectangular-method string->number-table
  (lambda (s radix exact?)
    (let ((len-1 (- (string-length s) 1)))
      (if (char=? (string-ref s len-1) #\i)
	  (let loop ((i (- len-1 1)))
	    (if (< i 0) (fail)
		(let ((c (string-ref s i)))
		  (if (or (char=? c #\+)
			  (char=? c #\-))
		      (let ((x (if (= i 0)
				   0
				   (really-string->number (substring s 0 i)
							  radix exact?)))
			    (y (if (= i (- len-1 1))
				   (if (char=? c #\+) 1 -1)
				   (really-string->number (substring s i len-1)
							  radix exact?))))
			(if (and (real? x) (real? y))
			    (make-rectangular x y)
			    (fail)))
		      (loop (- i 1))))))
	  (fail)))))

(define-method make-rectangular-table recnum-family
  (lambda (x y)
    (if (eq? (exact? x) (exact? y))
	(if (= y 0)
	    x
	    (make-recnum x y))
	(make-rectangular (if (exact? x) (exact->inexact x) x)
			  (if (exact? y) (exact->inexact y) y)))))
