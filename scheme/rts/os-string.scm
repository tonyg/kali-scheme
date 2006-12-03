; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; You may think that file names / environment variables / user names
; etc. are just text, but on most platforms, that assumption is wrong:
; They are usually NUL-terminated byte strings in some format.  The
; bytes are invariant, but the corresponding text may depend on the
; locale.  Also, byte sequences without a textual representation are
; possible.

; We assume that OS strings are encoded in some conservative extension
; of NUL-terminated ASCII.  On Unix, this assumption pretty much has
; to hold true because of the various constraints of locale handling
; there.  The Windows API uses an extension of UTF-16 that includes
; unpaired surrogates.  For this, we use a synthetic extension of
; UTF-8 called UTF-8of16 that also deals with unpaired surrogates.

; #### lossiness

(define-record-type os-string :os-string
  (make-os-string text-codec string byte-vector)
  os-string?
  (text-codec os-string-text-codec)
  ; may be #f, will get cached value
  (string os-string-string set-os-string-string!)
  ; may be #f, will get cached value
  (byte-vector os-string-byte-vector set-os-string-byte-vector!))

(define-record-discloser :os-string
  (lambda (oss)
    (list "OS-string"
	  (text-codec-names (os-string-text-codec oss))
	  (os-string->string oss))))

(define $os-string-text-codec
  (make-fluid (find-text-codec
	       (system-parameter (enum system-parameter-option os-string-encoding)))))

(define (call-with-os-string-text-codec codec thunk)
  (let-fluid $os-string-text-codec codec
	     thunk))
  
(define (string->os-string s)
  (let ((c (string-copy s)))
    (make-immutable! c)
    (make-os-string (fluid $os-string-text-codec)
		    c #f)))

(define (byte-vector->os-string b)
  (let ((c (byte-vector-copy b)))
    (make-immutable! b)
    (make-os-string (fluid $os-string-text-codec)
		    #f c)))

(define (os-string->byte-vector oss)
  (or (os-string-byte-vector oss)
      (let* ((string (os-string-string oss))
	     (codec (os-string-text-codec oss))
	     (size (string-encoding-length codec
					   string
					   0
					   (string-length string)))
	     (bytes (make-byte-vector (+ size 1) 0))) ; NUL termination
	(encode-string codec
		       string 0 (string-length string)
		       bytes 0 size)
	(set-os-string-byte-vector! oss bytes)
	(make-immutable! bytes)
	bytes)))

(define (os-string->string oss)
  (or (os-string-string oss)
      (let* ((bytes (os-string-byte-vector oss))
	     (size (- (byte-vector-length bytes) 1))
	     (codec (os-string-text-codec oss)))
	(call-with-values
	    (lambda ()
	      (bytes-string-size codec bytes 0 size #f))
	  (lambda (status consumed-count decoded-count)
	    (let ((string (make-string decoded-count)))
	      (decode-string codec bytes 0 size
			     string 0 decoded-count
			     #\?)
	      (set-os-string-string! oss string)
	      (make-immutable! string)
	      string))))))

(define (x->os-string x)
  (cond
   ((os-string? x) x)
   ((string? x) (string->os-string x))
   ((byte-vector? x) (byte-vector->os-string x))))

; doesn't really belong here

(define (byte-vector-copy b)
  (let* ((size (byte-vector-length b))
	 (result (make-byte-vector size 0)))
    (copy-bytes! b 0 result 0 size)
    result))
