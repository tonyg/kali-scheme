; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


(define-local-syntax (define-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args))))

(define-primitive ashl 2)
(define-primitive ashr 2)
(define-primitive = 2)
(define-primitive bitwise-and 2)

(define (input-type pred coercer)  ;Alonzo wins
  (lambda (f) (f pred coercer)))

(define (input-type-predicate type) (type (lambda (x y) y x)))
(define (input-type-coercion type)  (type (lambda (x y) x y)))

(define (no-coercion x) x)

(define (odd? x)
  (= 1 (bitwise-and x 1)))

(define (extract-odd x)
  (ashr x 1))

(define any->      (input-type (lambda (x) x #t) no-coercion))
(define odd->      (input-type odd?  extract-odd))

(define (test x y)
  (if (and ((input-type-predicate any->) x)
	   ((input-type-predicate odd->) y))
      (let ((a ((input-type-coercion any->) x))
	    (b ((input-type-coercion odd->) y)))
	(+ a b))
      x))

