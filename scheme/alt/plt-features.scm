; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file plt-features.scm.
; Synchronize any changes with all the other *-features.scm files.

; SIGNALS

; ERROR is built-in

(define (format-error-message message irritants)
  (apply string-append message
	 (map (lambda (irritant)
		(string-append " " ((error-value->string-handler) irritant 1000)))
	      irritants)))

(define (warn message . irritants)
  (void)
  (display (format-error-message message irritants)
	   (current-error-port))
  (newline (current-error-port))
)

(define (signal type . stuff)
  (apply warn "condition signalled" type stuff))

(define (syntax-error . rest)		; Must return a valid expression.
  (apply warn rest)
  ''syntax-error)

(define (call-error message proc . args)
  (error message (cons proc args)))


; FEATURES

(define force-output flush-output)

(define current-noise-port current-error-port)

(define (string-hash s) (abs (equal-hash-code s)))

(define (make-immutable! thing) thing) ; PLT can only do this upon construction
; IMMUTABLE? is built in


; BITWISE

; ARITHMETIC-SHIFT is built-in
; BITWISE-AND is built-in
; BITWISE-IOR is built-in
; BITWISE-NOT is built-in


; ASCII

(define char->ascii char->integer)
(define ascii->char integer->char)
(define ascii-limit 127)
(define ascii-whitespaces '(32 10 9 12 13))

; CELLS

(define make-cell box)
(define cell-ref unbox)
(define cell-set! set-box!)

; CODE-VECTORS

(define make-code-vector make-bytes)

(define code-vector? bytes?)

(define code-vector-ref bytes-ref)

(define code-vector-set! bytes-set!)

(define code-vector-length bytes-length)

; BINARY I/O

; WRITE-BYTE is built-in
