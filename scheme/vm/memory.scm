; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file memory.scm.

; Fundamental parameters

(define bits-per-byte 8)
(define bytes-per-cell 4)
(define bits-per-cell (* bits-per-byte bytes-per-cell))

(define (bytes->cells bytes)
  ; using shift instead of quotient for speed
  ; (quotient (+ bytes (- bytes-per-cell 1)) bytes-per-cell)
  (arithmetic-shift-right (+ bytes (- bytes-per-cell 1))
			  2))  ; log(bytes-per-cell)

(define (cells->bytes cells)
  (* cells bytes-per-cell))

; Addresses
;
;  An "addressing unit" is the smallest quantum of storage addressed by
;  an address on a particular machine.  On a DEC-20, 3600, or other
;  word-addressed architecture there is one addressing unit per cell.  On
;  the VAX or 68000, though, the addressing unit is the byte, of which there
;  are 4 to a cell.
;
;  Note: by a "byte" is meant enough bits to store either a character or
;  a bytecode.  That probably means either 7, 8, or 9 bits.
;
;  If the addressing unit is smaller than a cell each address will have some
;  number of "unused bits" at its low end.  On a byte-addressable machine with
;  32 bit addresses, there are two.

(define unused-field-width 2)

(define addressing-units-per-cell 4)

(define (cells->a-units cells)
  (adjoin-bits cells 0 unused-field-width))

(define (a-units->cells cells)
  (high-bits cells unused-field-width))

(define (bytes->a-units byte-count)
  (cells->a-units (bytes->cells byte-count)))

(define (address1+ x)
  (address+ x addressing-units-per-cell))

(define (address2+ x)
  (address1+ (address1+ x)))

; Memory access

(define *memory*)
(define *memory-begin* 0)
(define *memory-end* 0)

(define (memory-begin)
  *memory-begin*)

; Size of memory in cells.
(define (memory-size)
  (a-units->cells (address-difference *memory-end* *memory-begin*)))

(define (create-memory size initial-value)   ;size in cells
  (let ((size (cells->a-units size)))
    (cond ((not (= size (address-difference *memory-end* *memory-begin*)))
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

(define fetch-string char-pointer->string)
(define fetch-nul-terminated-string char-pointer->nul-terminated-string)

