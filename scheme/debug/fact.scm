; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; don't copyright this, silly shell script


(define (fact n)
  (if (= n 0)
      1
      (* n (fact (- n 1)))))
