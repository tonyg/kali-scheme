; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.


(define-local-syntax (define-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args))))

(define-primitive ashr 2)

(define high-bits ashr)

