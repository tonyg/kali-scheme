; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Redefinitions of some usual Scheme things so as to make the new
; exception system kick in when it needs to.

(define (number? n)
  (or ((structure-ref true-scheme number?) n)
      (extended-number? n)))

;(define (integer? n)
;  (if ((structure-ref true-scheme number?) n)
;      ((structure-ref true-scheme integer?) n)
;      (and (extended-number? n)
;           ... raise exception ...)))

(define (+ x y) ((structure-ref true-scheme +) x y))
(define (* x y) ((structure-ref true-scheme *) x y))
(define (- x y) ((structure-ref true-scheme -) x y))
(define (/ x y) ((structure-ref true-scheme /) x y))
(define (= x y) ((structure-ref true-scheme =) x y))
(define (< x y) ((structure-ref true-scheme <) x y))
(define (make-vector x y) ((structure-ref true-scheme make-vector) x y))
(define (make-string x y) ((structure-ref true-scheme make-string) x y))
(define (apply x y) ((structure-ref true-scheme apply) x y))

(define (read-char x) ((structure-ref true-scheme read-char) x))
(define (peek-char x) ((structure-ref true-scheme peek-char) x))
(define (char-ready? x) ((structure-ref true-scheme char-ready?) x))
(define (write-char x y) ((structure-ref true-scheme write-char) x y))
