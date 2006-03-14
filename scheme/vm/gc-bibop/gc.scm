; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; exported for use by c/bibop/generation_gc.c

(define (s48-trace-continuation contents-pointer size)
  (trace-continuation contents-pointer size)
  (unspecific))

