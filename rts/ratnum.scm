; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; This is file ratnum.scm.

; Rational arithmetic
; Assumes that +, -, etc. perform integer arithmetic.

(define ratnum-type
  (make-extended-number-type "Ratnum" '(num den)))

(define make-ratnum (extended-number-constructor ratnum-type '(num den)))
(define ratnum? (extended-number-predicate ratnum-type))
(define ratnum-numerator   (extended-number-accessor ratnum-type 'num))
(define ratnum-denominator (extended-number-accessor ratnum-type 'den))

(define extended-rational? ratnum?)

(define (make-rational m n)
  (if (< n 0)
      (make-rational (- 0 m) (- 0 n))
      (if (= n 0)
	  (error "rational division by zero" m)
	  (let ((g (gcd m n)))
	    (let ((m (quotient m g))
		  (n (quotient n g)))
	      (if (= n 1)
		  m
		  (make-ratnum m n)))))))

(define (rational-numerator p)
  (if (ratnum? p)
      (ratnum-numerator p)
      (numerator p)))

(define (rational-denominator p)
  (if (ratnum? p)
      (ratnum-denominator p)
      (denominator p)))

; a/b * c/d = a*c / b*d

(define (rational* p q)
  (make-rational (* (rational-numerator p) (rational-numerator q))
		 (* (rational-denominator p) (rational-denominator q))))

; a/b / c/d = a*d / b*c

(define (rational/ p q)
  (make-rational (* (rational-numerator p) (rational-denominator q))
		 (* (rational-denominator p) (rational-numerator q))))

; a/b + c/d = (a*d + b*c)/(b*d)

(define (rational+ p q)
  (make-rational (+ (* (rational-numerator p) (rational-denominator q))
		    (* (rational-denominator p) (rational-numerator q)))
		 (* (rational-denominator p) (rational-denominator q))))

; a/b - c/d = (a*d - b*c)/(b*d)

(define (rational- p q)
  (make-rational (- (* (rational-numerator p) (rational-denominator q))
		    (* (rational-denominator p) (rational-numerator q)))
		 (* (rational-denominator p) (rational-denominator q))))

; a/b < c/d  when  a*d < b*c

(define (rational< p q)
  (< (* (rational-numerator p) (rational-denominator q))
     (* (rational-denominator p) (rational-numerator q))))

; a/b = c/d  when a = b and c = d  (always lowest terms)

(define (rational= p q)
  (and (= (rational-numerator p) (rational-numerator q))
       (= (rational-denominator p) (rational-denominator q))))

; (rational-truncate p) = integer of largest magnitude <= (abs p)

(define (rational-truncate p)
  (quotient (rational-numerator p) (rational-denominator p)))

; (floor p) = greatest integer <= p

(define (rational-floor p)
  (if (integer? p)
      p
      (let ((q (quotient (rational-numerator p) (rational-denominator p))))
	(if (>= (rational-numerator p) 0)
	    q
	    (- q 1)))))

;

(define (rational-exact? r)
  (exact? (rational-numerator r)))

(define (rational-exact->inexact p)
  (make-rational (exact->inexact (rational-numerator p))
		 (exact->inexact (rational-denominator p))))

(define (rational-inexact->exact p)
  (make-rational (inexact->exact (rational-numerator p))
		 (inexact->exact (rational-denominator p))))

; Extend the generic number procedures

(define ratnum-family (make-family 'ratnum 8))

(define (define-rational-method table proc)
  (define-method table ratnum-family proc))

; Methods for ratnums only:

(define (true-for-ratnums n)
  (if (ratnum? n) #t (fail)))

(define-rational-method rational?-table true-for-ratnums)
(define-rational-method real?-table     true-for-ratnums)
(define-rational-method complex?-table  true-for-ratnums)
(define-rational-method number?-table   true-for-ratnums)

(define (when-ratnum proc)
  (lambda (n) (if (ratnum? n) (proc n) (fail))))

(define-rational-method numerator-table   (when-ratnum ratnum-numerator))
(define-rational-method denominator-table (when-ratnum ratnum-denominator))

; Methods that work on all rationals, assuming that numerator and
; denominator work:

(define (when-rational proc)
  (lambda (n)
    (if (and (rational? n) (not (integer? n)))
	(proc n)
	(fail))))

(define-rational-method exact?-table (when-rational rational-exact?))

(define-rational-method exact->inexact-table
  (when-rational rational-exact->inexact))
(define-rational-method inexact->exact-table
  (when-rational rational-inexact->exact))

(define (when-rationals proc)
  (lambda (m n)
    (if (and (rational? m)
	     (rational? n)
	     ;; Careful -- we can't make progress in this case without bignums!
	     (not (and (integer? m) (integer? n))))
	(proc m n)
	(fail))))

(define-rational-method plus-table (when-rationals rational+))
(define-rational-method minus-table (when-rationals rational-))
(define-rational-method *-table (when-rationals rational*))
(define-rational-method =-table (when-rationals rational=))
(define-rational-method <-table (when-rationals rational<))

(define-rational-method /-table
  (lambda (m n)
    (if (and (rational? m) (rational? n) (not (= n 0)))
	(rational/ m n)
	(fail))))

(define-rational-method floor-table (when-rational rational-floor))

(define (rational-sqrt p)
  (let* ((numerator (rational-numerator p))
	 (neg? (< numerator 0))
	 (ans (make-rational (sqrt (if neg? (- 0 numerator) numerator))
			     (sqrt (rational-denominator p)))))
    (if neg? (make-rectangular 0 ans) ans)))

(define-rational-method sqrt-table
  (when-rational rational-sqrt))

; I/O
; Implement exponential notation one of these days.

(define-rational-method number->string-table
  (lambda (n radix)
    (if (and (rational? n)
	     (not (integer? n)))
	(if (exact? n)
	    (ratio-string (numerator n) (denominator n) radix)
	    (let ((n (inexact->exact n)))
	      (let ((p (numerator n))
		    (q (denominator n)))
		(if (= radix 10)
		    (let ((foo (decimable? q)))
		      (if foo
			  (decimal-representation p q foo)
			  (string-append "#i" (ratio-string p q radix))))
		    (string-append "#i" (ratio-string p q radix))))))
	(fail))))

; The Scheme report obligates us to print inexact rationals using
; decimal points whenever this can be done without losing precision.

(define (decimal-representation p q foo)
  (let ((kludge (really-number->string (* (car foo) (abs (remainder p q)))
				       10)))
    (string-append (if (< p 0) "-" "")
		   (really-number->string (quotient (abs p) q) 10)
		   "."
		   (string-append (do ((i (- (cdr foo) (string-length kludge))
					  (- i 1))
				       (l '() (cons #\0 l)))
				      ((<= i 0) (list->string l)))
				  kludge))))


(define (ratio-string p q radix)
  (string-append (really-number->string p radix)
		 "/"
		 (really-number->string q radix)))

(define (decimable? n)
  (let loop ((n n) (d 1) (i 0))
    (if (= n 1)
	(cons d i)
	(let ((q (quotient n 10))
	      (r (remainder n 10)))
	  (cond ((= r 0) (loop q d (+ i 1)))
		((= r 5) (loop (quotient n 5) (* d 2) (+ i 1)))
		((even? r) (loop (quotient n 2) (* d 5) (+ i 1)))
		(else #f))))))

; This accepts way too many things.  Needs to be rethought and rewritten.

(define-rational-method string->number-table
  (lambda (s radix exact?)
    (let ((len (string-length s)))
      (cond ((or (char=? (string-ref s (- len 1)) #\i)
		 (string-position #\@ s))
	     (fail))		;let the complex number parser handle this
	    ((string-position #\/ s)
	     => (lambda (slash)
		  (let ((p (string->integer (substring s 0 slash)
					    radix exact?))
			(q (string->integer (substring s (+ slash 1) len)
					    radix exact?)))
		    (if (and p q (not (= q 0)))
			(/ p q)
			#f))))
	    ((string-position #\. s)
	     => (lambda (dot)
		  ;; Talk about kludges.  This is REALLY kludgey.
		  (if (> len 1)
		      (let* ((j (if (or (char=? (string-ref s 0) #\+)
					(char=? (string-ref s 0) #\-))
				    1
				    0))
			     (p (if (= dot j)
				    0
				    (string->integer (substring s j dot)
						     radix exact?)))
			     (q (if (= dot (- len 1))
				    0
				    (string->integer (substring s (+ dot 1)
								len)
						     radix exact?))))
			(if (and p q)
			    (let ((n (+ p (/ q (expt radix
						     (- len (+ dot 1)))))))
			      (if (char=? (string-ref s 0) #\-)
				  (- 0 n)
				  n))
			    (fail)))
		      #f)))
	    (else
	     (fail))))))

(define (string-position c s)
  (let loop ((i 0))
    (if (>= i (string-length s))
	#f
	(if (char=? c (string-ref s i))
	    i
	    (loop (+ i 1))))))

