; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This file has to be loaded into the initial-image before any use of
; DEFINE-STRUCTURE.  Compare with alt/init-defpackage.scm.

; The procedure given to DEFINE-REFLECTIVE-TOWER-MAKER is called when
; a DEFINE-STRUCTURE form is evaluated.

(define-reflective-tower-maker
  (let ((reflective-tower (*structure-ref syntactic 'reflective-tower))
	(make-simple-interface (*structure-ref interfaces 'make-simple-interface))
	(env (interaction-environment)))
    (lambda (clauses id)
      (if (null? clauses)
	  ;; (make-reflective-tower eval (list scheme) id)
	  (reflective-tower (package->environment env))
	  (delay (let ((p (eval `(a-package ((for-syntax ,id)) ,@clauses)
				env)))
		   (ensure-loaded (make-structure p
						  (lambda () (make-simple-interface #f '()))
						  'for-syntax))
		   (cons eval p)))))))
