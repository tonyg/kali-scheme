; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

(define (real-time)
  (time (enum time-option real-time) #f))

(define (run-time)
  (time (enum time-option run-time) #f))
