; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Sleeping for N milliseconds.

(define (sleep n)
  (let ((queue (make-thread-queue)))  ; only one entry, but it must be a queue
    (disable-interrupts!)
    (enqueue-thread! queue (current-thread))
    (session-data-set! dozers
		       (insert (cons (+ (real-time) n) queue)
			       (session-data-ref dozers)
			       (lambda (frob1 frob2)
				 (< (car frob1) (car frob2)))))
    (block)))

(define dozers (make-session-data-slot! '()))
	  
(define (insert x l <)
  (cond ((null? l)
	 (list x))
	((< x (car l))
	 (cons x l))
	(else
	 (cons (car l)
	       (insert x (cdr l) <)))))

; Called by root scheduler, so won't be interrupted.
; This returns two values, a boolean that indicates if any threads were
; woken and the time until the next sleeper wakes.  We have to check for
; threads that have been started for some other reason.

(define (wake-some-threads)
  (if (null? (session-data-ref dozers))
      (values #f #f)
      (let ((time (real-time)))
	(let loop ((to-do (session-data-ref dozers)) (woke? #f))
	  (if (null? to-do)
	      (begin
		(session-data-set! dozers '())
		(values woke? #f))
	      (let ((next (car to-do)))
		(cond ((thread-queue-empty? (cdr next))
		       (loop (cdr to-do) woke?))
		      ((< time (car next))
		       (session-data-set! dozers to-do)
		       (values woke? (- (car next) time)))
		      (else
		       (make-ready (dequeue-thread! (cdr next)))
		       (loop (cdr to-do) #t)))))))))
