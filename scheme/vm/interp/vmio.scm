; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file vmio.scm.

; *vm-channels* is a vector of descriptors for open channels.

(define *number-of-channels* 100)

(define *vm-channels*)

; Both of the above are needed when writing images.

(define (s48-channels)
  *vm-channels*)

(define (s48-channel-count)
  *number-of-channels*)

; The channel statuses as fixnums.

(define closed-status
  (enter-fixnum (enum channel-status-option closed)))
(define input-status
  (enter-fixnum (enum channel-status-option input)))
(define output-status
  (enter-fixnum (enum channel-status-option output)))
(define special-input-status
  (enter-fixnum (enum channel-status-option special-input)))
(define special-output-status
  (enter-fixnum (enum channel-status-option special-output)))

; Predicates for channels.

(define (input-channel? channel)
  (= (channel-status channel) input-status))

(define (output-channel? channel)
  (= (channel-status channel) output-status))

(define (open? channel)
  (not (= (channel-status channel) closed-status)))

; Initialization - create the channel vector and the three standard channels.

(define (initialize-i/o-system+gc)
  (set! *number-of-channels*
	(max *number-of-channels*
	     (+ 1
		(max (input-port->channel (current-input-port))
		     (max (output-port->channel (current-output-port))
			  (output-port->channel (current-error-port)))))))
  (set! *vm-channels* (make-vector *number-of-channels*
				   (input-port->channel (current-input-port))))
  (set! *pending-channels-head* false)
  (set! *pending-channels-tail* false)
  (if (null-pointer? *vm-channels*)
      (error "out of memory, unable to continue"))
  (vector+length-fill! *vm-channels* *number-of-channels* false)
  (let ((key (ensure-space (* 3 (+ channel-size
				   (vm-string-size
				     (string-length "standard output")))))))
    (values (make-initial-channel (input-port->channel (current-input-port))
				  input-status
				  "standard input"
				  key)
	    (make-initial-channel (output-port->channel (current-output-port))
				  output-status
				  "standard output"
				  key)
	    (make-initial-channel (output-port->channel (current-error-port))
				  output-status
				  "standard error"
				  key))))

(define (make-initial-channel channel status name key)
  (let ((vm-channel (make-channel status
				  (enter-string name key)
				  (enter-fixnum channel)
				  false    ; close-silently?
				  false    ; next
				  false    ; os-status
				  key)))
    (vector-set! *vm-channels* channel vm-channel)
    vm-channel))

(define (os-index->channel index)
  (vector-ref *vm-channels* index))

; Make a new channel.  The os-index is used to handle I/O-completion interrupts
; so we have to ensure that there is at most one channel using each index.

(define (make-registered-channel mode id os-index close-silently? key)
  (cond ((not (or (< os-index *number-of-channels*)
		  (add-more-channels os-index)))
	 (values false (enum exception out-of-memory)))
	((false? (vector-ref *vm-channels* os-index))
	 (let ((channel (make-channel (enter-fixnum mode)
				      id
				      (enter-fixnum os-index)
				      close-silently?
				      false   ; next
				      false   ; os-status
				      key)))
	   (vector-set! *vm-channels* os-index channel)
	   (values channel
		   (enum exception out-of-memory)))) ; exception is ignored
	(else
	 (values false (enum exception channel-os-index-already-in-use)))))

; Called from outside the VM.  It's up to the caller to be GC-safe.
; Returns FALSE if anything goes wrong.

(define (s48-really-add-channel mode id os-index)
  (receive (channel status)
      (make-registered-channel (extract-fixnum mode)
			       id
			       os-index
			       false
			       (ensure-space channel-size))
    (if (channel? channel)
	channel
	(enter-fixnum status))))

; Called from outside to change the os-index of a particular channel.

(define (s48-set-channel-os-index channel os-index)
  (cond ((not (or (< os-index *number-of-channels*)
		  (add-more-channels os-index)))
	 (enter-fixnum (enum exception out-of-memory)))
	((false? (vector-ref *vm-channels* os-index))
	 (let ((old-index (extract-fixnum (channel-os-index channel))))
	   (if (vm-eq? (channel-os-status channel)
		       true)
	       (enqueue-channel! old-index (channel-abort old-index)))
	   (vector-set! *vm-channels* old-index false)
	   (vector-set! *vm-channels* os-index channel)
	   (set-channel-os-index! channel (enter-fixnum os-index))
	   true))
	(else
	 (enter-fixnum (enum exception channel-os-index-already-in-use)))))

; Extend the vector of channels.

(define (add-more-channels index)
  (let* ((new-count (max (+ index 1)
			 (+ *number-of-channels* 8)))
	 (old-count *number-of-channels*)
	 (new-vm-channels (make-vector new-count (vector-ref *vm-channels* 0))))
    (cond ((null-pointer? new-vm-channels)
	   #f)
	  (else
	   (do ((i 0 (+ i 1)))
	       ((= i *number-of-channels*))
	     (vector-set! new-vm-channels i (vector-ref *vm-channels* i)))
	   (do ((i *number-of-channels* (+ i 1)))
	       ((= i new-count))
	     (vector-set! new-vm-channels i false))
	   (deallocate *vm-channels*)
	   (set! *vm-channels* new-vm-channels)
	   (set! *number-of-channels* new-count)
	   #t))))

; We abort any operation pending on the channel and then close it, freeing
; up the index.  The status from the OS's close function is returned.

(define (close-channel! channel)
  (let ((os-index (extract-fixnum (channel-os-index channel))))
    (if (vm-eq? (channel-os-status channel)
		true)
	(enqueue-channel! os-index (channel-abort os-index)))
    (let ((status (if (or (= input-status (channel-status channel))
			  (= special-input-status (channel-status channel)))
		      (close-input-channel os-index)
		      (close-output-channel os-index))))
      (vector-set! *vm-channels* os-index false)
      (set-channel-status! channel closed-status)
      status)))

; Called from outside the VM.  Closes the channel at OS-INDEX, should we have
; such.

(define (s48-close-channel os-index)
  (if (and (<= 0 os-index)
	   (< os-index *number-of-channels*)
	   (channel? (os-index->channel os-index)))
      (close-channel! (os-index->channel os-index)))
  (unspecific))

; Called to close an OS channel when we have been unable to make the
; corresponding Scheme channel.

(define (close-channel-index! index name mode)
  (let ((status (if (input-channel-status? mode)
		    (close-input-channel index)
		    (close-output-channel index))))
    (if (error? status)
	(channel-close-error status index name))))

(define (input-channel-status? mode)
  (or (= mode (enum channel-status-option input))
      (= mode (enum channel-status-option special-input))))

(define (channel-close-error status index id)
  (write-error-string "Error: ")
  (write-error-string (error-string status))
  (write-error-newline)
  (write-error-string " while closing port ")
  (if (vm-string? id)
      (write-error-string (extract-string id))
      (write-error-integer (extract-fixnum index)))
  (write-error-newline))

; Return a list of the open channels, for the opcode of the same name.
; Not that it's important, but the list has the channels in order of
; their os-indexes.

(define (open-channels-list)
  (let ((key (ensure-space (* vm-pair-size *number-of-channels*))))
    (do ((i (- *number-of-channels* 1) (- i 1))
	 (res null
	      (let ((channel (vector-ref *vm-channels* i)))
		(if (channel? channel)
		    (vm-cons channel res key)
		    res))))
	((= i -1)
	 res))))

;----------------------------------------------------------------
; Handling i/o-completion interrupts
; Currently, because the GC may move buffers, strings, etc. around, the OS
; must buffer the data while waiting for i/o to complete.
;
; Unix: the i/o completion just means that the channel is ready; no characters
; are ever transfered asynchronously.
;
; DOS/Windows: no non-blocking i/o of any kind.
;
; WindowsNT: we will need a fancier GC or something.

; These are a queue of channels with pending interrupts

(define *pending-channels-head* false)
(define *pending-channels-tail* false)

(define (channel-queue-empty?)
  (false? *pending-channels-head*))

(define (enqueue-channel! index status)
  (let ((channel (os-index->channel index)))
    (set-channel-os-status! channel (enter-fixnum status))
    (cond ((or (not (false? (channel-next channel))) ; already queued (how?)
	       (eq? channel *pending-channels-head*) ; first and only
	       (eq? channel *pending-channels-tail*)); last (i.e. no next)
	   (unspecific))  ; for the type checker
	  ((false? *pending-channels-head*)
	   (set! *pending-channels-head* channel)
	   (set! *pending-channels-tail* channel))
	  (else
	   (set-channel-next! *pending-channels-tail* channel)
	   (set! *pending-channels-tail* channel)))))

(define (dequeue-channel!)
  (let* ((channel *pending-channels-head*)
	 (next (channel-next channel)))
    (set! *pending-channels-head* next)
    (set-channel-next! channel false)
    (if (false? next)
	(set! *pending-channels-tail* false))
    channel))

; See if a the OS has already finished with CHANNEL and return its status
; if it has.  If not, call the OS and have it abort the channel's current
; operation.

(define (vm-channel-abort channel)
  (let ((head *pending-channels-head*))
    (cond ((false? head)
	   (set-channel-os-status! channel false) ; no longer pending
	   (enter-fixnum (channel-abort
			  (extract-fixnum (channel-os-index channel)))))
	  ((vm-eq? channel head)
	   (dequeue-channel!)
	   (if (false? *pending-channels-head*)
	       (pending-interrupts-remove!
		(interrupt-bit
		 (enum interrupt i/o-completion))))
	   (channel-os-status channel))
	  (else
	   (let loop ((ch (channel-next head)) (prev head))
	     (cond ((false? ch)
		    (set-channel-os-status! channel false) ; no longer pending
		    (enter-fixnum (channel-abort
				    (extract-fixnum (channel-os-index channel)))))
		   ((vm-eq? ch channel)
		    (if (vm-eq? ch *pending-channels-tail*)
			(set! *pending-channels-tail* prev))
		    (set-channel-next! prev (channel-next ch))
		    (set-channel-next! ch false)
		    (channel-os-status ch))
		   (else
		    (loop (channel-next ch) ch))))))))

(define (trace-io trace-value)
  (set! *pending-channels-head* (trace-value *pending-channels-head*))
  (set! *pending-channels-tail* (trace-value *pending-channels-tail*)))

;----------------------------------------------------------------
; Automatically closing channels.

; Make sure all open channel names survive the GC.

(define (trace-channel-names s48-trace-value)
  (do ((i 0 (+ i 1)))
      ((= i *number-of-channels*) #f)
    (let ((channel (vector-ref *vm-channels* i)))
      (if (and (not (false? channel))
	       (open? channel))
	  (set-channel-id! channel
			   (s48-trace-value (channel-id channel)))))))

; The following is called after the GC finishes.

(define (close-untraced-channels! s48-extant? s48-trace-value)
  (do ((i 0 (+ i 1)))
      ((= i *number-of-channels*) #f)
    (let ((channel (vector-ref *vm-channels* i)))
      (if (not (false? channel))
	  (let ((new (cond ((s48-extant? channel)
			    (s48-trace-value channel))
			   ((open? channel) ; channel was not copied
			    (close-channel-noisily! channel)
			    false)
			   (else
			    false))))
	    (vector-set! *vm-channels* i new))))))

(define (close-channel-noisily! channel)
  (let ((status (close-channel! channel))
	(id (channel-id channel)))
    (if (error? status)
	(channel-close-error status (channel-os-index channel) id))
    (if (= false (channel-close-silently? channel))
	(notify-channel-closed channel))))

(define (notify-channel-closed channel)
  (let ((id (channel-id channel)))
    (write-error-string "Channel closed: ")
    (if (fixnum? id)
	(write-error-integer (extract-fixnum id))
	(write-error-string (extract-string id)))
    (write-error-string " ")
    (write-error-integer (extract-fixnum (channel-os-index channel)))
    (write-error-newline)))
