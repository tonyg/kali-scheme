; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
  (lambda (ignore) (+ channel-size error-string-size))
  (lambda (spec mode key)
    (let* ((lose (lambda (reason)
		   (raise-exception* reason 0 spec (enter-fixnum mode))))
	   (os-lose (lambda (status)
		      (raise-exception os-error 0
				       spec
				       (enter-fixnum mode)
				       (get-error-string status key))))
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
		      (os-lose status)))))
	    (else
	     (lose (enum exception wrong-type-argument)))))))

(define (open-channel-status? mode)
  (or (= mode (enum channel-status-option input))
      (= mode (enum channel-status-option output))
      (= mode (enum channel-status-option special-input))
      (= mode (enum channel-status-option special-output))))

(define-consing-primitive close-channel (channel->)
  (lambda (ignore) error-string-size)
  (lambda (channel key)
    (if (open? channel)
	(let ((status (close-channel! channel)))
	  (if (error? status)
	      (raise-exception os-error 0 channel (get-error-string status key))
	      (goto no-result)))
	(raise-exception wrong-type-argument 0 channel))))

(define (channel-read-or-write read? proc)
  (lambda (thing start count wait? channel key)
    (let ((lose (lambda (reason)
		  (if read?
		      (raise-exception*
		         reason 0
			 thing
			 (enter-fixnum start)
			 (enter-fixnum count)
			 (enter-boolean wait?)
			 channel)
		      (raise-exception*
		         reason 0
			 thing
			 (enter-fixnum start)
			 (enter-fixnum count)
			 channel))))
	  (os-lose (lambda (status)
		     (if read?
			 (raise-exception os-error 0
					  thing
					  (enter-fixnum start)
					  (enter-fixnum count)
					  (enter-boolean wait?)
					  channel
					  (get-error-string status key))
			 (raise-exception os-error 0
					  thing
					  (enter-fixnum start)
					  (enter-fixnum count)
					  channel
					  (get-error-string status key))))))
      (if (and (if read?
		   (and (input-channel? channel)
			(not (immutable? thing)))
		   (output-channel? channel))
	       (or (vm-string? thing)
		   (code-vector? thing)))
	  (let ((length (if (vm-string? thing)
			    (vm-string-length thing)
			    (code-vector-length thing)))
		(addr (address+ (address-after-header thing) start)))
	    (if (< length (+ start count))
		(lose (enum exception index-out-of-range))
		(proc channel addr count wait? lose os-lose)))
	  (lose (enum exception wrong-type-argument))))))

; FETCH-STRING here avoids a type warning in the C code, but is not really
; the right thing.

(define (do-channel-read channel start count wait? lose os-lose)
  (receive (got eof? pending? status)
      (channel-read-block (extract-channel channel) start count wait?)
    (cond ((error? status)
	   (os-lose status))
	  (eof?
	   (goto return eof-object))
	  (pending?
	   (set-channel-os-status! channel true)  ; note that we're waiting
	   (disable-interrupts!)  ; keep the result pending for a moment
	   (lose (enum exception pending-channel-i/o)))
	  (else
	   (goto return-fixnum got)))))

; WAIT? is ignored when writing.

(define (do-channel-write channel start count wait? lose os-lose)
  (receive (got pending? status)
      (channel-write-block (extract-channel channel) start count)
    (cond ((error? status)
	   (os-lose status))
	  (pending?
	   (set-channel-os-status! channel true)  ; note that we're waiting
	   (disable-interrupts!)  ; keep the result pending for a moment
	   (lose (enum exception pending-channel-i/o)))
	  (else
	   (goto return-fixnum got)))))

(define (extract-channel channel)
  (extract-fixnum (channel-os-index channel)))

(let ((do-it (channel-read-or-write #t do-channel-read)))
  (define-consing-primitive channel-maybe-read
    (any-> fixnum-> fixnum-> boolean-> channel->)
    (lambda (ignore) error-string-size)
    do-it))
  
(let ((do-it (channel-read-or-write #f do-channel-write)))	   
  (define-consing-primitive channel-maybe-write
    (any-> fixnum-> fixnum-> channel->)
    (lambda (ignore) error-string-size)
    (lambda (buffer start count channel key)
      (do-it buffer start count #f channel key))))

(define-primitive channel-abort (channel->)
  (lambda (channel)
    (goto return (vm-channel-abort channel))))

(define-primitive open-channels-list ()
  (lambda ()
    (goto return (open-channels-list))))

; Copying error strings into the heap.

(define max-error-string-length 256)

(define error-string-size (vm-string-size max-error-string-length))

(define (get-error-string status key)
  (let* ((string (error-string status))
	 (len (min (string-length string)
		   max-error-string-length))
	 (new (vm-make-string len key)))
    (do ((i 0 (+ i 1)))
	((= i len))
      (vm-string-set! new i (string-ref string i)))
    new))

;----------------------------------------------------------------
; Port instructions.
;
; These are only for speed.  If no port was supplied by the user they have
; to look up the appropriate port in the current dynamic environments.
; This is a complete hack, also done for speed.  See rts/current-port.scm
; for the other end.

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
	  (let ((i (extract-fixnum (port-index port)))
		(l (extract-fixnum (port-limit port)))
		(b (port-buffer port)))
	    (cond ((or (vm-eq? (port-locked? port) true)
		       (= i l))
		   (raise-exception buffer-full/empty 1 port))
		  (else
		   (if read?
		       (set-port-index! port (enter-fixnum (+ i 1))))
		   (goto continue-with-value
			 (enter-char (ascii->char (code-vector-ref b i)))
			 1))))
	  (raise-exception wrong-type-argument 1 port)))))

(let ((do-it (read-or-peek-char #t)))
  (define-primitive read-char () do-it))

(let ((do-it (read-or-peek-char #f)))
  (define-primitive peek-char () do-it))

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
      (if (and (vm-char? char)
	       (port? port)
	       (port-has-status? port
				 (enum port-status-options open-for-output)))
	  (let ((i (extract-fixnum (port-index port)))
		(b (port-buffer port)))
	    (cond ((or (vm-eq? (port-locked? port) true)
		       (= i (code-vector-length b)))
		   (raise-exception buffer-full/empty 1 char port))
		  (else
		   (set-port-index! port (enter-fixnum (+ i 1)))
		   (code-vector-set! (port-buffer port)
				     i
				     (char->ascii (extract-char char)))
		   (goto continue-with-value
			 unspecific-value
			 1))))
	  (raise-exception wrong-type-argument 1 char port)))))
	  
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
	 (write-char (extract-char thing) out))
	((typed-record? thing)
	 (write-string "#{" out)
	 (write-string (extract-string (record-type-name thing)) out)
	 (write-char #\} out))
	(else
	 (write-string (cond ((vm-string? thing)
			      (extract-string thing))
			     ((vm-symbol? thing)
			      (extract-string (vm-symbol->string thing)))
			     ((vm-boolean? thing)
			      (if (extract-boolean thing) "#t" "#f"))
			     ((vm-eq? thing null)
			      "()")
			     ((vm-pair? thing)
			      "(...)")
			     ((vm-vector? thing)
			      "#(...)")
			     ((closure? thing)
			      "#{procedure}")
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

(define-primitive copy-bytes! (any-> fixnum-> any-> fixnum-> fixnum->)
  (lambda (from from-index to to-index count)
    (cond ((and (or (vm-string? from)
		    (code-vector? from))
		(or (vm-string? from)
		    (code-vector? from))
		(<= 0 from-index)
		(<= 0 to-index)
		(<= 0 count)
		(<= (+ from-index count)
		    (if (vm-string? from)
			(vm-string-length from)
			(code-vector-length from)))
		(<= (+ to-index count)
		    (if (vm-string? to)
			(vm-string-length to)
			(code-vector-length to))))
	   (copy-memory! (address+ (address-after-header from) from-index)
			 (address+ (address-after-header to) to-index)
			 count)
	   (goto no-result))
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

(define-consing-primitive write-image (string-> any-> string->)
  (lambda (ignore) error-string-size)
  (lambda (filename resume-proc comment-string key)
    (let* ((lose (lambda (reason status)
		   (raise-exception* reason 0
				     filename resume-proc comment-string
				     (get-error-string status key))))
	   (port-lose (lambda (reason status port)
			(if (error? (close-output-port port))
			    (error-message "Unable to close image file"))
			(lose reason status))))
      (if (not (s48-image-writing-okay?))
	  (raise-exception unimplemented-instruction 0
			   filename resume-proc comment-string)
	  (receive (port status)
	      (open-output-file (extract-string filename))
	    (if (error? status)
		(lose (enum exception cannot-open-channel) status)
		(let ((status (write-string (extract-string comment-string) port)))
		  (if (error? status)
		      (port-lose (enum exception os-error) status port)
		      (let ((status (s48-write-image resume-proc port)))
			(if (error? status)
			    (port-lose (enum exception os-error) status port)
			    (let ((status (close-output-port port)))
			      (if (error? status)
				  (lose (enum exception os-error) status)
				  (receive (undumpables undumpable-count)
				      (s48-undumpable-records)
				    (if (= undumpable-count 0)
					(goto no-result)
		     ; ---------------->
		     (raise-exception unresumable-records-in-image 0
				      filename
				      resume-proc
				      comment-string
				      undumpables
				      (enter-fixnum undumpable-count))))))))))))))))

; READ-IMAGE needs to protect some values against GCs (this can't be with
; READ-IMAGE as that is compiled separately.)

(add-gc-root! s48-initializing-gc-root)

