; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; The DEFINE-INTERFACE and DEFINE-STRUCTURE macros.

(define-syntax def
  (syntax-rules ()
    ;; Redundant work around for bug in Scheme->C
    ((def (?name) ?exp) (def ?name ?exp))
    ((def (?name ...) ?exp)
     (begin (verify-later! (lambda () ?name))
	    ...
	    (define-values (?name ...) ?exp)))
    ((def ?name ?exp)
     (define ?name (begin (verify-later! (lambda () ?name))
			  ?exp)))))

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


; Interfaces

; <definition> ::= (define-interface <name> <int>)
; <int> ::= <name> | (export <item> ...) | (compound-interface <int> ...)

(define-syntax define-interface
  (syntax-rules ()
    ((define-interface ?name ?int)
     (define ?name
       (begin (verify-later! (lambda () ?name))
	      (an-interface ?name ?int))))))

(define-syntax export
  (syntax-rules ()
    ((export ?item ...)
     (really-export #f ?item ...))))

(define-syntax compound-interface
  (syntax-rules ()
    ((compound-interface ?int ...)
     (make-compound-interface #f ?int ...))))

(define-syntax an-interface
  (syntax-rules (export compound-interface)
    ((an-interface ?name (export ?item ...))
     (really-export '?name ?item ...))
    ((an-interface ?name (compound-interface ?int ...))
     (make-compound-interface '?name (an-interface #f ?int) ...))
    ((an-interface ?name ?int)	;name
     ?int)))


; <item> ::= <name> | (<name> <type>) | ((<name> ...) <type>)

(define-syntax really-export
  (lambda (e r c)
    (let ((name (cadr e))
	  (items (cddr e)))
      (let loop ((items items)
		 (plain '())
		 (others '()))
	(if (null? items)
	    `(,(r 'make-simple-interface)
	      ,(cadr e)
	      (,(r 'list) (,(r 'quote) ,(list (reverse plain)
					      ':undeclared))
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
  (make-simple-interface list quote value))

		    


; Structures

(define-syntax define-structure
  (syntax-rules ()
    ((define-structure ?name ?int ?clause1 ?clause ...)
     (define-structures ((?name ?int)) ?clause1 ?clause ...))
    ;; For compatibility (?)
    ((define-structure ?name ?exp)
     (def ?name ?exp))))


; Packages

(define-syntax define-structures
  (syntax-rules (implement export)
    ((define-structures ((?name ?int) ...)
       (?keyword ?stuff ...) ...)
     (def (?name ...)
       (let ((p (a-package (?name ...) (?keyword ?stuff ...) ...)))
	 (values (make-structure p (lambda () (an-interface #f ?int)) '?name)
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
		 (display "Ignoring invalid define-structures clause")
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
		       (,(string->symbol ".make-reflective-tower.")
			(,%quote ,for-syntaxes)
			(,%quote ,names))
		       (,%file-name)
		       (,%quote ,others)
		       (,%quote ,(cadr form)))))))))
  (cons lambda list make-a-package quote %file-name%))



; (DEFINE-REFLECTIVE-TOWER-MAKER <proc>)
;   <proc> should be an expression that evaluates to a procedure of
;   two arguments.  The first argument is a list of DEFINE-STRUCTURE
;   clauses, and the second is some identifying information (no
;   semantic content).  The procedure should return a "reflective
;   tower", which is a pair (<eval-proc> . <env>).  To evaluate the
;   right-hand side of a DEFINE-SYNTAX (LET-SYNTAX, etc.) form,
;   <eval-proc> is called on the right-hand side and <env>.
; Got that?

(define-syntax define-reflective-tower-maker
  (lambda (e r c)
    `(,(r 'define) ,(string->symbol ".make-reflective-tower.") ,(cadr e)))
  (define))

(define-syntax export-reflective-tower-maker
  (lambda (e r c)
    `(,(r 'export) ,(string->symbol ".make-reflective-tower.")))
  (export))


; Modules  = package combinators

(define-syntax define-module
  (syntax-rules ()
    ((define-module (?name . ?args) ?body ...)
     (def ?name (lambda ?args ?body ...)))))
