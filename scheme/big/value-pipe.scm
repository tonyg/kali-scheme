; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Value pipes (queues where readers and writers can block)
;
; (make-value-pipe [max-size [id]]) -> <pipe>
; (empty-pipe? <pipe>) -> <boolean>
; (pipe-read! <pipe>) -> <value>
; (pipe-write! <pipe> <value>)
; (pipe-push! <pipe> <value>)
; (empty-pipe! <pipe>)
; (pipe-maybe-read! <pipe>) -> <value>
; (pipe-maybe-write! <pipe> <value>)

(define-synchronized-record-type pipe :pipe
  (really-make-pipe queue threads count max-count id)
  (count max-count)			; synchronize on these
  pipe?
  (queue     pipe-queue)			; queue of values
  (threads   pipe-threads)			; queue of waiting threads
  (count     pipe-count set-pipe-count!)	; length of value queue
  (max-count pipe-max-count)			; maximum length of value queue
  (id        pipe-id))				; whatever

(define-record-discloser :pipe
  (lambda (pipe)
    (if (pipe-id pipe)
	(list 'pipe (pipe-id pipe))
	'(pipe))))

(define (make-pipe . more)
  (let ((size (if (null? more)
		  #f			; any negative number will do
		  (let ((size (car more)))
		    (if (or (not size)
			    (and (integer? size)
				 (exact? size)
				 (< 0 size)))
			size
			(call-error "invalid argument" make-pipe more)))))
	(id (if (or (null? more)
		    (null? (cdr more)))
		#f
		(cadr more))))
    (really-make-pipe (make-queue)
		      (make-queue)
		      0
		      size
		      id)))

(define (empty-pipe? pipe)
  (= 0 (pipe-count pipe)))

; General function for adding or removing values.
; OP either enqueues or dequeues a value.
; If the current count is BLOCK-COUNT we call BLOCK.  Otherwise we call OP,
; change value count by DELTA and if it was RELEASE-COUNT we release any
; waiting threads.

(define (pipe-read-or-write! pipe op block-count delta release-count win block)
  (with-new-proposal (lose)
    (let ((count (pipe-count pipe)))
      (if (and block-count
	       (= count block-count))
	  (block pipe lose)
	  (let ((value (op (pipe-queue pipe))))
	    (set-pipe-count! pipe (+ count delta))
	    (if (if (and release-count
			 (= count release-count))
		    (maybe-commit-and-make-ready (pipe-threads pipe))
		    (maybe-commit))
		(win value)
		(lose)))))))
		  
(define (block-on-pipe pipe lose)
  (or (maybe-commit-and-block-on-queue (pipe-threads pipe))
      (lose)))

(define (make-pipe-reader win block)
  (lambda (pipe)
    (pipe-read-or-write! pipe dequeue! 0 -1 (pipe-max-count pipe) win block))) 

(define pipe-read!
  (make-pipe-reader (lambda (x) x) block-on-pipe))

(define pipe-maybe-read!
  (make-pipe-reader (lambda (x) x)
		    (lambda (pipe lose)
		      (if (maybe-commit)
			  #f
			  (lose)))))

(define pipe-maybe-read?!
  (make-pipe-reader (lambda (value)
		      (values #t value))
		    (lambda (pipe lose)
		      (if (maybe-commit)
			  (values #f #f)
			  (lose)))))

(define (make-pipe-writer win block)
  (lambda (pipe value)
    (pipe-read-or-write! pipe
			 (lambda (queue)
			   (enqueue! queue value))
			 (pipe-max-count pipe)		; block-count
			 1				; delta
			 0				; release-count
			 win
			 block)))

(define pipe-write!
  (make-pipe-writer (lambda (value)
		      (values))
		    block-on-pipe))

(define pipe-maybe-write!
  (make-pipe-writer (lambda (value)
		      #t)
		    (lambda (pipe lose)
		      (if (maybe-commit)
			  #f
			  (lose)))))

; Same as PIPE-WRITE except that we push the first value off the end if
; the pipe is full.

(define (pipe-push! pipe value)
  (with-new-proposal (lose)
    (let ((count (pipe-count pipe)))
      (if (and (pipe-max-count pipe)
	       (= count (pipe-max-count pipe)))
	  (dequeue! (pipe-queue pipe))
	  (set-pipe-count! pipe (+ count 1)))
      (enqueue! (pipe-queue pipe) value)
      (if (not (if (= count 0)
		   (maybe-commit-and-make-ready (pipe-threads pipe))
		   (maybe-commit)))
	  (lose)))))

; Remove all values from PIPE.

(define (empty-pipe! pipe)
  (with-new-proposal (lose)
   (let ((count (pipe-count pipe)))
     (if (< 0 count)
	 (empty-queue! (pipe-queue pipe)))
     (set-pipe-count! pipe 0)
     (if (not (if (and (pipe-max-count pipe)
		       (= count (pipe-max-count pipe)))
		  (maybe-commit-and-make-ready (pipe-threads pipe))
		  (maybe-commit)))
	  (lose)))))

