; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Placeholders (single-assignment cells for use with threads)

(define-synchronized-record-type placeholder :placeholder
  (really-make-placeholder value queue id)
  (value queue)				; synchronize on this
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
  (really-make-placeholder (unspecific)
			   (make-queue)
			   (if (null? id-option) #f (car id-option))))

(define (placeholder-value placeholder)
  (with-new-proposal (lose)
    (let ((queue (placeholder-queue placeholder)))
      (if queue
	  (or (maybe-commit-and-block-on-queue queue)
	      (lose)))))
  (placeholder-real-value placeholder))

(define (placeholder-set! placeholder new-value)
  (with-new-proposal (lose)
    (let ((queue (placeholder-queue placeholder)))
      (cond (queue
	     (set-placeholder-value! placeholder new-value)
	     (set-placeholder-queue! placeholder #f)
	     (or (maybe-commit-and-make-ready queue)
		 (lose)))
	    (else
	     ;; We only read queue and value and they are set atomically,
	     ;; so there is no need to commit here.
	     (assertion-violation 'placeholder-set!
				  "placeholder is already assigned"
				  placeholder
				  (placeholder-real-value placeholder)))))))
