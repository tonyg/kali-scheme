; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file resume.scm.

;;;; Top level entry into Scheme48 system

; RESUME is the main entry point to the entire system

(define (resume filename startup-string)
  (let* ((resume-space (+ (vm-string-size (string-length startup-string))
			  (code-vector-size 2)))
	 (startup-proc (read-image filename resume-space)))
    (call-startup-procedure startup-proc startup-string)))

(define (call-startup-procedure startup-proc startup-string)
  (clear-registers)
  (push (enter-string startup-string))	; get it in the heap so suspend will
					; save it
  (push (initial-input-port))
  (push (initial-output-port))
  (let ((code (make-code-vector 2 universal-key)))
    (code-vector-set! code 0 op/call)
    (code-vector-set! code 1 3)         ; nargs    
    (set! *code-pointer* (address-after-header code)))
  (restart startup-proc))

(define (restart value)
  (set! *val* value)
  (let loop ()
    (let ((option (interpret)))
      (cond ((= option return-option/exit)
	     *val*)
	    ((= option return-option/external-call)
	     (set! *val* (call-external-value             ; type inference hack
			  (fetch (address-after-header (external-value *val*)))
			  *nargs*
			  (pointer-to-top-of-stack)))
	     (stack-add (- 0 (+ *nargs* 1)))  ; remove proc and args
	     (loop))
	    (else
             (error "unkown VM return option" option)
	     -1)))))

(define-enumeration return-option
  (exit
   external-call
   native-call
   native-return
   ))

;----------------------------------------------------------------
; Entry point for debugging

; Used by RUN

(define (start-vm thunk)
  (set! *val* thunk)
  (set! *nargs* 0)
  (set! *pending-interrupts* 0)
  (perform-application 0)
  *val*)

; (define (run x) (start-vm (make-closure (compile-form `(halt ,x) e) 'run)))
; (define write-instruction (access-scheme48 'write-instruction))

