; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Placeholders (single-assignment cells for use with threads)

(define-record-type placeholder :placeholder
  (really-make-placeholder queue id)
  placeholder?
  (queue placeholder-queue set-placeholder-queue!) ; #f means VALUE has been set
  (value placeholder-real-value set-placeholder-value!)
  (id placeholder-id))

(define-record-discloser :placeholder
  (lambda (placeholder)
    (cons 'placeholder
	  (if (placeholder-id placeholder)
	      (list (placeholder-id placeholder))
	      '()))))

(define (make-placeholder . id-option)
  (really-make-placeholder (make-thread-queue)
			   (if (null? id-option) #f (car id-option))))

(define (placeholder-value placeholder)
  (with-interrupts-inhibited
   (lambda ()
     (if  (placeholder-queue placeholder)
	  (begin
	    (enqueue-thread! (placeholder-queue placeholder)
			     (current-thread))
	    (block)))
     (placeholder-real-value placeholder))))

(define (placeholder-set! placeholder value)
  (let ((waiters (with-interrupts-inhibited
		  (lambda ()
		    (let ((queue (placeholder-queue placeholder)))
		      (cond (queue
			     (set-placeholder-value! placeholder value)
			     (set-placeholder-queue! placeholder #f)
			     (do ((waiters '() (cons (dequeue-thread! queue)
						     waiters)))
				 ((thread-queue-empty? queue)
				  waiters)))
			    (else #f)))))))
    (if waiters
	(for-each make-ready waiters)
	(if (not (eq? value (placeholder-value placeholder)))
	    (error "placeholder is already assigned"
		   placeholder
		   value)))))
