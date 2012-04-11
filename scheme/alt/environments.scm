; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; don't put a copyright notice, silly shell script

(define (*structure-ref struct name)
  (eval name (interaction-environment)))

