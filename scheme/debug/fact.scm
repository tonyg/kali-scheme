; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; don't copyright this, silly shell script


(define (fact n)
  (if (= n 0)
      1
      (* n (fact (- n 1)))))
