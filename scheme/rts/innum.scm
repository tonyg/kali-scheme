; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Inexact numbers as mere shells surrounding exact numbers.

(define-extended-number-type :innum (:inexact)
  (make-innum exact)
  innum?
  (exact innum-exact))

(define-method &exact?  ((n :innum)) #f)

(define-method &complex?  ((n :innum)) (complex?  (innum-exact n)))
(define-method &real?     ((n :innum)) (real?	  (innum-exact n)))
(define-method &rational? ((n :innum)) (rational? (innum-exact n)))
(define-method &integer?  ((n :innum)) (integer?  (innum-exact n)))

(define-method &exact->inexact ((n :number))
  (if (innum? n)
      (next-method)
      (make-innum n)))

(define-method &inexact->exact ((n :innum)) (innum-exact n))

(define (inexactify n)
  (if (exact? n)
      (exact->inexact n)
      n))

(define (define-innum-method mtable proc)
  (define-method mtable ((m :innum) (n :number))
    (inexactify (proc (innum-exact m) n)))
  (define-method mtable ((m :number) (n :innum))
    (inexactify (proc m (innum-exact n)))))

(define-innum-method &+ +)
(define-innum-method &- -)
(define-innum-method &* *)
(define-innum-method &/ /)
(define-innum-method &quotient quotient)
(define-innum-method &remainder remainder)

(define (define-innum-comparison mtable proc)
  (define-method mtable ((m :innum) (n :number))
    (proc (innum-exact m) n))
  (define-method mtable ((m :number) (n :innum))
    (proc m (innum-exact n))))

(define-innum-comparison &= =)
(define-innum-comparison &< <)

(define-method &numerator   ((n :innum))
  (inexactify (numerator (innum-exact n))))

(define-method &denominator ((n :innum))
  (inexactify (denominator (innum-exact n))))

(define-method &floor ((n :innum))
  (inexactify (floor (innum-exact n))))

(define-method &number->string ((i :innum) radix)
  (let ((n (innum-exact i)))
    (cond ((integer? n)
	   (string-append (number->string n radix) "."))
	  ((rational? n)
	   (let ((q (denominator n)))
	     (if (= radix 10)
		 (let ((foo (decimable? q)))
		   (if foo
		       (decimal-representation (numerator n) q foo)
		       (string-append "#i" (number->string n radix))))
		 (string-append "#i" (number->string n radix)))))
	  (else
	   (string-append "#i" (number->string n radix))))))

; The Scheme report obligates us to print inexact rationals using
; decimal points whenever this can be done without losing precision.

(define (decimal-representation p q foo)
  (let ((kludge (number->string (* (car foo) (abs (remainder p q)))
				10)))
    (string-append (if (< p 0) "-" "")
		   (number->string (quotient (abs p) q) 10)
		   "."
		   (string-append (do ((i (- (cdr foo) (string-length kludge))
					  (- i 1))
				       (l '() (cons #\0 l)))
				      ((<= i 0) (list->string l)))
				  kludge))))

(define (ratio-string p q radix)
  (string-append (number->string p radix)
		 "/"
		 (number->string q radix)))

; (decimable? n) => non-#f iff n is a product of 2's and 5's.
; The value returned is (k . i) such that 10^i divides n * k.

(define (decimable? n)
  (let loop ((n n) (d 1) (i 0))
    (if (= n 1)
	(cons d i)
	(let ((q (quotient n 10))
	      (r (remainder n 10)))
	  (cond ((= r 0) (loop q d (+ i 1)))
		((= r 5) (loop (quotient n 5) (* d 2) (+ i 1)))
		((even? r) (loop (quotient n 2) (* d 5) (+ i 1)))
		(else #f))))))
