; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; The DEFINE-PACKAGE and DEFINE-SIGNATURE macros.

; Signatures

(define-syntax define-signature
  (syntax-rules ()
    ((define-signature name sig)
     (define name
       (begin (verify-later! (lambda () name))
	      (a-signature sig))))))

(define-syntax a-signature
  (syntax-rules ()
    ((a-signature (a . pair))		;export
     (a . pair))
    ((a-signature sig)			;name
     (lambda () sig))))

(define-syntax compound-signature
  (syntax-rules ()
    ((compound-signature sig ...)
     (make-compound-signature (a-signature sig) ...))))

(define-syntax export
  (syntax-rules ()
    ((export item ...)
     (make-simple-signature '(item ...)))))


; Structures

(define-syntax define-structure
  (syntax-rules ()
    ((define-structure name exp)
     (define name (begin (verify-later! (lambda () name))
			 exp)))))


; Packages

(define-syntax define-package
  (syntax-rules (implement export)
    ((define-package ((name sig))    ;Redundant; work around bug in scheme->c
       (keyword stuff ...) ...)
     (define-structure name
       (make-structure 'name
		       (a-signature sig)
		       (a-package (keyword stuff ...) ...))))
    ((define-package ((name1 sig1) (name sig) ...)
       (keyword stuff ...) ...)
     (begin (define name (verify-later! (lambda () name)))
	    ...
	    (define-structure name1
	      (let ((m (a-package (keyword stuff ...) ...)))
		(set! name (make-structure 'name (a-signature sig) m))
		...
		(make-structure 'name1 (a-signature sig1) m)))))))

(define-syntax a-package
  (let ()

    (define (parse-package-description form rename compare return)
      (let ((%open (rename 'open))
	    (%access (rename 'access)))
	(let loop ((clauses (cdr form))
		   (opens '())
		   (accesses '())
		   (others '()))
	  (cond ((null? clauses)
		 (return opens accesses (reverse others)))
		((not (list? (car clauses)))
		 (syntax-error "ignoring invalid define-package clause"
			       (car clauses))
		 (loop (cdr clauses)
		       opens
		       accesses
		       others))
		(else
		 (let ((keyword (caar clauses)))
		   (cond ((compare keyword %open)
			  (loop (cdr clauses)
				(append opens (cdar clauses))
				accesses
				others))
			 ((compare keyword %access)
			  (loop (cdr clauses)
				opens
				(append (cdar clauses) accesses)
				others))
			 (else
			  (loop (cdr clauses)
				opens
				accesses
				(cons (car clauses) others))))))))))

    (lambda (form rename compare)
      (parse-package-description form rename compare
        (lambda (opens accesses others)
	  (let ((%make (rename 'make-a-package))
		(%lambda (rename 'lambda))
		(%cons (rename 'cons))
		(%list (rename 'list))
		(%quote (rename 'quote))
		(%file-name (rename '%file-name%)))
	    `(,%make (,%lambda () (,%list ,@opens))
		     (,%lambda ()
		       (,%list ,@(map (lambda (a) `(,%cons (,%quote ,a) ,a))
				      accesses)))
		     (,%file-name)
		     (,%quote ,others)))))))
  (cons lambda list make-a-package quote %file-name%))


; Modules  = package combinators

(define-syntax define-module
  (syntax-rules ()
    ((define-module (name . args) body ...)
     (define name
       (begin (verify-later! (lambda () name))
	      (lambda args body ...))))))

; Grumble.

(define-syntax structure-ref
  (syntax-rules ()
    ((structure-ref struct name)
     (*structure-ref struct 'name))))
