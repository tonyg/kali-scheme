; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This configuration file provides alternative implementations of many
; of the interfaces used by the Scheme 48 run-time system, byte-code
; compiler, and linker.


; Run-time structures: (cf. signature definition in packages.scm)

; Same as in rts-packages.scm:
(define-package ((architecture architecture-signature))
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

(define-package ((define-record-types define-record-types-signature))
  (open scheme-level-1 record)
  (files (rts jar-defrecord)))
(define-structure bummed-define-record-types define-record-types)

(define-package ((closures closures-signature))
  (open scheme-level-1 record)
  (files (alt closure)))

; Same as in rts-packages.scm:
(define-package ((enumerated enumerated-signature))
  (open scheme-level-1 signals)
  (files (rts enum)
	 (rts defenum scm)))

(define-package ((fluids fluids-signature))
  (open scheme-level-1 signals)
  (files (alt fluid)))

(define-package ((locations locations-signature))
  (open scheme-level-2 signals)
  (files (alt locations)))

(define-package ((loopholes (export (loophole syntax))))
  (open scheme-level-2)
  (files (alt loophole)))

(define-package ((source-file-names (export (%file-name% syntax))))
  (open scheme-level-1 fluids syntactic)
  (files (alt file-name)))  ;tiny file

(define-package ((scheme-level-2 scheme-level-2-signature)
		 (scheme-level-1 scheme-level-1-signature))
  (open scheme))

(define-package ((template template-signature))
  (open scheme-level-1)
  (files (alt template)
	 (rts template)))

(define-package ((util util-signature))
  (open scheme-level-1)
  (files (rts util)))

(define-package ((weak weak-signature))
  (open scheme-level-1 signals)
  (files (alt weak)
	 (rts population)))

(define-package ((write-images (export write-image)))
  (open scheme-level-2
	table			;Forward reference
	features bitwise ascii enumerated
	architecture
	template
	closures
	signals)
  (files (link data)
	 (link transport)
	 (link write-image)))


; --------------------
; Run-time internal structures: (cf. signature definition in packages.scm)

; condition            - same as run-time's
; continuation	       - same as run-time's
; display-conditions   - same as run-time's

(define-package ((escapes escapes-signature))
  (open scheme-level-2 define-record-types signals)
  (files (alt escape)))

; exception	       - no way
; fluids-internal      - no way
; generics	       - same as run-time's
; interrupts	       - no way

(define-package ((low-level low-level-signature)
		 (silly (export really-string->symbol reverse-list->string)))
  (open scheme-level-2 signals escapes)
  (files (alt low)))

; number-i/o	       - ?
; port		       - ?

; These are mentioned in more-packages.scm:
;  reading wind exception record-internal number-i/o strucure-refs
;  fluids-internal port

(define-package ((alt-primitives primitives-signature)
		 (primitives-internal (export maybe-handle-interrupt
					      raise-exception
					      get-exception-handler
					      ?start)))
  (open scheme-level-2
	bitwise record
	features
	signals
	template)
  (files (alt primitives)
	 (alt weak)
	 (alt contin)))

; port		       - ?
; reading	       - ?
; record-internal      - ?
; scheme-level-2-internal  - no way
; shadowing	       - no way
; structure-refs       - ?
; wind		       - ?
