; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

;;;; Condition hierarchy

; General design copied from GNU Emacs

;; This is the low-level condition representation for use by the
;; run-time system; it uses lists whose first symbol represents the
;; condition type.

;; Systems built on top of this one may choose a different
;; representation.  If they do, they need to provide a condition
;; decoder for use by the RTS to do some basic processing---mainly
;; distinguishing errors from the rest.

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

;; alist of (predicate . decode)
;; Each decoder procedure should return a pair (type . stuff)
(define *condition-decoder-alist* '())

(define (decode-condition condition)
  (let loop ((alist *condition-decoder-alist*))
    (cond
     ((null? alist)
      (if (pair? condition)
	  (values (car condition) (cdr condition))
	  (values #f #f)))
     (((caar alist) condition)
      ((cdar alist) condition))
     (else
      (loop (cdr alist))))))

(define (define-condition-decoder predicate decoder)
  (set! *condition-decoder-alist*
	(cons (cons predicate decoder)
	      *condition-decoder-alist*)))

(define (condition-type condition)
  (call-with-values
      (lambda () (decode-condition condition))
    (lambda (type stuff)
      type)))

(define (condition-stuff condition)
  (call-with-values
      (lambda () (decode-condition condition))
    (lambda (type stuff)
      stuff)))

(define (condition-predicate name)
  (lambda (c)
    (let ((type (condition-type c)))
      (and type
	   (let ((probe (condition-supertypes type)))
	     (if probe
		 (if (memq name probe) #t #f)
		 #f))))))
; Errors

(define-condition-type 'error '())
(define error? (condition-predicate 'error))

(define-condition-type 'call-error '(error))
(define call-error? (condition-predicate 'call-error))

(define-condition-type 'read-error '(error))
(define read-error? (condition-predicate 'read-error))

; VM Exceptions

(define-condition-type 'vm-exception '(error))
(define vm-exception? (condition-predicate 'vm-exception))
(define (vm-exception-opcode c)
  (car (condition-stuff c)))
(define (vm-exception-reason c)
  (cadr (condition-stuff c)))
(define (vm-exception-arguments c)
  (cddr (condition-stuff c)))

(define (make-vm-exception opcode reason args)
  (make-condition 'vm-exception (cons opcode (cons reason args))))

; I/O errors

(define-condition-type 'i/o-error '(error))
(define i/o-error? (condition-predicate 'i/o-error))
(define (i/o-error-status c)
  (car (condition-stuff c)))
(define (i/o-error-message c)
  (cadr (condition-stuff c)))
(define (i/o-error-operation c)
  (caddr (condition-stuff c)))
(define (i/o-error-arguments c)
  (cdddr (condition-stuff c)))

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
