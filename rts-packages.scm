; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for runtime system (without compiler).

; Assumes signatures.scm.

(define-structure all-primitives (make-compiler-base))

; Subsets of the all-primitives package:

(define-package ((scheme-primitives scheme-primitives-signature)
		 (primitives primitives-signature)
		 (bitwise bitwise-signature)
		 (closures closures-signature)
		 (code-vectors code-vectors-signature)
		 (features features-signature)
		 (write-images (export write-image)))  ;for linker
  (open all-primitives))

(define-package ((low-level low-level-signature)
		 (escapes escapes-signature)
		 (vm-exposure vm-exposure-signature)
		 (ascii ascii-signature)
		 (locations locations-signature)
		 (silly (export really-string->symbol reverse-list->string))
		 (structure-refs (export structure-ref)))
  (open all-primitives)
  (usual-transforms)
  (files (rts low))
  (optimize auto-integrate))

; --- Use of all-primitives package prohibited beyond this point.


; "Level 1"

(define-package ((scheme-level-1 scheme-level-1-signature)
		 (util util-signature))
  (open scheme-primitives low-level ascii)
  (usual-transforms)
  (files (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

(define-package ((signals signals-signature))
  (open scheme-level-1 low-level)
  (files (rts signal))
  (optimize auto-integrate))

(define-package ((generics generics-signature))
  (open scheme-level-1 signals)
  (files (rts generic)))

(define-package ((number-i/o number-i/o-signature))
  (open scheme-level-1 signals ascii generics)
  (files (rts numio)))

(define-package ((record record-signature)
		 (record-internal record-internal-signature))
  (open scheme-level-1 signals generics primitives)
  (files (rts record))
  (optimize auto-integrate))

(define-package ((bummed-define-record-types define-record-types-signature))
  (open scheme-level-1 record record-internal)
  (files (rts bummed-jar-defrecord)))

(define-package ((fluids fluids-signature)
		 (fluids-internal fluids-internal-signature))
  (open scheme-level-1 bummed-define-record-types primitives)
  (files (rts fluid))
  (optimize auto-integrate))

(define-package ((wind wind-signature))
  (open scheme-level-1 signals bummed-define-record-types
	fluids fluids-internal
	escapes)
  (files (rts wind))
  (optimize auto-integrate))

(define-package ((port port-signature))
  (open scheme-level-1 signals fluids wind
	primitives)	;write-string, force-output
  (files (rts port)))

(define-package ((enumerated enumerated-signature))
  (open scheme-level-1 signals)
  (files (rts enum)
	 (rts defenum scm)))

(define-package ((architecture architecture-signature))
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

(define-package ((condition condition-signature))
  (open scheme-level-1 signals)
  (files (rts condition)))

; Hairier stuff now.

(define-package ((template template-signature))
  (open scheme-level-1 primitives)
  (files (rts template)))

(define-package ((continuation continuation-signature))
  (open scheme-level-1 primitives)
  (files (rts continuation)))

(define-package ((exception exception-signature)
		 (handle handle-signature))
  (open scheme-level-1
	signals port fluids
	condition	  ;make-exception, etc.
	primitives	  ;set-exception-handler!, etc.
	wind		  ;CWCC
	util		  ;reduce used in xprim.scm
	generics
	low-level	  ;halt
	template	  ;template-code
	continuation	  ;continuation-pc, etc.
	template	  ;template-info
	locations	  ;location?, location-id
	closures	  ;closure-template
	architecture)
  (files (rts exception)  ; Needs generic, arch
	 (rts xprim)))	  ; Needs arch, exception.  Support (+ x y z), etc.

(define-package ((interrupts interrupts-signature))
  (open scheme-level-1
	signals fluids condition
	bitwise
	primitives
	architecture)
  (files (rts interrupt))
  (optimize auto-integrate)) ;mostly for threads package...

(define-package ((writing writing-signature))
  (open scheme-level-1
	number-i/o
	port				;output-port-option, write-string
	generics)			;disclose
  (files (rts write)))
	 
(define-package ((reading reading-signature))
  (open scheme-level-1
	number-i/o
	port		;input-port-option
	ascii		;for dispatch table
	signals		;warn, signal-condition, make-condition
	condition	;define-condition-type
	silly)		;really-string->symbol, reverse-list->string
  (files (rts read)))

(define-package ((scheme-level-2 scheme-level-2-signature))
  (open scheme-level-1
	number-i/o
	writing
	reading
	wind
	port))

(define-package ((scheme-level-2-internal
		  (export usual-resumer)))
  (open scheme-level-1
	port		;with-initial-ports
	fluids-internal	;initialize-dynamic-state!
	exception	;initialize-exceptions!
	interrupts)	;initialize-interrupts!
  (files (rts init)))



; This hooks the compiler up with an exception handler for unbound variables.

(define-package ((shadowing shadowing-signature))
  (open scheme-level-1
	vm-exposure		;primitive-catch
	continuation template locations code-vectors
	exception signals
	architecture)	;(enum op global)
  (files (rts shadow)))     ;Exception handler to support package system


; Weak pointers & populations

(define-package ((weak weak-signature))
  (open scheme-level-1 signals
	primitives)	;Open primitives instead of loading (alt weak)
  (files ;;(alt weak)   ;Only needed if VM's weak pointers are buggy
	 (rts population)))
