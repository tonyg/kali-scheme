
(define-record-type record-type
  (name)
  (fields)  ; filled in later because of circularity
  )

(define-record-discloser type/record-type
  (lambda (rtype)
    (list 'record-type (record-type-name rtype))))

(define-record-type record-field
  (record-type
   name
   type
   )
  ())

(define *record-type-table* (make-symbol-table))

(define (reset-record-data!)
  (set! *record-type-table* (make-symbol-table)))

(define (get-record-type id)
  (cond ((table-ref *record-type-table* id)
	 => identity)
	(else
	 (error "no record type ~S" id))))

(define (lookup-record-type id)
  (table-ref *record-type-table* id))

(define (all-record-types)
  (table->entry-list *record-type-table*))

(define (make-record-type id specs)
  (let ((rt (record-type-maker id)))
    (if (table-ref *record-type-table* id)
	(user-error "multiple definitions of record type ~S" id))
    (table-set! *record-type-table* id rt)
    (set-record-type-fields!
     rt
     (map (lambda (spec)
	    (record-field-maker rt
				(car spec)
				(expand-type-spec (cadr spec))))
	  specs))
    rt))

(define (get-record-type-field type-id field-id)
  (let ((rtype (get-record-type type-id)))
    (cond ((any (lambda (field)
		  (eq? field-id (record-field-name field)))
		(record-type-fields rtype))
	   => identity)
	  (else
	   (user-error "~S is not a field of ~S" field-id rtype)))))

(define (expand-define-record-type exp r c)
  (let ((id (cadr exp))
	(maker (caddr exp))
	(fields (cdddr exp)))
    (let ((rt (make-record-type id fields)))
      `(,(r 'begin)
	(,(r 'define) (,maker) (,(r 'make-record) ',id))
	,@(map (lambda (field)
		 `(,(r 'define) (,(concatenate-symbol id '- (car field))
				 ,(r id))
				(,(r 'record-ref) ,(r id) ',id ',(car field))))
	       fields)
	,@(map (lambda (field)
		 `(,(r 'define)
		   (,(concatenate-symbol 'set- id '- (car field) '!)
		    ,(r id) ,(r 'x))
		   (,(r 'record-set!)
		    ,(r id) ,(r 'x)',id ',(car field))))
	       fields)))))

; primitives
;  (make-record 'type)         ; what about running out of space?
;  (record-ref thing 'field)
;  (record-set! thing value 'field)
;
; C record creator
;  global list of these things
  
  