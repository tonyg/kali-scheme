; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


(define-local-syntax (define-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args))))

(define-primitive + 2)
(define-primitive - 2)
(define-primitive * 2)
(define-primitive < 2)

(define-primitive make-vector 2)
(define-primitive pointer-add 2)

(define (vector-ref vec index)
  (call-primitively contents (pointer-add vec index)))

(define (vector-set! vec index value)
  (call-primitively set-contents! (pointer-add vec index) value))

(define (cons x y)
  (let ((p (make-vector 2 0)))
    (vector-set! p 0 x)
    (vector-set! p 1 y)
    p))

(define (car p)
  (vector-ref p 0))

(define (cdr p)
  (vector-ref p 1))

