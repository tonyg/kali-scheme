; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A parameterized scheduler.

; (run-threads event-handler) -> unspecific
;   (event-handler thread time-left event event-data) -> [thread args time]
; A bogus BLOCKED event is passed to the handler to get the initial thread.

(define (run-threads event-handler)
  (call-with-values
   (lambda ()
     (event-handler #f 0 (enum event-type blocked) '()))
   (lambda (thread time)
     (if thread
	 (let loop ((thread thread) (time time))
	   (call-with-values
	    (lambda ()
	      (run thread time))
	    (lambda (time-left event . event-data)
	      (call-with-values
	       (lambda ()
		 (event-handler thread time-left event event-data))
	       (lambda (thread time)
		 (if thread
		     (loop thread time)))))))))))
	    
; Same thing, with the addition of a housekeeping thunk that gets
; run periodically.

(define (run-threads-with-housekeeper event-handler housekeeper delay)
  (call-with-values
   (lambda ()
     (event-handler #f 0 (enum event-type blocked) '()))
   (lambda (thread time)
     (if thread
	 (let loop ((thread thread) (time time) (hk-time delay))
	   (call-with-values
	    (lambda ()
	      (run thread time))
	    (lambda (time-left event . event-data)
	      (let ((hk-time (let ((temp (- hk-time (- time time-left))))
			       (if (<= temp 0)
				   (begin
				     (housekeeper)
				     delay)
				   temp))))
		(call-with-values
		 (lambda ()
		   (event-handler thread time-left event event-data))
		 (lambda (thread time)
		   (if thread
		       (loop thread time hk-time))))))))))))

; An event-handler that does round-robin scheduling.
; Arguments:
;    runnable         ; queue of threads
;    quantum          ; number of ticks each thread gets
;    dynamic-env      ; initial dynamic environments for new threads
;    thread-count     ; counter tracking the number of threads
;    event-handler : event-type event-data -> handled?
;    upcall-handler : thread token . args -> return-values
;    wait             ; thunk returns #t if scheduling is to continue

(define (round-robin-event-handler runnable quantum dynamic-env thread-count
				   event-handler upcall-handler wait)

  (define (thread-event-handler thread time-left event event-data)
    (enum-case event-type event

      ;; the thread stops, either temporarily or permanently
      ((blocked)
       (next-thread))
      ((completed killed)
       (decrement-counter! thread-count)
       (next-thread))
      ((out-of-time)
       (enqueue! runnable thread)
       (next-thread))

      ;; the thread keeps running
      ((upcall)
       (call-with-values
	(lambda ()
	  (apply upcall-handler event-data))
	(lambda results
	  (set-thread-arguments! thread results)
	  (values thread time-left))))
      (else
       (asynchronous-event-handler event event-data)
       (values thread time-left))))

  ;; We call EVENT-HANDLER first so that it can override the default behavior
  (define (asynchronous-event-handler event event-data)
    (or (event-handler event event-data)
	(enum-case event-type event
	  ((runnable)
	   (enqueue! runnable (car event-data)))
	  ((spawned)
	   (increment-counter! thread-count)
	   (let ((thread (car event-data)))
	     (set-thread-dynamic-env! thread dynamic-env)
	     (set-thread-scheduler! thread (current-thread))
	     (enqueue! runnable thread)))
	  ((no-event)
	   (values))
	  (else
	   (error "unhandled event"
		  (cons (enumerand->name event event-type)
			event-data)
		  event-handler)))))

  (define (next-thread)
    (if (queue-empty? runnable)
	(call-with-values
	  get-next-event!
	  (lambda (event . data)
	    (cond ((not (eq? event (enum event-type no-event)))
		   (asynchronous-event-handler event data)
		   (next-thread))
		  ((wait)
		   (next-thread))
		  (else
		   (values #f 0)))))
	(values (dequeue! runnable)
		quantum)))

  thread-event-handler)

; Simple counting cell

(define (make-counter)
  (list 0))

(define counter-value car)

(define (increment-counter! count)
  (set-car! count (+ 1 (car count))))

(define (decrement-counter! count)
  (set-car! count (- (car count) 1)))
