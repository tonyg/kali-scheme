; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file ratnum.scm.

; Rational arithmetic
; Assumes that +, -, etc. perform integer arithmetic.

(define-simple-type :exact-rational (:rational :exact)
  (lambda (n) (and (rational? n) (exact? n))))

(define-extended-number-type :ratnum (:exact-rational :exact) ;?
  (make-ratnum num den)
  ratnum?
  (num ratnum-numerator)
  (den ratnum-denominator))

(define (integer/ m n)
  (cond ((< n 0)
	 (integer/ (- 0 m) (- 0 n)))
	((= n 0)
	  (assertion-violation '/ "rational division by zero" m))
	((and (exact? m) (exact? n))
	 (let ((g (gcd m n)))
	   (let ((m (quotient m g))
		 (n (quotient n g)))
	     (if (= n 1)
		 m
		 (make-ratnum m n)))))
	(else (/ m n))))    ;In case we get flonums

(define (rational-numerator p)
  (if (ratnum? p)
      (ratnum-numerator p)
      (numerator p)))

(define (rational-denominator p)
  (if (ratnum? p)
      (ratnum-denominator p)
      (denominator p)))

; a/b * c/d = a*c / b*d

(define (rational* p q)
  (integer/ (* (rational-numerator p) (rational-numerator q))
	    (* (rational-denominator p) (rational-denominator q))))

; a/b / c/d = a*d / b*c

(define (rational/ p q)
  (integer/ (* (rational-numerator p) (rational-denominator q))
	    (* (rational-denominator p) (rational-numerator q))))

; a/b + c/d = (a*d + b*c)/(b*d)

(define (rational+ p q)
  (let ((b (rational-denominator p))
	(d (rational-denominator q)))
    (integer/ (+ (* (rational-numerator p) d)
		 (* b (rational-numerator q)))
	      (* b d))))

; a/b - c/d = (a*d - b*c)/(b*d)

(define (rational- p q)
  (let ((b (rational-denominator p))
	(d (rational-denominator q)))
    (integer/ (- (* (rational-numerator p) d)
		 (* b (rational-numerator q)))
	      (* b d))))

; a/b < c/d  when  a*d < b*c

(define (rational< p q)
  (< (* (rational-numerator p) (rational-denominator q))
     (* (rational-denominator p) (rational-numerator q))))

; a/b = c/d  when a = b and c = d  (always lowest terms)

(define (rational= p q)
  (and (= (rational-numerator p) (rational-numerator q))
       (= (rational-denominator p) (rational-denominator q))))

; (rational-truncate p) = integer of largest magnitude <= (abs p)

(define (rational-truncate p)
  (quotient (rational-numerator p) (rational-denominator p)))

; (floor p) = greatest integer <= p

(define (rational-floor p)
  (let* ((n (numerator p))
	 (q (quotient n (denominator p))))
    (if (>= n 0)
	q
	(- q 1))))


; Extend the generic number procedures

(define-method &rational? ((n :ratnum)) #t)

(define-method &numerator   ((n :ratnum)) (ratnum-numerator n))
(define-method &denominator ((n :ratnum)) (ratnum-denominator n))

(define-method &exact? ((n :ratnum)) #t)

;(define-method &exact->inexact ((n :ratnum))
;  (/ (exact->inexact (numerator n))
;     (exact->inexact (denominator n))))

;(define-method &inexact->exact ((n :rational))  ;?
;  (/ (inexact->exact (numerator n))
;     (inexact->exact (denominator n))))

(define-method &/ ((m :exact-integer) (n :exact-integer))
  (integer/ m n))

(define (define-ratnum-method mtable proc)
  (define-method mtable ((m :ratnum) (n :exact-rational)) (proc m n))
  (define-method mtable ((m :exact-rational) (n :ratnum)) (proc m n)))

(define-ratnum-method &+ rational+)
(define-ratnum-method &- rational-)
(define-ratnum-method &* rational*)
(define-ratnum-method &/ rational/)
(define-ratnum-method &= rational=)
(define-ratnum-method &< rational<)

(define-method &floor ((m :ratnum)) (rational-floor m))

;(define-method &sqrt ((p :ratnum))
;  (if (< p 0)
;      (next-method)
;      (integer/ (sqrt (numerator p))
;                (sqrt (denominator p)))))


(define-method &number->string ((p :ratnum) radix)
  (string-append (number->string (ratnum-numerator p) radix)
		 "/"
		 (number->string (ratnum-denominator p) radix)))
