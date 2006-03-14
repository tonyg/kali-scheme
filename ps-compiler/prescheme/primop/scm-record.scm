; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.

(define-complex-primitive (make-record symbol?)
  (lambda (type)
    (bug "no evaluator for MAKE-RECORD"))
  (lambda (args node depth return?)
    (let ((type-id (cadr (node-form (car args)))))
      (make-pointer-type (get-record-type type-id))))
  #f  ; no closed form
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'make-record) args type)))

(define-complex-primitive (record-ref any?  ; no RECORD? available
				      symbol? symbol?)
  (lambda (thing type field)
    (bug "no evaluator for RECORD-REF"))
  (lambda (args node depth return?)
    (let ((type-id (cadr (node-form (cadr args))))
	  (field-id (cadr (node-form (caddr args)))))
      (let ((record-type (make-pointer-type (get-record-type type-id)))
	    (field-type (record-field-type
			 (get-record-type-field type-id field-id))))
	(check-arg-type args 0 record-type depth node)
	field-type)))
  #f  ; no closed form
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'record-ref) args type)))

(define-complex-primitive (record-set! any?  ; no RECORD? available
				       any? symbol? symbol?)
  (lambda (thing value type field)
    (bug "no evaluator for RECORD-SET!"))
  (lambda (args node depth return?)
    (let ((type-id (cadr (node-form (caddr args))))
	  (field-id (cadr (node-form (cadddr args)))))
      (let ((record-type (make-pointer-type (get-record-type type-id)))
	    (field-type (record-field-type
			 (get-record-type-field type-id field-id))))
	(check-arg-type args 0 record-type depth node)
	(check-arg-type args 1 field-type depth node)
	type/unit)))
  #f  ; no closed form
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'record-set!) args type)))

; (x->union value 'union-type 'variant)
;
;(define-complex-primitive (x->union any?
;                                    symbol?             ; union type
;                                    symbol?)            ; variant
;  (lambda (thing type field)
;    (bug "no evaluator for X->UNION"))
;  (lambda (args node depth return?)
;    (let ((type-id (cadr (node-form (cadr args))))
;          (member-id (cadr (node-form (caddr args)))))
;      (let ((union-type (make-pointer-type (get-union-type type-id)))
;            (field-type (union-member-type
;                         (get-union-type-member type-id member-id))))
;        (check-arg-type args 0 field-type depth node)
;        union-type)))
;  #f  ; no closed form
;  (lambda (args type)
;    (make-primop-call-node (get-prescheme-primop 'x->union) args type)))





