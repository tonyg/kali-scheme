; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Threads.

; This is based on Haynes et al's  engines.
;
; The fundamental operation is (RUN <thread> <time> <arguments>), which
; runs the thread for the given amount of time, passing <arguments> to its
; continuation.
;
; Each thread has:
;   saved continuation
;   saved interrupt mask
;   scheduler, which is the thread that RUNs this one
;   remaining time in clock ticks ('waiting = waiting for events)
;   list of queue entries that are holding this thread
;   dynamic environment
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
  (really-make-thread dynamic-env continuation scheduler queues
		      events current-task uid name)
  thread?
  (dynamic-env    thread-dynamic-env)  ;Must be first!  (See fluid.scm)
  (continuation   thread-continuation set-thread-continuation!)
  (queues         thread-queues set-thread-queues!)
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
				    (thunk->continuation
				     (thread-top-level thunk))
				    (current-thread) ; scheduler
				    '()              ; queues
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

; Return a continuation that will call THUNK with continuation CONT.
; Synopsis: we grab the current continuation, install the continuation
; we want to create, and then at the last minute save the new continuation
; and return it to the one we grabbed on entry.

;(define (compose-continuation thunk cont)
;  (primitive-cwcc               ; grab the current continuation so that
;   (lambda (k)                  ;    we can return
;     (with-continuation         ; install CONT or an empty continuation
;       (or cont (loophole :escape #f))
;       (lambda ()
;         (primitive-cwcc        ; grab a continuation that will call THUNK and
;          (lambda (k2)          ;    then return to the installed continuation
;            (with-continuation  ; return the thunk-calling continuation to
;              k                 ;    the continuation we grabbed on entry
;              (lambda () k2))))
;         (thunk))))))           ; we reach here when continuation K2 is invoked

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
              (with-continuation ; return the thunk-calling continuation to
               k                 ;    the continuation we grabbed on entry
               (lambda () k2)))))
	 proc))))))
      
;----------------

; Rename the (doubly-)linked-queue operations as thread-specific ones.
; Doubly linked queues allow constant time deletion, which matters when
; threads block on multiple queues.

(define make-thread-queue (structure-ref linked-queues make-queue))
(define thread-queue-empty? (structure-ref linked-queues queue-empty?))
(define thread-queue-head (structure-ref linked-queues queue-head))

(define (enqueue-thread! queue thread . stuff)
  (let ((entry ((structure-ref linked-queues enqueue!) queue
						       (if (null? stuff)
							   thread
							   (cons thread stuff)))))
    (set-thread-queues! thread (cons entry (thread-queues thread)))))

(define (exclusively-enqueue-thread! queue thread stuff)
  (if (not (null? (thread-queues thread)))
      (error "enqueued thread being added to exclusive queue" thread queue))
  (let ((entry ((structure-ref linked-queues enqueue!) queue (cons thread stuff))))
    (set-thread-queues! thread (cons entry (thread-queues thread)))))

(define (dequeue-thread! queue)
  (let ((thread ((structure-ref linked-queues dequeue!) queue)))
    (remove-thread-from-queues! (if (pair? thread) (car thread) thread))
    thread))

(define (remove-thread-from-queues! thread)
  (for-each (structure-ref linked-queues delete-queue-entry!)
	    (thread-queues thread))
  (set-thread-queues! thread '()))

;----------------

(define *time-limit* #f)      ; how long we are currently allowed to run

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

; (RUN <thread> <time> <return-values>) -> <time-left> <event-type> . <stuff>
;
; Run <thread> for no more than <time>, passing <return-values> to its current
; continuation.  The call returns when the thread stops, returning the remaining
; time, the reason the thread stopped, and any addition information relating
; to the reason.  Times are in milliseconds.
;
; What this does:
; 1. Check that THREAD is runnable, that it belongs to the current thread,
;    and that it can accept RETURN-VALUES.
; 2. Return immediately if an event is pending.
; 3. Otherwise suspend the current thread, make THREAD its task, and then
;    run THREAD (or the thread that it is running or ...)

(define (run thread time return-values)
  (disable-interrupts!)
  (let ((scheduler (current-thread)))
    (if (thread-continuation thread)
	(if (eq? (thread-scheduler thread) scheduler)
	    (cond ((and (thread-current-task thread)
			(not (null? return-values)))
		   (enable-interrupts!)
		   (error "returning values to running thread" thread values))
		  ((event-pending?)
		   (if (not (null? return-values))
		       (return-values-to-thread! thread return-values))
		   (enable-interrupts!)
		   (apply values time (dequeue! (thread-events (current-thread)))))
		  (else
		   (primitive-cwcc
		    (lambda (cont)
		      (set-thread-continuation! scheduler cont)
		      (set-thread-current-task! scheduler thread)
		      (set-thread-time! thread time)
		      (run-next-thread scheduler thread return-values)))))
	    (begin
	      (enable-interrupts!)
	      (error "thread run by wrong scheduler" thread scheduler)))
	(begin
	  (enable-interrupts!)
	  (error "RUN called with a completed thread" thread)))))

; Have THREAD receive RETURN-VALUES when it resumes.

(define (return-values-to-thread! thread return-values)
  (set-thread-continuation!
    thread
    (compose-continuation
      (lambda ()
        (apply values return-values))
      (thread-continuation thread))))

; Find the thread at the end of the list of running threads, and the least
; remaining time of any currently running thread.  Starting from the current
; thread we first go up the THREAD-SCHEDULER pointers looking for the lowest
; time, and then go down the THREAD-TASK pointers checking times and finding
; the thread at the bottom.
;
; Once we have found the thread at the end of the chain we schedule an
; interrupt and start running the thread.

(define (run-next-thread scheduler new-thread return-values)
  (let loop ((scheduler scheduler) (time (thread-time new-thread)))
    (let ((next (thread-scheduler scheduler)))
      (if next
	  (loop next (min time (thread-time scheduler)))
	  (let loop ((thread new-thread) (time time))
	    (let ((next (thread-current-task thread)))
	      (if next
		  (loop next (min time (thread-time next)))
		  (begin
		    (set! *time-limit* time)
		    (schedule-interrupt! time)
		    (switch-to-thread thread return-values)))))))))

; Fast, binary version of MIN

(define (min x y)
  (if (< x y) x y))
    
; This assumes that THREAD has already been linked into the list of
; running threads.

(define (switch-to-thread thread return-values)
  (set-current-thread! thread)
  (set-thread-current-task! thread #f)
  (with-continuation (thread-continuation thread)
      (lambda ()
	(set-thread-continuation! thread #F)	; HCC: for GC
	(enable-interrupts!)
	(apply values return-values))))

; Debit the times of all running threads, set a new timer interrupt, and
; switch to a new thread if some thread has run out of time.  The only time
; there is no new time limit is when the new thread is the top thread.

(define (handle-timer-interrupt interrupted-template ei)
  (if (thread-scheduler (current-thread))
      (call-with-values
       (lambda ()
	 (debit-thread-times! *time-limit*))
       (lambda (time-limit new-thread)
	 (if new-thread
	     (begin
	       (if time-limit
		   (begin
		     (set! *time-limit* time-limit)
		     (schedule-interrupt! time-limit)))
	       (primitive-cwcc
		(lambda (cont)
		  (set-thread-continuation! (current-thread) cont)
		  (switch-to-thread new-thread
				    (list 0 (enum event-type out-of-time)))))))))))

; Loop up the running threads subtracting the latest running time from
; each one.  Returns the scheduler of highest thread that has run out of
; time, along with the remaining time on all threads above the lapsed one.
; If no thread is out of time the lowest remaining time is returned.

(define (debit-thread-times! amount)
  (let loop ((thread (current-thread)) (time #f) (next-to-run #f))
    (let ((scheduler (thread-scheduler thread)))
      (if (not scheduler)              ; THREAD is the top thread
	  (values time next-to-run)
	  (let ((time-left (- (thread-time thread) amount)))
	    (set-thread-time! thread time-left)
	    (if (>= 0 time-left)
		(loop scheduler #f scheduler)
		(loop scheduler
		      (if (and time (< time time-left))
			  time
			  time-left)
		      next-to-run)))))))

; Enqueue a RUNNABLE for THREAD's scheduler.

(define (make-ready thread . args)
  (if (not (null? (thread-queues thread)))
      (error "trying to schedule a queued thread" thread))
  (if (thread-scheduler thread)
      (apply schedule-event
	     (thread-scheduler thread)
	     (enum event-type runnable)
	     thread
	     args)))

; (SUSPEND <reason> <stuff>) stops the current thread and returns from
; the call to RUN that invoked it.  The arguments passed to SUSPEND become
; the return values of the call to RUN.  SUSPEND itself returns the arguments
; passed to RUN when the stopped thread is restarted.

(define (suspend reason stuff)
  (disable-interrupts!)
  (suspend-to (thread-scheduler (current-thread)) (cons reason stuff)))

; Stop running the current thread and return from the RUN call in
; SCHEDULER with the given reason.

(define (suspend-to scheduler event)  ; called with interrupts disabled
  (primitive-cwcc
   (lambda (cont)
     (set-thread-continuation! (current-thread) cont)
     (switch-to-thread scheduler
		       (cons (- *time-limit* (interrupt-timer-time))
			     event)))))

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
		    (lambda ()
		      (suspend (enum event-type killed) '()))))

; Wait for something to happen.  If an event is pending we return immediately.
; If the current thread has a scheduler (i.e. if it isn't the top thread) we
; block to allow other threads to run.  If the current thread is the top thread
; then there is nothing to do so we tell the VM to stop executing until the
; next external event.

(define (wait)
  (let ((interrupts (set-enabled-interrupts! no-interrupts)))
    (let ((thread (current-thread)))
      (if (not (and (thread-events thread)
		    (not (queue-empty? (thread-events thread)))))
	  (begin
	    (set-thread-time! thread 'waiting)
	    (block))))
    (set-enabled-interrupts! interrupts)))

; Is THREAD waiting for something to happen.

(define (waiting? thread)
  (eq? (thread-time thread) 'waiting))

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
  (let ((events (thread-events (current-thread))))
    (if (or (not events)
	    (queue-empty? events))
	(enum event-type no-event)
	(apply values (dequeue! events)))))

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
	   (return-values-to-thread! thread
				     (cons (thread-time (thread-current-task thread))
					   event))
	   (set-thread-current-task! thread #f))
	  (else
	   (add-event! thread event)))
    (set-enabled-interrupts! interrupts)
    ((structure-ref primitives unspecific))))

; Make THREAD's scheduler aware of the fact that THREAD is runnable, and
; similarly for its own scheduler and so on up the tree.

(define (schedule-wakeup thread)
  (remove-thread-from-queues! thread)
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
  (apply message "Running:" (do ((e (current-thread) (thread-scheduler e))
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

; This is an I/O routine we are handed on startup and is used for
; writing debugging messages.  Getting it more directly would introduce
; a circular module dependency.

(define message)

; Install our own handler for timer interrupts and then start running threads.

(define (with-threads m thunk)
  (set! message m)
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
    (lambda (exit)
      (with-handler
       (lambda (c punt)
	 (if (deadlock? c) (exit 0) (punt)))
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
		   (set-current-thread! thread)
		   (session-data-set! root-scheduler-slot thread)
		   ;; Interrupts were turned off by START-THREADS
		   (enable-interrupts!)
		   ;; EXIT to avoid the SUSPEND below because we have no one
		   ;; to suspend to
		   (exit (thunk)))))))
	 ;; land here when terminating a thread
	 (suspend (enum event-type completed) '()))))))

(define-condition-type 'deadlock '())
(define deadlock? (condition-predicate 'deadlock))

(define-condition-type 'terminate '())
(define terminate? (condition-predicate 'terminate))

(define (terminate-current-thread)
  (signal 'terminate)
  (error "can't terminate current thread")
  0)    ; suppress bogus compiler warning


(define root-scheduler-slot (make-session-data-slot! #f))

(define (root-scheduler)
  (session-data-ref root-scheduler-slot))


