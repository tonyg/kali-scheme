; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This configuration file provides alternative implementations of many
; of the interfaces used by the Scheme 48 run-time system, byte-code
; compiler, and linker.


; Run-time structures: (cf. interface definition in packages.scm)

; Same as in rts-packages.scm:
(define-structures ((architecture architecture-interface))
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

(define-structures ((define-record-types define-record-types-interface))
  (open scheme-level-1 records)
  (files (rts jar-defrecord)))
(define-structure bummed-define-record-types define-record-types)

(define-structures ((closures closures-interface))
  (open scheme-level-1 records)
  (files (alt closure)))

; Same as in rts-packages.scm:
(define-structures ((enumerated enumerated-interface))
  (open scheme-level-1 signals)
  (files (rts enum)
	 (rts defenum scm)))

(define-structures ((fluids fluids-interface))
  (open scheme-level-1 signals)
  (files (alt fluid)))

(define-structures ((locations locations-interface))
  (open scheme-level-2 signals)
  (files (alt locations)))

(define-structures ((loopholes (export (loophole :syntax))))
  (open scheme-level-2)
  (files (alt loophole)))

(define-structures ((source-file-names (export (%file-name% :syntax))))
  (open scheme-level-1 fluids syntactic)
  (files (alt file-name)))  ;tiny file

(define-structures ((scheme-level-2 scheme-level-2-interface)
		 (scheme-level-1 scheme-level-1-interface))
  (open scheme))

(define-structures ((templates templates-interface))
  (open scheme-level-1)
  (files (alt template)
	 (rts template)))

(define-structures ((util util-interface))
  (open scheme-level-1)
  (files (rts util)))

(define-structures ((weak weak-interface))
  (open scheme-level-1 signals)
  (files (alt weak)
	 (rts population)))

(define-structures ((write-images (export write-image)))
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
; Run-time internal structures: (cf. interface definition in packages.scm)

; conditions           - same as run-time's
; continuations	       - same as run-time's
; display-conditions   - same as run-time's

(define-structures ((escapes escapes-interface))
  (open scheme-level-2 define-record-types signals)
  (files (alt escape)))

; exception	       - no way
; fluids-internal      - no way
; generics	       - same as run-time's
; interrupts	       - no way

(define-structures ((low-level low-level-interface)
		    (silly (export really-string->symbol reverse-list->string)))
  (open scheme-level-2 signals escapes)
  (files (alt low)))

; number-i/o	       - ?
; ports		       - ?

; These are mentioned in more-packages.scm:
;  reading wind exception records-internal number-i/o strucure-refs
;  fluids-internal ports

(define-structures ((alt-primitives primitives-interface)
		    (primitives-internal (export maybe-handle-interrupt
						 raise-exception
						 get-exception-handler
						 ?start)))
  (open scheme-level-2
	bitwise records
	features
	signals
	templates)
  (files (alt primitives)
	 (alt weak)
	 (alt contin)))

; ports		       - ?
; reading	       - ?
; records-internal      - ?
; scheme-level-2-internal  - no way
; shadowing	       - no way
; structure-refs       - ?
; wind		       - ?
