; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; ((method () 1)) => 1
; ((method () 1) 2) => error

; ((method (x) 1) 2) => 1
; ((method ((x <symbol>)) 1) 2) => error

; ((method ((x <symbol>)) 1) 'foo) => 1
; ((method ((x <symbol>)) (next-method)) 'foo) => error



(define-generic-function elt (s))

(define-method elt ((x <vector>) y)
  (vector-ref x y))

; (elt '#(a b c) 1) => b

(define-method elt ((x <string>) y)
  (string-ref x y))

(define-method elt ((x <list>) y)
  (list-ref x y))


; Setters

(define-generic-function (setter elt)
  (thing index new-value))

(define-method (setter elt) ((x <vector>) i z)
  (vector-set! x i z))

; (let ((v (vector 1 2 3))) (set (elt v 1) 'foo) v) => '#(1 foo 3)


; Classes

(define-class <mumble> () a b)

; (a (make <mumble>)) => '*uninitialized*
; (let ((m (make <mumble>))) (set (a m) 'foo) (a m)) => 'foo

(define-method initialize ((m <mumble>) z)
  (set (a m) z))

; (a (make <mumble> 3)) => 3


