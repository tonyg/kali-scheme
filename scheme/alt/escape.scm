; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.



; For an explanation, see comments in rts/low.scm.

; The debugger invokes EXTRACT-CONTINUATION on a "native" continuation
; as obtained by PRIMITIVE-CWCC in order to get a VM continuation.
; The distinction between native and VM continuations is useful when
; debugging a program running under a VM that's different from
; whatever machine is running the debugger.

(define-record-type escape :escape
  (make-escape proc)
  (proc escape-procedure))

(define (with-continuation esc thunk)
  (if esc
      ((escape-procedure esc) thunk)
      (let ((answer (thunk)))
	(signal 'vm-return answer)	;#f means halt
	(call-error "halt" answer))))

(define (primitive-cwcc proc)
  (call-with-current-continuation
    (lambda (done)
      ((call-with-current-continuation
	 (lambda (k)
	   (call-with-values
	       (lambda ()
		 (proc (make-escape k)))
	     done)))))))
