; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file schemetoc-record.scm.
; Synchronize any changes with the other *record.scm files.

;;;; Records

(define (make-record-type type-id field-names)

  (define unique (lambda () the-descriptor))

  (define size (+ (length field-names) 1))

  (define (constructor . names-option)
    (let* ((names (if (null? names-option)
		      field-names
		      (car names-option)))
	   (foo (cons unique
		      (map (lambda (name) 'uninitialized) field-names)))
	   (number-of-inits (length names))
	   (indexes (map field-index names)))
      (lambda field-values
	(if (= (length field-values) number-of-inits)
	    (let ((record (list->%record foo)))
	      (for-each (lambda (index value)
			  (%record-set! record index value))
			indexes
			field-values)
	      (%record-methods-set! record usual-record-methods)
	      record)
	    (error "wrong number of arguments to record constructor"
		   field-values type-id names)))))

  (define (predicate obj)
    (and (%record? obj)
	 (= (%record-length obj) size)
	 (eq? (%record-ref obj 0) unique)))

  (define (accessor name)
    (let ((i (field-index name)))
      (lambda (record)
	(if (predicate record) ;Faster: (eq? (%record-ref record 0) unique)
	    (%record-ref record i)
	    (error "invalid argument to record accessor"
		   record type-id name)))))

  (define (modifier name)
    (let ((i (field-index name)))
      (lambda (record new-value)
	(if (predicate record) ;Faster: (eq? (%record-ref record 0) unique)
	    (%record-set! record i new-value)
	    (error "invalid argument to record modifier"
		   record type-id name)))))

  (define (field-index name)
    (let loop ((l field-names) (i 1))
      (if (null? l)
	  (error "bad field name" name)
	  (if (eq? name (car l))
	      i
	      (loop (cdr l) (+ i 1))))))

  (define (discloser r) (list type-id))

  (define the-descriptor
    (lambda (request)
      (case request
	((constructor) constructor)
	((predicate) predicate)
	((accessor) accessor)
	((modifier) modifier)
	((identification) type-id)
	((field-names) field-names)
	((discloser) discloser)
	((set-discloser!) (lambda (d) (set! discloser d))))))

  the-descriptor)

(define (record-type x)
  (if (%record? x)
      (let ((probe (%record-ref x 0)))
	(if (procedure? probe)
	    (probe)
	    #f))
      #f))

(define (record-type-identification r-t)
  (r-t 'identification))

(define (record-type-field-names r-t)
  (r-t 'field-names))

(define (record-constructor r-t . names-option)
  (apply (r-t 'constructor) names-option))

(define (record-predicate r-t)
  (r-t 'predicate))

(define (record-accessor r-t field-name)
  ((r-t 'accessor) field-name))

(define (record-modifier r-t field-name)
  ((r-t 'modifier) field-name))

(define (define-record-discloser r-t proc)
  ((r-t 'set-discloser!) proc))

(define (disclose-record r)
  (((record-type r) 'discloser) r))

(define usual-record-methods
  (list (cons '%to-write
	      (lambda (r port indent levels length seen)
		(write-char #\# port)
		(write-char %record-prefix-char port)
		(list (disclose-record r))))))

(set! %record-prefix-char #\~)
