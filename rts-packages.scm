; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for runtime system (without compiler).

; Assumes signature definitions in signatures.scm.

(define-package ((scheme-level-0 scheme-level-0-signature)
		 (primitives primitives-signature)
		 (bitwise bitwise-signature)
		 (closures closures-signature)
		 (code-vectors code-vectors-signature)
		 (features features-signature)
		 (write-images (export write-image))  ;for linker
		 (source-file-names (export %file-name%))
		 (loopholes (export (loophole syntax)))

		 (low-level low-level-signature)
		 (escapes escapes-signature)
		 (vm-exposure vm-exposure-signature)
		 (ascii ascii-signature)
		 (locations locations-signature)
		 (signals signals-signature)
		 (silly (export really-string->symbol reverse-list->string))
		 (structure-refs (export (structure-ref syntax))))
  (define-all-operators)		; Primitive Scheme, in the LSC paper
  (usual-transforms and cond do let let* or)
  (files (rts low)
	 (rts signal))
  (optimize auto-integrate))


; "Level 1" - all the easy things.  There is no dependence on any
; low-level or unusual primitives.

(define-package ((scheme-level-1 scheme-level-1-signature)
		 (util util-signature))
  (open scheme-level-0 ascii signals)
  (usual-transforms case delay quasiquote syntax-rules)
  (files (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

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
  (open scheme-level-1 record record-internal loopholes)
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
  (files (rts port))
  (optimize auto-integrate))

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
	;; low-level	  ;halt
	vm-exposure	  ;primitive-catch
	template	  ;template-code
	continuation	  ;continuation-pc, etc.
	template	  ;template-info
	locations	  ;location?, location-id
	closures	  ;closure-template
	architecture
	number-i/o)       ; number->string, for backtrace
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



; Weak pointers & populations

(define-package ((weak weak-signature))
  (open scheme-level-1 signals
	primitives)	;Open primitives instead of loading (alt weak)
  (files ;;(alt weak)   ;Only needed if VM's weak pointers are buggy
	 (rts population)))


; Utility for displaying error messages

(define-package ((display-conditions display-conditions-signature))
  (open scheme-level-2
	writing
	generics
	handle)			;ignore-errors
  (files (env dispcond)))
