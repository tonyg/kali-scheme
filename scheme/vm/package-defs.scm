; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Additional packages are in s48-package-defs.scm (for loading into
; Scheme 48) and ps-package-defs.scm (for compiling as Pre-Scheme).

(define-structure memory memory-interface
  (open prescheme ps-memory vm-utilities)
  ;(optimize auto-integrate)
  (files memory))

(define-structure vm-architecture vm-architecture-interface
  (open prescheme)
  (files arch))

(define-structure data data-interface
  (open prescheme vm-utilities
	system-spec vm-architecture memory)
  ;(optimize auto-integrate)
  (files data))

(define-structures ((allocation allocation-interface)
		    (heap heap-interface)
		    (heap-init heap-init-interface))
  (open prescheme ps-receive vm-utilities vm-architecture memory data)
  (for-syntax (open scheme vm-architecture destructuring))
  ;(optimize auto-integrate)
  (files heap gc))

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

(define-structure vmio vmio-interface
  (open prescheme channel-io vm-utilities data stob struct allocation memory
	vm-architecture)  ; port-status
  ;(optimize auto-integrate)
  (files vmio))

(define-structures ((stack stack-interface)
		    (environment environment-interface))
  (open prescheme vm-utilities ps-receive ps-memory
	vm-architecture memory data stob struct allocation)
  ;(optimize auto-integrate)
  (files stack env))

(define-structure fixnum-arithmetic fixnum-arithmetic-interface
  (open prescheme vm-utilities data
	memory)  ; bits-per-cell
  ;(optimize auto-integrate)
  (files arith))

(define-structure interpreter interpreter-interface
  (open prescheme ps-receive vm-utilities vm-architecture enum-case
	events
	channel-io
	memory data stob struct allocation vmio
	heap stack environment external fixnum-arithmetic)
  (for-syntax (open scheme destructuring signals))
  (files interp call define-primitive prim interrupt prim-io resume)
  ;(optimize auto-integrate)
  )

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

