; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This file has to be loaded into the initial-image before any use of
; DEFINE-STRUCTURE.  Compare with alt/init-defpackage.scm.

; The procedure given to DEFINE-REFLECTIVE-TOWER-MAKER is called when
; a DEFINE-STRUCTURE form is evaluated.

(define-reflective-tower-maker
  (let ((environment-macro-eval
	  (*structure-ref compiler-envs 'environment-macro-eval))
	(make-simple-interface
	  (*structure-ref interfaces 'make-simple-interface))
	(env (interaction-environment)))
    (lambda (clauses id)
      (if (null? clauses)
	  ;; (make-reflective-tower eval (list scheme) id)
	  (environment-macro-eval (package->environment env))
	  (delay
	    (let ((package (eval `(a-package ,(if id
						  `(for-syntax ,id)
						  '(for-syntax))
					     ,@clauses)
				 env)))
	      (ensure-loaded (make-structure package
					     (lambda ()
					       (make-simple-interface #f '()))
					     'for-syntax))
	      (cons eval package)))))))
