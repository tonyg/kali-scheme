; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Structures that export structures: the big picture.


; Scheme 48 run time system.

(define-interface run-time-structures-interface
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
	    templates
	    util
	    weak
	    write-images)
	   structure)))

(define-interface features-structures-interface
  (export ((ascii
	    bitwise
	    code-vectors
	    features
	    handle
	    records
	    signals)
	   structure)))

(define-interface run-time-internals-interface
  (export ((conditions
	    continuations
	    display-conditions
	    escapes
	    exceptions
	    fluids-internal
	    generics
	    interrupts
	    low-level
	    number-i/o
	    ports
	    primitives
	    reading
	    records-internal
	    scheme-level-2-internal
	    structure-refs
	    vm-exposure
	    wind)
	   structure)))

(define-package ((run-time-structures run-time-structures-interface)
		 (features-structures features-structures-interface)
		 (run-time-internals  run-time-internals-interface))
  (open module-system
	;; the-interfaces
	)
  (files rts-packages))
  

; Byte-code compiler and related things.

(define-interface compiler-structures-interface
  (export ((compiler
	    debug-data
	    defpackage
	    filenames
	    inline
	    meta-types
	    interfaces
	    module-system  ;?
	    packages
	    packages-internal
	    reconstruction
	    scan
	    segments
	    syntactic
	    tables
	    types
	    usual-macros)
	   structure)))

(define-module (make-compiler-structures run-time-structures
					 features-structures)

  (define-package ((compiler-structures compiler-structures-interface))
    (open module-system
	  run-time-structures
	  features-structures
	  ;; the-interfaces
	  )
    (files comp-packages))

  compiler-structures)

(define-structure compiler-structures
  (make-compiler-structures run-time-structures features-structures))


; Initial system (initial.image).

(define-interface initial-structures-interface
  (export environments
	  evaluation
	  scheme
	  command
	  ensures-loaded
	  ;; These two are in there, but shouldn't get reified.
	  ;; initial-system
	  ;; for-reification
	  ))

(define-package ((initial-structures initial-structures-interface))
  (open run-time-structures
	compiler-structures
	;; the-interfaces
	)
  (files initial-packages))


; Static linker.

(define-module (make-linker-structures features-structures
				       run-time-structures
				       compiler-structures)

  (define-package ((linker-structures linker-structures-interface))
    (open module-system
	  features-structures
	  run-time-structures
	  compiler-structures)
    (files link-packages))

  linker-structures)


; Initial + usual (scheme48.image).

(define-interface usual-structures-interface
  (export ((?)
	   structure)))

(define-interface linker-structures-interface
  (export ((analysis
	    debuginfo
	    expander
	    flatloading
	    linker
	    link-config
	    loadc
	    reification)
	   structure)))

(define-package ((usual-structures usual-structures-interface))
  (open module-system
	run-time-structures
	compiler-structures
	initial-structures
	(make-linker-structures features-structures
				run-time-structures
				compiler-structures))
  (files ;; more-interfaces
	 more-packages))



; Alternative implementations.

(define-module (make-alternate-structures features-structures)

  (define-package ((alternate-structures run-time-structures-interface))
    (open module-system features-structures)
    (files alt-packages))

  alternate-structures)

(define-package ((vanilla-features-structures features-structures-interface))
  (open module-system)
  (files (alt packages)))

(define-package ((cheat features-structures-interface))
  (open module-system)
  (begin (define-package ((signals signals-interface)
			  (handle handle-interface)
			  (features features-interface)
			  (records records-interface)
			  (ascii ascii-interface)
			  (bitwise bitwise-interface)
			  (code-vectors code-vectors-interface))
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
