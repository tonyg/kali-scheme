; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; This is file record.scm.

;;;; Records

; Every record in the image is assumed to be made either by
; make-record-type or by a procedure returned by record-constructor.

(define (record-type r)
  (record-ref r 0))

(define *record-type-uid* -1)
(define *record-type* #f)

(define (make-record-type name field-names)
  (set! *record-type-uid* (+ *record-type-uid* 1))
  (let ((r (make-record 5 (unspecific))))
    (record-set! r 0 *record-type*)
    (record-set! r 1 *record-type-uid*)
    (record-set! r 2 name)
    (record-set! r 3 field-names)
    (record-set! r 4 default-record-discloser)
    r))

(define (record-type? obj)
  (and (record? obj)
       (eq? (record-type obj) *record-type*)))

(define (record-type-uid rt)         (record-ref rt 1))
(define (record-type-name rt)        (record-ref rt 2))
(define (record-type-field-names rt) (record-ref rt 3))
(define (record-type-discloser rt)   (record-ref rt 4))

(define (record-field-index rt name)
  (let loop ((names (record-type-field-names rt))
	     (i 1))
    (cond ((null? names) (error "unknown field"
				(record-type-name rt)
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
  (if (and (record-type? rt)
	   (procedure? proc))
      (record-set! rt 4 proc)
      (call-error "invalid argument" define-record-discloser rt proc)))

(define (disclose-record r)
  (if (record? r)
      (let ((rt (record-type r)))
	(if (record-type? rt)
	    (or ((record-type-discloser rt) r)
		(list (record-type-name rt)))
	    #f))			;Not one of ours.
      #f))

(define default-record-discloser
  (lambda (r) #f))

; Patch

(set! *record-type*
      (make-record-type 'record-type '(uid name field-names discloser)))
(record-set! *record-type* 0 *record-type*)

(define :record-type *record-type*)

(define-record-discloser :record-type
  (lambda (rt)
    (list 'record-type
	  (record-type-uid rt)
	  (record-type-name rt))))
