; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; unworthy of copyright notice

(define (make-table . hash-procedure-option) (list 'table))

(define (table-ref table key)
  (let ((probe (assq key (cdr table))))
    (if probe (cdr probe) #f)))

(define (table-set! table key value)
  (let ((probe (assq key (cdr table))))
    (if probe
	(set-cdr! probe value)
	(set-cdr! table (cons (cons key value) (cdr table))))))
