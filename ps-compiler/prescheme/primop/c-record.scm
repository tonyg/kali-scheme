; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.


(define-c-generator make-record #t
  (lambda (args)
    (bug "no eval method for MAKE-RECORD"))
  (lambda (call depth)
    (reconstruct-make-record call depth))
  (lambda (call port indent)
    (let ((type (node-type call)))
      (write-c-coercion type port)
      (format port "malloc(sizeof(")
      (display-c-type (pointer-type-to type) #f port)
      (format port ") * ")
      (c-value (call-arg call 0) port)
      (format port ")"))))
      
(define (reconstruct-make-record call depth)
  (let* ((args (call-exp-args call))
	 (arg-types (call-arg-types (cdr args) depth))
	 (record-type (quote-exp-value (car args)))
	 (type (record-type-type record-type))
	 (maker-type (record-type-maker-type record-type)))
    (unify! maker-type (make-arrow-type arg-types type))
    type))

(define-c-scheme-primop make-record
  'allocate
  (lambda (call)
    (record-type-type (literal-value (node-ref call 0))))
  default-simplifier)

(define-scheme-primop record-ref
  'read
  (lambda (call)
    (record-slot-type (literal-value (node-ref call 0))))
  default-simplifier)

(define-scheme-primop record-set!
  'write
  (lambda (call) type/unit)
  default-simplifier)
