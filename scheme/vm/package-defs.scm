
; The VM

(define-structure vm vm-interface
  (open prescheme ps-receive vm-architecture vm-utilities
	external 
	bignum-low
	integer-arithmetic
	flonum-arithmetic
	data struct
	interpreter interpreter-internal
	stack gc interpreter-gc gc-util
	vmio
	arithmetic-opcodes
	external-opcodes
	symbols
	io-opcodes
	external-gc-roots
	proposal-opcodes
	read-image
	return-codes)
  (files (interp resume))
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
    
    ; arguments must either both be intergers or both floanums
    (define (s48-integer-or-floanum-add x y)
      (if (double? x)
	  (flonum-add x y)
	  (integer-add x y)))
    (define (s48-integer-or-floanum-sub x y)
      (if (double? x)
	  (flonum-subtract x y)
	  (integer-subtract x y)))

    (define (s48-integer-or-floanum-mul x y)
      (if (double? x)
	  (flonum-multiply x y)
	  (integer-multiply x y)))

    (define-syntax define-integer-or-floanum-comparison
      (syntax-rules ()
	((define-integer-or-floanum-comparison 
	   s48-integer-or-floanum-proc integer-op flonum-op)
	 (define (s48-integer-or-floanum-proc x y)
	   (enter-boolean
	    (if (double? x)
		(flonum-op x y)
		(integer-op x y)))))))

    (define-integer-or-floanum-comparison s48-integer-or-floanum-= integer= flonum=)
    (define-integer-or-floanum-comparison s48-integer-or-floanum-< integer< flonum<)
    (define-integer-or-floanum-comparison s48-integer-or-floanum-> integer> flonum>)
    (define-integer-or-floanum-comparison s48-integer-or-floanum-<= integer<= flonum<=)
    (define-integer-or-floanum-comparison s48-integer-or-floanum->= integer>= flonum>=)

    (define (s48-integer-bitwise-not x)
      (integer-bitwise-not x))
    (define (s48-integer-bit-count x)
      (integer-bit-count x))
    (define (s48-integer-bitwise-and x y)
      (integer-bitwise-and x y))
    (define (s48-integer-bitwise-ior x y)
      (integer-bitwise-ior x y))
    (define (s48-integer-bitwise-xor x y)
      (integer-bitwise-xor x y))
    ))

; Byte code architecture.

(define-structure vm-architecture vm-architecture-interface
  (open prescheme)
  (files (interp arch)))

;----------------------------------------------------------------
; The interpreter.

(define-structures ((interpreter interpreter-interface)
		    (interpreter-internal interpreter-internal-interface))
  (open prescheme ps-receive vm-utilities vm-architecture enum-case
	events
	pending-interrupts
	memory data stob struct allocation vmio
	return-codes
	gc-roots gc gc-util
	heap stack external)
  (for-syntax (open scheme destructuring signals))
  (files (interp interp)
	 (interp call)
	 (interp define-primitive)
	 (interp prim)
	 (interp interrupt))
  ;(optimize auto-integrate)
  )

(define-structure pending-interrupts (export pending-interrupts-empty?
					     pending-interrupts-remove!
					     pending-interrupts-add!
					     pending-interrupts-clear!
					     pending-interrupts-mask
					     interrupt-bit)
  (open prescheme)
  (begin
    (define *pending-interrupts*)	; bitmask of pending interrupts
					     
    (define (pending-interrupts-add! interrupt-bit)
      (set! *pending-interrupts*
	    (bitwise-ior *pending-interrupts* interrupt-bit)))

    (define (pending-interrupts-remove! interrupt-bit)
      (set! *pending-interrupts*
	    (bitwise-and *pending-interrupts*
			 (bitwise-not interrupt-bit))))
    (define (pending-interrupts-clear!)
      (set! *pending-interrupts* 0))
    
    (define (pending-interrupts-empty?)
      (= *pending-interrupts* 0))

    (define (pending-interrupts-mask)
      *pending-interrupts*)

    ; Return a bitmask for INTERRUPT.

    (define (interrupt-bit interrupt)
      (shift-left 1 interrupt))
    ))

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
  (files (arith integer-op)))

(define-structure external-opcodes external-opcodes-interface
  (open prescheme vm-architecture ps-receive
	interpreter-internal stack
	memory data struct
	gc gc-roots gc-util
	string-tables
	external)
  (files (interp external-call)))

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
  (files (interp prim-io)))

(define-structure proposal-opcodes (export initialize-proposals!+gc)
  (open prescheme vm-utilities vm-architecture ps-receive
	interpreter-internal
	memory data struct
	gc-util
	stob
	external	;get-proposal-lock! release-proposal-lock!
	gc		;s48-trace-value
	gc-roots)	;add-gc-root!
  (files (interp proposal)))

(define-structures ((stack stack-interface)
		    (initialize-stack (export initialize-stack+gc)))
  (open prescheme vm-utilities ps-receive ps-memory
	vm-architecture memory data stob struct
	return-codes
	allocation
	gc-roots gc
	heap)		; for debugging function STACK-CHECK
  ;(optimize auto-integrate)
  (files (interp stack)
	 (interp stack-gc)))

(define-structure vmio vmio-interface
  (open prescheme ps-receive channel-io vm-utilities
	data stob struct allocation memory
	pending-interrupts
	vm-architecture)	;port-status
  ;(optimize auto-integrate)
  (files (interp vmio)))

; The VM needs return pointers for a few special continuations (bottom-of-stack,
; exceptions frame, and interrupt frames).  These have to have the correct data
; format.

(define-structure return-codes (export make-return-code
				       s48-make-blank-return-code
				       return-code-size
				       return-code-pc)
  (open prescheme vm-architecture struct)
  (begin
    (define return-code-pc 11)

    ;; Number of entries of the code vector
    (define blank-return-code-count 13)
    (define (make-return-code-count opcode-count)
      (+ blank-return-code-count opcode-count))
    (define first-opcode-index 13)

    ;; value for VM
    (define return-code-count (make-return-code-count 1))

    ;; Size in bytes of the return code frame
    (define (make-return-code-size return-code-count)
      (code-vector-size return-code-count))

    ;; value for VM
    (define return-code-size (make-return-code-size return-code-count))
    
    ;; procedure for VM
    (define (make-return-code protocol opcode frame-size key)
      (let ((blank-return-code (make-blank-return-code protocol frame-size 1 key)))
	(code-vector-set! blank-return-code first-opcode-index opcode)
	blank-return-code))

    (define (make-blank-return-code protocol frame-size opcode-count key)
      (let ((code (make-code-vector (make-return-code-count opcode-count) key)))
	; A whole lot of stuff to make the GC and disassembler happy.
	(code-vector-set! code 0 (enum op protocol))
	(code-vector-set! code 1 0)	; no arguments       - for disassembler
	(code-vector-set! code 2 #b00)	; no env or template - for disassembler
	(code-vector-set! code 3 (enum op cont-data))	;    - etc.
	(code-vector-set! code 4 0)
	(code-vector-set! code 5 8)		; low byte of return offset
	(code-vector-set! code 6 0)		; high byte of offset
	(code-vector-set! code 7 return-code-pc); low byte of offset
	(code-vector-set! code 8 0)		; GC mask - all pointers
	(code-vector-set! code 9 (high-byte frame-size))
	(code-vector-set! code 10 (low-byte frame-size))
	(code-vector-set! code 11 (enum op protocol))
	(code-vector-set! code 12 protocol)
	code))

    (define (s48-make-blank-return-code protocol frame-size opcode-count)
      (make-blank-return-code protocol 
			      frame-size 
			      opcode-count
			      (ensure-space (make-return-code-size 
					     (make-return-code-count opcode-count)))))

    (define (high-byte n)
      (low-byte (arithmetic-shift-right n 8)))
    (define (low-byte n)
      (bitwise-and n #xFF))))

;----------------------------------------------------------------
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
	memory data
	gc gc-roots)
  (files (heap gc-root)))

;----------------------------------------------------------------
; Data structures

(define-structure data vm-data-interface
  (open prescheme vm-utilities
	system-spec vm-architecture)
  ;(optimize auto-integrate)
  (files (data data)))

(define-structure memory memory-interface
  (open prescheme ps-memory vm-utilities data)
  ;(optimize auto-integrate)
  (files (data memory)))

(define-structure stob stob-interface
  (open prescheme ps-receive vm-utilities vm-architecture
	memory heap data allocation)
  ;(optimize auto-integrate)
  (files (data stob)))

(define-structure struct struct-interface
  (open prescheme vm-utilities
	vm-architecture memory data stob allocation)
  (for-syntax (open scheme vm-architecture destructuring))
  ;(optimize auto-integrate)
  (files (data defdata)
	 (data struct)))

(define-structure string-tables string-table-interface
  (open prescheme vm-utilities vm-architecture
	data struct stob)
  (files (data vm-tables)))

(define-structure symbols (export s48-symbol-table
				  install-symbols!+gc)
  (open prescheme vm-utilities vm-architecture
	interpreter-internal
	memory heap data struct string-tables
	gc gc-roots)
  (files (data symbol)))

;----------------------------------------------------------------
; Memory management

(define-structures ((heap heap-interface)
		    (heap-gc-util heap-gc-util-interface)
		    (heap-init heap-init-interface))
  (open prescheme ps-receive vm-utilities vm-architecture memory data
	ps-memory)
  (files (heap heap)))

(define-structure gc gc-interface
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data
	heap heap-gc-util
	interpreter-gc)
  (files (heap gc)))

; This should be in heap.scm except that it needs GC and GC needs HEAP,
; so we have to put this in its own package to avoid a dependency loop.

(define-structure gc-static-hack (export)
  (open prescheme gc heap-gc-util gc-roots)
  (begin
    (add-gc-root! (lambda ()
		    (walk-impure-areas
		     (lambda (start end)
		       (s48-trace-locations! start end)
		       #t))))))

(define-structure allocation allocation-interface
  (open prescheme memory heap-gc-util gc data vm-architecture
	gc-static-hack)
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

;----------------------------------------------------------------
; Reading and writing images

(define-structure read-image read-image-interface
  (open prescheme vm-utilities ps-receive vm-architecture
	memory data struct
	string-tables
	heap-init		;s48-initialize-heap
	gc)			;s48-trace-value
  (files (heap read-image)))

(define-structure write-image (export s48-write-image)
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data struct
	heap
	image-table
	image-util
	string-tables
	symbols			;s48-symbol-table
	external-opcodes)	;s48-imported-bindings s48-exported-bindings
  (files (heap write-image)))

(define-interface image-table-interface
  (export make-image-location
          image-location-new-descriptor
          image-location-next
          set-image-location-next!

          make-table
	  deallocate-table
	  break-table!
	  table-okay?
          table-set!
          table-ref))

(define-structure image-table image-table-interface
  (open prescheme ps-memory ps-record-types
	vm-utilities)
  (files (heap image-table)))

(define-interface image-util-interface
  (export write-page
	  (write-check :syntax)
	  write-header-integer
	  image-write-init
	  image-write-terminate
	  image-write-status
	  write-descriptor
	  write-image-block
	  empty-image-buffer!))

(define-structure image-util image-util-interface
  (open prescheme ps-memory
	(subset memory	(address1+)))
  (files (heap image-util)))

;----------------------------------------------------------------
; Arithmetic

(define-structure fixnum-arithmetic fixnum-arithmetic-interface
  (open prescheme vm-utilities data
	memory)  ; bits-per-cell
  ;(optimize auto-integrate)
  (files (arith arith)))

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
  (files (arith bignum-low)))

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
  (files (arith bignum-arith)))

(define-structure integer-arithmetic integer-arithmetic-interface
  (open prescheme 
	fixnum-arithmetic
	bignum-arithmetic
	external
	bignum-low
	struct
	data)
  (files (arith integer)))

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
  (files (arith flonum-arith)))

;----------------------------------------------------------------
; Random utility

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

