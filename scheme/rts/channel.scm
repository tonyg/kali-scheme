; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Channel interrupt stuff.

; Install an interrupt handler that queues up the results of completed I/O
; operations and spawn a thread to cope with them.  This is written so as
; to avoid having state in top-level variables, because their values are
; saved in dumped images.

(define (initialize-channel-i/o!)
  (session-data-set! i/o-wait-queues-slot (make-i/o-wait-queues))
  (session-data-set! i/o-wait-count-slot 0)
  (set-interrupt-handler! (enum interrupt i/o-completion)
			  i/o-completion-handler))

(define (i/o-completion-handler channel status enabled-interrupts)
  (let ((queue (i/o-wait-queue channel)))
    (if (thread-queue-empty? queue)
	(spawn-on-root
	 (lambda ()
	   (warn "dropping ignored channel i/o result" channel status)))
	(begin
	  (decrement-i/o-wait-count!)
	  (make-ready (dequeue-thread! queue) status)))))

; Exported procedure

(define (waiting-for-i/o?)
  (< 0 (i/o-wait-count)))

; Block until the current I/O operation on CHANNEL has completed.
; This returns the result of the operation.
;
; This needs to be called with interrupts disabled.

(define (wait-for-channel channel)
  (let ((queue (i/o-wait-queue channel)))
    (if (thread-queue-empty? queue)
	(begin
	  (increment-i/o-wait-count!)
	  (enqueue-thread! queue (current-thread))
	  (block))
	(begin
	  (warn "channel has two pending operations" channel)
	  (terminate-current-thread)))))

; Abort any pending operation on by OWNER on CHANNEL.  We first process
; the request queues as OWNER may be on one.
; Called with interrupts disabled.
  
(define (steal-channel! channel owner)
  (let ((queue (i/o-wait-queue channel)))
    (if (thread-queue-empty? queue)
	#f
	(let ((thread (dequeue-thread! queue)))
	  (cond ((eq? thread owner)
		 (decrement-i/o-wait-count!)
		 (channel-abort channel))
		(else
		 (warn "channel in use by other than port owner" channel)
		 (enqueue-thread! queue thread)
		 #f))))))

; Have CHANNEL-READ and CHANNEL-WRITE wait if a pending-channel-i/o
; exception occurs.

(define-exception-handler (enum op channel-maybe-read)
  (lambda (opcode reason buffer start count wait? channel)
    (if (= reason (enum exception pending-channel-i/o))
	(wait-for-channel channel)
	(begin
	  (enable-interrupts!)
	  (signal-exception opcode reason buffer start count wait? channel)))))

(define-exception-handler (enum op channel-maybe-write)
  (lambda (opcode reason buffer start count channel)
    (if (= reason (enum exception pending-channel-i/o))
	(wait-for-channel channel)
	(begin
	  (enable-interrupts!)
	  (signal-exception opcode reason buffer start count channel)))))

; Two session slots
;   - the number of threads waiting for I/O completion events
;   - a vector of mapping channel-os-indicies to queues for waiting threads

(define i/o-wait-count-slot (make-session-data-slot! 0))

(define (i/o-wait-count)
  (session-data-ref i/o-wait-count-slot))

(define (increment-i/o-wait-count!)
  (session-data-set! i/o-wait-count-slot (+ (i/o-wait-count) 1)))

(define (decrement-i/o-wait-count!)
  (session-data-set! i/o-wait-count-slot (- (i/o-wait-count) 1)))

(define i/o-wait-queues-slot (make-session-data-slot! #f))

(define (i/o-wait-queue channel)
  ((session-data-ref i/o-wait-queues-slot) (channel-os-index channel)))

; Vector of queues for threads waiting for I/O to complete.  There is at
; most one thread on each queue.
  
(define (make-i/o-wait-queues)
  (let ((waiting-for-i/o '#()))
    (lambda (channel-os-index)
      (let ((size (vector-length waiting-for-i/o)))
	(if (>= channel-os-index size)
	    (let ((new (make-vector (max (+ channel-os-index 1)
					 (+ size 8))
				    #f)))
	      (do ((i 0 (+ i 1)))
		  ((= i size))
		(vector-set! new i (vector-ref waiting-for-i/o i)))
	      (do ((i size (+ i 1)))
		  ((= i (vector-length new)))
		(vector-set! new i (make-thread-queue)))
	      (set! waiting-for-i/o new)))
	(vector-ref waiting-for-i/o channel-os-index)))))
  
