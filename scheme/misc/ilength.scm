; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Integer-length, a la Common Lisp, written in portable Scheme.

(define-syntax cons-stream
  (syntax-rules ()
    ((cons-stream head tail)
     (cons head (delay tail)))))
(define head car)
(define (tail s) (force (cdr s)))

(define integer-length
  (let ()
    (define useful
      (let loop ((p 256) (n 4))
	(cons-stream (cons p n)
		     (loop (* p p) (* n 2)))))
    (define (recur n)
      (if (< n 16)
	  (vector-ref '#(0 1 2 2 3 3 3 3 4 4 4 4 4 4 4 4) n)
	  (let loop ((s useful) (prev 16))
	    (let ((z (head s)))
	      (if (< n (car z))
		  (+ (cdr z) (recur (quotient n prev)))
		  (loop (tail s) (car z)))))))
    (define (integer-length n)
      (if (exact? n)
	  (if (< n 0)
	      (recur (- -1 n))
	      (recur n))
	  (integer-length (inexact->exact n))))
    integer-length))
