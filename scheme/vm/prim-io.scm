; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; I/O primitives

; See doc/io.txt for a description of Scheme 48's I/O system.

; Argument specs

(define open-input-port->
  (input-type (lambda (x)
		(and (port? x)
		     (port-has-status? x (enum port-status-options
					       open-for-input))))
	      no-coercion))

(define open-output-port->
  (input-type (lambda (x)
		(and (port? x)
		     (port-has-status? x (enum port-status-options
					       open-for-output))))
	      no-coercion))

(define channel->     (input-type channel?        no-coercion))

(define (port-has-status? port status)
  (not (= 0 (bitwise-and (extract-fixnum (port-status port))
			 (shift-left 1 status)))))

; Check SPEC type and then call OPEN-CHANNEL.

(define-consing-primitive open-channel (any-> fixnum->)
  (lambda (ignore) channel-size)
  (lambda (spec mode key)
    (let* ((lose (lambda (reason)
		   (raise-exception* reason 0 spec (enter-fixnum mode))))
	   (win (lambda (index)
		  (receive (channel reason)
		      (make-registered-channel mode spec index key)
		    (cond ((false? channel)
			   (if (vm-string? spec)
			       (close-channel-index! index spec mode))
			   (lose reason))
			  (else
			   (goto return channel)))))))
      (cond ((not (open-channel-status? mode))
	     (lose (enum exception wrong-type-argument)))
	    ((fixnum? spec)
	     (if (<= 0 (extract-fixnum spec))
		 (win (extract-fixnum spec))
		 (lose (enum exception wrong-type-argument))))
	    ((vm-string? spec)
	     (receive (channel status)
		 (if (or (= mode (enum channel-status-option input))
			 (= mode (enum channel-status-option special-input)))
		     (open-input-file-channel (extract-string spec))
		     (open-output-file-channel (extract-string spec)))
	       (cond ((eq? status (enum errors no-errors))
		      (win channel))
		     ((eq? status (enum errors file-not-found))
		      (lose (enum exception cannot-open-channel)))
		     (else
		      (lose (enum exception os-error))))))
	    (else
	     (lose (enum exception wrong-type-argument)))))))

(define (open-channel-status? mode)
  (or (= mode (enum channel-status-option input))
      (= mode (enum channel-status-option output))
      (= mode (enum channel-status-option special-input))
      (= mode (enum channel-status-option special-output))))

(define-primitive close-channel (channel->)
  (lambda (channel)
    (if (open? channel)
	(if (error? (close-channel! channel))
	    (raise-exception os-error 0 channel)
	    (goto return-unspecific 0))
	(raise-exception wrong-type-argument 0 channel))))

(define (channel-read-or-write read? proc)
  (lambda (thing start count wait? channel)
    (let* ((lose (lambda (reason)
		   (if read?
		       (raise-exception* reason 0
					 thing
					 (enter-fixnum start)
					 (enter-fixnum count)
					 (enter-boolean wait?)
					 channel)
		       (raise-exception* reason 0
					 thing
					 (enter-fixnum start)
					 (enter-fixnum count)
					 channel)))))
      (if (and (if read?
		   (and (input-channel? channel)
			(not (immutable? thing)))
		   (output-channel? channel))
	       (or (vm-string? thing)
		   (code-vector? thing)))
	  (let ((length (if (vm-string? thing)
			    (vm-string-length thing)
			    (code-vector-length thing)))
		(addr (+ (address-after-header thing) start)))
	    (if (< length (+ start count))
		(lose (enum exception index-out-of-range))
		(proc channel addr count wait? lose)))
	  (lose (enum exception wrong-type-argument))))))

; FETCH-STRING here avoids a type warning in the C code, but is not really
; the right thing.

(define (do-channel-read channel start count wait? lose)
  (receive (got eof? pending? status)
      (channel-read-block (extract-channel channel) start count wait?)
    (cond ((error? status)
	   (lose (enum exception os-error)))
	  (eof?
	   (goto return eof-object))
	  (pending?
	   (disable-interrupts!)  ; keep the result pending for a moment
	   (lose (enum exception pending-channel-i/o)))
	  (else
	   (goto return-fixnum got)))))

; WAIT? is ignored when writing.

(define (do-channel-write channel start count wait? lose)
  (receive (got pending? status)
      (channel-write-block (extract-channel channel) start count)
    (cond ((error? status)
	   (lose (enum exception os-error)))
	  (pending?
	   (disable-interrupts!)  ; keep the result pending for a moment
	   (lose (enum exception pending-channel-i/o)))
	  (else
	   (goto return-fixnum got)))))

(define (extract-channel channel)
  (extract-fixnum (channel-os-index channel)))

(let ((do-it (channel-read-or-write #t do-channel-read)))
  (define-primitive channel-maybe-read
    (any-> fixnum-> fixnum-> boolean-> channel->)
    do-it))
  
(let ((do-it (channel-read-or-write #f do-channel-write)))	   
  (define-primitive channel-maybe-write
    (any-> fixnum-> fixnum-> channel->)
    (lambda (buffer start count channel)
      (do-it buffer start count #f channel))))

(define-primitive channel-abort (channel->)
  (lambda (channel)
    (goto return (vm-channel-abort channel))))

;----------------------------------------------------------------
; Port instructions.

(define (read-or-peek-char read?)
  (lambda (port)
    (let ((i (extract-fixnum (port-index port)))
	  (l (extract-fixnum (port-limit port)))
	  (b (port-buffer port)))
      (cond ((or (vm-eq? (port-locked? port) true)
		 (= i l))
	     (raise-exception buffer-full/empty 0 port))
	    (else
	     (if read?
		 (set-port-index! port (enter-fixnum (+ i 1))))
	     (goto return-char (ascii->char (code-vector-ref b i))))))))

(let ((do-it (read-or-peek-char #t)))
  (define-primitive read-char (open-input-port->) do-it))

(let ((do-it (read-or-peek-char #f)))
  (define-primitive peek-char (open-input-port->) do-it))

(define-primitive write-char (char-> open-output-port->)
  (lambda (char port)
    (let ((i (extract-fixnum (port-index port)))
	  (b (port-buffer port)))
      (cond ((or (vm-eq? (port-locked? port) true)
		 (= i (code-vector-length b)))
	     (raise-exception buffer-full/empty 0 (enter-char char) port))
	    (else
	     (set-port-index! port (enter-fixnum (+ i 1)))
	     (code-vector-set! (port-buffer port) i (char->ascii char))
	     (goto return-unspecific 0))))))

; *** Our entry for the obscure comment of the year contest.
;
; Pclsring is the term in ITS for the mechanism that makes the operating system
; appear to be a virtual machine.  The paradigm is that of the BLT instruction
; on the PDP-10: its arguments are in a set of registers, and if the instruction
; gets interrupted in the middle, the registers reflect the intermediate state;
; the PC is set to the BLT instruction itself, and the process can be resumed
; in the usual way.
; For more on pclsring see `Pclsring: Keeping Process State Modular' by Alan
; Bawden (ftp.ai.mit.edu:pub/alan/pclsr.memo).

(define-primitive copy-bytes! (any-> fixnum-> any-> fixnum-> fixnum->)
  (lambda (from from-index to to-index count)
    (cond ((and (or (vm-string? from)
		    (code-vector? from))
		(or (vm-string? from)
		    (code-vector? from))
		(<= 0 from-index)
		(<= 0 to-index)
		(<= 0 count)
		(addr<= (addr+ from-index count)
			(if (vm-string? from)
			    (vm-string-length from)
			    (code-vector-length from)))
		(addr<= (addr+ to-index count)
			(if (vm-string? to)
			    (vm-string-length to)
			    (code-vector-length to))))
	   (copy-memory! (addr+ (address-after-header from) from-index)
			 (addr+ (address-after-header to) to-index)
			 count)
	   (goto return-unspecific 0))
	  (else
	   (raise-exception wrong-type-argument 0
			    from (enter-fixnum from-index)
			    to (enter-fixnum to-index)
			    (enter-fixnum count))))))

;----------------------------------------------------------------
; RESUME-PROC is called when the image is resumed.
; This does a garbage collection rooting from RESUME-PROC, writes the heap
; into a file, and then aborts the garbage collection (which didn't modify
; any VM registers or the stack).

; Bug: finalizers for things in the image are ignored.

(define-primitive write-image (string-> any-> string->)
  (lambda (filename resume-proc comment-string)
    (let* ((lose (lambda (reason)
		   (raise-exception* reason 0
				     filename resume-proc comment-string)))
	   (port-lose (lambda (reason port)
			(if (error? (close-output-port port))
			    (error-message "Unable to close image file"))
			(lose reason))))
      (if (not (image-writing-okay?))
	  (lose (enum exception unimplemented-instruction))
	  (receive (port status)
	      (open-output-file (extract-string filename))
	    (if (error? status)
		(lose (enum exception cannot-open-channel))
		(if (error? (write-string (extract-string comment-string) port))
		    (port-lose (enum exception os-error) port)
		    (if (error? (write-image resume-proc port mark-traced-channels-closed!))
			(port-lose (enum exception os-error) port)
			(if (error? (close-output-port port))
			    (lose (enum exception os-error))
			    (goto return-unspecific 0))))))))))

