; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


;;;; Raising and handling conditions

; An exception is an unusual situation detected by the virtual machine.
; A condition is a run-time system structure describing unusual situations,
; including exceptions.

; Usual exception handler vector.

(define (define-exception-handler opcode proc)
  (vector-set! exception-handlers opcode proc))

(define (signal-exception opcode reason . args)
  (really-signal-condition (make-exception opcode
					   (if reason
					       (enumerand->name reason exception)
					       #f)
					   args)))

(define exception-handlers
  (make-vector op-count signal-exception))

; TRAP is the same as SIGNAL-CONDITION.

(define-exception-handler (enum op trap)
  (lambda (opcode reason arg)
    (really-signal-condition arg)))

; The time opcode sometimes needs a little help.

(define-exception-handler (enum op time)
  (lambda (opcode reason option arg0 . maybe-arg1)
    (if (= reason (enum exception arithmetic-overflow))
	(+ (* arg0 1000)		; seconds
	   (car maybe-arg1))		; milliseconds
	(apply signal-exception opcode reason option arg0 maybe-arg1))))

; This is for generic arithmetic, mostly

(define (extend-opcode! opcode make-handler)
  (let* ((except (lambda args
		   (apply signal-exception
			  opcode
			  #f   ; lost our reason
			  args)))
	 (handler (make-handler except)))
    (define-exception-handler opcode
      (lambda (opcode reason . args)
	(apply handler args)))))

; Raising and handling conditions.
; (fluid $condition-handlers) is a cell containing a list of handler procedures.
; Each handler takes two arguments: the condition to be handled, and
; a thunk that can be called if the handler decides to decline handling
; the condition.  The continuation to a call to a handler is that
; of the call to signal-condition.

(define (really-signal-condition condition)
  (let loop ((hs (fluid-cell-ref $condition-handlers)))
    ((car hs) condition (lambda () (loop (cdr hs))))))

(define (with-handler h thunk)
  (let-fluid $condition-handlers
      (make-cell (cons h (fluid-cell-ref $condition-handlers)))
    thunk))

(define $condition-handlers
  (make-fluid (make-cell #f)))

(define (initialize-exceptions! thunk)
  (call-with-current-continuation
    (lambda (k)
      (fluid-cell-set! $condition-handlers
		       (list (last-resort-condition-handler k)))
      (set-exception-handlers! exception-handlers)
      (thunk))))

(define (last-resort-condition-handler halt)
  (let ((interrupt/keyboard (enum interrupt keyboard))
	(losing? #f))
    (lambda (condition punt)
      (cond ((error? condition)
	     (primitive-catch
	       (lambda (c)
		 (if (not losing?)
		     (begin (set! losing? #t)
			    (report-utter-lossage condition c)))
		 (halt 123))))
	    ((and (interrupt? condition)
		  (= (cadr condition) interrupt/keyboard))
	     (halt 2))
	    (else
	     (unspecific))))))	;proceed

; This will print a list of template id's, which you can look up in
; initial.debug to get some idea of what was going on.

(define (report-utter-lossage condition c)
  (cond ((exception? condition)
	 (debug-message "VM exception `"
			(exception-reason condition)
			"' with no handler in place")
	 (debug-message "opcode is: "
			(enumerand->name (exception-opcode condition)
					 op)))
	(else
	 (apply debug-message
		(condition-type condition)
		" with no handler in place: "
		(condition-stuff condition))))
  (apply debug-message
	 "stack template id's: "
	 (map (lambda (id+pc)
		(if (number? (car id+pc))
		    (string-append (number->string (car id+pc))
				   " <- ")
		    " <- "))
	      (continuation-preview c))))

(define (continuation-preview c)
  (if (continuation? c)
      (cons (cons (let ((template (continuation-template c)))
		    (if template
			(template-info template)
			'?))
		  (continuation-pc c))
	    (continuation-preview (continuation-cont c)))
      '()))

; ERROR is a compiler primitive, but if it weren't, it could be
; defined as follows:

;(define (error message . irritants)
;  (signal-condition (make-condition 'error (cons message irritants))))

; Run THUNK, returning either the value returned by THUNK or any error
; that occurs.

(define (ignore-errors thunk)
  (call-with-current-continuation
    (lambda (k)
      (with-handler (lambda (c next)
		      (if (error? c)
			  (k c)
			  (next)))
	thunk))))

; Downgrade errors to warnings while executing THUNK.  Returns #T if an
; error occured.

(define (report-errors-as-warnings thunk message . irritants)
  (let ((condition (ignore-errors
		    (lambda ()
		      (thunk)
		      #f))))
    (if condition
	(begin
	  (apply warn message (append irritants (list condition)))
	  #t)
	#f)))

; Define disclosers that are most important for error messages.

(define-method &disclose ((obj :closure))
  (list 'procedure (template-ref (closure-template obj) 1)))

(define-method &disclose ((obj :location))
  (list 'location (location-id obj)))

; (put 'with-handler 'scheme-indent-hook 1)
