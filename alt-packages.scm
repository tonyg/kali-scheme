; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This configuration file provides alternative implementations of many
; of the interfaces used by the Scheme 48 run-time system, byte-code
; compiler, and linker.


; The following several packages have Scheme-implementation-specific 
; variants that are much better for one reason or another than
; the generic versions defined here.

(define-package ((signals signals-signature)
		 (handle (export ignore-errors))
		 (features features-signature))
  (open scheme-level-2)
  (files (alt features)))

(define-package ((record record-signature))
  (open scheme-level-2 signals)
  (files (alt record)))

(define-package ((ascii (export ascii->char char->ascii)))
  (open scheme-level-2 signals)
  (files (alt ascii)))

(define-package ((bitwise bitwise-signature))
  (open scheme-level-2 signals)
  (files (alt bitwise)))

(define-package ((code-vectors code-vectors-signature))
  (open scheme-level-1)
  (files (alt code-vectors)))

; 

(define-package ((closures closures-signature))
  (open scheme-level-1 record)
  (files (alt closure)))

(define-package ((locations locations-signature))
  (open scheme-level-2 signals)
  (files (alt locations)))

(define-package ((util util-signature))
  (open scheme-level-1)
  (files (rts util)))

(define-package ((define-record-types define-record-types-signature))
  (open scheme-level-1 record)
  (files (rts jar-defrecord)))
(define-structure bummed-define-record-types define-record-types)

(define-package ((fluids fluids-signature))
  (open scheme-level-1 signals)
  (files (alt fluid)))

(define-package ((template template-signature))
  (open scheme-level-1)
  (files (alt template)
	 (rts template)))

(define-package ((weak weak-signature))
  (open scheme-level-1 signals)
  (files (alt weak)
	 (rts population)))

(define-package ((low-level (export %file-name%)))
  (open scheme-level-1 fluids syntactic)
  (files (alt file-name)))  ;tiny file

(define-package ((escapes escapes-signature))
  (open scheme-level-2 define-record-types)
  (files (alt escape)))

; Same as in rts-packages.scm:
(define-package ((enumerated enumerated-signature))
  (open scheme-level-1 signals)
  (files (rts enum)
	 (rts defenum scm)))

; Same as in rts-packages.scm:
(define-package ((architecture architecture-signature))
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

(define-package ((write-images (export write-image)))
  (open scheme-level-2
	table			;Forward reference
	features bitwise ascii enumerated
	architecture
	template
	signals)
  (files (link data)
	 (link transport)
	 (link write-image)))

(define-package ((environments environments-signature))
  (open scheme-level-2 signals)
  (begin (define (*structure-ref . rest)
	   (error "you shouldn't need *structure-ref"))))


; Muffle undefined variable warnings:

(define-package ((loser (export))
		 (vm-exposure (export))
		 (scheme-level-2-internal (export))
		 (structure-refs (export))
		 (shadowing (export)))
  (open "you shouldn't be trying to load this package"))

(define-module (make-mini-command . rest) loser)
(define-module (make-system-stuff . rest) (list loser loser))

;(define *structure-ref 'loser)
;(define built-in-structures '())

; These are mentioned in more-packages.scm:
;  condition display-conditions generics continuation
;  reading wind exception record-internal number-i/o strucure-refs
;  fluids-internal port

(define-package ((cont-primitives
		  (export make-continuation
			  continuation-length
			  continuation-ref
			  continuation-set!
			  continuation?)))
  (open scheme-level-2)
  (files (alt contin)))

(define-package ((alt-primitives primitives-signature)
		 (primitives-internal (export maybe-handle-interrupt
					      raise-exception
					      get-exception-handler
					      ?start)))
  (open scheme-level-2
	bitwise record
	features
	signals
	cont-primitives
	template)
  (files (alt primitives)
	 (alt weak)))

(define-package ((escapes escapes-signature))
  (open scheme-level-2 define-record-types signals)
  (files (alt escape)))

(define-package ((low-level low-level-signature)
		 (silly (export really-string->symbol reverse-list->string)))
  (open scheme-level-2 signals escapes)
  (files (alt low)))
