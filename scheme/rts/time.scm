; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (real-time)
  (time (enum time-option real-time) #f))

(define (run-time)
  (time (enum time-option run-time) #f))
