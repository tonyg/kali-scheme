
; To do:
;
; There needs to be an area-heap structure as well.  It has to handle
; find-all somehow.  Keep asking for more space, I guess.  Start out
; with a few pages.  If the vector ends up small then copy it to the
; current small-object area.  Perhaps this could be abstracted out
; somehow to allow the two GC's to share code.
;
; Add S48-TRACE-CONTINUATION-CONTENTS! to area-gc, or put it in a separate
; gc-util struct along with the S48-TRACE-LOCATIONS! wrappers.
;
; Keeping track of stack depth:
;   - maintain the current in-heap depth in a register, updating it whenever
;     frames are copied in or out
;   - call/cc creates a special continuation that holds a regular continuation
;     and its depth (with a special 'native code' template)
;   - resuming a non-special continuation, which only the debugger does,
;     requires walking the down to get the depth
;   - Scheme continuation code elides the depth-holding continuations, as it
;     does with exception continuations
;   - requires a new exception along with a handler for the appropriate opcodes
;     (or maybe use an interrupt instead?)

(define-structure page-constants (export bytes-per-page
					 log-bytes-per-page
					 page-start-address
					 index-in-page
					 bytes->pages
					 pages->bytes
					 add-pages)
  (open prescheme memory)
  (begin
    (define log-bytes-per-page 12)

    (define bytes-per-page (shift-left 1 log-bytes-per-page))

    (define page-index-mask
      (- bytes-per-page 1))

    (define (page-start-address address)
      (integer->address (bitwise-and (address->integer address)
				     (bitwise-not page-index-mask))))

    (define (index-in-page address)
      (bitwise-and (address->integer address)
		   page-index-mask))

    (define (bytes->pages n)
      (arithmetic-shift-right (+ n (- bytes-per-page 1)) log-bytes-per-page))

    (define (pages->bytes n)
      (shift-left n log-bytes-per-page))

    (define (add-pages address pages)
      (address+ address (pages->bytes pages)))
    ))

(define-interface areas-interface
  (export allocate-area
	  area-gc-action set-area-gc-action!
	  area-start     set-area-start!
	  area-end       set-area-end!
	  area-frontier  set-area-frontier!
	  area-next      set-area-next! clear-area-next!))

(define-structure areas areas-interface
  (open prescheme ps-record-types ps-receive
	memory data
	page-constants
	memory-map
	page-allocation)
  (files (heap area)))

(define-structure area-gc gc-interface
  (open prescheme ps-record-types
	enum-case
	interpreter-gc
	memory-map
	areas)
  (files (heap area-gc)))

(define-structure memory-map (export memory-map-ref
				     memory-map-set!)
  (open prescheme
	ps-record-types
	ps-memory		; address->integer
	page-constants
	vm-utilities)		; vector+length-fill
  (files (heap memory-map)))

(define-structure page-allocation (export allocate-pages free-pages!)
  (open prescheme vm-utilities
	ps-record-types
	ps-memory
	page-constants)
  (files (heap page-alloc)))

