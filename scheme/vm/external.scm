; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (fake-it name)
  (lambda args
    (display "Call to ")
    (display (cons name args))
    (newline)
    0))

(define extended-vm        (fake-it 'extended-vm))
(define external-call      (fake-it 'call-external-value))
(define schedule-interrupt (fake-it 'schedule-interrupt))

(define (real-time) 0)
(define (run-time) 0)
(define (cheap-time) 0)

