; Additional packages are in s48-package-defs.scm (for loading into
; Scheme 48) and ps-package-defs.scm (for compiling as Pre-Scheme).

(define-structures ((memory memory-interface))
  (open pre-scheme vm-utilities)
  (files memory))

(define-structures ((vm-architecture vm-architecture-interface))
  (open pre-scheme ps-enumerated)
  (files arch))

(define-structures ((data data-interface))
  (open pre-scheme vm-utilities ps-enumerated
	system-spec vm-architecture memory)
  (files data))

(define-structures ((heap heap-interface)
		    (stob stob-interface))
  (open pre-scheme vm-utilities vm-architecture memory data)
  (files heap stob gc))

(define-structures ((struct struct-interface))
  (open pre-scheme vm-utilities
	vm-architecture memory data stob)
  (for-syntax (open scheme vm-architecture destructuring))
  (files struct))

(define-structures ((vmio vmio-interface))
  (open pre-scheme vm-utilities data stob struct)
  (files vmio))

(define-structures ((stack stack-interface)
		    (environment environment-interface))
  (open pre-scheme vm-utilities ps-enumerated
	vm-architecture memory data stob struct)
  (files stack env))

(define-structures ((fixnum-arithmetic fixnum-arithmetic-interface))
  (open pre-scheme vm-utilities data
	memory)  ; bits-per-cell
  (files arith))

(define-structures ((interpreter interpreter-interface))
  (open pre-scheme vm-utilities vm-architecture ps-enumerated
	memory data stob struct vmio
	heap stack environment external fixnum-arithmetic)
  (for-syntax (open scheme destructuring))
  (files interp define-primitive prim resume)  ; disasm if run in Scheme
  (begin
    (define (write-instruction template pc) 0))) ; stub

; From Scheme 48 (for the benefit of the Pre-Scheme compiler)
(define-structure ps-enumerated ps-enumerated-interface
  (open pre-scheme)
  (files defenum))

; For debugging

(define-structures ((vm-disassembler
		       (export disassemble write-instruction)))
  (open scheme
	struct
        enumerated              ; enumerand->name
        vm-architecture
	bitwise                 ; arithmetic-shift
	signals)                ; error
  (files disasm))

