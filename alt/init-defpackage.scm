; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Initialize the defpackage package by providing it with an EVAL to
; stick into packages that it creates.  This file should be loaded
; into the bootstrap linker before any use of DEFINE-PACKAGE.  This
; seems wrong somehow, but I haven't thought of a better way to do it
; yet.

(let ((eval (let ((env (interaction-environment)))
	      (lambda (form p)
		;; Ignore p !
		(eval form env)))))
  (init-defpackage! eval
		    (lambda ()
		      (delay (make-simple-package '()
						  eval
						  (delay #f))))))
