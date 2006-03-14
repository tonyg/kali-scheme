; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Somewhat redundant with vm/istruct.scm.  Fix this some day.

; Templates
;  Templates are made only by the compiler.

;(define make-template make-vector)

;(define (template? obj)
;  (and (vector? obj)
;       (>= (vector-length obj) template-overhead)
;       (code-vector? (template-code obj))
;       ))

;(define template-length vector-length)
;(define template-ref vector-ref)
;(define template-set! vector-set!)

(define template-overhead 4)

(define (template-code       tem) (template-ref tem 0))
(define (template-byte-code  tem) (template-ref tem 1))
(define (template-info       tem) (template-ref tem 2))
(define (template-package-id tem) (template-ref tem 3))
(define (set-template-code!       tem cv)   (template-set! tem 0 cv))
(define (set-template-byte-code!  tem cv)   (template-set! tem 1 cv))
(define (set-template-info!       tem info) (template-set! tem 2 info))
(define (set-template-package-id! tem id)   (template-set! tem 3 id))

