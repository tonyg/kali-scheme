; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; A parameterized scheduler.

; (run-threads event-handler) -> unspecific
;   (event-handler thread time-left event event-data) -> [thread args time]
; A bogus BLOCKED event is passed to the handler to get the initial thread.

(define (run-threads event-handler)
  (call-with-values
   (lambda ()
     (event-handler #f 0 (enum event-type blocked) '()))
   (lambda (thread args time)
     (if thread
	 (let loop ((thread thread) (args args) (time time))
	   (call-with-values
	    (lambda ()
	      (run thread time args))
	    (lambda (time-left event . event-data)
	      (call-with-values
	       (lambda ()
		 (event-handler thread time-left event event-data))
	       (lambda (thread args time)
		 (if thread (loop thread args time)))))))))))

; Same thing, with the addition of a housekeeping thunk that gets
; run periodically.

(define (run-threads-with-housekeeper event-handler housekeeper delay)
  (call-with-values
   (lambda ()
     (event-handler #f 0 (enum event-type blocked) '()))
   (lambda (thread args time)
     (if thread
	 (let loop ((thread thread) (args args) (time time) (hk-time delay))
	   (call-with-values
	    (lambda ()
	      (run thread time args))
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
		 (lambda (thread args time)
		   (if thread
		       (loop thread args time hk-time))))))))))))

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
       (exclusively-enqueue-thread! runnable thread '())
       (next-thread))

      ;; the thread keeps running
      ((upcall)
       (call-with-values
	(lambda ()
	  (apply upcall-handler event-data))
	(lambda results
	  (values thread results time-left))))
      (else
       (asynchronous-event-handler event event-data)
       (values thread '() time-left))))

  ;; We call EVENT-HANDLER first so that it can override the default behavior
  (define (asynchronous-event-handler event event-data)
    (or (event-handler event event-data)
	(enum-case event-type event
	  ((runnable)
	   (exclusively-enqueue-thread! runnable (car event-data) (cdr event-data)))
	  ((spawned)
	   (increment-counter! thread-count)
	   (exclusively-enqueue-thread! runnable
					(make-thread (car event-data)
						     dynamic-env
						     (cadr event-data))
					'()))
	  ((no-event)
	   (values))
	  (else
	   (error "unhandled event"
		  (cons (enumerand->name event event-type)
			event-data)
		  event-handler)))))

  (define (next-thread)
    (if (thread-queue-empty? runnable)
	(call-with-values
	 get-next-event!
	 (lambda (event . data)
	   (cond ((not (eq? event (enum event-type no-event)))
		  (asynchronous-event-handler event data)
		  (next-thread))
		 ((wait)
		  (next-thread))
		 (else
		  (values #f '() 0)))))   ; scheduler quits
	(real-next-thread)))

  (define (real-next-thread)	       
    (let ((pair (dequeue-thread! runnable)))
      (values (car pair) (cdr pair) quantum)))

  thread-event-handler)

; Simple counting cell

(define (make-counter)
  (list 0))

(define counter-value car)

(define (increment-counter! count)
  (set-car! count (+ 1 (car count))))

(define (decrement-counter! count)
  (set-car! count (- (car count) 1)))
