; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Encoders/decoders from text to bytes and vice versa, for use by the
; the ports subsystem.

; Note that encoders and decoders must operate on buffers only
; provisionally.

(define-record-type text-codec :text-codec
  (make-text-codec name
		   encode-char-proc
		   decode-char-proc)
  text-codec?
  (name text-codec-name)
  ;; (char buffer start count) -> (ok? #f or #bytes consumed or #bytes needed)
  (encode-char-proc text-codec-encode-char-proc)
  ;; (buffer start count) -> (char #bytes consumed)
  ;;                      or (#f #total bytes needed at least)
  ;;                      or (#f #f) (failure)
  (decode-char-proc text-codec-decode-char-proc))

(define null-text-codec
  (make-text-codec "null"
		   (lambda (char buffer start count)
		     #f)
		   (lambda (buffer start count)
		     (values #f #f))))

;; Latin 1

(define (encode-char/latin-1 char buffer start count)
  (let ((scalar-value (char->scalar-value char)))
    (if (< scalar-value 256)
	(begin
	  (provisional-byte-vector-set! buffer start scalar-value)
	  (values #t 1))
	(values #f #f))))

(define (decode-char/latin-1 buffer start count)
  (values (scalar-value->char (provisional-byte-vector-ref buffer start))
	  1))

(define latin-1-codec (make-text-codec "ISO8859-1"
				      encode-char/latin-1
				      decode-char/latin-1))

;; UTF 8

(define (encode-char/utf-8 char buffer start count)
  (let ((scalar-value (char->scalar-value char)))
    (cond
     ((<= scalar-value #x7f)
      (provisional-byte-vector-set! buffer start scalar-value)
      (values #t 1))
     ((<= scalar-value #x7ff)
      (if (>= count 2)
	  (begin
	    (provisional-byte-vector-set!
	     buffer start
	     (+ #xc0
		(arithmetic-shift (bitwise-and scalar-value #b11111000000)
				  -6)))
	    (provisional-byte-vector-set!
	     buffer (+ 1 start)
	     (+ #x80
		(bitwise-and scalar-value #b111111)))
	    (values #t 2))
	  (values #f 2)))
     ((<= scalar-value #xffff)
      (if (>= count 3)
	  (begin
	    (provisional-byte-vector-set!
	     buffer start
	     (+ #xe0
		(arithmetic-shift (bitwise-and scalar-value #b1111000000000000)
				  -12)))
	    (provisional-byte-vector-set!
	     buffer (+ 1 start)
	     (+ #x80
		(arithmetic-shift (bitwise-and scalar-value #b111111000000)
				  -6)))
	    (provisional-byte-vector-set!
	     buffer (+ 2 start)
	     (+ #x80
		(bitwise-and scalar-value #b111111)))
	    (values #t 3))
	  (values #f 3)))
     (else
      (if (>= count 4)
	  (begin
	    (provisional-byte-vector-set!
	     buffer start
	     (+ #xf0
		(arithmetic-shift (bitwise-and scalar-value #b111000000000000000000)
				  -18)))
	    (provisional-byte-vector-set!
	     buffer (+ 1 start)
	     (+ #x80
		(arithmetic-shift (bitwise-and scalar-value #b111111000000000000)
				  -12)))
	    (provisional-byte-vector-set!
	     buffer (+ 2 start)
	     (+ #x80
		(arithmetic-shift (bitwise-and scalar-value #b111111000000)
				  -6)))
	    (provisional-byte-vector-set!
	     buffer (+ 3 start)
	     (+ #x80
		(bitwise-and scalar-value #b111111)))
	    (values #t 4))
	  (values #f 4))))))

; The table, and the associated decoding algorithm, is from
; Richard Gillam: "Unicode Demystified", chapter 14

(define *utf-8-state-table*
  '#(;; state 0
     0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 -1 -1 -1 -1 -1 -1 -1 -1 1 1 1 1 2 2 3 -1
     ;; state 1
     -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 0 0 0 0 0 0 0 0 -2 -2 -2 -2 -2 -2 -2 -2
     ;; state 2
     -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 1 1 1 1 1 1 1 1 -2 -2 -2 -2 -2 -2 -2 -2
     ;; state 3
     -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 -2 2 2 2 2 2 2 2 2 -2 -2 -2 -2 -2 -2 -2 -2))

(define *utf-8-masks* '#(#x7f #x1f #x0f #x07))

; We don't check for non-shortest-form UTF-8.  Too bad.

(define (decode-char/utf-8 buffer start count)
  (let loop ((q 0) (state 0) (mask 0) (scalar-value 0))
    (if (< q count)
	(let* ((c (provisional-byte-vector-ref buffer (+ start q)))
	       (state (vector-ref *utf-8-state-table*
				  (+ (arithmetic-shift state 5)	; (* state 32)
				     (arithmetic-shift c -3)))))
	  (case state
	    ((0)
	     (let ((scalar-value (+ scalar-value
				    (bitwise-and c #x7f))))
	       (if (scalar-value? scalar-value)
		   (values (scalar-value->char scalar-value)
			   (+ q 1))
		   (values #f #f))))
	    ((1 2 3)
	     (loop (+ 1 q) state #x3f
		   (arithmetic-shift (+ scalar-value
					(bitwise-and c
						     (if (zero? mask)
							 (vector-ref *utf-8-masks* state)
							 mask)))
				     6)))
	    ((-2 -1)
	     (values #f #f))))
	(values #f (+ 1 q)))))

(define utf-8-codec (make-text-codec "UTF-8"
				    encode-char/utf-8
				    decode-char/utf-8))
