; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Link scripts.

(load-configuration "signatures.scm")
(load-configuration "rts-packages.scm")
(load-configuration "comp-packages.scm")

(define (link-initial-system)
  (let ((initial-structures
	 (struct-list environments
		      defpackage	; contains macros...
		      ensures-loaded
		      packages)))
    (link-reified-system (append (desirable-structures)
				 (struct-list scheme signals)
				 initial-structures)
			 'initial
			 `(start '(scheme ,@(map car initial-structures)))
			 ;; Packages to open for evaluating the expression
			 ;; that evaluates to the startup procedure
			 initial-system
			 for-reification)))

(define (desirable-structures)
  (struct-list primitives
	       low-level		;must occur early in list (reify bug)
	       vm-exposure		;ditto
	       architecture
	       ascii
	       bitwise
	       bummed-define-record-types
	       closures
	       code-vectors
	       command
	       compiler
	       condition
	       continuation
	       debug-data
	       display-conditions	;disclosers
	       enumerated
	       environments
	       escapes			;threads
	       evaluation
	       exception
	       features
	       filenames		;linker
	       fluids
	       fluids-internal		;threads
	       generics			;extended-numbers
	       handle
	       inline			;?
	       interrupts
	       locations
	       number-i/o
	       port			;threads
	       reading
	       record			;For command loop, etc.
	       record-internal
	       scan
	       scheme-level-1
	       scheme-level-2
	       scheme-level-2-internal
	       shadowing
	       structure-refs
	       syntactic
	       table
	       template
	       usual-macros
	       util
	       weak			;disclosers
	       wind
	       write-images		;linker
	       writing
	       ))
