
(define foo
  (external (=> (int32 int32) unit)
	    (lambda (x y)
	      (display (+ x y)))))

(define (test)
  (foo 3 4))
