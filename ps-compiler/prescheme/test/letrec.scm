; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


(define (t1)
  (let loop ((x 1))
    (if (g x)
	(loop (h x))
	x)))

(define (t2)
  (let loop ((x 1))
    (if (g x) x y)))

(define (t3)
  (letrec ((loop (lambda (x)
		   (if #t x (loop x)))))
    (loop 4)))

(define (t4)
  (letrec ((loop (lambda (x)
		   (if #t x (loop x))))
	   (loop2 (lambda (x)
		    (if x x (loop2 x)))))
    (loop 4)))

(define (t5)
  (letrec ((loop (lambda (x)
		   (if #t x (loop x))))
	   (loop2 (lambda (x)
		    (if x x (loop2 x)))))
    (g (loop 4) (loop2 5))))

(define (t6 y)
  (letrec ((loop (if y
		     (lambda (x) 5)
		     (lambda (x) 6))))
    (loop 4)))

(define (t7 y)
  (letrec ((loop (if y
		     (lambda (x) 5)
		     (lambda (x) (loop 6)))))
    (loop 4)))

