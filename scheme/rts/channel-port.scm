; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Ports built on OS channels.

; Repeatedly calls CHANNEL-READ until enough characters have been obtained.
; There are three different conditions under which the buffer-procedure
; is called:
;  1) any number of characters is fine, but an immediate response is
;     needed (the originating procedure was CHAR-READY?)
;  2) wait for at least one character, but not after that (from READ-CHAR)
;  3) exactly NEEDED characters are required (from READ-BLOCK)

(define input-channel-handler
  (make-port-handler
   (lambda (channel)
     (list 'input-port channel))
   (lambda (channel)
     (close-channel channel))
   (lambda (channel buffer start needed)
     (channel-read buffer start needed channel))))

(define (input-channel->port channel . maybe-buffer-size)
  (let ((buffer-size (if (null? maybe-buffer-size)
			 default-buffer-size
			 (car maybe-buffer-size))))
    (if (>= 0 buffer-size)
	(call-error "invalid buffer size" input-channel->port channel buffer-size)
	(make-input-port input-channel-handler
			 channel
			 (make-code-vector buffer-size 0)
			 0
			 0))))
  
(define output-channel-handler
  (make-port-handler
   (lambda (channel)
     (list 'output-port channel))
   (lambda (channel)
     (close-channel channel))
   (lambda (channel buffer start count)
     (channel-write buffer start count channel))))

; Unbuffered channel output ports.  Used for the default error port.

(define (make-unbuffered-output-channel-handler)
  (let ((buffer (make-code-vector 1 0)))
    (make-port-handler
     (lambda (channel)
       (list 'output-port channel))
     (lambda (channel)
       (close-channel channel))
     (lambda (channel char)
       (code-vector-set! buffer 0 (char->ascii char))
       (channel-write buffer 0 1 channel)))))

; Dispatch on the buffer size to make the appropriate port.  A buffer
; size of zero creates an unbuffered port.  Buffered output ports get a
; finalizer to flush the buffer if the port is GC'ed.

(define (output-channel->port channel . maybe-buffer-size)
  (let ((buffer-size (if (null? maybe-buffer-size)
			 default-buffer-size
			 (car maybe-buffer-size))))
    (cond ((> 0 buffer-size)
	   (call-error "invalid buffer size"
		       output-channel->port channel buffer-size))
	  ((= 0 buffer-size)
	   (make-unbuffered-output-port (make-unbuffered-output-channel-handler)
					channel))
	  (else
	   (let ((port (make-output-port output-channel-handler
					 channel
					 (make-code-vector buffer-size 0)
					 0
					 buffer-size)))
	     (periodically-force-output! port)
	     ((structure-ref primitives add-finalizer!) port
							maybe-force-output)
	     port)))))
	     
; Flush PORT's output buffer if the port isn't locked.

(define (maybe-force-output port)
  (cond ((maybe-obtain-lock (port-lock port))
	 (if (open-output-port? port)
	     (really-force-output port))
	 (release-lock (port-lock port)))))

;----------------
; Various ways to open ports on files.

; First a generic procedure to do the work.

(define (maybe-open-file filename option coercion)
  (let ((channel (open-channel filename option)))
    (if channel
	(coercion channel default-buffer-size)
	#f)))
  
; And then all of RnRS's file opening procedures.

(define (open-input-file string)
  (if (string? string)
      (or (maybe-open-file string
			   (enum channel-status-option input)
			   input-channel->port)
	  (error "can't open for input" string))
      (call-error "invalid argument" open-input-file string)))

(define (open-output-file string)
  (if (string? string)
      (or (maybe-open-file string
			   (enum channel-status-option output)
			   output-channel->port)
	  (error "can't open for output" string))
      (call-error "invalid argument" open-output-file string)))

(define (call-with-input-file string proc)
  (let* ((port (open-input-file string))
         (results (call-with-values (lambda () (proc port))
				    list)))
    (close-input-port port)
    (apply values results)))

(define (call-with-output-file string proc)
  (let* ((port (open-output-file string))
         (results (call-with-values (lambda () (proc port))
				    list)))
    (close-output-port port)
    (apply values results)))

(define (with-input-from-file string thunk)
  (call-with-input-file string
    (lambda (port)
      (call-with-current-input-port port thunk))))

(define (with-output-to-file string thunk)
  (call-with-output-file string
    (lambda (port)
      (call-with-current-output-port port thunk))))

;----------------
; The following is used to make the REPL's input, output, and error ports
; available after a keyboard interrupt.  If PORT is a locked channel port
; we save the its state and then reinitialize it.  The OS is told to
; abort any pending operation on the port's channel.  Finally, the owning
; thread's continuation is munged to restore the port when the thread
; resumes.  Any buffered input is thrown away at that point (it could
; be saved away with the channel).
; 
; SPLICE-CONTINUATION! is passed as an argument to avoid a module dependency.
; Any values being returned to OWNER are thrown away.  The assumption is that
; no upcalls are done while the port is locked.  SPLICE-CONTINUATION! could
; be hacked to fix the problem.
;
; If the port is locked by us or one of our ancestors there is no point in
; trying to grab it.

(define (steal-channel-port! port splice-continuation!)
  (if (channel? (port-data port))
      (begin
	(disable-interrupts!)
	(let ((owner (if (lock-owner-uid (port-lock port))
			 (thread-uid->thread (lock-owner-uid (port-lock port)))
			 #f)))
	  (if (and owner
		   (not (running? owner)))
	      (really-steal-channel-port! port owner splice-continuation!))
	  (enable-interrupts!)))))

(define (really-steal-channel-port! port owner splice-continuation!)
  (let ((lock (port-lock port))
	(buffer (port-buffer port))
	(index (port-index port))
	(limit (port-limit port))
	(eof? (port-pending-eof? port))
	(status (steal-channel! (port-data port) owner)))
    (set-port-buffer! port (make-code-vector (code-vector-length buffer) 0))
    (set-port-index! port 0)
    (set-port-limit! port (if (input-port? port) 0 (code-vector-length buffer)))
    (set-port-pending-eof?! port #f)
    (set-port-locked?! port #f)
    (set-port-lock! port (make-lock))
    (interrupt-thread owner
		      (lambda results
			(obtain-lock (port-lock port))
			(cond ((output-port? port)
			       (really-force-output port))
			      ((< (port-index port)
				  (port-limit port))
			       (warn "dropping input from port" port)))
			(set-port-buffer! port buffer)
			(set-port-index! port index)
			(set-port-limit! port limit)
			(set-port-pending-eof?! port eof?)
			(set-port-lock! port lock)
			(or status (apply values results))))
    ; if we took OWNER off a channel-wait queue we need to make it ready to run
    (if status (make-ready owner))))
