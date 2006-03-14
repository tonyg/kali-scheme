; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
	     (register-dozer-unsafe! (+ (real-time) n)
				     (lambda ()
				       (and (cell-ref cell)
					    #t))
				     (lambda ()
				       (make-ready (cell-ref cell))))
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

; We insert a pair consisting of a wakeup time and another pair.

; The second pair contains two thunks; the first one checks if the
; dozer is still alive, the second wakes it up.

(define (register-dozer-unsafe! wakeup-time alive? wakeup!)
  (session-data-set! dozers
		     (insert (cons wakeup-time
				   (cons alive? wakeup!))
			     (session-data-ref dozers)
			     (lambda (frob1 frob2)
			       (< (car frob1)
				  (car frob2))))))

; Note that, if ALIVE? or WAKEUP! isn't a thunk or doesn't run without
; problems, there'll be hell to pay upn wakeup.

(define (register-dozer! user-wakeup-time alive? wakeup!)
  (let ((wakeup-time (coerce-to-nonnegative-integer user-wakeup-time)))
    (cond ((not wakeup-time)
	   (call-error "wrong type argument" register-dozer! user-wakeup-time))
	  (else
	   (let ((ints (set-enabled-interrupts! 0)))
	     (register-dozer-unsafe! wakeup-time alive? wakeup!)
	     (set-enabled-interrupts! ints))))))

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
		     (alive? (cadr next)))
		(cond
		 ((not (alive?))
		  (loop (cdr to-do) woke?))
		 ((< time (car next))
		  (session-data-set! dozers to-do)
		  (values woke? (- (car next) time)))
		 (else
		  ((cddr next))
		  (loop (cdr to-do) #t)))))))))

