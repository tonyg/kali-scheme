; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This knows about the implementation of records and creates the various
; accessors, mutators, etc. directly instead of calling the procedures
; from the record structure.  This is done to allow the optional auto-inlining
; optimizer to inline the accessors, mutators, etc.

; LOOPHOLE is used to get a little compile-time type checking (in addition to
; the usual complete run-time checking).

(define-syntax define-record-type    ;same as in jar-defrecord.scm
  (syntax-rules ()
    ((define-record-type ?id ?type
       (?constructor ?arg ...)
       (?field . ?field-stuff)
       ...)
     (begin (define ?type (make-record-type '?id '(?field ...)))
	    (define-constructor ?constructor ?type
	      ((?arg :value) ...)
	      (?field ...))
	    (define-accessors ?type (?field . ?field-stuff) ...)))
    ((define-record-type ?id ?type
       (?constructor ?arg ...)
       ?pred
       ?more ...)
     (begin (define-record-type ?id ?type
	      (?constructor ?arg ...)
	      ?more ...)
	    (define ?pred
	      (lambda (x)
		(and (record? x)
		     (eq? ?type (record-ref x 0)))))))))

; (define-constructor <id> <type> ((<arg> <arg-type>)*) (<field-name>*))

(define-syntax define-constructor
  (lambda (e r c)
    (let ((%record (r 'record))
	  (%begin (r 'begin))
	  (%lambda (r 'lambda))
	  (%loophole (r 'loophole))
	  (%proc (r 'proc))
	  (%unspecific (r 'unspecific))
	  (name (cadr e))
	  (type (caddr e))
	  (args (map car (cadddr e)))
	  (arg-types (map cadr (cadddr e)))
	  (fields (caddr (cddr e))))
      (define (mem? name list)
	(cond ((null? list)        #f)
	      ((c name (car list)) #t)
	      (else
	       (mem? name (cdr list)))))
      `(define ,name
	 (,%loophole (,%proc ,arg-types ,type)
		     (,%lambda ,args
			       (,%record ,type . ,(map (lambda (field)
							 (if (mem? field args)
							     field
							     (list %unspecific)))
						       fields)))))))
  (record begin lambda loophole proc unspecific))

(define-syntax define-accessors
  (lambda (e r c)
    (let ((%define-accessor (r 'define-accessor))
	  (%begin (r 'begin))
	  (type (cadr e))
	  (field-specs (cddr e)))
      (do ((i 1 (+ i 1))
	   (field-specs field-specs (cdr field-specs))
	   (ds '()
	       (cons `(,%define-accessor ,type ,i ,@(cdar field-specs))
		     ds)))
	  ((null? field-specs)
	   `(,%begin ,@ds)))))
  (define-accessor begin))

(define-syntax define-accessor
  (syntax-rules ()
    ((define-accessor ?type ?index ?accessor)
     (define ?accessor
       (loophole (proc (?type) :value)
		 (lambda (r)
		   (checked-record-ref (loophole :record r) ?type ?index)))))
    ((define-accessor ?type ?index ?accessor ?modifier)
     (begin (define-accessor ?type ?index ?accessor)
	    (define ?modifier
	      (loophole (proc (?type :value) :unspecific)
			(lambda (r new)
			  (checked-record-set! (loophole :record r) ?type ?index new))))))
    ((define-accessor ?type ?index)
     (begin))))
