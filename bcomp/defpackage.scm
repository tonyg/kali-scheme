; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; The DEFINE-PACKAGE and DEFINE-SIGNATURE macros.

(define-syntax define-modular
  (syntax-rules ()
    ((define-modular (?name) ?exp)
     ;; Redundant work around for bug in Scheme->C
     (define ?name (begin (verify-later! (lambda () ?name))
			  ?exp)))
    ((define-modular (?name ...) ?exp)
     (begin (verify-later! (lambda () ?name))
	    ...
	    (define-values (?name ...) ?exp)))))

(define-syntax define-values
  (syntax-rules ()
    ((define-values (?name ...) ?exp)
     (begin (define ?name)
	    ...
	    (let ((frob (lambda things
			  (begin (set! ?name (car things))
				 (set! things (cdr things)))
			  ...)))
	      (call-with-values (lambda () ?exp)
		(lambda (?name ...)
		  (frob ?name ...))))))))


; Signatures

; <definition> ::= (define-signature <name> <sig>)
; <sig> ::= <name> | (export <item> ...) | (compound-signature <sig> ...)

(define-syntax define-signature
  (syntax-rules ()
    ((define-signature ?name ?sig)
     (define ?name
       (begin (verify-later! (lambda () ?name))
	      (a-signature ?name ?sig))))))

(define-syntax a-signature
  (syntax-rules (export compound-signature)
    ((a-signature ?name (export ?item ...))
     (really-export '?name ?item ...))
    ((a-signature ?name (compound-signature ?sig ...))
     (make-compound-signature '?name (a-signature #f ?sig) ...))
    ((a-signature ?name ?sig)	;name
     ?sig)))


; <item> ::= <name> | (<name> <type>) | ((<name> ...) <type>)

(define-syntax really-export
  (lambda (e r c)
    (let ((name (cadr e))
	  (items (cddr e)))
      (let loop ((items items)
		 (plain '())
		 (others '()))
	(if (null? items)
	    `(,(r 'make-simple-signature)
	      ,(cadr e)
	      (,(r 'list) (,(r 'quote) ,(list (reverse plain)
					      'undeclared))
			  ,@(reverse others)))
	    (let ((item (car items)))
	      (if (pair? item)
		  (loop (cdr items)
			plain
			(cons `(,(r 'list) (,(r 'quote) ,(car item))
					   ,(cadr item))
			      others))
		  (loop (cdr items)
			(cons item plain)
			others)))))))
  (make-simple-signature list quote value))

		    


; Structures

(define-syntax define-structure
  (syntax-rules ()
    ((define-structure ?name ?exp)
     (define-modular (?name) ?exp))))


; Packages

(define-syntax define-package
  (syntax-rules (implement export)
    ((define-package ((?name ?sig) ...)
       (?keyword ?stuff ...) ...)
     (define-modular (?name ...)
       (let ((p (a-package (?name ...) (?keyword ?stuff ...) ...)))
	 (values (make-structure p (lambda () (a-signature #f ?sig)) '?name)
		 ...))))))

(define-syntax a-package
  (let ()

    (define (parse-package-clauses clauses rename compare)
      (let ((%open (rename 'open))
	    (%access (rename 'access))
	    (%for-syntax (rename 'for-syntax)))
	(let loop ((clauses clauses)
		   (opens '())
		   (accesses '())
		   (for-syntaxes '())
		   (others '()))
	  (cond ((null? clauses)
		 (values opens accesses for-syntaxes (reverse others)))
		((not (list? (car clauses)))
		 (display "Ignoring invalid define-package clause")
		 (newline)
		 (write (car clauses)) (newline)
		 (loop (cdr clauses)
		       opens
		       accesses
		       for-syntaxes
		       others))
		(else
		 (let ((keyword (caar clauses)))
		   (cond ((compare keyword %open)
			  (loop (cdr clauses)
				(append opens (cdar clauses))
				accesses
				for-syntaxes
				others))
			 ((compare keyword %access)
			  (loop (cdr clauses)
				opens
				(append (cdar clauses) accesses)
				for-syntaxes
				others))
			 ((compare keyword %for-syntax)
			  (loop (cdr clauses)
				opens
				accesses
				(append (cdar clauses) for-syntaxes)
				others))
			 (else
			  (loop (cdr clauses)
				opens
				accesses
				for-syntaxes
				(cons (car clauses) others))))))))))

    (lambda (form rename compare)
      (let ((names (cadr form))
	    (clauses (cddr form)))
	(call-with-values (lambda ()
			    (parse-package-clauses clauses rename compare))
	  (lambda (opens accesses for-syntaxes others)
	    (let ((%make (rename 'make-a-package))
		  (%lambda (rename 'lambda))
		  (%cons (rename 'cons))
		  (%list (rename 'list))
		  (%quote (rename 'quote))
		  (%a-package (rename 'a-package))
		  (%file-name (rename '%file-name%)))
	      `(,%make (,%lambda () (,%list ,@opens))
		       (,%lambda ()
			   (,%list ,@(map (lambda (a)
					    `(,%cons (,%quote ,a) ,a))
					  accesses)))
		       ,(if (null? for-syntaxes)
			    `#f
			    `(,%lambda ()
				 (,%a-package ((for-syntax ,@names))
					      ,@for-syntaxes)))
		       (,%file-name)
		       (,%quote ,others)
		       (,%quote ,(cadr form)))))))))
  (cons lambda list make-a-package quote %file-name%))


; Modules  = package combinators

(define-syntax define-module
  (syntax-rules ()
    ((define-module (?name . ?args) ?body ...)
     (define ?name
       (begin (verify-later! (lambda () ?name))
	      (lambda ?args ?body ...))))))
