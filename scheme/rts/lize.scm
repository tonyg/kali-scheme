; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Simplest rational within an interval.  Copied from IEEE P1178/D4 nimpl.tex.

(define (rationalize x e)
  (let ((e (abs e)))
    (simplest-rational (- x e) (+ x e))))

(define (simplest-rational x y)
  (define (simplest-rational-internal x y)
    ;; assumes 0 < X < Y
    (let ((fx (floor x))
          (fy (floor y)))
      (cond ((not (< fx x))
             fx)
            ((= fx fy)
             (+ fx
		(/ 1 (simplest-rational-internal
		      (/ 1 (- y fy))
		      (/ 1 (- x fx))))))
            (else
             (+ 1 fx)))))
  ;; Do some juggling to satisfy preconditions of simplest-rational-internal.
  (cond ((not (< x y))
         (if (rational? x) x (error "(rationalize <irrational> 0)" x)))
        ((positive? x)
         (simplest-rational-internal x y))
        ((negative? y)
         (- 0 (simplest-rational-internal (- 0 y) (- 0 x))))
        (else
         (if (and (exact? x) (exact? y))
             0
             (exact->inexact 0)))))

