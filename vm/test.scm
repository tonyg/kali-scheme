; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.



(define (primitive-throw cont val)
  (with-continuation cont (lambda () val)))

(define (test output)
  (write-string (primitive-catch
		 (lambda (cont)
		   (begin
		     (primitive-throw cont "Hello sailor...")
		     "oops!")))
		output)
  (write-char #\newline output))
