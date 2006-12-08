; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

; The argument list needs to be in sync with MAKE-USUAL-RESUMER in
; rts/init.scm, and MAKE-STARTUP-PROCEDURE in bcomp/comp.scm.

(define (s48-call-startup-procedure startup-vector startup-vector-length)
  (clear-registers)
  (push (enter-startup-argument+gc startup-vector startup-vector-length))
  (receive (input input-encoding output output-encoding error error-encoding)
      (initialize-i/o-system+gc)
    (push input)
    (push input-encoding)
    (push output)
    (push output-encoding)
    (push error)
    (push error-encoding)
    (push (s48-resumer-records))
    (s48-initialization-complete!)
    (s48-restart (s48-startup-procedure) 8)))
  
(define (enter-startup-argument+gc startup-vector startup-vector-length)
  (let* ((argv-total-bytes-count
	  (let loop ((i 0) (count 0))
	    (if (= i startup-vector-length)
		count
		(goto loop
		      (+ 1 i)
		      (+ count (+ (string-length (vector-ref startup-vector i)) 1))))))
	 (key (ensure-space
	       (+ stob-overhead startup-vector-length
		  (* startup-vector-length stob-overhead)
		  (bytes->cells argv-total-bytes-count))))
	 (vector (make-d-vector (enum stob vector) startup-vector-length key)))
    (natural-for-each (lambda (i)
			(vm-vector-set! vector
					i
					(enter-os-string-byte-vector
					 (vector-ref startup-vector i)
					 key)))
		      startup-vector-length)
    vector))

(define (enter-os-string-byte-vector s key)
  (let* ((len (string-length s))
	 (vec (make-code-vector (+ len 1) key))) ; NUL
    (do ((i 0 (+ 1 i)))
	((> i len) vec)
      (code-vector-set! vec i (char->ascii (string-ref s i))))))
      
;----------------
; Restart the interpreter, calling PROC with NARGS arguments already on the
; stack.

(define (s48-restart proc nargs)
  (cond ((closure? proc)
         (set-val! proc)
	 (let ((retval (perform-application nargs)))
	   ;; This is necessary to remove the stack from a callback
	   ;; from C.  If we don't do this, a single callback works,
	   ;; but two in a row fails.  I'm not sure if this is the
	   ;; right place for this fix.  --Mike
	   (remove-current-frame)
	   retval))
	(else
	 (error "s48-restart called with non-procedure" proc))))
