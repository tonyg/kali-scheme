; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Low-level things that can't rely on the fact that we're running under the
; Scheme 48 VM.

(define really-string->symbol string->symbol)

(define (reverse-list->string l n)
  (list->string (reverse l)))

(define (flush-the-symbol-table!) #f)


; I/O

(define maybe-open-input-file open-input-file)
(define maybe-open-output-file open-output-file)


; HALT is there for the exception system to call when all else fails.

(define (halt n)
  (with-continuation #f (lambda () n)))

(define (unspecific) (if #f #f))

(define (unassigned) '(*unassigned*))

; Suppress undefined export warnings.

(define-syntax %file-name%
  (syntax-rules ()
    ((%file-name%) "")))

(define-syntax structure-ref
  (syntax-rules ()
    ((structure-ref struct name)
     (error "structure-ref isn't implemented" 'struct 'name))))
