; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; don't put a copyright notice, silly shell script

(define (*structure-ref struct name)
  (eval name (interaction-environment)))

