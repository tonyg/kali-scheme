; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Templates implemented as vectors.

(define *template-marker* (list '*template-marker*))

(define (make-template len init)
  (let ((t (make-vector (+ len 1) init)))
    (vector-set! t 0 *template-marker*)
    t))

(define (template? obj)
  (and (vector? obj)
       (> (vector-length obj) 0)
       (eq? (vector-ref obj 0) *template-marker*)))

(define (template-length t) (- (vector-length t) 1))
(define (template-ref t i) (vector-ref t (+ i 1)))
(define (template-set! t i x) (vector-set! t (+ i 1) x))
