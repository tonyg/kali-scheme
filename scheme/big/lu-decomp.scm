; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; LU Decomposition (a rewriting of a Pascal program from `Numerical Recipes
; in Pascal'; look there for a detailed description of what is going on).

; A is an NxN matrix that is updated in place.
; This returns a row permutation vector and the sign of that vector.

(define *lu-decomposition-epsilon* 1.0e-20)

(define (lu-decomposition a)
  (let* ((n (car (array-shape a)))
	 (indx (make-vector n))
	 (sign 1.0)
	 (vv (make-vector n)))

    (do ((i 0 (+ i 1)))
	((>= i n))
      (do ((j 0 (+ j 1))
	   (big 0.0 (max big (abs (array-ref a i j)))))
	  ((>= j n)
	   (if (= big 0.0)
	       (error "lu-decomposition matrix has a zero row" a i))
	   (vector-set! vv i (/ big)))))

    (do ((j 0 (+ j 1)))
	((>= j n))
      (let ()
	(define (sum-elts i end)	  
	  (do ((k 0 (+ k 1))
	       (sum (array-ref a i j)
		    (- sum (* (array-ref a i k)
			      (array-ref a k j)))))
	      ((>= k end)
	       sum)))

	(do ((i 0 (+ i 1)))
	    ((>= i j))
	  (array-set! a (sum-elts i i) i j))

	(receive (big imax)
	    (let loop ((i j) (big 0.0) (imax 0))
	      (if (>= i n)
		  (values big imax)
		  (let ((sum (sum-elts i j)))
		    (array-set! a sum i j)
		    (let ((temp (* (vector-ref vv i) (abs sum))))
		      (if (>= temp big)
			  (loop (+ i 1) temp i)
			  (loop (+ i 1) big imax))))))
	
	  (if (not (= j imax))
	      (begin
		(do ((k 0 (+ k 1)))
		    ((>= k n))
		  (let ((temp (array-ref a imax k)))
		    (array-set! a (array-ref a j k) imax k)
		    (array-set! a temp j k)))
		(set! sign (- sign))
		(vector-set! vv imax (vector-ref vv j))))
	  
	  (vector-set! indx j imax)
	  
	  (if (= (array-ref a j j) 0.0)
	      (array-set! a *lu-decomposition-epsilon* j j))
	  
	  (if (not (= j (- n 1)))
	      (let ((temp (/ (array-ref a j j))))
		(do ((i (+ j 1) (+ i 1)))
		    ((>= i n))
		  (array-set! a (* (array-ref a i j) temp) i j)))))))
      
      (values indx sign)))

(define (lu-back-substitute a indx b)
  (let ((n (car (array-shape a))))
	  
    (let loop ((i 0) (ii #f))
      (if (< i n)
	  (let* ((ip (vector-ref indx i))
		 (temp (vector-ref b ip)))
	    (vector-set! b ip (vector-ref b i))
	    (let ((new (if ii
			   (do ((j ii (+ j 1))
				(sum temp (- sum (* (array-ref a i j)
						    (vector-ref b j)))))
			       ((>= j i)
				sum))
			   temp)))
	      (vector-set! b i new)
	      (loop (+ i 1)
		    (if (or ii (= temp 0.0)) ii i))))))

    (do ((i (- n 1) (- i 1)))
	((< i 0))
      (do ((j (+ i 1) (+ j 1))
	   (sum (vector-ref b i) (- sum (* (array-ref a i j)
					   (vector-ref b j)))))
	  ((>= j n)
	   (vector-set! b i (/ sum (array-ref a i i))))))))

;(define m
;  (array '(4 4)
;         1.0  2.0 3.0 -2.0
;         8.0 -6.0 6.0  1.0
;         3.0 -2.0 0.0 -7.0
;         4.0  7.0 2.0 -1.0))
;
;(define b '#(2.0 1.0 3.0 -2.0))
;
;(define (test m b)
;  (let* ((a (copy-array m))
;         (n (car (array-shape m)))
;         (x (make-vector n)))
;
;    (do ((i 0 (+ i 1)))
;        ((>= i n))
;      (vector-set! x i (vector-ref b i)))
;
;    (display "b = ")
;    (display b)
;    (newline)
;
;    (call-with-values
;     (lambda ()
;       (lu-decomposition a))
;     (lambda (indx sign)
;       (lu-back-substitute a indx x)
;
;       (display "x = ")
;       (display x)
;       (newline)
;
;       (let ((y (make-vector (vector-length b))))
;         (do ((i 0 (+ i 1)))
;             ((>= i n))
;           (do ((j 0 (+ j 1))
;                (t 0.0 (+ t (* (array-ref m i j) (vector-ref x j)))))
;               ((>= j n)
;                (vector-set! y i t))))
;
;         (display "a * x =")
;         (display y)
;         (newline))))))
