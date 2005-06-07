; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.

; Type schemes

(define-record-type type-scheme :type-scheme
  (make-type-scheme type free-uvars)
  type-scheme?
  (type type-scheme-type)		; a type
  (free-uvars type-scheme-free-uvars))	; uvars that are free

(define-record-discloser :type-scheme
  (lambda (type-scheme)
    (list 'type-scheme
	  (map uvar-id (type-scheme-free-uvars type-scheme))
	  (type-scheme-type type-scheme))))

; If TYPE has any variables bound at DEPTH this returns a type scheme making
; those variables polymorphic; otherwise TYPE is returned.

; Would like to do limited finalizing of uvars, but can't.
; Consider (lambda (g x) (tuple (g 3) (g x) x))
; (a -> b) -> c -> [d, e, f] with
; a > int8, d > b, a > c, e > b, f > c
; No polymorphism, and no simplification without restricting someone
; But consider NOT  a ->b, bool > a, b > bool
; It could just as well be bool -> bool.
; Simplification okay on variables that are not used inside other types?

(define *free-uvars* '())

(define (schemify-type type depth)
  (set! *free-uvars* '())
  (let* ((type (find-free-uvars type depth))
	 (free-uvars *free-uvars*))
    (set! *free-uvars* '()) ; drop pointers
    (for-each (lambda (uvar)
		(set-uvar-place! uvar #f))
	      free-uvars)
    (if (not (null? free-uvars))
	(make-type-scheme type free-uvars)
	type)))
  
(define (find-free-uvars type depth)
  (let label ((type type))
    (cond ((other-type? type)
	   (make-other-type (other-type-kind type)
			    (map label
				 (other-type-subtypes type))))
	  ((not (uvar? type))
	   type)
	  ((uvar-binding type)
	   => label)
	  ((and (not (uvar-place type))
		(<= depth (uvar-depth type)))
	   (set-uvar-place! type type)
	   (set! *free-uvars* (cons type *free-uvars*))
	   type)
	  (else
	   type))))

; Instantiate SCHEME at DEPTH.
;
; New sequence:
;   (instantiate-type-scheme scheme depth)
;   ... elide bindings in new copy ...
;   (clean-type-scheme scheme)

(define (instantiate-type-scheme scheme depth . maybe-thunk)
  (instantiate-type-scheme! scheme depth)
  (let ((type (copy-type (type-scheme-type scheme))))
    (if (not (null? maybe-thunk))
	((car maybe-thunk)))
    (clean-type-scheme! scheme)
    type))

(define (instantiate-type-scheme! scheme depth)
  (let ((uid (unique-id)))
    (for-each (lambda (uvar)
		(set-uvar-place!
		 uvar
		 (make-uvar (uvar-prefix uvar) depth uid)))
	      (type-scheme-free-uvars scheme))))

(define (clean-type-scheme! scheme)
  (for-each (lambda (uvar)
	      (set-uvar-place! uvar #f))
	    (type-scheme-free-uvars scheme)))

(define (copy-type type)
  (cond ((other-type? type)
	 (make-other-type (other-type-kind type)
			  (map copy-type
			       (other-type-subtypes type))))
	((not (uvar? type))
	 type)
	((uvar-place type)
	 => identity)
	((uvar-binding type)
	 => copy-type)
	(else
	 type)))


