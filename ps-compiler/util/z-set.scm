; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


; Sets of integers implemented as integers.

(define (make-empty-integer-set)
  0)

(define (add-to-integer-set set integer)
  (bitwise-ior set (arithmetic-shift 1 integer)))

(define integer-set-chunk-size 24)
(define word-mask (- (arithmetic-shift 1 integer-set-chunk-size) 1))

; The nested loop reduces the amount of bignum arithmetic needed (and reduces
; the time by as much as a factor of 10).

(define (map-over-integer-set proc set)
  (do ((set set (arithmetic-shift set (- integer-set-chunk-size)))
       (i 0 (+ i integer-set-chunk-size))
       (l '() (do ((set (bitwise-and set word-mask) (arithmetic-shift set -1))
		   (j 0 (+ j 1))
		   (l l (if (odd? set)
			    (cons (proc (+ i j)) l)
			    l)))
		  ((or (= 0 set) (>= j integer-set-chunk-size))
		   l))))
      ((= 0 set)
       (reverse l))))

(define integer-set-and bitwise-and)
(define integer-set-ior bitwise-ior)
(define integer-set-not bitwise-not)

(define (integer-set-subtract set1 set2)
  (bitwise-and set1 (bitwise-not set2)))

(define integer-set-equal? =)
