; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Additional packages are in s48-package-defs.scm (for loading into
; Scheme 48) and ps-package-defs.scm (for compiling as Pre-Scheme).

; The VM

(define-structure vm vm-interface
  (open prescheme ps-receive vm-architecture vm-utilities
	data struct
	interpreter interpreter-internal
	stack gc interpreter-gc
	vmio
	arithmetic-opcodes
	external-opcodes
	symbols
	io-opcodes
	images)
  (files resume)
  (begin
    (define (s48-disable-interrupts!)
      (disable-interrupts!))

    (define (s48-enable-interrupts!)
      (enable-interrupts!))

    ; used for raising exceptions in external code
    (define (s48-push x)
      (push x))
    
    (define (s48-stack-ref i)
      (stack-ref i))
    
    (define (s48-stack-set! x v)
      (stack-set! x v))))

;----------------
; The interpreter

(define-structure vm-architecture vm-architecture-interface
  (open prescheme)
  (files arch))

(define-structures ((interpreter interpreter-interface)
		    (interpreter-internal interpreter-internal-interface))
  (open prescheme ps-receive vm-utilities vm-architecture enum-case
	events
	memory data stob struct allocation vmio
	interpreter-gc gc
	heap stack environment external)
  (for-syntax (open scheme destructuring signals))
  (files interp call define-primitive prim interrupt)
  ;(optimize auto-integrate)
  )

; Assorted additional opcodes

(define-structure arithmetic-opcodes (export)
  (open prescheme interpreter-internal 
	data struct
	fixnum-arithmetic)
  (files fixnum-op))

(define-structure external-opcodes external-opcodes-interface
  (open prescheme vm-architecture ps-receive
	interpreter-internal stack
	memory data struct
	gc interpreter-gc
	string-tables
	external)
  (files external-call))

(define-structure symbols (export s48-copy-symbol-table
				  install-symbols!+gc)
  (open prescheme vm-utilities vm-architecture
	interpreter-internal
	memory heap data struct string-tables
	gc interpreter-gc)
  (files symbol))

(define-structure io-opcodes (export)
  (open prescheme vm-utilities vm-architecture ps-receive
	interpreter-internal
	channel-io vmio
	memory data struct
	images interpreter-gc
	symbols external-opcodes
	stack			;pop
	stob)			;immutable
  (files prim-io))

; The stack and lexical evironments

(define-structures ((stack stack-interface)
		    (initialize-stack (export initialize-stack+gc))
		    (environment environment-interface))
  (open prescheme vm-utilities ps-receive ps-memory
	vm-architecture memory data stob struct
	allocation
	interpreter-gc)
  ;(optimize auto-integrate)
  (files stack env))

;----------------
; Data structures

(define-structure memory memory-interface
  (open prescheme ps-memory vm-utilities)
  ;(optimize auto-integrate)
  (files memory))

(define-structure data data-interface
  (open prescheme vm-utilities
	system-spec vm-architecture memory)
  ;(optimize auto-integrate)
  (files data))

(define-structure stob stob-interface
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data allocation)
  ;(optimize auto-integrate)
  (files stob))

(define-structure struct struct-interface
  (open prescheme vm-utilities
	vm-architecture memory data stob allocation)
  (for-syntax (open scheme vm-architecture destructuring))
  ;(optimize auto-integrate)
  (files defdata struct))

;----------------
; Memory management

(define-structures ((allocation allocation-interface)
		    (heap heap-interface)
		    (heap-gc/image heap-gc/image-interface)
		    (heap-init heap-init-interface))
  (open prescheme ps-receive vm-utilities vm-architecture memory data)
  (files heap))

(define-structures ((gc gc-interface)
		    (image-gc (export begin-writing-image
				      abort-collection
				      s48-undumpable-records)))
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data struct
	heap heap-gc/image allocation)
  (files gc))

(define-structure images images-interface
  (open prescheme vm-utilities ps-receive vm-architecture
	memory data
	heap heap-gc/image
	gc image-gc
	vmio			;mark-traced-channels-closed!
	symbols			;copy-symbol-table
	external-opcodes)	;exported-bindings cleaned-imported-bindings
  (files image))

;----------------
; GC and allocation utilities for the interpreter.

(define-structure interpreter-gc interpreter-gc-interface
  (open prescheme ps-receive vm-utilities
	data
	gc
	allocation)
  (files interp-gc))

; Registering and tracing external GC roots.

(define-structure external-gc-roots external-gc-roots-interface
  (open prescheme ps-memory
	memory stob
	gc interpreter-gc)
  (files gc-root))

;----------------
; Low level stuff.

(define-structure vmio vmio-interface
  (open prescheme ps-receive channel-io vm-utilities
	data stob struct allocation memory
	interpreter-gc		;ensure-space
	vm-architecture)	;port-status
  ;(optimize auto-integrate)
  (files vmio))

(define-structure fixnum-arithmetic fixnum-arithmetic-interface
  (open prescheme vm-utilities data
	memory)  ; bits-per-cell
  ;(optimize auto-integrate)
  (files arith))

(define-structure string-tables string-table-interface
  (open prescheme vm-utilities
	data struct
	interpreter-gc gc)
  (files vm-tables))

(define-structure enum-case (export (enum-case :syntax))
  (open prescheme)
  (begin
    (define-syntax enum-case
      (syntax-rules (else)
	((enum-case enumeration (x ...) clause ...)
	 (let ((temp (x ...)))
	   (enum-case enumeration temp clause ...)))
	((enum-case enumeration value ((name ...) body ...) rest ...)
	 (if (or (= value (enum enumeration name)) ...)
	     (begin body ...)
	     (enum-case enumeration value rest ...)))
	((enum-case enumeration value (else body ...))
	 (begin body ...))
	((enum-case enumeration value)
	 (unspecific))))))

