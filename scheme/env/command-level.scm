; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Command levels for the command processor
;
; Each command level has its own threads and scheduling queues.  Only one
; command level is running at any time.  An exception stops the current
; level and all its threads.
;
; A command level also has the condition that caused the level to be pushed,
; if any.

;----------------------------------------------------------------
; Command levels

(define-record-type command-level :command-level
  (really-make-command-level queue thread-counter dynamic-env
			     levels throw terminated?
			     condition menu menu-position value-stack
			     paused threads)
  command-level?
  (queue command-level-queue)                   ; queue of runnable threads
  (thread-counter command-level-thread-counter) ; count of extant threads
  (dynamic-env command-level-dynamic-env)       ; used for spawns
  (levels command-level-levels)                 ; levels above this one
  (throw command-level-throw)                   ; exit from this level
  (terminated? command-level-terminated? set-command-level-terminated?!)
						; true if unwinds already run
  (condition command-level-condition)           ; condition which caused this
                                                ; level to be pushed
  (menu command-level-menu set-command-level-menu!)
  (menu-position command-level-menu-position set-command-level-menu-position!)
  (value-stack command-level-value-stack set-command-level-value-stack!)
  (repl-thread command-level-repl-thread set-command-level-repl-thread!)
						; thread running the REPL
  (paused command-level-paused-thread set-command-level-paused-thread!)
						; thread that pushed next level
  (threads x-command-level-threads set-command-level-threads!))
				; lazily generated list of this level's threads

(define (make-command-level condition inspecting? dynamic-env levels throw)
  (let ((level (really-make-command-level (make-queue)
					  (make-counter)
					  dynamic-env
					  levels
					  throw
					  #f	; not yet terminated
					  condition
					  #f	; no menu
					  #f	; no menu position
					  (if inspecting?	; value stack
					      '()
					      #f)
					  #f	; paused thread
					  #f)))	; undetermined thread list
    (spawn-repl-thread! level)
    level))

; Add THUNK as a thread to LEVEL.  The level is stored in the thread so
; that when it is rescheduled after blocking it can be put on the correct
; run queue.

(define (spawn-on-command-level level thunk id)    
  (let ((thread (make-thread thunk id)))
    (spawn-thread-on-command-level level thread)
    thread))

(define (spawn-thread-on-command-level level thread)
  (set-thread-dynamic-env! thread (command-level-dynamic-env level))
  (set-thread-scheduler! thread (command-thread))
  (set-thread-data! thread level)
  (enqueue! (command-level-queue level) thread)
  (increment-counter! (command-level-thread-counter level)))

; Add a new REPL thread to LEVEL.

(define (spawn-repl-thread! level)
  (let ((thread (spawn-on-command-level level
					(user-session-repl-thunk (user-session))
					'command-loop)))
    (set-command-level-repl-thread! level thread)))

; Find all of the threads belonging to LEVEL.  This may be expensive to call
; and may not return the correct value if LEVEL is currently running.

(define (command-level-threads level)
  (cond ((and (x-command-level-threads level)
	      (weak-pointer-ref (x-command-level-threads level)))
	 => (lambda (x) x))
	((= 1 (counter-value (command-level-thread-counter level)))
	 (list (command-level-repl-thread level)))
	(else
	 (let ((threads (all-threads)))
	   (do ((i 0 (+ i 1))
		(es '() (let ((thread (vector-ref threads i)))
			  (if (and (thread-continuation thread)
				   (eq? level (thread-data thread)))
			      (cons thread es)
			      es))))
	       ((= i (vector-length threads))
		(set-command-level-threads! level (make-weak-pointer es))
		es))))))

;----------------------------------------------------------------
; Entry point

; Starting the command processor.  This arranges for an interrupt if the heap
; begins to fill up or when a keyboard interrupts occurs, starts a new user
; session, runs an initial thunk and then pushes a command level.

(define (start-command-levels resume-args context
			      start-thunk repl-thunk
			      condition inspector-state
			      input-port output-port error-port)
  ;(debug-message "[Starting levels]")
  (notify-on-interrupts (current-thread))
  (let ((dynamic-env (get-dynamic-env))
	(session (make-user-session (current-thread)
				    (or context (make-user-context))
				    repl-thunk
				    input-port
				    output-port
				    error-port
				    resume-args	; focus values
				    #f		; exit status
				    (and (pair? resume-args)
					 (equal? (car resume-args) "batch")))))
    (with-handler command-levels-condition-handler
      (lambda ()
	(let-fluids $command-level-thread? #t
		    $user-session session
	  (lambda ()
	    ;(debug-message "[start-thunk]")
	    (start-thunk)
	    (let ((thunk (really-push-command-level condition
						    inspector-state
						    dynamic-env
						    '())))
	      (ignore-further-interrupts)
	      thunk)))))))

; A fluid to tell us when we are in the command level thread (used to
; avoid sending upcalls to whomever is running us).

(define $command-level-thread? (make-fluid #f))

(define (on-command-level-thread?)
  (fluid $command-level-thread?))

(define $user-session (make-fluid #f))

; If true exceptions cause a new command level to be pushed.

(define push-command-levels?
  (user-context-accessor 'push-command-levels (lambda () #t)))

(define set-push-command-levels?!
  (user-context-modifier 'push-command-levels))

; Have THREAD be sent an event when an interrupt occurs.

(define (notify-on-interrupts thread)
  (set-interrupt-handler! (enum interrupt keyboard)
			  (lambda stuff
			    (schedule-event thread
					    (enum event-type interrupt)
					    (enum interrupt keyboard))))
  (call-before-heap-overflow!
   (lambda stuff
     (schedule-event thread
		     (enum event-type interrupt)
		     (enum interrupt post-gc))))
  (call-when-deadlocked!
   (lambda stuff
     (schedule-event thread
		     (enum event-type deadlock)))))

(define (ignore-further-interrupts)
  (set-interrupt-handler! (enum interrupt keyboard)
			  (lambda stuff
			    (apply signal (cons 'interrupt stuff))))
  (call-before-heap-overflow! (lambda stuff #f))
  (call-when-deadlocked! #f))

; Handler for the command-levels thread.  Warnings and notes are printed,
; errors cause an exit.  This handler is used to catch errors before they
; go to the 

(define (command-levels-condition-handler c next-handler)
  (cond ((or (warning? c)
	     (note? c))
	 (force-output (current-output-port))   ; keep synchronous
	 (display-condition c (current-error-port))
	 (unspecific))				; proceed
        ((error? c)
	 (force-output (current-output-port))	; keep synchronous
	 (display-condition c (current-error-port))
	 (scheme-exit-now 1))
        (else                           
         (next-handler))))

;----------------------------------------------------------------
; Grab the current continuation, then make a command level and run it.
;
; The double-paren around the CWCC is because it returns a continuation which
; is the thing to do after the command level exits.
;
; Should this detect the difference between normal termination and a throw
; out?

(define (really-push-command-level condition inspecting? dynamic-env levels)
  ((call-with-current-continuation
     (lambda (throw)
       (let ((level (make-command-level condition
					inspecting?
					(preserve-interaction-env dynamic-env)
					levels
					throw)))
	 (let-fluid $current-level level
	   (lambda ()
	     (dynamic-wind
	      (lambda ()
		(if (command-level-terminated? level)
		    (error "trying to throw back into a command level" level)))
	      (lambda ()
		(run-command-level level #f))
	      (lambda ()
		(if (command-level-terminated? level)
		    (warn "abandoning failed level-termination unwinds.")
		    (begin
		      (set-command-level-terminated?! level #t)
		      (terminate-level level))))))))))))

; Rebind the interaction environment so that side-effects to it are local
; to a command level.

(define (preserve-interaction-env dynamic-env)
  (let ((old (get-dynamic-env)))
    (set-dynamic-env! dynamic-env)
    (let ((new (with-interaction-environment (interaction-environment)
					     get-dynamic-env)))
      (set-dynamic-env! old)
      new)))

; Fluid to tell us what the current level is.  This is only visible in the
; command-levels thread.

(define $current-level (make-fluid #f))

(define (terminate-level level)
  (let ((threads (command-level-threads level))
	(*out?* #f))
    (for-each (lambda (thread)
		(if (thread-continuation thread)
		    (terminate-level-thread thread level)))
	      threads)
    (dynamic-wind
     (lambda ()
       (if *out?*
	   (error "trying to throw back into a command level" level)))
     (lambda ()
       (run-command-level level #t))
     (lambda ()
       (set! *out?* #t)
       (let ((levels (command-level-levels level)))
	 (if (not (null? levels))
	     (reset-command-input! (car levels))))))))

; Put the thread on the runnable queue if it is not already there and then
; terminate it.  Termination removes the thread from any blocking queues
; and interrupts with a throw that will run any pending dynamic-winds.

(define (terminate-level-thread thread level)
  (let ((queue (command-level-queue level)))
    (if (not (on-queue? queue thread))
	(enqueue! queue thread))
    (terminate-thread! thread)))

(define (reset-command-input! level)
  (let ((repl (command-level-repl-thread level)))
    (if repl
	(interrupt-thread repl
			  (lambda return-values
			    (signal 'reset-command-input)
			    (apply values return-values))))))

(define-condition-type 'reset-command-input '())
(define reset-command-input? (condition-predicate 'reset-command-input))

; Make sure the input and output ports are available and then run the threads
; on LEVEL's queue.

(define (run-command-level level terminating?)
  (if (not terminating?)
      (set-exit-status! #f))
  (run-threads
    (round-robin-event-handler (command-level-queue level)
			       command-quantum
			       (unspecific)
			       (command-level-thread-counter level)
			       (command-level-event-handler level terminating?)
			       command-level-upcall-handler
			       (command-level-wait level terminating?))))

; The number of milliseconds per timeslice in the command interpreter
; scheduler.  Should be elsewhere?

(define command-quantum 200)

; Handling events.
; SPAWNED and RUNNABLE events require putting the job on the correct queue.
; A keyboard interrupt exits when in batch mode and pushes a new command
; level otherwise.

(define (command-level-event-handler level terminating?)
  (let ((levels (cons level (command-level-levels level))))
    (lambda (event args)
      (enum-case event-type event
	((spawned)
	 (spawn-thread-on-command-level level (car args))
	 #t)
	((runnable)
	 (let* ((thread (car args))
		(level (thread-data thread)))
	   (cond ((not (command-level? level))
		  (error "non-command-level thread restarted on a command level"
			 thread))
		 ((memq level levels)
		  (enqueue! (command-level-queue level) thread))
		 (else
		  (warn "dropping thread from exited command level"
			thread)))
	   #t))
	((interrupt)
	 (if terminating?
	     (warn "Interrupted while unwinding terminated level's threads."))
	 (quit-or-push-level (make-condition 'interrupt args) levels)
	 #t)
	((deadlock)
	 (if terminating?
	     (warn "Deadlocked while unwinding terminated level's threads."))
 	 (quit-or-push-level (make-condition 'error (list 'deadlocked))
 			     levels)
	 #t)
	(else
	 #f)))))

(define (quit-or-push-level condition levels)
  (if (batch-mode?)
      ((command-level-throw (last levels)) (lambda () (lambda () 0)))
      (really-push-command-level condition
				 #f
				 (command-level-dynamic-env (car levels))
				 levels)))

; Wait for events if there are blocked threads, otherwise add a new REPL
; thread if we aren't on the way out.

(define (command-level-wait level terminating?)
  (lambda ()
    (cond ((< 0 (counter-value (command-level-thread-counter level)))
	   (wait)
	   #t)
	  (terminating?
	   #f)
	  ((exit-status)
	   (exit-levels level (exit-status)))
	  (else
	   (warn "command interpreter has died; restarting")
	   (spawn-repl-thread! level)
	   #t))))

; Leave the command-level system with STATUS.

(define (exit-levels level status)
  (let ((top-level (last (cons level (command-level-levels level)))))
    ((command-level-throw top-level)
       (lambda () (lambda () status)))))

;----------------------------------------------------------------
; Upcalls

; The tokens are records which have contain the upcall procedure.

(define command-level-upcall-handler
  (lambda (thread token args)
    (if (upcall? token)
	(apply (upcall-procedure token) args)
	(propogate-upcall thread token args))))

(define-record-type upcall :upcall
  (make-upcall procedure id)
  upcall?
  (procedure upcall-procedure)
  (id upcall-id))

(define-record-discloser :upcall
  (lambda (upcall)
    (list 'upcall-token (upcall-id upcall))))

; If we are already in the command-level thread we just make the call;
; if not, we have to actually do the upcall.

(define-syntax define-upcall
  (syntax-rules ()
    ((define-upcall (id args ...) . body)
     (define id 
       (let ((token (make-upcall (lambda (args ...) . body)
				 'id)))
	 (lambda (args ...)
	   (if (on-command-level-thread?)
	       ((upcall-procedure token) args ...)
	       (upcall token args ...))))))))

;----------------
; The current command level and friends

; Return the current command level.

(define-upcall (command-level)
  (fluid $current-level))

; Return the current list of command levels.

(define (command-levels)
  (let ((current-level (command-level)))
    (cons current-level
	  (command-level-levels current-level))))

; Top-most command level.

(define (top-command-level)
  (last (command-levels)))

;----------------
; Menus and the value stack.

(define (maybe-menu)
  (command-level-menu (command-level)))

(define (set-menu! value)
  (set-command-level-menu! (command-level) value))

(define (menu-position)
  (command-level-menu-position (command-level)))

(define (set-menu-position! value)
  (set-command-level-menu-position! (command-level) value))

(define (value-stack)
  (command-level-value-stack (command-level)))

(define (set-value-stack! value)
  (set-command-level-value-stack! (command-level) value))

;----------------
; User session

(define-upcall (user-session)
  (fluid $user-session))

;----------------
; Command-level control

(define-upcall (terminate-command-processor! status)
  (set-exit-status! status)
  (let* ((level (command-level))
	 (repl-thread (command-level-repl-thread level)))
    (if repl-thread
	(begin 
	  (set-command-level-repl-thread! level #f)
	  (terminate-level-thread repl-thread level)))))

(define-upcall (push-command-level-upcall condition inspecting?
					  thread dynamic-env)
  (set-command-level-paused-thread! (command-level) thread)
  (really-push-command-level condition
			     inspecting?
			     dynamic-env
			     (command-levels)))

; Have to grab the current thread and dynamic environment before making the
; upcall.

(define (push-command-level condition inspecting?)
  (push-command-level-upcall condition
			     inspecting?
			     (current-thread)
			     (get-dynamic-env)))

(define-upcall (throw-to-command-level level thunk)
  ((command-level-throw level) thunk))

; This makes a new level just like the old one.

(define (restart-command-level level)
  (throw-to-command-level
   level
   (lambda ()
     (really-push-command-level (command-level-condition   level)
				#f		; drop the old value stack
				(command-level-dynamic-env level)
				(command-level-levels      level)))))

; Proceed with LEVEL causing RETURN-VALUES to be returned from the
; PUSH-COMMAND-LEVELS call that started LEVEL.

(define (proceed-with-command-level level . return-values)
  (throw-to-command-level (level-pushed-from level)
			  (lambda ()
			    (apply values return-values))))

; Find the level that was pushed from LEVEL.

(define (level-pushed-from level)
  (let loop ((levels (command-levels)))
    (cond ((null? (cdr levels))
	   (error "level not found" level))
	  ((eq? level (cadr levels))
	   (car levels))
	  (else
	   (loop (cdr levels))))))

; Kill the thread on LEVEL that caused a new level to be pushed.  This is
; used when the user wants to continue running the rest of LEVEL's threads.
; We enqueue the paused thread so that its dynamic-winds will be run.

(define (kill-paused-thread! level)
  (let ((paused (command-level-paused-thread level)))
    (if paused
	(begin
	  (if (eq? paused (command-level-repl-thread level))
	      (spawn-repl-thread! level))
	  (terminate-thread! paused)	; it's already running, so no enqueue
	  (set-command-level-paused-thread! level #f))
	(warn "level has no paused thread" level))))
