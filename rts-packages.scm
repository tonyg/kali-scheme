; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Package definitions for runtime system (without compiler).

; Assumes interface definitions in interfaces.scm.

; "Level 1" - all the easy things.  There is no dependence on any
; low-level or unusual primitives.

(define-structures ((scheme-level-1 scheme-level-1-interface)
		    (util util-interface))
  (open scheme-level-0 ascii signals)
  (usual-transforms case delay quasiquote syntax-rules)
  (files (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

(define-structures ((records records-interface)
		    (records-internal records-internal-interface))
  (open scheme-level-1 signals
	primitives)
  (files (rts record))
  (optimize auto-integrate))

(define-structure bummed-define-record-types define-record-types-interface
  (open scheme-level-1 records records-internal loopholes)
  (files (rts bummed-jar-defrecord)))

(define-structures ((methods methods-interface)
		    (meta-methods meta-methods-interface))
  (open scheme-level-1
	bummed-define-record-types
	records records-internal
	bitwise util
	signals)
  (files (rts method))
  (optimize auto-integrate))

(define-structure number-i/o number-i/o-interface
  (open scheme-level-1 methods signals ascii)
  (files (rts numio)))

(define-structures ((fluids fluids-interface)
		    (fluids-internal fluids-internal-interface))
  (open scheme-level-1 bummed-define-record-types primitives)
  (files (rts fluid))
  (optimize auto-integrate))

(define-structure wind wind-interface
  (open scheme-level-1 signals bummed-define-record-types
	fluids fluids-internal
	escapes)
  (files (rts wind))
  (optimize auto-integrate))

(define-structure ports ports-interface
  (open scheme-level-1 signals fluids wind
	primitives)	;write-string, force-output
  (files (rts port))
  (optimize auto-integrate))

(define-structure enumerated enumerated-interface
  (open scheme-level-1 signals)
  (files ;; (rts enum)
	 (rts defenum scm)))

(define-structure architecture architecture-interface
  (open scheme-level-1 signals enumerated)
  (files (rts arch)))

(define-structure conditions conditions-interface
  (open scheme-level-1 signals)
  (files (rts condition)))

; Hairier stuff now.

(define-structure templates templates-interface
  (open scheme-level-1 primitives methods)
  (files (rts template)))

(define-structure continuations continuations-interface
  (open scheme-level-1 primitives templates methods)
  (files (rts continuation)))

(define-structure more-types (export :closure :code-vector :location :template)
  (open methods
	closures code-vectors locations templates)
  (begin (define-simple-type :closure     (:value) closure?)
	 (define-simple-type :code-vector (:value) code-vector?)
	 (define-simple-type :location    (:value) location?)
	 (define-simple-type :template    (:value) template?)))


(define-structures ((exceptions exceptions-interface)
		    (handle handle-interface))
  (open scheme-level-1
	signals ports fluids
	conditions	  ;make-exception, etc.
	primitives	  ;set-exception-handler!, etc.
	wind		  ;CWCC
	util		  ;reduce used in xprim.scm
	methods
	meta-methods
	more-types
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

(define-structure interrupts interrupts-interface
  (open scheme-level-1
	signals fluids conditions
	bitwise
	primitives
	architecture)
  (files (rts interrupt))
  (optimize auto-integrate)) ;mostly for threads package...

(define-structure writing writing-interface
  (open scheme-level-1
	number-i/o
	ports				;output-port-option, write-string
	methods)			;disclose
  (files (rts write)))
	 
(define-structure reading reading-interface
  (open scheme-level-1
	number-i/o
	ports		;input-port-option
	ascii		;for dispatch table
	signals		;warn, signal-condition, make-condition
	conditions	;define-condition-type
	silly)		;really-string->symbol, reverse-list->string
  (files (rts read)))

(define-structure scheme-level-2 scheme-level-2-interface
  (open scheme-level-1
	number-i/o
	writing
	reading
	wind
	ports))

(define-structure scheme-level-2-internal
    (export usual-resumer)
  (open scheme-level-1
	ports		;with-initial-ports
	fluids-internal	;initialize-dynamic-state!
	exceptions	;initialize-exceptions!
	interrupts)	;initialize-interrupts!
  (files (rts init)))



; Weak pointers & populations

(define-structure weak weak-interface
  (open scheme-level-1 signals
	primitives)	;Open primitives instead of loading (alt weak)
  (files ;;(alt weak)   ;Only needed if VM's weak pointers are buggy
	 (rts population)))


; Utility for displaying error messages

(define-structure display-conditions display-conditions-interface
  (open scheme-level-2
	writing
	methods
	handle)			;ignore-errors
  (files (env dispcond)))
