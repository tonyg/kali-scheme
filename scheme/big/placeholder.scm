; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Placeholders (single-assignment cells for use with threads)

(define-synchronized-record-type placeholder :placeholder
  (really-make-placeholder value queue id)
  (value)				; synchronize on this
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

(define null-value (list 'null-placeholder-value))

(define (make-placeholder . id-option)
  (really-make-placeholder null-value
			   (make-queue)
			   (if (null? id-option) #f (car id-option))))

(define (placeholder-value placeholder)
  (with-new-proposal (dummy)
    (if (eq? (placeholder-real-value placeholder)
	     null-value)
	;; Kludge alert: The commit may fail, in which case
	;; (placeholder-queue placeholder) is #f.  Albeit,
	;; MAYBE-COMMIT-AND-BLOCK-ON-QUEUE *still* needs to have a
	;; queue as an argument.
	(maybe-commit-and-block-on-queue (or (placeholder-queue placeholder)
					     (make-queue)))))
  (placeholder-real-value placeholder))

(define (placeholder-set! placeholder new-value)
  (with-new-proposal (lose)
    (let ((value (placeholder-real-value placeholder)))
      (cond ((eq? value null-value)
	     (set-placeholder-value! placeholder new-value) 
	     (if (maybe-commit-and-make-ready (placeholder-queue placeholder))
		 (set-placeholder-queue! placeholder #f)	; assist GC
		 (lose)))
	    ((not (eq? new-value value))
	     (error "placeholder is already assigned"
		    placeholder
		    value))))))
             ; only did one read, so no need to commit
