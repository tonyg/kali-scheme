

; (make-record '<type> <value>*)

(define-scheme-primop make-record
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

