; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for byte-code compiler and initial image.


; Two basic structures needed to support the compiler.

(define-package ((table table-signature))
  (open scheme-level-1 signals bummed-define-record-types
	features)			;string-hash
  (files (big table))
  (optimize auto-integrate))

(define-package ((filenames filenames-signature))
  (open scheme-level-1 signals)
  (files (big filename)))


; Type system

(define-package ((meta-types meta-types-signature))
  (open scheme-level-2 util signals)
  (files (bcomp mtype))
  (optimize auto-integrate))

(define-package ((signatures signatures-signature))
  (open scheme-level-2 syntactic meta-types
	signals bummed-define-record-types table
	weak)
  (files (bcomp signature))
  (optimize auto-integrate))


; Transforms and operators

(define-package ((syntactic (compound-signature syntactic-signature
						nodes-signature)))
  (open scheme-level-2 meta-types
	signals bummed-define-record-types table fluids
	features		;make-immutable!
	;; locations		;location?
	)
  (files (bcomp syntax))
  (optimize auto-integrate))

(define-package ((usual-macros usual-macros-signature))
  (open scheme-level-2
	syntactic	;name?, $source-file-name
	fluids		;used in definition of %file-name%
	table signals)
  (files (bcomp usual)
	 (bcomp rules)))

(define-package ((reconstruction (export node-type)))
  (open scheme-level-2
	syntactic meta-types
	signals)
  (files (bcomp recon)))


; Package system

(define-package ((packages packages-signature)
		 (packages-internal packages-internal-signature))
  (open scheme-level-2 syntactic meta-types signatures
	signals bummed-define-record-types table fluids
	util features locations weak)
  (files (bcomp package))
  (optimize auto-integrate))

(define-package ((scan scan-signature))
  (open scheme-level-2
	packages syntactic usual-macros meta-types
	packages-internal
	signals fluids table util
	features		;force-output
	filenames)		;translate
  (files (bcomp scan)
	 (bcomp undefined)))

; Compiler back end

(define-package ((segments segments-signature)
		 (debug-data debug-data-signature))
  (open scheme-level-2 code-vectors template
	syntactic
	architecture
	bummed-define-record-types
	record util table fluids signals)
  (files (bcomp segment)
	 (bcomp state)
	 (bcomp ddata))
  (optimize auto-integrate))

; Byte-code compiler

(define-package ((compiler compiler-signature))
  (open scheme-level-2 syntactic scan meta-types
	architecture
	packages
	packages-internal       ;only for structure-package ?
	signatures		;signature-ref
	reconstruction
	segments
	signals
	table
	enumerated		;enumerand->name
	util			;reduce
	fluids 
	features)		;force-output
  (files (bcomp comp)
	 (bcomp cprim)
	 (bcomp ctop))
  (optimize auto-integrate))

; DEFINE-PACKAGE and friends

(define-package ((defpackage defpackage-signature))
  (open scheme-level-2
	packages syntactic usual-macros types
	signatures
	source-file-names	;%file-name%
	signals			;error
	table)
  (for-syntax (open scheme-level-2 signals))     ;syntax-error
  (files (bcomp defpackage)
	 (bcomp config)))

(define-package ((types types-signature)) ;Typing language
  (open scheme-level-2 meta-types loopholes)
  (files (bcomp type)))

; Static linker

(define-package ((inline inline-signature))
  (open scheme-level-2
	syntactic
	packages
	signals)
  (files (opt inline)))
