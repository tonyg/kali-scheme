; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Writing out a Scheme 48 image

(define (write-image file start-proc id-string)
  (if (not (= 0 (remainder bits-per-cell bits-per-io-byte)))
      (error "io-bytes to not fit evenly into cells"))
  (initialize-memory)
  (call-with-output-file file
    (lambda (port)
      (let ((start (transport start-proc))  ; transport the start-proc
	    (false (transport #f)))
	(display id-string port)
	(newline port)
	(write-page port)
	(newline port)
	(display architecture-version port)
	(newline port)
	(boot-write-number bytes-per-cell        port)
	(boot-write-number 0                     port)   ; newspace begin
	(boot-write-number (a-units->cells *hp*) port)
	(boot-write-number false                 port)   ; symbol table
	(boot-write-number false                 port)   ; imported bindings
	(boot-write-number false                 port)   ; exported bindings
	(boot-write-number false                 port)   ; resumer records
	(boot-write-number start                 port)   ; start-proc
	(write-page port)
	(write-descriptor 1 port)	; endianness indicator
	(write-heap port))))		; write out the heap
  )

(define bits-per-io-byte 8)    ; for writing images

(define (write-page port)
  (write-char (ascii->char 12) port))

(define (write-byte byte port)
  (write-char (ascii->char byte) port))
  
(define io-byte-mask
  (low-bits -1 bits-per-io-byte))

;(define bits-per-cell    -- defined in data.scm
;  (* bits-per-byte bytes-per-cell))

(define (big-endian-write-descriptor thing port)
  (let loop ((i (- bits-per-cell bits-per-io-byte)))
    (cond ((>= i 0)
           (write-byte (bitwise-and io-byte-mask
				    (arithmetic-shift thing (- 0 i))) port)
           (loop (- i bits-per-io-byte))))))

(define (little-endian-write-descriptor thing port)
  (let loop ((i 0))
    (cond ((< i bits-per-cell)
           (write-byte (bitwise-and io-byte-mask
				    (arithmetic-shift thing (- 0 i))) port)
           (loop (+ i bits-per-io-byte))))))

(define write-descriptor little-endian-write-descriptor)

(define (boot-write-number n port)
  (display n port)
  (newline port))

