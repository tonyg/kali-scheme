; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.



(define-data-type list
  (pair? (cons car cdr)
	 (car integer car set-car!)
	 (cdr list    cdr set-cdr!))
  (null? null))

(define (member? list x)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
	  ((= x (car list))
	   #t)
	  (else
	   (loop (cdr list))))))

(define (reverse! list)
  (if (or (null? list)
	  (null? (cdr list)))
      list
      (let loop ((list list) (prev null))
	(let ((next (cdr list)))
	  (set-cdr! list prev)
	  (if (null? next)
	      list
	      (loop next list))))))
