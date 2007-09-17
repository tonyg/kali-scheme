; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; 3.1 Process Creation and Execution

(import-dynamic-externals "=scheme48external/posix")
;
; FORK returns the child pid in the parent, and #f in the child.

(define (fork)
  (force-channel-output-ports!)
  (posix-fork))

(import-lambda-definition posix-fork ())

; Fork off a process to execute THUNK, but don't return a pid.  This avoids
; any problem with zombies.

(define (fork-and-forget thunk)
  (cond ((fork)
	 => wait-for-child-process)
	((fork)
	 (exit 0))
	(else
	 (thunk))))

;----------------
; The base Scheme procedure for exec() is EXEC-WITH-ALIAS (the `alias' is
; because the first argument may or may not be the name of the program or file).
;
; (EXEC-WITH-ALIAS program lookup? environment arguments)
;   program: byte vector, name of file or program
;   lookup?: should the program be looked up in PATH?
;   environment: either #f, which uses the parent's environment in the child,
;            or a list of byte vectors, representing text of the form "name=value".
;   arguments: a list of byte vectors

(import-lambda-definition external-exec-with-alias (program lookup? environment arguments)
			  "posix_exec")

(define (thing->exec-arg-byte-string thing)
  (os-string->byte-vector (x->os-string thing)))

(define (exec-with-alias program lookup? environment arguments)
  (external-exec-with-alias (thing->exec-arg-byte-string program)
			    lookup?
			    (and environment
				 (thing->exec-arg-byte-string environment))
			    (map thing->exec-arg-byte-string arguments)))

; Four versions of exec():
;  - program looked up, use default environment
;  - program looked up, environment argument
;  - file used as-is, use default environment
;  - file used as-is, environment argument
;
; In all cases, the program or file is added to the beginning of the list
; of arguments.
;
; When given, ENV should be a list of strings of the form "name=value".

(define (exec program . arguments)
  (exec-with-alias program #t #f (cons program arguments)))

(define (exec-with-environment program env . arguments)
  (exec-with-alias program #t env (cons program arguments)))

(define (exec-file file . arguments)
  (exec-with-alias file #f #f (cons file arguments)))

(define (exec-file-with-environment file env . arguments)
  (exec-with-alias file #f env (cons file arguments)))

;----------------
; Process ids
;
; Threads can wait for child process to terminate.  This requires polling,
; which in turn requires that we store the child's return status or terminating
; signal for later use.  Hence the extra fields.
;
; These should probably not be dumpable.
;
; Because these have state they must be unique.  The C code keeps a weak
; list of existing ones.

(define-record-type process-id :process-id
  (process-ids-are-made-from-c)
  process-id?
  (uid process-id->integer)		; the Unix PID
  ; The rest are initially #F and are set as events warrant.
  (exit-status process-id-exit-status)
  (terminating-signal process-id-terminating-signal)
  (placeholder process-id-placeholder set-process-id-placeholder!))

(define-record-discloser :process-id
  (lambda (process-id)
    (list 'process-id (process-id->integer process-id))))

(define (process-id=? p1 p2)
  (if (and (process-id? p1)
	   (process-id? p2))
      (eq? p1 p2)
      (assertion-violation 'process-id=? "argument type error" p1 p2)))

; We need to make these in the outside world.
(define-exported-binding "posix-process-id-type" :process-id)

(import-lambda-definition integer->process-id (int) "posix_enter_pid")

;----------------
; 3.2 Process Termination
;
; pid_t wait(int *stat_loc)
; pid_t waitpid(pid_t pid, int *stat_loc, int options)
; void _exit(int status);         Need to do this.

; Wait for a child process.  If the child isn't already known to have terminated
; we process any waiting, terminated children and try again.  If it still hasn't
; finished we created a placeholder for it and block.

(define (wait-for-child-process pid)
  (if (not (process-id? pid))
      (assertion-violation wait-for-child-process "not a process id" pid))
  (or (process-id-exit-status pid)
      (process-id-terminating-signal pid)
      (begin
	(process-terminated-children pid)
	(disable-interrupts!)
	(or (process-id-exit-status pid)
	    (process-id-terminating-signal pid)
	    (let ((placeholder (or (process-id-placeholder pid)
				   (let ((p (make-placeholder)))
				     (set-process-id-placeholder! pid p)
				     p))))
	      (placeholder-value placeholder)))
	(enable-interrupts!)))
  (values))

; Waiting for children.  We go through the terminated child processes until we
; find the one we are looking for or we run out.  This needs to be called by
; the SIGCHLD handler.

(define (process-terminated-children . maybe-pid)
  (let ((pid (if (null? maybe-pid)
		 #f
		 (car maybe-pid))))
    (let loop ()
      (let ((next (posix-waitpid)))
	(if next
	    (let ((placeholder (process-id-placeholder next)))
	      (if placeholder
		  (begin
		    (placeholder-set! placeholder #t)
		    (set-process-id-placeholder! pid #f))) ; no longer needed
	      (if (not (eq? pid next))
		  (loop))))))))

(import-lambda-definition posix-waitpid ())

(define (exit status)
  (force-channel-output-ports!)
  (scheme-exit-now status))

