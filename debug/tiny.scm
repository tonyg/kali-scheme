; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; Tiny image test system

;(initialize-defpackage! ?? ??)
;(define-structure base (make-compiler-base))
;(define-package ((tiny-system (export start)))
;  (open base)
;  (files (debug tiny)))
;(link-simple-system '(debug tiny) 'start tiny-system)

(define (start arg in out)
  (write-string "Hello " out)
  (if (string? arg)
      (write-string arg out))
  (write-char #\newline out)
  (force-output out)
  0)
