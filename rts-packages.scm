; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for runtime system (without compiler).

; Assumes interface definitions in interfaces.scm.

(define-package ((scheme-level-0 scheme-level-0-interface)
		 (primitives primitives-interface)
		 (bitwise bitwise-interface)
		 (closures closures-interface)
		 (code-vectors code-vectors-interface)
		 (features features-interface)
		 (write-images (export write-image))  ;for linker
		 (source-file-names (export %file-name%))
		 (loopholes (export (loophole syntax)))

		 (low-level low-level-interface)
		 (escapes escapes-interface)
		 (vm-exposure vm-exposure-interface)
		 (ascii ascii-interface)
		 (locations locations-interface)
		 (signals signals-interface)
		 (silly (export really-string->symbol reverse-list->string))
		 (structure-refs (export (structure-ref syntax))))
  (define-all-operators)		; Primitive Scheme, in the LSC paper
  (usual-transforms and cond do let let* or)
  (files (rts low)
	 (rts signal))
  (optimize auto-integrate))


; "Level 1" - all the easy things.  There is no dependence on any
; low-level or unusual primitives.

(define-package ((scheme-level-1 scheme-level-1-interface)
		 (util util-interface))
  (open scheme-level-0 ascii signals)
  (usual-transforms case delay quasiquote syntax-rules)
  (files (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

(define-package ((generics generics-interface))
  (open scheme-level-1 signals)
  (files (rts generic)))

(define-package ((number-i/o number-i/o-interface))
  (open scheme-level-1 signals ascii generics)
  (files (rts numio)))

(define-package ((records records-interface)
		 (records-internal records-internal-interface))
  (open scheme-level-1 signals generics primitives)
  (files (rts record))
  (optimize auto-integrate))

(define-package ((bummed-define-record-types define-record-types-interface))
  (open scheme-level-1 records records-internal loopholes)
  (files (rts bummed-jar-defrecord)))

(define-package ((fluids fluids-interface)
		 (fluids-internal fluids-internal-interface))
  (open scheme-level-1 bummed-define-record-types primitives)
  (files (rts fluid))
  (optimize auto-integrate))

(define-package ((wind wind-interface))
  (open scheme-level-1 signals bummed-define-record-types
	fluids fluids-internal
	escapes)
  (files (rts wind))
  (optimize auto-integrate))

(define-package ((ports ports-interface))
  (open scheme-level-1 signals fluids wind
	primitives)	;write-string, force-output
  (files (rts port))
  (optimize auto-integrate))

(define-package ((enumerated enumerated-interface))
  (open scheme-level-1 signals)
  (files (rts enum)
	 (rts defenum scm)))

(define-package ((architecture architecture-interface))
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

(define-package ((conditions conditions-interface))
  (open scheme-level-1 signals)
  (files (rts condition)))

; Hairier stuff now.

(define-package ((templates templates-interface))
  (open scheme-level-1 primitives)
  (files (rts template)))

(define-package ((continuations continuations-interface))
  (open scheme-level-1 primitives)
  (files (rts continuation)))

(define-package ((exceptions exceptions-interface)
		 (handle handle-interface))
  (open scheme-level-1
	signals ports fluids
	conditions	  ;make-exception, etc.
	primitives	  ;set-exception-handler!, etc.
	wind		  ;CWCC
	util		  ;reduce used in xprim.scm
	generics
	;; low-level	  ;halt
	vm-exposure	  ;primitive-catch
	templates	  ;template-code, template-info
	continuations	  ;continuation-pc, etc.
	locations	  ;location?, location-id
	closures	  ;closure-template
	architecture
	number-i/o)       ; number->string, for backtrace
  (files (rts exception)  ; Needs generic, arch
	 (rts xprim)))	  ; Needs arch, exceptions.  Support (+ x y z), etc.

(define-package ((interrupts interrupts-interface))
  (open scheme-level-1
	signals fluids conditions
	bitwise
	primitives
	architecture)
  (files (rts interrupt))
  (optimize auto-integrate)) ;mostly for threads package...

(define-package ((writing writing-interface))
  (open scheme-level-1
	number-i/o
	ports				;output-port-option, write-string
	generics)			;disclose
  (files (rts write)))
	 
(define-package ((reading reading-interface))
  (open scheme-level-1
	number-i/o
	ports		;input-port-option
	ascii		;for dispatch table
	signals		;warn, signal-condition, make-condition
	conditions	;define-condition-type
	silly)		;really-string->symbol, reverse-list->string
  (files (rts read)))

(define-package ((scheme-level-2 scheme-level-2-interface))
  (open scheme-level-1
	number-i/o
	writing
	reading
	wind
	ports))

(define-package ((scheme-level-2-internal
		  (export usual-resumer)))
  (open scheme-level-1
	ports		;with-initial-ports
	fluids-internal	;initialize-dynamic-state!
	exceptions	;initialize-exceptions!
	interrupts)	;initialize-interrupts!
  (files (rts init)))



; Weak pointers & populations

(define-package ((weak weak-interface))
  (open scheme-level-1 signals
	primitives)	;Open primitives instead of loading (alt weak)
  (files ;;(alt weak)   ;Only needed if VM's weak pointers are buggy
	 (rts population)))


; Utility for displaying error messages

(define-package ((display-conditions display-conditions-interface))
  (open scheme-level-2
	writing
	generics
	handle)			;ignore-errors
  (files (env dispcond)))
