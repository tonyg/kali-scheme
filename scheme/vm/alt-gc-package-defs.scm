; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; These are used to compile the VM separately from the GC.  It has new
; definitions of the heap and GC structures with all of the values defined
; as externals.

(define-structure allocation allocation-interface
  (open prescheme)
  (begin
    (define s48-make-available+gc
      (external "s48_make_availableAgc" (=> (integer) null)))
    (define s48-allocate-small
      (external "s48_allocate_small" (=> (integer) address)))
    (define s48-allocate-traced+gc
      (external "s48_allocate_tracedAgc" (=> (integer) address)))
    (define s48-allocate-untraced+gc
      (external "s48_allocate_untracedAgc" (=> (integer) address)))))

(define-structure heap heap-interface
  (open prescheme)
  (begin
    (define s48-available
      (external "s48_available" (=> () integer)))
    (define s48-available?
      (external "AVAILABLEp" (=> (integer) boolean))) ; macro
    (define s48-heap-size
      (external "s48_heap_size" (=> () integer)))
    (define s48-gather-objects
      (external "s48_gather_objects" (=> ((=> (integer) boolean)
					  (=> ((=> (integer) boolean)) boolean))
					 integer)))
    (define s48-find-all
      (external "s48_find_all" (=> (integer) integer)))
    (define s48-find-all-records
      (external "s48_find_all_records" (=> (integer) integer)))
    (define s48-write-barrier
      (external "S48_WRITE_BARRIER" (=> (integer address integer) null)))
    (define s48-*hp*
      (external "s48_ShpS" integer))
    (define s48-*limit*
      (external "s48_SlimitS" integer))
    (define s48-oldspace-begin
      (external "s48_oldspace_begin" (=> () address)))
    (define s48-oldspace-end
      (external "s48_oldspace_end" (=> () address)))
    (define s48-heap-begin
      (external "s48_heap_begin" (=> () address)))
    (define s48-heap-pointer
      (external "s48_heap_pointer" (=> () address)))))

(define-structure gc gc-interface
  (open prescheme)
  (begin
    (define s48-collect
      (external "s48_collect" (=> () null)))
    (define s48-trace-value
      (external "s48_trace_value" (=> (integer) integer)))
    (define s48-trace-locations!
      (external "s48_trace_locationsB" (=> (address address) null)))
    (define s48-trace-stob-contents!
      (external "s48_trace_stob_contentsB" (=> (integer) null)))
    (define s48-trace-continuation-contents!
      (external "s48_trace_continuation_contentsB"
		(=> (address address integer) null)))
    (define s48-extant?
      (external "s48_extantP" (=> (integer) boolean)))
    (define s48-gc-count
      (external "s48_gc_count" (=> () integer)))))

(define-structure heap-init (export s48-initialize-heap)
  (open prescheme)
  (begin
    (define s48-initialize-heap
      (external "s48_initialize_heap" (=> (integer integer address) address)))))

