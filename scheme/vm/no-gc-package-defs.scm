(define-structure allocation allocation-interface
  (open prescheme)
  (begin
    (define available?
      (external "AVAILABLEp" (=> (int32) boolean))) ; macro
    (define (preallocate-space size) 0)
    (define universal-key 0)
    (define (allocate-space type size key)
      (= key universal-key)  ; make type checker happy about KEY
      ((external "ALLOCATE_SPACE" (=> (int32 int32) int32)) type size))
    (define write-barrier
      (external "WRITE_BARRIER" (=> (int32 int32) null)))))

(define-structure heap heap-interface
  (open prescheme)
  (begin
    (define available
      (external "available" (=> () int32)))
    (define heap-size
      (external "heap_size" (=> () int32)))
    (define begin-collection
      (external "begin_collection" (=> () null)))
    (define trace-value
      (external "trace_value" (=> (int32) int32)))
    (define trace-locations!
      (external "trace_locationsB" (=> (int32 int32) null)))
    (define trace-stob-contents!
      (external "trace_stob_contentsB" (=> (int32) null)))
    (define do-gc
      (external "do_gc" (=> (int32) int32 int32)))
    (define end-collection
      (external "end_collection" (=> () null)))
    (define gc-count
      (external "gc_count" (=> () int32)))
    (define image-writing-okay?
      (external "image_writing_okayP" (=> () boolean)))
    (define write-image
      (external "write_image" (=> (int32 output-port (=> () null)) int32)))
    (define find-all
      (external "find_all" (=> (int7u) int32)))
    (define find-all-records
      (external "find_all_records" (=> (int32) int32)))))

