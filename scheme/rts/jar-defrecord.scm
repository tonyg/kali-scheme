; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This knows about the implementation of records and creates the various
; accessors, mutators, etc. directly instead of calling the procedures
; from the record structure.  This is done to allow the optional auto-inlining
; optimizer to inline the accessors, mutators, etc.

; LOOPHOLE is used to get a little compile-time type checking (in addition to
; the usual complete run-time checking).

(define-syntax define-record-type
  (syntax-rules ()
    ((define-record-type ?id ?type
       (?constructor ?arg ...)
       (?field . ?field-stuff)
       ...)
     (begin (define ?type (make-record-type '?id '(?field ...)))
	    (define-constructor ?constructor ?type
	      ((?arg :value) ...)
	      (?field ...))
	    (define-accessors ?type () (?field . ?field-stuff) ...)))
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

(define-syntax define-synchronized-record-type
  (syntax-rules ()
    ((define-synchronized-record-type ?id ?type
       (?constructor ?arg ...)
       ?pred
       (?field . ?field-stuff)
       ...)
     (define-synchronized-record-type ?id ?type
       (?constructor ?arg ...)
       (?field ...)
       ?pred
       (?field . ?field-stuff)
       ...))
    ((define-synchronized-record-type ?id ?type
       (?constructor ?arg ...)
       (?sync-field ...)
       ?pred
       (?field . ?field-stuff)
       ...)
     (begin (define ?type (make-record-type '?id '(?field ...)))
	    (define-constructor ?constructor ?type
	      ((?arg :value) ...)
	      (?field ...))
	    (define ?pred
	      (lambda (x)
		(and (record? x)
		     (eq? ?type (record-ref x 0)))))
	    (define-accessors ?type (?sync-field ...)
	      (?field . ?field-stuff) ...)))))

; (define-constructor <id> <type> ((<arg> <arg-type>)*) (<field-name>*))
;
; Checks to see that there is an <arg> corresponding to every <field-name>.

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
	  (args (map r (map car (cadddr e))))
	  (arg-types (map cadr (cadddr e)))
	  (fields (map r (caddr (cddr e)))))
      (define (mem? name list)
	(cond ((null? list)        #f)
	      ((c name (car list)) #t)
	      (else
	       (mem? name (cdr list)))))
      (define (every? pred list)
	(cond ((null? list)        #t)
	      ((pred (car list))
	       (every? pred (cdr list)))
	      (else #f)))
      (if (every? (lambda (arg)
		    (mem? arg fields))
		  args)
	  `(define ,name
	     (,%loophole (,%proc ,arg-types ,type)
			 (,%lambda ,args
			     (,%record ,type . ,(map (lambda (field)
						       (if (mem? field args)
							   field
							   (list %unspecific)))
						     fields)))))
	  e)))
  (record begin lambda loophole proc unspecific))

(define-syntax define-accessors
  (lambda (e r c)
    (let ((%define-accessor (r 'define-accessor))
	  (%begin (r 'begin))
	  (type (cadr e))
	  (sync-fields (caddr e))
	  (field-specs (cdddr e)))
      (define (mem? name list)
	(cond ((null? list)        #f)
	      ((c name (car list)) #t)
	      (else
	       (mem? name (cdr list)))))
      (do ((i 1 (+ i 1))
	   (field-specs field-specs (cdr field-specs))
	   (ds '()
	       (cons `(,%define-accessor
		       ,(mem? (caar field-specs)
			      sync-fields)
		       ,type ,i ,@(cdar field-specs))
		     ds)))
	  ((null? field-specs)
	   `(,%begin ,@ds)))))
  (define-accessor begin))

(define-syntax define-accessor
  (syntax-rules ()
    ((define-accessor ?sync? ?type ?index ?accessor)
     (define ?accessor
       (loophole (proc (?type) :value)
		 (lambda (r)
		   ((ref-proc ?sync?) (loophole :record r) ?type ?index)))))
    ((define-accessor ?sync? ?type ?index ?accessor ?modifier)
     (begin (define-accessor ?sync? ?type ?index ?accessor)
	    (define ?modifier
	      (loophole (proc (?type :value) :unspecific)
			(lambda (r new)
			  ((set-proc ?sync?)
			    (loophole :record r) ?type ?index new))))))
    ((define-accessor ?sync? ?type ?index)
     (begin))))

(define-syntax ref-proc
  (syntax-rules ()
    ((ref-proc #t)
     provisional-checked-record-ref)
    ((ref-proc #f)
     checked-record-ref)))

(define-syntax set-proc
  (syntax-rules ()
    ((set-proc #t)
     provisional-checked-record-set!)
    ((set-proc #f)
     checked-record-set!)))

