; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (%unspecific)
  (if #f #f))

(define (!= x y)
  (not (= x y)))

(define (%tuple . rest)
  (list->vector (cons 'tuple rest)))
