; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Various data structures used by the compiler, module system, etc.

; Type system

(define-structure meta-types meta-types-interface
  (open scheme-level-2
	define-record-types tables bitwise
	features ;make-immutable!
	util signals)
  (files (bcomp mtype))
  (optimize auto-integrate))

; Bindings

(define-structure bindings bindings-interface
  (open scheme-level-2
	meta-types
	locations)
  (files (bcomp binding))
  (optimize auto-integrate))

; Names & Transforms

(define-structures ((names names-interface)
		    (transforms transforms-interface))
  (open scheme-level-2
	define-record-types tables
	signals
	meta-types	;sexp->type
	bindings	;same-denotation?
	features)	;make-immutable! string-hash
  (files (bcomp name)
	 (bcomp transform))
  (optimize auto-integrate))

; A thingie (placecard?) is used to hold a spot for a location that is to be
; found later.  The compiler sticks them in templates and the module system
; later replaces them with locations.
;
; We can't use (BEGIN ...) for this trivial package because it is loaded
; by flatload, which can't handle them.

(define-structure thingies (export make-thingie
				   thingie?
				   thingie-binding
				   thingie-name
				   thingie-want-type)
  (open scheme-level-2 define-record-types)
  (optimize auto-integrate)
  (files (bcomp thingie)))

; Nodes

(define-structure nodes nodes-interface
  (open scheme-level-2
	meta-types names
	signals define-record-types tables
	util)
  (files (bcomp node)
	 (bcomp schemify))
  (optimize auto-integrate))

(define-structure compiler-envs compiler-envs-interface
  (open scheme-level-2
	meta-types names bindings)
  (files (bcomp cenv))
  (optimize auto-integrate))

;--------------------------------
; Macros

(define-structure syntactic syntactic-interface
  (open scheme-level-2 util
	meta-types names bindings
	nodes compiler-envs
	signals tables fluids
	var-utilities
	transforms
	features)		;make-immutable!
  (files (bcomp syntax))
  (optimize auto-integrate))

(define-structure usual-macros usual-macros-interface
  (open scheme-level-2
	names		;name?
	fluids		;used in definition of %file-name%
	code-quote
	util
	tables signals)
  (files (bcomp usual)
	 (bcomp rules)))

; Little utilities to be phased out by changing the format of lambda var lists
; in nodes.

(define-structure var-utilities (export n-ary?
					normalize-formals
					number-of-required-args)
  (open scheme-level-2)
  (files (bcomp var-util))) ; can't use (BEGIN ...) because this is flatloaded

;--------------------------------
; Byte-code compiler

; Lexical environment layout info for debugging

(define-structures ((debug-data debug-data-interface)
		    (debug-data-internal debug-data-internal-interface))
  (open scheme-level-2 
	define-record-types
	tables
	fluids
	record-types		;for debug-flags randomness
	features)		;make-immutable!
  (files (bcomp ddata)
	 (bcomp state))
  (optimize auto-integrate))

; Determining stack usage.  No longer used.
;
;(define-structure stack-check (export maximum-stack-use)
;  (open scheme-level-2 architecture code-vectors signals)
;  (files (bcomp stack-check))
;  (optimize auto-integrate))

; Compiler back end

(define-structure segments segments-interface
  (open scheme-level-2 util tables signals fluids
	define-record-types
	bitwise vm-data
	code-vectors
	templates
	architecture
	features		;make-immutable!
	debug-data debug-data-internal
	frames)
  (files (bcomp segment))
  (optimize auto-integrate))

; Primops

(define-structure primops primops-interface
  (open scheme-level-2 tables define-record-types
	meta-types
	signals)
  (files (bcomp primop))
  (optimize auto-integrate))

; Type reconstruction.

(define-structure reconstruction (export node-type reconstruct-type)
  (open scheme-level-2 tables
	meta-types nodes names bindings
	primops
	var-utilities		;n-ary?
	util			;last
	signals)
  (files (bcomp recon)))

; The compiler itself.

(define-structures ((compiler compiler-interface)
		    (bc-generation bc-generation-interface))
  (open scheme-level-2 util signals
	features		;force-output
	enumerated		;enumerand->name
	ascii
	architecture
	meta-types names bindings
	transforms
	nodes var-utilities
	primops
	segments
	debug-data-internal	; keep-source-code?
	flat-environments
	frames
	reconstruction)
  (files (bcomp comp-exp)
	 (bcomp comp-lambda)
	 (bcomp comp-prim)
	 (bcomp comp))
  (optimize auto-integrate))

(define-structure frames frames-interface
  (open scheme-level-2
	define-record-types
	names
	architecture			; two-byte-limit
	templates			; template-overhead
	debug-data-internal		; new-debug-data
	signals				; error
	thingies)
  (files (bcomp frame))
  (optimize auto-integrate))

;----------------
; Reading the forms in a file.
; This is used by scan-package and rts/eval.scm.

(define-structure reading-forms (export read-forms $note-file-package)
  (open scheme-level-2
	fluids filenames cells
	features		;current-noise-port force-output
	signals                         ;error
	)
  (files (bcomp read-form)))

;----------------
; Live-variable analysis for closures.

(define-structure flat-environments (export flatten-form)
  (open scheme-level-2 nodes signals
	optimizer primops
	util			;every
	var-utilities)
  (files (bcomp flatten)))

;----------------
; Module system

(define-structure interfaces interfaces-interface
  (open scheme-level-2
	define-record-types tables
	util			;filter every mvlet
	signals			;error
	weak			;populations
	meta-types)
  (files (bcomp interface))
  (optimize auto-integrate))

(define-structures ((packages packages-interface)
		    (packages-internal packages-internal-interface)
		    (undefined undefined-interface))
  (open scheme-level-2
	define-record-types tables fluids signals cells
	util features locations weak
        meta-types interfaces
	names bindings
	compiler-envs
	templates
	thingies)
  (files (bcomp package)
	 (bcomp package-undef))
  (optimize auto-integrate))

(define-structure scan-package scan-package-interface
  (open scheme-level-2 util
	packages packages-internal
	meta-types bindings
	reading-forms
	filenames
	signals
	features		;current-noise-port force-output
	)
  (files (bcomp scan-package)))

(define-structure optimizer optimizer-interface
  (open scheme-level-2
	signals tables
	util)
  (files (bcomp optimize)))

(define-structure compile-packages (export compile-package)
  (open scheme-level-2 util tables
	syntactic
	packages
	packages-internal	;package-name
	optimizer
	compiler
	primops			;walk-primops
	compiler-envs
	nodes
	scan-package
	usual-macros		;for usual-transforms
	transforms		;for usual-transforms
	meta-types)		;for usual-transforms and define-all-operators
  (files (bcomp comp-package)))

;----------------
; DEFINE-STRUCTURE and friends

(define-structure defpackage defpackage-interface
  (open scheme-level-2
	packages syntactic usual-macros types
	interfaces
	source-file-names	;%file-name%
	signals			;error
	tables)
  (files (bcomp module-language)
	 (bcomp config)))

(define-structure types types-interface  ;Typing language
  (open scheme-level-2 meta-types syntactic loopholes)
  (files (bcomp type))
  ;; (optimize auto-integrate)  - doesn't work
  )

(define-structure module-system (compound-interface defpackage-interface
						    types-interface)
  (open defpackage types))

;----------------
; Code analysis and inlining

(define-structure usages usages-interface
  (open scheme-level-2
	meta-types names nodes
	packages
	packages-internal	;package-refine-type!
	reconstruction
	var-utilities
	define-record-types
	util signals tables strong)
  (files (opt usage)
         (opt sort)))

(define-structure analysis (export analyze-forms)
  (open scheme-level-2
	meta-types bindings nodes primops
	packages-internal	;package-add-static!
	inline
	usages
	reconstruction
	var-utilities
	transforms
	packages
	signals
	features		;force-output
	optimizer		;set-optimizer!
	util)
  (files (opt analyze)))

(define-structure inline inline-interface
  (open scheme-level-2 util var-utilities
	meta-types names bindings nodes
	compiler-envs
	transforms
	packages
	usages
	signals)
  (files (opt inline)))

(define-structure strong (export strongly-connected-components)
  (open scheme-level-2 define-record-types signals)
  (files (big strong))) ;!

;----------------
; Two basic structures needed to support the compiler.

(define-structure tables general-tables-interface
  (open scheme-level-1
	define-record-types
	signals
	features)    ; string-hash, make-immutable!
  (files (big general-table))
  (optimize auto-integrate))

(define-structure filenames filenames-interface
  (open scheme-level-1 signals)
  (files (big filename)))

