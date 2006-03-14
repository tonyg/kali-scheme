; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


;;;; Signalling conditions

; I don't like the term "signal," but that's the one Gnu Emacs Lisp,
; Common Lisp, and Dylan use, so it's probably best to stick with it.

(define make-condition cons)

(define (signal type . stuff)
  (signal-condition (make-condition type stuff)))

; Error

(define (error message . irritants)
  (apply signal 'error message irritants))

; Warn

(define (warn message . irritants)
  (signal-condition (make-condition 'warning (cons message irritants))))

; Note

(define (note message . irritants)
  (signal-condition (make-condition 'note (cons message irritants))))

; Syntax errors

(define (syntax-error message . rest)  ; Must return a valid expression.
  (signal-condition (make-condition 'syntax-error (cons message rest)))
  ''syntax-error)


; "Call error" - this means that the condition's "stuff" (cdr) is of
; the form (message procedure . args), and should be displayed appropriately.
; Proceeding from such an error should return the value that the call
; to the procedure on the args should have returned.

(define (call-error message proc . args)
  (signal-condition (make-condition 'call-error
				    (cons message (cons proc args)))))
