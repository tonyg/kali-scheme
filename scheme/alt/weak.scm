; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


(define (make-weak-pointer x) (cons '<weak> x))
(define weak-pointer-ref cdr)
(define (weak-pointer? x)
  (and (pair? x) (eq? (car x) '<weak>)))

