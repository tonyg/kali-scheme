; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; These are used to compile the GC separately from the VM.  It has new
; definitions of the VM structures that the GC uses, with all of the values
; defined as externals.  We only need to define those values that the GC
; actually uses.

; These are all called when writing images.

(define-structure symbols (export s48-copy-symbol-table)
  (open prescheme)
  (begin
    (define s48-copy-symbol-table
      (external "s48_copy_symbol_table" (=> () integer)))))

(define-structure vmio (export s48-mark-traced-channels-closed!)
  (open prescheme)
  (begin
    (define s48-mark-traced-channels-closed!
      (external "s48_mark_traced_channels_closedB" (=> () null)))))

(define-structure external-opcodes (export s48-cleaned-imported-bindings
					   s48-exported-bindings)
  (open prescheme)
  (begin
    (define s48-cleaned-imported-bindings
      (external "s48_cleaned_imported_bindings" (=> () integer)))
    (define s48-exported-bindings
      (external "s48_exported_bindings" (=> () integer)))))
