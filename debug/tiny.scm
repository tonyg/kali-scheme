; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; Tiny image test system

;(initialize-defpackage! ?? ??)
;(define-package ((tiny-system (export start)))
;  (define-all-operators)
;  (files (debug tiny))
;  (optimize auto-integrate))
;(link-simple-system '(debug tiny) 'start tiny-system)

(define (foo x) (lambda (y)
		  (if y
		      'foo
		      (begin (set! x 13) 'bar))))

(define (start arg in out)
  (write-string "Hello " out)
  (if (string? arg)
      (write-string arg out))
  (write-char #\newline out)
  (force-output out)
  (lambda ()
    (+ 1 2 3)
    ((foo 1) 4 5))
  0)

