; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


(define (fact n)
  (let loop ((i n) (r 1))
    (if (<= i 1)
	r
	(loop (- i 1) (* i r)))))

(define (default y) y)

(define do-it (make-vector 5))

(do ((i 0 (+ i 1)))
    ((>= i 5))
  (vector-set! do-it i default))

(define (add-entry i k)
  (let ((f (fact k)))
    (vector-set! do-it i (lambda (x) (+ x f)))))

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

(define (digit? ch)
  (let ((ch (char->ascii ch)))
    (and (>= ch (char->ascii #\0))
	 (<= ch (char->ascii #\9)))))

(define (read-number port)
  (let loop ((r 0))
    (ps-read-char port
      (lambda (ch)
	(cond ((digit? ch)
	       (loop (+ (- (char->ascii ch) (char->ascii #\0))
			(* r 10))))
	      (else r)))
      (lambda () 0))))

(define (write-number x port)
  (really-write-number (cond ((< x 0)
			      (write-char #\- port)
			      (- 0 x))
			     (else
			      x))
		       port)
  (newline port))

(define (really-write-number x port)
    (if (>= x 10)
	(really-write-number (quotient x 10) port))
    (write-char (ascii->char (+ (remainder x 10)
				(char->ascii #\0)))
		port))
