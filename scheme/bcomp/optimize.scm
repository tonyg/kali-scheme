; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.

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
				   (lambda (forms) forms))))
			   names)
		      forms
		      package)))

(define (apply-optimizers optimizers forms package)
  (fold (lambda (optimizer forms)
	  (optimizer forms package))
	optimizers
	forms))

;----------------
; The following is all for Kali and isn't currently used.
;
; List of optimization passes that are always used.

(define *standard-optimizer-names* '())

; The default optimizer for the REPL, EVAL, etc.

(define (default-standard-optimizer forms filename p)
  forms)

; The optimizer for the REPL, EVAL etc.

(define *standard-optimizer* default-standard-optimizer)

; Set the optimizers that are used for all compilations.
;
; Don't use the normal expand optimizer for the REPL because it saves type
; information which may become inaccurate (because we don't have the whole
; source yet).
;
; Optimizers take a list of nodes and a package and return a similar list.

(define (set-standard-optimizers! . optimizer-names)
  (set! *standard-optimizer-names* optimizer-names)
  (set! *standard-optimizer* (get-optimizers optimizer-names)))

(define (get-optimizers names)
  (map (lambda (name)
	 (or (table-ref optimizers-table name)
	     (error "standard optimizer not found" name)))
       names))
       
; The standard optimizer has to convert a list of forms into the format the
; optimizers expect and then convert the result back into a list of forms.

(define (make-standard-optimizer optimizer-names)
  (let ((optimizers (get-optimizers optimizer-names)))
    (lambda (forms package)
       (apply-optimizers optimizers forms package))))

