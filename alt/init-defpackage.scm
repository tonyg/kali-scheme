; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Initialize the defpackage package by providing it with an EVAL to
; stick into packages that it creates.  This file should be loaded
; into the bootstrap linker before any use of DEFINE-PACKAGE.  This
; seems wrong somehow, but I haven't thought of a better way to do it
; yet.

(init-defpackage! #f
		  (lambda ()
		    (let ((env (interaction-environment)))
		      (delay (make-simple-package '()
						  (lambda (form p)
						    ;; Ignore p !
						    (eval form env))
						  (delay #f))))))
