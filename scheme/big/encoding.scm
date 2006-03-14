; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Common character/string encodings

; UTF-8

(define empty-buffer (make-byte-vector 0 0))

(define (char-encoding-length/utf-8 c)
  (call-with-values
      (lambda ()
	(encode-char (enum text-encoding-option utf-8)
		     c empty-buffer 0 0))
    (lambda (ok? count)
      count)))

(define (string-encoding-length/utf-8 s start-index count)
  (let loop ((utf-8-length 0)
	     (char-index 0))
    (if (>= char-index count)
	utf-8-length
	(loop (+ utf-8-length 
		 (char-encoding-length/utf-8 (string-ref s (+ start-index char-index))))
	      (+ 1 char-index)))))

; returns byte count of the encoding
(define (encode-char/utf-8 c target target-start)
  (call-with-values
      (lambda ()
	(encode-char (enum text-encoding-option utf-8)
		     c target target-start
		     (- (byte-vector-length target) target-start)))
    (lambda (ok? count)
      count)))

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
      (let ((c (string-ref source (+ source-start source-index))))
	(call-with-values
	    (lambda ()
	      (encode-char (enum text-encoding-option utf-8)
			   c
			   target (+ target-start target-index)
			   (max 0 (- target-count target-index))))
	  (lambda (ok? count)
	    (if (not ok?)
		(values (encoding-status insufficient)
			source-index
			target-index)
		(loop (+ source-index 1) (+ target-index count))))))))))

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

; Returns three values:
; - decoding status
; - character if status is COMPLETE, else #f
; - # bytes consumed if COMPLETE or INCOMPLETE, else #f

; We don't check for non-shortest-form UTF-8.  Too bad.

(define (decode-char/utf-8 bytes start-index count)
  (call-with-values
      (lambda ()
	(decode-char (enum text-encoding-option utf-8)
		     bytes start-index
		     count))
    (lambda (maybe-char count)
      (cond
       (maybe-char
	(values (decoding-status complete)
	       maybe-char
	       count))
       (count
	(values (decoding-status incomplete)
		#f
		count))
       (else
	(values (decoding-status invalid)
		#f
		#f))))))

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
	      (decode-char (enum text-encoding-option utf-8)
			   bytes
			   (+ start index)
			   (- count index)))
	  (lambda (char count)
	    (cond
	     (char
	      (loop (+ index count) (+ target-index 1)))
	     (count
	      (values (decoding-status incomplete)
		      index target-index))
	     (stop-at-invalid?
	      (values (decoding-status invalid)
		      index target-index))
	     (else
	      (loop (+ 1 index) (+ 1 target-index)))))))))

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
	    (decode-char (enum text-encoding-option utf-8)
			 bytes
			 (+ start index)
			 (- count index)))
	(lambda (char count)
	  (cond
	   (char
	    (string-set! target (+ target-start target-index) char)
	    (loop (+ index count) (+ target-index 1)))
	   (count
	    (values (decoding-status incomplete)
		    index target-index))
	   (maybe-error-char
	    (string-set! target (+ target-start target-index) maybe-error-char)
	    (loop (+ 1 index) (+ 1 target-index)))
	   (else
	    (values (decoding-status invalid)
		    index target-index)))))))))

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
	      (decode-char (enum text-encoding-option utf-8)
			   source
			   (+ start source-index)
			   (- source-count source-index)))
	  (lambda (char count)
	    (cond
	     (char
	      (loop (cons char rev-chars)
		    (+ 1 char-count)
		    (+ count source-index)))
	     ((not count)
	      (if maybe-error-char
		  (loop (cons maybe-error-char rev-chars)
			(+ 1 char-count)
			(+ 1 source-index))
		  (decoding-error "UTF-8"
				  "invalid encoding"
				  source (+ start source-index))))
	     ;; it's incomplete from here
	     (maybe-error-char
	      (let loop ((error-char-count (- source-count count))
			 (rev-chars '()))
		(if (zero? error-char-count)
		    (reverse-list->string rev-chars (+ 1 char-count))
		    (loop (- error-char-count 1) (cons maybe-error-char rev-chars)))))
	     (else
	      (decoding-error "UTF-8"
			      "incomplete encoding"
			      source (+ start source-index)))))))))

