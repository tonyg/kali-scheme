; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


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
(define-primitive contents 1)
(define-primitive set-contents! 2)

(define *current-input-port*
  (call-primitively stdin))

(define *current-output-port*
  (call-primitively stdout))

(define (current-input-port)
  *current-input-port*)

(define (current-output-port)
  *current-output-port*)

(define (vector-ref vec index)
  (contents (pointer-add vec index)))

(define (vector-set! vec index value)
  (set-contents! (pointer-add vec index) value))

(define (default y) y)

(define do-it (make-vector 5 default))

(define (add-entry i k)
  (vector-set! do-it i (lambda (x) (+ x k))))

(add-entry 0 1)
;(add-entry 1 1)
(add-entry 2 3)
(add-entry 3 4)
(add-entry 4 4)
   
(define (test x)
  ((vector-ref do-it x) 5))

(define (main)
  (write-number (test (read-number (current-input-port)))
		(current-output-port)))


