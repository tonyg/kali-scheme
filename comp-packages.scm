; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for byte-code compiler and initial image.

(define-package ((table table-signature))
  (open scheme-level-1 signals bummed-define-record-types
	;; We obtain only the STRING-HASH procedure from the features module.
	features)
  (files (rts table))
  (optimize auto-integrate))

(define-package ((filenames filenames-signature))
  (open scheme-level-1 signals)
  (files (rts filename)))


; Transforms and operators

(define-package ((syntactic syntactic-signature))
  (open scheme-level-2 signals bummed-define-record-types table fluids
	features		;make-immutable!
	locations)		;location?
  (files (bcomp syntax))
  (optimize auto-integrate))

(define-package ((usual-macros usual-macros-signature))
  (open scheme-level-2
	syntactic	;name?, $source-file-name
	fluids		;used in definition of %file-name%
	table signals)
  (files (bcomp usual)
	 (bcomp rules)))


; Package system

(define-package ((packages packages-signature))
  (open scheme-level-2 syntactic
	signals bummed-define-record-types table fluids
	util features locations weak handle)
  (files (bcomp structure)
	 (bcomp package)
	 (bcomp location))
  (optimize auto-integrate))

(define-package ((scan scan-signature))
  (open scheme-level-2
	packages syntactic usual-macros
	signals fluids table util
	features		;force-output
	filenames)		;translate
  (files (bcomp scan)))

; Byte-code compiler

(define-package ((compiler compiler-signature)
		 (debug-data debug-data-signature))
  (open scheme-level-2 packages syntactic scan
	architecture
	signals bummed-define-record-types record table fluids enumerated
	util
	features		;?
	code-vectors
	closures		;make-startup-procedure
	locations template)
  (files (bcomp state)
	 (bcomp ddata)
	 (bcomp segment)
	 (bcomp comp)
	 (bcomp cprim)
	 (bcomp ctop))
  (optimize auto-integrate))

; Access to values from packages and structures 

(define-package ((environments environments-signature))
  (open scheme-level-2
	packages syntactic
	fluids
	locations	; contents
	signals)	; error
  (files (rts env)))

; Utility for displaying error messages

(define-package ((display-conditions display-conditions-signature))
  (open scheme-level-2
	writing
	generics
	handle)			;ignore-errors
  (files (env dispcond)))

; DEFINE-PACKAGE and friends

(define-package ((defpackage defpackage-signature))
  (open scheme-level-2
	packages syntactic usual-macros
	environments		;*structure-ref
	low-level		;%file-name%
	signals			;error
	table)
  (files (env defpackage)
	 (env config)))

; Static linker

(define-package ((inline inline-signature))
  (open scheme-level-2
	syntactic
	packages
	signals)
  (files (opt inline)))

(define-package ((for-reification for-reification-signature))
  (open scheme-level-1
	packages
	syntactic usual-macros
	inline))



; EVAL and LOAD

(define-package ((evaluation evaluation-signature))
  (open scheme-level-2
	signals
	closures		;make-closure
	vm-exposure		;invoke-closure
	packages		;set-shadow-action!
	shadowing		;shadow-location!
	environments		;package-for-load
	compiler fluids)
  (files (rts eval)))

; Scheme = scheme-level-2 plus EVAL and friends

(define-module (make-scheme environments evaluation)
  (define-package ((scheme scheme-signature))
    (open scheme-level-2
	  environments
	  evaluation))
  scheme)

(define-structure scheme (make-scheme environments evaluation))


; Load packages, following dependency (OPEN and ACCESS) links

(define-package ((ensures-loaded  ;Cf. (link-initial-system) and Makefile
		  (export ensure-loaded)))
  (open scheme-level-2
	packages
	evaluation		;load
	filenames
	signals
	environments		;with-interaction-environment
	scan)			;scan-structures
  (files (env load-package)))


; Command processor.

(define-module (make-mini-command scheme) ;copied from debug-packages.scm
  (define-package ((mini-command (export command-processor)))
    (open scheme
	  signals condition handle
	  display-conditions)
    (files (debug mini-command)))
  mini-command)

; (define-module (make-command scheme) ...)  -- moved to more-packages.scm


; Take your pick:

;(define-structure command (make-command scheme))
(define-structure command (make-mini-command scheme))


; For building systems.

(define-module (make-initial-system scheme command)

  (define-package ((initial-system (export start)))
    (open scheme
	  command
	  packages		;make-simple-package
	  environments		;with-interaction-environment, etc.
	  signals		;error
	  scheme-level-2-internal ;usual-resumer
	  condition handle	;error? with-handler
	  vm-exposure		;primitive-catch - in case of error
	  exception)		;continuation-preview
    (files (env start)))

  initial-system)


(define-structure initial-system
  (make-initial-system scheme command))

