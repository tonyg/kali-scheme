; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; This is file resume.scm.

; All sizes are in cells

(define (required-init-space startup-vector startup-vector-length)
  (+ (+ (do ((i 0 (+ i 1))
	     (s 0 (+ s (vm-string-size
			(string-length
			 (vector-ref startup-vector i))))))
	    ((>= i startup-vector-length)
	     s))
	(code-vector-size 2))
     (+ initial-interpreter-heap-space
	(+ initial-stack-heap-space
	   initial-i/o-heap-space))))

(define (initialize-vm stack-begin stack-size)
  (initialize-stack stack-begin stack-size)
  (initialize-interpreter))

; The call to fire up the interpreter.

(define (call-startup-procedure startup-proc
				startup-vector
				startup-vector-length)
  (let ((vector (vm-make-vector startup-vector-length universal-key)))
    (do ((i 0 (+ i 1)))
	((>= i startup-vector-length))
      (vm-vector-set! vector i (enter-string (vector-ref startup-vector i))))
    (clear-registers)
    (push vector)
    (receive (input output error)
	(initialize-i/o-system)
      (push input)
      (push output)
      (push error)
      (let ((code (make-code-vector 2 universal-key)))
	(code-vector-set! code 0 (enum op call))
	(code-vector-set! code 1 4)         ; nargs    
	(set! *code-pointer* (address-after-header code)))
      (restart startup-proc))))
  
; Restart the interpreter.

(define (restart value)
  (set! *val* value)
  (let loop ()
    (let ((option (interpret *code-pointer*)))
      (cond ((= option (enum return-option exit))
	     *val*)
	    ((= option (enum return-option external-call))
	     (set! *val* (((external "(long(*)())"  ; cast to a procedure
				     (=> (integer) (=> (integer address) integer))
				     (lambda (x) x))
			   (fetch (address-after-header (external-value *val*))))
			  *external-call-nargs*
			  (pointer-to-stack-arguments)))
	     ; remove proc and args
	     (remove-stack-arguments (+ *external-call-nargs* 1))
	     (loop))
	    (else
             (error "unknown VM return option" option)
	     -1)))))

(define *external-call-nargs*)

(define-enumeration return-option
  (exit
   external-call
   native-call
   native-return
   ))
