; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
; The is a symbol table stored in a slot in the session state (see below).
; *USER-CONTEXT-INITIALIZERS* is a list of (<name> . <initial-value-thunk>)
; pairs.  The <thunk>s are called to get the initial value of the <name>d
; slots.

(define (make-user-context)
  (make-symbol-table))
  
(define (initialize-user-context!)
  (let ((t (user-context)))
    (for-each (lambda (name+thunk)
		(table-set! t (car name+thunk) ((cdr name+thunk))))
	      *user-context-initializers*)
    t))

(define *user-context-initializers* '())

; Add a new slot to the user context.

(define (user-context-accessor name initializer)
  (set! *user-context-initializers*
        (append *user-context-initializers*
                (list (cons name initializer))))
  (let ((context (user-context)))
    (if context
	(table-set! context name (initializer))))
  (lambda ()
    (table-ref (or (user-context)
		   (error "command interpreter not initialized - no user context"
			  name))
	       name)))

(define (user-context-modifier name)
  (lambda (new)
    (table-set! (or (user-context)
		    (error "command interpreter not initialized - no user context"
			   name))
		name
		new)))

; There are a few places that have alternate behavior based on this, but mostly
; it's used to detect premature uses of the user context.

(define (user-context)
  (if (session-started?)
      (real-user-context)
      #f))

;----------------
; Session state.

; This is a record stored in the session data.
; It has the command interpreter's ports, the most recent values returned
; by a command, an exit status, and the batch-mode and break-on-warnings
; switches.

(define-record-type session :session
  (make-session command-thread
		user-context
                input-port output-port error-port
		focus-values
		exit-status
		batch-mode? break-on-warnings?)
  session?
  (command-thread session-command-thread)
  (user-context session-user-context)
  (input-port session-input-port)
  (output-port session-output-port)
  (error-port session-error-port)
  (focus-values session-focus-values set-session-focus-values!)
  (exit-status session-exit-status set-session-exit-status!)
  (batch-mode? session-batch-mode? set-session-batch-mode?!)
  (break-on-warnings? session-break-on-warnings? set-session-break-on-warnings?!))

(define session-slot (make-session-data-slot! #f))

(define (session-started?)
  (session? (session-data-ref session-slot)))

(define (session-accessor accessor)
  (lambda () (accessor (session-data-ref session-slot))))

(define (session-modifier modifier)
  (lambda (new) (modifier (session-data-ref session-slot) new)))

(define command-thread (session-accessor session-command-thread))
(define real-user-context (session-accessor session-user-context))
(define command-input (session-accessor session-input-port))
(define command-output (session-accessor session-output-port))
(define command-error-output (session-accessor session-error-port))
(define focus-values (session-accessor session-focus-values))
(define set-focus-values! (session-modifier set-session-focus-values!))
(define batch-mode? (session-accessor session-batch-mode?))
(define set-batch-mode?! (session-modifier set-session-batch-mode?!))
(define break-on-warnings? (session-accessor session-break-on-warnings?))
(define set-break-on-warnings?! (session-modifier set-session-break-on-warnings?!))
(define exit-status (session-accessor session-exit-status))
(define set-exit-status! (session-modifier set-session-exit-status!))

; Log in

(define (start-new-session context iport oport eport resume-args batch?)
  (session-data-set! session-slot
		     (make-session (current-thread)
				   context
				   iport oport eport
				   resume-args
				   #f            ; no exit status yet
				   batch?
				   #f)))         ; don't break on warnings

;----------------
; Command levels

(define-record-type command-level :command-level
  (really-make-command-level queue thread-counter dynamic-env
			     levels throw repl-thunk repl-data paused threads)
  command-level?
  (queue command-level-queue)                   ; queue of runnable threads
  (thread-counter command-level-thread-counter) ; count of extant threads
  (dynamic-env command-level-dynamic-env)       ; used for spawns
  (levels command-level-levels)                 ; levels above this one
  (throw command-level-throw)                   ; exit from this level
  (repl-thunk command-level-repl-thunk)         ; thunk to (re)start level
  (repl-data command-level-repl-data set-command-level-repl-data!)
						; data used by REPL
  (repl-thread command-level-repl-thread set-command-level-repl-thread!)
						; thread running the REPL
  (paused command-level-paused-thread set-command-level-paused-thread!)
						; thread that pushed next level
  (threads x-command-level-threads set-command-level-threads!))
				; lazily generated list of this level's threads

(define (make-command-level repl-thunk repl-data dynamic-env levels throw)
  (let ((level (really-make-command-level (make-thread-queue)
					  (make-counter)
					  dynamic-env
					  levels
					  throw
					  repl-thunk
					  repl-data
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
    (enqueue-thread! (command-level-queue level) thread)
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

;----------------
; Entry point

; Starting the command processor.  This arranges for an interrupt if the heap
; begins to fill up or when a keyboard interrupts occurs, starts a new session,
; runs an initial thunk and then pushes a command level.

(define (start-command-levels resume-args context
			      start-thunk repl-thunk repl-data)
  (notify-on-interrupts (current-thread))
  (start-new-session (or context (make-user-context))
		     (current-input-port)
		     (current-output-port)
		     (current-error-port)
		     resume-args
		     (and (pair? resume-args)
			  (equal? (car resume-args) "batch")))
  (if (not context)
      (initialize-user-context!))
  (start-thunk)
  (let ((thunk (really-push-command-level repl-thunk
					  repl-data
					  (get-dynamic-env)
					  '())))
    (ignore-further-interrupts)
    thunk))

; If true exceptions cause a new command level to be pushed.

(define push-command-levels?
  (user-context-accessor 'push-command-levels (lambda () #t)))

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

; The number of milliseconds per timeslice in the command interpreter
; scheduler.  Should be elsewhere?

(define command-quantum 200)

; Grab the current continuation, then make a command level and run it.
;
; The double-paren around the CWCC is because it returns a continuation which
; is the thing to do after the command level exits.

(define (really-push-command-level repl-thunk repl-data dynamic-env levels)
  ((call-with-current-continuation
     (lambda (throw)
       (let ((*out?* #f)
	     (level (make-command-level repl-thunk repl-data dynamic-env
					levels throw)))
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
                      (remove-thread-from-queue! thread)
		      (interrupt-thread thread
					(lambda ignore
					  (terminate-current-thread)))
		      (enqueue-thread! queue thread))))
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
	(set-exit-status! #f)
	(steal-channel-port! (command-input))
	(steal-channel-port! (command-output))
	(steal-channel-port! (command-error-output))))
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
		  (enqueue-thread! (command-level-queue level)
				   thread))
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
       (really-push-command-level (command-level-repl-thunk (last levels))
				  condition
				  (command-level-dynamic-env (car levels))
				  levels)))

; Wait for events if there are blocked threads, otherwise add a new REPL
; thread if we aren't on the way out.

(define (command-level-wait level terminating?)
  (lambda ()
    (cond ((< 0 (counter-value (command-level-thread-counter level)))
	   (wait))
	  ((exit-status)
	   (exit-levels level (exit-status)))
	  (terminating?
	   #f)
	  (else
	   (warn "command interpreter has died; restarting")
	   (spawn-repl-thread! level)
	   #t))))

; Leave the command-level system with STATUS.

(define (exit-levels level status)
  (let ((top-level (last (cons level (command-level-levels level)))))
    ((command-level-throw top-level)
       (lambda () (lambda () status)))))

; Upcalls:
; return the current command levels
;  (command-levels)  ->  list of levels
; exit from LEVEL and calls THUNK
;  (throw-to-command-level level thunk)
; push a new command level
;  (push-command-level repl-thunk repl-data dynamic-env)
; stop running a repl
;  (terminate-repl status)

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
 	     (really-push-command-level (cadr args) (caddr args) (cadddr args)
 					levels))
	    ((eq? token terminate-repl-token)
	     (set-exit-status! (car args))
	     (let ((repl-thread (command-level-repl-thread level)))
	       (if repl-thread
		   (begin 
		     (set-command-level-repl-thread! level #f)
		     (kill-thread! repl-thread)))))
 	    ((eq? token repl-data-token)
 	     (command-level-repl-data level))
 	    ((eq? token set-repl-data!-token)
 	     (set-command-level-repl-data! level (car args)))
	    (else
	     (propogate-upcall thread token args))))))

(define command-levels-token (list 'command-levels-token))
(define push-command-level-token (list 'push-command-level-token))
(define throw-to-command-level-token (list 'throw-to-command-level-token))
(define terminate-repl-token (list 'terminate-repl-token))
(define repl-data-token (list 'repl-data-token))
(define set-repl-data!-token (list 'set-repl-data!-token))

(define (repl-data)
  (upcall repl-data-token))

(define (set-repl-data! value)
  (upcall set-repl-data!-token value))

(define (terminate-command-processor! status)
  (upcall terminate-repl-token status))

(define (command-levels)
  (upcall command-levels-token))

(define (command-level)
  (car (command-levels)))

(define (top-command-level)
  (last (command-levels)))

; Command level control

(define (push-command-level thunk data)
  (upcall push-command-level-token (current-thread) thunk data (get-dynamic-env)))

(define (throw-to-command-level level thunk)
  (upcall throw-to-command-level-token level thunk))

; This makes a new level just like the old one.

(define (restart-command-level level)
  (throw-to-command-level
   level
   (lambda ()
     (really-push-command-level (command-level-repl-thunk level)
				(command-level-repl-data level)
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
; We enqueue the paused thread so that its dynamic-winds will be run.

(define (kill-paused-thread! level)
  (let ((paused (command-level-paused-thread level)))
    (if (not paused)
	(error "level has no paused thread" level))
    (if (eq? paused (command-level-repl-thread level))
	(spawn-repl-thread! level))
    (interrupt-thread paused terminate-current-thread)
;		      (lambda ignore
;			(terminate-current-thread)))
    ;(enqueue-thread! (command-level-queue level) paused)
    (set-command-level-paused-thread! level #f)))


