; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This file contains things that tie together the compiler and the
; run-time system.


; EVAL

(define (eval form p)
  (compile-and-run-forms (list form) p #f
			 (lambda (template)
			   (invoke-template template p))
			 #f))

; LOAD-INTO - load file into package.

(define (load-into filename p)
  (compile-and-run-file filename p
			(lambda (template)
			  (invoke-template template p))
			(current-output-port)))

; Evaluate forms as if they came from the given file.

(define (eval-from-file forms p filename)
  (compile-and-run-forms forms p filename
			 (lambda (template)
			   (invoke-template template p))
			 (current-output-port)))

; For ENSURE-LOADED.

(define (eval-scanned-forms forms p filename)
  (compile-and-run-scanned-forms forms p filename
				 (lambda (template)
				   (invoke-template template p))
				 (current-output-port)))

(define (invoke-template template p)
  (invoke-closure (make-closure template (package-uid p))))


; LOAD

(define (load filename . package-option)
  (let ((p (if (null? package-option)
	       (interaction-environment)
	       (car package-option))))
    ;; (with-interaction-environment p
      ;; (lambda ()
	(noting-undefined-variables p
	  (lambda ()
	    (load-into filename p)))));; ))
