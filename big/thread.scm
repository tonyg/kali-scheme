; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Multitasking

; A thread has:
;   - dynamic environment
;   - continuation: #f if the thread is current or finished,
;     a primitive continuation otherwise.
;   - env-for-spawn - initial dynamic environment for threads spawned
;     by this thread
;   - run status (active, stopped, done) - used only for stop & start
;     operations
;   - uid - no semantic content.

;   Date: Mon, 31 Aug 92 18:17:23 -0400
;   From: "Guillermo J. Rozas" <jinx@martigny.ai.mit.edu>
;
;   1: Instead of requesting a periodic timer interrupt from the system,
;   request one-shot interrupts, and re-request at the end of each throw
;   (in the argument to within-continuation).  Thus the time to "task
;   switch" becomes "system time" and each thread gets the full time
;   slice.


; Threads

(define-record-type thread thread-type
  (really-make-thread dynamic-env
		      continuation env-for-spawn
		      status queue uid name)
  thread?
  (dynamic-env   thread-dynamic-env)  ;Must be first!  (See fluid.scm)
  (continuation  thread-continuation set-thread-continuation!)
  (env-for-spawn thread-env-for-spawn)
  (status	 thread-status set-thread-status!)
  (queue	 thread-queue set-thread-queue!)
  (uid           thread-uid)
  (name		 thread-name)) ;for debugging

(define *thread-uid* 0)

(define (make-thread thunk env-for-spawn name)
  (let ((thread (really-make-thread env-for-spawn
				    (compose-continuation
				     (lambda (ignore)
				       (thread-top-level thunk))
				     #f)
				    env-for-spawn
				    'stopped
				    runnable-threads
				    *thread-uid*
				    name)))
    (set! *thread-uid* (+ *thread-uid* 1))
    thread))

(define (thread-top-level thunk)
  (set-enabled-interrupts! all-interrupts)
  (thunk)
  (finish-current-thread))

(define (finish-current-thread)
  (set-enabled-interrupts! no-interrupts)
  (set-thread-status! (current-thread) 'done)
  (schedule-thread (another-thread)))

; Spawn a new thread

(define (spawn thunk . name-option)
  (let* ((name (if (null? name-option) #f (car name-option)))
	 (thread (make-thread thunk
			      (thread-env-for-spawn (current-thread))
			      name)))
    (start-thread thread)
    thread))


; --------------------
; Scheduler

(define runnable-threads (make-queue))

(define (make-ready! thread)
  (move-to-queue thread runnable-threads))

(define (handle-timer-interrupt ei)
  ;; Interrupts are disabled at this point
  (schedule-interrupt *quantum*)	;Allow another one to come along
  ;; (dispatch)
  (make-ready! (current-thread))
  (switch-to-thread (another-thread)) ;(suspend-this-thread)
  (set-enabled-interrupts! ei))

(define (suspend-this-thread)		;Call this with interrupts disabled
  (switch-to-thread (another-thread)))

(define (switch-to-thread thread)
  (primitive-cwcc
   (lambda (cont)
     (set-thread-continuation! (current-thread) cont)
     (schedule-thread thread))))

(define (schedule-thread thread)	;Call with interrupts disabled
  (set-current-thread! thread)
  (schedule-interrupt *quantum*)
  (let ((cont (thread-continuation (current-thread))))
    (set-thread-continuation! (current-thread) #f)
    (with-continuation cont unspecific)))

(define (unspecific) (if #f #f))

(define (another-thread)		;Call with interrupts disabled
  (if (queue-empty? runnable-threads)
      (begin (signal 'deadlock)
	     (error "no threads to run"))
      (dequeue runnable-threads)))

(define (dispatch)			;Interrupts disabled
  (make-ready! (current-thread))
  (suspend-this-thread))


(define (relinquish-timeslice)
  (with-interrupts-inhibited dispatch))

; --------------------
; Initialization

; (with-multitasking thunk) initializes for multitasking, then starts
; up a thread for the execution of thunk.  That thread and all others
; created will run in the dynamic context of the call to
; with-multitasking.  The call to with-multitasking finally returns
; only when the scheduler runs out of things to do.

(define interrupt/alarm (enum interrupt alarm))

(define (with-multitasking thunk)
  (set! runnable-threads (make-queue))
  (set! *quantum* (quotient one-second 2))
  (with-interrupts-inhibited
   (lambda ()
     (let ((swap! (let ((save handle-timer-interrupt))
		    (lambda ()
		      (schedule-interrupt 0) ;Turn off alarm
		      (let ((temp
			     (vector-ref interrupt-handlers interrupt/alarm)))
			(vector-set! interrupt-handlers interrupt/alarm save)
			(set! save temp))))))
       (dynamic-wind
	swap!
	(lambda ()
	  (call-with-current-continuation
	    (lambda (deadlock)
	      (with-handler (lambda (c punt)
			      (if (deadlock? c) (deadlock 0) (punt)))
		(lambda ()
		  (call-with-current-continuation
		    (lambda (terminate)
		      (with-handler (lambda (c punt)
				      (if (terminate? c)
					  (terminate 0)
					  (punt)))
			(lambda ()
			  ;; Schedule-thread never returns.
			  (schedule-thread (make-thread thunk
							(get-dynamic-env)
							'initial))))))
		  ;; Interrupts enabled here, probably
		  (finish-current-thread))))))
	swap!)))))

(define-condition-type 'terminate '())
(define terminate? (condition-predicate 'terminate))
(define-condition-type 'deadlock '())
(define deadlock? (condition-predicate 'deadlock))

(define *quantum* #f)

(define no-interrupts 0)

; --------------------
; Random operations: start, stop, kill, interrupt

(define (terminate-current-thread)
  (signal 'terminate)
  (error "can't terminate current thread"))

(define (start-thread thread)
  (if (thread? thread)
      (with-interrupts-inhibited
	(lambda ()
	  (if (eq? (thread-status thread) 'stopped)
	      (begin (set-thread-status! thread 'active)
		     (move-to-queue (current-thread) runnable-threads)
		     (switch-to-thread thread)
		     #t)
	      (eq? (thread-status thread) 'active))))
      (call-error "invalid argument" start-thread thread)))

(define (stop-thread thread)
  (if (thread? thread)
      (with-interrupts-inhibited
	(lambda ()
	  (if (eq? (thread-status thread) 'active)
	      (let ((q (thread-queue thread)))
		(if q (delete-from-queue! q thread))
		(set-thread-status! thread 'stopped)
		(if (eq? thread (current-thread))
		    (suspend-this-thread))
		#t)
	      (eq? (thread-status thread) 'stopped))))
      (call-error "invalid argument" stop-thread thread)))

(define (kill-thread thread)
  (interrupt-thread thread terminate-current-thread))

(define (interrupt-thread thread thunk)
  (if (thread? thread)
      (with-interrupts-inhibited
	(lambda ()
	  (cond ((eq? thread (current-thread))
		 (thunk))
		((stop-thread thread)
		 (set-thread-queue! thread runnable-threads)
		 (set-thread-continuation!
		  thread
		  (compose-continuation
		   (lambda (ignore)
		     (thunk))
		   (or (thread-continuation thread)
		       (error "this shouldn't happen" thread))))
		 (start-thread thread))
		(else #f))))
      (call-error "invalid argument" interrupt-thread thread)))

(define (move-to-queue thread q)
  (set-thread-queue! thread q)
  (enqueue q thread))

; --------------------
; Locks (= semaphores)

(define-record-type lock lock-type
  (really-make-lock owner queue uid)
  lock?
  (owner lock-owner set-lock-owner!)
  (queue lock-queue)
  (uid lock-uid))

(define *lock-uid* 0)

(define (make-lock)
  (let ((uid *lock-uid*))
    (set! *lock-uid* (+ uid 1))
    (really-make-lock #f (make-queue) uid)))

(define (obtain-lock lock)		;Interrupts enabled
  (if (lock? lock)
      (with-interrupts-inhibited
	(lambda ()
	  (let loop ()
	    (if (let ((owner (lock-owner lock)))
		  (and owner
		       (not (eq? owner (current-thread)))
		       (not (eq? (thread-status owner) 'done))))
		(begin (move-to-queue (current-thread)
				      (lock-queue lock))
		       (suspend-this-thread)
		       (loop))
		(set-lock-owner! lock (current-thread))))))
      (call-error "invalid argument" obtain-lock lock)))

(define (release-lock lock)		;Interrupts enabled
  (if (eq? (lock-owner lock) (current-thread))
      (with-interrupts-inhibited
	(lambda ()
	  (set-lock-owner! lock #f)
	  (if (not (queue-empty? (lock-queue lock)))
	      (begin (make-ready! (current-thread))
		     (switch-to-thread (dequeue (lock-queue lock)))))))))

(define (with-lock lock thunk)		;Interrupts enabled
  (if (eq? (lock-owner lock) (current-thread))
      (thunk)
      (dynamic-wind (lambda () (obtain-lock lock))
		    thunk
		    (lambda () (release-lock lock)))))


; --------------------
; Condition variables

(define (make-condvar)
  (cons (make-queue) #f))
(define (condvar? x) (and (pair? x) (queue? (car x))))
(define condvar-queue car)  ; #f means variable has been set
(define condvar-value cdr)

(define (condvar-ref condvar)		;Interrupts enabled
  (if (condvar? condvar)
      (with-interrupts-inhibited
	(lambda ()
	  (let loop ()
	    (if (condvar-queue condvar)
		(begin (move-to-queue (current-thread)
				      (condvar-queue condvar))
		       (suspend-this-thread)
		       (loop))
		(condvar-value condvar)))))
      (call-error "invalid argument" condvar-ref condvar)))

(define (condvar-set! condvar value)	;Interrupts enabled
  (if (condvar? condvar)
      (if (with-interrupts-inhibited
	    (lambda ()
	      (let ((q (condvar-queue condvar)))
		(if q
		    (begin (set-cdr! condvar value)
			   (set-car! condvar #f)
			   (if (queue-empty? q)
			       #f
			       (let ((first (dequeue q)))
				 (let loop ()
				   (if (queue-empty? q)
				       (begin (make-ready! (current-thread))
					      (switch-to-thread first)
					      #f)
				       (begin (make-ready! (dequeue q))
					      (loop)))))))
		    ;; OK to store the same value twice.
		    (not (eq? value (condvar-value condvar)))))))
	  (error "condition variable is already assigned"
		 (condvar-value condvar) value))
      (call-error "invalid argument" condvar-set! condvar value)))

(define (kill-condvar condvar)
  (for-each kill-thread (with-interrupts-inhibited
			  (lambda ()
			    (if (condvar-queue condvar)
				(queue->list (condvar-queue condvar))
				'())))))


; --------------------
; Input

(define (thread-read-char port)
  (with-interrupts-inhibited
    (lambda ()
      (let loop ()
	(if (char-ready? port)
	    (read-char port)
	    (begin (dispatch)
		   (loop)))))))

(define (thread-peek-char port)
  (with-interrupts-inhibited
    (lambda ()
      (let loop ()
	(if (char-ready? port)
	    (peek-char port)
	    (begin (dispatch)
		   (loop)))))))

; --------------------
; Silliness

(define (active-threads)		;debugging utility
  (with-interrupts-inhibited
    (lambda ()
      (queue->list runnable-threads))))


(define-record-discloser thread-type
  (lambda (thread)
    (cons 'thread
	  (cons (thread-uid thread)
		(let ((name (thread-name thread)))
		  (if name
		      (list name)
		      '()))))))

;(put 'with-interrupts-inhibited 'scheme-indent-hook 0)
;(put 'with-handler 'scheme-indent-hook 1)
