; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Threads.

; This was inspired by Haynes et al's engines.
;
; The fundamental operation is (RUN <thread> <time>), which runs the thread
; for the given amount of time.
;
; Each thread has:
;   dynamic environment
;   dynamic point
;   current proposal
;   saved continuation (if not currently runnning)
;   state
;   scheduler, which is the thread that RUNs this one
;   remaining time in clock ticks ('waiting = waiting for events)
;   queue that is holding this thread, if any
;   arguments waiting to be passed to the thread when it is next run
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
  (really-make-thread dynamic-env dynamic-point proposal
		      continuation scheduler
		      cell arguments
		      events current-task uid name)
  thread?

  ; These first three fields hold dynamic data used by various VM opcodes.
  (dynamic-env    thread-dynamic-env set-thread-dynamic-env!)
					;Must be first!  (See fluid.scm)
  (dynamic-point  thread-dynamic-point set-thread-dynamic-point!)
  					;Must be second! (See fluid.scm)
  (proposal       thread-proposal)	;Must be third!  (See fluid.scm)

  ; The time remaining for a thread to run.
  (time           thread-time             set-thread-time!)

  ; The saved state of a non-running thread
  (continuation   thread-continuation     set-thread-continuation!)
  (arguments      thread-arguments        set-thread-arguments!)

  ; Used by the engine algorithm
  (scheduler      thread-scheduler        set-thread-scheduler!)
  (current-task   thread-current-task     set-thread-current-task!)
  (events         thread-events           set-thread-events!)

  ; Used by schedulers
  (data           thread-data             set-thread-data!)
  (cell           thread-cell             set-thread-cell!)

  ; For debugging
  (uid            thread-uid)       ; (also used as a cheap weak pointer)
  (name		  thread-name))

(define-record-discloser :thread
  (lambda (thread)
    (cons 'thread
	  (cons (thread-uid thread)
		(let ((name (thread-name thread)))
		  (if name
		      (list name)
		      '()))))))

(define *thread-uid* 0)

(define (make-thread thunk name)
  (let ((thread (really-make-thread #f		     ; dynamic-env
				    #f               ; dynamic-point root
				    #f		     ; proposal
				    (thunk->continuation
				     (thread-top-level thunk))
				    #f 		     ; scheduler
				    #f               ; cell
				    '()              ; arguments
				    #f               ; events
				    #f               ; current-task
				    *thread-uid*
				    name)))
    (set! *thread-uid* (+ *thread-uid* 1))
    thread))

;----------------
; Call THUNK and then suspend.  The LET is just to give the thunk a name
; in the debugger.  This thunk shows up at the bottom of every ,preview.

(define (thread-top-level thunk)
  (let ((thread-start (lambda ()
			(call-with-values
			  thunk
			  (lambda values
			    (suspend (enum event-type completed) values))))))
    thread-start))

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
  (find-all-records :thread))

; Add EVENT to THREAD's event queue.
; Called with interrupts disabled.

(define (add-event! thread event)
  (enqueue! (or (thread-events thread)
		(let ((q (make-queue)))
		  (set-thread-events! thread q)
		  q))
	    event))

(define (next-event! thread)
  (let ((queue (thread-events thread)))
    (if (and queue
	     (not (queue-empty? queue)))
	(dequeue! queue)
	#f)))

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
; Removing threads from queues and cells.
; These are utility procedures for users.  They are not used here.

(define (maybe-dequeue-thread! queue)
  (let loop ()
    (let ((cell (maybe-dequeue! queue)))
      (if cell
	  (or (provisional-cell-ref cell)
	      (loop))
	  #f))))

; Look for a non-empty cell.

(define (thread-queue-empty? queue)
  (ensure-atomicity
   (let loop ()
     (cond ((queue-empty? queue)
	    #t)
	   ((provisional-cell-ref (queue-head queue))
	    #f)
	   (else
	    (dequeue! queue)
	    (loop))))))

;----------------
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
	  ((thread-cell thread)
	   (enable-interrupts!)
	   (error "thread run while still blocked" thread))
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
; no time left or a thread that has an event queued.  We then continue either
; with that thread's scheduler or with the bottom thread of the chain.

; This could be modified to add the current time to NEW-THREAD and any threads
; below.  Then the old time limit could be reused if none of the new threads
; got less time than SCHEDULER and above.  This is slower and simpler.

(define (find-and-run-next-thread new-thread time)
  (set-thread-time! new-thread time) ; in case we don't run it now
  (let loop ((thread new-thread) (time time))
    (let ((next (thread-current-task thread)))
      (cond ((or (not next)
		 (< 0 (thread-time next)))
	     (debit-thread-times-and-run! thread time #f))
	    ((next-event! thread)
	     => (lambda (event)
		  (set-thread-current-task! thread #f)
		  (set-thread-arguments! thread (cons (thread-time next)
						      event))
		  (debit-thread-times-and-run! thread time #f)))
	    (else
	     (loop next (min time (thread-time next))))))))

; Debit the times of all threads from the current-thread on up.  If we find a
; thread with no time left, then that thread's scheduler becomes the potential
; next thread.
;
; Only the root thread can end up with a time-limit of #f, as all other threads
; have schedulers.

(define (debit-thread-times-and-run! next-to-run time-limit time-out?)
  (let ((elapsed (interrupt-timer-time)))
    (let loop ((thread (current-thread))
	       (time-limit time-limit)
	       (next-to-run next-to-run))
      (let ((scheduler (thread-scheduler thread)))
	(cond (scheduler
	       (let ((time-left (max2 (- (thread-time thread)
					 elapsed)
				      0)))
		 (set-thread-time! thread time-left)
		 (if (<= time-left 0)
		     (loop scheduler #f scheduler)
		     (loop scheduler
			   (if time-limit
			       (min time-limit time-left)
			       time-left)
			   next-to-run))))
	      (next-to-run
;               (debug-message (if time-out?
;                                  "[quantum ran out]"
;                                  "[run called]"))
	       (run-next-thread next-to-run time-limit))
	      (else
	       (schedule-interrupt! time-limit)))))))

; Debit the times of all running threads and run whomever is next.

(define (handle-timer-interrupt interrupted-template ei)
  (if (thread-scheduler (current-thread))
      (debit-thread-times-and-run! #f #f #t)))

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
;  (debug-message "[Running " (thread-name thread)
;                 " " (thread-uid thread) " on "
;                 (length return-values) " values]")
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
;  (debug-message "[suspending for "
;		 (enumerand->name (car event) event-type)
;		 "]")
  (debit-down! (thread-current-task scheduler))
  (switch-to-thread scheduler
		    (cons (thread-time (thread-current-task scheduler))
			  event)))

(define (debit-down! thread)  
  (let ((elapsed (interrupt-timer-time)))
    (let loop ((thread thread))
      (if thread
          (begin
            (set-thread-time! thread
			      (max2 (- (thread-time thread)
				       elapsed)
				    0))
            (loop (thread-current-task thread)))))))

; Same thing, except that we don't save the current continuation and
; we don't need to debit the thread's time.  This is used for completed
; and killed threads and is not exported.

(define (exit reason stuff)
  (disable-interrupts!)
;  (debug-message "[thread exiting]")
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
;	    (debug-message "[thread waiting]")
	    (set-thread-time! thread 'waiting)
	    (switch-to-thread (thread-scheduler thread)
			      (list time-left (enum event-type blocked)))))
      (set-enabled-interrupts! interrupts))))

; Is THREAD waiting for something to happen.

(define (waiting? thread)
  (eq? (thread-time thread) 'waiting))

; Various calls to SUSPEND.

(define (relinquish-timeslice)
  (suspend (enum event-type out-of-time) '()))

(define (block)
  (suspend (enum event-type blocked) '()))

; Block if the current proposal succeeds.  Returns true if successful and false
; if the commit fails.  The cell becomes the thread's cell.  It will be cleared
; if the thread is killed.

(define (maybe-commit-and-block cell)
  (disable-interrupts!)
  (cond ((maybe-commit)
	 (set-thread-cell! (current-thread) cell)
	 (suspend-to (thread-scheduler (current-thread))
		     (list (enum event-type blocked)))
	 #t)
	(else
	 (enable-interrupts!)
	 #f)))

; Utility procedure for the common case of blocking on a queue.

(define (maybe-commit-and-block-on-queue queue)
  (let ((cell (make-cell (current-thread))))
    (enqueue! queue cell)
    (maybe-commit-and-block cell)))

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

; Also ends the thread, but lets it run any pending dynamic-winds.

(define (terminate-thread! thread)
  (let ((interrupts (set-enabled-interrupts! no-interrupts)))
    (clear-thread-cell! thread)
    (interrupt-thread thread terminate-current-thread)))

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
	 (event (next-event! (current-thread))))
    (set-enabled-interrupts! interrupts)
    (if event
	(apply values event)
	(enum event-type no-event))))

(define (event-pending?)
  (let ((events (thread-events (current-thread))))
    (and events (not (queue-empty? events)))))

; Schedule the given event for THREAD and then check to see if any of the
; current thread's ancestors have received an event.

(define (schedule-event thread . event)
  (let ((interrupts (disable-interrupts!)))
    (really-schedule-event thread event)
    (maybe-suspend)
    (set-enabled-interrupts! interrupts)
    (unspecific)))

; Resume running the highest thread which has both a current task and a
; waiting event.  If there are none such the current thread can continue.

(define (maybe-suspend)
  (let loop ((thread (root-scheduler)))
    (let ((next (thread-current-task thread)))
      (if next
	  (let ((event (next-event! thread)))
	    (if event
		(suspend-to thread event)
		(loop next)))))))

; Add EVENT to THREAD's event queue.  If THREAD is already running some other
; thread but is not currently runnning, then we arrange for it's call to RUN
; to return as soon as it is resumed.

(define (really-schedule-event thread event)
;  (debug-message "[event "
;                 (enumerand->name (car event) event-type)
;                 " for "
;                 (thread-uid thread)
;                 "]")
  (cond ((and (thread-current-task thread)
	      (not (running? thread)))
	 (set-thread-arguments! thread
				(cons (thread-time (thread-current-task thread))
				      event))
	 (set-thread-current-task! thread #f))
	(else
	 (add-event! thread event)))
  (if (waiting? thread)
      (schedule-wakeup thread)))

; Make THREAD's scheduler aware of the fact that THREAD is runnable.
; Called with interrupts disabled.

(define (schedule-wakeup thread)
  (let ((scheduler (thread-scheduler thread)))
    (set-thread-time! thread 0)             ; clear WAITING flag
    (if scheduler
	(really-schedule-event scheduler 
			       (list (enum event-type runnable)
				     thread)))))

; Debugging routine

(define (show-running)
  (apply debug-message "Running:" (do ((e (current-thread) (thread-scheduler e))
				       (l '() (cons (thread-name e)
						    (cons " " l))))
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
  (apply spawn-on-scheduler
	 (thread-scheduler (current-thread))
	 thunk
	 id))

(define (spawn-on-root thunk . id)
  (if (root-scheduler)
      (apply spawn-on-scheduler
	     (root-scheduler)
	     thunk
	     id)
      (thunk)))

(define (spawn-on-scheduler scheduler thunk . id)
  (let ((thread (make-thread thunk
			     (if (null? id) #f (car id)))))
    (schedule-event scheduler
		    (enum event-type spawned)
		    thread)
    thread))

; Enqueue a RUNNABLE event for THREAD's scheduler.

(define (make-ready thread . args)
  (clear-thread-cell! thread)
  (set-thread-arguments! thread args)
  (if (thread-scheduler thread)
      (schedule-event (thread-scheduler thread)
		      (enum event-type runnable)
		      thread)
      (error "MAKE-READY thread has no scheduler" thread)))

(define (clear-thread-cell! thread)
  (let ((cell (thread-cell thread)))
    (if cell
	(begin
	  (set-thread-cell! thread #f)
	  (cell-set! cell #f)))))

; Same as MAKE-READY, except that we only schedule the thread if the current
; proposal succeeds.

(define (maybe-commit-and-make-ready thread-or-queue . args)
  (let ((ints (set-enabled-interrupts! 0)))
    (cond ((maybe-commit)
	   (if (queue? thread-or-queue)
	       (apply make-threads-ready thread-or-queue args)
	       (apply make-ready thread-or-queue args))
	   (set-enabled-interrupts! ints)
	   #t)
	  (else
	   (set-enabled-interrupts! ints)
	   #f))))

;; Common pattern

(define (maybe-commit-no-interrupts thunk)
  (let ((ints (disable-interrupts!)))
    (cond ((maybe-commit)
	   (thunk)
	   (set-enabled-interrupts! ints)
	   #t)
	  (else
	   (set-enabled-interrupts! ints)
	   #f))))

; Make all of the threads on QUEUE ready (and don't run any of them until
; all have been processed).

(define (make-threads-ready queue . args)
  (let loop ()
    (if (queue-empty? queue)
	(maybe-suspend)
	(let ((thread (cell-ref (dequeue! queue))))
	  (if thread
	      (apply make-ready thread args))
	  (loop)))))

;----------------

(define (schedule-interrupt! time)
  (schedule-interrupt time))

(define (cancel-timer-interrupt!)
  (schedule-interrupt 0))

; Use CHEAP-TIME to save the cost of a system call on every context switch.
; This clock is reset whenever we request an interrupt.

(define (interrupt-timer-time)
  (time (enum time-option cheap-time) #f))

; Used for waking sleepers

(define (real-time)
  (time (enum time-option real-time) #f))

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
	 (if (deadlock? c)
	     (exit-multitasking 0)
	     (punt)))
       (lambda ()
	 (call-with-current-continuation
	   (lambda (terminate)
	     (with-handler (lambda (c punt)
			     (if (terminate? c)
				 (terminate 0)
				 (punt)))
	       (lambda ()
		 (set! *thread-uid* 0)
		 (let ((thread (make-thread #f    ; thunk
					    'initial-thread)))
		   (set-thread-dynamic-env! thread (get-dynamic-env))
		   (set-thread-time! thread #f)
		   (set-thread-dynamic-point! thread (get-dynamic-point))
		   (set-current-thread! thread)
		   (session-data-set! root-scheduler-slot thread))
		 ;; End the LET to get THREAD out of the continuation to THUNK
		 ;; Interrupts were turned off by START-THREADS
		 (enable-interrupts!)
		 ;; We throw out after THUNK to avoid the EXIT below, as we
		 ;; have no scheduler to exit to.
		 (exit-multitasking (thunk))))))
	 ;; land here when terminating a thread
	 (exit (enum event-type completed) '()))))))

; Raised when there is nothing to run.

(define-condition-type 'deadlock '())
(define deadlock? (condition-predicate 'deadlock))

; Raised when the current thread has been killed.

(define-condition-type 'terminate '())
(define terminate? (condition-predicate 'terminate))

; Kill the current thread.  DEBUG-MESSAGE is used to try and make sure that some
; record exists when an error occured.  The system may be too broken for ERROR
; to work properly.

(define (terminate-current-thread)
  (signal 'terminate)
  (debug-message "Can't terminate current thread "
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

;----------------
; Fast binary version of max.

(define (max2 x y)
  (if (< x y)
      y
      x))

