; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


(define (%unspecific)
  (if #f #f))

(define (!= x y)
  (not (= x y)))

(define (%tuple . rest)
  (list->vector (cons 'tuple rest)))
