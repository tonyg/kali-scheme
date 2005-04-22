; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; Packages for two-space copier GC
;----------------------------------------------------------------
; Memory management

(define-interface heap-two-space-interface
  (export heap-begin

	  set-heap-pointer!
	  heap-pointer
	  swap-spaces
	  heap-limit
	  allocate

	  get-new-heap-start-addr

	  in-trouble?))

(define-structures ((heap heap-interface)
		    (heap-gc-util heap-gc-util-interface)
		    (heap-init heap-init-interface)
		    (heap-two-space heap-two-space-interface))
  (open prescheme ps-receive vm-utilities vm-architecture memory data
	ps-memory
	debugging)
  (files (gc-twospace heap)))

(define-structure gc gc-interface
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data
	heap heap-gc-util
	heap-two-space
	interpreter-gc
	debugging)
  (files (gc-twospace gc)
	 (heap trace-continuation)))

(define-structure allocation allocation-interface
  (open prescheme memory
	heap-gc-util heap-two-space
	gc
	gc-static-hack
	data
	vm-architecture
	debugging
	vm-utilities
	)
  (files (gc-twospace allocation)))


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

; Image handling

(define-structure read-image-gc-specific read-image-gc-specific-interface
  (open prescheme ps-receive enum-case
	vm-utilities vm-architecture
	memory 
	data struct
	ps-memory               ;allocate/deallocate-memory
	heap                    ;s48-heap-size
	heap-init		;s48-initialize-heap
     	                        ;s48-initialize-image-areas
     	                        ;s48-check-heap-size
	heap-two-space
	image-util
	image-table             ;make-table
	read-image-util
	read-image-portable
	)
 (files (gc-twospace read-image)))

(define-structure read-image-util-gc-specific read-image-util-gc-specific-interface
  (open prescheme
	ps-memory
	read-image-util)
  (begin
    (define (get-small-start-addr heap-image-pointer)
      (address+ heap-image-pointer
		(address-difference (get-weaks-img-end-addr)
				    (get-large-img-start-addr))))
    (define (get-large-start-addr heap-image-pointer)
      (address+ heap-image-pointer
		(address-difference (get-weaks-img-end-addr)
				    (get-weaks-img-start-addr))))
    
    (define (get-weaks-start-addr heap-image-pointer)
      heap-image-pointer)))

(define-structure write-image-gc-specific write-image-gc-specific-interface
  (open prescheme ps-receive
	vm-utilities vm-architecture
	memory data struct
	heap
	heap-two-space
	image-table
	image-util
	write-image-util
	string-tables
	symbols				;s48-symbol-table
	external-opcodes  ;s48-imported-bindings s48-exported-bindings
	)
  (files (gc-twospace write-image)))

