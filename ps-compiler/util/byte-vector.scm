; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.


; byte vectors are just code vectors

(define make-byte-vector   make-code-vector)
(define byte-vector?       code-vector?)
(define byte-vector-length code-vector-length)

(define byte-vector-ref  code-vector-ref)
(define byte-vector-set! code-vector-set!)

(define (byte-vector-endianess)
  (if (eq? byte-vector-word-ref high-byte-vector-word-ref)
      'high
      'low))

(define (set-byte-vector-endianess! high-or-low)
  (case high-or-low
    ((high)
     (set! byte-vector-word-ref       high-byte-vector-word-ref)
     (set! byte-vector-half-word-ref  high-byte-vector-half-word-ref)
     (set! byte-vector-word-set!      high-byte-vector-word-set!)
     (set! byte-vector-half-word-set! high-byte-vector-half-word-set!))
    ((low)
     (set! byte-vector-word-ref       low-byte-vector-word-ref)
     (set! byte-vector-half-word-ref  low-byte-vector-half-word-ref)
     (set! byte-vector-word-set!      low-byte-vector-word-set!)
     (set! byte-vector-half-word-set! low-byte-vector-half-word-set!))
    (else
     (error "endianess specifier is neither HIGH nor LOW" high-or-low))))

(define (high-byte-vector-word-ref vector index)
  (+ (code-vector-ref vector (+ index 3))
     (arithmetic-shift
      (+ (code-vector-ref vector (+ index 2))
	 (arithmetic-shift
	  (+ (code-vector-ref vector (+ index 1))
	     (arithmetic-shift
	      (code-vector-ref vector index)
	      8))
	  8))
      8)))

(define (high-byte-vector-word-set! vector index value)
  (byte-vector-set! vector index       (arithmetic-shift value -24))
  (byte-vector-set! vector (+ index 1) (arithmetic-shift value -16))
  (byte-vector-set! vector (+ index 2) (arithmetic-shift value -8))
  (byte-vector-set! vector (+ index 3)                   value))

(define (high-byte-vector-half-word-ref vector index)
  (+ (code-vector-ref vector (+ index 1))
     (arithmetic-shift
      (code-vector-ref vector index)
      8)))

(define (high-byte-vector-half-word-set! vector index value)
  (byte-vector-set! vector index       (arithmetic-shift value -8))
  (byte-vector-set! vector (+ index 1)                   value))

(define (low-byte-vector-word-ref vector index)
  (+ (code-vector-ref vector index)
     (arithmetic-shift
      (+ (code-vector-ref vector (+ index 1))
	 (arithmetic-shift
	  (+ (code-vector-ref vector (+ index 2))
	     (arithmetic-shift
	      (code-vector-ref vector (+ index 3))
	      8))
	  8))
      8)))

(define (low-byte-vector-word-set! vector index value)
  (byte-vector-set! vector index                         value)
  (byte-vector-set! vector (+ index 1) (arithmetic-shift value -8))
  (byte-vector-set! vector (+ index 2) (arithmetic-shift value -16))
  (byte-vector-set! vector (+ index 3) (arithmetic-shift value -24)))

(define (low-byte-vector-half-word-ref vector index)
  (+ (code-vector-ref vector index)
     (arithmetic-shift
      (code-vector-ref vector (+ index 1))
      8)))

(define (low-byte-vector-half-word-set! vector index value)
  (byte-vector-set! vector index                         value)
  (byte-vector-set! vector (+ index 1) (arithmetic-shift value -8)))

; Start high-endian
(define byte-vector-word-ref       high-byte-vector-word-ref)
(define byte-vector-half-word-ref  high-byte-vector-half-word-ref)
(define byte-vector-word-set!      high-byte-vector-word-set!)
(define byte-vector-half-word-set! high-byte-vector-half-word-set!)
