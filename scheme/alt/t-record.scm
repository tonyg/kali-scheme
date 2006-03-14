; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file t-record.scm.
; Synchronize any changes with the other *record.scm files.

;;;; Records

(define make-record-type
  (let ((make-stype        (*value t-standard-env 'make-stype))
	(crawl-exhibit     (*value t-standard-env 'crawl-exhibit))
	(exhibit-structure (*value t-standard-env 'exhibit-structure))
	(structure-type    (*value t-standard-env 'structure-type))
	(object-hash	   (*value t-standard-env 'object-hash))
	(print   	   (*value t-standard-env 'print))
	(format		   (*value t-standard-env 'format)))
    (lambda (id names)
      (letrec ((rtd
		(make-stype id names
		  (#[syntax object] #f
		     ((crawl-exhibit self)
		      (exhibit-structure self))
		     ((print self port)
		      (format port "#{Record~_~S~_~S}" id (object-hash self)))
		     ((structure-type self) rtd)))))
	rtd))))

(define record-predicate (*value t-standard-env 'stype-predicator))

(define record-accessor (*value t-standard-env 'stype-selector))

(define (record-modifier rtd name)
  (setter (record-accessor rtd name)))

(define (record-constructor rtd names)
  (let ((number-of-inits (length names))
	(modifiers (map (lambda (name) (record-modifier rtd name))
			names))
	(make ((*value t-implementation-env 'stype-constructor) rtd)))
    (lambda values
      (let ((record (make)))
	(let loop ((vals values)
		   (ups modifiers))
	  (cond ((null? vals)
		 (if (null? ups)
		     record
		     (error "too few arguments to record constructor"
			    values type-id names)))
		((null? ups)
		 (error "too many arguments to record constructor"
			values type-id names))
		(else
		 ((car ups) record (car vals))
		 (loop (cdr vals) (cdr ups)))))))))

(define (define-record-discloser rtd proc) 'unimplemented)
