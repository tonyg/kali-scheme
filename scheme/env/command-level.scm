; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Command levels for the command processor

; The command processor's state is divided into three parts:
; 1. User context - preserved across dump commands.
;    This includes the designated user and configuration environments.
; 2. Session state - one per "login"; not preserved across dump commands.
;    This includes ## and the command loop's interactive ports.
; 3. Command levels - one for each different command level.
;    This includes the threads being run at that level and the condition
;    that caused the level to be pushed, if any.
;
; Each command level has its own threads and scheduling queues.  Only one
; command level is running at any time.  An exception stops the current
; level and all its threads.

;----------------
; User context.
;
; This is a symbol table stored in the fluid $USER-CONTEXT.
; *USER-CONTEXT-INITIALIZERS* is a list of (<name> . <initial-value-thunk>)
; pairs.  The <thunk>s are called to get the initial value of the <name>d
; slots.  MAKE-USER-CONTEXT also takes an initalizing thunk that is called
; after the standard initializers have been run.

(define (make-user-context thunk)
  (let ((t (make-symbol-table)))
    (let-fluid $user-context t
      (lambda ()
        (for-each (lambda (name+thunk)
                    (table-set! t (car name+thunk) ((cdr name+thunk))))
                  *user-context-initializers*)
        (thunk)
        t))))

(define *user-context-initializers* '())

(define $user-context (make-fluid #f))

; (set-fluid! $user-context (make-user-context unspecific)) ;Bad for GC

(define (user-context) (fluid $user-context))

; Add a new slot to the user context.

(define (user-context-accessor name initializer)
  (set! *user-context-initializers*
        (append *user-context-initializers*
                (list (cons name initializer))))
  (let ((probe (fluid $user-context)))
    (if probe (table-set! probe name (initializer))))
  (lambda ()
    (table-ref (user-context) name)))

(define (user-context-modifier name)
  (lambda (new)
    (table-set! (user-context) name new)))

; If true exceptions cause a new command level to be pushed.

(define push-command-levels?
  (user-context-accessor 'push-command-levels (lambda () #t)))

;----------------
; Session state.

; This is a record stored in the fluid $SESSION.
; It has the command interpreter's ports, the most recent values returned
; by a command, and the batch-mode and break-on-warnings switches.

(define-record-type session :session
  (make-session command-thread
                input-port output-port error-port
		focus-values
		batch-mode? break-on-warnings?)
  (command-thread session-command-thread)
  (input-port session-input-port)
  (output-port session-output-port)
  (error-port session-error-port)
  (focus-values session-focus-values set-session-focus-values!)
  (batch-mode? session-batch-mode? set-session-batch-mode?!)
  (break-on-warnings? session-break-on-warnings? set-session-break-on-warnings?!))

(define $session
  (make-fluid 'session-not-initialized))

(define (session-accessor accessor)
  (lambda () (accessor (fluid $session))))

(define (session-modifier modifier)
  (lambda (new) (modifier (fluid $session) new)))

(define command-thread (session-accessor session-command-thread))
(define command-input (session-accessor session-input-port))
(define command-output (session-accessor session-output-port))
(define command-error-output (session-accessor session-error-port))
(define focus-values (session-accessor session-focus-values))
(define set-focus-values! (session-modifier set-session-focus-values!))
(define batch-mode? (session-accessor session-batch-mode?))
(define set-batch-mode?! (session-modifier set-session-batch-mode?!))
(define break-on-warnings? (session-accessor session-break-on-warnings?))
(define set-break-on-warnings?! (session-modifier set-session-break-on-warnings?!))

; Log in

(define (with-new-session context iport oport eport resume-args batch? thunk)
  (let-fluids $user-context context
	      $session (make-session (current-thread)
				     iport oport eport resume-args batch? #f)
    thunk))

;----------------
; Command levels

(define-record-type command-level :command-level
  (really-make-command-level queue thread-counter dynamic-env
			     levels throw repl-thunk paused threads)
  command-level?
  (queue command-level-queue)                    ; queue of runnable threads
  (thread-counter command-level-thread-counter)  ; count of extant threads
  (dynamic-env command-level-dynamic-env)        ; used for spawns
  (levels command-level-levels)                  ; levels above this one
  (throw command-level-throw)                    ; exit from this level
  (repl-thunk command-level-repl-thunk)          ; thunk to (re)start level
  (repl-thread command-level-repl-thread set-command-level-repl-thread!)
                                             ; thread running the REPL
  (paused command-level-paused-thread set-command-level-paused-thread!)
                                             ; thread that pushed next level
  (threads x-command-level-threads set-command-level-threads!))
                             ; lazily generated list of this level's threads

(define (make-command-level repl-thunk dynamic-env levels throw)
  (let ((level (really-make-command-level (make-thread-queue)
					  (make-counter)
					  dynamic-env
					  levels
					  throw
					  repl-thunk
					  #f      ; paused thread
					  #f)))   ; undetermined thread list
    (spawn-repl-thread! level)
    level))

; Add THUNK as an thread to LEVEL.  The level is stored in the thread so
; that when it is rescheduled after blocking it can be put on the correct
; run queue.

(define (spawn-on-command-level level thunk id)    
  (let ((thread (make-thread thunk (command-level-dynamic-env level) id)))
    (set-thread-scheduler! thread (command-thread))
    (set-thread-data! thread level)
    (exclusively-enqueue-thread! (command-level-queue level) thread '())
    (increment-counter! (command-level-thread-counter level))
    thread))

; Add a new REPL thread to LEVEL.

(define (spawn-repl-thread! level)
  (let ((thread (spawn-on-command-level level
					(command-level-repl-thunk level)
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

; Get the value of FLUID in LEVEL's dynamic environment.

(define (command-level-fluid level fluid)
  (fluid-lookup (command-level-dynamic-env level) fluid))

;----------------
; Entry point

; Starting the command processor.  This arranges for an interrupt if the heap
; begins to fill up or when a keyboard interrupts occurs, starts a new session,
; runs an initial thunk and then pushes a command level.
;
; The double-paren around the WITH-NEW-SESSION is because it returns a
; continuation which is the thing to do after the command-processor exits.

(define (start-command-levels resume-args context
			      start-thunk repl-thunk interrupt-repl-proc)
    (notify-on-interrupts (current-thread) interrupt-repl-proc)
    ((with-new-session context
		       (current-input-port)
		       (current-output-port)
		       (current-error-port)
		       resume-args
		       (and (pair? resume-args)
			    (equal? (car resume-args) "batch"))
	(lambda ()
	  (start-thunk)
	  (let ((thunk (really-push-command-level repl-thunk
						  (get-dynamic-env)
						  '())))
	    (ignore-further-interrupts)
	    thunk)))))

(define (notify-on-interrupts thread interrupt-repl-proc)
  (set-interrupt-handler! (enum interrupt keyboard)
			  (lambda stuff
			    (schedule-event thread
					    (enum event-type interrupt)
					    (enum interrupt keyboard)
					    interrupt-repl-proc)))
  (call-before-heap-overflow!
   (lambda stuff
     (schedule-event thread
		     (enum event-type interrupt)
		     (enum interrupt post-gc)
		     interrupt-repl-proc)))
  (call-when-deadlocked!
   (lambda stuff
     (schedule-event thread
		     (enum event-type deadlock)
		     interrupt-repl-proc))))

(define (ignore-further-interrupts)
  (set-interrupt-handler! (enum interrupt keyboard)
			  (lambda stuff
			    (apply signal (cons 'interrupt stuff))))
  (call-before-heap-overflow! (lambda stuff #f))
  (call-when-deadlocked! #f))

; The number of milliseconds per timeslice in the command interpreter
; scheduler.  Should be elsewhere?

(define command-quantum 200)

; Grab the current continuation, then make a command level and run it.
;
; The double-paren around the CWCC is because it returns a continuation which
; is the thing to do after the command level exits.

(define (really-push-command-level repl-thunk dynamic-env levels)
  ((call-with-current-continuation
     (lambda (throw)
       (let ((*out?* #f)
	     (level (make-command-level repl-thunk dynamic-env levels throw)))
	 (dynamic-wind
	  (lambda ()
	    (if *out?*
		(error "can't throw back into a command level" level)))
	  (lambda ()
	    (run-command-level level #f))
	  (lambda ()
	    (set! *out?* #t)
	    (terminate-level level))))))))

(define (terminate-level level)
  (let ((threads (command-level-threads level))
	(queue (command-level-queue level))
	(*out?* #f))
    (for-each (lambda (thread)
		(if (thread-continuation thread)
		    (begin
		      (remove-thread-from-queues! thread)
		      (interrupt-thread thread terminate-current-thread)
		      (exclusively-enqueue-thread! queue thread '()))))
	      threads)
    (dynamic-wind
     (lambda ()
       (if *out?*
	   (error "can't throw back into a command level" level)))
     (lambda ()
       (run-command-level level #t))
     (lambda ()
       (set! *out?* #t)
       (let ((levels (command-level-levels level)))
	 (if (not (null? levels))
	     (reset-command-input! (car levels))))))))

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
      (begin
	(steal-channel-port! (command-input) interrupt-thread)
	(steal-channel-port! (command-output) interrupt-thread)
	(steal-channel-port! (command-error-output) interrupt-thread)))
  (run-threads
    (round-robin-event-handler (command-level-queue level)
			       command-quantum
			       (unspecific)
			       (command-level-thread-counter level)
			       (command-level-event-handler level terminating?)
			       (command-level-upcall-handler level)
			       (command-level-wait level terminating?))))

; Handling events.
; SPAWNED and RUNNABLE events require putting the job on the correct queue.
; A keyboard interrupt exits when in batch mode and pushes a new command
; level otherwise.

(define (command-level-event-handler level terminating?)
  (let ((levels (cons level (command-level-levels level))))
    (lambda (event args)
      (enum-case event-type event
	((spawned)
	 (spawn-on-command-level level (car args) (cadr args))
	 #t)
	((runnable)
	 (let* ((thread (car args))
		(level (thread-data thread)))
	   (cond ((not (command-level? level))
		  (error "non-command-level thread restarted on a command level"
			 thread))
		 ((memq level levels)
		  (exclusively-enqueue-thread! (command-level-queue level)
					       thread
					       (cdr args))))
	   #t))
	((interrupt)
	 (if terminating?
	     (warn "Interrupted while unwinding terminated level's threads."))
	 (quit-or-push-level 'interrupt (car args) (cadr args) levels)
	 #t)
	((deadlock)
	 (if terminating?
	     (warn "Deadlocked while unwinding terminated level's threads."))
	 (quit-or-push-level 'deadlocked (unspecific) (car args) levels)
	 #t)
	(else
	 #f)))))

; There are two dynamic environments that we need to take care of.  First
; we install the one for spawned threads, to allow REPL-PROC to add whatever
; it needs.  We then save that one for spawning threads and install the
; command-levels dyanmic environment before calling REALLY-PUSH-COMMAND-LEVEL.
;
; When exiting in batch mode two thunks are needed to get past two different
; restart points at the top level.

(define (quit-or-push-level why data repl-proc levels)
  (if (batch-mode?)
      ((command-level-throw (last levels)) (lambda () (lambda () 0)))
      (let ((levels-env (get-dynamic-env)))
	(set-dynamic-env! (command-level-dynamic-env (car levels)))
	(repl-proc
	 why
	 data
	 (lambda (thunk)
	   (let ((spawn-env (get-dynamic-env)))
	     (set-dynamic-env! levels-env)
	     (really-push-command-level thunk spawn-env levels)))))))

; Wait for events if there are blocked threads, otherwise add a new REPL
; thread if we aren't on the way out.

(define (command-level-wait level terminating?)
  (lambda ()
    (cond ((< 0 (counter-value (command-level-thread-counter level)))
	   (wait))
	  (terminating?
	   #f)
	  (else
	   (warn "command interpreter has died; restarting")
	   (spawn-repl-thread! level)
	   #t))))

; Four different upcalls:
;  (command-levels)  ->  return the current command levels
;  (throw-to-command-level level thunk)        ; exit from LEVEL and calls THUNK
;  (push-command-level repl-thunk dynamic-env) ; push a new command level
;  (restart-repl)    ->  kill the current REPL and restart it

(define (command-level-upcall-handler level)
  (let ((levels (cons level (command-level-levels level))))
    (lambda (thread token args)
      (cond ((eq? token command-levels-token)
	     levels)
	    ((eq? token throw-to-command-level-token)
	     ; arguments are LEVEL THUNK
 	     ((command-level-throw (car args)) (cadr args)))
	    ((eq? token push-command-level-token)
	     ; arguments are CALLING-THREAD REPL-THUNK DYNAMIC-ENV
	     (set-command-level-paused-thread! level (car args))
	     (really-push-command-level (cadr args) (caddr args) levels))
	    (else
	     (propogate-upcall thread token args))))))

(define command-levels-token (list 'command-levels-token))
(define push-command-level-token (list 'push-command-level-token))
(define throw-to-command-level-token (list 'throw-to-command-level-token))

(define (command-levels)
  (upcall command-levels-token))

(define (command-level)
  (car (command-levels)))

(define (top-command-level)
  (last (command-levels)))

; Command level control

(define (push-command-level thunk)
  (upcall push-command-level-token (current-thread) thunk (get-dynamic-env)))

(define (throw-to-command-level level thunk)
  (upcall throw-to-command-level-token level thunk))

; This makes a new level just like the old one.

(define (restart-command-level level)
  (throw-to-command-level
   level
   (lambda ()
     (really-push-command-level (command-level-repl-thunk level)
				(command-level-dynamic-env level)
				(command-level-levels level)))))

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

(define (kill-paused-thread! level)
  (let ((paused (command-level-paused-thread level)))
    (if (not paused)
	(error "level has no paused thread" level))
    (if (eq? paused (command-level-repl-thread level))
	(spawn-repl-thread! level))
    (kill-thread! paused)
    (set-command-level-paused-thread! level #f)))

