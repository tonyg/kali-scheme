; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.



; (make-record 'type-id)
; (record-ref <record> 'type-id 'field-id)
; (record-set! <record> <value> 'type-id 'field-id)

(define-polymorphic-scheme-primop make-record allocate
  (lambda (call)
    (get-record-type (literal-value (node-ref call 0)))))

(define-polymorphic-scheme-primop record-ref read
  (lambda (call)
    (record-field-type
     (get-record-type-field (get-record-type (literal-value (node-ref call 1)))
			    (literal-value (node-ref call 2))))))

(define-scheme-primop record-set! write type/unit)
