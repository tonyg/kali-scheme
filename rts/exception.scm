; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


;;;; Raising and handling conditions

; An exception is an unusual situation detected by the virtual machine.
; A condition is a run-time system structure describing unusual situations,
; including exceptions.

; Usual exception handler vector.

(define (usual-exception-handler opcode . args)
  ((vector-ref exception-handlers opcode) opcode args))

(define (define-exception-handler opcode proc)
  (vector-set! exception-handlers opcode proc))

(define (signal-exception opcode args)
  (really-signal-condition (make-exception opcode args)))

(define exception-handlers
  (make-vector op-count signal-exception))


; TRAP is the same as SIGNAL-CONDITION.

(define-exception-handler (enum op trap)
  (lambda (opcode args)
    (if (pair? (car args)) ;minimal attempt at condition well-formedness
	(really-signal-condition (car args))
	(signal-exception opcode args))))

; This is for generic arithmetic, mostly

(define make-opcode-generic!
  (let ((f (lambda (opcode)
	     (lambda (next-method . args)
	       (signal-exception opcode args))))
	(p (lambda (perform)
	     (lambda (opcode args)
	       ((perform) args)))))
    (lambda (opcode mtable)
      (set-final-method! mtable (f opcode))
      (define-exception-handler opcode
	(p (method-table-get-perform mtable))))))

; Raising and handling conditions.
; (fluid $condition-handlers) is a list of handler procedures.
; Each handler takes two arguments: the condition to be handled, and
; a thunk that can be called if the handler decides to decline handling
; the condition.  The continuation to a call to a handler is that
; of the call to signal-condition.

(define (really-signal-condition condition)
  (let loop ((hs (fluid $condition-handlers)))
    ((car hs) condition (lambda () (loop (cdr hs))))))

(define (with-handler h thunk)
  (let-fluid $condition-handlers
      (cons h (fluid $condition-handlers))
    thunk))

(define $condition-handlers
  (make-fluid #f))


(define (initialize-exceptions! thunk)
  (call-with-current-continuation
    (lambda (k)
      (set-fluid! $condition-handlers
		  (list (last-resort-condition-handler k)))
      (set-exception-handler! usual-exception-handler)
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
	    (else (unspecific))))))	;proceed

; This will print a list of template id's, which you can look up in
; initial.debug to get some idea of what was going on.

(define (report-utter-lossage condition c)
  (let ((out (error-output-port)))
    (if out
	(begin
	  (if (exception? condition)
	      (begin
		(write-string (number->string (exception-opcode condition))
			      out)
		(write-string " / " out)))
	  (for-each (lambda (id+pc)
		      (if (number? (car id+pc))
			  (write-string (number->string
					 (car id+pc))
					out))
		      (write-string " <- " out))
		    (continuation-preview c))
	  (newline out)))))

(define (continuation-preview c)
  (if (continuation? c)
      (cons (cons (template-info (continuation-template c))
		  (continuation-pc c))
	    (continuation-preview (continuation-parent c)))
      '()))


; ERROR is a compiler primitive, but if it weren't, it could be
; defined as follows:

;(define (error message . irritants)
;  (signal-condition (make-condition 'error (cons message irritants))))


(define (ignore-errors thunk)
  (call-with-current-continuation
    (lambda (k)
      (with-handler (lambda (c next)
		      (if (error? c)
			  (k c)
			  (next)))
	thunk))))



; Define disclosers that are most important for error messages.

(define-method &disclose ((obj :closure))
  (list 'procedure (template-ref (closure-template obj) 1)))

(define-method &disclose ((obj :location))
  (list 'location (location-id obj)))

; (put 'with-handler 'scheme-indent-hook 1)
