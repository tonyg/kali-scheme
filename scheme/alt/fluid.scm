; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Fluid variables

(define (make-fluid val)
  (vector '<fluid> val))

(define (fluid f) (vector-ref f 1))

(define (set-fluid! f val)
  (vector-set! f 1 val))

(define (let-fluid f val thunk)
  (let ((swap (lambda () (let ((temp (fluid f)))
			   (set-fluid! f val)
			   (set! val temp)))))
    (dynamic-wind swap thunk swap)))

(define (let-fluids . args)		;Kind of gross
  (let loop ((args args)
	     (swap (lambda () #f)))
    (if (null? (cdr args))
	(dynamic-wind swap (car args) swap)
	(loop (cddr args)
	      (let ((f (car args))
		    (val (cadr args)))
		(lambda ()
		  (swap)
		  (let ((temp (fluid f)))
		    (set-fluid! f val)
		    (set! val temp))))))))
		
