; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1997 by NEC Research Institute, Inc.    See file COPYING.

(define-structure allocation allocation-interface
  (open prescheme)
  (begin
    (define available?
      (external "AVAILABLEp" (=> (integer) boolean))) ; macro
    (define (preallocate-space size) 0)
    (define universal-key 0)
    (define (allocate-space type size key)
      (= key universal-key)  ; make type checker happy about KEY
      ((external "ALLOCATE_SPACE" (=> (integer integer) address)) type size))
    (define write-barrier
      (external "WRITE_BARRIER" (=> (address integer) null)))))

(define-structure heap heap-interface
  (open prescheme)
  (begin
    (define available
      (external "available" (=> () integer)))
    (define heap-size
      (external "heap_size" (=> () integer)))
    (define begin-collection
      (external "begin_collection" (=> () null)))
    (define trace-value
      (external "trace_value" (=> (integer) integer)))
    (define trace-locations!
      (external "trace_locationsB" (=> (address address) null)))
    (define trace-stob-contents!
      (external "trace_stob_contentsB" (=> (integer) null)))
    (define do-gc
      (external "do_gc" (=> (integer) integer integer)))
    (define end-collection
      (external "end_collection" (=> () null)))
    (define gc-count
      (external "gc_count" (=> () integer)))
    (define image-writing-okay?
      (external "image_writing_okayP" (=> () boolean)))
    (define write-image
      (external "write_image" (=> (integer output-port (=> () null)) integer)))
    (define find-all
      (external "find_all" (=> (integer) integer)))
    (define find-all-records
      (external "find_all_records" (=> (integer) integer)))
    (define *hp*
      (external "ShpS" integer))
    (define *limit*
      (external "SlimitS" integer))))

