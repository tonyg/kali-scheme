; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.



(define (foo)
  (fact 10)
  (fact 20))

(foo)
(foo)
(fact 5)

(define *one* (unassigned))

(define-local-syntax (define-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args))))

(define-local-syntax (define-effect-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args)
       (call-primitively undefined-value))))

(define-primitive + 2)
(define-primitive - 2)
(define-primitive * 2)
(define-primitive < 2)

;(define-primitive quotient  2)
;(define-primitive remainder 2)

(define-primitive char->ascii 1)
(define-primitive ascii->char 1)

(define-effect-primitive write-char   2) 

(define (unassigned) (call-primitively undefined-value))

(define (byte-vector-ref vec index)
  (call-primitively byte-contents (ptr+ vec index)))

(define (byte-vector-set! vec index value)
  (call-primitively byte-set-contents! (ptr+ vec index) value))

(define (vector-set! vec index value)
  (call-primitively set-contents! (ptr+ vec (* index 4)) value))

;(write-number-no-newline 102 port)

;(define (write-number-no-newline x port)
;  (let ((x (cond ((< x 0)
;                  (write-char '#\- port)
;                  (- 0 x))
;                 (else
;                  x))))
;    (let loop ((x x) (mask foo))
;      (let ((digit (quotient x mask)))
;        (write-char (ascii->char (+ digit (char->ascii '#\0))) port)
;        (if (< mask 1)
;	    (loop (remainder x mask) (quotient mask 10)))))))

(define (fact n)
  (let loop ((i n) (r *one*))
    (if (<= *one* i)
	(loop (- i *one*) (* i r))
	r)))

;(define (poobah x)
;  (+ x (* x (+ x (* x *two*)))))

;(define *two* 2)

(define (<= x y)
  (not (< y x)))

(define (not x)
  (if x #f #t))

(define (identity x)
  x)

(define (two x)
  2)
