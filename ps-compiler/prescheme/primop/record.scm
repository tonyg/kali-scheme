

; (make-record 'type-id)
; (record-ref <record> 'type-id 'field-id)
; (record-set! <record> <value> 'type-id 'field-id)

(define-scheme-primop make-record
  'allocate
  (lambda (call)
    (get-record-type (literal-value (node-ref call 0))))
  default-simplifier)

(define-scheme-primop record-ref
  'read
  (lambda (call)
    (record-field-type
     (get-record-type-field (get-record-type (literal-value (node-ref call 1)))
			    (literal-value (node-ref call 2)))))
  default-simplifier)

(define-scheme-primop record-set!
  'write
  (lambda (call) type/unit)
  default-simplifier)

