; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file record.scm.

;;;; Records

; This is completely vanilla Scheme code.  Should work anywhere.

(define (make-record-type type-id field-names)

  (define unique (list type-id))

  (define size (+ (length field-names) 1))

  (define (constructor . names-option)
    (let* ((names (if (null? names-option)
		      field-names
		      (car names-option)))
	   (number-of-inits (length names))
	   (indexes (map field-index names)))
      (lambda field-values
	(if (= (length field-values) number-of-inits)
	    (let ((record (make-vector size 'uninitialized)))
	      (vector-set! record 0 unique)
	      (for-each (lambda (index value)
			  (vector-set! record index value))
			indexes
			field-values)
	      record)
	    (error "wrong number of arguments to record constructor"
		   field-values type-id names)))))

  (define (predicate obj)
    (and (vector? obj)
	 (= (vector-length obj) size)
	 (eq? (vector-ref obj 0) unique)))

  (define (accessor name)
    (let ((i (field-index name)))
      (lambda (record)
	(if (predicate record) ;Faster: (eq? (vector-ref record 0) unique)
	    (vector-ref record i)
	    (error "invalid argument to record accessor"
		   record type-id name)))))

  (define (modifier name)
    (let ((i (field-index name)))
      (lambda (record new-value)
	(if (predicate record) ;Faster: (eq? (vector-ref record 0) unique)
	    (vector-set! record i new-value)
	    (error "invalid argument to record modifier"
		   record type-id name)))))

  (define (field-index name)
    (let loop ((l field-names) (i 1))
      (if (null? l)
	  (error "bad field name" name)
	  (if (eq? name (car l))
	      i
	      (loop (cdr l) (+ i 1))))))

  (define the-descriptor
    (lambda (request)
      (case request
	((constructor) constructor)
	((predicate) predicate)
	((accessor) accessor)
	((modifier) modifier)
	((name) type-id)
	((field-names) field-names))))

  the-descriptor)

(define (record-constructor r-t . names-option)
  (apply (r-t 'constructor) names-option))

(define (record-predicate r-t)
  (r-t 'predicate))

(define (record-accessor r-t field-name)
  ((r-t 'accessor) field-name))

(define (record-modifier r-t field-name)
  ((r-t 'modifier) field-name))

(define (record-type-name r-t) (r-t 'name))
(define (record-type-field-names r-t) (r-t 'field-names))

(define (record-type? r-t)
  (and (procedure? r-t)
       (error "record-type? not implemented" r-t)))

(define (define-record-discloser r-t proc)
  "ignoring define-record-discloser form")
