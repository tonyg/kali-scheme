; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Channel interrupt stuff.

; Install an interrupt handler that queues up the results of completed I/O
; operations and spawn a thread to cope with them.  This is written so as
; to avoid having state in top-level variables, because their values are
; saved in dumped images.

(define (initialize-channel-i/o!)
  (session-data-set! channel-wait-queues-slot '())
  (session-data-set! channel-wait-count-slot 0)
  (set-interrupt-handler! (enum interrupt i/o-completion)
			  i/o-completion-handler))

; The warning message is printed using DEBUG-MESSAGE because to try to make
; sure it appears in spite of whatever problem's the I/O system is having.
;
; Called with interrupts disabled.

(define (i/o-completion-handler channel status enabled-interrupts)
  (let ((queue (fetch-channel-wait-queue! channel)))
    (if queue
	(begin
	  (decrement-channel-wait-count!)
	  (make-ready (dequeue-thread! queue) status))
	(debug-message "Warning: dropping ignored channel i/o result {Channel "
		       (channel-os-index channel)
		       " "
		       (channel-id channel)
		       "}"))))

; Exported procedure

(define (waiting-for-i/o?)
  (< 0 (channel-wait-count)))

; Block until the current I/O operation on CHANNEL has completed.
; This returns the result of the operation.
;
; This needs to be called with interrupts disabled.
;
; We install a DYNAMIC-WIND to abort the operation if the waiting thread is
; terminated.

(define (wait-for-channel channel)
  (let ((queue (fetch-channel-wait-queue! channel)))
    (if queue
	(begin
	  (add-channel-wait-queue! channel queue)
	  (warn "channel has two pending operations" channel)
	  (terminate-current-thread))
	(let ((queue (make-thread-queue)))
	  (increment-channel-wait-count!)
	  (enqueue-thread! queue (current-thread))
	  (add-channel-wait-queue! channel queue)
	  (dynamic-wind nothing
			block
			(lambda ()
			  (disable-interrupts!)
			  (let ((new-queue (fetch-channel-wait-queue! channel)))
			    (cond ((eq? queue new-queue)
				   (channel-abort channel)
				   (wait-for-channel channel))
				  (new-queue
				   (add-channel-wait-queue! channel new-queue)))
			    (enable-interrupts!))))))))

(define (nothing) #f)

; Abort any pending operation on by OWNER on CHANNEL.
; Called with interrupts disabled.
  
(define (steal-channel! channel owner)
  (let ((queue (fetch-channel-wait-queue! channel)))
    (if queue
	(let ((thread (dequeue-thread! queue)))
	  (cond ((eq? thread owner)
		 (decrement-channel-wait-count!)
                 (channel-abort channel))
		(else
		 (warn "channel in use by other than port owner" channel)
		 (enqueue-thread! queue thread)
		 #f)))
	#f)))

; Have CHANNEL-READ and CHANNEL-WRITE wait if a pending-channel-i/o
; exception occurs.

(define-exception-handler (enum op channel-maybe-read)
  (lambda (opcode reason buffer start count wait? channel . maybe-os-message)
    (if (= reason (enum exception pending-channel-i/o))
	(wait-for-channel channel)
	(begin
	  (enable-interrupts!)
 	  (apply signal-exception
 		 opcode reason buffer start count wait? channel
 		 maybe-os-message)))))

(define-exception-handler (enum op channel-maybe-write)
  (lambda (opcode reason buffer start count channel . maybe-os-message)
    (if (= reason (enum exception pending-channel-i/o))
	(wait-for-channel channel)
	(begin
	  (enable-interrupts!)
 	  (apply signal-exception
 		 opcode reason buffer start count channel
 		 maybe-os-message)))))

; Two session slots
;   - the number of threads waiting for I/O completion events
;   - an alist mapping channels to queues for waiting threads

(define channel-wait-count-slot (make-session-data-slot! 0))

(define (channel-wait-count)
  (session-data-ref channel-wait-count-slot))

(define (increment-channel-wait-count!)
  (session-data-set! channel-wait-count-slot (+ (channel-wait-count) 1)))

(define (decrement-channel-wait-count!)
  (session-data-set! channel-wait-count-slot (- (channel-wait-count) 1)))

(define channel-wait-queues-slot (make-session-data-slot! '()))

; Adding a queue and channel - the caller has already determined there is no
; existing queue for this channel.

(define (add-channel-wait-queue! channel queue)
  (session-data-set! channel-wait-queues-slot
		     (cons (cons channel queue)
			   (session-data-ref channel-wait-queues-slot))))

; This is just deleting from an a-list.

(define (fetch-channel-wait-queue! channel)
  (let* ((queues (session-data-ref channel-wait-queues-slot))
	 (queue (cond ((null? queues)
		       #f)
		      ((eq? channel (caar queues))
		       (session-data-set! channel-wait-queues-slot
					  (cdr queues))
		       (cdar queues))
		      (else
		       (let loop ((queues (cdr queues)) (prev queues))
			 (cond ((null? queues)
				#f)
			       ((eq? channel (caar queues))
				(set-cdr! prev (cdr queues))
				(cdar queues))
			       (else
				(loop (cdr queues) queues))))))))
    (if (or (not queue)
	    (thread-queue-empty? queue))
	#f
	queue)))
  

  
