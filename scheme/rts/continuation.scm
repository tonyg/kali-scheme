; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Continuations

(define (continuation-cont     c) (continuation-ref c 0))
(define (continuation-pc       c) (continuation-ref c 1))
(define (continuation-template c) (continuation-ref c 2))
(define (continuation-env      c) (continuation-ref c 3))
(define continuation-overhead 4)
(define (continuation-arg c i)
  (continuation-ref c (+ continuation-overhead i)))
(define (continuation-arg-count c)
  (- (continuation-length c) continuation-overhead))


; If (continuation-cont A) = B, then ignore B if
;   1. (continuation-template B) = (continuation-template A)
;   2. (continuation-pc B) > (continuation-pc A)
;   3. (continuation-env B) = (continuation-env A)
;                             or some parent of (continuation-env A)
; I don't think this is foolproof, but I have so far been unable to
; contrive a situation in which it fails.  I think a double recursion of a 
; procedure of no arguments is required, at the very least.

(define (continuation-parent a)
  (let ((b (continuation-cont a)))
    (if (and (continuation? b)
	     (eq? (continuation-template b) (continuation-template a))
	     (> (continuation-pc b) (continuation-pc a))
	     (let loop ((env (continuation-env A)))
	       (or (eq? env (continuation-env B))
		   (and (vector? env)
			(loop (vector-ref env 0))))))
	(continuation-parent b)
	b)))

(define-simple-type :continuation (:value) continuation?)

(define-method &disclose ((obj :continuation))
  (list 'continuation
	`(pc ,(continuation-pc obj))
	(template-info (continuation-template obj))))
