; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; quick hack

(define (vm-extension operation arg)
  (case operation
    ((7) 0)				;read jumpers
    (else unbound-marker)))
