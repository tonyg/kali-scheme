
(define (fake-it name)
  (lambda args
    (display "Call to ")
    (display (cons name args))
    (newline)
    0))

(define extended-vm           (fake-it 'extended-vm))
(define lookup-external-name  (fake-it 'lookup-external-name))
(define call-external-value   (fake-it 'call-external-value))
(define ps-schedule-interrupt (fake-it 'ps-schedule-interrupt))

(define (ps-real-time) 0)
(define (ps-run-time) 0)
(define (ps-ticks-per-second) 1)

