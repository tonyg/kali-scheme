; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; --------------------

; Fake interrupt and exception system.
; This needs to be reconciled with alt/primitives.scm.

(define (with-exceptions thunk)
  (with-handler
       (lambda (c punt)
	 (cond ((and (exception? c)
		     (procedure? (get-exception-handler)))
		(handle-exception-carefully c))
	       ((interrupt? c)
		(if (not (deal-with-interrupt c))
		    (punt)))
	       ;; ((vm-return? c)
	       ;;  (vm-return (cadr c)))
	       (else
		(punt))))
     thunk))

(define (handle-exception-carefully c)
  (display "(Exception: ") (write c) (display ")") (newline)
  (noting-exceptional-context c
    (lambda ()
      (raise-exception (exception-opcode c)
		       (exception-arguments c)))))

(define (noting-exceptional-context c thunk)
  (call-with-current-continuation
    (lambda (k)
      ;; Save for future inspection, just in case.
      (set! *exceptional-context* (cons c k))
      (thunk))))

(define *exceptional-context* #f)

(define (deal-with-interrupt c)
  (noting-exceptional-context c
    (lambda ()
      (maybe-handle-interrupt
       (if (and (pair? (cdr c)) (integer? (cadr c)))
	   (cadr c)
	   (enum interrupt keyboard))))))

; (define (poll-for-interrupts) ...)


; Get the whole thing started

(define (?start-with-exceptions entry-point arg)
  (with-exceptions
   (lambda ()
     (?start entry-point arg))))

(define (in struct form)
  (eval form (structure-package struct)))
