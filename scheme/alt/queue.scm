; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Queues

(define (make-queue)
  (cons '() '()))

(define (queue-empty? q)
  (and (null? (car q))
       (null? (cdr q))))

(define (enqueue! q obj)
  (set-car! q (cons obj (car q))))

(define (dequeue! q)
  (normalize-queue! q)
  (let ((head (car (cdr q))))
    (set-cdr! q (cdr (cdr q)))
    head))

(define (normalize-queue! q)
  (if (null? (cdr q))
      (begin (set-cdr! q (reverse (car q)))
	     (set-car! q '()))))

(define (queue-head q)
  (normalize-queue! q)
  (car (cdr q)))
