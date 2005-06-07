; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Christopher P. Haynes and Daniel P. Friedman.
; Engines build process abstractions.
; 1984 ACM Symposium on Lisp and Functional Programming, pages 18-24.

; This is incompatible with the threads package.

; ,open primitives interrupts

(define interrupt/alarm (enum interrupt alarm))

(define (run thunk interval when-done when-timeout)
  (let ((save (vector-ref interrupt-handlers interrupt/alarm)))
    (let ((finish
	   (call-with-current-continuation
	     (lambda (k)
	       (vector-set! interrupt-handlers
			    interrupt/alarm
			    (lambda (tem ei)
			      (set-enabled-interrupts! ei)
			      (call-with-current-continuation
				(lambda (resume)
				  (k (lambda ()
				       (when-timeout (lambda ()
						       (resume #f)))))))))
	       (schedule-interrupt interval *exponent* #f)
	       (call-with-values thunk
		 (lambda vals
		   (let ((time-remaining (schedule-interrupt 0 0 #f)))
		     (lambda ()
		       (apply when-done time-remaining vals)))))))))
      (vector-set! interrupt-handlers
		   interrupt/alarm
		   save)
      (finish))))

(define *exponent* -3)

(define-syntax engine
  (syntax-rules ()
    ((engine ?E) (%engine (lambda () ?E)))))

(define (%engine thunk)
  (lambda (ticks done out)
    (run thunk
	 ticks
	 (lambda (ticks val)
	   (done val ticks))
	 (lambda (new-thunk)
	   (out (%engine new-thunk))))))


; Example from the LFP '84 paper (verbatim)

;(define-syntax rec
;  (syntax-rules () ((rec ?X ?E) (letrec ((?X ?E)) ?X))))
;
;(define complete
;  (lambda (eng)
;    ((rec loop
;       (lambda (eng count)
;         (eng 1000
;              (lambda (val ticks-left)
;                (cons val
;                      (+ (- 1000 ticks-left)
;                         count)))
;              (lambda (eng)
;                (loop eng (+ count 1000))))))
;     eng 0)))
