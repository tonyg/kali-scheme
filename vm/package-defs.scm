; Additional packages are in s48-package-defs.scm (for loading into
; Scheme 48) and ps-package-defs.scm (for compiling as Pre-Scheme).

; Copied from Scheme 48
(define-structures ((enumerated (export define-enumeration enum)))
  (open pre-scheme)
  (files defenum))

(define-structures ((memory memory-interface))
  (open pre-scheme vm-utilities)
  (files memory))

(define-structures ((vm-architecture vm-architecture-interface))
  (open pre-scheme enumerated)
  (files arch))

(define-structures ((data data-interface))
  (open pre-scheme vm-utilities enumerated
	system-spec vm-architecture memory)
  (files data))

(define-structures ((heap heap-interface)
		 (stob stob-interface))
  (open pre-scheme vm-utilities vm-architecture memory data)
  (files heap stob gc))

(define-structures ((struct struct-interface))
  (open pre-scheme vm-utilities
	vm-architecture memory data stob)
  (for-syntax (open scheme vm-architecture
		    big-scheme)) ; for destructure
  (files struct))

(define-structures ((vmio vmio-interface))
  (open pre-scheme vm-utilities data stob struct)
  (files vmio))

(define-structures ((stack stack-interface)
		 (environment environment-interface))
  (open pre-scheme vm-utilities vm-architecture memory data stob struct)
  (files stack env))

(define-structures ((fixnum-arithmetic fixnum-arithmetic-interface))
  (open pre-scheme vm-utilities data)
  (files arith))

(define-structures ((interpreter (export resume do-gc)))
  (open pre-scheme vm-utilities vm-architecture enumerated
	memory data stob struct vmio
	heap stack environment external fixnum-arithmetic)
  (for-syntax (open scheme
		    big-scheme)) ; for destructure
  (files interp define-primitive prim resume)  ; disasm if run in Scheme
  (begin
    (define (write-instruction template pc) 0))) ; stub
