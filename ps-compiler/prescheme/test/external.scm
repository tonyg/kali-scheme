; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


(define foo
  (external (=> (int32 int32) unit)
	    (lambda (x y)
	      (display (+ x y)))))

(define (test)
  (foo 3 4))
