; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file prescheme-util.scm

; PreScheme versions of those things that differ in Scheme and PreScheme.

; Nothing local about it...

(define-syntax define-vm-syntax
  (lambda (form rename compare)
    (let ((pattern (cadr form))
	  (body (cddr form)))
      `(define-syntax ,(car pattern)
	 (lambda (form rename compare)
	   (destructure ((,(cdr pattern) (cdr form)))
	     . ,body))))))

(define (unassigned) (call-primitively uninitialized-value))

; ASSERT can be redefined for debugging purposes.

(define-vm-syntax (assert . stuff)
  ''0)

(define (pointer->integer x)
  (call-primitively coerce x '(pointer int8u) 'int32))

(define (integer->pointer x)
  (call-primitively coerce x 'int32 '(pointer int8u)))

(define (make-byte-vector-pointer vector offset)
  (+ (pointer->integer vector) offset))

(define (byte-vector-pointer-ref vec pointer)
  (byte-vector-ref (integer->pointer pointer) 0))

(define (byte-vector-pointer-set! vec pointer value)
  (byte-vector-set! (integer->pointer pointer) 0 value))

(define (byte-vector-pointer-word-ref vec pointer)
  (byte-vector-word-ref (integer->pointer pointer) 0))

(define (byte-vector-pointer-word-set! vec pointer value)
  (byte-vector-word-set! (integer->pointer pointer) 0 value))

(define (null-pointer? p)
  (call-primitively null-pointer? p))

;;; Reading and writing integers

; read an integer from PORT

(define (read-number port)
  (let loop ()
    (let ((ch (read-char port)))
      (if (eof-object? ch)
	  0
	  (let ((cha (char->ascii ch)))
	    (cond ((= cha (char->ascii #\-))
		   (- 0 (really-read-number port (read-char port))))
		  ((= cha (char->ascii #\+))
		   (really-read-number port (read-char port)))
		  ((and (>= cha (char->ascii #\0))
			(<= cha (char->ascii #\9)))
		   (really-read-number port ch))
		  (else
		   (loop))))))))

(define (really-read-number port ch)
  (let loop ((r 0) (ch ch))
    (if (eof-object? ch)
	r
	(let ((ch (- (char->ascii ch) (char->ascii #\0))))
	  (if (and (>= ch 0) (<= ch 9))
	      (loop (+ ch (* r 10)) (read-char port))
	      r)))))

; Return 10**n such that 10**n <= x < 10**(n+1)

(define (integer-mask x)
  (do ((x x (quotient x 10))
       (mask 1 (* mask 10)))
      ((< x 10) mask)))

; Write positive integer X out to PORT

(define (write-number x port)
  (write-number-no-newline x port)
  (write-char '#\newline port))

(define (write-number-no-newline x port)
  (let ((x (cond ((< x 0)
                  (write-char '#\- port)
                  (- 0 x))
                 (else
                  x))))
    (let loop ((x x) (mask (integer-mask x)))
      (let ((digit (quotient x mask)))
        (write-char (ascii->char (+ digit (char->ascii '#\0))) port)
        (if (> mask 1)
	    (loop (remainder x mask) (quotient mask 10)))))))

(define (read-block port bytev start count)
  (let ((got (call-primitively read-block port (integer->pointer start) count)))
    (if (not (= got count))
	(error "block read did not get enough bytes"))))

; The Unix procedure `write-block' can return smaller than expected
; amount if an interrupt occurs during the write.  This should return
; a success/failure value so that an interrupt doesn't kill the system.

(define (write-block port bytev start count)
  (let ((sent (call-primitively write-block
				port
				(integer->pointer start)
				count)))
    (if (not (= sent count))
	(error "block write did not send enough bytes"))))

; GOTO is part of PreScheme

(define (deallocate-string string)
  (deallocate (pointer-add string -4)))

(define (vector+length-fill! v length x)
  (do ((i 0 (+ i 1)))
      ((>= i length))
    (vector-set! v i x)))

