; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


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
	    (define-constructor ?constructor ?id ?type (?arg value) ...)
	    (define-accessors ?type ?id (?field . ?field-stuff) ...)))
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
    ((define-constructor ?constructor ?id ?type (?arg ?arg-type) ...)
     (define ?constructor
       (loophole (proc (?arg-type ...) ?id)
		 (record-constructor ?type '(?arg ...)))))))

(define-syntax define-accessors
  (lambda (e r c)
    (let ((%define-accessor (r 'define-accessor))
	  (%begin (r 'begin))
	  (type (cadr e))
	  (id (caddr e))
	  (field-specs (cdddr e)))
      (do ((i 1 (+ i 1))
	   (field-specs field-specs (cdr field-specs))
	   (ds '()
	       (cons `(,%define-accessor ,id ,i ,@(cdar field-specs))
		     ds)))
	  ((null? field-specs)
	   `(,%begin ,@ds)))))
  (define-accessor begin))

(define-syntax define-accessor
  (syntax-rules ()
    ((define-accessor ?id ?index ?accessor)
     (define ?accessor
       (loophole (proc (?id) value)
		 (lambda (r) (record-ref r ?index)))))
    ((define-accessor ?id ?index ?accessor ?modifier)
     (begin (define-accessor ?id ?index ?accessor)
	    (define ?modifier
	      (loophole (proc (?id value) unspecific)
			(lambda (r new) (record-set! r ?index new))))))))
