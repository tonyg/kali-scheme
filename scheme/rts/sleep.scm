; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Sleeping for N milliseconds.

(define (sleep n)
  (let ((queue (make-thread-queue)))  ; only one entry, but it must be a queue
    (disable-interrupts!)
    (enqueue-thread! queue (current-thread))
    (set! *dozers*
	  (insert (cons (+ (real-time) n) queue)
		  *dozers*
		  (lambda (frob1 frob2)
		    (< (car frob1) (car frob2)))))
    (block)))

(define *dozers* '())  ; List of (wakeup-time . queue)
	  
(define (insert x l <)
  (cond ((null? l) (list x))
	((< x (car l)) (cons x l))
	(else (cons (car l) (insert x (cdr l) <)))))

; Called by root scheduler, so won't be interrupted.
; This returns two values, a boolean that indicates if any threads were
; woken and the time until the next sleeper wakes.  We have to check for
; threads that have been started for some other reason.

(define (wake-some-threads)
  (if (null? *dozers*)
      (values #f #f)
      (let ((time (real-time)))
	(let loop ((dozers *dozers*) (woke? #f))
	  (if (null? dozers)
	      (begin
		(set! *dozers* '())
		(values woke? #f))
	      (let ((next (car dozers)))
		(cond ((thread-queue-empty? (cdr next))
		       (loop (cdr dozers) woke?))
		      ((< time (car next))
		       (set! *dozers* dozers)
		       (values woke? (- (car next) time)))
		      (else
		       (make-ready (dequeue-thread! (cdr next)))
		       (loop (cdr dozers) #t)))))))))
