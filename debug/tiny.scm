; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; Tiny image test system

;(initialize-defpackage! ?? ??)
;(define-structure tiny-system (export start)
;  (define-all-operators)
;  (files (debug tiny)))
;(link-simple-system '(debug tiny) 'start tiny-system)


(define (start arg in out)
  (write-string "Hello " out)
  (if (string? arg)
      (write-string arg out))
  (write-char #\newline out)
  (force-output out)
  0)

