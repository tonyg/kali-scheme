; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file bignum.scm.

; Integer arithmetic

(define-extended-number-type :bignum (:exact-integer)
  (make-bignum sign magnitude)
  bignum?
  (sign bignum-sign)
  (magnitude bignum-magnitude))

(define (integer->bignum m)
  (if (bignum? m)
      m
      (cond ((>= m 0)
	     (make-bignum 1 (integer->magnitude m)))
	    ((= m least-non-bignum)
	     (make-bignum -1 least-non-bignum-magnitude))
	    (else
	     (make-bignum -1 (integer->magnitude (- 0 m)))))))

;(define (bignum->integer n)             ;For debugging
;  (* (bignum-sign n)
;     (reduce (lambda (d n) (+ d (* n radix)))
;             0
;             (bignum-magnitude n))))

(define (make-integer sign mag)
  (if (> sign 0)
      (if (smaller-magnitude? greatest-non-bignum-magnitude mag)
	  (make-bignum sign mag)
	  (magnitude->integer mag))
      (if (smaller-magnitude? least-non-bignum-magnitude mag)
	  (make-bignum sign mag)
	  (if (same-magnitude? mag least-non-bignum-magnitude)
	      least-non-bignum
	      (- 0 (magnitude->integer mag))))))

; Arithmetic

(define (integer+ m n)
  (let ((m (integer->bignum m))
	(n (integer->bignum n)))
    (let ((m-sign (bignum-sign m))
	  (m-mag (bignum-magnitude m))
	  (n-sign (bignum-sign n))
	  (n-mag (bignum-magnitude n)))
    (if (= m-sign n-sign)
	(make-integer m-sign (add-magnitudes m-mag n-mag))
	(if (smaller-magnitude? m-mag n-mag)
	    (make-integer (- 0 m-sign) (subtract-magnitudes n-mag m-mag))
	    (make-integer m-sign (subtract-magnitudes m-mag n-mag)))))))

(define (integer- m n)
  (integer+ m (integer-negate n)))

(define (integer-negate m)
  (cond ((bignum? m)
	 (make-integer (- 0 (bignum-sign m))
		       (bignum-magnitude m)))
	((= m least-non-bignum)
	 (make-bignum 1 least-non-bignum-magnitude))
	(else (- 0 m))))

(define (integer* m n)
  (let ((m (integer->bignum m))
	(n (integer->bignum n)))
    (make-integer (* (bignum-sign m) (bignum-sign n))
		  (multiply-magnitudes
		   (bignum-magnitude m)
		   (bignum-magnitude n)))))

(define (integer-divide m n cont)
  (let ((m (integer->bignum m))
	(n (integer->bignum n)))
    (divide-magnitudes
       (bignum-magnitude m)
       (bignum-magnitude n)
       (lambda (q r)
	 (cont (make-integer (* (bignum-sign m) (bignum-sign n)) q)
	       (make-integer (bignum-sign m) r))))))

(define (integer-quotient m n)
  (integer-divide m n (lambda (q r) q)))

(define (integer-remainder m n)
  (integer-divide m n (lambda (q r) r)))

(define integer=
  (lambda (m n)
    (let ((m (integer->bignum m))
	  (n (integer->bignum n)))
      (and (= (bignum-sign m) (bignum-sign n))
	   (same-magnitude? (bignum-magnitude m)
			    (bignum-magnitude n))))))

(define integer<
  (lambda (m n)
    (let ((m (integer->bignum m))
	  (n (integer->bignum n)))
      (let ((m-sign (bignum-sign m))
	    (n-sign (bignum-sign n)))
	(or (< m-sign n-sign)
	    (and (= m-sign n-sign)
		 (if (< m-sign 0)
		     (smaller-magnitude? (bignum-magnitude n)
					 (bignum-magnitude m))
		     (smaller-magnitude? (bignum-magnitude m)
					 (bignum-magnitude n)))))))))


; Magnitude (unsigned integer) arithmetic

(define log-radix 14)			;Cutting it close here...
(define radix (expt 2 log-radix))

(define greatest-non-bignum (+ (expt 2 28) (- (expt 2 28) 1)))
(define least-non-bignum (* (expt 2 28) -2))

(define zero-magnitude '())
(define zero-magnitude? null?)

(define (low-digit m)
  (if (zero-magnitude? m)
      0
      (car m)))

(define (high-digits m)
  (if (zero-magnitude? m)
      m
      (cdr m)))

(define (adjoin-digit d m)
  (if (and (= d 0) (zero-magnitude? m))
      m
      (cons d m)))

(define (integer->magnitude n)
  (if (= n 0)
      zero-magnitude
      (let ((digit (remainder n radix)))
	(adjoin-digit digit
		      (integer->magnitude (quotient n radix))))))

(define (magnitude->integer m)
  (if (zero-magnitude? m)
      0
      (+ (low-digit m)
	 (* radix (magnitude->integer (high-digits m))))))

(define greatest-non-bignum-magnitude
  (integer->magnitude greatest-non-bignum))

(define least-non-bignum-magnitude
  (adjoin-digit (- 0 (remainder least-non-bignum radix))
		(integer->magnitude
		 (- 0 (quotient least-non-bignum radix)))))

; Combine two numbers digitwise using op.

(define (combine-magnitudes m n op)
  (let recur ((m m) (n n) (carry 0))
    (if (and (zero-magnitude? m) (zero-magnitude? n))
	(integer->magnitude carry)
	(let ((result (+ carry (op (low-digit m) (low-digit n)))))
	  (let ((q (quotient result radix))
		(r (remainder result radix)))
	    (if (< r 0)
		(adjoin-digit (+ r radix)
			      (recur (high-digits m)
				     (high-digits n)
				     (- q 1)))
		(adjoin-digit r
			      (recur (high-digits m)
				     (high-digits n)
				     q))))))))

(define (add-magnitudes m n)
  (combine-magnitudes m n +))

(define (subtract-magnitudes m n)
  (combine-magnitudes m n -))

; Compare

(define same-magnitude? equal?)

(define (smaller-magnitude? m n)
  (let ((m-len (length m))
	(n-len (length n)))
    (cond ((< m-len n-len)
	   #t)
	  ((< n-len m-len)
	   #f)
	  (else
	   (let loop ((m m) (n n) (a #f))
	     (cond ((zero-magnitude? m)
		    (or (not (zero-magnitude? n)) a))
		   ((zero-magnitude? n) #f)
		   (else
		    (loop (high-digits m)
			  (high-digits n)
			  (or (< (low-digit m) (low-digit n))
			      (and (= (low-digit m) (low-digit n)) a))))))))))

; Multiply

(define (multiply-magnitudes m n)
  (let recur ((m m) (a zero-magnitude))
    (if (zero-magnitude? m)
	a
	(let ((a (combine-magnitudes a n (lambda (d e)
					   (+ d (* e (low-digit m)))))))
	  (adjoin-digit (low-digit a)
			(recur (high-digits m) (high-digits a)))))))

; Divide m/n: find q and r such that m = q*n + r, where 0 <= r < n.
; Oh no... time to get out Knuth...
; The main thing we don't do that Knuth does is to normalize the
; divisor (n) by shifting it left.

(define (divide-magnitudes m n cont)
  (if (zero-magnitude? (high-digits n))
      (divide-by-digit m (low-digit n)
		       (lambda (q r)
			 (cont q (adjoin-digit r zero-magnitude))))
      (let recur ((m m) (cont cont))
	(if (smaller-magnitude? m n)
	    (cont zero-magnitude m)
	    (recur
	     (high-digits m)
	     (lambda (q r)
	       ;; 0 <= r < n  and  d < b
	       ;; so  b*r + d < b*n.
	       (divide-step (adjoin-digit (low-digit m) r)
			    n
			    (lambda (q1 r)
			      (cont (adjoin-digit q1 q) r)))))))))
	
; Divide m by n, where  n <= m < b*n, i.e. 1 <= quotient < b.
; E.g.  if  n = 100  then  100 <= m <= 999
;       if  n = 999  then  999 <= m <= 9989

(define (divide-step m n cont)
  (do ((m-high m (high-digits m-high))
       (n-high n (high-digits n-high)))
      ((zero-magnitude? (high-digits (high-digits n-high)))
       ;; Occasionally q^ is one larger than the actual first digit.
       ;; This loop will never iterate more than once.
       (let loop ((q^ (min (guess-quotient-digit m-high n-high)
			   (- radix 1))))
	 (let ((r (combine-magnitudes m n (lambda (d e)
					    (- d (* e q^))))))
	   (if (improper-magnitude? r)
	       ;; (begin (write `(addback ,m ,n ,q^ ,r)) (newline) ...)
	       (loop (- q^ 1))
	       (cont q^ r)))))))

; Compute q such that [m1 m2 m3] = q*[n1 n2] + r with 0 <= r < [n1 n2]
; Can assume b <= [0 n1 n2] <= [m1 m2 m3] <= [n1 n2 b-1]
; Some examples:
;  m / n :  100[1] / 10[02], 099 / 10, 099[1] / 99[0], 999[8] / 99[99]
; Various hacks are possible to improve performance.  In particular, the
; second division can be eliminated if the divisor is normalized.
; See Knuth.
;  [m1 m2] = q0*[n1] + r0
;  [m1 m2 m3] = q0*[n1 n2] + r^
;  r^ = b*r0 + m3 - q0*n2

(define (guess-quotient-digit m n)
  (let ((n1 (low-digit (high-digits n)))
	(n2 (low-digit n))
	(m1 (low-digit (high-digits (high-digits m))))
	(m2 (low-digit (high-digits m)))
	(m3 (low-digit m)))
    (let ((m12 (+ (* m1 radix) m2)))
      (let ((q0 (quotient m12 n1))
	    (r0 (remainder m12 n1)))
	(let ((r^ (- (+ (* radix r0) m3) (* q0 n2)))
	      (n12 (+ (* n1 radix) n2)))
	  (let ((q1 (quotient r^ n12))
		(r1 (remainder r^ n12)))
	    (if (> q1 0)
		(begin (display "This should never happen: q1 = ")
		       (write q1) (newline)))
	    (let ((q (+ q0 q1)))
	      (if (< r1 0) (- q 1) q))))))))

(define (improper-magnitude? m)
  (and (not (zero-magnitude? m))
       (or (< (low-digit m) 0)
	   (improper-magnitude? (high-digits m)))))

; Special case of division algorithm for single-digit divisor.

(define (divide-by-digit m d cont)
  (if (= d 0)
      (error "integer division by zero" m d)
      (let recur ((m m) (cont cont))
	(if (and (zero-magnitude? (high-digits m))
		 (< (low-digit m) d))
	    (cont zero-magnitude (low-digit m))
	    (recur (high-digits m)
		   (lambda (q r)
		     (let ((m1 (+ (low-digit m) (* radix r))))
		       (cont (adjoin-digit (quotient m1 d) q)
			     (remainder m1 d)))))))))

;(define (divide-test seed)
;  (let ((random (make-random seed)))
;    (let loop ()
;      (let* ((z1 (integer+ (random) (integer* (random) 10000000)))
;             (z2 (integer+ (random) (integer* (random) 10000000)))
;             (n (max z1 z2))
;             (r (min z1 z2))
;             (q (random))
;             (m (integer+ (integer* n q) r)))
;        (if (not (= n r))
;            (integer-divide m n
;                            (lambda (q1 r1)
;                              (if (and (= q q1) (= r r1))
;                                  (begin (display ".")
;                                         (force-output (current-output-port)))
;                                  (error "division error" m n q q1 r r1)))))
;        (loop)))))


; Extend the generic arithmetic operators.

(define-method &integer? ((n :bignum)) #t)
(define-method &exact? ((n :bignum)) #t)

(define-method &+ ((n1 :exact-integer) (n2 :exact-integer)) (integer+ n1 n2))
(define-method &- ((n1 :exact-integer) (n2 :exact-integer)) (integer- n1 n2))
(define-method &* ((n1 :exact-integer) (n2 :exact-integer)) (integer* n1 n2))
(define-method &= ((n1 :exact-integer) (n2 :exact-integer)) (integer= n1 n2))
(define-method &< ((n1 :exact-integer) (n2 :exact-integer)) (integer< n1 n2))

(define-method &quotient ((n1 :exact-integer) (n2 :exact-integer))
  (integer-quotient n1 n2))
(define-method &remainder ((n1 :exact-integer) (n2 :exact-integer))
  (integer-remainder n1 n2))
