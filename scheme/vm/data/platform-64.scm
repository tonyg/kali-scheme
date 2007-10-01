; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Essential constants for 32-bit platforms.

; Fundamental parameters

(define bytes-per-cell 8)
(define log-bytes-per-cell 3)

(define bits-per-byte 8)
(define bits-per-cell (* bits-per-byte bytes-per-cell))
(define addressing-units-per-cell bytes-per-cell)

; This is actually the mimimum for the different PreScheme implementations.
; The Scheme version of PreScheme leaves 30 bits for PreScheme's fixnums.
; There have to be enough bits to represent the largest fixnum in the system.
; USEFUL-BITS-PER-WORD is not written in the image.
(define s48-useful-bits-per-word      62) ; in Scheme48
(define c-useful-bits-per-word        64) ; when compiled

; Addresses
;
;  An "addressing unit" is the smallest quantum of storage addressed by
;  an address on a particular machine.  On a DEC-20, 3600, or other
;  word-addressed architecture there is one addressing unit per cell.  On
;  the VAX or 68000, though, the addressing unit is the byte, of which there
;  are 4 to a cell.
;
;  Note: by a "byte" is meant enough bits to store a bytecode.  That
;  probably means either 7, 8, or 9 bits.
;
;  If the addressing unit is smaller than a cell each address will have some
;  number of "unused bits" at its low end.  On a byte-addressable machine with
;  32 bit addresses, there are two.

(define unused-field-width log-bytes-per-cell)

; Descriptors
;  A descriptor describes a Scheme object.
;  A descriptor is represented as an integer whose low two bits are
;  tag bits.  The high bits contain information whose format and
;  meaning are dependent on the tag.
(define tag-field-width 2)
(define data-field-width (- bits-per-cell tag-field-width))

; Immediates
;  This streamlines 8-bit-byte-oriented implementations.
(define immediate-type-field-width
  (- bits-per-byte tag-field-width))

; Pre-Scheme
(define pre-scheme-integer-size bits-per-cell)
