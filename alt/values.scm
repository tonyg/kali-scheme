; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Multiple return values

(define multiple-value-token (vector 'multiple-value-token))

(define (values . things)
  (if (and (pair? things)
	   (null? (cdr things)))
      (car things)
      (cons multiple-value-token things)))

(define (call-with-values producer consumer)
  (let ((things (producer)))
    (if (and (pair? things)
	     (eq? (car things) multiple-value-token))
	(apply consumer (cdr things))
	(consumer things))))
