; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Portable versions of low-level things that would really like to rely
; on the Scheme 48 VM or on special features provided by the byte code
; compiler.

(define (vector-unassigned? v i) #f)

(define (flush-the-symbol-table!) #f)

(define maybe-open-input-file open-input-file)
(define maybe-open-output-file open-output-file)


; Suppress undefined export warnings.

(define-syntax %file-name%
  (syntax-rules ()
    ((%file-name%) "")))

(define-syntax structure-ref
  (syntax-rules ()
    ((structure-ref ?struct ?name)
     (error "structure-ref isn't implemented" '?struct '?name))))
