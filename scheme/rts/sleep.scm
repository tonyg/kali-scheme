; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Sleeping for N milliseconds.
;
; The current thread is the only one every on QUEUE, but we cannot block without
; being on some queue.
;
; CALL-ERROR has to be called from us, not COERCE-TO-... because it is supposed
; to have our continuation.

(define (sleep user-n)
  (let ((n (coerce-to-nonnegative-integer user-n)))
    (cond ((not n)
	   (call-error "wrong type argument" sleep user-n))
	  ((< 0 n)
	   (let ((cell (make-cell (current-thread))))
	     (disable-interrupts!)
	     (set-thread-cell! (current-thread) cell)
	     (session-data-set! dozers
				(insert (cons (+ (real-time) n)
					      cell)
					(session-data-ref dozers)
					(lambda (frob1 frob2)
					  (< (car frob1)
					     (car frob2)))))
	     (block))))))

(define (coerce-to-nonnegative-integer n)
  (if (real? n)
      (let* ((n (round n))
	     (n (if (exact? n)
		    n
		    (inexact->exact n))))
	(if (<= 0 n)
	    n
	    #f))
      #f))

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
	      (let* ((next (car to-do))
		     (thread (cell-ref (cdr next))))
		(cond ((not thread)
		       (loop (cdr to-do) woke?))
		      ((< time (car next))
		       (session-data-set! dozers to-do)
		       (values woke? (- (car next) time)))
		      (else
		       (make-ready thread)
		       (loop (cdr to-do) #t)))))))))

