; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Alternate implementations of the low-structures.
; Cf. low-structures-interface in ../packages.scm and ../alt-structures.scm.

; Most of the low-structures are assumed to be inherited or obtained
; elsewhere (probably from a running Scheme 48).  This only defines
; structures that export privileged operations.

(define-structure escapes escapes-interface
  (open scheme-level-2 define-record-types simple-signals)
  (files escape))

(define-structures ((primitives primitives-interface)
		    (primitives-internal (export maybe-handle-interrupt
						 raise-exception
						 get-exception-handler
						 ?start)))
  (open scheme-level-2
	define-record-types
	bitwise	    ;Only for re-export
	features    ;Only for re-export
	simple-signals
	;; templates -- unneeded now?
	)
  (files primitives
	 weak
	 contin))

(define-structure code-quote (export (code-quote :syntax))
  (open scheme-level-2)
  (begin
    (define-syntax code-quote
      (lambda (e r c)
	`(,(r 'quote) ,(cadr e))))))

