; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Simple-minded kludges for the initial image; these versions are
; Latin-1 only, but we can replace them later by procedures that
; respect the default encoding.

(define (string-encoding-length string start count)
  count)

; returns encoding byte count
(define (encode-string string start count bytes bytes-start)
  (do ((i 0 (+ 1 i)))
      ((>= i count)
       count)
    (byte-vector-set! bytes (+ i bytes-start)
		      (char->scalar-value (string-ref string (+ start i))))))

(define (string-decoding-length bytes start count)
  count)

; returns decoding character count
(define (decode-string bytes start count string string-start)
  (do ((i 0 (+ 1 i)))
      ((>= i count)
       count)
    (string-set! string (+ string-start i)
		 (scalar-value->char (byte-vector-ref bytes (+ start i))))))

(define (set-string-encoding-procedures! encoding-length
					 encode
					 decoding-length
					 decode)
  (set! string-encoding-length encoding-length)
  (set! encode-string encode)
  (set! string-decoding-length decoding-length)
  (set! decode-string decode))

; Utilities

(define (string->byte-vector s)
  (let* ((len (string-length s))
	 (size (string-encoding-length s 0 len))
	 (bytes (make-byte-vector size 0)))
    (encode-string s 0 len bytes 0)
    bytes))

(define (byte-vector->string bytes)
  (let* ((len (byte-vector-length bytes))
	 (size (string-decoding-length bytes 0 len))
	 (string (make-string size)))
    (decode-string bytes 0 len string 0)
    string))

; same as STRING->BYTE-VECTOR, but NUL-terminated

(define (string->byte-string s)
  (let* ((len (string-length s))
	 (size (string-encoding-length string 0 len))
	 (bytes (make-byte-vector (+ 1 size) 0)))
    (encode-string s 0 len bytes 0)
    bytes))

