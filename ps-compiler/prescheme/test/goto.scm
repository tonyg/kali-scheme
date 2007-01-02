; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.



(define (odd? x)
  (cond ((= x 0)
	 #f)
;	((= x 100)
;	 (foo))
	(else
	 (goto even? (- x 1)))))

(define (even? x)
  (if (= x 0)
      #t
      (goto odd? (- x 1))))

(define (test x)
  (if (odd? (+ x 1))
      (error "an even number" x)))
