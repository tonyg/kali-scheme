; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Character/string encodings

; We abstract over the primitive encode-char/decode-char characters to
; get two sets of procedures, one going through the general
; text-encoding infrastructure, and the other making use of the VM
; instructions.

(define-syntax define-coding-procs
  (syntax-rules ()
    ((define-coding-procs
       (do-encode-char do-decode-char)
       char-encoding-length
       string-encoding-length
       encode-char
       encode-string
       string->bytes-n
       string->bytes
       decode-char
       bytes-string-size
       decode-string
       bytes->string
       bytes->string-n)
     (begin

(define (char-encoding-length enc c)
  (call-with-values
      (lambda ()
	(do-encode-char enc c empty-buffer 0 0))
    (lambda (ok? count)
      count)))

(define (string-encoding-length enc s start-index count)
  (let loop ((enc-length 0)
	     (char-index 0))
    (if (>= char-index count)
	enc-length
	(loop (+ enc-length 
		 (char-encoding-length enc (string-ref s (+ start-index char-index))))
	      (+ 1 char-index)))))

; returns byte count of the encoding
(define (encode-char enc c target target-start)
  (call-with-values
      (lambda ()
	(do-encode-char enc
			c target target-start
			(- (byte-vector-length target) target-start)))
    (lambda (ok? count)
      count)))

; Will only produce complete encodings
; returns three values:
; - encoding status
; - # characters consumed
; - # bytes decoded

(define (encode-string enc source source-start source-count
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
	      (do-encode-char enc
			      c
			      target (+ target-start target-index)
			      (max 0 (- target-count target-index))))
	  (lambda (ok? count)
	    (if (not ok?)
		(values (encoding-status insufficient)
			source-index
			target-index)
		(loop (+ source-index 1) (+ target-index count))))))))))

(define (string->bytes-n enc s start count)
  (let* ((size (string-encoding-length enc s 0 count))
	 (result (make-byte-vector size 0)))
    (encode-string enc s 0 count result 0 size)
    result))

(define (string->bytes enc s)
  (string->bytes-n enc s 0 (string-length s)))

; Decoding

; Returns three values:
; - decoding status
; - character if status is COMPLETE, else #f
; - # bytes consumed if COMPLETE or INCOMPLETE, else #f

(define (decode-char enc bytes start-index count)
  (call-with-values
      (lambda ()
	(do-decode-char enc
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

(define (bytes-string-size enc bytes start count stop-at-invalid?)
  (let loop ((index 0)
	     (target-index 0))
    (if (>= index count)
	(values (decoding-status complete)
		index target-index)
	(call-with-values
	    (lambda ()
	      (do-decode-char enc
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

(define (decode-string enc
		       bytes start count
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
	    (do-decode-char enc
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

(define (bytes->string enc source maybe-error-char)
  (bytes->string-n enc source 0 (byte-vector-length source) maybe-error-char))

(define (bytes->string-n enc source start source-count maybe-error-char)
  (let loop ((rev-chars '())
	     (char-count 0)
	     (source-index 0))
    (if (>= source-index source-count)
	(reverse-list->string rev-chars char-count)
	(call-with-values
	    (lambda ()
	      (do-decode-char enc
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
		  (decoding-error enc	; ####
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
	      (decoding-error enc	; ####
			      "incomplete encoding"
			      source (+ start source-index)))))))))

))))

(define-coding-procs (primitive-encode-char primitive-decode-char)
  char-encoding-length/encoding
  string-encoding-length/encoding
  encode-char/encoding
  encode-string/encoding
  string->bytes-n/encoding
  string->bytes/encoding
  decode-char/encoding
  bytes-string-size/encoding
  decode-string/encoding
  bytes->string/encoding
  bytes->string-n/encoding)

(define-syntax primitive-encode-char/text-codec
  (syntax-rules ()
    ((encode-char/text-codec enc ch buffer start count)
     (atomically
      ((text-codec-encode-char-proc enc) ch buffer start count)))))

(define-syntax primitive-decode-char/text-codec
  (syntax-rules ()
    ((decode-char/text-codec enc buffer start count)
     (atomically
      ((text-codec-decode-char-proc enc) buffer start count)))))

(define-coding-procs (primitive-encode-char/text-codec primitive-decode-char/text-codec)
  char-encoding-length/text-codec
  string-encoding-length/text-codec
  encode-char/text-codec
  encode-string/text-codec
  string->bytes-n/text-codec
  string->bytes/text-codec
  decode-char/text-codec
  bytes-string-size/text-codec
  decode-string/text-codec
  bytes->string/text-codec
  bytes->string-n/text-codec)

(define-syntax define-text-codec-proc
  (syntax-rules ()
    ((define-text-codec-proc (?name ?arg ...) ?name/codec ?name/encoding)
     (define (?name codec ?arg ...)
       (let ((spec (text-codec->spec codec)))
	 (if (text-codec? spec)
	     (?name/codec spec ?arg ...)
	     (?name/encoding spec ?arg ...)))))))

(define-text-codec-proc (char-encoding-length c)
  char-encoding-length/text-codec char-encoding-length/encoding)
(define-text-codec-proc (string-encoding-length s start-index count)
  string-encoding-length/text-codec string-encoding-length/encoding)
(define-text-codec-proc (encode-char c target target-start)
  encode-char/text-codec encode-char/encoding)
(define-text-codec-proc (encode-string source source-start source-count
				       target target-start target-count)
  encode-string/text-codec encode-string/encoding)
(define-text-codec-proc (string->bytes-n s start count)
  string->bytes-n/text-codec string->bytes-n/encoding)
(define-text-codec-proc (string->bytes s)
  string->bytes/text-codec string->bytes/encoding)
(define-text-codec-proc (decode-char bytes start-index count)
  decode-char/text-codec decode-char/encoding)
(define-text-codec-proc (bytes-string-size bytes start count stop-at-invalid?)
  bytes-string-size/text-codec bytes-string-size/encoding)
(define-text-codec-proc (decode-string bytes start count
				       target target-start target-count
				       maybe-error-char)  
  decode-string/text-codec decode-string/encoding)
(define-text-codec-proc (bytes->string source maybe-error-char)
  bytes->string/text-codec bytes->string/encoding)
(define-text-codec-proc (bytes->string-n source start source-count maybe-error-char)
  bytes->string-n/text-codec bytes->string-n/encoding)

;; Utilities

(define empty-buffer (make-byte-vector 0 0))

(define-enumerated-type encoding-status :encoding-status
  encoding-status?
  encoding-statuses
  encoding-status-name
  encoding-status-index
  (complete insufficient))

(define-condition-type &decoding-error &error
  decoding-error?
  (encoding-name decoding-error-encoding-name))

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

;; UTF-8

(define (char-encoding-length/utf-8 c)
  (char-encoding-length/encoding (enum text-encoding-option utf-8) c))

(define (string-encoding-length/utf-8 s start-index count)
  (string-encoding-length/encoding (enum text-encoding-option utf-8) 
				   s start-index count))

(define (encode-char/utf-8 c target target-start)
  (encode-char/encoding (enum text-encoding-option utf-8) c target target-start))

(define (encode-string/utf-8 source source-start source-count
			     target target-start target-count)
  (encode-string/encoding (enum text-encoding-option utf-8)
			  source source-start source-count
			  target target-start target-count))

(define (string->utf-8-n s start count)
  (string->bytes-n/encoding (enum text-encoding-option utf-8) s start count))

(define (string->utf-8 s)
  (string->bytes/encoding (enum text-encoding-option utf-8) s))

(define (decode-char/utf-8 bytes start-index count)
  (decode-char/encoding (enum text-encoding-option utf-8) bytes start-index count))

(define (bytes-string-size/utf-8 bytes start count stop-at-invalid?)
  (bytes-string-size/encoding (enum text-encoding-option utf-8)
			      bytes start count stop-at-invalid?))

(define (decode-string/utf-8 bytes start count
			     target target-start target-count
			     maybe-error-char)
  (decode-string/encoding (enum text-encoding-option utf-8)
			  bytes start count
			  target target-start target-count
			  maybe-error-char))

(define (utf-8->string source maybe-error-char)
  (bytes->string/encoding (enum text-encoding-option utf-8)
			  source maybe-error-char))

(define (utf-8->string-n source start source-count maybe-error-char)
  (bytes->string-n/encoding (enum text-encoding-option utf-8)
			    source start source-count maybe-error-char))
