; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file resume.scm.

(define (s48-initialize-vm stack-begin stack-size)
  (install-symbols!+gc (s48-initial-symbols))
  (install-shared-bindings!+gc (s48-initial-imported-bindings)
			       (s48-initial-exported-bindings))
  (initialize-stack+gc stack-begin stack-size)
  (initialize-interpreter+gc))

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
  (let* ((size (+ (do ((i 0 (+ i 1))
		       (size 0 (+ size (vm-string-size
					(string-length
					 (vector-ref startup-vector i))))))
		      ((= i startup-vector-length)
		       size))
		  (vm-vector-size startup-vector-length)))
	 (key (ensure-space size))
	 (vector (vm-make-vector startup-vector-length key)))
    (natural-for-each (lambda (i)
			(vm-vector-set! vector
					i
					(enter-string
					 (vector-ref startup-vector i)
					 key)))
		      startup-vector-length)
    vector))

;----------------
; Restart the interpreter, calling PROC with NARGS arguments already on the
; stack.

(define (s48-restart proc nargs)
  (receive (key proc)
      (ensure-space-saving-temp (code-vector-size 2) proc)
    (let ((code (make-code-vector 2 key)))
      (code-vector-set! code 0 (enum op call))
      (code-vector-set! code 1 nargs)
      (set-code-pointer! code 0)
      (set-val! proc)
      (interpret (code-pointer)))))
