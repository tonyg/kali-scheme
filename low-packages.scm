; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Things provided by the byte compiler / VM, together with a few
; things with rather sensitive definitions (low.scm).

(define-structures ((scheme-level-0 scheme-level-0-interface)
		    (primitives primitives-interface)
		    (bitwise bitwise-interface)
		    (closures closures-interface)
		    (code-vectors code-vectors-interface)
		    (features features-interface)
		    (write-images (export write-image))	;for linker
		    (source-file-names (export (%file-name% :syntax)))
		    (loopholes (export (loophole :syntax)))

		    (low-level low-level-interface)
		    (escapes escapes-interface)
		    (vm-exposure vm-exposure-interface)
		    (ascii ascii-interface)
		    (locations locations-interface)
		    (signals signals-interface)
		    (silly (export reverse-list->string))
		    (structure-refs (export (structure-ref :syntax))))
  (define-all-operators)		; Primitive Scheme, in the LSC paper
  (usual-transforms and cond do let let* or)
  (files (rts low)
	 (rts signal))
  (optimize auto-integrate))

