; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file enum.scm.

;;;; Enumerated types

;   (enumerand->name <integer> <e-type>) => a symbol
;   (name->enumerand <symbol>  <e-type>) => an integer

(define (enumerand->name e e-type)
  (vector-ref e-type e))

(define (name->enumerand e e-type)
  (let loop ((i 0))			;vector-posq
    (if (< i (vector-length e-type))
	(if (eq? e (vector-ref e-type i))
	    i
	    (loop (+ i 1)))
	#f)))

(define (definitely-name->enumerand e e-type e-type-name)
  (or (name->enumerand e e-type)
      (error "unknown enumerand name" e-type-name e)))
