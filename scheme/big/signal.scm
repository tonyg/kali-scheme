; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; For backwards compatibility with old versions of Scheme 48
; only---don't use in new code.

;;;; Signalling conditions

; I don't like the term "signal," but that's the one Gnu Emacs Lisp,
; Common Lisp, and Dylan use, so it's probably best to stick with it.

(define (make-condition type stuff)
  (let ((base
	 (case type
	   ((error) (make-error))
	   ((warning) (make-warning))
	   ((note) (make-note))
	   ((syntax-error) (make-syntax-violation))
	   ((call-error) (make-assertion-violation))
	   (else (make-assertion-violation)))))
    (call-with-values
	(lambda ()
	  (cond
	   ((null? stuff) (values #f '()))
	   ((string? (car stuff)) (values (car stuff) (cdr stuff)))
	   (else (values #f stuff))))
      (lambda (message irritants)
	(let* ((con
		(if message
		    (condition base
			       (make-message-condition message))
		    base))
	       (con
		(condition con (make-irritants-condition irritants))))
	  con)))))

(define (signal type . stuff)
  (signal-condition
   (make-condition type stuff)))

; Error

(define (error message . irritants)
  (apply signal 'error message irritants))

; Warn

(define (warn message . irritants)
  (signal-condition (make-condition 'warning (cons message irritants))))

; Note

(define (note message . irritants)
  (signal-condition (make-condition 'note (cons message irritants))))

; Syntax errors

(define (syntax-error message . rest)  ; Must return a valid expression.
  (signal-condition (make-condition 'syntax-error (cons message rest)))
  ''syntax-error)


; "Call error" - this means that the condition's "stuff" (cdr) is of
; the form (message procedure . args), and should be displayed appropriately.
; Proceeding from such an error should return the value that the call
; to the procedure on the args should have returned.

(define (call-error message proc . args)
  (signal-condition (make-condition 'call-error
				    (cons message (cons proc args)))))
