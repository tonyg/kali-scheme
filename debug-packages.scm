; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Debugging stuff.

; For example: in the linker image or after ,take l.s48:
;
;   ,load scripts.scm
;   (load-configuration "debug-packages.scm")
;   (link-little-system)


; Alternative command processor.  Handy for debugging the bigger one.

(define-module (make-mini-command scheme)
  (define-structure mini-command (export command-processor)
    (open scheme
	  signals conditions handle
	  display-conditions)
    (files (debug mini-command)))
  mini-command)

; Miniature EVAL, for debugging runtime system sans package system.

(define-structures ((mini-eval evaluation-interface)
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
  (define-structure scheme scheme-interface
    (open scheme-level-2
	  environments
	  evaluation))
  scheme)

(define mini-scheme (make-scheme mini-environments mini-eval))

; Stand-alone system that doesn't contain a byte-code compiler.
; This is useful for various testing purposes.

(define little-system (make-mini-command mini-scheme))

(define (link-little-system)
  (link-simple-system '(debug little)
		      '(usual-resumer command-processor) 
		      little-system scheme-level-2-internal))




; --------------------
; Hack: smallest possible reified system.

(define-structure mini-packages
    (compound-interface for-reification-interface
			(export make-simple-package))
  (open scheme-level-2
	tables
	features		;contents
	locations
	signals)		;error
  (files (debug mini-package)))

(define mini-command (make-mini-command mini-scheme))

(define-structure mini-system (export start)
  (open mini-scheme
	mini-command
	mini-packages
	mini-eval-internal
	signals			;error
	conditions handle	;error? with-handler
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

(define-structure run evaluation-interface
  (open scheme-level-2 syntactic packages scan types
	environments
	signals
	locations
	features   ;force-output
	tables
	fluids)
  (files (debug run)))


; Hack: an interpreter-based system.

(define medium-scheme (make-scheme environments run))

(define medium-system
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
