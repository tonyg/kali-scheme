; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A VM exception is an unusual situation detected by the virtual machine.

; Usual exception handler vector.

(define (define-vm-exception-handler opcode proc)
  (vector-set! vm-exception-handlers opcode proc))

(define signal-condition (unspecific))

(define (signal-vm-exception opcode reason . args)
  (signal-condition (make-vm-exception opcode
				       (if reason
					   (enumerand->name reason exception)
					   #f)
				       args)))

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
