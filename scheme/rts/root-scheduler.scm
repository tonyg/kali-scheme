; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; The root scheduler.

(define (root-scheduler thunk quantum housekeeping-quantum)
  (let ((*result* 123))
    (call-with-current-continuation
      (lambda (abort)
	(call-with-values
	 (lambda ()
	   (make-root-event-handler quantum abort))
	 (lambda (handler add-thread!)
	   (let ((housekeeper (lambda ()
				(spawn-output-forcers add-thread! #t)
				(wake-some-threads))))
	     (initialize-channel-i/o!)
	     (add-thread! (lambda () (set! *result* (thunk)))
			  'scheduler-initial-thread)
	     (run-threads-with-housekeeper handler
					   housekeeper
					   housekeeping-quantum)
	     *result*)))))))

(define (make-root-event-handler quantum abort)
  (let ((runnable (make-thread-queue))
	(thread-count (make-counter))
	(dynamic-env (get-dynamic-env)))
    (let ((add-thread!
	   (lambda (thunk id)
	     (increment-counter! thread-count)
	     (exclusively-enqueue-thread!
	      runnable
	      (make-thread thunk dynamic-env id)
	      '()))))
      (values (round-robin-event-handler
	         runnable quantum dynamic-env thread-count
		 (lambda args #f)              ; we handle no events
		 (lambda (thread token args)          ; upcall handler
		   (if (eq? token abort-token)
		       (abort (car args))
		       (propogate-upcall thread token args)))
		 (lambda () (root-wait add-thread!)))
	      add-thread!))))

; Upcall token

(define abort-token (list 'abort-token))

; Getting around to calling the VM's WAIT procedure.  We disable interrupts
; to keep things from happening behind our back, and then see if there is
; any thread to run or any event pending, or if work may appear in the future.

(define (root-wait add-thread!)
  (let loop ()
    (with-interrupts-inhibited
     (lambda ()
       (let ((forcers? (spawn-output-forcers add-thread! #f)))
	 (call-with-values
	  wake-some-threads
	  (lambda (woke-some? time-until-wakeup)
	    (cond ((or forcers? woke-some? (event-pending?))
		   #t)
		  ((or time-until-wakeup
		       (waiting-for-i/o?))
		   (do-some-waiting time-until-wakeup)
		   (loop))
		  ((session-data-ref deadlock-handler)
		   => (lambda (handler)
			(handler)
			#t))
		  (else
		   #f)))))))))

(define one-day-of-milliseconds (* (* 1000 60) (* 60 24)))

; A mess because a fixnum's worth of milliseconds is only a few days.
; The VM's WAIT procedure takes its maximum-wait argument in either
; milliseconds or minutes.

(define (do-some-waiting time-until-wakeup)
  (call-with-values
   (lambda ()
     (cond ((not time-until-wakeup)
	    (values -1 #f))
	   ((< time-until-wakeup one-day-of-milliseconds)
	    (values time-until-wakeup #f))
	   (else
	    (values (quotient time-until-wakeup 60000)
		    #t))))
   (structure-ref primitives wait)))

(define deadlock-handler (make-session-data-slot! #f))

(define (call-when-deadlocked! thunk)
  (session-data-set! deadlock-handler thunk))

(define (spawn-output-forcers add-thread! others-waiting?)
  (let ((thunks (output-port-forcers others-waiting?)))
    (cond ((null? thunks)
	   #f)
	  (else
	   (for-each (lambda (thunk)
		       (add-thread! thunk 'forcing-output))
		     thunks)
	   #t))))
	 
(define unspecific (structure-ref primitives unspecific))
