; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


; Oops - this is polymorphic!

(define (vector+length-fill! v length x)
  (do ((i 0 (+ i 1)))
      ((>= i length))
    (vector-set! v i x)))

(define *v* (unassigned))

(define (test x)
  (set! *v* (make-vector 10))
  (vector+length-fill! *v* 10 3)
  (vector-ref *v* x))

;(define (find-port-index)
;  (let loop ((i 0))
;    (cond ((>= i 10)
;           -1)
;          ((= 3 (vector-ref *v* i))
;           i)
;          (else (loop (+ i 1))))))
;
;(define (foo)
;  (let loop ((i (find-port-index)))
;    (if (>= i 5)
;        (let ((v *v*))
;          (bar)
;          (vector-set! v i (baz)))
;        (loop (find-port-index)))))
