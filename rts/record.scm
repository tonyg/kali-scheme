; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; This is file record.scm.

;;;; Records

; Every record in the image is assumed to be made either by
; make-record-type or by a procedure returned by record-constructor.

(define (record-type r)
  (record-ref r 0))

(define *record-type-uid* 0)
(define record-type-marker (list 'record-type))

(define (make-record-type id field-names)
  (set! *record-type-uid* (+ *record-type-uid* 1))
  (let ((r (make-record 5 (unspecific))))
    (record-set! r 0 record-type-marker)
    (record-set! r 1 *record-type-uid*)
    (record-set! r 2 id)
    (record-set! r 3 field-names)
    (record-set! r 4 default-record-discloser)
    r))

(define (record-type? obj)
  (and (record? obj)
       (eq? (record-type obj) record-type-marker)))

(define (record-type-uid rt)            (record-ref rt 1))
(define (record-type-identification rt) (record-ref rt 2))
(define (record-type-field-names rt)    (record-ref rt 3))
(define (record-type-discloser rt)      (record-ref rt 4))

(define (record-field-index rt name)
  (let loop ((names (record-type-field-names rt))
	     (i 1))
    (cond ((null? names) (error "unknown field"
				(record-type-identification rt)
				name))
	  ((eq? name (car names))
	   i)
	  (else (loop (cdr names) (+ i 1))))))

(define (record-constructor rt names)
  (let ((indexes (map (lambda (name)
			(record-field-index rt name))
		      names))
	(size (+ 1 (length (record-type-field-names rt)))))
    (lambda args
      (let ((r (make-record size (unspecific))))
	(record-set! r 0 rt)
	(let loop ((is indexes) (as args))
	  (if (null? as)
	      (if (null? is)
		  r
		  (error "too few arguments to record constructor"
			 rt names args))
	      (if (null? is)
		  (error "too many arguments to record constructor"
			 rt names args)
		  (begin (record-set! r (car is) (car as))
			 (loop (cdr is) (cdr as))))))))))

(define (record-accessor rt name)
  (let ((index (record-field-index rt name))
	(error-cruft `(record-accessor ,rt ',name)))
    (lambda (r)
      (if (eq? (record-type r) rt)
	  (record-ref r index)
	  (call-error "invalid record access" error-cruft r)))))

(define (record-modifier rt name)
  (let ((index (record-field-index rt name))
	(error-cruft `(record-modifier ,rt ',name)))
    (lambda (r x)
      (if (eq? (record-type r) rt)
	  (record-set! r index x)
	  (call-error "invalid record modification" error-cruft r x)))))

(define (record-predicate rt)
  (lambda (x)
    (and (record? x)
	 (eq? (record-type x) rt))))

; disclose-record calls the record's discloser procedure to obtain a list
; whose head is a string and whose tail is other stuff.

(define (define-record-discloser rt proc)
  (if (record-type? rt)
      (record-set! rt 4 proc)
      (call-error "invalid record type" define-record-discloser rt proc)))

(define (disclose-record r)
  (let ((p (record-type-discloser (record-type r))))
    (if (procedure? p)
	(p r)
	(list (record-type-uid (record-type r))))))

(define (default-record-discloser r)
  (list (record-type-identification (record-type r))))

(define-method disclose-methods (make-family 'record 0)
  (lambda (obj)
    (cond ((not (record? obj)) (fail))
	  ((record-type? obj)
	   (list 'record-type
		 (record-type-uid obj)
		 (record-type-identification obj)))
	  ((record-type? (record-type obj))
	   (disclose-record obj))
	  (else (fail)))))
