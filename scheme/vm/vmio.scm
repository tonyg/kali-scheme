; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; This is file vmio.scm.

; *vm-channels* is a vector of descriptors for open channels.

(define *number-of-channels* 100)

(define *vm-channels* (unspecific))

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

(define (input-channel? channel)
  (= (channel-status channel) input-status))

(define (output-channel? channel)
  (= (channel-status channel) output-status))

(define (open? channel)
  (not (= (channel-status channel) closed-status)))

(define (initialize-i/o-system)
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
  (values (make-initial-channel (input-port->channel (current-input-port))
				input-status
				"standard input")
	  (make-initial-channel (output-port->channel (current-output-port))
				output-status
				"standard output")
	  (make-initial-channel (output-port->channel (current-error-port))
				output-status
				"standard error")))

(define (make-initial-channel channel status name)
  (let ((vm-channel (make-channel status
				  (enter-string name)
				  (enter-fixnum channel)
				  false    ; next
				  false    ; os-status
				  universal-key)))
    (vector-set! *vm-channels* channel vm-channel)
    vm-channel))

; A channel and a name for each of initial-input-port, initial-output-port,
; and initial-error-port.

(define initial-i/o-heap-space
  (* 3 (+ channel-size
	  (vm-string-size (string-length "standard output")))))

(define (os-index->channel index)
  (vector-ref *vm-channels* index))

; Make a new channel.  The os-index is used to handle I/O-completion interrupts
; so we have to ensure that there is at most one channel using each index.

(define (make-registered-channel mode id os-index key)
  (cond ((not (or (< os-index *number-of-channels*)
		  (add-more-channels os-index)))
	 (values false (enum exception out-of-memory)))
	((false? (vector-ref *vm-channels* os-index))
	 (let ((channel (make-channel (enter-fixnum mode)
				      id
				      (enter-fixnum os-index)
				      false   ; next
				      false   ; os-status
				      key)))
	   (vector-set! *vm-channels* os-index channel)
	   (values channel
		   (enum exception out-of-memory)))) ; exception is ignored
	(else
	 (values false (enum exception channel-os-index-already-in-use)))))

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

(define (close-channel! channel)
  (let* ((os-index (extract-fixnum (channel-os-index channel)))
	 (status (if (or (= input-status (channel-status channel))
			 (= special-input-status (channel-status channel)))
		     (close-input-channel os-index)
		     (close-output-channel os-index))))
    (vector-set! *vm-channels* os-index false)
    (set-channel-status! channel closed-status)
    status))

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
    (cond ((not (false? (channel-next channel))) ; already queued (how?)
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
	   (enter-fixnum (channel-abort
			  (extract-fixnum (channel-os-index channel)))))
	  ((vm-eq? channel head)
	   (dequeue-channel!)
	   (channel-os-status channel))
	  (else
	   (let loop ((ch (channel-next head)) (prev head))
	     (cond ((false? ch)
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

; The following is called after the GC finishes.

(define (close-untraced-channels!)
  (do ((i 0 (+ i 1)))
      ((= i *number-of-channels*) #f)
    (let ((channel (vector-ref *vm-channels* i)))
      (if (not (false? channel))
	  (let* ((header (stob-header channel))
		 (new (cond ((stob? header)  ; channel was copied
			     header)
			    ((open? channel) ; channel was not copied
			     (close-channel-noisily! channel)
			     false)
			    (else
			     false))))
	    (vector-set! *vm-channels* i new))))))

(define (close-channel-noisily! channel)
  (let ((status (close-channel! channel))
	(id (let ((id (channel-id channel)))
	      (cond ((fixnum? id)
		     id)
		    ((stob? (stob-header id))
		     (stob-header id))
		    (else
		     id)))))
    (if (error? status)
	(channel-close-error status (channel-os-index channel) id))
    (write-error-string "Channel closed: ")
    (if (fixnum? id)
	(write-error-integer (extract-fixnum id))
	(write-error-string (extract-string id)))
    (write-error-newline)))

; Mark channels in about-to-be-dumped heaps as closed.

(define (mark-traced-channels-closed!)
  (do ((i 0 (+ i 1)))
      ((= i *number-of-channels*))
    (let ((channel (vector-ref *vm-channels* i)))
      (if (not (false? channel))
	  (let ((header (stob-header channel)))
	    (if (stob? header)    ; channel was copied
		(begin
		  (write-error-string "Channel closed in dumped image: ")
		  (if (fixnum? (channel-id channel))
		      (write-error-integer (extract-fixnum (channel-id channel)))
		      (write-error-string (extract-string (channel-id channel))))
		  (write-error-newline)
		  (set-channel-status! header closed-status)
		  (set-channel-os-index! header (enter-fixnum -1))))))))
  (unspecific))
