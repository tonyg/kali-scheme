; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; You may think that file names / environment variables / user names
; etc. are just text, but on most platforms, that assumptions, that
; assumption is wrong: They are usually NUL-terminated byte strings in
; some format.  The bytes are invariant, but the corresponding text
; may depend on the locale.  Also, byte sequences without a textual
; representation are possible.

; Therefore, we introduce thin abstract data types for these things
; (called "string/bytes") that remember how it was created.

; Note that a string/bytes object created from a string may denote different
; files depending on locale.  Mike isn't sure whether this is a good
; idea.  (PLT Scheme converts at creation time, but this comes with a
; different set of problems.)

(define-syntax define-string/bytes-type 
  (syntax-rules ()
    ((define-string/bytes-type
       ?disclose-name ?type-name
       ?predicate

       ;; these match the interface of the corresponding procedures in
       ;; DEFAULT-STRING-ENCODINGS
       ?string-encoding-length ?encode-string
       ?string-decoding-length ?decode-string

       ?thing->string/bytes
       ?string->string/bytes
       ?byte-vector->string/bytes

       ?string/bytes->string
       ?string/bytes->byte-vector ?string/bytes->byte-string)
     (begin
       (define-record-type ?disclose-name ?type-name
	 (make-string/bytes data)
	 ?predicate
	 ;; this is either a byte vector or a string
	 (data string/bytes-data))
       
       (define-record-discloser ?type-name
	 (lambda (r)
	   (list '?disclose-name
		 (?string/bytes->string r))))

       (define (?string->string/bytes s)
	 (make-string/bytes s))
       
       (define (?byte-vector->string/bytes bytes)
	 (make-string/bytes bytes))

       (define (?thing->string/bytes thing)
	 (cond
	  ((?predicate thing) thing)
	  ((string? thing)
	   (make-string/bytes thing))
	  ((byte-vector? thing)
	   (make-string/bytes thing))
	  (else
	   (call-error "can't convert" ?thing->string/bytes thing))))
  
       (define ?string/bytes->string
	 (make-string/bytes->string string/bytes-data
				    ?string-decoding-length ?decode-string))

       (define ?string/bytes->byte-vector
	 (make-string/bytes->byte-vector string/bytes-data
					 ?string-encoding-length ?encode-string))

       (define ?string/bytes->byte-string
	 (make-string/bytes->byte-string string/bytes-data
					 ?string-encoding-length ?encode-string))))))


(define (make-string/bytes->string string/bytes-data
				   string-decoding-length decode-string)
  (lambda (f)
    (let ((data (string/bytes-data f)))
      (cond
       ((string? data) data)
       ((byte-vector? data)
	(let* ((size (byte-vector-length data))
	       (text-byte-count
		(if (zero? (byte-vector-ref data (- size 1)))
		    (- size 1)
		    size))
	       (string-size (string-decoding-length data 0 text-byte-count))
	       (string (make-string string-size)))
	  (decode-string data 0 text-byte-count string 0)
	  string))))))

(define (make-string/bytes->byte-vector string/bytes-data
					string-encoding-length encode-string)
  (lambda (f)
    (let ((data (string/bytes-data f)))
      (cond
       ((byte-vector? data) data)
       ((string? data)
	(let* ((len (string-length data))
	       (size (string-encoding-length f 0 len))
	       (bytes (make-byte-vector size 0)))
	  (encode-string data 0 len bytes 0)
	  bytes))))))

; Byte strings are NUL-terminated byte vectors.  This is for use by
; for NUL-terminated file name representation by the code that knows
; the representation.

(define (make-string/bytes->byte-string string/bytes-data
					string-encoding-length encode-string)
  (lambda (f)
    (let ((data (string/bytes-data f)))
      (cond
       ((byte-vector? data) data)
       ((string? data)
	(let* ((len (string-length data))
	       (size (string-encoding-length f 0 len))
	       (bytes (make-byte-vector (+ 1 size) 0)))
	  (encode-string data 0 len bytes 0)
	  bytes))))))
