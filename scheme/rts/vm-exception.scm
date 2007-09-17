; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A VM exception is an unusual situation detected by the virtual machine.

; Usual exception handler vector.

(define (define-vm-exception-handler opcode proc)
  (vector-set! vm-exception-handlers opcode proc))

(define signal-condition (unspecific))

(define (signal-vm-exception opcode reason . args)
  (signal-condition 
   (condition (make-vm-exception opcode
				 (if reason
				     (enumerand->name reason exception)
				     #f))
	      (make-message-condition (vm-exception-reason->message reason))
	      (make-who-condition (enumerand->name opcode op))
	      (make-assertion-violation) ; when in doubt
	      (make-irritants-condition args))))

(define (vm-exception-reason->message reason)
  (enum-case exception reason
    ((unassigned-local) "LETREC variable used before its value has been produced")
    ((undefined-global) "undefined global variable")
    ((unbound-global) "unbound global variable")
    ((bad-procedure) "attempt to call a non-procedure")
    ((wrong-number-of-arguments) "wrong number of arguments")
    ((wrong-type-argument) "argument of wrong type")
    ((arithmetic-overflow) "arithmetic overflow")
    ((index-out-of-range) "index out of range")
    ((heap-overflow) "heap overflow")
    ((out-of-memory) "out of memory")
    ((cannot-open-channel) "cannot open channel")
    ((channel-os-index-already-in-use) "OS index of channel already in use")
    ((closed-channel) "channel closed")
    ((buffer-full/empty) "buffer full or empty")
    ((unimplemented-instruction) "unimplemented instruction")
    ((trap) "VM trap")
    ((proceeding-after-exception) "proceeding after exception")
    ((bad-option) "bad option")
    ((unbound-external-name) "unbound external name")
    ((too-many-arguments-to-external-procedure)
     "too many arguments to externalprocedure")
    ((too-many-arguments-in-callback) "too many arguments in callback")
    ((callback-return-uncovered) "uncovered callback return")
    ((extension-exception) "exception in VM extension")
    ((extension-return-error) "return error in VM extension")
    ((os-error) "OS error")
    ((gc-protection-mismatch) "GC protection mismatch in external code")
    ((no-current-proposal) "no current proposal")
    ((native-code-not-supported) "native code not supported")
    ((illegal-exception-return) "illegal return from exception")
    (else "unknown VM exception")))

(define vm-exception-handlers
  (make-vector op-count signal-vm-exception))

(define (initialize-vm-exceptions! the-signal-condition)
  (set! signal-condition the-signal-condition)
  (set-exception-handlers! vm-exception-handlers))

; TRAP is the same as SIGNAL-CONDITION.

(define-vm-exception-handler (enum op trap)
  (lambda (opcode reason arg)
    (signal-condition arg)))

; The time opcode sometimes needs a little help.

(define-vm-exception-handler (enum op time)
  (lambda (opcode reason option arg0 . maybe-arg1)
    (if (= reason (enum exception arithmetic-overflow))
	(+ (* arg0 1000)		; seconds
	   (car maybe-arg1))		; milliseconds
	(apply signal-vm-exception opcode reason option arg0 maybe-arg1))))

; This is for generic arithmetic, mostly

(define (extend-opcode! opcode make-handler)
  (let* ((except (lambda args
		   (apply signal-vm-exception
			  opcode
			  #f   ; lost our reason
			  args)))
	 (handler (make-handler except)))
    (define-vm-exception-handler opcode
      (lambda (opcode reason . args)
	(apply handler args)))))
