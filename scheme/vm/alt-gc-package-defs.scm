; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
    (define s48-allocate-weak+gc
      (external "s48_allocate_weakAgc" (=> (integer) address)))
    (define s48-allocate-traced+gc
      (external "s48_allocate_tracedAgc" (=> (integer) address)))
    (define s48-allocate-untraced+gc
      (external "s48_allocate_untracedAgc" (=> (integer) address)))
    (define s48-allocate-untraced-unmovable+gc
      (external "s48_allocate_untraced_unmovableAgc" (=> (integer) address)))
    ))

(define-structure heap heap-interface
  (open prescheme)
  (begin
    (define s48-available
      (external "s48_available" (=> () integer)))
    (define s48-heap-size
      (external "s48_heap_size" (=> () integer)))
    (define s48-max-heap-size
      (external "s48_max_heap_size" (=> () integer)))
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

    (define s48-stob-in-heap?
      (external "s48_stob_in_heapP" (=> (integer) boolean)))
    ))


(define-structure gc gc-interface
  (open prescheme)
  (begin
    (define s48-collect
      (external "s48_collect" (=> (boolean) null)))
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
      (external "s48_gc_count" (=> () integer)))
    (define s48-gc-run-time
      (external "s48_gc_run_time" (=> () integer integer)))
    ))

