; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Debugging stuff.

; For example: in the linker image or after ,take l.s48:
;
;   ,load scripts.scm
;   (load-configuration "debug-packages.scm")
;   (link-little-system)


; Alternative command processor.  Handy for debugging the bigger one.

(define-module (make-mini-command scheme)
  (define-package ((mini-command (export command-processor)))
    (open scheme
	  signals condition handle
	  display-conditions)
    (files (debug mini-command)))
  mini-command)

; Miniature EVAL, for debugging runtime system sans package system.

(define-package ((mini-eval evaluation-signature)
		 (mini-eval-internal
		  (export set-interaction-environment!
			  set-scheme-report-environment!))
		 (mini-environments
		  (export interaction-environment
			  scheme-report-environment)))
  (open scheme-level-2
	signals)		;error
  (files (debug mini-eval)))

(define-module (make-scheme environments evaluation)
  (define-package ((scheme scheme-signature))
    (open scheme-level-2
	  environments
	  evaluation))
  scheme)

(define-structure mini-scheme (make-scheme mini-environments mini-eval))

; Stand-alone system that doesn't contain a byte-code compiler.
; This is useful for various testing purposes.

(define-structure little-system (make-mini-command mini-scheme))

(define (link-little-system)
  (link-simple-system '(debug little)
		      '(usual-resumer command-processor) 
		      little-system scheme-level-2-internal))




; --------------------
; Hack: smallest possible reified system.

(define-package ((mini-packages for-reification-signature))
  (open scheme-level-2
	table
	features		;contents
	signals)		;error
  (files (debug mini-package)))

(define-structure mini-command (make-mini-command mini-scheme))

(define-package ((mini-system (export start)))
  (open mini-scheme
	mini-command
	mini-packages
	mini-eval-internal
	signals			;error
	condition handle	;error? with-handler
	scheme-level-2-internal ;usual-resumer
	)
  (files (debug mini-start)))

(define (link-mini-system)
  (link-reified-system (list (cons 'scheme mini-scheme))
		       '(debug mini)
		       'start
		       mini-system mini-packages))



; --------------------
; S-expression interpreter

(define-package ((run evaluation-signature))
  (open scheme-level-2 syntactic packages scan types
	environments
	signals
	locations
	features   ;force-output
	table
	fluids)
  (files (debug run)))


; Hack: an interpreter-based system.

(define-structure medium-scheme (make-scheme environments run))

(define-structure medium-system
  (make-initial-system medium-scheme
		       (make-mini-command medium-scheme)))

(define (link-medium-system)		;cf. scripts.scm
  (let* ((scheme medium-scheme)
	 (initial-structures
	  (struct-list scheme signals packages)))
    (link-reified-system initial-structures
			 '(debug medium)
			 `(start ',(map car initial-structures))
			 ;; Packages to open for evaluating the expression
			 ;; that evaluates to the startup procedure
			 medium-system
			 for-reification)))
