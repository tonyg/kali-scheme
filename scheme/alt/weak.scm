; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (make-weak-pointer x) (cons '<weak> x))
(define weak-pointer-ref cdr)
(define (weak-pointer? x)
  (and (pair? x) (eq? (car x) '<weak>)))

