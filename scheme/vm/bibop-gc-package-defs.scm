; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; Packages for BIBOP GC
;----------------------------------------------------------------
; Memory management

(define-interface heap-bibop-interface
  (export s48-set-max-heap-size!
	  s48-max-heap-size

	  s48-set-new-small-start-addr!
	  s48-get-new-small-start-addr

	  s48-set-new-large-start-addr!
	  s48-get-new-large-start-addr

	  s48-set-new-weaks-start-addr!
	  s48-get-new-weaks-start-addr))

(define-structures ((heap heap-interface)
		    (heap-gc-util heap-gc-util-interface)
		    (heap-init heap-init-interface)
		    (heap-bibop heap-bibop-interface))
  (open prescheme ps-receive vm-utilities vm-architecture memory data
	ps-memory
	debugging
	bibop-gc-external)
  (files (gc-bibop heap)))

(define-structures ((gc gc-interface)
		    (gc-bibop-util (export s48-trace-continuation)))
  (open prescheme ps-receive vm-utilities vm-architecture
	memory data
	heap heap-gc-util
	interpreter-gc
	bibop-gc-external)
  (files (gc-bibop gc)
	 (heap trace-continuation)))

(define-structure allocation allocation-interface
  (open prescheme memory heap-gc-util gc data vm-architecture
	gc-static-hack
	bibop-gc-external)
  (files (gc-bibop allocation)))


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

;; These are the things defined in c/bibop/*

(define-structure bibop-gc-external
  (export s48-make-available+gc
	  s48-allocate-small
	  s48-allocate-traced+gc
	  s48-allocate-untraced+gc
	  s48-allocate-untraced-unmovable+gc
	  s48-allocate-weak+gc
	  s48-forbid-gc!
	  s48-allow-gc!

	  s48-collect
	  s48-trace-value
	  s48-trace-locations!
	  s48-trace-stob-contents!
	  s48-extant?
	  s48-gc-count

	  s48-write-barrier
	  s48-check-heap
	  s48-stob-in-heap?
	  s48-available
	  s48-find-all
	  s48-find-all-records
	  s48-gather-objects
	  s48-heap-size
	  s48-initialize-bibop-heap
	  s48-allocate-heap-size
	  s48-initialize-image-areas
	  s48-check-heap-size!)
  (open prescheme)
  (begin
    ;; (pre)allocates space for a small, fixed-sized objects
    (define s48-make-available+gc
      (external "s48_make_availableAgc" (=> (integer) null)))

    ;; actually allocate a small, fixed-sized object, with no heap
    ;; check and no GC
    (define s48-allocate-small
      (external "s48_allocate_small" (=> (integer) address)))
    
    ;; allocate a potentially large object containing pointers, GCing
    ;; to get room if necessary
    (define s48-allocate-traced+gc
      (external "s48_allocate_tracedAgc" (=> (integer) address)))

    ;; allocate a potentially large object not containing pointers,
    ;; GCing to get room if necessary
    (define s48-allocate-untraced+gc
      (external "s48_allocate_untracedAgc" (=> (integer) address)))

    ;; allocate an unmovable object (allocation uses the large area
    ;; discarding the size of the object. The large area is collected
    ;; with the non-copy algorithmus). GCing to get room if necessary
    (define s48-allocate-untraced-unmovable+gc
      (external "s48_allocate_untraced_unmovableAgc" (=> (integer) address)))

    ;; allocate a weak-pointer object, GCing to get room if necessary
    (define s48-allocate-weak+gc
      (external "s48_allocate_weakAgc" (=> (integer) address)))

    ;; tell the GC not to collect in any case mainly used on startup
    ;; during reading the image
    (define s48-forbid-gc!
      (external "s48_forbid_gcB" (=> () null)))

    ;; the GC may collect again
    (define s48-allow-gc!
      (external "s48_allow_gcB" (=> () null)))

    ;; these are defined in c/bibop/area_gc.c
    (define s48-collect
      (external "s48_collect" (=> (boolean) null)))
    
    (define s48-trace-value ;; s48_value -> s48_value
      (external "s48_trace_value" (=> (integer) integer)))
    
    (define s48-trace-locations! ;; address, address -> void
      (external "s48_trace_locationsB" (=> (address address) null)))
    
    (define s48-trace-stob-contents! ;; s48_value -> void
      (external "s48_trace_stob_contentsB" (=> (integer) null)))
    
    (define s48-extant? ;; s48_value -> bool
      (external "s48_extantP" (=> (integer) boolean)))
    
    (define s48-gc-count ;; void -> integer
      (external "s48_gc_count" (=> () integer)))

    (define s48-write-barrier
      (external "S48_WRITE_BARRIER" (=> (integer address integer) null)))
    
    (define s48-check-heap
      (external "s48_check_heap" (=> (integer) boolean)))
    
    (define s48-stob-in-heap?
      (external "s48_stob_in_heapP" (=> (integer) boolean)))
    
    (define s48-available ;; void -> integer (cells)
      (external "s48_available" (=> () integer)))
    
    (define s48-find-all ;; integer -> s48_value
      (external "s48_find_all" (=> (integer) integer)))
    
    (define s48-find-all-records ;; s48_value -> s48_value
      (external "s48_find_all_records" (=> (integer) integer)))
    
    (define s48-gather-objects
      (external "s48_gather_objects" (=> ((=> (integer) boolean)
					  (=> ((=> (integer) boolean)) boolean))
					 integer)))

    (define s48-heap-size
      (external "s48_heap_size" (=> () integer)))
    
    (define s48-initialize-bibop-heap
      (external "s48_initialize_bibop_heap" (=> () null)))
    
    ;; defined in generation_gc.c and used by the dumper
    (define s48-initialize-image-areas
      (external "s48_initialize_image_areas"
		(=> (integer integer integer integer integer integer) null)))
    
    (define s48-check-heap-size!
      (external "s48_check_heap_sizeB" (=> () null)))
    ))

; Image handling

(define-structure read-image-gc-specific read-image-gc-specific-interface
  (open prescheme ps-receive enum-case
	vm-utilities vm-architecture
	memory 
	data struct
	(subset string-tables (relocate-table-two-space)) ; ####
	ps-memory               ;allocate/deallocate-memory
	heap                    ;s48-heap-size
	heap-bibop
	image-util
	image-table             ;make-table
	heap-init
	read-image-util
	read-image-portable
	)
 (files (gc-bibop read-image)))

(define-structure read-image-util-gc-specific read-image-util-gc-specific-interface
  (open prescheme
	heap-bibop)
  (begin
    (define (get-small-start-addr heap-image-pointer)
      (s48-get-new-small-start-addr))
    (define (get-large-start-addr heap-image-pointer)
      (s48-get-new-large-start-addr))
    (define (get-weaks-start-addr heap-image-pointer)
      (s48-get-new-weaks-start-addr))))

(define-structure write-image-gc-specific write-image-gc-specific-interface
  (open prescheme ps-receive enum-case
	vm-utilities vm-architecture
	memory data struct
	heap
	heap-bibop
	image-table
	image-util
	write-image-util
	string-tables
	symbols				;s48-symbol-table
	external-opcodes ;s48-imported-bindings s48-exported-bindings
	ps-record-types
	(subset allocation (area-type-size))
	)
  (files (gc-bibop write-image)))
