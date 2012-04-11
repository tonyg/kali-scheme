; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Code-vectors implemented as vectors.

(define *code-vector-marker* (list '*code-vector-marker*))

(define (make-code-vector len init)
  (let ((t (make-vector (+ len 1) init)))
    (vector-set! t 0 *code-vector-marker*)
    t))

(define (code-vector? obj)
  (and (vector? obj)
       (> (vector-length obj) 0)
       (eq? (vector-ref obj 0) *code-vector-marker*)))

(define (code-vector-length t) (- (vector-length t) 1))
(define (code-vector-ref t i) (vector-ref t (+ i 1)))
(define (code-vector-set! t i x) (vector-set! t (+ i 1) x))
