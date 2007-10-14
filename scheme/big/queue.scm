; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Queues
; Richard's code with Jonathan's names.
;
;     Richard's names:     Jonathan's names (modified by popular demand):
;      make-empty-queue     make-queue
;      add-to-queue!	    enqueue!
;      remove-from-queue!   dequeue!
;
; Now using optimistic concurrency.  We really need two sets of procedures to
; allow those who don't care to avoid the cost of the concurrency checks.

(define-synchronized-record-type queue :queue
  (really-make-queue uid head tail)
  (head tail)		; synchronize on these
  queue?
  (uid queue-uid)
  (head real-queue-head set-queue-head!)
  (tail queue-tail set-queue-tail!))

(define queue-uid (list 0))

(define (next-uid)
  (atomically
    (let ((uid (provisional-car queue-uid)))
      (provisional-set-car! queue-uid (+ uid 1))
      uid)))

(define (make-queue)
  (really-make-queue (next-uid) '() '()))

; The procedures for manipulating queues.

(define (queue-empty? q)
  (null? (real-queue-head q)))

(define (enqueue! q v)
  (ensure-atomicity!
    (let ((p (cons v '())))
      (cond ((null? (real-queue-head q))
	     (set-queue-head! q p))
	    ((null? (queue-tail q))		; someone got in first
	     (invalidate-current-proposal!))
	    (else
	     (set-cdr! (queue-tail q) p)))
      (set-queue-tail! q p))))

(define (queue-head q)
  (ensure-atomicity
    (if (queue-empty? q)
	(assertion-violation 'queue-head "queue is empty" q)
	(car (real-queue-head q)))))

(define (dequeue! q)
  (ensure-atomicity
    (let ((pair (real-queue-head q)))
      (cond ((null? pair)	;(queue-empty? q)
	     (assertion-violation 'dequeue! "empty queue" q))
	    (else
	     (let ((value (car pair))
		   (next  (cdr pair)))
	       (set-queue-head! q next)
	       (if (null? next)
		   (set-queue-tail! q '()))   ; don't retain pointers
	       value))))))

; Same again, except that we return #F if the queue is empty.
; This is a simple way of avoiding a race condition if the queue is known
; not to contain #F.

(define (maybe-dequeue! q)
  (ensure-atomicity
    (let ((pair (real-queue-head q)))
      (cond ((null? pair)	;(queue-empty? q)
	     #f)
	    (else
	     (let ((value (car pair))
		   (next  (cdr pair)))
	       (set-queue-head! q next)
	       (if (null? next)
		   (set-queue-tail! q '()))   ; don't retain pointers
	       value))))))

(define (empty-queue! q)
  (ensure-atomicity
    (set-queue-head! q '())
    (set-queue-tail! q '())))

(define (on-queue? q v)
  (ensure-atomicity
   (memq v (real-queue-head q))))

; This removes the first occurrence of V from Q.

(define (delete-from-queue! q v)
  (delete-from-queue-if! q
			 (lambda (x)
			   (eq? x v))))

(define (delete-from-queue-if! q pred)
  (ensure-atomicity
   (let ((head (real-queue-head q)))
     (cond ((null? head)
	    #f)
	   ((pred (car head))
	    (set-queue-head! q (cdr head))
	    ;; force proposal check
	    (set-queue-tail! q (if (null? (cdr head))
				   '()
				   (let ((p (queue-tail q))) 
				     (cons (car p) (cdr p)))))
	    #t)
	   ((null? (cdr head))
	    #f)
	   (else
	    (let loop ((list head))
	      (let ((tail (cdr list)))
		(cond ((null? tail)
		       #f)
		      ((pred (car tail))
		       (set-cdr! list (cdr tail))
		       ;; force proposal check
		       (set-queue-head! q (cons (car head) (cdr head)))
		       (set-queue-tail! q (if (null? (cdr tail))
					      list
					      (let ((p (queue-tail q)))
						(cons (car p) (cdr p)))))
		       #t)
		      (else
		       (loop tail))))))))))

(define (queue->list q)
  (ensure-atomicity
    (map (lambda (x) x)
	 (real-queue-head q))))

(define (list->queue list)
  (if (null? list)
      (make-queue)
      (let ((head (cons (car list) '())))
	(let loop ((rest (cdr list)) (tail head))
	  (if (null? rest)
	      (really-make-queue (next-uid) head tail)
	      (begin
		(let ((next (cons (car rest) '())))
		  (set-cdr! tail next)
		  (loop (cdr rest) next))))))))

(define (queue-length q)
  (ensure-atomicity
    (length (real-queue-head q))))

