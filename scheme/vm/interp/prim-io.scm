; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

;; Must be a C-level string, as a byte vector
(define (extract-filename filename)
  (extract-low-string filename))

; Check SPEC type and then call OPEN-CHANNEL.

(define-consing-primitive open-channel (any-> any-> fixnum-> any->)
  (lambda (ignore) channel-size)
  (lambda (spec id mode close-silently? key)
    (let* ((lose (lambda (reason)
		   (raise-exception* reason 0 spec (enter-fixnum mode))))
	   (win (lambda (index)
		  (receive (channel reason)
		      (make-registered-channel mode id index close-silently? key)
		    (cond ((false? channel)
			   (if (code-vector? spec)
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
	    ((code-vector? spec)
	     (receive (channel status)
		 (let ((filename (extract-filename spec)))
		   (if (or (= mode (enum channel-status-option input))
			   (= mode (enum channel-status-option special-input)))
		       (open-input-file-channel filename)
		       (open-output-file-channel filename)))
	       (cond ((eq? status (enum errors no-errors))
		      (win channel))
		     (else
		      (raise-exception os-error 0
				       spec
				       (enter-fixnum mode)
				       (enter-fixnum status))))))
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
	(let ((status (close-channel! channel)))
	  (if (error? status)
	      (raise-exception os-error 0 channel status)
	      (goto no-result)))
	(raise-exception wrong-type-argument 0 channel))))

(define-primitive channel-ready? (channel->)
  (lambda (channel)
    (if (open? channel)
	(receive (ready? status)
	    (channel-ready? (extract-channel channel)
			    (input-channel? channel))
	  (if (error? status)
	      (raise-exception os-error 0 channel status)
	      (goto return-boolean ready?)))
	(raise-exception wrong-type-argument 0 channel))))

;----------------
; Reading from and writing to channels.
;
; This is a wrapper around CHANNEL-READ-BLOCK.  We check argument
; types and interpret the return value.  We either return a
; number---the number of bytes read---or a cell containing the OS
; error code in the case of an I/O error.

(define-consing-primitive channel-maybe-read
  (channel-> any-> fixnum-> fixnum-> boolean->)
  (lambda (ignore) cell-size)
  (lambda (channel buffer start count wait? key)
    (if (and (input-channel? channel)
	     (buffer? buffer)
	     (not (immutable? buffer))
	     (<= (+ start count)
		 (buffer-length buffer)))
	(receive (got eof? pending? status)
	    (channel-read-block (extract-channel channel)
				(address+ (address-after-header buffer)
					  start)
				count
				wait?)
	  (goto return
		(cond ((error? status)
		       (make-cell (enter-fixnum status) key))
		      (eof?
		       ;; possible on Windows
		       (if pending?
			   (set-channel-os-status! channel true))
		       eof-object)
		      (pending?
		       (set-channel-os-status! channel true)
		       false)
		      (else
		       (enter-fixnum got)))))
	(raise-exception wrong-type-argument 0
			 channel
			 buffer
			 (enter-fixnum start)
			 (enter-fixnum count)
			 (enter-boolean wait?)))))

; This is a wrapper around CHANNEL-WRITE-BLOCK.  We check argument
; types and interpret the return value.  We either return a
; number---the number of bytes written---or a cell containing the OS
; error code in the case of an I/O error.

(define-consing-primitive channel-maybe-write
  (channel-> any-> fixnum-> fixnum->)
  (lambda (ignore) cell-size)
  (lambda (channel buffer start count key)
    (if (and (output-channel? channel)
	     (buffer? buffer)
	     (<= (+ start count)
		 (buffer-length buffer)))
	(receive (got pending? status)
	    (channel-write-block (extract-channel channel)
				 (address+ (address-after-header buffer)
					   start)
				 count)
	  (goto return
		(cond
		 ((error? status)
		  (make-cell (enter-fixnum status) key))
		 (pending?
		  (set-channel-os-status! channel true)
		  false)
		 (else
		  (enter-fixnum got)))))
	(raise-exception wrong-type-argument 0
			 channel
			 buffer
			 (enter-fixnum start)
			 (enter-fixnum count)))))

;----------------
; Utilities for the above two opcodes.

(define (buffer? thing)
  (code-vector? thing))

(define (buffer-length buffer)
  (code-vector-length buffer))

(define (extract-channel channel)
  (extract-fixnum (channel-os-index channel)))

;----------------

;; random stuff
(define-primitive channel-parameter (fixnum->)
  (lambda (param)
    (enum-case channel-parameter-option param
      ((buffer-size)
       (goto return-fixnum (channel-buffer-size)))
      ((crlf?)
       (goto return-boolean (channel-crlf?)))
      (else
       (raise-exception bad-option 0 (enter-fixnum param))))))

(define-primitive channel-abort (channel->)
  (lambda (channel)
    (goto return (vm-channel-abort channel))))

(define-primitive open-channels-list ()
  (lambda ()
    (goto return (open-channels-list))))

;----------------------------------------------------------------
; Port instructions.
;
; These are only for speed.  If no port was supplied by the user they have
; to look up the appropriate port in the current dynamic environments.
; This is a complete hack, also done for speed.  See rts/current-port.scm
; for the other end.

(define (read-or-peek-byte read?)
  (lambda ()
    (let ((port (if (= (code-byte 0) 0)
		    (val)
		    (get-current-port
		      (enter-fixnum
		        (enum current-port-marker current-input-port))))))
      (if (and (port? port)
	       (port-has-status? port
				 (enum port-status-options open-for-input)))
	  (let ((b (port-buffer port)))
	    (if (false? b)
		(raise-exception buffer-full/empty 1 port)
		(let ((i (extract-fixnum (port-index port)))
		      (l (extract-fixnum (port-limit port))))
		  (cond ((= i l)
			 (raise-exception buffer-full/empty 1 port))
			(else
			 (if read?
			     (set-port-index! port (enter-fixnum (+ i 1))))
			 (goto continue-with-value
			       (enter-fixnum (code-vector-ref b i))
			       1))))))
	  (raise-exception wrong-type-argument 1 port)))))

(let ((do-it (read-or-peek-byte #t)))
  (define-primitive read-byte () do-it))

(let ((do-it (read-or-peek-byte #f)))
  (define-primitive peek-byte () do-it))

(define (read-or-peek-char read?)
  (lambda ()
    (let ((port (if (= (code-byte 0) 0)
		    (val)
		    (get-current-port
		      (enter-fixnum
		        (enum current-port-marker current-input-port))))))
      (if (and (port? port)
	       (port-has-status? port
				 (enum port-status-options open-for-input)))
	  (let ((b (port-buffer port)))
	    (if (false? b)
		(raise-exception buffer-full/empty 1 port)
		(let loop ((i (extract-fixnum (port-index port))))
		  (let ((l (extract-fixnum (port-limit port)))
			(codec (port-text-codec-spec port))
			(lose
			 (lambda ()
			   ;; we may have gotten out of synch because of CR/LF conversion
			   (if read?
			       (set-port-index! port (enter-fixnum i)))
			   (raise-exception buffer-full/empty 1 port))))
		    (cond ((= i l)
			   (lose))
			  ((not (fixnum? codec))
			   (lose))
			  (else
			   (call-with-values
			       (lambda ()
				 (decode-scalar-value (extract-fixnum codec) 
						      (address+ (address-after-header b) i)
						      (- l i)))
			     (lambda (encoding-ok? ok? incomplete? value count)
			       
			       (define (deliver)
				 (if read?
				     (begin
				       (set-port-pending-cr?! port false)
				       (set-port-index! port (enter-fixnum (+ i count)))))
				 (goto continue-with-value
				       (scalar-value->char value)
				       1))
			       
			       (cond
				((not encoding-ok?)
				 (raise-exception wrong-type-argument 1 port))
				((or (not ok?) incomplete?)
				 (lose))
				((not (false? (port-crlf? port)))
				 ;; CR/LF handling.  Great.
				 (cond
				  ((= value cr-code)
				   (if read?
				       (begin
					 (set-port-pending-cr?! port true)
					 (set-port-index! port (enter-fixnum (+ i count)))))
				   (goto continue-with-value (scalar-value->char lf-code) 1))
				  ((and (= value lf-code)
					(not (false? (port-pending-cr? port))))
				   (if read?
				       (set-port-pending-cr?! port false))
				   (loop (+ i count)))
				  (else
				   (deliver))))
				(else
				 (deliver)))))))))))
	  (raise-exception wrong-type-argument 1 port)))))

(let ((do-it (read-or-peek-char #t)))
  (define-primitive read-char () do-it))

(let ((do-it (read-or-peek-char #f)))
  (define-primitive peek-char () do-it))

(define-primitive write-byte ()
  (lambda ()
    (receive (byte port)
	(if (= (code-byte 0) 0)
	    (values (pop)
		    (val))
	    (values (val)
		    (get-current-port (enter-fixnum
				       (enum current-port-marker
					     current-output-port)))))
      (cond
       ((not (and (fixnum? byte)
		  (port? port)
		  (port-has-status? port
				    (enum port-status-options open-for-output))))
	(raise-exception wrong-type-argument 1 byte port))
       ((false? (port-limit port))	; unbuffered
	(raise-exception buffer-full/empty 1 byte port))
       (else
	(let ((b (port-buffer port))
	      (i (extract-fixnum (port-index port))))
	  (cond ((= i (code-vector-length b))
		 (raise-exception buffer-full/empty 1 byte port))
		(else
		 (set-port-index! port (enter-fixnum (+ i 1)))
		 (code-vector-set! b i (extract-fixnum byte))
		 (goto continue-with-value unspecific-value 1)))))))))

(define cr-code 13)
(define lf-code 10)

(define-primitive write-char ()
  (lambda ()
    (receive (char port)
	(if (= (code-byte 0) 0)
	    (values (pop)
		    (val))
	    (values (val)
		    (get-current-port (enter-fixnum
				       (enum current-port-marker
					     current-output-port)))))
      (cond
       ((not (and (vm-char? char)
		  (port? port)
		  (port-has-status? port
				    (enum port-status-options open-for-output))))
	(raise-exception wrong-type-argument 1 char port))
       ((false? (port-limit port))	; unbuffered
	(raise-exception buffer-full/empty 1 char port))
       (else
	(let ((codec (port-text-codec-spec port))
	      (lose
	       ;; #### this isn't really the right exception
	       (lambda () (raise-exception buffer-full/empty 1 char port))))
	  (if (not (fixnum? codec))
	      (lose)
	      (let* ((b (port-buffer port))
		     (i (extract-fixnum (port-index port)))
		     (l (code-vector-length b)))
		(cond
		 ((= i l) (lose))
		 ;; CR/LF handling is atrocious
		 ((and (not (false? (port-crlf? port)))
		       (= (char->scalar-value char) lf-code))
		  (call-with-values
		      (lambda ()
			(encode-scalar-value (extract-fixnum codec) cr-code 
					     (address+ (address-after-header b) i)
					     (- l i)))
		    (lambda (codec-ok? encoding-ok? out-of-space? count)
		      (cond
		       ((not codec-ok?)
			(raise-exception wrong-type-argument 1 char port))
		       ((or (not encoding-ok?) out-of-space?)
			(lose))
		       (else
			(let ((i (+ i count)))
			  (if (= i l)
			      (lose)
			      (call-with-values
				  (lambda ()
				    (encode-scalar-value (extract-fixnum codec) lf-code 
							 (address+ (address-after-header b) i)
							 (- l i)))
				(lambda (codec-ok? encoding-ok? out-of-space? count)
				  (cond
				   ;; the codec is the same as before, so it must be OK
				   ((or (not encoding-ok?) out-of-space?)
				    (lose))
				   (else
				    (set-port-index! port (enter-fixnum (+ i count)))
				    (goto continue-with-value unspecific-value 1))))))))))))
		 (else
		  (call-with-values
		      (lambda ()
			(encode-scalar-value (extract-fixnum codec) (char->scalar-value char) 
					     (address+ (address-after-header b) i)
					     (- l i)))
		    (lambda (codec-ok? encoding-ok? out-of-space? count)
		      (cond
		       ((not codec-ok?)
			(raise-exception wrong-type-argument 1 char port))
		       ((or (not encoding-ok?) out-of-space?)
			(lose))
		       (else
			(set-port-index! port (enter-fixnum (+ i count)))
			(goto continue-with-value unspecific-value 1)))))))))))))))
	  
; Do an ASSQ-like walk up the current dynamic environment, looking for
; MARKER.

(define (get-current-port marker)
  (let ((thread (current-thread)))
    (if (and (record? thread)
	     (< 1 (record-length thread)))
	(let loop ((env (record-ref thread 1)))
	  (cond ((not (and (vm-pair? env)
			   (vm-pair? (vm-car env))))
		 (if (vm-eq? env null)
		     (error (if (eq? (extract-fixnum marker)
				     (enum current-port-marker
					   current-output-port))
				"dynamic environment doesn't have current-output-port"
				"dynamic environment doesn't have current-input-port"))
		     (error "dynamic environment is not a proper list")))
		((vm-eq? marker (vm-car (vm-car env)))
		 (vm-cdr (vm-car env)))
		(else
		 (loop (vm-cdr env)))))
	(error "current thread is not a record"))))

(define-consing-primitive os-error-message (fixnum->)
  (lambda (status)
    (vm-string-size (string-length (error-string status))))
  (lambda (status key)
    (goto return (get-error-string status key))))

; Copying error strings into the heap.

(define (get-error-string status key)
  (let* ((string (error-string status))
	 (len (string-length string))
	 (new (vm-make-string len key)))
    (do ((i 0 (+ i 1)))
	((= i len))
      (vm-string-set! new i (char->ascii (string-ref string i))))
    new))

;----------------
; A poor man's WRITE for use in debugging.

(define-primitive message (any->)
  (lambda (stuff)
    (let ((out (current-error-port)))
      (let loop ((stuff stuff))
	(if (vm-pair? stuff)
	    (begin
	      (message-element (vm-car stuff) out)
	      (loop (vm-cdr stuff)))))
      (newline out)))
  return-unspecific)

(define (message-element thing out)
  (cond ((fixnum? thing)
	 (write-integer (extract-fixnum thing) out))
	((vm-char? thing)
	 (write-string "#\\" out)
	 (write-char (ascii->char (char->scalar-value thing)) out)) ; ####
	((typed-record? thing)
	 (write-string "#{" out)
	 (write-vm-string (record-type-name thing) out)
	 (write-char #\} out))
	((vm-string? thing)
	 (write-vm-string thing out))
	((vm-symbol? thing)
	 (write-vm-string (vm-symbol->string thing) out))
	(else
	 (write-string (cond ((vm-boolean? thing)
			      (if (extract-boolean thing) "#t" "#f"))
			     ((vm-eq? thing null)
			      "()")
			     ((vm-pair? thing)
			      "(...)")
			     ((vm-vector? thing)
			      "#(...)")
			     ((closure? thing)
			      "#{procedure}")
			     ((template? thing)
			      "#{template}")
			     ((location? thing)
			      "#{location}")
			     ((code-vector? thing)
			      "#{code-vector}")
			     ((continuation? thing)
			      "#{continuation}")
			     (else
			      "???"))
		       out))))

(define (typed-record? thing)
  (and (record? thing)
       (< 0 (record-length thing))
       (let ((type (record-ref thing 0)))
	 (and (record? type)
	      (< 2 (record-length type))
	      (vm-symbol? (record-ref type 2))))))

(define (record-type-name record)
  (vm-symbol->string (record-ref (record-ref record 0) 2)))

;----------------------------------------------------------------
; RESUME-PROC is called when the image is resumed.
; This does a garbage collection rooting from RESUME-PROC, writes the heap
; into a file, and then aborts the garbage collection (which didn't modify
; any VM registers or the stack).

; Bug: finalizers for things in the image are ignored.

(define-primitive write-image-low (code-vector-> any-> code-vector-> vector->)
  (lambda (filename resume-proc comment-string undumpables)
    (let* ((lose (lambda (status)
		   (raise-exception* (enum exception os-error) 0
				     (enter-fixnum status)
				     filename
				     resume-proc comment-string)))
	   (port-lose (lambda (status port)
			(let ((close-status (close-output-port port)))
			  (if (error? close-status)
			      (lose close-status)
			      (lose status))))))
      (receive (port status)
	  (open-output-file (extract-filename filename))
	(if (error? status)
	    (lose status)
	    (let ((status (write-string (extract-low-string comment-string) port)))
	      (if (error? status)
		  (port-lose status port)
		  (let ((status (s48-write-image resume-proc
						 undumpables
						 port)))
		    (if (error? status)
			(port-lose status port)
			(let ((status (close-output-port port)))
			  (if (error? status)
			      (lose status)
			      (goto no-result))))))))))))

; READ-IMAGE needs to protect some values against GCs (this can't be with
; READ-IMAGE as that is compiled separately.)

(add-gc-root! s48-initializing-gc-root)

