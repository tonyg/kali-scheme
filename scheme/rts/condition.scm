; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file condition.scm.

;;;; Condition hierarchy

; General design copied from gnu emacs.

(define *condition-types* '())

(define (condition-supertypes type)
  (assq type *condition-types*))

(define (define-condition-type type supertypes)
  (set! *condition-types*
	(cons (cons type (apply append
				(map (lambda (sup)
				       (or (condition-supertypes sup)
					   (error "unrecognized condition type"
						  sup)))
				     supertypes)))
	      *condition-types*)))

(define (condition-predicate name)
  (lambda (c)
    (and (pair? c)
	 (let ((probe (condition-supertypes (car c))))
	   (if probe
	       (if (memq name probe) #t #f)
	       #f)))))

(define (condition? x)
  (and (pair? x)
       (list? x)
       (condition-supertypes (car x))))
(define condition-type car)
(define condition-stuff cdr)


; Errors

(define-condition-type 'error '())
(define error? (condition-predicate 'error))

(define-condition-type 'call-error '(error))
(define call-error? (condition-predicate 'call-error))

(define-condition-type 'read-error '(error))
(define read-error? (condition-predicate 'read-error))

; Exceptions

(define-condition-type 'exception '(error))
(define exception? (condition-predicate 'exception))
(define exception-opcode cadr)
(define exception-reason caddr)
(define exception-arguments cdddr)

(define (make-exception opcode reason args)
  (make-condition 'exception (cons opcode (cons reason args))))

; I/O errors

(define-condition-type 'i/o-error '(error))
(define i/o-error? (condition-predicate 'i/o-error))
(define i/o-error-status cadr)
(define i/o-error-message caddr)
(define i/o-error-operation cadddr)
(define i/o-error-arguments cddddr)

(define (make-i/o-error status operation args)
  (make-condition 'i/o-error
		  (cons status
			(cons (os-error-message status)
			      (cons operation args)))))

; Warnings

(define-condition-type 'warning '())
(define warning? (condition-predicate 'warning))

(define-condition-type 'syntax-error '(warning))
(define syntax-error? (condition-predicate 'syntax-error))


; Notes

(define-condition-type 'note '())
(define note? (condition-predicate 'note))

; Interrupts

(define-condition-type 'interrupt '())
(define interrupt? (condition-predicate 'interrupt))
