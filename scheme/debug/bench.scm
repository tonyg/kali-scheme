; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Tiny benchmarking image.

; This returns the result of calling recursive FIB on its first argument.

(define (start arg in out error)
  (fib (string->integer (vector-ref arg 0))))

(define (string->integer s)
  (letrec ((loop (lambda (i r)
		   (if (= i (string-length s))
		       r
		       (loop (+ i 1)
			     (+ (- (char->ascii (string-ref s i))
				   (char->ascii #\0))
				(* 10 r)))))))
    (loop 0 0)))

(define (fib n)
  (if (< n 2)
      1
      (+ (fib (- n 1)) (fib (- n 2)))))
