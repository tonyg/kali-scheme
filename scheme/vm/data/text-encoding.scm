; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (encode-scalar-value encoding value buffer start count)
  (let-syntax ((encode
		(syntax-rules ()
		  ((encode ?encode-proc)
		   (call-with-values
		       (lambda ()
			 (?encode-proc value buffer start count))
		     (lambda (encoding-ok? out-of-space? count)
		       (values #t encoding-ok? out-of-space? count)))))))
    (enum-case
     text-encoding-option encoding
     ((us-ascii) (encode encode-scalar-value/us-ascii))
     ((latin-1) (encode encode-scalar-value/latin-1))
     ((utf-8) (encode encode-scalar-value/utf-8))
     ((utf-16le) (encode encode-scalar-value/utf-16le))
     ((utf-16be) (encode encode-scalar-value/utf-16be))
     ((utf-32le) (encode encode-scalar-value/utf-32le))
     ((utf-32be) (encode encode-scalar-value/utf-32be))
     (else
      (values #f #f #f 0)))))

(define (decode-scalar-value encoding buffer start count)
  (let-syntax ((decode
		(syntax-rules ()
		  ((decode ?decode-proc)
		   (call-with-values
		       (lambda () (?decode-proc buffer start count))
		     (lambda (ok? incomplete? value count)
		       (values #t ok? incomplete? value count)))))))
    (enum-case
     text-encoding-option encoding
     ((us-ascii) (decode decode-scalar-value/us-ascii))
     ((latin-1) (decode decode-scalar-value/latin-1))
     ((utf-8) (decode decode-scalar-value/utf-8))
     ((utf-16le) (decode decode-scalar-value/utf-16le))
     ((utf-16be) (decode decode-scalar-value/utf-16be))
     ((utf-32le) (decode decode-scalar-value/utf-32le))
     ((utf-32be) (decode decode-scalar-value/utf-32be))
     (else
      (values #f #f #f 0 0)))))

;; US-ASCII

;; This is mainly needed because it might be the default locale
;; encoding reported by the OS.

(define (encode-scalar-value/us-ascii value buffer start count)
  (cond
   ((< count 1)
    (values #t #t 1))
   ((< value 128)
    (code-vector-set! buffer start value)
    (values #t #f 1))
   (else
    (values #f #f 0))))

(define (decode-scalar-value/us-ascii buffer start count)
  (values #t ; OK?
	  #f ; incomplete?
	  (code-vector-ref buffer start)
	  1))

; Latin-1

(define (encode-scalar-value/latin-1 value buffer start count)
  (cond
   ((< count 1)
    (values #t #t 1))
   ((< value 256)
    (code-vector-set! buffer start value)
    (values #t #f 1))
   (else
    (values #f #f 0))))

(define (decode-scalar-value/latin-1 buffer start count)
  (values #t ; OK?
	  #f ; incomplete?
	  (code-vector-ref buffer start)
	  1))
; UTF-8

(define (encode-scalar-value/utf-8 value buffer start count)
  (cond
   ((<= value #x7f)
    (if (>= count 1)
	(begin
	  (code-vector-set! buffer start value)
	  (values #t #f 1))
	(values #t #t 1)))
   ((<= value #x7ff)
    (if (>= count 2)
	(begin
	  (code-vector-set!
	   buffer start
	   (+ #xc0
	      (logical-shift-right (bitwise-and value #b11111000000)
				   6)))
	  (code-vector-set!
	   buffer (+ 1 start)
	   (+ #x80
	      (bitwise-and value #b111111)))
	  (values #t #f 2))
	(values #t #t 2)))
   ((<= value #xffff)
    (if (>= count 3)
	(begin
	  (code-vector-set!
	   buffer start
	   (+ #xe0
	      (logical-shift-right (bitwise-and value #b1111000000000000)
				   12)))
	  (code-vector-set!
	   buffer (+ 1 start)
	   (+ #x80
	      (logical-shift-right (bitwise-and value #b111111000000)
				   6)))
	  (code-vector-set!
	   buffer (+ 2 start)
	   (+ #x80
	      (bitwise-and value #b111111)))
	  (values #t #f 3))
	(values #t #t 3)))
   (else
    (if (>= count 4)
	(begin
	  (code-vector-set!
	   buffer start
	   (+ #xf0
	      (logical-shift-right (bitwise-and value #b111000000000000000000)
				   18)))
	  (code-vector-set!
	   buffer (+ 1 start)
	   (+ #x80
	      (logical-shift-right (bitwise-and value #b111111000000000000)
				   12)))
	  (code-vector-set!
	   buffer (+ 2 start)
	   (+ #x80
	      (logical-shift-right (bitwise-and value #b111111000000)
				   6)))
	  (code-vector-set!
	   buffer (+ 3 start)
	   (+ #x80
	      (bitwise-and value #b111111)))
	  (values #t #f 4))
	(values #t #t 4)))))


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

(define (decode-scalar-value/utf-8 buffer start count)
  (let loop ((q 0) (state 0) (mask 0) (scalar-value 0))
    (if (< q count)
	(let* ((c (code-vector-ref buffer (+ start q)))
	       (state (vector-ref *utf-8-state-table*
				  (+ (shift-left state 5)	; (* state 32)
				     (arithmetic-shift-right c 3)))))
	  (case state
	    ((0)
	     (let ((scalar-value (+ scalar-value
				    (bitwise-and c #x7f))))
	       (if (scalar-value? scalar-value)
		   (values #t #f scalar-value (+ q 1))
		   (values #f #f 0 0))))
	    ((1 2 3)
	     (loop (+ 1 q) state #x3f
		   (shift-left (+ scalar-value
				  (bitwise-and c
					       (if (= 0 mask)
						   (vector-ref *utf-8-masks* state)
						   mask)))
			       6)))
	    ((-2 -1)
	     (values #f #f 0 0))
	    (else ; this can't happen
	     (values #f #f 0 0))))
	(values #t #t 0 (+ 1 q)))))

; UTF-16

(define (code-vector-set-word16/le! buffer start word)
  (code-vector-set! buffer start
		    (bitwise-and #b11111111 word))
  (code-vector-set! buffer (+ 1 start)
		    (logical-shift-right word 8)))

(define (code-vector-set-word16/be! buffer start word)
  (code-vector-set! buffer start
		    (logical-shift-right word 8))
  (code-vector-set! buffer (+ 1 start)
		    (bitwise-and #b11111111 word)))

(define (make-encode-scalar-value/utf-16 code-vector-set-word16!)
  (lambda (value buffer start count)
    (if (<= value #xffff)
	(if (< count 2)
	    (values #t #t 2)
	    (begin
	      (code-vector-set-word16! buffer start value)
	      (values #t #f 2)))
	(if (< count 4)
	    (values #t #t 4)
	    (begin
	      (code-vector-set-word16!
	       buffer start
	       (+ (logical-shift-right value 10) #xd7c0))
	      (code-vector-set-word16!
	       buffer (+ 2 start)
	       (+ (bitwise-and value #x3ff) #xdc00))
	      (values #t #f 4))))))

(define encode-scalar-value/utf-16le
  (make-encode-scalar-value/utf-16 code-vector-set-word16/le!))
(define encode-scalar-value/utf-16be
  (make-encode-scalar-value/utf-16 code-vector-set-word16/be!))

(define (code-vector-ref-word16/le codes start)
  (+ (code-vector-ref codes start)
     (shift-left (code-vector-ref codes (+ 1 start))
		 8)))

(define (code-vector-ref-word16/be codes start)
  (+ (shift-left (code-vector-ref codes start)
		 8)
     (code-vector-ref codes (+ 1 start))))

(define (make-decode-scalar-value/utf-16 code-vector-ref-word16)
  (lambda (buffer start count)
    (if (< count 2)
	(values #t #t 0 2)
	(let ((word0 (code-vector-ref-word16 buffer start)))
	  (cond
	   ((or (< word0 #xd800)
		(> word0 #xdfff))
	    (values #t #f word0 2))
	   ((< count 4)
	    (values #t #t 0 4))
	   ((<= word0 #xdbff)
	    (let ((word1 (code-vector-ref-word16 buffer (+ 2 start))))
	      (if (and (>= word1 #xdc00)
		       (<= word1 #xdfff))
		  (values #t #f
			  (+ (shift-left (- word0 #xd7c0) 10)
			     (bitwise-and word1 #x3ff))
			  4)
		  (values #f #f 0 0))))
	   (else
	    (values #f #f 0 0)))))))

(define decode-scalar-value/utf-16le
  (make-decode-scalar-value/utf-16 code-vector-ref-word16/le))
(define decode-scalar-value/utf-16be
  (make-decode-scalar-value/utf-16 code-vector-ref-word16/be))

; UTF-32

(define (encode-scalar-value/utf-32le value buffer start count)
  (if (< count 4)
      (values #t #t 4)
      (begin
	(code-vector-set! buffer start
			  (bitwise-and value #xff))
	(code-vector-set! buffer (+ 1 start)
			  (logical-shift-right
			   (bitwise-and value #xff00)
			   8))
	(code-vector-set! buffer (+ 2 start)
			  (logical-shift-right
			   (bitwise-and value #xff0000)
			   16))
	(code-vector-set! buffer (+ 3 start)
			  (logical-shift-right value 24))
	(values #t #f 4))))

(define (encode-scalar-value/utf-32be value buffer start count)
  (if (< count 4)
      (values #t #t 4)
      (begin
	(code-vector-set! buffer start
			  (logical-shift-right value 24))
	(code-vector-set! buffer (+ 1 start)
			  (logical-shift-right
			   (bitwise-and value #xff0000)
			   16))
	(code-vector-set! buffer (+ 2 start)
			  (logical-shift-right
			   (bitwise-and value #xff00)
			   8))
	(code-vector-set! buffer (+ 3 start)
			  (bitwise-and value #xff))
	(values #t #f 4))))

(define (decode-scalar-value/utf-32le buffer start count)
  (if (< count 4)
      (values #t #t 0 4)
      (let ((code-point
	     (+ (code-vector-ref buffer start)
		(shift-left (code-vector-ref buffer (+ 1 start))
			    8)
		(shift-left (code-vector-ref buffer (+ 2 start))
			    16)
		(shift-left (code-vector-ref buffer (+ 3 start))
			    24))))
	(if (scalar-value? code-point)
	    (values #t #f 
		    code-point
		    4)
	    (values #f #f 0 0)))))

(define (decode-scalar-value/utf-32be buffer start count)
  (if (< count 4)
      (values #t #t 0 4)
      (let ((code-point
	     (+ (shift-left (code-vector-ref buffer start)
			    24)
		(shift-left (code-vector-ref buffer (+ 1 start))
			    16)
		(shift-left
		 (code-vector-ref buffer (+ 2 start))
		 8)
		(code-vector-ref buffer (+ 3 start)))))
	(if (scalar-value? code-point)
	    (values #t #f
		    code-point
		    4)
	    (values #f #f 0 0)))))

; Utilities

(define (scalar-value? x)
  (and (>= x 0)
       (or (<= x #xd7ff)
	   (and (>= x #xe000) (<= x #x10ffff)))))
