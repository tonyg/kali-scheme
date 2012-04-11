; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

(define (error? status)
  (not (eq? status (enum errors no-errors))))

(define (write-error-string string)
  (write-string string (current-error-port)))

(define (write-error-integer integer)
  (write-integer integer (current-error-port)))

(define (error-newline)
  (write-char #\newline (current-error-port)))
