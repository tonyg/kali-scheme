; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Same as jar-defrecord.scm, but field access is "optimized" in a
; brutally unmodular way.  The accessors and modifiers are easily
; recognized as inlinable because instead of being produced by the
; usual record-accessor and record-modifier combinators, they're
; defined directly as procedures that do record-ref and record-set!
; with constant indexes.  There is no check to make sure that the
; record is a record of the correct type.

(define-syntax define-record-type    ;same as in jar-defrecord.scm
  (syntax-rules ()
    ((define-record-type id type
       (constructor arg ...)
       (field . field-stuff)
       ...)
     (begin (define type (make-record-type 'id '(field ...)))
	    (define constructor (record-constructor type '(arg ...)))
	    (define-accessors type (field . field-stuff) ...)))
    ((define-record-type id type
       (constructor arg ...)
       pred
       more ...)
     (begin (define-record-type id type
	      (constructor arg ...)
	      more ...)
	    (define pred (record-predicate type))))))

(define-syntax define-accessors
  (lambda (e r c)
    (let ((%define-accessor (r 'define-accessor))
	  (%begin (r 'begin))
	  (type (cadr e))
	  (field-specs (cddr e)))
      (do ((i 1 (+ i 1))
	   (field-specs field-specs (cdr field-specs))
	   (ds '()
	       (cons `(,%define-accessor ,type ,i ,@(car field-specs))
		     ds)))
	  ((null? field-specs)
	   `(,%begin ,@ds))))))

(define-syntax define-accessor
  (syntax-rules ()
    ((define-accessor type index field accessor)
     (define (accessor r) (record-ref r index)))
    ((define-accessor type index field accessor modifier)
     (begin (define (accessor r) (record-ref r index))
	    (define (modifier r new) (record-set! r index new))))))
