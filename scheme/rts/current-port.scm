; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Current input, output, error, and noise ports.

; These two ports are needed by the VM for the READ-BYTE and WRITE-BYTE
; opcodes.
(define $current-input-port  (enum current-port-marker current-input-port))
(define $current-output-port (enum current-port-marker current-output-port))

(define $current-error-port  (make-fluid #f))
(define $current-noise-port  (make-fluid #f))  ; defaults to the error port

(define (current-input-port)
  (fluid $current-input-port))

(define (current-output-port)
  (fluid $current-output-port))

(define (current-error-port)
  (fluid $current-error-port))

(define (current-noise-port)
  (fluid $current-noise-port))

(define (initialize-i/o input output error thunk)
  (with-current-ports input output error thunk))

(define (with-current-ports in out error thunk)
  (let-fluids $current-input-port in
	      $current-output-port out
	      $current-error-port error
	      $current-noise-port error
    thunk))

(define (call-with-current-input-port port thunk)
  (let-fluid $current-input-port port thunk))

(define (call-with-current-output-port port thunk)
  (let-fluid $current-output-port port thunk))

(define (call-with-current-noise-port port thunk)
  (let-fluid $current-noise-port port thunk))

(define (silently thunk)
  (call-with-current-noise-port (make-null-output-port) thunk))

;----------------
; Procedures with default port arguments.

; We probably lose a lot of speed here as compared with the
; specialized VM instructions.

(define (read-char . port-option)
  (real-read-char (input-port-option port-option)))

(define (peek-char . port-option)
  (real-peek-char (input-port-option port-option)))

(define (write-char c . port-option)
  (real-write-char c (output-port-option port-option)))

(define (newline . port-option)
  (write-char #\newline (output-port-option port-option)))

(define (byte-ready? . port-option)
  (real-byte-ready? (input-port-option port-option)))

; CHAR-READY? sucks
(define (char-ready? . port-option)
  (real-char-ready? (input-port-option port-option)))

(define (output-port-option port-option)
  (cond ((null? port-option) (current-output-port))
	((null? (cdr port-option)) (car port-option))
	(else (error "write-mumble: too many arguments" port-option))))

(define (input-port-option port-option)
  (cond ((null? port-option) (current-input-port))
	((null? (cdr port-option)) (car port-option))
	(else (error "read-mumble: too many arguments" port-option))))

