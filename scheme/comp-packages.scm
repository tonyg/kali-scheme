; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Package definitions for byte-code compiler and initial image.


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

; Type system

(define-structure meta-types meta-types-interface
  (open scheme-level-2
	define-record-types tables bitwise
	features ;make-immutable!
	util signals)
  (files (bcomp mtype))
  (optimize auto-integrate))

(define-structure interfaces interfaces-interface
  (open scheme-level-2 syntactic meta-types
	signals define-record-types tables
	weak)
  (files (bcomp interface))
  (optimize auto-integrate))


; Transforms and operators

(define-structure syntactic
    (compound-interface syntactic-interface
			nodes-interface)
  (open scheme-level-2 meta-types
	signals define-record-types tables fluids
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
	util
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
	signals define-record-types tables fluids
	util features locations weak)
  (files (bcomp package))
  (optimize auto-integrate))

; Compiler's pre-scan of top-level forms
 
(define-structure scan scan-interface
  (open scheme-level-2
	packages syntactic
	usual-macros		; for dealing with (usual-transforms ...)
	meta-types
	packages-internal
	signals fluids tables util
	features		;force-output current-noise-port
	filenames)		;translate
  (files (bcomp scan)
	 (bcomp undefined))
  (optimize auto-integrate))

; Lexical environment layout info for debugging

(define-structures ((debug-data debug-data-interface))
  (open scheme-level-2 
	define-record-types)
  (files (bcomp ddata))
  (optimize auto-integrate))

; Determining stack usage.

(define-structure stack-check (export maximum-stack-use)
  (open scheme-level-2 architecture code-vectors signals)
  (files (bcomp stack-check))
  (optimize auto-integrate))

; Compiler back end

(define-structures ((segments segments-interface))
  (open scheme-level-2 code-vectors templates
	debug-data
	syntactic
	stack-check
	architecture
	define-record-types
	records   ; for debug-flags randomness
	features  ;make-immutable!
	util tables fluids signals)
  (files (bcomp state)
	 (bcomp segment))
  (optimize auto-integrate))

; Byte-code compiler

(define-structure compiler compiler-interface
  (open scheme-level-2 syntactic scan meta-types
	segments
	reconstruction
	packages
	packages-internal       ;only for structure-package ?
	interfaces		;interface-ref
	locations		;make-undefined-location
	architecture
	enumerated		;enumerand->name
	signals tables util fluids 
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
