; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.



(xodd? 10)
(xodd? 5)

(define-local-syntax (define-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args))))

(define-primitive + 2)
(define-primitive - 2)
(define-primitive * 2)
(define-primitive < 2)
(define-primitive = 2)

(define (xodd? x)
  (cond ((= 0 x)
	 #f)
	((< 100 x)                  ; efficiency hack
	 (goto odd? (- x 100)))
	((< 1000 x)                  ; efficiency hack
	 (goto xodd? (- x 100)))
	(else
	 (goto xeven? (- x 1)))))

(define (xeven? x)
  (cond ((= 0 x)
	 #t)
	((< 100 x)                  ; efficiency hack
	 (goto xeven? (- x 100)))
	(else
	 (goto xodd? (- x 1)))))


(define (odd? x)
  (cond ((= 0 x)
	 #f)
	((< 100 x)                  ; efficiency hack
	 (goto odd? (- x 100)))
	(else
	 (goto even? (- x 1)))))

(define (even? x)
  (cond ((= 0 x)
	 #t)
	((< 100 x)                  ; efficiency hack
	 (goto even? (- x 100)))
	(else
	 (goto odd? (- x 1)))))


