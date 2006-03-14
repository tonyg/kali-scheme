; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (number-of-required-args formals)
  (do ((l formals (cdr l))
       (i 0 (+ i 1)))
      ((not (pair? l)) i)))
    
(define (n-ary? formals)
  (cond ((null? formals) #f)
	((pair? formals) (n-ary? (cdr formals)))
	(else #t)))
    
(define (normalize-formals formals)
  (cond ((null? formals)
	 '())
	((pair? formals)
	 (cons (car formals)
	       (normalize-formals (cdr formals))))
	(else
	 (list formals))))

