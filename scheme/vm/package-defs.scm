
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
	return-codes

	;; For debugging
	memory ;; fetch

	)
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
    
    (define (s48-integer-remainder x y)
      (integer-remainder x y))

    (define (s48-integer-quotient x y)
      (integer-quotient x y))

    (define (s48-integer-divide x y)
      (integer-divide x y))

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

    (define (s48-string-set s i c)
      (vm-string-set! s i c))

    (define (s48-string-ref s i)
      (vm-string-ref s i))

    (define (s48-string-length s)
      (vm-string-length s))

    (define (s48-allocate-string len)
      (vm-make-string+gc len))

    (define (s48-enter-string-latin-1 s)
      (enter-string+gc s))

    (define (s48-copy-string-to-scheme-string-latin-1 string len vm-string)
      (copy-string-to-vm-string/latin-1! string len vm-string))

    (define (s48-copy-scheme-string-to-string-latin-1 vm-string string)
      (copy-vm-string-to-string/latin-1! vm-string string))
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
	text-encodings
	return-codes
	gc-roots gc gc-util
	heap stack external)
  (for-syntax (open scheme destructuring signals))
  (files (interp interp)
	 (interp call)
	 (interp define-primitive)
	 (interp prim)
	 (interp interrupt)
	 )
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
	heap ; S48-GATHER-OBJECTS
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
	stob			;immutable
	text-encodings)
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
    (define return-code-pc 13)

    ;; Number of entries of the code vector
    (define blank-return-code-count 15)
    (define (make-return-code-count opcode-count)
      (+ blank-return-code-count opcode-count))
    (define first-opcode-index 15)

    ;; value for VM
    (define return-code-count (make-return-code-count 1))

    ;; Size in bytes of the return code frame
    (define (make-return-code-size return-code-count)
      (code-vector-size return-code-count))

    ;; value for VM
    (define return-code-size (make-return-code-size return-code-count))
    
    ;; procedure for VM
    (define (make-return-code protocol template opcode frame-size key)
      (let ((blank-return-code (make-blank-return-code protocol template frame-size 1 key)))
	(code-vector-set! blank-return-code first-opcode-index opcode)
	blank-return-code))

    (define (make-blank-return-code protocol template frame-size opcode-count key)
      (let ((code (make-code-vector (make-return-code-count opcode-count) key)))
	; A whole lot of stuff to make the GC and disassembler happy.
	(code-vector-set! code 0 (enum op protocol))
	(code-vector-set! code 1 protocol) 
	(code-vector-set! code 2 #b00)	; no env or template - for disassembler
	(code-vector-set! code 3 (enum op cont-data))	;    - etc.
	(code-vector-set! code 4 0)             ; high byte of size  
	(code-vector-set! code 5 8)		; low byte of size
                                                ; no mask
        (code-vector-set! code 6 (high-byte template))
        (code-vector-set! code 7 (low-byte template))
	(code-vector-set! code 8 0)		; high byte of offset
	(code-vector-set! code 9 return-code-pc); low byte of offset
	(code-vector-set! code 10 0)		; GC mask size
	(code-vector-set! code 11 (high-byte frame-size))
	(code-vector-set! code 12 (low-byte frame-size))
	(code-vector-set! code 13 (enum op protocol))
	(code-vector-set! code 14 protocol)
	code))

    (define (s48-make-blank-return-code protocol template frame-size opcode-count)
      (make-blank-return-code protocol
                              template
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
	((define-extensible-proc (proc arg ...) body-form extender temp)
	 (begin
	   (define (temp arg ...)
	     body-form
	     (unspecific))
	   (define (proc arg ...) (temp arg ...))
	   (define (extender more)
	     (let ((old temp))
	       (set! temp (lambda (arg ...)
			    (more arg ...)
			    (old arg ...)))))))))

    (define-extensible-proc (s48-gc-root)
      (unspecific)
      add-gc-root!
      *gc-root-proc*)

    (define-extensible-proc (s48-post-gc-cleanup major? in-trouble?)
      (begin
	(eq? major? #t) 
	(eq? in-trouble? #t))		; for the type checker
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
	memory heap data allocation debugging)
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
	data struct stob
	ps-memory            ; address->integer - BIBOP
	memory               ; address->stob-descriptor - BIBOP
        image-table          ; image-location-new-descriptor - BIBOP
	)
  (files (data vm-tables)))

(define-structure symbols (export s48-symbol-table
				  install-symbols!+gc)
  (open prescheme vm-utilities vm-architecture
	interpreter-internal
	memory heap data struct string-tables
	gc gc-roots)
  (files (data symbol)))

(define-structure text-encodings text-encodings-interface
  (open prescheme enum-case
	struct
	vm-architecture)
  (files (data text-encoding)))

;----------------------------------------------------------------
;; DUMPER
;----------------------------------------------------------------
;; Reading and writing images

;; The new READ-IMAGE uses a helper structure READ-IMAGE-KERNEL
(define-structure read-image read-image-interface
  (open prescheme enum-case ps-receive ps-memory
	debugging
	vm-utilities
	(subset vm-architecture (architecture-version))
	image-util
	read-image-gc-specific
	read-image-util
	data
	(subset memory (fetch))
	heap-init
	(subset gc (s48-trace-value)))
  (files (heap read-image)))

(define-structure read-image-portable read-image-portable-interface
  (open prescheme ps-receive enum-case
	vm-utilities vm-architecture
	memory 
	data struct
	(subset string-tables (relocate-table))
	ps-memory               ;allocate/deallocate-memory
	heap                    ;s48-heap-size
	image-table             ;make-table
	image-util
	heap-init
	read-image-util
	read-image-util-gc-specific
	)
 (files (heap read-image-portable)))

(define-structure write-image write-image-interface
  (open prescheme ps-receive enum-case
	vm-utilities vm-architecture
	memory data struct
	heap
	image-table
	image-util
	write-image-util
	string-tables
	symbols				;s48-symbol-table
	external-opcodes  ;s48-imported-bindings s48-exported-bindings
	ps-record-types			;define-record-type
	write-image-gc-specific
	)
  (files (heap write-image)))

(define-structure image-table image-table-interface
  (open prescheme ps-memory ps-record-types
	vm-utilities)
  (files (heap image-table)))

(define-structure image-util image-util-interface
  (open prescheme enum-case)
  (files (heap image-util)))

(define-structure read-image-util read-image-util-interface
  (open prescheme ps-receive
	memory
	(subset ps-memory (read-block address+ address<))
	(subset data (bytes->a-units b-vector-header? header-length-in-a-units stob?))
	vm-utilities
	(subset allocation (s48-allocate-traced+gc))
	(subset struct (vm-symbol-next
			vm-set-symbol-next!
			shared-binding-next
			set-shared-binding-next!))
	string-tables)
  (files (heap read-image-util)))

(define-structure write-image-util write-image-util-interface
  (open prescheme ps-memory
	(subset memory	(address1+)))
  (files (heap write-image-util)))

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


; Memory management
;
; These are dummies  to avoid warnings during compilation.
; The real modules are in each GC subdirectory (gc-twospace and gc-bibop)
; and will be loaded after this file.

;----------------------------------------------------------------

(define-structures ((heap heap-interface)
		    (heap-gc-util heap-gc-util-interface)
		    (heap-init heap-init-interface)
		    (gc gc-interface)
		    (allocation allocation-interface)
		    (read-image-gc-specific read-image-gc-specific-interface)
		    (read-image-util-gc-specific read-image-util-gc-specific-interface)
		    (write-image-gc-specific write-image-gc-specific-interface))
  (open)
  (files))


;; JUST FOR DEBUGGING:
;; To activate/deactivate it, the flag 'debug-mode?' must be set in
;; debugging.scm
(define-structure debugging debugging-interface
  (open prescheme vm-utilities)
  (files debugging))
