; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Threads.

; This is inspired by Haynes et al's  engines.
;
; The fundamental operation is (RUN <thread> <time>), which runs the thread
; for the given amount of time.
;
; Each thread has:
;   saved continuation
;   saved interrupt mask
;   scheduler, which is the thread that RUNs this one
;   remaining time in clock ticks ('waiting = waiting for events)
;   queue that is holding this thread, if any
;   arguments waiting to be passed to the thread when it is next run
;   dynamic environment
;   dynamic point
;   whatever data the scheduler wants
; Schedulers also have:
;   list of pending events
;   thread that this scheduler is currently running
;
; A `scheduler' is any thread that has called RUN.  All threads are organized
; into a tree by the THREAD-SCHEDULER field, with the pointers pointing from
; the leaves to the root.
;
; There is a doubly linked list of running threads linked by the
; the thread-scheduler and thread-current-task fields.
;   e1 <-> e2 <-> ... <-> eN-1 <-> eN
; e1 is the top thread and eN is the thread whose code is actually running.
; All except eN are in the middle of a call to RUN.  The scheduler pointers
; point to the left and the current-task pointers point to the right.
;
; When an thread stops, its scheduler is run and the portion of the list from
; the stopped thread down is saved for when the stopped thread is resumed.  For
; example, suppose e3 runs out of time.  Then the list is shortened to
; e1 <-> e2 and the e3 <-...-> eN portion is saved.  When e3 is resumed,
; the list is spliced back together and eN's continuation is resumed.

(define-record-type thread :thread
  (really-make-thread dynamic-env dynamic-point continuation scheduler
		      queue arguments
		      events current-task uid name)
  thread?
  (dynamic-env    thread-dynamic-env)   ;Must be first!  (See fluid.scm)
  (dynamic-point  thread-dynamic-point set-thread-dynamic-point!)
  					;Must be second!  (See fluid.scm)
  (continuation   thread-continuation set-thread-continuation!)
  (queue          thread-queue set-thread-queue!)
  (arguments      thread-arguments set-thread-arguments!)
  (time           thread-time set-thread-time!)
  (scheduler      thread-scheduler set-thread-scheduler!)
  (data           thread-data set-thread-data!)
  (events         thread-events set-thread-events!)             ; schedulers
  (current-task   thread-current-task set-thread-current-task!) ; schedulers
  (uid            thread-uid)       ; for debugging (and as a cheap weak pointer)
  (name		  thread-name))     ; for debugging

(define-record-discloser :thread
  (lambda (thread)
    (cons 'thread
	  (cons (thread-uid thread)
		(let ((name (thread-name thread)))
		  (if name
		      (list name)
		      '()))))))

(define *thread-uid* 0)

(define (make-thread thunk dynamic-env name)
  (let ((thread (really-make-thread dynamic-env
				    #f               ; dynamic-point root
				    (thunk->continuation
				     (thread-top-level thunk))
				    (current-thread) ; scheduler
				    #f               ; queue
				    '()              ; arguments
				    #f               ; events
				    #f               ; current-task
				    *thread-uid*
				    name)))
    (set! *thread-uid* (+ *thread-uid* 1))
    thread))

;----------------
; Call THUNK and then suspend.

(define (thread-top-level thunk)
  (lambda ()
    (call-with-values thunk
		      (lambda values
			(suspend (enum event-type completed) values)))))

; Find the thread with the indicated uid.  This is expensive.  It is used
; by rts/channel-port.scm to when forcibly unlocking one of the REPL's ports.

(define (thread-uid->thread uid)
  (let ((threads (all-threads)))
    (let loop ((i 0))
      (cond ((= i (vector-length threads))
	     #f)
	    ((= uid (thread-uid (vector-ref threads i)))
	     (vector-ref threads i))
	    (else
	     (loop (+ i 1)))))))

(define (all-threads)
  ((structure-ref primitives find-all-records) :thread))

; Add EVENT to THREAD's event queue.
; Called with interrupts disabled.

(define (add-event! thread event)
  (enqueue! (or (thread-events thread)
		(let ((q (make-queue)))
		  (set-thread-events! thread q)
		  q))
	    event))

; A bit of magic courtesy of JAR.  We need to use PRIMITIVE-CWCC to save
; thread continuations because CALL-WITH-CURRENT-CONTINUATION saves the
; dynamic state, including the current thread.  PRIMITIVE-CWCC's continuations
; are not procedures, so we need this thing to convert a thread's initial
; thunk into a continuation.

; (Alternatively, we could make a version of CWCC that didn't save the
; dynamic state.  That would slow down context switching, which is
; presumably more frequent than thread creation.)

(define (thunk->continuation thunk)
  (compose-continuation thunk #f))

; Return a continuation that will call PROC with continuation CONT.
; Synopsis: we grab the current continuation, install the continuation
; we want to create, and then at the last minute save the new continuation
; and return it to the one we grabbed on entry.

(define (compose-continuation proc cont)
  (primitive-cwcc                ; grab the current continuation so that
   (lambda (k)                   ;    we can return
     (with-continuation          ; install CONT or an empty continuation
      (or cont (loophole :escape #f))
      (lambda ()
        (call-with-values        ; install PROC as a continuation
         (lambda ()
           (primitive-cwcc       ; grab a continuation that will call PROC and
            (lambda (k2)         ;    then return to the installed continuation
              (with-continuation ; return the PROC-calling continuation to
               k                 ;    the continuation we grabbed on entry
               (lambda () k2)))))
	 proc))))))
      
;----------------
; Enqueueing and dequeuing threads.

; Rename the queue operations as thread-specific ones (both for clarity
; and because we will want to use priority queues in the future).

(define make-thread-queue make-queue)
(define thread-queue-empty? queue-empty?)

(define (enqueue-thread! queue thread)
  (if (thread-queue thread)
      (error "enqueued thread being added to another queue" thread queue))
  (set-thread-queue! thread queue)
  (enqueue! queue thread))

(define (dequeue-thread! queue)
  (let ((thread (dequeue! queue)))
    (set-thread-queue! thread #f)
    thread))

(define (remove-thread-from-queue! thread)
  (if (thread-queue thread)
      (begin
	(delete-from-queue! (thread-queue thread) thread)
	(set-thread-queue! thread #f))))

;----------------

(define current-thread (structure-ref primitives current-thread))
(define set-current-thread! (structure-ref primitives set-current-thread!))

; Return values for RUN.

(define-enumeration event-type
  ;; Reason        Additional return values
  (
  ;; events relating to the running thread
   out-of-time
   completed      ; . <results>       
   blocked
   killed 
   upcall         ; <args>              unknown upcalls are passed up

  ;; asynchronous events
   runnable       ; <thread> <args>     <thread> is now runnable
   spawned        ; <thunk> <id> ...    spawn <thunk> as a new thread
   interrupt      ; <type> . <stuff>    an interrupt has occured
   deadlock       ;                     no one can run
 
   no-event       ;                     there are no pending events
   ))

; DEADLOCK is used by the REPL to gain control when the thread system deadlocks.

; (RUN <thread> <time>) -> <time-left> <event-type> . <stuff>
;
; Run <thread> for no more than <time>.  The call returns when the thread
; stops, returning the remaining time, the reason the thread stopped, and
; any addition information relating to the reason.  Times are in milliseconds.
;
; What this does:
; 1. Check that THREAD is runnable, that it belongs to the current thread,
;    and that it can accept any values being returned.
; 2. Return immediately if an event is pending.
; 3. Otherwise suspend the current thread, make THREAD its task, and then
;    run THREAD (or the thread that it is running or ...)

(define (run thread time)
  (disable-interrupts!)
  (let ((scheduler (current-thread)))
    (cond ((not (thread-continuation thread))
	   (enable-interrupts!)
	   (error "RUN called with a completed thread" thread))
	  ((not (eq? (thread-scheduler thread) scheduler))
	   (enable-interrupts!)
	   (error "thread run by wrong scheduler" thread scheduler))
	  ((thread-queue thread)
	   (enable-interrupts!)
	   (error "thread run while still on a queue" thread))
	  ((and (thread-current-task thread)
		(not (null? (thread-arguments thread))))
	   (enable-interrupts!)
	   (error "returning values to running thread"
		  thread
		  (thread-arguments thread)))
	  ((event-pending?)
	   (enable-interrupts!)
	   (apply values time (dequeue! (thread-events (current-thread)))))
	  (else
	   (set-thread-current-task! scheduler thread)
	   (find-and-run-next-thread thread time)))))

; The next thread to run is the scheduler of the highest thread in the chain
; with no time left or, if there is no such thread, the bottom thread in the
; chain.  The time limit is the minimum of the remaining times of threads
; above the thread to be run.
;
; We first go down from the user-provided thread, looking for a thread with
; no time left.  We then continue either with that thread's scheduler or with
; the bottom thread of the chain.

; This could be modified to add the current time to NEW-THREAD and any threads
; below.  Then the old time limit could be reused if none of the new threads
; got less time than SCHEDULER and above.  This is slower and simpler.

(define (find-and-run-next-thread new-thread time)
  (set-thread-time! new-thread time) ; in case we don't run it now
  (let loop ((thread new-thread) (time time))
    (let ((next (thread-current-task thread)))
      (if (and next
	       (< 0 (thread-time next)))
	  (loop next (min time (thread-time next)))
	  (debit-thread-times-and-run! thread time)))))

; Debit the times of all threads from the current-thread on up.  If we find a
; thread with no time left, then that thread's scheduler becomes the potential
; next thread.
;
; Only the root thread can end up with a time-limit of #f, as all other threads
; have schedulers.

(define (debit-thread-times-and-run! next-to-run time-limit)
  (let ((elapsed (interrupt-timer-time)))
    (let loop ((thread (current-thread))
	       (time-limit time-limit)
	       (next-to-run next-to-run))
      (let ((scheduler (thread-scheduler thread)))
	(cond (scheduler
	       (let ((time-left (- (thread-time thread) elapsed)))
		 (set-thread-time! thread time-left)
		 (if (<= time-left 0)
		     (loop scheduler #f scheduler)
		     (loop scheduler
			   (if time-limit
			       (min time-limit time-left)
			       time-left)
			   next-to-run))))
	      (next-to-run
	       (run-next-thread next-to-run time-limit))
	      (else
	       (schedule-interrupt! time-limit)))))))

; Debit the times of all running threads and run whomever is next.

(define (handle-timer-interrupt interrupted-template ei)
  (if (thread-scheduler (current-thread))
      (debit-thread-times-and-run! #f #f)))

; Run the next thread, first scheduling an interrupt if the thread is not the
; root thread.  If the next thread has a current task, then the return values
; are the normal out-of-time values. The bottom thread of the chain gets
; whatever return values have been stashed earlier.

(define (run-next-thread thread time-limit)
  (if time-limit (schedule-interrupt! time-limit))
  (let ((arguments (if (thread-current-task thread)
		       (list 0 (enum event-type out-of-time))
		       (thread-arguments thread))))
    (set-thread-arguments! thread '())
    (switch-to-thread thread arguments)))

; Fast binary version of MIN

(define (min x y)
  (if (< x y) x y))
    
;----------------
; Save the current thread and start running NEW-THREAD.

(define (switch-to-thread thread return-values)
  (primitive-cwcc
   (lambda (cont)
     (set-thread-continuation! (current-thread) cont)
     (run-thread thread return-values))))

; Start running THREAD.  This assumes that THREAD has already been linked into
; the list of running threads.

(define (run-thread thread return-values)
  (set-current-thread! thread)
  (set-thread-current-task! thread #f)
  (with-continuation (thread-continuation thread)
    (lambda ()
      (set-thread-continuation! thread #f)	; HCC: for GC
      (enable-interrupts!)
      (apply values return-values))))

;----------------
; (SUSPEND <reason> <stuff>) stops the current thread and returns from
; the call to RUN that invoked it.  The arguments passed to SUSPEND become
; the return values of the call to RUN.  SUSPEND itself returns the arguments
; passed to RUN when the stopped thread is restarted.

(define (suspend reason stuff)
  (disable-interrupts!)
  (suspend-to (thread-scheduler (current-thread))
	      (cons reason stuff)))

; Stop running the current thread and return from the RUN call in
; SCHEDULER with the given reason.  We need to debit the time of every
; thread between the current one and SCHEDULER.

(define (suspend-to scheduler event)  ; called with interrupts disabled
  (debit-down! (thread-current-task scheduler))
  (switch-to-thread scheduler
		    (cons (thread-time (thread-current-task scheduler))
			  event)))

(define (debit-down! thread)  
  (let ((elapsed (interrupt-timer-time)))
    (let loop ((thread thread))
      (if thread
          (begin
            (set-thread-time! thread (- (thread-time thread) elapsed))
            (loop (thread-current-task thread)))))))

; Same thing, except that we don't save the current continuation and
; we don't need to debit the thread's time.  This is used for completed
; and killed threads and is not exported.

(define (exit reason stuff)
  (disable-interrupts!)
  (let ((thread (current-thread)))
    (set-thread-continuation! thread #f)
    (run-thread (thread-scheduler thread)
		(cons (- (thread-time thread)
			 (interrupt-timer-time))
		      (cons reason stuff)))))

; Wait for something to happen.  If an event is pending we return immediately.
; Another same thing, except that we have to be careful because we need to
; set the current thread's time field to a non-integer.

(define (wait)
  (let ((interrupts (set-enabled-interrupts! no-interrupts)))
    (let ((thread (current-thread)))
      (if (not (and (thread-events thread)
		    (not (queue-empty? (thread-events thread)))))
	  (let ((time-left (- (thread-time thread)
			      (interrupt-timer-time))))
	    (set-thread-time! thread 'waiting)
	    (switch-to-thread (thread-scheduler thread)
			      (list time-left (enum event-type blocked)))))
      (set-enabled-interrupts! interrupts))))

; Is THREAD waiting for something to happen.

(define (waiting? thread)
  (eq? (thread-time thread) 'waiting))

; Various calls to SUSPEND.

(define (block)
  (suspend (enum event-type blocked) '()))

(define (relinquish-timeslice)
  (suspend (enum event-type out-of-time) '()))

; Send the upcall to the current scheduler and check the return value(s)
; to see if it was handled properly.

(define (upcall token . args)
  (propogate-upcall (current-thread) token args))

; Send the upcall to the current thread's handler, if it has one.

(define (propogate-upcall thread token args)
  (if (thread-scheduler (current-thread))
      (suspend (enum event-type upcall) (list thread token args))
      (begin
	(interrupt-thread thread
			  (lambda ()
			    (apply error "unhandled upcall" token args)))
	(values))))

(define (kill-thread! thread)   ; dangerous!
  (interrupt-thread thread
		    (lambda ignored
		      (exit (enum event-type killed) '()))))

;----------------
; Make THREAD execute PROC the next time it is run.  The thread's own
; continuation is passed whatever PROC returns.

(define (interrupt-thread thread proc)
  (let ((interrupts (set-enabled-interrupts! no-interrupts)))
    (cond ((and (thread? thread)
		(not (running? thread)))
	   (set-thread-continuation!
	    thread
	    (compose-continuation proc (thread-continuation thread)))
	   (set-enabled-interrupts! interrupts))
	  (else
	   (set-enabled-interrupts! interrupts)
	   (call-error "invalid argument" interrupt-thread thread)))))
  
;----------------
; Dealing with event queues

; Returns the next event scheduled for the current thread.

(define (get-next-event!)
  (let* ((interrupts (disable-interrupts!))
	 (events (thread-events (current-thread))))
    (if (or (not events)
	    (queue-empty? events))
	(begin
	  (set-enabled-interrupts! interrupts)
	  (enum event-type no-event))
	(let ((event (dequeue! events)))
	  (set-enabled-interrupts! interrupts)
	  (apply values event)))))

(define (event-pending?)
  (let ((events (thread-events (current-thread))))
    (and events (not (queue-empty? events)))))

; Schedule the given event for THREAD.  If THREAD is waiting, a RUNNABLE
; event is also scheduled for its scheduler and so on up the tree.  If
; THREAD is running we suspend to allow the interrupt to be processed
; immediately (running threads that are not the current thread must be
; above the current thread in the tree and so have priority over it).

(define (schedule-event thread . event)
  (let ((interrupts (disable-interrupts!)))
    (cond ((eq? thread (current-thread))
	   (add-event! thread event)
	   (values))
	  ((waiting? thread)
	   (add-event! thread event)
	   (schedule-wakeup thread))
	  ((running? thread)
	   (suspend-to thread event))
	  ((thread-current-task thread)
	   (set-thread-arguments! thread
				  (cons (thread-time (thread-current-task thread))
					event))
	   (set-thread-current-task! thread #f))
	  (else
	   (add-event! thread event)))
    (set-enabled-interrupts! interrupts)
    ((structure-ref primitives unspecific))))

; Make THREAD's scheduler aware of the fact that THREAD is runnable, and
; similarly for its own scheduler and so on up the tree.
; Called with interrupts disabled.

(define (schedule-wakeup thread)
  (set-thread-time! thread 0)             ; clear WAITING flag
  (let ((scheduler (thread-scheduler thread)))
    (if scheduler
	(let ((event (list (enum event-type runnable) thread)))
	  (cond ((eq? scheduler (current-thread))
		 (add-event! scheduler event)
		 (values))
		((waiting? scheduler)
		 (add-event! scheduler event)
		 (schedule-wakeup scheduler))
		((running? scheduler)
		 (suspend-to scheduler event)))))))

; Debugging routine

(define (show-running)
  (apply user-message "Running:" (do ((e (current-thread) (thread-scheduler e))
				 (l '() (cons (thread-name e) (cons " " l))))
				((not e)
				 (reverse l)))))

; Walk up the scheduler pointers from the current thread to see if THREAD is
; running.

(define (running? thread)
  (let loop ((running (current-thread)))
    (cond ((not running)
	   #f)
	  ((eq? running thread)
	   #t)
	  (else
	   (loop (thread-scheduler running))))))

; Spawn a new thread either on the current thread's scheduler or on a specific
; scheduler.

(define (spawn thunk . id)
  (apply spawn-on-scheduler (thread-scheduler (current-thread)) thunk id))

(define (spawn-on-root thunk . id)
  (if (root-scheduler)
      (apply spawn-on-scheduler
	     (root-scheduler)
	     thunk
	     id)
      (thunk)))

(define (spawn-on-scheduler scheduler thunk . id)
  (schedule-event scheduler
		  (enum event-type spawned)
		  thunk
		  (if (null? id) #f (car id))))

; Enqueue a RUNNABLE for THREAD's scheduler.

(define (make-ready thread . args)
  (if (thread-queue thread)
      (error "trying to schedule a queued thread" thread))
;  (if (not (null? (thread-arguments thread)))
;      (error "trying to replace thread arguments"))
  (set-thread-arguments! thread args)
  (if (thread-scheduler thread)
      (schedule-event (thread-scheduler thread)
		      (enum event-type runnable)
		      thread)
      (error "MAKE-READY thread has no scheduler" thread)))

;----------------

(define (schedule-interrupt! time)
  (schedule-interrupt time))

(define (cancel-timer-interrupt!)
  (schedule-interrupt 0))

; Use CHEAP-TIME to save the cost of a system call on every context switch.
; This clock is reset whenever we request an interrupt.

(define (interrupt-timer-time)
  ((structure-ref primitives time) (enum time-option cheap-time) #f))

; Used for waking sleepers

(define (real-time)
  ((structure-ref primitives time) (enum time-option real-time) #f))

; Install our own handler for timer interrupts and then start running threads.

(define (with-threads thunk)
  (with-interrupts-inhibited
   (lambda ()
     (dynamic-wind
      (lambda ()
	(set-interrupt-handler! (enum interrupt alarm)
				handle-timer-interrupt))
      (lambda ()
	(start-multitasking thunk))
      (lambda ()
	(cancel-timer-interrupt!))))))

; Add handler for deadlock and termination and morph ourselves into the
; initial thread.

(define (start-multitasking thunk)
  (call-with-current-continuation
    (lambda (exit-multitasking)
      (with-handler
       (lambda (c punt)
	 (if (deadlock? c) (exit-multitasking 0) (punt)))
       (lambda ()
	 (call-with-current-continuation
	   (lambda (terminate)
	     (with-handler (lambda (c punt)
			     (if (terminate? c) (terminate 0) (punt)))
	       (lambda ()
		 (set! *thread-uid* 0)
		 (let ((thread (make-thread #f    ; thunk
					    (get-dynamic-env)
					    'initial-thread)))
		   (set-thread-scheduler! thread #f)
		   (set-thread-time! thread #f)
		   (set-thread-dynamic-point! thread (get-dynamic-point))
		   (set-current-thread! thread)
		   (session-data-set! root-scheduler-slot thread)
		   ;; Interrupts were turned off by START-THREADS
		   (enable-interrupts!)
		   ;; EXIT to avoid the SUSPEND below because we have no one
		   ;; to suspend to
		   (exit-multitasking (thunk)))))))
	 ;; land here when terminating a thread
	 (exit (enum event-type completed) '()))))))

; Raised when there is nothing to run.

(define-condition-type 'deadlock '())
(define deadlock? (condition-predicate 'deadlock))

; Raised when the current thread has been killed.

(define-condition-type 'terminate '())
(define terminate? (condition-predicate 'terminate))

; Kill the current thread.  USER-MESSAGE is used to try and make sure that some
; record exists when an error occured.  The system may be too broken for ERROR
; to work properly.

(define (terminate-current-thread)
  (signal 'terminate)
  (user-message "Can't terminate current thread "
		(thread-uid (current-thread))
		" "
		(thread-name (current-thread)))
  (error "can't terminate current thread")
  0)    ; suppress bogus compiler warning


;----------------
; A slot in the session data to hold the root thread.

(define root-scheduler-slot (make-session-data-slot! #f))

(define (root-scheduler)
  (session-data-ref root-scheduler-slot))


