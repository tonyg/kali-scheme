; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-macro (define-syntax macro-name transformer . stuff)
  `(define-macro (,macro-name . args)
     (,transformer (cons ',macro-name args)
		   (lambda (x) x)
		   eq?)))
