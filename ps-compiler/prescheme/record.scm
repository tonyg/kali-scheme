; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; Records that translate into C structs.

; Representation of record types.

(define-record-type record-type :record-type
  (really-make-record-type name)
  record-type?
  (name record-type-name)
  ; FIELDS and CONSTRUCTOR-ARGS are filled in later because of circularity
  (fields record-type-fields set-record-type-fields!)
  (constructor-args			; fields passed to the constructor
   record-type-constructor-args set-record-type-constructor-args!))

(define-record-discloser :record-type
  (lambda (rtype)
    (list 'record-type (record-type-name rtype))))

; Fields of record types.

(define-record-type record-field :record-field
  (make-record-field record-type name type)
  record-field?
  (record-type	record-field-record-type)
  (name		record-field-name)
  (type		record-field-type))

; Global table of record types.  Since we compile to a single C file the
; record types used within a single computation must have distinct names.
; (This should really be a fluid.)

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

; Construction a record type.  This gets the name, the list of fields whose
; initial values are passed to the constructor, and the field specifications.
; Each field specification consists of a name and a type.

(define (make-record-type id constructor-args specs)
  (let ((rt (really-make-record-type id)))
    (if (table-ref *record-type-table* id)
	(user-error "multiple definitions of record type ~S" id))
    (table-set! *record-type-table* id rt)
    (set-record-type-fields! rt (map (lambda (spec)
				       (make-record-field
					  rt
					  (car spec)
					  (expand-type-spec (cadr spec))))
				     specs))
    (set-record-type-constructor-args! rt
				       (map (lambda (name)
					      (get-record-type-field id name))
					    constructor-args))
    rt))

; Return the field record for FIELD-ID in record-type TYPE-ID.

(define (get-record-type-field type-id field-id)
  (let ((rtype (get-record-type type-id)))
    (cond ((any (lambda (field)
		  (eq? field-id (record-field-name field)))
		(record-type-fields rtype))
	   => identity)
	  (else
	   (user-error "~S is not a field of ~S" field-id rtype)))))

; The macro expander for DEFINE-RECORD-TYPE.
;
; (define-record-type <id> <type-id>
;   (<constructor> . <field-names>)
;   (<field-name> <type> <accessor-name> [<modifier-name>]) ...)
; 
; The <type-id> is used only by Pre-Scheme-in-Scheme.

(define (expand-define-record-type exp r c)
  (let ((id (cadr exp))
	(maker (cadddr exp))
	(fields (cddddr exp)))
    (let ((rt (make-record-type id (cdr maker) fields)))
      `(,(r 'begin)
	(,(r 'define) ,maker
	   (,(r 'let) ((,(r id) (,(r 'make-record) ',id)))
	     (,(r 'if) (,(r 'not) (,(r 'null-pointer?) ,(r id)))
	       (,(r 'begin)
		 . ,(map (lambda (name)
			   `(,(r 'record-set!) ,(r id) ,name ',id ',name))
			 (cdr maker))))
	     ,(r id)))
	,@(map (lambda (field)
		 `(,(r 'define) (,(caddr field) ,(r id))
				(,(r 'record-ref) ,(r id) ',id ',(car field))))
	       fields)
	,@(map (lambda (field)
		 `(,(r 'define) (,(cadddr field) ,(r id) ,(r 'x))
			(,(r 'record-set!)
			 ,(r id) ,(r 'x)',id ',(car field))))
	       (filter (lambda (spec)
			 (not (null? (cdddr spec))))
		       fields))))))

; primitives
;  (make-record 'type . args)
;  (record-ref thing 'type 'field)
;  (record-set! thing value 'type 'field)
;
; C record creator
;  global list of these things

