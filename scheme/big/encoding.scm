; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Common character/string encodings

; It's a shame the algorithms are implemented multiple times for the
; port encodings and this, only in different forms.  But I don't know
; how to unify the different algorithms.

; There's grotesque amounts of code duplication within this file.

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

(define-enumerated-type encoding-status :encoding-status
  encoding-status?
  encoding-statuses
  encoding-status-name
  encoding-status-index
  (complete insufficient))

; Will only produce complete encodings
; returns three values:
; - encoding status
; - # characters consumed
; - # bytes decoded

(define (encode-string/utf-8 source source-start source-count
			     target target-start target-count)
  (let loop ((source-index 0)
	     (target-index 0))
    (cond
     ((>= source-index source-count)
      (values (encoding-status complete)
	      source-index
	      target-index))
     ((>= target-index target-count)
      (values (encoding-status insufficient)
	      source-index
	      target-index))
     (else
      (let* ((c (string-ref source (+ source-start source-index)))
	     (size (char-encoding-length/utf-8  c)))
	(if (> (+ target-index size) target-count)
	    (values (encoding-status insufficient)
		    source-index
		    target-index)
	    (begin
	      (encode-char/utf-8 (string-ref source (+ source-start source-index))
				 target (+ target-start target-index))
	      (loop (+ source-index 1) (+ target-index size)))))))))

(define (string->utf-8-n s start count)
  (let* ((size (string-encoding-length/utf-8 s 0 count))
	 (result (make-byte-vector size 0)))
    (encode-string/utf-8 s 0 count result 0 size)
    result))

(define (string->utf-8 s)
  (string->utf-8-n s 0 (string-length s)))

; Decoding

(define-condition-type &decoding-error &error
  decoding-error?
  (encoding-name decoding-error-encoding-name))

; Scheme 48 has:
;
; (define-condition-type &irritants &condition
;   irritants?
;   (values condition-irritants))

(define (decoding-error encoding-name
			message
			bytes start)
  (raise
   (condition
    (&decoding-error
     (encoding-name encoding-name))
    (&irritants
     (values (list bytes start)))
    (&message
     (message
      (string-append "error while decoding " encoding-name ": " message))))))

(define-enumerated-type decoding-status :decoding-status
  decoding-status?
  decoding-statuses
  decoding-status-name
  decoding-status-index
  (complete incomplete insufficient invalid))

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

; Returns three values:
; - decoding status
; - character if status is COMPLETE, else #f
; - # bytes consumed if COMPLETE or INCOMPLETE, else #f

; We don't check for non-shortest-form UTF-8.  Too bad.

(define (decode-char/utf-8 bytes start-index count)
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
	       (if (scalar-value? scalar-value)
		   (values (decoding-status complete)
			   (scalar-value->char scalar-value)
			   (+ q 1))
		   (values (decoding-status invalid)
			   #f
			   #f))))
	    ((1 2 3)
	     (loop (+ 1 q) state #x3f
		   (arithmetic-shift (+ scalar-value
					(bitwise-and c
						     (if (zero? mask)
							 (vector-ref *utf-8-masks* state)
							 mask)))
				     6)))
	    ((-2 -1)
	     (values (decoding-status invalid)
		     #f
		     #f))))
	(values (decoding-status incomplete)
		q
		#f))))

; If STOP-AT-INVALID? is #f, we'll skip an invalid byte, and pretend
; it generated one character.

; Returns three values:
; - :DECODING-STATUS object
; - # bytes consumed
; - # characters decoded

; This is just a slice of decode-string/utf-8, regrettably.
(define (bytes-string-size/utf-8 bytes start count stop-at-invalid?)
  (let loop ((index 0)
	     (target-index 0))
    (if (>= index count)
	(values (decoding-status complete)
		index target-index)
	(call-with-values
	    (lambda ()
	      (decode-char/utf-8 bytes
				 (+ start index)
				 (- count index)))
	  (lambda (status c consumed)
	    (cond
	     ((eq? status (decoding-status complete))
	      (loop (+ index consumed) (+ target-index 1)))
	     ((eq? status (decoding-status incomplete))
	      (values (decoding-status incomplete)
		      index target-index))
	     ((eq? status (decoding-status invalid))
	      (if stop-at-invalid?
		  (values (decoding-status invalid)
			  index target-index)
		  (loop (+ 1 index) (+ 1 target-index))))))))))

; Returns three values:
; - :DECODING-STATUS object
; - # bytes consumed
; - # characters decoded

(define (decode-string/utf-8 bytes start count
			     target target-start target-count
			     maybe-error-char)
  (let loop ((index 0)
	     (target-index 0))
    (cond
     ((>= index count)
      (values (decoding-status complete)
	      index
	      target-index))
     ((>= target-index target-count)
      (values (decoding-status insufficient)
	      index target-index))
     (else
      (call-with-values
	  (lambda ()
	    (decode-char/utf-8 bytes
			       (+ start index)
			       (- count index)))
	(lambda (status c consumed)
	  (cond
	   ((eq? status (decoding-status complete))
	    (string-set! target (+ target-start target-index) c)
	    (loop (+ index consumed) (+ target-index 1)))
	   ((eq? status (decoding-status incomplete))
	    (values (decoding-status incomplete)
		    index target-index))
	   ((eq? status (decoding-status invalid))
	    (if maybe-error-char
		(begin
		  (string-set! target (+ target-start target-index) maybe-error-char)
		  (loop (+ 1 index) (+ 1 target-index)))
		(values (decoding-status invalid)
			index target-index))))))))))

; may be slightly faster because of REVERSE-LIST->STRING
; If MAYBE-ERROR-CHAR is #f, we'll raise an error upon an invalid encoding
; If it's a character, it will be used at invalid *and incomplete* encodings

(define (utf-8->string source maybe-error-char)
  (utf-8->string-n source 0 (byte-vector-length source) maybe-error-char))

(define (utf-8->string-n source start source-count maybe-error-char)
  (let loop ((rev-chars '())
	     (char-count 0)
	     (source-index 0))
    (if (>= source-index source-count)
	(reverse-list->string rev-chars char-count)
	(call-with-values
	    (lambda ()
	      (decode-char/utf-8 source
				 (+ start source-index)
				 (- source-count source-index)))
	  (lambda (status c consumed)
	    (cond
	     ((eq? status (decoding-status complete))
	      (loop (cons c rev-chars)
		    (+ 1 char-count)
		    (+ consumed source-index)))
	     ((eq? status (decoding-status invalid))
	      (if maybe-error-char
		  (loop (cons maybe-error-char rev-chars)
			(+ 1 char-count)
			(+ 1 source-index))
		  (decoding-error "UTF-8"
				  "invalid encoding"
				  source (+ start source-index))))
	     ((eq? status (decoding-status incomplete))
	      (if maybe-error-char
		  (let loop ((error-char-count (- source-count consumed))
			     (rev-chars '()))
		    (if (zero? error-char-count)
			(reverse-list->string rev-chars (+ 1 char-count))
			(loop (- error-char-count 1) (cons maybe-error-char rev-chars))))
		  (decoding-error "UTF-8"
				  "incomplete encoding"
				  source (+ start source-index))))))))))

