; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Additional packages are in s48-package-defs.scm (for loading into
; Scheme 48) and ps-package-defs.scm (for compiling as Pre-Scheme).

; The VM

(define-structure vm vm-interface
  (open prescheme ps-receive vm-architecture vm-utilities
	external 
	bignum-low
	integer-arithmetic
	data struct
	interpreter interpreter-internal
	stack gc interpreter-gc gc-util
	vmio
	arithmetic-opcodes
	external-opcodes
	symbols
	environment		; *env*
	io-opcodes
	external-gc-roots
	proposal-opcodes
	read-image)
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
      (stack-set! x v))
    
    (define (s48-enter-integer x)
      (enter-integer x (ensure-space long-as-integer-size)))
    ))

;----------------
; The interpreter

; Byte code architecture.

(define-structure vm-architecture vm-architecture-interface
  (open prescheme)
  (files arch))

; The interpreter itself.

(define-structures ((interpreter interpreter-interface)
		    (interpreter-internal interpreter-internal-interface))
  (open prescheme ps-receive vm-utilities vm-architecture enum-case
	events
	memory data stob struct allocation vmio
	gc-roots gc gc-util
	heap stack environment external)
  (for-syntax (open scheme destructuring signals))
  (files interp call define-primitive prim interrupt)
  ;(optimize auto-integrate)
  )

; Assorted additional opcodes

(define-structure arithmetic-opcodes (export)
  (open prescheme interpreter-internal 
	interpreter-gc
	data struct
	fixnum-arithmetic
	vm-architecture
	bignum-arithmetic
	flonum-arithmetic
	integer-arithmetic)
  (files integer-op))

(define-structure external-opcodes external-opcodes-interface
  (open prescheme vm-architecture ps-receive
	interpreter-internal stack
	memory data struct
	gc gc-roots gc-util
	string-tables
	external)
  (files external-call))

(define-structure symbols (export s48-symbol-table
				  install-symbols!+gc)
  (open prescheme vm-utilities vm-architecture
	interpreter-internal
	memory heap data struct string-tables
	gc gc-roots)
  (files symbol))

(define-structure io-opcodes (export)
  (open prescheme vm-utilities vm-architecture ps-receive
	interpreter-internal
	channel-io vmio
	memory data struct
	read-image write-image
	gc-roots
	symbols external-opcodes
	stack			;pop
	stob)			;immutable
  (files prim-io))

(define-structure proposal-opcodes (export initialize-proposals!+gc)
  (open prescheme vm-utilities vm-architecture ps-receive
	interpreter-internal
	memory data struct
	gc-util
	stob
	external	;get-proposal-lock! release-proposal-lock!
	gc		;s48-trace-value
	gc-roots)	;add-gc-root!
  (files proposal))

; The stack and lexical evironments

(define-structures ((stack stack-interface)
		    (initialize-stack (export initialize-stack+gc))
		    (environment environment-interface))
  (open prescheme vm-utilities ps-receive ps-memory
	vm-architecture memory data stob struct
	allocation
	gc-roots)
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
	memory heap data allocation)
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

(define-structures ((heap heap-interface)
		    (heap-gc-util heap-gc-util-interface)
		    (heap-init heap-init-interface))
  (open prescheme ps-receive vm-utilities vm-architecture memory data
	ps-memory)
  (files heap))

(define-structure gc gc-interface
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data
	heap heap-gc-util
	interpreter-gc)
  (files gc))

(define-structure allocation allocation-interface
  (open prescheme memory heap-gc-util gc data vm-architecture)
  (begin

    (define (s48-make-available+gc len)
      (if (not (bytes-available? len))
	  (s48-collect))
      (if (not (bytes-available? len))
	  (error "Scheme 48 heap overflow")))

    (define s48-allocate-small allocate)

    (define (s48-allocate-traced+gc len)
      (if (not (bytes-available? len))
	  (s48-collect))
      (if (not (bytes-available? len))
	  null-address
	  (allocate len)))

    ; Same again.  Just doing (define x y) for exported procedures X and Y
    ; causes the Pre-Scheme compiler to emit bad code.
    (define (s48-allocate-untraced+gc len)
      (s48-allocate-traced+gc len))

    ; For allocation done outside the VM.

    (define (s48-allocate-stob type size)
      (let* ((traced? (< type least-b-vector-type))
	     (length-in-bytes (if traced?
				  (cells->bytes size)
				  size))
	     (needed (+ length-in-bytes (cells->bytes stob-overhead)))
	     (thing (if traced?
			(s48-allocate-traced+gc needed)
			(s48-allocate-untraced+gc needed))))
	(if (null-address? thing)
	    (error "insufficient heap space for external allocation")
	    (begin
	      (store! thing (make-header type length-in-bytes))
	      (address->stob-descriptor (address+ thing
						  (cells->bytes stob-overhead)))))))

  ))

(define-structure read-image read-image-interface
  (open prescheme vm-utilities ps-receive vm-architecture
	memory data struct
	string-tables
	heap-init		;s48-initialize-heap
	gc)			;s48-trace-value
  (files read-image))

(define-structure write-image (export s48-write-image)
  (open prescheme vm-utilities vm-architecture
	memory data
	image-gc
	vmio			;s48-channels s48-channel-count
	symbols			;s48-symbol-table
	external-opcodes)	;s48-imported-bindings s48-exported-bindings
  (files write-image))

(define-structure image-gc image-gc-interface
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data struct
	heap
	string-tables)
  (files image-gc))

;----------------
; GC and allocation utilities for the interpreter.

(define-structures ((interpreter-gc interpreter-gc-interface)
		    (gc-roots gc-roots-interface))
  (open prescheme)
  (begin
    ; GC-ROOT and POST-GC-CLEANUP are defined incrementally.
    ;
    ; (ADD-GC-ROOT! <thunk>)           ; call <thunk> when tracing the GC roots
    ; (ADD-POST-GC-CLEANUP! <thunk>)   ; call <thunk> when a GC has finished
    ;
    ; (S48-GC-ROOT)                    ; call all the root thunks
    ; (S48-POST-GC-CLEANUP)            ; call all the cleanup thunks
    
    (define-syntax define-extensible-proc
      (syntax-rules ()
	((define-extensible-proc proc extender temp)
	 (begin
	   (define temp unspecific)
	   (define (proc) (temp))
	   (define (extender more)
	     (let ((old temp))
	       (set! temp (lambda ()
			(more)
			(old)))))))))

    (define-extensible-proc s48-gc-root
      add-gc-root!
      *gc-root-proc*)

    (define-extensible-proc s48-post-gc-cleanup
      add-post-gc-cleanup!
      *post-gc-cleanup*)))

(define-structure gc-util gc-util-interface
  (open prescheme data gc gc-roots)
  (begin
    (define *temp0* false)
    (define *temp1* false)

    (add-gc-root! (lambda ()
		    (set! *temp0* (s48-trace-value *temp0*))
		    (set! *temp1* (s48-trace-value *temp1*))))

    (define (save-temp0! value)
      (set! *temp0* value))

    (define (recover-temp0!)
      (let ((value *temp0*))
	(set! *temp0* false)
	value))

    (define (save-temp1! value)
      (set! *temp1* value))

    (define (recover-temp1!)
      (let ((value *temp1*))
	(set! *temp1* false)
	value))))

; Registering and tracing external GC roots.

(define-structure external-gc-roots external-gc-roots-interface
  (open prescheme ps-memory
	memory stob
	gc gc-roots)
  (files gc-root))

;----------------
; Low level stuff.

(define-structure vmio vmio-interface
  (open prescheme ps-receive channel-io vm-utilities
	data stob struct allocation memory
	vm-architecture)	;port-status
  ;(optimize auto-integrate)
  (files vmio))

(define-structure fixnum-arithmetic fixnum-arithmetic-interface
  (open prescheme vm-utilities data
	memory)  ; bits-per-cell
  ;(optimize auto-integrate)
  (files arith))

(define-structure bignum-low bignum-low-interface
  (open prescheme 
	vm-utilities
	stob
	gc
	struct memory
	vm-architecture
	external
	interpreter-gc
	data)
  (files bignum-low))

(define-structure bignum-arithmetic bignum-arithmetic-interface
  (open prescheme
	vm-utilities
	external
	struct
	ps-receive
	interpreter-internal
	data
	system-spec
	gc-util
	bignum-low)
  (files bignum-arith))

(define-structure integer-arithmetic integer-arithmetic-interface
  (open prescheme 
	fixnum-arithmetic
	bignum-arithmetic
	external
	bignum-low
	struct
	data)
  (files integer))

(define-structure flonum-arithmetic (export flonum-add
					    flonum-subtract
					    flonum-multiply
					    flonum-divide
					    flonum= flonum< flonum>
					    flonum<= flonum>=)
  (open prescheme 
	ps-memory
	ps-flonums
	gc-util
	data		; false
	struct)
  (files flonum-arith))

(define-structure string-tables string-table-interface
  (open prescheme vm-utilities vm-architecture
	data struct stob)
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

