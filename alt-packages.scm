; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This configuration file provides alternative implementations of the
; low, run-time, run-time-internals structures.  Cf. the interface
; definitions in packages.scm.

; Run-time structures assumed to be inherited from somewhere: (none of
; these is used by the linker)
;  conditions
;  continuations
;  display-conditions
;  exceptions
;  fluids-internal
;  methods
;  meta-methods
;  interrupts
;  low-level
;  more-types
;  number-i/o
;  ports
;  reading
;  records-internal
;  scheme-level-2-internal
;  wind
;  writing

; --------------------
; low

; Features, assumed inherited:
;  ascii
;  bitwise
;  code-vectors
;  features
;  signals
; Unimplemented (you'll need a VM to do these):
;  vm-exposure
; Defined in alt/low-packages.scm:
;  escapes
;  primitives

(define-structure scheme-level-0 scheme-level-0-interface
  (open scheme))

(define-structure escapes escapes-interface ;cf. alt/low-packages.scm
  (open scheme-level-2 define-record-types signals)
  (files (alt escape)))

(define-structures ((low-level low-level-interface)
		    (source-file-names (export (%file-name% :syntax)))
		    (structure-refs (export (structure-ref :syntax))))
  (open scheme-level-2 signals)
  (files (alt low)))

(define-structure closures closures-interface
  (open scheme-level-1 records)
  (files (alt closure)))

(define-structure locations locations-interface
  (open scheme-level-2 signals)
  (files (alt locations)))

(define-structure loopholes (export (loophole :syntax))
  (open scheme-level-2)
  (files (alt loophole)))

(define-structure silly (export reverse-list->string)
  (open scheme-level-1)
  (files (alt silly)))

(define-structure write-images (export write-image)
  (open scheme-level-2
	tables			;Forward reference
	features bitwise ascii enumerated
	architecture
	templates
	closures
	signals)
  (files (link data)
	 (link transport)
	 (link write-image)))

; --------------------
; run-time (generally speaking, things needed by the linker)

; Same as in rts-packages.scm:
(define-structure architecture architecture-interface
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

; Use the non-bummed version!
(define-structure bummed-define-record-types define-record-types-interface
  (open scheme-level-1 records)
  (files (rts jar-defrecord)))

; Same as in rts-packages.scm:
(define-structure enumerated enumerated-interface
  (open scheme-level-1 signals)
  (files (rts enum)
	 (rts defenum scm)))

(define-structure fluids fluids-interface
  (open scheme-level-1 signals)
  (files (alt fluid)))

(define-structures ((scheme-level-2 scheme-level-2-interface)
		    (scheme-level-1 scheme-level-1-interface))
  (open scheme))

(define-structure templates templates-interface
  (open scheme-level-1)
  (files (alt template)
	 (rts template)))

(define-structure util util-interface
  (open scheme-level-1)
  (files (rts util)))

(define-structure weak weak-interface
  (open scheme-level-1 signals)
  (files (alt weak)
	 (rts population)))


; --------------------
; run-time internals (generally speaking, things not needed by the linker)

; * = mentioned in more-packages.scm
;   conditions
;   continuations
;   display-conditions
; * exceptions
; * fluids-internal
;   methods
;   meta-methods
;   interrupts
;   low-level
;   more-types
; * number-i/o
; * ports
; * reading
; * records-internal
;   scheme-level-2-internal
; * wind
;   writing

(define-structure wind wind-interface
  (open scheme-level-2)
  (files (alt reroot)))

; --------------------
; These don't really belong here, but I sure don't know where they
; ought to go.

(define-structure environments (export *structure-ref)
  (open )				;Assume flatloading
  (files (alt environments)))


; Procedure annotations

(define-structure annotations
    (export annotate-procedure procedure-annotation)
  (open scheme-level-1)
  (files (alt annotate)))
