; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This file has to be loaded into the initial-image before any use of
; DEFINE-STRUCTURE.  Compare with alt/init-defpackage.scm.

; The procedure given to DEFINE-REFLECTIVE-TOWER-MAKER is called when
; a DEFINE-STRCTURE form is evaluated.

(define-reflective-tower-maker
  (let ((env (interaction-environment))
	(reflective-tower (*structure-ref syntactic 'reflective-tower)))
    (lambda (clauses id)
      (if (null? clauses)
	  ;; (make-reflective-tower eval (list scheme) id)
	  (reflective-tower (package->environment env))
	  (delay (cons eval (eval `(a-package ((for-syntax ,id)) ,@clauses)
				  env)))))))
