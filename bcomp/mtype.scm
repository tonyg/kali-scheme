; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; A little type system.  See doc/type.txt.


; Top elements:

(define syntax-type ':syntax)

(define any-values-type ':values)


; Multiple values

(define (make-some-values-type types)
  (for-each check-value-type types)
  (if (or (null? types)
	  (not (null? (cdr types))))
      `(some-values ,@types)
      (car types)))

(define (some-values-type? type)
  (and (pair? type) (eq? (car type) 'some-values)))

(define (some-values-type-components type)
  (if (some-values-type? type)
      (cdr type)
      (list type)))


; Individual values

(define value-type ':value)

(define (check-value-type type)
  (if (not (compatible-types? type value-type))
      (warn "improper value type" type)))

; Bottom value type, sort of:

(define undeclared-type ':undeclared)


; Procedure types

(define (procedure-type domain codomain)
  (if (not (compatible-types? domain any-values-type))
      (warn "improper domain type" domain))
  (if (not (compatible-types? codomain any-values-type))
      (warn "improper codomain type" codomain))
  (cond ((some-values-type? domain)
	 (let ((domain-list (some-values-type-components domain)))
	   (if (and (eq? codomain any-values-type)
		    (every (lambda (type) (eq? type value-type))
			   domain-list))
	       (length domain-list)	;+++ space saver
	       `(proc ,domain-list ,codomain))))
	((eq? domain any-values-type)
	 `(procedure ,domain ,codomain)) ;Arity unknown.
	((and (eq? domain value-type)
	      (eq? codomain any-values-type))
	 1)
	(else
	 `(proc (,domain) ,codomain))))

(define (procedure-type? type)
  (or (and (pair? type) (memq (car type) '(proc procedure)))
      (integer? type)))

(define (fixed-arity-procedure-type? type)
  (or (and (pair? type) (eq? (car type) 'proc))
      (integer? type)))

(define (procedure-type-codomain type)
  (if (pair? type)
      (caddr type)
      any-values-type))

(define (procedure-type-domain type)
  (if (pair? type)
      (if (eq? (car type) 'procedure)
	  (cadr type)
	  (make-some-values-type (cadr type)))
      (make-some-values-type (nargs->domain-list type))))

(define (nargs->domain-list n)
  (if (= n 0)
      '()
      (cons value-type (nargs->domain-list (- n 1)))))

(define (procedure-type-argument-types type)
  (if (pair? type)
      (if (eq? (car type) 'proc)
	  (cadr type)
	  (error "can't compute argument types" type))
      (nargs->domain-list type)))

(define (procedure-type-arity type)
  (if (pair? type)
      (length (cadr type))
      type))



; Variable types

(define (variable-type type)
  (check-value-type type)
  (list 'variable type))

(define (variable-type? type)
  (and (pair? type) (eq? (car type) 'variable)))
(define variable-value-type cadr)


; Type comparison: will an X do where I need a Y?
; If not, we return #f.
; If so, then
;  - if there exists a type Z such that Z <= X and Z <= Y, return 'maybe;
;  - otherwise, return 'definitely.

(define (compatible-types? have-type want-type)
  (cond ((equal? have-type want-type) 'definitely)

	((eq? want-type syntax-type) #f)
	((eq? have-type syntax-type) #f)

	((eq? want-type any-values-type) 'definitely)
	((eq? have-type any-values-type) 'maybe)

	((some-values-type? want-type)
	 (and (some-values-type? have-type)
	      (compatible-type-lists?
	           (some-values-type-components have-type)
		   (some-values-type-components want-type))))
	((some-values-type? have-type) #f)

	((variable-type? want-type) #f)
	((variable-type? have-type)
	 (compatible-types? (variable-value-type have-type) want-type))

	((eq? want-type value-type) 'definitely)
	((eq? have-type value-type) 'maybe)

	((procedure-type? want-type)
	 (and (procedure-type? have-type)
	      (let ((c1 (compatible-types? (procedure-type-domain want-type)
					   (procedure-type-domain have-type)))
		    (c2 (compatible-types? (procedure-type-codomain have-type)
					(procedure-type-codomain want-type))))
		(if (eq? c1 'definitely)
		    c2
		    (if c2 c1 #f)))))
	(else #f)))

(define (compatible-type-lists? l1 l2)
  (if (null? l1)
      (if (null? l2) 'definitely #f)
      (and (not (null? l2))
	   (let ((c1 (compatible-types? (car l1) (car l2))))
	     (if (eq? c1 'definitely)
		 (compatible-type-lists? (cdr l1) (cdr l2))
		 (let ((c2 (compatible-type-lists? (cdr l1) (cdr l2))))
		   ;; Only maybe.
		   (if c2 c1 #f)))))))


; Usual type for Scheme variables.

(define usual-variable-type (variable-type value-type))


; Some of the base types

(define boolean-type ':boolean)
(define char-type ':char)
(define number-type ':number)
(define null-type ':null)
(define unspecific-type ':unspecific)

(define pair-type ':pair)
(define string-type ':string)
(define symbol-type ':symbol)
(define vector-type ':vector)

(define zero-type ':zero)       ;Hmm.
(define escape-type ':escape)   ;Hmm.  See whether any errors crop up.


; --------------------
; Handy things:

(define (some-values-type . types)	;convenient
  (make-some-values-type types))

(define any-procedure-type
  (procedure-type any-values-type any-values-type))

(define-syntax proc
  (syntax-rules ()
    ((proc (?type ...) ?cod)
     (procedure-type (some-values-type ?type ...) ?cod))))
