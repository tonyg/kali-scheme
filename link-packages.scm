; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Static linker.  Doesn't work very well this way (debug info is
; screwed up), so it's probably better to continue using linker.image
; instead.

(define-structure linker linker-interface
  (open scheme-level-2
	tables compiler write-images debuginfo
	packages
	packages-internal       ;set-package-integrate?!
	scan
	syntactic		;name->symbol		
	reification
	closures		;make closure to pass to write-image
	filenames
	debug-data		;with-fresh-compiler-state
	locations
	fluids
	signals			;error
	util)			;sublist
  (files (link link)))

(define-structure reification (export reify-structures)
  (open scheme-level-2
	packages
	packages-internal ;?
	usual-macros		;find-free-names-in-syntax-rules
	syntactic		;name-hash, etc.
	meta-types			;usual-variable-type
	locations
	tables records
	signals			;error
	features		;force-output
	util			;filter
	inline)			;name->extrinsic
  (files (link reify)))

(define-structure expander expander-interface
  (open scheme-level-2
	syntactic packages scan meta-types reconstruction
	bummed-define-record-types
	util signals tables fluids)
  (files (opt expand)))

(define-structure analysis (export analyze-forms)
  (open scheme
	syntactic packages scan inline meta-types expander
	packages-internal	;set-package-integrate?!
	reconstruction
	signals
	locations
	features		;force-output
	tables
	fluids
	util)
  (files (opt analyze)))


; Database of procedure names 'n' stuff.
; (copy in more-packages.scm ...)

(define-structure debuginfo debuginfo-interface
  (open scheme-level-2
	tables
	debug-data
	packages
	packages-internal
	syntactic)
  (files (env debuginfo)))


; Mumble.

(define-structure flatloading (export flatload)
  (open scheme packages packages-internal filenames)
  (files (alt flatload)))

(define-structure loadc (export load-configuration
				;; (structure-ref :syntax)
				)
  (open scheme
	environments		; *structure-ref
	syntactic		; $source-file-name
	fluids)
  (files (link loadc)))


; Everything.

(define-structure link-config (export )  ;dummy structure...
  (open linker
	defpackage
	types
	analysis
	loadc
	flatloading
	interfaces)
  ;; (files (alt init-defpackage.scm))  -- or (env ...), depending
  )
