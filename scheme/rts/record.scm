; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;;;; Records

; Every record in the image is assumed to be made either by MAKE-RECORD-TYPE
; or by a procedure returned by record-constructor.  A record-type is a
; record that describes a type of record.  At the end of the file we create
; a record type that describes record types.

; We number the record types for debugging purposes.

(define *record-type-uid* -1)

; This is the record type that describes record types.  It is set a the end
; of the file.  Its first slot points to itself.

(define *record-type* #f)

; Make a record type from a name, used for printing and debugging, and
; a list of field names.
;
; The VM references both the record type and the resumer, so their offsets
; should not be changed.

(define (make-record-type name field-names)
  (set! *record-type-uid* (+ *record-type-uid* 1))
  (let ((r (make-record 7 (unspecific))))
    (record-set! r 0 *record-type*)
    (record-set! r 1 default-record-resumer)
    (record-set! r 2 *record-type-uid*)
    (record-set! r 3 name)
    (record-set! r 4 field-names)
    (record-set! r 5 (length field-names))
    (record-set! r 6 (make-default-record-discloser name))
    r))

(define (record-type? obj)
  (and (record? obj)
       (eq? (record-type obj) *record-type*)))

; The various fields in a record type.

(define (record-type-resumer rt)          (record-ref rt 1))
(define (set-record-type-resumer! rt r)   (record-set! rt 1 r))
(define (record-type-uid rt)              (record-ref rt 2))
(define (record-type-name rt)             (record-ref rt 3))
(define (record-type-field-names rt)      (record-ref rt 4))
(define (record-type-number-of-fields rt) (record-ref rt 5))
(define (record-type-discloser rt)        (record-ref rt 6))
(define (set-record-type-discloser! rt d) (record-set! rt 6 d))

; This is a hack; it is read by the script that makes c/scheme48.h.

(define record-type-fields
  '(resumer uid name field-names number-of-fields discloser))

;----------------
; Given a record type and the name of a field, return the field's index.

(define (record-field-index rt name)
  (let loop ((names (record-type-field-names rt))
	     (i 1))
    (cond ((null? names)
	   (assertion-violation 'record-field-index
				"unknown field"
				(record-type-name rt)
				name))
	  ((eq? name (car names))
	   i)
	  (else
	   (loop (cdr names) (+ i 1))))))

; Return procedure for contstruction records of type RT.  NAMES is a list of
; field names which the constructor will take as arguments.  Other fields are
; uninitialized.

(define (record-constructor rt names)
  (let ((indexes (map (lambda (name)
			(record-field-index rt name))
		      names))
	(size (+ 1 (record-type-number-of-fields rt))))
    (lambda args
      (let ((r (make-record size (unspecific))))
	(record-set! r 0 rt)
	(let loop ((is indexes) (as args))
	  (if (null? as)
	      (if (null? is)
		  r
		  (assertion-violation 'record-constructor
				       "too few arguments to record constructor"
				       rt names args))
	      (if (null? is)
		  (assertion-violation 'record-constructor
				       "too many arguments to record constructor"
				       rt names args)
		  (begin (record-set! r (car is) (car as))
			 (loop (cdr is) (cdr as))))))))))

; Making accessors, modifiers, and predicates for record types.

(define (record-accessor rt name)
  (let ((index (record-field-index rt name))
	(error-cruft `(record-accessor ,rt ',name)))
    (lambda (r)
      (if (eq? (record-type r) rt)
	  (record-ref r index)
	  (assertion-violation 'record-accessor "invalid record access"
			       error-cruft r)))))

(define (record-modifier rt name)
  (let ((index (record-field-index rt name))
	(error-cruft `(record-modifier ,rt ',name)))
    (lambda (r x)
      (if (eq? (record-type r) rt)
	  (record-set! r index x)
	  (assertion-violation 'record-modifier "invalid record modification"
			       error-cruft r x)))))

(define (record-predicate rt)
  (lambda (x)
    (and (record? x)
	 (eq? (record-type x) rt))))

;----------------
; A discloser is a procedure that takes a record of a particular type and
; returns a list whose head is a string or symbol and whose tail is other
; stuff.
;
; Set the discloser for record type RT.

(define (define-record-discloser rt proc)
  (if (and (record-type? rt)
	   (procedure? proc))
      (set-record-type-discloser! rt proc)
      (assertion-violation 'define-record-discloser "invalid argument" rt proc)))

; By default we just return the name of the record type.

(define (make-default-record-discloser record-type-name)
  (lambda (r)
    (list record-type-name)))

; DISCLOSE-RECORD calls the record's discloser procedure to obtain a list.

(define (disclose-record r)
  (if (record? r)
      (let ((rt (record-type r)))
	(if (record-type? rt)
	    ((record-type-discloser rt) r)
	    #f))
      #f))

;----------------
; A resumer is a procedure that the VM calls on all records of a given
; type on startup.
;
; A resumer may be:
;  #t -> do nothing on startup.
;  #f -> records of this type do not survive a dump/resume; in images they
;        are replaced by their first slot (so we make sure they have one)
;  a one-argument procedure -> pass the record to this procedure
;
; Resumers are primarily intended for use by external code which keeps
; fields in records which do not survive a dump under their own power.
; For example, a record may contain a reference to a OS-dependent value.
;
; Resumers are called by the VM on startup.

(define (define-record-resumer rt resumer)
  (if (and (record-type? rt)
	   (or (eq? #t resumer)
	       (and (eq? #f resumer)
		    (< 0 (record-type-number-of-fields rt)))
	       (procedure? resumer)))
      (set-record-type-resumer! rt resumer)
      (assertion-violation 'define-record-resumer "invalid argument" rt resumer)))

; By default we leave records alone.

(define default-record-resumer
  #t)

(define (initialize-records! resumer-records)
  (if (vector? resumer-records)
      (do ((i 0 (+ i 1)))
	  ((= i (vector-length resumer-records)))
	(resume-record (vector-ref resumer-records i)))))

(define (resume-record record)
  ((record-type-resumer (record-type record))
     record))

;----------------
; Initializing *RECORD-TYPE* and making a type.

(set! *record-type*
      (make-record-type 'record-type record-type-fields))

(record-set! *record-type* 0 *record-type*)

(define :record-type *record-type*)

(define-record-discloser :record-type
  (lambda (rt)
    (list 'record-type
	  (record-type-uid rt)
	  (record-type-name rt))))

