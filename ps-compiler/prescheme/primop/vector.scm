; Copyright (c) 1994 Richard Kelsey.  See file COPYING.

(define-scheme-primop make-vector
  'allocate
  (lambda (call)
    (make-pointer-type (node-type (call-arg call 1))))
  default-simplifier)

(define-scheme-primop vector-ref
  'read
  (lambda (call)
    (pointer-type-to (node-type (call-arg call 0))))
  default-simplifier)

(define-nonsimple-scheme-primop vector-set! 'write default-simplifier)

(define-scheme-primop make-string
  'allocate
  (lambda (call) type/string)
  default-simplifier)

(define-scheme-primop string-length
  #f
  (lambda (call) type/int32)
  default-simplifier)

(define-scheme-primop string-ref
  'read
  (lambda (call) type/int8u)
  default-simplifier)

(define-nonsimple-scheme-primop string-set! 'write default-simplifier)

(define-scheme-primop make-record
  'allocate
  (lambda (call)
    (literal-value (call-arg call 1)))
  default-simplifier)

(define-scheme-primop record-ref
  'read
  (lambda (call)
    (record-field-type (literal-value (call-arg call 1))))
  default-simplifier)

(define-nonsimple-scheme-primop record-set! 'write default-simplifier)

(define-scheme-primop deallocate
  'deallocate
  (lambda (call) type/unit)
  default-simplifier)

(define-scheme-primop allocate-memory
  'allocate
  (lambda (call) type/int32)
  default-simplifier)

(define-scheme-primop deallocate-memory
  'deallocate
  (lambda (call) type/unit)
  default-simplifier)

(define-scheme-primop copy-memory!
  'write
  (lambda (call) type/unit)
  default-simplifier)

(define-scheme-primop memory-equal?
  #f
  (lambda (call) type/boolean)
  default-simplifier)

(define-scheme-primop byte-ref 
  'read
  (lambda (call) type/int8u)
  default-simplifier)

(define-scheme-primop word-ref
  'read
  (lambda (call) type/int32)
  default-simplifier)

(define-nonsimple-scheme-primop byte-set! 'write default-simplifier)
(define-nonsimple-scheme-primop word-set! 'write default-simplifier)

; We delete the length argument because we don't need it.  This is allowable
; because trivial calls can't have WRITE side effects.

(define-scheme-primop char-pointer->string #f
  (lambda (call) type/string)
  (lambda (call)
    (if (= 2 (call-arg-count call))
	(remove-call-arg call 1))))

(define-scheme-primop char-pointer->nul-terminated-string #f
  (lambda (call) type/string)
  default-simplifier)

; (COMPUTED-GOTO <exit0> <exit1> ... <exitN> <dispatch-value>)

; Remove an unecessary coercion on the disptach-value, if possible.

(define (simplify-computed-goto call)
  (simplify-args call 0)
  (let ((value (call-arg call (call-exits call))))
    (cond ((and (call-node? value)
		(eq? 'coerce (primop-id (call-primop value)))
		(< (call-exits call) 256)
		(eq? type/int8u (literal-value (call-arg value 1))))
	   (replace value (detach (call-arg value 0)))))))

(define-nonsimple-scheme-primop computed-goto #f simplify-computed-goto)

