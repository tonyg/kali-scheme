; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Arithmetic that checks for overflow

(define (carefully op)
  (lambda (x y succ fail)
    (let ((z (op (extract-fixnum x)
		 (extract-fixnum y))))
      (if (or (too-big-for-fixnum? z)
	      (too-small-for-fixnum? z))
	  (goto fail x y)
	  (goto succ (enter-fixnum z))))))

(define add-carefully (carefully +))
(define subtract-carefully (carefully -))

(define half-word-size (quotient bits-per-cell 2))
(define half-word-mask (- (shift-left 1 half-word-size) 1))
(define max-middle (shift-left 1 (- (- bits-per-fixnum 1) half-word-size)))

; Uses SMALL* to do half-word multiplies.  Some implementations
; really care about this.

(define (multiply-carefully x y succ fail)
  (let* ((a (extract-fixnum x))
         (b (extract-fixnum y))
         (positive-result? (if (>= a 0)
                               (>= b 0)
                               (< b 0)))
         (a (abs a))
         (b (abs b))
	 (lo-a (bitwise-and half-word-mask a))
	 (lo-b (bitwise-and half-word-mask b))
         (hi-a (bitwise-and half-word-mask (high-bits a half-word-size)))
         (hi-b (bitwise-and half-word-mask (high-bits b half-word-size)))
	 (lo-c (small* lo-a lo-b))
	 (mid-c (+ (small* lo-a hi-b) (small* lo-b hi-a)))
	 (c (+ lo-c (shift-left mid-c half-word-size))))
    (cond ((or (and (> hi-a 0) (> hi-b 0))
	       (too-big-for-fixnum? lo-c)
               (> 0 lo-c)
	       (> mid-c max-middle))
	   (goto fail x y))
	  (positive-result?
	   (goto succ c))
	  (else
	   (goto succ (- 0 c))))))

(define small*
  (external "SMALL_MULTIPLY" (=> (integer integer) integer) *))

; Test cases for bits-per-cell = 28, bits-per-fixnum = 26

;   (do ((i 2 (* i 2))
;        (j (* -2 (expt 2 23)) (/ j 2)))
;       ((>= j 0) 'ok)
;     (write `((* ,i ,j) ?=? ,(* i j)))
;     (newline))

(define (divide-carefully x y succ fail)
  (if (= y (enter-fixnum 0))
      (goto fail x y)
      (let* ((a (extract-fixnum x))
	     (b (extract-fixnum y))
	     (positive-result? (if (>= a 0)
				   (>= b 0)
				   (< b 0)))
	     (a (abs a))
	     (b (abs b))
	     (c (quotient a b)))
	(cond ((not (= 0 (remainder a b)))
	       (goto fail x y))
	      ((not positive-result?)
	       (goto succ (- 0 c)))
	      (else
	       (goto succ c))))))

; Watch out for (quotient least-fixnum -1)
(define (quotient-carefully x y succ fail)
  (if (= y (enter-fixnum 0))
      (fail x y)
      (let* ((a (extract-fixnum x))
	     (b (extract-fixnum y))
	     (positive-result? (if (>= a 0)
				   (>= b 0)
				   (< b 0)))
	     (a (abs a))
	     (b (abs b))
	     (c (quotient a b)))
	(cond ((not positive-result?)
	       (goto succ (enter-fixnum (- 0 c))))
	      ((too-big-for-fixnum? c)  ; (quotient least-fixnum -1)
	       (goto fail x y))
	      (else
	       (goto succ (enter-fixnum c)))))))

; Overflow check not necessary
(define (remainder-carefully x y succ fail)
  (if (= y (enter-fixnum 0))
      (goto fail x y)
      (let* ((a (extract-fixnum x))
	     (b (extract-fixnum y))
	     (positive-result? (>= a 0))
	     (a (abs a))
	     (b (abs b))
	     (c (remainder a b)))
	(goto succ (enter-fixnum (if positive-result? c (- 0 c)))))))

(define (shift-carefully value+tag count+tag succ fail)
  (let ((value (extract-fixnum value+tag))
	(count (extract-fixnum count+tag)))
    (if (< count 0)
	(goto succ (arithmetic-shift-right value (- 0 count)))
	(let ((result (shift-left value count)))
	  (if (and (= value (arithmetic-shift-right result count))
		   (if (>= value 0)
		       (>= result 0)
		       (< result 0)))
	      (goto succ result)
	      (goto fail value+tag count+tag))))))

; beware of (abs least-fixnum)
(define (abs-carefully n succ fail)
  (let ((r (abs (extract-fixnum n))))
    (if (too-big-for-fixnum? r)
	(goto fail n)
	(goto succ (enter-fixnum r)))))

(define (fixnum-bit-count x)
  (let* ((x (extract-fixnum x))
	 (x (if (< x 0) 
		(bitwise-not x)
		x)))
    (do ((x x (arithmetic-shift-right x 1))
	 (count 0 (+ count (bitwise-and x 1))))
	((= x 0)
	 (enter-fixnum count)))))
    