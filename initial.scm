; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Link scripts.

(load-configuration "interfaces.scm")

(define-package ((module-system (export ))) ) ;Necessary for flatload.
(load-configuration "packages.scm")

(flatload initial-structures)


(define (link-initial-system)
  (let ((structures-to-open		;Structures to open for the initial
	 (struct-list scheme		;system's read-eval-print loop.
		      environments
		      defpackage	; contains macros...
		      types
		      ensures-loaded
		      packages
		      packages-internal)))  ; package-for-syntax
    (link-reified-system (append (desirable-structures)
				 structures-to-open)
			 'initial
			 `(start ',(map car structures-to-open))
			 ;; Structures to open for evaluating the expression
			 ;; that evaluates to the startup procedure
			 initial-system
			 for-reification
			 ;; scheme-level-1
			 )))


(define (desirable-structures)
  (let ((env (interaction-environment))
	(l '()))
    (for-each (lambda (sig)
		(for-each-declaration
		     (lambda (name type)
		       (set! l
			     (cons (cons name
					 (eval name env))
				   l)))
		     sig))
	      (list run-time-structures-interface
		    features-structures-interface
		    run-time-internals-interface
		    compiler-structures-interface
		    initial-structures-interface))
    (reverse l)))
