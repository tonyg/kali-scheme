; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Encoders/decoders from text to bytes and vice versa, for use by the
; the ports subsystem.

(define-record-type text-codec :text-codec
  (make-text-codec name write-char-proc input-char-proc)
  text-codec?
  (name text-codec-name)
  ;; (char port) -> ()
  (write-char-proc text-codec-write-char-proc)
  ;; (port read?) -> char or (eof-object)
  (input-char-proc text-codec-input-char-proc))

(define null-text-codec
  (make-text-codec "null"
		   (lambda (char port)
		     (unspecific))
		   (lambda (port read?)
		     (eof-object))))

;; Latin 1

(define *?-byte* (char->ascii #\?))

(define (write-char/latin-1 char port)
  (let ((scalar-value (char->scalar-value char)))
    (write-byte (if (<= scalar-value 256)
		    scalar-value
		    *?-byte*)
		port)))

(define (input-char/latin-1 port read?)
  (let ((byte (if read? (read-byte port) (peek-byte port))))
    (if (eof-object? byte)
	byte
	(scalar-value->char byte))))

(define latin-1-codec (make-text-codec "ISO8859-1"
				      write-char/latin-1
				      input-char/latin-1))

;; UTF 8

(define (write-char/utf-8 char port)
  (let ((scalar-value (char->scalar-value char)))
    (cond
     ((<= scalar-value #x7f)
      (write-byte scalar-value port))
     ((<= scalar-value #x7ff)
      (write-byte (+ #xc0
		     (arithmetic-shift (bitwise-and scalar-value #b11111000000)
				       -6))
		  port)
      (write-byte (+ #x80
		     (bitwise-and scalar-value #b111111))
		  port))
     ((<= scalar-value #xffff)
      (write-byte (+ #xe0
		     (arithmetic-shift (bitwise-and scalar-value #b1111000000000000)
				       -12))
		  port)
      (write-byte (+ #x80
		     (arithmetic-shift (bitwise-and scalar-value #b111111000000)
				       -6))
		  port)
      (write-byte (+ #x80
		     (bitwise-and scalar-value #b111111))
		  port))
     (else
      (write-byte (+ #xf0
		     (arithmetic-shift (bitwise-and scalar-value #b111000000000000000000)
				       -18))
		  port)
      (write-byte (+ #x80
		     (arithmetic-shift (bitwise-and scalar-value #b111111000000000000)
				       -12))
		  port)
      (write-byte (+ #x80
		     (arithmetic-shift (bitwise-and scalar-value #b111111000000)
				       -6))
		  port)
      (write-byte (+ #x80
		     (bitwise-and scalar-value #b111111))
		  port)))))

; This probably runs like a dog; at least, we want REAL-SKIP-BYTES or
; something.  Also, it would be nice to have this be table-driven or
; something.  Or just do something in PreScheme.

; Also, the standard says we have to actively disallow
; non-shortest-form encodings.  Currently, we don't.

; This is so painful.  We really want to look at the buffer directly
; somehow, but Mike hasn't had it in him yet.

(define (input-char/utf-8 port read?)
  (let ((first (peek-byte port)))
    (cond
     ((eof-object? first)
      (if read? (read-byte port))
      first)
     ((<= first #x7f)
      (if read? (read-byte port))
      (scalar-value->char first))
     ((<= first #xbf)
      (if read? (read-byte port))
      #\?)
     ((<= first #xdf)
      (call-with-values
	  (lambda () (real-peek-bytes 2 port))
	(lambda (first second)
	  (cond
	   ((eof-object? second)
	    (if read? (read-byte port))
	    #\?)
	   ((or (< second #x80)
		(> second #xbf))
	    (if read? (read-byte port))
	    #\?)
	   (else
	    (if read? (real-read-bytes 2 port))
	    (scalar-value->char
	     (bitwise-ior (arithmetic-shift (- first #xc0)
					    6)
			  (- second #x80))))))))
     ((<= first #xef)
      (call-with-values
	  (lambda () (real-peek-bytes 3 port))
	(lambda (first second third)
	  (cond
	   ((or (eof-object? second)
		(eof-object? third))
	    (if read? (read-byte port))
	    #\?)
	   ((or (< second #x80)
		(> second #xbf)
		(< third #x80)
		(> third #xbf))
	    (if read? (read-byte port))
	    #\?)
	   (else
	    (let ((scalar-value
		   (bitwise-ior (arithmetic-shift (- first #xe0)
						  12)
				(arithmetic-shift (- second #x80)
						  6)
				(- third #x80))))
	      (if (scalar-value? scalar-value)
		  (begin
		    (if read? (real-read-bytes 3 port))
		    (scalar-value->char scalar-value))
		  (begin
		    (if read? (read-byte port))
		    #\?))))))))
     ((<= first #xf7)
      (call-with-values
	  (lambda () (real-peek-bytes 4 port))
	(lambda (first second third fourth)
	  (cond
	   ((or (eof-object? second)
		(eof-object? third)
		(eof-object? fourth))
	    (if read? (read-byte port))
	    #\?)
	   ((or (< second #x80)
		(> second #xbf)
		(< third #x80)
		(> third #xbf)
		(< fourth #x80)
		(> fourth #xbf))
	    (if read? (read-byte port))
	    #\?)
	   (else
	    (let ((scalar-value
		   (bitwise-ior (arithmetic-shift (- first #xf0)
						  18)
				(arithmetic-shift (- second #x80)
						  12)
				(arithmetic-shift (- third #x80)
						  6)
				(- fourth #x80))))
	      (if (scalar-value? scalar-value)
		  (begin
		    (if read? (real-read-bytes 3 port))
		    (scalar-value->char scalar-value))
		  (begin
		    (if read? (read-byte port))
		    #\?))))))))
     (else
      (if read? (read-byte port))
      #\?))))

(define utf-8-codec (make-text-codec "UTF-8"
				    write-char/utf-8
				    input-char/utf-8))
