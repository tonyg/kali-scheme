; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Initialize the defpackage package by providing it with an EVAL to
; stick into packages that it creates.  This file should be loaded
; into the initial-image before any use of DEFINE-STRUCTURE.  This seems
; wrong somehow, but I haven't thought of a better way to do it yet.

(init-defpackage! eval (lambda ()
			 (let ((p (interaction-environment)))
			   (delay (package-for-syntax p)))))
