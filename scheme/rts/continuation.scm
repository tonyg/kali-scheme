; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Continuations

(define (make-ref index)
  (lambda (c)
    (continuation-ref c index)))

(define continuation-cont          (make-ref continuation-cont-index))
(define real-continuation-pc       (make-ref continuation-pc-index))
(define real-continuation-template (make-ref continuation-template-index))
(define continuation-env           (make-ref continuation-env-index))
(define exception-cont-pc          (make-ref exception-cont-pc-index))
(define exception-cont-template    (make-ref exception-cont-template-index))

; This one is exported
(define exception-continuation-exception
  (make-ref exception-cont-exception-index))

; Exception continuations contain the state of the VM when an exception occured.

(define (exception-continuation? thing)
  (and (continuation? thing)
       (= 3 (real-continuation-pc thing))
       (let ((code (template-code (real-continuation-template thing))))
	 (and (= 1				; one return value
		 (code-vector-ref code 4))
	      (= (enum op return-from-exception)
		 (code-vector-ref code 5))))))

(define (continuation-pc c)
  (if (exception-continuation? c)
      (exception-cont-pc c)
      (real-continuation-pc c)))

(define (continuation-template c)
  (if (exception-continuation? c)
      (exception-cont-template c)
      (real-continuation-template c)))

; Accessing the saved operand stack.

(define (continuation-arg c i)
  (continuation-ref c (+ (if (exception-continuation? c)
			     exception-continuation-cells
			     continuation-cells)
			 i)))

(define (continuation-arg-count c)
  (- (continuation-length c)
     (if (exception-continuation? c)
	 exception-continuation-cells
	 continuation-cells)))

(define-simple-type :continuation (:value) continuation?)

(define-method &disclose ((obj :continuation))
  (if (exception-continuation? obj)
      (list 'exception-continuation
	    `(pc ,(exception-cont-pc obj))
	    (template-info (exception-cont-template obj)))
      (list 'continuation
	    `(pc ,(continuation-pc obj))
	    (template-info (continuation-template obj)))))

; If (continuation-cont A) = B, then ignore B if the following are all true:
;   1. (continuation-template B) = (continuation-template A)
;   2. (continuation-pc B) > (continuation-pc A)
;   3. (continuation-env B) = (continuation-env A)
;                             or some parent of (continuation-env A)
;
; I don't think this is foolproof, but I have so far been unable to
; contrive a situation in which it fails.  I think a double recursion of a 
; procedure of no arguments is required, at the very least.

(define (continuation-parent a)
  (let ((b (continuation-cont a)))
    (if (and (continuation? b)
	     (eq? (continuation-template a) (continuation-template b))
	     (< (continuation-pc a) (continuation-pc b))
	     (let loop ((env (continuation-env a)))
	       (or (eq? env (continuation-env b))
		   (and (vector? env)
			(loop (vector-ref env 0))))))
	(continuation-parent b)
	b)))
