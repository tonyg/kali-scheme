; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



(define-structure primitives (make-compiler-base))

; "Level 1"

(define-package ((scheme-level-1 scheme-level-1-signature)
		 (scheme-level-1-internal scheme-level-1-internal-signature)
		 (bitwise bitwise-signature)
		 (util util-signature)
		 (signals signals-signature)
		 (features features-signature)
		 (ascii ascii-signature)
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

