; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Common character/string encodings

; It's a shame the algorithms are implemented multiple times for the
; port encodings and this, only in different forms.  But I don't know
; how to unify the different algorithms.  (Other than via 

; There's also grotesque amounts of code duplication within this file.
; Help welcome.

; UTF-8

(define (char-encoding-length/utf-8 char)
  (let ((scalar-value (char->scalar-value char)))
    (cond
     ((<= scalar-value #x7f) 1)
     ((<= scalar-value #x7ff) 2)
     ((<= scalar-value #xffff) 3)
     (else 4))))

(define (string-encoding-length/utf-8 s start-index count)
  (let loop ((utf-8-length 0)
	     (char-index 0))
    (if (>= char-index count)
	utf-8-length
	(loop (+ utf-8-length 
		 (char-encoding-length/utf-8 (string-ref s (+ start-index char-index))))
	      (+ 1 char-index)))))

; returns byte count of the encoding
(define (encode-char/utf-8 char target target-start)
  (let ((scalar-value (char->scalar-value char)))
    (cond
     ((<= scalar-value #x7f)
      (byte-vector-set! target target-start scalar-value)
      1)
     ((<= scalar-value #x7ff)
      (byte-vector-set! target target-start
			(+ #xc0
			   (arithmetic-shift (bitwise-and scalar-value #b11111000000)
					     -6)))
      (byte-vector-set! target (+ 1 target-start)
			(+ #x80
			   (bitwise-and scalar-value #b111111)))
      2)
     ((<= scalar-value #xffff)
      (byte-vector-set! target target-start
			(+ #xe0
			   (arithmetic-shift (bitwise-and scalar-value #b1111000000000000)
					     -12)))
      (byte-vector-set! target (+ 1 target-start)
			(+ #x80
			   (arithmetic-shift (bitwise-and scalar-value #b111111000000)
					     -6)))
      (byte-vector-set! target (+ 2 target-start)
			(+ #x80
			   (bitwise-and scalar-value #b111111)))
      3)
     (else
      (byte-vector-set! target target-start
			(+ #xf0
			   (arithmetic-shift (bitwise-and scalar-value #b111000000000000000000)
					     -18)))
      (byte-vector-set! target (+ 1 target-start)
			(+ #x80
			   (arithmetic-shift (bitwise-and scalar-value #b111111000000000000)
					     -12)))
      (byte-vector-set! target (+ 2 target-start)
			(+ #x80
			   (arithmetic-shift (bitwise-and scalar-value #b111111000000)
					     -6)))
      (byte-vector-set! target (+ 3 target-start)
			(+ #x80
			   (bitwise-and scalar-value #b111111)))
      4))))

; returns byte count of encoded string
(define (encode-string/utf-8 source source-start source-count target target-start)
  (let loop ((source-index 0)
	     (target-index 0))
    (if (>= source-index source-count)
	target-index
	(let ((count (encode-char/utf-8 (string-ref source (+ source-start source-index))
					target (+ target-start target-index))))
	  (loop (+ source-index 1) (+ count target-index))))))

(define (string->utf-8 s)
  (let* ((string-size (string-length s))
	 (size (string-encoding-length/utf-8 s 0 string-size))
	 (result (make-byte-vector size 0)))
    (encode-string/utf-8 s 0 string-size result 0)
    result))

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

; This returns the size of the encoding of the first character.  If
; RAISE-ERROR? is true, we raise an error if the encoding is
; incomplete or incorrect.  If it's false, we just return the number
; of bytes the decoder would consume.

(define (bytes-char-encoding-length/utf-8 bytes start-index count raise-error?)
  (let loop ((q 0) (state 0) (mask 0) (scalar-value 0))
    (if (< q count)
	(let* ((c (byte-vector-ref bytes (+ start-index q)))
	       (state (vector-ref *utf-8-state-table*
				  (+ (arithmetic-shift state 5)	; (* state 32)
				     (arithmetic-shift c -3)))))
	  (case state
	    ((0)
	     (let ((scalar-value (+ scalar-value
					    (bitwise-and c #x7f))))
	       (cond
		((scalar-value? scalar-value)
		 (values (scalar-value->char scalar-value)
			 (+ q 1)))
		(raise-error?
		 (call-error "illegal scalar value in UTF-8 encoding"
			     decode-char/utf-8
			     bytes start-index scalar-value))
		(else
		 1))))
	    ((1 2 3)
	     (loop (+ 1 q) state #x3f
		   (arithmetic-shift (+ scalar-value
					(bitwise-and c
						     (if (zero? mask)
							 (vector-ref *utf-8-masks* state)
							 mask)))
				     6)))
	    ((-2 -1)
	     (if raise-error?
		 (call-error "illegal byte in UTF-8 encoding"
			     decode-char/utf-8
			     bytes (+ start-index q))
		 1))))
	(if raise-error?
	    1
	    (call-error "insufficient data for multi-byte UTF-8 char"
			bytes-char-encoding-length/utf-8
			bytes start-index count)))))

(define (bytes-char-encoding-length/utf-8 bytes start-index count raise-error?)
  (let loop ((q 0) (state 0))
    (if (< q count)
	(let* ((c (byte-vector-ref bytes (+ start-index q)))
	       (state (vector-ref *utf-8-state-table*
				  (+ (arithmetic-shift state 5)	; (* state 32)
				     (arithmetic-shift c -3)))))
	  (case state
	    ((0) (+ q 1))
	    ((1 2 3) (loop (+ 1 q) state))
	    ((-2 -1)
	     (if raise-error?
		 (call-error "illegal byte in UTF-8 encoding"
			     bytes-char-encoding-length/utf-8
			     bytes (+ start-index q))
		 1))))
	(if raise-error?
	    (call-error "insufficient data for multi-byte UTF-8 char"
			bytes-char-encoding-length/utf-8
			bytes start-index count)
	    1))))


(define (bytes-string-length/utf-8 bytes start count raise-error?)
  (let loop ((index 0)
	     (length 0))
    (if (>= index count)
	length
	(loop (+ index
		 (bytes-char-encoding-length/utf-8 bytes (+ start index) (- count index) raise-error?))
	      (+ 1 length)))))

; Returns the character itself and the number of bytes consumed
; if ERROR-CHAR is #f, we raise an exception

; We don't check for non-shortest-form UTF-8.  Too bad.

(define (decode-char/utf-8 bytes start-index count error-char)
  (let loop ((q 0) (state 0) (mask 0) (scalar-value 0))
    (if (< q count)
	(let* ((c (byte-vector-ref bytes (+ start-index q)))
	       (state (vector-ref *utf-8-state-table*
				  (+ (arithmetic-shift state 5)	; (* state 32)
				     (arithmetic-shift c -3)))))
	  (case state
	    ((0)
	     (let ((scalar-value (+ scalar-value
					    (bitwise-and c #x7f))))
	       (cond
		((scalar-value? scalar-value)
		 (values (scalar-value->char scalar-value)
			 (+ q 1)))
		(error-char
		 (values error-char 1))
		(else
		 (call-error "illegal scalar value in UTF-8 encoding"
			     decode-char/utf-8
			     bytes start-index scalar-value)))))
	    ((1 2 3)
	     (loop (+ 1 q) state #x3f
		   (arithmetic-shift (+ scalar-value
					(bitwise-and c
						     (if (zero? mask)
							 (vector-ref *utf-8-masks* state)
							 mask)))
				     6)))
	    ((-2 -1)
	     (if error-char
		 (values error-char 1)
		 (call-error "illegal byte in UTF-8 encoding"
			     decode-char/utf-8
			     bytes (+ start-index q))))))
	(if error-char
	    (call-error "insufficient data for multi-byte UTF-8 char"
			bytes-char-encoding-length/utf-8
			bytes start-index count)
	    (values error-char 1)))))

(define (decode-string/utf-8 bytes start count target target-start error-char)
  (let* ((string-size (bytes-string-length/utf-8 bytes start count (not error-char)))
	 (result (make-string string-size)))
    (let loop ((source-index 0)
	       (target-index 0))
      (if (>= source-index source-index)
	  source-index
	  (call-with-values
	      (lambda ()
		(decode-char/utf-8 bytes
				   (+ start source-index)
				   (- count source-index)
				   error-char))
	    (lambda (c size)
	      (string-set! result (+ target-start target-index) c)
	      (loop (+ source-index size)
		    (+ target-index 1))))))))

; may be slightly faster because of REVERSE-LIST->STRING
(define (utf-8->string source error-char)
  (let ((source-count (byte-vector-length source)))
    (let loop ((rev-chars '())
	       (char-count 0)
	       (source-index 0))
      (if (>= source-index source-count)
	  (reverse-list->string rev-chars char-count)
	  (call-with-values
	      (lambda ()
		(decode-char/utf-8 source
				   source-index
				   (- source-count source-index)
				   error-char))
	    (lambda (c size)
	      (loop (cons c rev-chars) (+ 1 char-count) (+ source-index size))))))))
