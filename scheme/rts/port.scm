; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Ports and port handlers

; See doc/io.txt for a description of the i/o system, including ports,
; port handlers, and so forth.

;  (discloser <port>) -> (<symbol> <value> ...)
;  (close <port>) -> whatever
;
; Input ports
;  (char <port>) -> <char>
;  (block <port> <buffer> <start> <count>) -> <char count>
;  (char-ready? <port>) -> <boolean>
;
; Output ports
;  (char <port> <char>) -> whatever
;  (block <port> <buffer> <start> <count>) -> whatever
;  (force-output <port>) -> whatever

(define-record-type port-handler :port-handler
  (make-port-handler discloser close char block ready? force)
  port-handler?
  (discloser port-handler-discloser)
  (close     port-handler-close)
  (char      port-handler-char)
  (block     port-handler-block)
  (ready?    port-handler-ready?)
  (force     port-handler-force))		; only used for output

;----------------
; Disclosing ports by calling the disclose handler.

(define (disclose-port port)
  ((port-handler-discloser (port-handler port))
     port))

(define-method &disclose ((port :input-port))
  (disclose-port port))

(define-method &disclose ((port :output-port))
  (disclose-port port))

;----------------
; Set up exception handlers for the three unnecessary I/O primitives,
; READ-CHAR, PEEK-CHAR, and WRITE-CHAR.  These do the right thing in
; the case of unbuffered ports or buffer overflow or underflow.
;
; This is abstracted to avoid a circular module dependency.

(define (initialize-i/o-handlers! define-exception-handler signal-exception)
  (define-exception-handler (enum op read-char)
    (one-arg-proc->handler (lambda (port)
			     ((port-handler-char (port-handler port))
			       port
			       #t))
			   signal-exception))
    
  (define-exception-handler (enum op peek-char)
    (one-arg-proc->handler (lambda (port)
			     ((port-handler-char (port-handler port))
			       port
			       #f))
			   signal-exception))
  
  (define-exception-handler (enum op write-char)
    (two-arg-proc->handler (lambda (char port)
			     ((port-handler-char (port-handler port))
			       port
			       char))
			   signal-exception)))

; Check the exception and then lock the port.

(define (one-arg-proc->handler proc signal-exception)
  (lambda (opcode reason port)
    (if (= reason (enum exception buffer-full/empty))
	(proc port)
	(signal-exception opcode reason port))))

; This could combined with on-arg-... if the port were the first argument.

(define (two-arg-proc->handler proc signal-exception)
  (lambda (opcode reason arg port)
    (if (= reason (enum exception buffer-full/empty))
	(proc arg port)
	(signal-exception opcode reason arg port))))

;----------------
; Wrappers for the various port operations.  These check types and arguments
; and then call the appropriate handler procedure.

; See if there is a character available.  CHAR-READY? itself is defined
; in current-ports.scm as it needs CURRENT-INPUT-PORT when called with
; no arguments.

(define (real-char-ready? port)
  (if (open-input-port? port)
      ((port-handler-ready? (port-handler port))
         port)
      (call-error "invalid argument" char-ready? port)))

; Reading in a block of characters at once.

(define (read-block buffer start count port . maybe-wait?)
  (if (and (open-input-port? port)
	   (okay-limits? buffer
			 start
			 count))
      (if (= count 0)
	  0
	 ((port-handler-block (port-handler port))
	    port
	    buffer
	    start
	    count
	    (or (null? maybe-wait?)
		(car maybe-wait?))))
      (call-error "invalid argument" read-block buffer start count port)))

; Write the COUNT bytes beginning at START from BUFFER to PORT.

(define (write-block buffer start count port)
  (if (and (open-output-port? port)
	   (okay-limits? buffer start count))
      (if (< 0 count)
	  ((port-handler-block (port-handler port))
	     port
	     buffer
	     start
	     count))
      (call-error "invalid argument" write-block buffer start count port)))

; WRITE-STRING is a front for WRITE-BLOCK.

(define (write-string string port)
  (write-block string 0 (string-length string) port))

; CHAR-READY? for output ports.

(define (output-port-ready? port)
  (if (open-output-port? port)
      ((port-handler-ready? (port-handler port))
         port)
      (call-error "invalid argument" output-port-ready? port)))

; Forcing output.

(define (force-output port)
  (if (open-output-port? port)
      ((port-handler-force (port-handler port))
         port
	 #t)			; raise error if PORT is not open
      (call-error "invalid argument" force-output port)))

(define (force-output-if-open port)
  (if (open-output-port? port)
      ((port-handler-force (port-handler port))
         port
	 #f)))			; do not raise error if PORT is not open

; Closing input and output ports.
; RnRS says that CLOSE-{IN|OUT}PUT-PORT is idempotent.

(define (close-input-port port)
  (if (input-port? port)
      (begin
	(if (open-input-port? port)
	    ((port-handler-close (port-handler port))
	       port))
	(unspecific))
      (call-error "invalid argument" close-input-port port)))

(define (close-output-port port)
  (if (output-port? port)
      (begin
	(if (open-output-port? port)
	    ((port-handler-close (port-handler port))
	       port))
	(unspecific))
      (call-error "invalid argument" close-output-port port)))

;----------------
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
		 ((byte-vector? buffer)
		  (byte-vector-length buffer))
		 (else
		  -1)))))

;----------------
; Is PORT open?

(define (open-port? port)
  (not (= 0 (bitwise-and open-port-mask (provisional-port-status port)))))

(define open-port-mask
  (bitwise-ior (arithmetic-shift 1 (enum port-status-options open-for-input))
	       (arithmetic-shift 1 (enum port-status-options open-for-output))))

;----------------
; Input ports

(define input-port-mask
  (arithmetic-shift 1
		    (enum port-status-options input)))

(define open-input-port-mask
  (arithmetic-shift 1
		    (enum port-status-options open-for-input)))

(define open-input-port-status
  (bitwise-ior input-port-mask
	       open-input-port-mask))

(define (open-input-port? port)
  (not (= 0 (bitwise-and open-input-port-mask
			 (provisional-port-status port)))))

(define (make-input-port-closed! port)
  (provisional-set-port-status! port
				(bitwise-and (provisional-port-status port)
					     (bitwise-not open-input-port-mask))))

(define (make-unbuffered-input-port handler data)
  (if (port-handler? handler)
      (make-port handler
		 (bitwise-ior input-port-mask open-input-port-mask)
		 #f		; timestamp (not used for unbuffered ports)
		 #f             ; locked?    (not currently used)
		 data
		 #f		; buffer
		 #f		; index
		 #f		; limit
		 #f)            ; pending-eof?
      (call-error "invalid argument"
		  make-unbuffered-input-port handler data)))

;----------------
; Output ports

(define output-port-mask
  (arithmetic-shift 1
		    (enum port-status-options output)))

(define open-output-port-mask
  (arithmetic-shift 1
		    (enum port-status-options open-for-output)))

(define open-output-port-status
  (bitwise-ior output-port-mask
	       open-output-port-mask))

(define (open-output-port? port)
  (not (= 0 (bitwise-and open-output-port-mask
			 (provisional-port-status port)))))

(define (make-output-port-closed! port)
  (provisional-set-port-status! port
				(bitwise-and (provisional-port-status port)
					     (bitwise-not open-output-port-mask))))

(define (make-unbuffered-output-port handler data)
  (if (port-handler? handler)
      (make-port handler
		 open-output-port-status
		 #f		; lock     (not used in unbuffered ports)
		 #f             ; locked?  (not currently used)
		 data
		 #f		; buffer
		 #f		; index
		 #f		; limit
		 #f)            ; pending-eof?
      (call-error "invalid argument"
		  make-unbuffered-output-port handler data)))

;----------------
; Output ports that just discard any output.

(define null-output-port-handler
  (make-port-handler
    (lambda (ignore)			; disclose
      (list 'null-output-port))
    make-output-port-closed!		; close
    (lambda (port char)			; one-char (we just empty the buffer)
      (set-port-index! port 0))
    (lambda (port buffer start count)	; write-block
      count)
    (lambda (port)			; ready?
      #t)
    (lambda (port error-if-closed?)	; force-output
      (unspecific))))

; They can all share a buffer.  The buffer is needed because the WRITE-CHAR
; byte code actually wants to put characters somewhere.

(define null-output-buffer
  (make-byte-vector 1024 0))

(define (make-null-output-port)
  (make-port null-output-port-handler
	     open-output-port-mask
	     #f		; timestamp
	     #f         ; locked?   (not currently used)
	     #f		; data
	     null-output-buffer
	     0		; index
	     (byte-vector-length null-output-buffer)	; limit
	     #f))	; pending-eof?
