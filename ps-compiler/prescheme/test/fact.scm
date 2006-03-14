; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.



(define *one* 1)

(define (fact n)
  (let loop ((i n) (r *one*))
    (if (<= *one* i)
	(loop (- i *one*) (* i r))
	r)))

(define (all)
  (set! *one* (fact (if (> (fact 10) 100) 10 20))))
