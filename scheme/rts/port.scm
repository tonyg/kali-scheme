; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Ports and port handlers

; See doc/io.txt for a description of the i/o system, including ports,
; port handlers, and so forth.

(define-record-type port-handler :port-handler
  (make-port-handler discloser close buffer-proc)
  port-handler?
  (discloser   port-handler-discloser)
  (close       port-handler-close)
  (buffer-proc port-handler-buffer-proc))

(define (disclose-port port)
  ((port-handler-discloser (port-handler port)) (port-data port)))

(define-method &disclose ((port :input-port))
  (disclose-port port))

(define-method &disclose ((port :output-port))
  (disclose-port port))

; The PORT-LOCKED? bit is checked by READ-CHAR, PEEK-CHAR, and WRITE-CHAR
; which are too simple-minded to understand real locks.  The three procedures
; below ensure that (PORT-LOCKED? <port>) is #t whenever someone has aquired
; <port>'s lock.

(define (obtain-port-lock port)
  (set-port-locked?! port #t)
  (obtain-lock (port-lock port)))

(define (maybe-obtain-port-lock port)
  (let ((ints (set-enabled-interrupts! 0)))
    (set-port-locked?! port #t)
    (let ((result (if (maybe-obtain-lock (port-lock port))
		      #t
		      (begin
			(set-port-locked?! port #f)
			#f))))
      (set-enabled-interrupts! ints)
      result)))

(define (release-port-lock port)
  (let ((ints (set-enabled-interrupts! 0)))
    (if (release-lock (port-lock port))
	(set-port-locked?! port #f))
    (set-enabled-interrupts! ints)))

; Set up exception handlers for the three unnecessary I/O primitives,
; READ-CHAR, PEEK-CHAR, and WRITE-CHAR.  These do the right thing in
; the case of buffer overflow or underflow.
;
; This is abstracted to avoid a circular module dependency.

(define (initialize-i/o-handlers! define-exception-handler signal-exception)
  (define-exception-handler (enum op read-char)
    (one-arg-proc->handler (read-char-handler #t) signal-exception))
    
  (define-exception-handler (enum op peek-char)
    (one-arg-proc->handler (read-char-handler #f) signal-exception))
  
  (define-exception-handler (enum op write-char)
    (two-arg-proc->handler write-char-handler signal-exception)))

; Check the exception and then lock the port.

(define (one-arg-proc->handler proc signal-exception)
  (lambda (opcode reason port)
    (if (= reason (enum exception buffer-full/empty))
	(begin
	  (obtain-port-lock port)
	  (let ((value (proc port)))
	    (release-port-lock port)
	    value))
	(signal-exception opcode reason port))))

; This could combined with on-arg-... if the port were the first argument.

(define (two-arg-proc->handler proc signal-exception)
  (lambda (opcode reason arg port)
    (if (= reason (enum exception buffer-full/empty))
	(begin
	  (obtain-port-lock port)
	  (let ((value (proc arg port)))
	    (release-port-lock port)
	    value))
	(signal-exception opcode reason arg port))))

; If a character is available, use it; if there is an EOF waiting, use that;
; otherwise fill the buffer and go from there.

(define (read-char-handler read?)
  (lambda (port)
    (cond ((< (port-index port) (port-limit port))
	   (let ((index (port-index port)))
	     (if read?
		 (set-port-index! port (+ 1 index)))
	     (ascii->char (code-vector-ref (port-buffer port) index))))
	  ((port-pending-eof? port)
	   (if read?
	       (set-port-pending-eof?! port #f))
	   (eof-object))
	  (else
	   (let ((got (fill-port-buffer! port 'any)))
	     (cond ((or (eof-object? got)
			(= 0 got))
		    (if (not read?)
			(set-port-pending-eof?! port #t))
		    (eof-object))
		   (else
		    (set-port-index! port (if read? 1 0))
		    (set-port-limit! port got)
		    (ascii->char (code-vector-ref (port-buffer port) 0)))))))))

(define (fill-port-buffer! port needed)
  ((port-handler-buffer-proc (port-handler port))
   (port-data port)
   (port-buffer port)
   0
   needed))

; Write the character if there is room, or call the handler if there is
; no actual buffering.  Otherwise use the handler to empty the buffer.

(define (write-char-handler char port)
  (cond ((< (port-index port) (port-limit port))
	 (code-vector-set! (port-buffer port)
			   (port-index port)
			   (char->ascii char))
	 (set-port-index! port (+ 1 (port-index port))))
	((= 0 (port-limit port))
	 ((port-handler-buffer-proc (port-handler port)) (port-data port) char))
	(else
	 (empty-port-buffer! port)
	 (code-vector-set! (port-buffer port) 0 (char->ascii char))
	 (set-port-index! port 1)))
  (unspecific))

; This may change PORT's buffer.

(define (empty-port-buffer! port)
  ((port-handler-buffer-proc (port-handler port))
   (port-data port)
   (port-buffer port)
   0
   (port-index port))
  (set-port-index! port 0)
  (set-port-flushed?! port #t))

(define port-flushed? port-pending-eof?)
(define set-port-flushed?! set-port-pending-eof?!)

;----------------
; Closing is done with the appropriate handler.
; R4RS says that CLOSE-... is idempotent.

(define (close-input-port port)
  (if (input-port? port)
      (begin
	(obtain-port-lock port)
	(if (open-input-port? port)
	    (begin
	      (make-input-port-closed! port)
	      ((port-handler-close (port-handler port)) (port-data port))))
	(release-port-lock port))
      (call-error "invalid argument" close-input-port port)))
	
(define (close-output-port port)
  (if (output-port? port)
      (begin
	(obtain-port-lock port)
	(if (open-output-port? port)
	    (begin
	      (really-force-output port)
	      (make-output-port-closed! port)
	      ((port-handler-close (port-handler port)) (port-data port))))
	(release-port-lock port))
      (call-error "invalid argument" close-output-port port)))

;----------------
; Port operations that do not have opcodes.  Each of these needs to check its
; arguments and then lock the port.  A macro is used to add the checking and
; locking code.  This has to check that the port is a port before locking it
; and then check that it is open after locking it.

; A macro to handle locking ports and checking that they're open.

(define-syntax define-port-op
  (syntax-rules ()
    ((define-port-op (?id ?args ...) ?port ?predicate ?body)
     (define (?id ?args ...)
       (if ?predicate                         ; if args are okay
	   (begin
	     (obtain-port-lock ?port)         ; lock the port
	     (if (open-port? ?port)           ; check that it's open
		 (let ((value ?body))         ; do the work
		   (release-port-lock ?port)  ; release the lock
		   value)                     ; return
		 (begin
		   (release-port-lock ?port)
		   (call-error "invalid argument" ?id ?args ...))))
	   (call-error "invalid argument" ?id ?args ...))))

    ; ?port defaults to the first argument
    ((define-port-op (?id ?port ?args ...) ?predicate ?body)
     (define-port-op (?id ?port ?args ...) ?port ?predicate ?body))))

;----------------
; See if there is a character available.  CHAR-READY? itself is defined
; in current-ports.scm as it needs CURRENT-INPUT-PORT when called with
; no arguments.

(define (real-char-ready? port)
  (cond ((not (open-input-port? port))
	 (call-error "invalid argument" char-ready? port))
	((not (maybe-obtain-port-lock port))
	 #f)
	((not (open-port? port))  ; have to check again after the lock call
	 (release-port-lock port)
	 (call-error "invalid argument" char-ready? port))
	((or (< (port-index port) (port-limit port))
	     (port-pending-eof? port))
	 (release-port-lock port)
	 #t)
	(else
	 (let ((got (fill-port-buffer! port 'immediate)))
	   (cond ((eof-object? got)
		  (set-port-pending-eof?! port #t)
		  (release-port-lock port)
		  #t)
		 (else
		  (set-port-index! port 0)
		  (set-port-limit! port got)
		  (release-port-lock port)
		  (> got 0)))))))

;----------------
; Check the arguments and the state of the buffer.  Leave any actual work
; up to the next procedure.
;
; The arguments are the same as those for input port handlers: BUFFER is either
; a string or a code vector, START is the initial index, and COUNT is either
; the required number of characters, 'ANY (one or more characters wanted),
; or 'IMMEDIATE (get what's available, but don't block).

(define-port-op (read-block buffer start count port) port
  (and (open-input-port? port)
       (okay-limits? buffer start (if (or (eq? count 'any)
					  (eq? count 'immediate))
				      1
				      count)))
  (cond ((port-pending-eof? port)
	 (set-port-pending-eof?! port #f)
	 (eof-object))
	((eq? count 0)
	 0)
	(else
	 (really-read-block buffer start count port))))

; If nothing is available, call the port's handler procedure.  Otherwise,
; copy any available characters into the buffer.  If more are needed the
; buffer handler is called.

(define (really-read-block buffer start count port)
  (let ((have (- (port-limit port) (port-index port))))
    (if (= have 0)
	((port-handler-buffer-proc (port-handler port))
	   (port-data port)
	   buffer
	   start
	   count)
	(let ((move (min have
			 (if (symbol? count)
			     (- (buffer-length buffer) start)
			     count))))
	  (copy-bytes! (port-buffer port) (port-index port) buffer start move)
	  (set-port-index! port (+ (port-index port) move))
	  (if (or (symbol? count)
		  (= move count))
	      move
	      (read-more buffer start count port move))))))

(define (buffer-length buffer)
  (if (string? buffer)
      (string-length buffer)
      (code-vector-length buffer)))

(define (read-more buffer start count port have)
  (let ((more ((port-handler-buffer-proc (port-handler port))
	         (port-data port)
		 buffer
		 (+ start have)
		 (- count have))))
    (if (eof-object? more)
	(if (= 0 have)
	    (eof-object)
	    (begin
	      (set-port-pending-eof?! port #t)
	      have))
	(+ more have))))
    
; Check that BUFFER contains COUNT characters starting from START.

(define (okay-limits? buffer start count)
  (and (integer? start)
       (exact? start)
       (<= 0 start)
       (integer? count)
       (exact? count)
       (<= 0 count)
       (<= (+ start count)
	   (cond ((string? buffer)
		  (string-length buffer))
		 ((code-vector? buffer)
		  (code-vector-length buffer))
		 (else
		  -1)))))

;----------------
; Write the COUNT bytes beginning at START from BUFFER to PORT.

(define-port-op (write-block buffer start count port) port
  (and (open-output-port? port)
       (okay-limits? buffer start count))
  (if (= 0 (port-limit port))
      (write-unbuffered-block buffer start count port)
      (write-buffered-block buffer start count port)))

; WRITE-STRING is a front for WRITE-BLOCK.

(define (write-string string port)
  (write-block string 0 (string-length string) port))

; Copy the bytes into the buffer if there is room, otherwise write out anything
; in the buffer and then write BUFFER.

(define (write-buffered-block buffer start count port)
  (let ((space (- (port-limit port) (port-index port))))
    (cond ((>= space count)
	   (copy-bytes! buffer start (port-buffer port) (port-index port) count)
	   (set-port-index! port (+ (port-index port) count)))
	  (else
	   (really-force-output port)
	   ((port-handler-buffer-proc (port-handler port))
	    (port-data port) buffer start count)))))

; For unbuffered ports we have to call the handler on each character
; separately.

(define (write-unbuffered-block buffer start count port)
  (let ((proc (port-handler-buffer-proc (port-handler port)))
        (data (port-data port))
        (string? (string? buffer)))
    (do ((i 0 (+ i 1)))
        ((= i count))
      (proc data (if string?
                     (string-ref buffer (+ start i))
                     (ascii->char (code-vector-ref buffer (+ start i))))))))

;----------------
; Empty the buffer if it contains anything.

(define-port-op (force-output port)
  (open-output-port? port)
  (really-force-output port))

(define (really-force-output port)
  (if (< 0 (port-index port))
      (empty-port-buffer! port)))

; Used to avoid race conditions elsewhere.

(define (force-output-if-open port)
  (if (output-port? port)
      (begin
	(obtain-port-lock port)
	(if (open-output-port? port)
	    (really-force-output port))
	(release-port-lock port))
      (call-error "invalid argument" force-output-if-open port)))

;----------------

(define default-buffer-size 4096)  ; should get this from the system

;----------------
; Is PORT open?

(define (open-port? port)
  (not (= 0 (bitwise-and open-port-mask (port-status port)))))

(define open-port-mask
  (bitwise-ior (arithmetic-shift 1 (enum port-status-options open-for-input))
	       (arithmetic-shift 1 (enum port-status-options open-for-output))))

;----------------
; Input ports

(define input-port-mask
  (arithmetic-shift 1 (enum port-status-options input)))

(define open-input-port-mask
  (arithmetic-shift 1 (enum port-status-options open-for-input)))

(define open-input-port-status
  (bitwise-ior input-port-mask open-input-port-mask))

(define (open-input-port? port)
  (not (= 0 (bitwise-and open-input-port-mask (port-status port)))))

(define (make-input-port-closed! port)
  (set-port-status! port (bitwise-and (port-status port)
				      (bitwise-not open-input-port-mask))))

(define (make-input-port handler data buffer index limit)
  (if (and (okay-buffer? buffer index limit)
	   (port-handler? handler))
      (make-port handler
		 (bitwise-ior input-port-mask open-input-port-mask)
		 (make-lock)
		 #f             ; locked?
		 data
		 buffer
		 index
		 limit
		 #f)            ; pending-eof?
      (call-error "invalid argument"
		  make-input-port handler data buffer index limit)))

(define (okay-buffer? buffer index limit)
  (and (code-vector? buffer)
       (let ((length (code-vector-length buffer)))
	 (integer? limit)
	 (<= 0 limit)
	 (<= limit length)
	 (<= index limit)
	 (integer? index)
	 (<= 0 index)
	 (<= index limit))))

;----------------
; Output ports

(define output-port-mask
  (arithmetic-shift 1 (enum port-status-options output)))

(define open-output-port-mask
  (arithmetic-shift 1 (enum port-status-options open-for-output)))

(define open-output-port-status
  (bitwise-ior output-port-mask open-output-port-mask))

(define (open-output-port? port)
  (not (= 0 (bitwise-and open-output-port-mask (port-status port)))))

(define (make-output-port-closed! port)
  (set-port-status! port (bitwise-and (port-status port)
				      (bitwise-not open-output-port-mask))))

(define (make-output-port handler data buffer index limit)
  (if (and (okay-buffer? buffer index limit)
	   (> limit 0)
	   (port-handler? handler))
      (make-port handler
		 open-output-port-status
		 (make-lock)
		 #f             ; locked?
		 data
		 buffer
		 index
		 limit
		 #f)            ; pending-eof?
      (call-error "invalid argument"
		  make-output-port handler data buffer index limit)))

(define (make-unbuffered-output-port handler data)
  (if (port-handler? handler)
      (make-port handler
		 open-output-port-status
		 (make-lock)
		 #f             ; locked?
		 data
		 (make-code-vector 0 0)
		 0
		 0
		 #f)            ; pending-eof?
      (call-error "invalid argument"
		  make-unbuffered-output-port handler data)))

(define null-output-port-handler
  (make-port-handler
   (lambda (ignore)
     (list 'null-output-port))
   (lambda (ignore)
     (unspecific))
   (lambda (channel buffer start need)
     (unspecific))))

(define (make-null-output-port)
  (make-port null-output-port-handler
	     open-output-port-status
	     (make-lock)    ; wasted
	     #f             ; locked?
	     (unspecific)
	     (make-code-vector 1 0)
	     0
	     1              ; if 0 it would look unbuffered
	     #f))           ; pending-eof?

;----------------
; Code to periodically flush output ports.

(define *flush-these-ports* (cons #f '()))

(define (initialize-output-port-list!)
  (set! *flush-these-ports* (cons #f '())))

(define (periodically-force-output! port)
  (set-cdr! *flush-these-ports*
	    (cons (make-weak-pointer port) (cdr *flush-these-ports*))))

; Return a list of thunks that will flush the buffer of each open, unlocked
; port that contains characters that have been there since the last time
; this was called.  The actual i/o is done using separate threads to keep
; i/o errors from killing anything vital.
; 
; If USE-FLUSHED?-FLAGS? is true this won't flush buffers that have been
; flushed by someone else sinse the last call.  If it is false then flush
; all non-empty buffers, because the system has nothing to do and is going
; to pause while waiting for external events.

(define (output-port-forcers use-flushed?-flags?)
  (let loop ((next (cdr *flush-these-ports*))
             (last *flush-these-ports*)
             (thunks '()))
    (if (null? next)
        thunks
        (let ((port (weak-pointer-ref (car next))))
          (cond ((or (not port)                      ; GCed or closed
                     (not (open-output-port? port))) ;  so drop it from the list
                 (set-cdr! last (cdr next))
                 (loop (cdr next) last thunks))
                ((not (maybe-obtain-port-lock port)) ; locked
                 (loop (cdr next) next thunks))
                ((and use-flushed?-flags?            ; flushed recently
                      (port-flushed? port))
                 (set-port-flushed?! port #f)
                 (release-port-lock port)
                 (loop (cdr next) next thunks))
                ((< 0 (port-index port))              ; non-empty
                 (release-port-lock port)
                 (loop (cdr next) next
		       (cons (make-forcing-thunk port)
			     thunks)))
                (else                                 ; empty
                 (release-port-lock port)
                 (loop (cdr next) next thunks)))))))

; Returns a list of the current ports that are flushed whenever.
; This is used to flush channel ports before forking.

(define (periodically-flushed-ports)
  (let ((ints (set-enabled-interrupts! 0)))
    (let loop ((next (cdr *flush-these-ports*))
	       (last *flush-these-ports*)
	       (ports '()))
      (if (null? next)
	  (begin
	    (set-enabled-interrupts! ints)
	    ports)
	  (let ((port (weak-pointer-ref (car next))))
	    (cond ((or (not port)                      ; GCed or closed
		       (not (open-output-port? port))) ; so drop it from the list
		   (set-cdr! last (cdr next))
		   (loop (cdr next) last ports))
		  (else
		   (loop (cdr next)
			 next
			 (cons port ports)))))))))

; Write out PORT's buffer.  If a problem occurs it is reported and PORT
; is closed.

(define (make-forcing-thunk port)
  (lambda ()
    (if (and (report-errors-as-warnings
	      (lambda ()
		(if (maybe-obtain-port-lock port)
		    (begin
		      (really-force-output port)
		      (release-port-lock port))))
	      "error when flushing buffer; closing port"
	      port)
	     (open-output-port? port))
	(report-errors-as-warnings
	   (lambda ()
	     (make-output-port-closed! port)
	     ((port-handler-close (port-handler port)) (port-data port)))
	   "error when closing port"
	   port))))
	
			       
