; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file resume.scm.

(define (s48-initialize-vm stack-begin stack-size)
  (install-symbols!+gc (s48-initial-symbols))
  (install-shared-bindings!+gc (s48-initial-imported-bindings)
			       (s48-initial-exported-bindings))
  (initialize-stack+gc stack-begin stack-size)
  (initialize-interpreter+gc)
  (initialize-bignums)
  (initialize-proposals!+gc))

;----------------
; Push the arguments to the initial procedure (a vector of strings passed
; in from the outside and the three standard channels) and call it.

(define (s48-call-startup-procedure startup-vector startup-vector-length)
  (clear-registers)
  (push (enter-startup-argument+gc startup-vector startup-vector-length))
  (receive (input output error)
      (initialize-i/o-system+gc)
    (push input)
    (push output)
    (push error)
    (push (s48-resumer-records))
    (s48-initialization-complete!)
    (s48-restart (s48-startup-procedure) 5)))
  
(define (enter-startup-argument+gc startup-vector startup-vector-length)
  (let ((vector (vm-make-vector+gc startup-vector-length)))
    (natural-for-each (lambda (i)
			(vm-vector-set! vector
					i
					(enter-string+gc
					 (vector-ref startup-vector i))))
		      startup-vector-length)
    vector))

;----------------
; Restart the interpreter, calling PROC with NARGS arguments already on the
; stack.

(define (s48-restart proc nargs)
  (cond ((closure? proc)
         (set-val! proc)
	 (let ((retval (perform-application nargs)))
	   (remove-current-frame)
	   retval))
	(else
	 (error "s48-restart called with non-procedure" proc))))
