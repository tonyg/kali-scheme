; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


;;;; Portable definitions of char->ascii and ascii->char

; Don't detabify this file!

; This module defines char->ascii and ascii->char in terms of
; char->integer and integer->char, with no assumptions about the encoding.
; Portable except maybe for the strings that contain tab, page, and
; carriage return characters.  Those can be flushed if necessary.

(define ascii-limit 128)

(define ascii-chars
  (let* ((ascii-chars (make-vector ascii-limit #f))
	 (unusual (lambda (s)
		    (if (or (not (= (string-length s) 1))
			    (let ((c (string-ref s 0)))
			      (or (char=? c #\space)
				  (char=? c #\newline))))
			(error "unusual whitespace character lost" s)
			s)))
	 (init (lambda (i s)
		 (do ((i i (+ i 1))
		      (j 0 (+ j 1)))
		     ((= j (string-length s)))
		   (vector-set! ascii-chars i (string-ref s j))))))
    (init 9 (unusual "	"))   ;tab
    (init 12 (unusual ""))  ;page
    (init 13 (unusual ""))  ;carriage return
    (init 10 (string #\newline))
    (init 32 " !\"#$%&'()*+,-./0123456789:;<=>?")
    (init 64 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_")
    (init 96 "`abcdefghijklmnopqrstuvwxyz{|}~")
    ascii-chars))

(define (ascii->char n)
  (or (vector-ref ascii-chars n)
      (error "not a standard character's ASCII code" n)))

(define native-chars
  (let ((end (vector-length ascii-chars)))
    (let loop ((i 0)
	       (least    #f)
	       (greatest #f))
      (cond ((= i end)
	     (let ((v (make-vector (+ (- greatest least) 1) #f)))
	       (do ((i 0 (+ i 1)))
		   ((= i end) (cons least v))
		 (let ((c (vector-ref ascii-chars i)))
		   (if c
		       (vector-set! v (- (char->integer c) least) i))))))
	    (else
	     (let ((c (vector-ref ascii-chars i)))
	       (if c
		   (let ((n (char->integer c)))
		     (loop (+ i 1)
			   (if least    (min least    n) n)
			   (if greatest (max greatest n) n)))
		   (loop (+ i 1) least greatest))))))))

(define (char->ascii char)
  (or (vector-ref (cdr native-chars)
		  (- (char->integer char) (car native-chars)))
      (error "not a standard character" char)))

(define ascii-whitespaces '(32 10 9 12 13)) ;space linefeed tab page return
