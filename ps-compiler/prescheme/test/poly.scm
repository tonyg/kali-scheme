; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


(define (identity x) x)  ; can't get much more polymorphic than that

(define (test x)
  (cond (#f
	 (vector-ref (identity (make-vector 3)) 2))
	(else
	 (+ (identity (+ x 3)) (identity (+ x 2))))))

