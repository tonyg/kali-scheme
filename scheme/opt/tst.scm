; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.



(define-structure primitives (make-compiler-base))

; "Level 1"

(define-structures ((scheme-level-1 scheme-level-1-interface)
		    (scheme-level-1-internal scheme-level-1-internal-interface)
		    (bitwise bitwise-interface)
		    (util util-interface)
		    (simple-signals signals-interface)
		    (features features-interface)
		    (ascii ascii-interface)
		    (structure-refs (export structure-ref)))
  (open primitives)
  (usual-transforms)
  (optimize auto-integrate)
  (files (rts base)
	 (rts util)
	 (rts signal)
	 (rts number)
	 (rts lize)	  ; Rationalize
	 ))

