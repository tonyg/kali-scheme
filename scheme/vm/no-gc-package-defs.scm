; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; These are used to compile the VM separately from the GC.  It has new
; definitions of the heap and GC structures with all of the values defined
; as externals.

(define-structure allocation allocation-interface
  (open prescheme)
  (begin
    (define s48-available?
      (external "AVAILABLEp" (=> (integer) boolean))) ; macro
    (define (s48-preallocate-space size) 0)
    (define (s48-allocate-space type size key)
      (= key 0)  ; make type checker happy about KEY
      ((external "ALLOCATE_SPACE" (=> (integer integer) address)) type size))
    (define s48-write-barrier
      (external "S48_WRITE_BARRIER" (=> (integer address integer) null)))))

(define-structure heap heap-interface
  (open prescheme)
  (begin
    (define s48-available
      (external "s48_available" (=> () integer)))
    (define s48-heap-size
      (external "s48_heap_size" (=> () integer)))
    (define s48-find-all
      (external "s48_find_all" (=> (integer) integer)))
    (define s48-find-all-records
      (external "s48_find_all_records" (=> (integer) integer)))
    (define s48-*hp*
      (external "s48_ShpS" integer))
    (define s48-*limit*
      (external "s48_SlimitS" integer))))

(define-structure gc gc-interface
  (open prescheme)
  (begin
    (define s48-begin-collection
      (external "s48_begin_collection" (=> () null)))
    (define s48-trace-value
      (external "s48_trace_value" (=> (integer) integer)))
    (define s48-trace-locations!
      (external "s48_trace_locationsB" (=> (address address) null)))
    (define s48-trace-stob-contents!
      (external "s48_trace_stob_contentsB" (=> (integer) null)))
    (define s48-extant?
      (external "s48_extantP" (=> (integer) boolean)))
    (define s48-do-gc
      (external "s48_do_gc" (=> () null)))
    (define s48-end-collection
      (external "s48_end_collection" (=> () null)))
    (define s48-gc-count
      (external "s48_gc_count" (=> () integer)))))

(define-structure images images-interface
  (open prescheme)
  (begin
    (define s48-image-writing-okay?
      (external "s48_image_writing_okayP" (=> () boolean)))
    (define s48-write-image
      (external "s48_write_image" (=> (integer output-port) integer)))
    (define s48-check-image-header
      (external "s48_check_image_header" (=> ((^ char)) integer)))
    (define s48-read-image
      (external "s48_read_image" (=> (integer) integer)))

    (define s48-startup-procedure
      (external "s48_startup_procedure" (=> () integer)))
    (define s48-initial-symbols
      (external "s48_initial_symbols" (=> () integer)))
    (define s48-initial-imported-bindings
      (external "s48_initial_imported_bindings" (=> () integer)))
    (define s48-initial-exported-bindings
      (external "s48_initial_exported_bindings" (=> () integer)))
    (define s48-resumer-records
      (external "s48_resumer_records" (=> () integer)))
    (define s48-undumpable-records
      (external "s48_undumpable_records" (=> () integer integer)))

    (define s48-initialization-complete!
      (external "s48_initialization_completeB" (=> () null)))
    (define s48-initializing-gc-root
      (external "s48_initializing_gc_root" (=> () null)))))

