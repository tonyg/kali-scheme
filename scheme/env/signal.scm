; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; Converting from the simple conditions raised from the lower levels.

;; We offer the same interface as SIMPLE-SIGNALS, except we convert to
;; SRFI-35 conditions immediately.  This is for backwards
;; compatibility only; the use of this stuff is deprecated.

(define (simple-condition->condition simple-condition)
  (let ((type (sc:condition-type simple-condition))
	(stuff (sc:condition-stuff simple-condition)))
    (let ((shim
	   (condition (&simple-condition
		       (type type)
		       (stuff stuff))))
	  (converted
	   ;; this is the stuff in simple-condition.scm
	   (cond
	    ((sc:call-error? simple-condition)
	     (condition (&message (message (car stuff)))
			(&call-error
			 (proc (cadr stuff))
			 (args (cddr stuff)))))
	    ((sc:read-error? simple-condition)
	     (let* ((rev-stuff (reverse stuff)) ; brain damage
		    (port (car rev-stuff))
		    (irritants (reverse (cdr rev-stuff))))
	       (condition (&message (message (car stuff)))
			  (&irritants (values (cdr irritants)))
			  (&i/o-port-error (port port))
			  (&i/o-read-error))))
	    ((sc:vm-exception? simple-condition)
	     (let ((opcode (sc:vm-exception-opcode simple-condition))
		   (reason (sc:vm-exception-reason simple-condition))
		   (arguments (sc:vm-exception-arguments simple-condition)))
	       (condition (&vm-exception
			   (opcode opcode)
			   (reason reason)
			   (arguments arguments))
			  (&message
			   ;; kludge
			   (message (cadr (disclose-vm-condition opcode reason arguments)))))))
	    ((sc:i/o-error? simple-condition)
	     (condition (&message (message (sc:i/o-error-message simple-condition)))
			(&primitive-i/o-error
			 (status (sc:i/o-error-status simple-condition))
			 (operation (sc:i/o-error-operation simple-condition))
			 (arguments (sc:i/o-error-arguments simple-condition)))))
	    ((sc:error? simple-condition)
	     (condition (&message (message (cadr simple-condition)))
			(&error)	; probably not always true
			(&irritants
			 (values (cddr simple-condition)))))
	    ((sc:syntax-error? simple-condition)
	     (condition (&message (message (cadr simple-condition)))
			(&syntax-error)
			(&irritants
			 (values (cddr simple-condition)))))
	    ((sc:warning? simple-condition)
	     (condition (&message (message (cadr simple-condition)))
			(&warning)
			(&irritants
			 (values (cddr simple-condition)))))
	    ((sc:note? simple-condition)
	     (condition (&message (message (cadr simple-condition)))
			(&note)
			(&irritants
			 (values (cddr simple-condition)))))
	    ((sc:interrupt? simple-condition)
	     (condition (&interrupt (type (cadr simple-condition)))))
	    (else #f))))
      
      (if converted
	  (make-compound-condition converted shim)
	  shim))))

(define (coerce-to-condition thing)
  (if (condition? thing)
      thing
      (simple-condition->condition thing)))

(define (condition->simple-condition condition)
  (if (simple-condition? condition)
      (cons (simple-condition-type condition)
	    (simple-condition-stuff condition))
      (let ((message (if (message-condition? condition)
			 (condition-message condition)
			 "unknown")))
	(cond
	 ((error? condition)
	  (list 'error message))
	 ((warning? condition)
	  (list 'warning message))
	 ((note? condition)
	  (list 'note message))
	 (else
	  (list 'unknown message))))))

(define (coerce-to-simple-condition condition)
  (if (condition? condition)
      (condition->simple-condition condition)
      condition))

(sc:define-condition-decoder condition?
  (lambda (condition)
    (let ((simple-condition (condition->simple-condition condition)))
      (values (car simple-condition)
	      (cdr simple-condition)))))

(define (signal-condition condition)
  (really-signal-condition (coerce-to-condition condition)))

(define (make-condition type stuff)
  (simple-condition->condition (cons type stuff)))

; Legacy code, copied verbatim from simple-signal.scm

(define (signal type . stuff)
  (signal-condition (make-condition type stuff)))

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

