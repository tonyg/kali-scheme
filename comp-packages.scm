; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for byte-code compiler and initial image.


; Two basic structures needed to support the compiler.

(define-structure tables general-tables-interface
  (open scheme-level-1
	bummed-define-record-types
	signals
	features)    ; string-hash, make-immutable!
  (files (big general-table))
  (optimize auto-integrate))

(define-structure filenames filenames-interface
  (open scheme-level-1 signals)
  (files (big filename)))


; Type system

(define-structure meta-types meta-types-interface
  (open scheme-level-2
	bummed-define-record-types tables bitwise
	features ;make-immutable!
	util signals)
  (files (bcomp mtype))
  (optimize auto-integrate))

(define-structure interfaces interfaces-interface
  (open scheme-level-2 syntactic meta-types
	signals bummed-define-record-types tables
	weak)
  (files (bcomp interface))
  (optimize auto-integrate))


; Transforms and operators

(define-structure syntactic
    (compound-interface syntactic-interface
			nodes-interface)
  (open scheme-level-2 meta-types
	signals bummed-define-record-types tables fluids
	features		;make-immutable!
	;; locations		;location?
	)
  (files (bcomp syntax)
	 (bcomp schemify))
  (optimize auto-integrate))

(define-structure usual-macros usual-macros-interface
  (open scheme-level-2
	syntactic	;name?, $source-file-name
	fluids		;used in definition of %file-name%
	tables signals)
  (files (bcomp usual)
	 (bcomp rules)))

(define-structure reconstruction (export node-type reconstruct-type)
  (open scheme-level-2
	syntactic meta-types
	util		; last
	signals)
  (files (bcomp recon)))


; Package system

(define-structures ((packages packages-interface)
		    (packages-internal packages-internal-interface))
  (open scheme-level-2 syntactic meta-types interfaces
	signals bummed-define-record-types tables fluids
	util features locations weak)
  (files (bcomp package))
  (optimize auto-integrate))

(define-structure scan scan-interface
  (open scheme-level-2
	packages syntactic
	usual-macros		; for dealing with (usual-transforms ...)
	meta-types
	packages-internal
	signals fluids tables util
	features		;force-output
	filenames)		;translate
  (files (bcomp scan)
	 (bcomp undefined))
  (optimize auto-integrate))

; Compiler back end

(define-structures ((segments segments-interface)
		    (debug-data debug-data-interface))
  (open scheme-level-2 code-vectors templates
	syntactic
	architecture
	bummed-define-record-types
	features  ;make-immutable!
	records util tables fluids signals)
  (files (bcomp segment)
	 (bcomp state)
	 (bcomp ddata))
  (optimize auto-integrate))

; Byte-code compiler

(define-structure compiler compiler-interface
  (open scheme-level-2 syntactic scan meta-types
	architecture
	packages
	packages-internal       ;only for structure-package ?
	interfaces		;interface-ref
	locations		;make-undefined-location
	reconstruction
	segments
	signals
	tables
	enumerated		;enumerand->name
	util			;reduce
	fluids 
	features)		;force-output
  (files (bcomp comp)
	 (bcomp cprim)
	 (bcomp ctop))
  (optimize auto-integrate))

; DEFINE-STRUCTURE and friends

(define-structure defpackage defpackage-interface
  (open scheme-level-2
	packages syntactic usual-macros types
	interfaces
	source-file-names	;%file-name%
	signals			;error
	tables)
  (for-syntax (open scheme-level-2 signals))     ;syntax-error
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

; Static linker

(define-structure inline inline-interface
  (open scheme-level-2
	syntactic
	packages
	signals)
  (files (opt inline)))
