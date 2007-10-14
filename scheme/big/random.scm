; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Random number generator, extracted from T sources.  Original
; probably by Richard Kelsey.

; Tests have shown that this is not particularly random.

(define half-log 14)
(define full-log (* half-log 2))
(define half-mask (- (arithmetic-shift 1 half-log) 1))
(define full-mask (- (arithmetic-shift 1 full-log) 1))
(define index-log 6)
(define random-1 (bitwise-and 314159265 full-mask))
(define random-2 (bitwise-and 271828189 full-mask))

; (MAKE-RANDOM <seed>) takes an integer seed and returns a procedure of no
; arguments that returns a new pseudo-random number each time it is called.
; <Seed> should be between 0 and 2**28 - 1 (exclusive).

(define (make-random seed)
  (if (and (integer? seed)
	   (< 0 seed)
	   (<= seed full-mask))
      (make-random-vector seed
        (lambda (vec a b)
	  (lambda ()
	    (set! a (randomize a random-1 random-2))
	    (set! b (randomize b random-2 random-1))
	    (let* ((index (arithmetic-shift a (- index-log full-log)))
		   (c (vector-ref vec index)))
	      (vector-set! vec index b)
	      c))))
      (assertion-violation 'make-random "invalid argument" seed)))

(define (randomize x mult ad)
  (bitwise-and (+ (low-bits-of-product x mult) ad)
               full-mask))

(define (make-random-vector seed return)
  (let* ((size (arithmetic-shift 1 index-log))
         (vec (make-vector size 0)))
    (do ((i 0 (+ i 1))
         (b seed (randomize b random-2 random-1)))
        ((>= i size)
         (return vec seed b))
      (vector-set! vec i b))))

; Compute low bits of product of two fixnums using only fixnum arithmetic.
; [x1 x2] * [y1 y2] = [x1y1 (x1y2+x2y1) x2y2]

(define (low-bits-of-product x y)
  (let ((x1 (arithmetic-shift x (- 0 half-log)))
        (y1 (arithmetic-shift y (- 0 half-log)))
        (x2 (bitwise-and x half-mask))
        (y2 (bitwise-and y half-mask)))
     (bitwise-and (+ (* x2 y2)
                     (arithmetic-shift (bitwise-and (+ (* x1 y2) (* x2 y1))
                                                    half-mask)
                                       half-log))
                  full-mask)))
