; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.



(define (main)
  (write-number (+ (read-number (current-input-port)) 100)
		(current-output-port))
  0)



