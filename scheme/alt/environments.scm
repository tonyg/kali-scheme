; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; don't put a copyright notice, silly shell script

(define (*structure-ref struct name)
  (eval name (interaction-environment)))

