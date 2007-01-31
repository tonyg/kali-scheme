; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This file should be loaded into the bootstrap linker before any use
; of DEFINE-STRUCTURE.  Compare with env/init-defpackage.scm.

(define (evaluate-transformer exp env)
  (if (and (pair? exp)
	   (eq? (car exp) 'syntax-rules))
      
      (if (pair? (cdr exp))
	  (let ((subkeywords (cadr exp))
		(rules (cddr exp)))
	    (if (and (list? subkeywords)
		     (every name? subkeywords))
		;; Pair of the procedure and list of auxiliary names
		(cons
		  (eval `(let-syntax ((code-quote
				       (syntax-rules ()
					 ((code-quote ?thing) '?thing))))
			   ,(process-rules rules subkeywords (lambda (x) x) eq?))
			env)
		  (find-free-names-in-syntax-rules subkeywords rules))
		exp))
	  exp)      
      (eval exp env)))

(define-reflective-tower-maker
  (lambda (clauses names)
    (let ((env (interaction-environment)))
      (delay
	(begin (if (not (null? clauses))
		   (warn "a FOR-SYNTAX clause appears in a package being linked by the cross-linker"
			 `(for-syntax ,@clauses)))
	       (cons evaluate-transformer env))))))

(define-syntax code-quote
  (syntax-rules ()
    ((code-quote ?thing) '?thing)))
