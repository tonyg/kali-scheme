; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (church n)
  (let loop ((application 'x) (n n))
    (if (zero? n)
	`(lambda (f) (lambda (x) ,application))
	(loop `(f ,application) (- n 1)))))

(define c10000 (church 10000))

(define (f)
  (eval c10000 (interaction-environment)))
