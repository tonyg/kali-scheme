; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The DEFINE-INTERFACE and DEFINE-STRUCTURE macros.

(define-syntax def
  (syntax-rules ()
    ((def (?name . ?args) ?body ...)
     (really-def () ?name (lambda ?args ?body ...)))
    ((def ?name ...)
     (really-def () ?name ...))))

(define-syntax really-def
  (syntax-rules ()
    ((really-def (?name ...) ?exp)
     (define-multiple (?name ...)
       (begin (verify-later! (lambda () ?name))
	      ...
	      ?exp)))
    ((really-def (?name ...) ?name1 ?etc ...)
     (really-def (?name ... ?name1) ?etc ...))))

(define-syntax define-multiple
  (syntax-rules ()
    ((define-multiple (?name) ?exp)
     (define ?name (note-name! ?exp '?name)))
    ((define-multiple (?name ...) ?exp)
     (begin (define ?name #f)
	    ...
	    (let ((frob (lambda things
			  (begin (set! ?name
				       (note-name! (car things) '?name))
				 (set! things (cdr things)))
			  ...)))
	      (call-with-values (lambda () ?exp) frob))))))


; Interfaces

; <definition> ::= (define-interface <name> <int>)
; <int> ::= <name> | (export <item> ...) | (compound-interface <int> ...)

(define-syntax define-interface
  (syntax-rules ()
    ((define-interface ?name ?int)
     (def ?name ?int))))

(define-syntax compound-interface
  (syntax-rules ()
    ((compound-interface ?int ...)
     (make-compound-interface #f ?int ...))))

; <item> ::= <name> | (<name> <type>) | ((<name> ...) <type>)

(define-syntax export
  (lambda (e r c)
    (let ((items (cdr e)))
      (let loop ((items items)
		 (plain '())
		 (others '()))
	(if (null? items)
	    `(,(r 'make-simple-interface)
	      #f
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
     (def ?name (structure ?int ?clause1 ?clause ...)))
    ;; For compatibility.  Use DEF instead.
    ((define-structure ?name ?exp)
     (def ?name ?exp))))

(define-syntax define-structures
  (syntax-rules ()
    ((define-structures ((?name ?int) ...)
       ?clause ...)
     (def ?name ... (structures (?int ...) ?clause ...)))))

(define-syntax structure
  (syntax-rules ()
    ((structure ?int ?clause ...)
     (structures (?int) ?clause ...))))

(define-syntax structures
  (syntax-rules ()
    ((structures (?int ...) ?clause ...)
     (let ((p (a-package #f ?clause ...)))
       (values (make-structure p (lambda () ?int))
	       ...)))))

(define-syntax modify
  (syntax-rules ()
    ((modify ?struct ?command ...)
     (make-modified-structure ?struct '(?command ...)))))

; Two handy shorthands for MODIFY.

(define-syntax subset
  (syntax-rules ()
    ((restrict struct (name ...))
     (modify struct (expose name ...)))))

(define-syntax with-prefix
  (syntax-rules ()
    ((with-prefix struct the-prefix)
     (modify struct (prefix the-prefix)))))

; Packages

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
			(,%quote ,names))		; for discloser
		       (,%file-name)
		       (,%quote ,others)
		       (,%quote ,names))))))))
  (cons lambda list make-a-package quote make-reflective-tower %file-name%))

(define-syntax receive
  (syntax-rules ()
    ((receive (?var ...) ?producer . ?body)
     (call-with-values (lambda () ?producer)
       (lambda (?var ...)
	 (note-name! ?var '?var) ...
	 (let () . ?body))))))


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

; Modules  = package combinators...

(define-syntax define-module
  (syntax-rules ()
    ((define-module (?name . ?args) ?body ...)
     (def ?name (lambda ?args ?body ...)))))
