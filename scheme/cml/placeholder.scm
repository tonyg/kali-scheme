; Placeholders (single-assignment cells for use with threads)
; these are equivalent to ID-90 I-structures

(define-synchronized-record-type placeholder :placeholder
  (really-make-placeholder priority queue value id)
  (priority value)
  placeholder?
  (priority placeholder-priority set-placeholder-priority!)
  (queue placeholder-queue set-placeholder-queue!)
  (value placeholder-value-internal set-placeholder-value!)
  (id placeholder-id))

(define-record-discloser :placeholder
  (lambda (placeholder)
    (cons 'placeholder
	  (if (placeholder-id placeholder)
	      (list (placeholder-id placeholder))
	      '()))))

(define-record-type q-item :q-item
  (make-q-item trans-id cleanup-proc wrap-proc)
  q-item?
  (trans-id q-item-trans-id)
  (cleanup-proc q-item-cleanup-proc)
  (wrap-proc q-item-wrap-proc))

(define (clean-and-enqueue! queue value)
  (clean-queue-head! queue)
  (enqueue! queue value))

(define (clean-and-dequeue! queue)
  (let loop ()
    (if (queue-empty? queue)
	#f
	(let ((front (dequeue! queue)))
	  (if (trans-id-cancelled? (q-item-trans-id front))
	      (loop)
	      front)))))

(define (clean-queue-head! queue)
  (let loop ()
    (if (not (queue-empty? queue))
	(let ((front (queue-head queue)))
	  (if (trans-id-cancelled? (q-item-trans-id front))
	      (begin
		(dequeue! queue)
		(loop)))))))

(define (make-placeholder . id-option)
  (really-make-placeholder 0
			   (make-queue)
			   (unspecific)
			   (if (null? id-option)
			       #f
			       (car id-option))))

(define (placeholder-value-rv placeholder)
  (make-base
   (lambda ()
     (cond
      ((placeholder-queue placeholder)
       => (lambda (queue)
	    (make-blocked
	     (lambda (trans-id cleanup-thunk wrap-proc)
	       (clean-and-enqueue! queue
				   (make-q-item trans-id
						cleanup-thunk
						wrap-proc))))))
      (else
       (let ((priority (placeholder-priority placeholder)))
	 (set-placeholder-priority! placeholder (+ 1 priority))
	 (make-enabled
	  priority
	  (lambda (queue)
	    (placeholder-value-internal placeholder)))))))))

(define (placeholder-set! placeholder value)
  (if (not
       (with-new-proposal (lose)
         (cond
	  ((placeholder-queue placeholder)
	   => (lambda (queue)
		(let ((thread-queue (make-queue)))
		  (set-placeholder-value! placeholder value)
		  (set-placeholder-queue! placeholder #f)
		  (let loop ()
		    (cond
		     ((clean-and-dequeue! queue)
		      => (lambda (q-item)
			   ((q-item-cleanup-proc q-item) thread-queue)
			   (let ((trans-id (q-item-trans-id q-item)))
			     (trans-id-set-value! trans-id
						  (cons value
							(q-item-wrap-proc q-item)))
			     (enqueue! thread-queue (trans-id-thread-cell trans-id)))
			   (loop)))))
		  (or (maybe-commit-and-make-ready thread-queue)
		      (lose)))))
	  (else #f))))
       (error "placeholder is already assigned" placeholder value)))

(define (placeholder-value placeholder)
  (sync (placeholder-value-rv placeholder)))


     