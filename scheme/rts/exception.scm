; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


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

; This is for generic arithmetic, mostly

(define make-opcode-generic!
  (let ((except (lambda (opcode)
		  (lambda (next-method . args)
		    (apply signal-exception opcode #f args)))) ; lost our reason
	(handler (lambda (perform)
		   (lambda (opcode reason . args)
		     ((perform) args)))))
    (lambda (opcode mtable)
      (set-final-method! mtable (except opcode))
      (define-exception-handler opcode
	(handler (method-table-get-perform mtable))))))

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

; CURRENT-ERROR-PORT and WRITE-STRING are passed in to avoid circular
; module dependencies.

(define (initialize-exceptions! current-error-port write-string thunk)
  (call-with-current-continuation
    (lambda (k)
      (set-fluid! $condition-handlers
		  (list (last-resort-condition-handler k
						       current-error-port
						       write-string)))
      (set-exception-handlers! exception-handlers)
      (thunk))))

(define (last-resort-condition-handler halt current-error-port write-string)
  (let ((interrupt/keyboard (enum interrupt keyboard))
	(losing? #f))
    (lambda (condition punt)
      (cond ((error? condition)
	     (primitive-catch
	       (lambda (c)
		 (if (not losing?)
		     (begin (set! losing? #t)
			    (report-utter-lossage condition
						  c
						  current-error-port
						  write-string)))
		 (halt 123))))
	    ((and (interrupt? condition)
		  (= (cadr condition) interrupt/keyboard))
	     (halt 2))
	    (else
	     (unspecific))))))	;proceed

; This will print a list of template id's, which you can look up in
; initial.debug to get some idea of what was going on.

(define (report-utter-lossage condition c current-error-port write-string)
  (let ((out (current-error-port)))
    (if out
	(begin
	  (cond ((exception? condition)
		 (write-string (number->string (exception-opcode condition))
			       out)
		 (write-string " / " out))
		((or (error? condition)
		     (warning? condition))
		 (write-string (car (condition-stuff condition)) out)
		 (write-string " / " out)))
	  (for-each (lambda (id+pc)
		      (if (number? (car id+pc))
			  (write-string (number->string
					 (car id+pc))
					out))
		      (write-string " <- " out))
		    (continuation-preview c))
	  (write-char #\newline out)))))

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
