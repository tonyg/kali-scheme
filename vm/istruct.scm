; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file istruct.scm.
; Redundant with rts/rtsistruct.scm.  Fix this some day.

;;;; Interpreter data structures

(define byte-limit (ashl 1 bits-used-per-byte))

; Continuations
;  Continuations are made only by the interpreter.
;  CONT must be first (for the stack)

(define (continuation-cont     c) (continuation-ref c 0))
(define (continuation-pc       c) (continuation-ref c 1))
(define (continuation-template c) (continuation-ref c 2))
(define (continuation-env      c) (continuation-ref c 3))

(define (set-continuation-cont!     c val) (continuation-set! c 0 val))
(define (set-continuation-pc!       c val) (continuation-set! c 1 val))
(define (set-continuation-template! c val) (continuation-set! c 2 val))
(define (set-continuation-env!      c val) (continuation-set! c 3 val))

(define continuation-cells 4)
