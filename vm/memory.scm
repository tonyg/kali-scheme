; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file memory.scm.

;;;; Memory

; Memory abstraction for simulating the S48 heap inside of Scheme.

; Fundamental parameters

(define bits-per-byte 8)  ;or 8 or 9, choose one
(define bytes-per-cell 4)
(define bits-per-cell (* bits-per-byte bytes-per-cell))

(define (bytes->cells bytes)
  (quotient (+ bytes (- bytes-per-cell 1))
	    bytes-per-cell))

(define (cells->bytes cells)
  (* cells bytes-per-cell))

; Addresses
;
;  An "addressing unit" is the smallest quantum of storage addressed by
;  an address on a particular machine.  In the simulation, which has a
;  big array with one cell per entry, there is one addressing unit per
;  cell.  Similarly on a DEC-20, 3600, or other word-addressed
;  architecture.  On the VAX or 68000, though, the addressing unit is
;  the byte, of which there are 4 to a cell.
;
;  Note: by a "byte" is meant enough bits to store either a character or
;  a bytecode.  That probably means either 7, 8, or 9 bits.
;
;  Each address may have some number of "unused bits" at its low end.
;  When memory is a Scheme vector, there are none, but when it's VAX or
;  68000 memory, there are two.

(define unused-field-width 2)
;(define addressing-units-per-cell (ashl 1 unused-field-width))
(define addressing-units-per-cell 4)

(define (cells->a-units cells)
  (adjoin-bits cells 0 unused-field-width))

(define (a-units->cells cells)
  (high-bits cells unused-field-width))

(define (bytes->a-units byte-count)
  (cells->a-units (bytes->cells byte-count)))

; The following operations work on addresses (which just happen to be
; implemented as fixnums).

(define addr+ +)
(define addr- -)             ; subtracting a constant
(define addr-difference -)   ; subtracting another pointer
(define addr=  = )
(define addr<  < )
(define addr<= <=)
(define addr>  > )
(define addr>= >=)

(define (addr1+ x)
  (addr+ x addressing-units-per-cell))

; Memory access

(define *memory*       (unassigned))
(define *memory-begin* 0)
(define *memory-end*   0)

(define (memory-begin)
  *memory-begin*)

; Size of memory in cells.
(define (memory-size)
  (a-units->cells (addr-difference *memory-end* *memory-begin*)))

(define (create-memory size initial-value)   ;size in cells
  (let ((size (cells->a-units size)))
    (cond ((not (= size (addr-difference *memory-end* *memory-begin*)))
	   (if (not (= *memory-end* 0))
	       (deallocate-memory *memory*))
           (set! *memory* (allocate-memory size))
	   (if (= -1 *memory*)
	       (error "out of memory, unable to continue"))
	   (set! *memory-begin* *memory*)
           (set! *memory-end* (+ *memory* size))))))

(define fetch word-ref)
(define fetch-byte unsigned-byte-ref)
(define store! word-set!)
(define store-byte! unsigned-byte-set!)

(define (read-bytes pointer count port)
  (read-block port pointer count))

(define (write-bytes pointer count port)
  (write-block port pointer count))
