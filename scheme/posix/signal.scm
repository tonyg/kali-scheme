; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; 3.3 Signals
;
; int kill(pid_t pid, int sig)
;
; Also signal sets, sigaction(), blocked and waiting signals,
; alarm(), pause(), sleep().
;
; There are two signal record types, one for signals whose names we know and
; one for those which are anonymous.  The anonymous signals cannot be dumped
; in images, because we have no way of knowing if they will have the same
; meaning on startup.
;
; Only the C code can determine which names correspond to signals in the current
; OS, and then only at compile time by doing
;   #ifdef SIGSTKFLT		/* or whatever the name is */
;     ...
;   #endif
; So what we do is construct a C file that checks for the presence of all of the
; signals we have ever heard of.  Any others have to be anonymous and cannot be
; saved in images.

(define-record-type unnamed-signal :unnamed-signal
  (unnamed-signals-are-made-by-c-code)
  unnamed-signal?
  (resume-value unnamed-signal-resume-value)
  (os-number    unnamed-signal-os-number)
  (queues	unnamed-signal-queues set-unnamed-signal-queues!))

(define-record-discloser :unnamed-signal
  (lambda (u-s)
    (list 'signal (unnamed-signal-os-number u-s))))

; These are not meaningful after a dump (because the value may not have the
; same meaning on the OS on which we are resumed).

(define-record-resumer :unnamed-signal #f)

(define-finite-type signal :named-signal
  (queues)
  named-signal?
  named-signals
  named-signal-name
  named-signal-index
  (os-number named-signal-os-number set-named-signal-os-number!)
  (queues named-signal-queues set-named-signal-queues!)
  (; POSIX
   (abrt '())	; abort - abnormal termination (as by abort())
   (alrm '())	; alarm - timeout signal (as by alarm())
   (fpe  '())	; floating point exception
   (hup  '())	; hangup - hangup on controlling terminal or death of
   		;  controlling process
   (ill  '())	; illegal instruction
   (int  '())	; interrupt - interaction attention
   (kill '())	; kill - termination signal, cannot be caught or ignored
   (pipe '())	; pipe - write on a pipe with no readers
   (quit '())	; quit - interaction termination
   (segv '())	; segmentation violation - invalid memory reference
   (term '())	; termination - termination signal
   (usr1 '())	; user1 - for use by applications
   (usr2 '())	; user2 - for use by applications
   (chld '())	; child - child process stopped or terminated
   (cont '())	; continue - continue if stopped
   (stop '())	; stop - cannot be caught or ignored
   (tstp '())	; interactive stop
   (ttin '())	; read from control terminal attempted by background process
   (ttou '())	; write to control terminal attempted by background process
   (bus  '())	; bus error - access to undefined portion of memory

   ; Additional signals from (RedHat) Linux
   (trap   '())	; trace or breakpoint trap
   (iot    '())	; IOT trap - a synonym for ABRT
   (emt    '())	; ambulance?
   (sys    '())	; bad argument to routine	(SVID)
   (stkflt '())	; stack fault on coprocessor
   (urg    '())	; urgent condition on socket	(4.2 BSD)
   (io     '())	; I/O now possible		(4.2 BSD)
   (poll   '())	; A synonym for SIGIO		(System V)
   (cld    '())	; A synonym for SIGCHLD
   (xcpu   '())	; CPU time limit exceeded	(4.2 BSD)
   (xfsz   '())	; File size limit exceeded	(4.2 BSD)
   (vtalrm '())	; Virtual alarm clock		(4.2 BSD)
   (prof   '())	; Profile alarm clock
   (pwr    '())	; Power failure			(System V)
   (info   '())	; A synonym for SIGPWR
   (lost   '())	; File lock lost
   (winch  '())	; Window resize signal		(4.3 BSD, Sun)
   (unused '())	; Unused signal
   ))

(define-record-discloser :named-signal
  (lambda (n-s)
    (list 'signal (named-signal-name n-s))))

; Find the signal called `name'.

(define (name->signal name)
  (if (not (symbol? name))
      (call-error "argument not a symbol" name->signal name)
      (let loop ((i 0))
	(cond ((= i (vector-length named-signals))
	       #f)
	      ((eq? name
		    (named-signal-name
		      (vector-ref named-signals i)))
	       (vector-ref named-signals i))
	      (else
	       (loop (+ i 1)))))))

;----------------
; Code to produce a C include file that checks whether each signal is defined.
; The output file looks like:
;
; signal_count_is(<number of signals>);
; #ifdef SIGABRT
; signal_map[0] = SIGABRT;
; #endif
; #ifdef SIGALRM
; signal_map[1] = SIGALRM;
; #endif
; ...

(define (write-c-signal-include-file filename)
  (call-with-output-file filename
    (lambda (out)
      (display (string-append "signal_count_is("
			      (number->string (vector-length named-signals))
			      ");"
			      newline-string)
	       out)
      (do ((i 0 (+ i 1)))
	  ((= i (vector-length named-signals)))
	(let ((name (symbol->string
		     (named-signal-name
		      (vector-ref named-signals i)))))
	  (display (string-append "#ifdef SIG" (string-upcase name)
				  newline-string
				  "signal_map["
				  (number->string i)
				  "] = SIG"
				  (string-upcase name)
				  ";"
				  newline-string
				  "#endif"
				  newline-string)
		   out))))))

(define newline-string (list->string '(#\newline)))

(define (string-map proc)
  (lambda (list)
    (list->string (map proc (string->list list)))))

(define string-upcase   (string-map char-upcase))
(define string-downcase (string-map char-downcase))

;----------------
; Dispatching on the two kinds of signals.

(define (signal? x)
  (or (named-signal? x)
      (unnamed-signal? x)))

(define (signal-name x)
  (cond ((named-signal? x)
	 (named-signal-name x))
	((unnamed-signal? x)
	 #f)
	(else
	 (call-error "argument not a signal" signal-name x))))

(define (signal-os-number x)
  (cond ((named-signal? x)
	 (named-signal-os-number x))
	((unnamed-signal? x)
	 (unnamed-signal-os-number x))
	(else
	 (call-error "argument not a signal" signal-os-number x))))

(define (signal-queues x)
  (cond ((named-signal? x)
	 (named-signal-queues x))
	((unnamed-signal? x)
	 (unnamed-signal-queues x))
	(else
	 (call-error "argument not a signal" signal-queues x))))

(define (set-signal-queues! x qs)
  (cond ((named-signal? x)
	 (set-named-signal-queues! x qs))
	((unnamed-signal? x)
	 (set-unnamed-signal-queues! x qs))
	(else
	 (call-error "argument not a signal" set-signal-queues! x qs))))

(define (clean-signal-queues x)
  (let* ((old (signal-queues x))
	 (new (clean-weaks old)))
    (if (not (eq? new old))
	(set-signal-queues! x new))
    new))

; Two signals are the same if they are exactly the same or if they are
; both named signals and have the same (non-#F) os number.

(define (signal=? s1 s2)
  (or (eq? s1 s2)
      (and (named-signal? s1)
	   (named-signal? s2)
	   (named-signal-os-number s1)
	   (eq? (named-signal-os-number s1)
		(named-signal-os-number s2)))))

;----------------
; What we contribute to and receive from the C layer.

(define-exported-binding "posix-signals-vector"        named-signals)
(define-exported-binding "posix-named-signal-type"     :named-signal)
(define-exported-binding "posix-unnamed-signal-type"   :unnamed-signal)
(define-exported-binding "posix-unnamed-signal-marker" 'nonportable-signal)

(import-lambda-definition integer->signal (int) "posix_integer_to_signal")
(import-lambda-definition initialize-named-signals ()
			  "posix_initialize_named_signals")
(import-lambda-definition request-interrupts! (os-number)
			  "posix_request_interrupts")
(import-lambda-definition cancel-interrupt-request! (os-number)
			  "posix_cancel_interrupt_request")

;----------------
; A vector mapping os-signal numbers to signals and add to it any signals
; that have existing signal queues.

(define os-signal-map (make-session-data-slot! #f))

; Initializing the above vector.

(define (initialize-signals)
  (let ((ints (set-enabled-interrupts! no-interrupts)))
    (initialize-named-signals)
    (let* ((named (vector->list named-signals))
	   (size (+ 1 (apply max
			     (map (lambda (signal)
				    (or (signal-os-number signal)
					-1))
				  named))))
	   (mapper (make-vector size '())))
      (for-each (lambda (signal)
		  (if (and (signal-os-number signal)
			   (not (null? (clean-signal-queues signal))))
		      (let* ((number (signal-os-number signal))
			     (old (vector-ref mapper number)))
			(if (null? old)
			    (request-interrupts! number))
			(vector-set! mapper number (cons signal old)))))
		named)
      (session-data-set! os-signal-map mapper)
      (set-enabled-interrupts! ints))))

; Add SIGNAL to the list of those waiting for that signal number from the OS.
; If this is the first such we tell the OS we want the signal.
;
; Called with interrupts disabled.

(define (maybe-request-os-signal! signal)
  (let* ((os-number (signal-os-number signal))
	 (mapper (session-data-ref os-signal-map))
	 (mapper (if (< os-number (vector-length mapper))
		     mapper
		     (let ((new (make-vector (+ os-number 1) '())))
		       (do ((i 0 (+ i 1)))
			   ((= i (vector-length mapper)))
			 (vector-set! new i (vector-ref mapper i)))
		       (session-data-set! os-signal-map new)
		       new)))
	 (old (vector-ref mapper os-number)))
    (if (not (memq signal old))
	(begin
	  (vector-set! mapper os-number (cons signal old))
	  (if (null? old)
	      (request-interrupts! os-number))))))

;----------------
; Sending a signal to a process.

(import-lambda-definition signal-process (pid signal) "posix_kill")

;----------------
; Handling signals sent to the current process.  Runs with interrupts disabled.
;
; SIGCHLD has meaning for the POSIX layer, nothing else does.  The proc code
; could create a queue to receive SIGCHLD signals, but that would require a
; separate thread.  That would be too much mechanism.
;
; Find the list of signals for OS-NUMBER and then deliver the signal to each.
; If no one really wants it we tell the OS layer to stop delivering it to us.

(define (os-signal-handler os-number enabled-interrupts)
  (if (= os-number (signal-os-number (signal chld)))
      (process-terminated-children))
  (let ((mapper (session-data-ref os-signal-map)))
    (if (<= (vector-length mapper)
	    os-number)
	(cancel-interrupt-request! os-number)
	(let ((signals (vector-ref mapper os-number)))
	  (let loop ((signals signals)
		     (okay '()))
	    (cond ((null? signals)
		   (if (null? okay)
		       (cancel-interrupt-request! os-number))
		   (vector-set! mapper os-number okay))
		  (else
		   (loop (cdr signals)
			 (if (deliver-signal (car signals))
			     (cons (car signals) okay)
			     okay)))))))))

(set-interrupt-handler! (enum interrupt os-signal) os-signal-handler)

; Send SIGNAL to each of its queues.

(define (deliver-signal signal)
  (let loop ((queues (signal-queues signal))
	     (okay '()))
    (cond ((null? queues)
	   (set-signal-queues! signal okay)
	   (not (null? okay)))
	  (else
	   (loop (cdr queues)
		 (cond ((weak-pointer-ref (car queues))
			=> (lambda (queue)
			     (if (memq signal
				       (signal-queue-signals queue))
				 (begin
				   (pipe-push! (signal-queue-pipe queue)
					       signal)
				   (cons (car queues)
					 okay))
				 okay)))
		       (else
			okay)))))))

; Adding and removing QUEUE to the list of queues receiving SIGNAL.
;
; 1. check to see if it is already doing so.
; 2. add the signal to the queue and the (weak) queue to the signal
; 3. check that the os-signal has been requested

(define (add-signal-queue-signal! queue signal)
  (let ((ints (set-enabled-interrupts! no-interrupts)))
    (if (not (memq signal (signal-queue-signals queue)))
	(begin
	  (set-signal-queues! signal
			      (cons (signal-queue-weak queue)
				    (signal-queues signal)))
	  (set-signal-queue-signals! queue
				     (cons signal
					   (signal-queue-signals queue)))
	  (maybe-request-os-signal! signal)))
    (set-enabled-interrupts! ints)))

; Undo the above actions.  The signal will be un-requested the next time it
; is delivered.

(define (remove-signal-queue-signal! queue signal)
  (let ((ints (set-enabled-interrupts! no-interrupts)))
    (if (memq signal (signal-queue-signals queue))
	(begin
	  (set-signal-queues! signal (delq (signal-queue-weak queue)
					   (signal-queues signal)))
	  (set-signal-queue-signals! signal
				     (delq signal
					   (signal-queue-signals queue)))))
    (set-enabled-interrupts! ints)))

(define (delq signal signals)
  (let recur ((signals signals))
    (cond ((null? signals)
	   '())
	  ((eq? signal (car signals))
	   (cdr signals))
	  (else
	   (cons (car signals)
		 (recur (cdr signals)))))))

; A signal queue has:
;  - list of the the signals of interest
;  - pipe for received signals
;  - stashed weak pointer for adding to signals' queue lists

(define-record-type signal-queue :signal-queue
  (really-make-signal-queue signals pipe)
  signal-queue?
  (signals signal-queue-signals set-signal-queue-signals!)
  (pipe    signal-queue-pipe)
  (weak    signal-queue-weak set-signal-queue-weak!))

; Exported version that copies the list.

(define (signal-queue-monitored-signals queue)
  (apply list (signal-queue-signals queue)))

(define (make-signal-queue signals)
  (let ((queue (really-make-signal-queue '() (make-pipe))))
    (set-signal-queue-weak! queue (make-weak-pointer queue))
    (for-each (lambda (signal)
		(add-signal-queue-signal! queue signal))
	      signals)
    queue))

; (read-signal <signal-queue>) -> <signal> is analogous to
; (read-char <input-port>) -> <char>
; MAYBE-READ-SIGNAL returns #f if there are no signals currently on the queue.

(define (dequeue-signal! queue)
  (find-next-signal queue pipe-read!))

(define (maybe-dequeue-signal! queue)
  (find-next-signal queue pipe-maybe-read!))

(define (find-next-signal queue pipe-reader)
  (let ((pipe (signal-queue-pipe queue)))
    (let loop ()
      (let ((signal (pipe-reader pipe)))
	(cond ((not signal)
	       #f)
	      ((memq signal (signal-queue-signals queue))
	       signal)
	      (else
	       (loop)))))))

;----------------
; Returns the weak-pointers in WEAKS that still have values.  No copying is done
; if all have values.

(define (clean-weaks weaks)
  (let recur ((weaks weaks) (top? #t))
    (cond ((null? weaks)
	   (if top? '() #f))
	  ((weak-pointer-ref (car weaks))
	   (let ((rest (recur (cdr weaks) #f)))
	     (if rest
		 (cons (car weaks)
		       rest)
		 weaks)))
	  (else
	   (recur (cdr weaks) #t)))))

;----------------
; Initialize signals now ...

(initialize-signals)

; ... and on later startups.

(define-reinitializer signal-reinitializer initialize-signals)

