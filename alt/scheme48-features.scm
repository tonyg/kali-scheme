; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file scheme48-features.scm.
; Synchronize any changes with the other *-features.scm files.

;;;; Features

(define error (access-scheme48 'error))
(define warn (access-scheme48 'warn))

;(define eval (access-scheme48 'eval))
;(define interaction-environment (access-scheme48 'package-for-load))
;(define scheme-report-environment
;  (let ((env (interaction-environment)))
;    (lambda (n) env)))

;(define (ignore-errors thunk)
;  '(error "ignore-errors isn't implemented"))
(define ignore-errors (access-scheme48 'ignore-errors))

(define force-output (access-scheme48 'force-output))

; The rest of these can be done in portable Scheme (see e.g.
; alt/ascii.scm and alt/bitwise.scm), but are imported from the
; Scheme48 implementation for the sake of speed.

(define char->ascii  (access-scheme48 'char->ascii))
(define ascii->char  (access-scheme48 'ascii->char))  ;dump/restore needs

(define string-hash (access-scheme48 'string-hash))

(define arithmetic-shift (access-scheme48 'arithmetic-shift))
(define bitwise-and (access-scheme48 'bitwise-and))
(define bitwise-ior (access-scheme48 'bitwise-ior))

(define make-code-vector   (access-scheme48 'make-code-vector))
(define code-vector?	   (access-scheme48 'code-vector?))
(define code-vector-length (access-scheme48 'code-vector-length))
(define code-vector-ref	   (access-scheme48 'code-vector-ref))
(define code-vector-set!   (access-scheme48 'code-vector-set!))
