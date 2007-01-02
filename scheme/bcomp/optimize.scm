; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Optimizers

(define optimizers-table (make-table))

(define (set-optimizer! name opt)
  (table-set! optimizers-table name opt))

(define (get-optimizer names)
  (lambda (forms package)
    (apply-optimizers (map (lambda (name)
			     (or (table-ref optimizers-table name)
				 (begin
				   (signal 'note
					   "optional optimization pass not invoked"
					   name)
				   (lambda (forms package) forms))))
			   names)
		      forms
		      package)))

(define (apply-optimizers optimizers forms package)
  (fold (lambda (optimizer forms)
	  (optimizer forms package))
	optimizers
	forms))

