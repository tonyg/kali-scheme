; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Continuations implemented as vectors.

(define *continuation-marker* (list '*continuation-marker*))

(define (make-continuation len init)
  (let ((c (make-vector (+ len 1) init)))
    (vector-set! c 0 *continuation-marker*)
    c))

(define (continuation? obj)
  (and (vector? obj)
       (> (vector-length obj) 0)
       (eq? (vector-ref obj 0) *continuation-marker*)))

(define (continuation-length c) (- (vector-length c) 1))
(define (continuation-ref c i) (vector-ref c (+ i 1)))
(define (continuation-set! c i x) (vector-set! c (+ i 1) x))
