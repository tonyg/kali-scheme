; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file features.scm.
; Synchronize any changes with all the other *-features.scm files.

; These definitions should be quite portable to any Scheme implementation.
; Assumes Revised^5 Report Scheme, for EVAL and friends.


; SIGNALS

(define (error message . irritants)
  (display-error-message "Error: " message irritants)
  (an-error-occurred-now-what?))

(define (warn message . irritants)
  (display-error-message "Warning: " message irritants))

(define (display-error-message heading message irritants)
  (display heading)
  (display message)
  (newline)
  (let ((spaces (list->string
		 (map (lambda (c) #\space) (string->list heading)))))
    (for-each (lambda (irritant)
		(display spaces)
		(write irritant)
		(newline))
	      irritants)))

; Linker also needs SIGNAL, SYNTAX-ERROR, CALL-ERROR

; FEATURES

(define (force-output port) #f)

(define current-noise-port current-output-port)

(define (string-hash s)
  (let ((n (string-length s)))
    (do ((i 0 (+ i 1))
         (h 0 (+ h (char->ascii (string-ref s i)))))
        ((>= i n) h))))

(define (make-immutable! thing) thing)
(define (immutable? thing) #f)
(define (unspecific) (if #f #f))

; BITWISE -- use alt/bitwise.scm (!)
; ASCII -- use alt/ascii.scm
; CODE-VECTORS -- use alt/code-vectors.scm
