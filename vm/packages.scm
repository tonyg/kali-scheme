; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Configuration file hacked quickly by JAR on 7/11/93 based on
; load.scm and experiment.

(define-package ((vm (export resume)))
  (open scheme bitwise
	features		;force-output
	ascii
	signals
	structure-refs
	architecture
	enumerated
	bigbit
	debugging)		;breakpoint
  (access code-vectors)
  ;; (for-syntax (open scheme architecture for-syntax))
  (files s48-features	;List of files from load.scm
	 scheme-util
	 macros
	 
	 memory
	 arch
	 data
	 struct
	 vmio
	 init
	 gc
	 
	 istruct
	 stack
	 interp
	 prim
	 
	 resume)
  (begin (set! no-exceptions? #t)
	 (define (ps-ticks-per-second) 1000)))

(define-package ((for-syntax (export concatenate-symbol)))
  (open scheme-level-1)
  (files for-syntax))
