; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Lost: (ARITHMETIC-SHIFT 5 27) => -402653184 [wanted 671088640.]
; Lost: (ARITHMETIC-SHIFT 5 28) => 268435456 [wanted 1342177280.]


(define (testit name proc x y z)
  (let ((result (proc x y)))
    (if (not (= result z))
	(begin (display "Lost: ")
	       (write `(,name ,x ,y))
	       (display " => ")
	       (write result)
	       (display " [wanted ")
	       (write z)
	       (display "]")
	       (newline)))))

(define most-positive-fixnum
  (let ((n (arithmetic-shift 2 27))) (+ n (- n 1))))

(define (test-left-shifts x)
  (let ((crossover (arithmetic-shift 2 27)))
    (do ((y 0 (+ y 1))
	 (z x (* z (if (>= z crossover) 2. 2))))
	((= y 34))
      (testit 'arithmetic-shift arithmetic-shift x y z))))

(test-left-shifts 5)
(test-left-shifts -5)

(define (test-right-shifts x)
  (do ((y 0 (- y 1))
       (z x (quotient z 2)))
      ((= y -34))
    (testit 'arithmetic-shift arithmetic-shift x y z)))

(test-right-shifts (* 5 (expt 2 36)))
(test-right-shifts (* -5 (expt 2 36)))

(define (bit1? x)
  (if (< x 0)
      (even? (quotient (- -1 x) 2))
      (odd? (quotient x 2))))

(define (try-truth-table name proc predicate)
  (do ((x -4 (+ x 1)))
      ((= x 4))
    (do ((y -4 (+ y 1)))
	((= y 4))
      (testit name proc x y
	      (+ (if (predicate (odd? x) (odd? y)) 1 0)
		 (if (predicate (bit1? x) (bit1? y)) 2 0)
		 (if (predicate (negative? x) (negative? y)) -4 0))))))

(try-truth-table 'bitwise-and bitwise-and (lambda (a b) (and a b)))
(try-truth-table 'bitwise-ior bitwise-ior (lambda (a b) (or a b)))
