; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-structure write-image (export s48-write-image)
  (open prescheme)
  (begin
    (define s48-write-image
      (external "s48_write_image" (=> (integer integer output-port) integer)))))

(define-structure read-image read-image-interface
  (open prescheme)
  (begin
    (define s48-write-image
      (external "s48_write_image" (=> (integer integer output-port) integer)))
    (define s48-read-image
      (external "s48_read_image" (=> (integer integer) integer)))

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

    (define s48-relocate-all
      (external "s48_relocate_all" (=> (integer address address integer integer integer) null)))

    (define s48-initialization-complete!
      (external "s48_initialization_completeB" (=> () null)))
    (define s48-initializing-gc-root
      (external "s48_initializing_gc_root" (=> () null)))

    ;; For debugging
    (define get-all-globals
      (external "get_all_globals" (=> () null)))
))

