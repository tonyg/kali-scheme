; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Structures that export structures: the big picture.


; Scheme 48 run time system.

(define-signature run-time-structures-signature
  (export ((architecture
	    bummed-define-record-types
	    closures
	    enumerated
	    fluids
	    locations
	    loopholes
	    source-file-names
	    scheme-level-1
	    scheme-level-2
	    template
	    util
	    weak
	    write-images)
	   structure)))

(define-signature features-structures-signature
  (export ((ascii
	    bitwise
	    code-vectors
	    features
	    handle
	    record
	    signals)
	   structure)))

(define-signature run-time-internals-signature
  (export ((condition
	    continuation
	    display-conditions
	    escapes
	    exception
	    fluids-internal
	    generics
	    interrupts
	    low-level
	    number-i/o
	    port
	    primitives
	    reading
	    record-internal
	    scheme-level-2-internal
	    structure-refs
	    vm-exposure
	    wind)
	   structure)))

(define-package ((run-time-structures run-time-structures-signature)
		 (features-structures features-structures-signature)
		 (run-time-internals  run-time-internals-signature))
  (open module-system
	;; the-signatures
	)
  (files rts-packages))
  

; Byte-code compiler and related things.

(define-signature compiler-structures-signature
  (export ((compiler
	    debug-data
	    defpackage
	    filenames
	    inline
	    meta-types
	    signatures
	    module-system  ;?
	    packages
	    packages-internal
	    reconstruction
	    scan
	    segments
	    syntactic
	    table
	    types
	    usual-macros)
	   structure)))

(define-module (make-compiler-structures run-time-structures
					 features-structures)

  (define-package ((compiler-structures compiler-structures-signature))
    (open module-system
	  run-time-structures
	  features-structures
	  ;; the-signatures
	  )
    (files comp-packages))

  compiler-structures)

(define-structure compiler-structures
  (make-compiler-structures run-time-structures features-structures))


; Initial system (initial.image).

(define-signature initial-structures-signature
  (export environments
	  evaluation
	  scheme
	  command
	  ensures-loaded
	  ;; These two are in there, but shouldn't get reified.
	  ;; initial-system
	  ;; for-reification
	  ))

(define-package ((initial-structures initial-structures-signature))
  (open run-time-structures
	compiler-structures
	;; the-signatures
	)
  (files initial-packages))


; Static linker.

(define-module (make-linker-structures features-structures
				       run-time-structures
				       compiler-structures)

  (define-package ((linker-structures linker-structures-signature))
    (open module-system
	  features-structures
	  run-time-structures
	  compiler-structures)
    (files link-packages))

  linker-structures)


; Initial + usual (scheme48.image).

(define-signature usual-structures-signature
  (export ((?)
	   structure)))

(define-signature linker-structures-signature
  (export ((analysis
	    debuginfo
	    expander
	    flatloading
	    linker
	    link-config
	    loadc
	    reification)
	   structure)))

(define-package ((usual-structures usual-structures-signature))
  (open module-system
	run-time-structures
	compiler-structures
	initial-structures
	(make-linker-structures features-structures
				run-time-structures
				compiler-structures))
  (files ;; more-signatures
	 more-packages))



; Alternative implementations.

(define-module (make-alternate-structures features-structures)

  (define-package ((alternate-structures run-time-structures-signature))
    (open module-system features-structures)
    (files alt-packages))

  alternate-structures)

(define-package ((vanilla-features-structures features-structures-signature))
  (open module-system)
  (files (alt packages)))

(define-package ((cheat features-structures-signature))
  (open module-system)
  (begin (define-package ((signals signals-signature)
			  (handle handle-signature)
			  (features features-signature)
			  (record record-signature)
			  (ascii ascii-signature)
			  (bitwise bitwise-signature)
			  (code-vectors code-vectors-signature))
	   ;; Implemented with a manual ,open signals handle ...
	   )))

(define-structure alt-features-structures cheat);Or vanilla-features-structures


; Linker image for bootstrap (link/linker.image).

(define-structure alternate-structures
  (make-alternate-structures alt-features-structures))

(define-structure linker-structures
  (make-linker-structures alt-features-structures
			  alternate-structures
			  (make-compiler-structures alt-features-structures
						    alternate-structures)))
