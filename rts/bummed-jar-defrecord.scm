; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Same as jar-defrecord.scm, but field access is "optimized" in a
; brutally unmodular way.  The accessors and modifiers are easily
; recognized as inlinable because instead of being produced by the
; usual record-accessor and record-modifier combinators, they're
; defined directly as procedures that do record-ref and record-set!
; with constant indexes.  There is no check to make sure that the
; record is a record of the correct type.

; Since the record types are not checked at run time, we use LOOPHOLE
; to at least try to get a little bit of compile-time checking.

(define-syntax define-record-type    ;same as in jar-defrecord.scm
  (syntax-rules ()
    ((define-record-type ?id ?type
       (?constructor ?arg ...)
       (?field . ?field-stuff)
       ...)
     (begin (define ?type (make-record-type '?id '(?field ...)))
	    (define-constructor ?constructor ?type (?arg :value) ...)
	    (define-accessors ?type (?field . ?field-stuff) ...)))
    ((define-record-type ?id ?type
       (?constructor ?arg ...)
       ?pred
       ?more ...)
     (begin (define-record-type ?id ?type
	      (?constructor ?arg ...)
	      ?more ...)
	    (define ?pred (record-predicate ?type))))))

(define-syntax define-constructor
  (syntax-rules ()
    ((define-constructor ?constructor ?type (?arg ?arg-type) ...)
     (define ?constructor
       (loophole (proc (?arg-type ...) ?type)
		 (record-constructor ?type '(?arg ...)))))))

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
		   (record-ref (loophole :record r) ?index)))))
    ((define-accessor ?type ?index ?accessor ?modifier)
     (begin (define-accessor ?type ?index ?accessor)
	    (define ?modifier
	      (loophole (proc (?type :value) :unspecific)
			(lambda (r new)
			  (record-set! (loophole :record r) ?index new))))))
    ((define-accessor ?type ?index)
     (begin))))
