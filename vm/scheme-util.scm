; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file scheme-util.scm

; Scheme versions of those things that differ in Scheme and PreScheme.

; (error ...)   ; just us the available Scheme definition

(define unassigned
  (let ((marker (list '<unassigned>)))
    (lambda () marker)))

(define (assert test)
  (if (not test) (error "assertion failed")))

(define (make-byte-vector-pointer vector offset)
  offset)

(define byte-vector-pointer-ref  byte-vector-ref)
(define byte-vector-pointer-set! byte-vector-set!)

(define byte-vector-pointer-word-ref  byte-vector-word-ref)
(define byte-vector-pointer-word-set! byte-vector-word-set!)

(define null-pointer? not)  ; used for checking for correct return values

(define (write-number num port)
  (write num port)
  (newline port))

(define (read-number port)
  (read port))

; Tail recursive calls to global variables

(define (goto proc . args)
  (apply proc args))

(define (deallocate-string string)
  #f)

(define (vector+length-fill! v length x)
  (if (not (= length (vector-length v)))
      (error "wrong length in vector+length-fill!" v length x))
  (vector-fill! v x))
