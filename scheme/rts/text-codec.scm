; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Encoders/decoders from text to bytes and vice versa, for use by the
; the ports subsystem.

; Note that encoders and decoders must operate on buffers only
; provisionally.

(define-record-type text-codec :text-codec
  (make-text-codec names
		   encode-char-proc
		   decode-char-proc)
  text-codec?
  (names text-codec-names)
  ;; (char buffer start count) -> (ok? #f or #bytes consumed or #bytes needed)
  (encode-char-proc text-codec-encode-char-proc)
  ;; (buffer start count) -> (char #bytes consumed)
  ;;                      or (#f #total bytes needed at least)
  ;;                      or (#f #f) (failure)
  (decode-char-proc text-codec-decode-char-proc))

(define-record-discloser :text-codec
  (lambda (r)
    (cons 'text-codec (text-codec-names r))))

(define *text-codecs* '())

(define (register-text-codec! codec)
  (set! *text-codecs* (cons codec *text-codecs*)))

(define (find-text-codec name)
  (let loop ((codecs *text-codecs*))
    (cond
     ((null? codecs) #f)
     ((member name (text-codec-names (car codecs)))
      (car codecs))
     (else (loop (cdr codecs))))))

(define-syntax define-text-codec
  (syntax-rules ()
    ((define-text-codec ?id (?name ...) ?encode-proc ?decode-proc)
     (begin
       (define ?id (make-text-codec '(?name ...) ?encode-proc ?decode-proc))
       (register-text-codec! ?id)))
    ((define-text-codec ?id ?name ?encode-proc ?decode-proc)
     (define-text-codec ?id (?name) ?encode-proc ?decode-proc))))
       
(define-text-codec null-text-codec "null"
  (lambda (char buffer start count)
    #f)
  (lambda (buffer start count)
    (values #f #f)))

;; US-ASCII

;; This is mainly needed because it might be the default locale
;; encoding reported by the OS.

(define (encode-char/us-ascii char buffer start count)
  (let ((scalar-value (char->scalar-value char)))
    (if (< scalar-value 128)
	(begin
	  (provisional-byte-vector-set! buffer start scalar-value)
	  (values #t 1))
	(values #f #f))))

(define (decode-char/us-ascii buffer start count)
  (values (scalar-value->char (provisional-byte-vector-ref buffer start))
	  1))

(define-text-codec us-ascii-codec "US-ASCII"
  encode-char/us-ascii
  decode-char/us-ascii)

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

(define-text-codec latin-1-codec "ISO8859-1"
  encode-char/latin-1
  decode-char/latin-1)

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

(define-text-codec utf-8-codec "UTF-8"
  encode-char/utf-8
  decode-char/utf-8)

; UTF-16

(define (provisional-byte-vector-set-word/le! bytes start word)
  (provisional-byte-vector-set! bytes start
				(bitwise-and #b11111111 word))
  (provisional-byte-vector-set! bytes (+ 1 start)
				(arithmetic-shift word -8)))

(define (provisional-byte-vector-set-word/be! bytes start word)
  (provisional-byte-vector-set! bytes start
				(arithmetic-shift word -8))
  (provisional-byte-vector-set! bytes (+ 1 start)
				(bitwise-and #b11111111 word)))

(define (make-encode-char/utf-16 provisional-byte-vector-set-word!)
  (lambda (char buffer start count)
    (let ((scalar-value (char->scalar-value char)))
      (if (<= scalar-value #xffff)
	  (if (< count 2)
	      (values #f 2)
	      (begin
		(provisional-byte-vector-set-word! buffer start scalar-value)
		(values #t 2)))
	  (if (< count 4)
	      (values #f 4)
	      (begin
		(provisional-byte-vector-set-word!
		 buffer start
		 (+ (arithmetic-shift scalar-value -10) #xd7c0))
		(provisional-byte-vector-set-word!
		 buffer (+ 2 start)
		 (+ (bitwise-and scalar-value #x3ff) #xdc00))
		(values #t 4)))))))

(define (provisional-byte-vector-ref-word/le bytes start)
  (+ (provisional-byte-vector-ref bytes start)
     (arithmetic-shift (provisional-byte-vector-ref bytes (+ 1 start))
		       8)))

(define (provisional-byte-vector-ref-word/be bytes start)
  (+ (arithmetic-shift (provisional-byte-vector-ref bytes start)
		       8)
     (provisional-byte-vector-ref bytes (+ 1 start))))

(define (make-decode-char/utf-16 provisional-byte-vector-ref-word)
  (lambda (buffer start count)
    (if (< count 2)
	(values #f 2)
	(let ((word0 (provisional-byte-vector-ref-word buffer start)))
	  (cond
	   ((or (< word0 #xd800)
		(> word0 #xdfff))
	    (values (scalar-value->char word0) 2))
	   ((< count 4)
	    (values #f 4))
	   ((<= word0 #xdbff)
	    (let ((word1 (provisional-byte-vector-ref-word buffer (+ 2 start))))
	      (if (and (>= word1 #xdc00)
		       (<= word1 #xdfff))
		  (values (scalar-value->char
			   (+ (arithmetic-shift (- word0 #xd7c0) 10)
			      (bitwise-and word1 #x3ff)))
			  4)
		  (values #f #f))))
	   (else
	    (values #f #f)))))))

(define-text-codec utf-16le-codec "UTF-16LE"
  (make-encode-char/utf-16 provisional-byte-vector-set-word/le!)
  (make-decode-char/utf-16 provisional-byte-vector-ref-word/le))

(define-text-codec utf-16be-codec "UTF-16BE"
  (make-encode-char/utf-16 provisional-byte-vector-set-word/be!)
  (make-decode-char/utf-16 provisional-byte-vector-ref-word/be))
