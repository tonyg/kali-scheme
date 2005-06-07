; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


(define *count* 0)

(define (increment)
  (set! *count* (+ *count* 1)))

(define (value)
  *count*)

(define (test out)
  (increment)
  (increment)
  (write-number (value) out)
  (newline out))

(test (current-output-port))
