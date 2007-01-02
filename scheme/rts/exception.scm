; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;;;; Raising and handling of exceptions

; A condition is a run-time system structure describing an unusual situation.

; Raising and handling conditions.
; (fluid $exception-handlers) is a cell containing a list of handler procedures.
; Each handler takes two arguments: the condition to be handled, and
; a thunk that can be called if the handler decides to decline handling
; the condition.  The continuation to a call to a handler is that
; of the call to signal-condition.

(define (really-signal-condition condition)
  (let loop ((hs (fluid-cell-ref $exception-handlers))
	     (condition condition))
    ((car hs) condition
     (lambda maybe-condition
       (if (null? maybe-condition)
	   (loop (cdr hs) condition)
	   (loop (cdr hs) (car maybe-condition)))))))

(define (with-handler h thunk)
  (let-fluid $exception-handlers
      (make-cell (cons h (fluid-cell-ref $exception-handlers)))
    thunk))

(define $exception-handlers
  (make-fluid (make-cell #f)))

(define (initialize-exceptions! thunk)
  (call-with-current-continuation
    (lambda (k)
      (fluid-cell-set! $exception-handlers
		       (list (last-resort-exception-handler k)))
      (thunk))))

(define (last-resort-exception-handler halt)
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
  (cond ((vm-exception? condition)
	 (debug-message "VM exception `"
			(vm-exception-reason condition)
			"' with no handler in place")
	 (debug-message "opcode is: "
			(enumerand->name (vm-exception-opcode condition)
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

;; SRFI 34

(define (with-exception-handler handler thunk)
  (with-handler
   (lambda (condition punt)
     (handler condition))
   thunk))

;; no tail recursive call to the handler here
(define (raise obj)
  (let* ((cell (make-cell (fluid-cell-ref $exception-handlers)))
	 (last-handler #f))
    (let-fluid
     $exception-handlers cell
     (lambda ()
       (let loop ((handlers (fluid-cell-ref $exception-handlers))
		  (obj obj))
	 (cell-set! cell (cdr handlers))
	 ((car handlers) obj
	  (lambda maybe-obj
	    (if (null? maybe-obj)
		(loop (cdr handlers) obj)
		(loop (cdr handlers) (car maybe-obj)))))
	 (set! last-handler (car handlers)))))
    (error "exception handler returned" last-handler obj)))

(define-syntax guard
  (syntax-rules ()
    ((guard (var clause ...) e1 e2 ...)
     ((call-with-current-continuation
       (lambda (guard-k)
         (with-exception-handler
          (lambda (condition)
            ((call-with-current-continuation
               (lambda (handler-k)
                 (guard-k
                  (lambda ()
                    (let ((var condition))      ; clauses may SET! var
                      (guard-aux (handler-k (lambda ()
                                              (raise condition)))
                                 clause ...))))))))
          (lambda ()
            (call-with-values
             (lambda () e1 e2 ...)
             (lambda args
               (guard-k (lambda ()
                          (apply values args)))))))))))))

(define-syntax guard-aux
  (syntax-rules (else =>)
    ((guard-aux reraise (else result1 result2 ...))
     (begin result1 result2 ...))
    ((guard-aux reraise (test => result))
     (let ((temp test))
       (if temp 
           (result temp)
           reraise)))
    ((guard-aux reraise (test => result) clause1 clause2 ...)
     (let ((temp test))
       (if temp
           (result temp)
           (guard-aux reraise clause1 clause2 ...))))
    ((guard-aux reraise (test))
     test)
    ((guard-aux reraise (test) clause1 clause2 ...)
     (let ((temp test))
       (if temp
           temp
           (guard-aux reraise clause1 clause2 ...))))
    ((guard-aux reraise (test result1 result2 ...))
     (if test
         (begin result1 result2 ...)
         reraise))
    ((guard-aux reraise (test result1 result2 ...) clause1 clause2 ...)
     (if test
         (begin result1 result2 ...)
         (guard-aux reraise clause1 clause2 ...)))))
