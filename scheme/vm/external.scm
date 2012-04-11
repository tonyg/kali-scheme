; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


(define (fake-it name)
  (lambda args
    (display "Call to ")
    (display (cons name args))
    (newline)
    0))

(define extended-vm           (fake-it 'extended-vm))
(define lookup-external-name  (fake-it 'lookup-external-name))
(define call-external-value   (fake-it 'call-external-value))
(define schedule-interrupt (fake-it 'schedule-interrupt))

(define (real-time) 0)
(define (run-time) 0)
(define (cheap-time) 0)

