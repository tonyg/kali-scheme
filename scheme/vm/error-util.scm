; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (error? status)
  (not (eq? status (enum errors no-errors))))

(define (write-error-string string)
  (write-string string (current-error-port)))

(define (write-error-integer integer)
  (write-integer integer (current-error-port)))

(define (error-newline)
  (write-char #\newline (current-error-port)))
