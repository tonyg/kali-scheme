; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This file should be loaded into the bootstrap linker before any use
; of DEFINE-STRUCTURE.  Compare with env/init-defpackage.scm.

(define-reflective-tower-maker
  (lambda (clauses names)
    (let ((env (interaction-environment)))
      (delay
	(begin (if (not (null? clauses))
		   (warn "a FOR-SYNTAX clause appears in a package being linked by the cross-linker"
			 `(for-syntax ,@clauses)))
	       (cons eval env))))))
