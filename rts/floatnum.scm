; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; Inexact rational arithmetic using hacked-in floating point numbers.

(define-extended-number-type :floatnum (:rational)
  (make-floatnum code-vec)
  floatnum?
  (code-vec floatnum-code-vec))

; Floating point at interrupt level?  Naw!
(define float-vec (make-vector 4))

(define (fixnum->float fix)
  (let ((res (make-code-vector 8 0)))
    (vector-set! float-vec 0 6)
    (vector-set! float-vec 1 fix)
    (vector-set! float-vec 2 res)
    (vm-extension 99 float-vec)
    (make-floatnum res)))

(define (float&float op float1 float2)
  (vector-set! float-vec 0 op)
  (vector-set! float-vec 1 (floatnum-code-vec float1))
  (vector-set! float-vec 2 (floatnum-code-vec float2))
  (vm-extension 99 float-vec))

(define (float&float->float op)
  (lambda (a b)
    (let ((float1 (x->float a))
	  (float2 (x->float b)))
      (let ((res (make-code-vector 8 0)))
	(vector-set! float-vec 3 res)
	(float&float op float1 float2)
	(make-floatnum res)))))

(define (float&float->boolean op)
  (lambda (a b)
    (let ((float1 (x->float a))
	  (float2 (x->float b)))
      (= 0 (float&float op float1 float2)))))

(define (float1 op float)
  (vector-set! float-vec 0 op)
  (vector-set! float-vec 1 (floatnum-code-vec float))
  (vm-extension 99 float-vec))

(define (float->float op)
  (lambda (a)
    (let* ((float (x->float a))
	   (res (make-code-vector 8 0)))
      (vector-set! float-vec 2 res)
      (float1 op float)
      (make-floatnum res))))

(define (string->float string)
  (let ((res (make-code-vector 8 0)))
    (vector-set! float-vec 0 7)
    (vector-set! float-vec 1 string)
    (vector-set! float-vec 2 (string-length string))
    (vector-set! float-vec 3 res)
    (vm-extension 99 float-vec)
    (make-floatnum res)))

(define (float->string float)
  (let ((res (make-string 40 #\space)))
    (vector-set! float-vec 0 8)
    (vector-set! float-vec 1 (floatnum-code-vec float))
    (vector-set! float-vec 2 res)
    (vm-extension 99 float-vec)
    (let ((str (substring res 0 (vector-ref float-vec 3))))
      (let loop ((i 0))
	(cond ((>= i (string-length str))
	       (string-append str "."))
	      ((or (char=? (string-ref str i) #\e)
		   (char=? (string-ref str i) #\.))
	       str)
	      (else
	       (loop (+ i 1))))))))

(define (x->float x)
  (cond ((floatnum? x) x)
	((integer? x)
	 (integer->float (if (exact? x)
			     x
			     (inexact->exact x))))
	((rational? x)
	 (float/ (numerator x) (denominator x)))
	(else
	 (error "cannot coerce to a float" x))))

(define integer->float
  (let ((foo (expt 2 28)))
    (lambda (x)
      (if (in-fixnum-range? x)
	  (fixnum->float x)
	  (float+ (float* foo (quotient x foo))
		  (remainder x foo))))))

(define in-fixnum-range?
  (let ((foo (expt 2 28)))
    (let ((most-positive (+ foo (- foo 1)))
	  (most-negative (* foo -2)))
      (lambda (x)
	(and (<= x most-positive)
	     (>= x most-negative))))))

(define (floatnum->fixnum float) (float1 20 float))

(define (integral-floatnum? float) (float1 19 float))

(define float+ (float&float->float 0))
(define float- (float&float->float 1))
(define float* (float&float->float 2))
(define float/ (float&float->float 3))
(define float-quotient (float&float->float 21))
(define float-remainder (float&float->float 22))
(define float= (float&float->boolean 4))
(define float< (float&float->boolean 5))


(define float-exp (float->float 9))
(define float-log (float->float 10))
(define float-sin (float->float 11))
(define float-cos (float->float 12))
(define float-tan (float->float 13))
(define float-asin (float->float 14))
(define float-acos (float->float 15))
(define float-sqrt (float->float 17))
(define float-floor (float->float 18))

(define float-atan (float&float->float 16))

; Temporary hack.  Do ,open floatnum
(begin 
  (define exp float-exp)
  (define log float-log)
  (define sin float-sin)
  (define cos float-cos)
  (define tan float-tan)
  (define asin float-asin)
  (define acos float-acos)
  (define atan float-atan)
  (define sqrt float-sqrt))

(define (float-fraction-length z)
  (let ((two (integer->float 2)))
    (do ((z z (float* z two))
	 (i 0 (+ i 1)))
	((integral-floatnum? z) i)
      (if (> i 1000) (error "I'm bored." z)))))

(define (float-denominator z)
  (expt (integer->float 2) (float-fraction-length z)))

(define (float-numerator z)
  (float* z (float-denominator z)))

(define (float->exact z)
  (if (integral-floatnum? z)
      (float->exact-integer z)
      (let ((q (expt 2 (float-fraction-length z))))
	(let ((e (/ (float->exact-integer (float* z (integer->float q)))
		    q)))
	  (if (exact? e)
	      e
	      (call-error "no exact representation"
			  inexact->exact z))))))

(define float->exact-integer
  (let ((foo (expt 2 28)))
    (lambda (x)
      (if (integral-floatnum? x)
	  (if (in-fixnum-range? x)
	      (floatnum->fixnum x)
	      (+ (* foo (inexact->exact (quotient x foo)))
		 (inexact->exact (remainder x foo))))
	  (call-error "invalid argument" float->exact-integer x)))))


; Methods on floatnums

(define-method &integer? ((z :floatnum))
  (integral-floatnum? z))

(define-method &rational? ((n :floatnum)) #t)

(define-method &exact? ((z :floatnum)) #f)

(define-method &inexact->exact ((z :floatnum))
  (float->exact z))

(define-method &exact->inexact ((z :rational))
  (x->float z))		;Should do this only if the number is within range.

(define-method &floor ((z :floatnum)) (float-floor z))

; beware infinite regress
(define-method &numerator ((z :floatnum)) (float-numerator z))
(define-method &denominator ((z :floatnum)) (float-denominator z))

(define (define-floatnum-method mtable proc)
  (define-method mtable ((m :rational) (n :rational)) (proc m n)))

(define :lose :rational)

(define-floatnum-method &+ float+)
(define-floatnum-method &- float-)
(define-floatnum-method &* float*)
(define-floatnum-method &/ float/)
(define-floatnum-method &quotient float-quotient)
(define-floatnum-method &remainder float-remainder)
(define-floatnum-method &= float=)
(define-floatnum-method &< float<)

(define-method &numerator ((z :rational)) (float-numerator z))
(define-method &denominator ((z :rational)) (float-denominator z))

(define-method &exp ((z :rational)) (float-exp z))
(define-method &log ((z :rational)) (float-log z))
(define-method &sqrt ((z :rational)) (float-sqrt z))
(define-method &sin ((z :rational)) (float-sin z))
(define-method &cos ((z :rational)) (float-cos z))
(define-method &tan ((z :rational)) (float-tan z))
(define-method &acos ((z :rational)) (float-acos z))
(define-method &atan ((z :rational)) (float-atan z))

(define-method &number->string ((n :floatnum) radix)
  (if (= radix 10)
      (float->string n)
      (next-method)))

; Oog.

(define (float-string? s)
  (let ((len (string-length s)))
    (define (start)
      (cond ((< len 2)
	     #f)
	    ((char-numeric? (string-ref s 0))
	     (digits 1 #f #f))
	    ((and (or (char=? (string-ref s 0) #\+)
		      (char=? (string-ref s 0) #\-))
		  (char-numeric? (string-ref s 1)))
	     (digits 2 #f #f))
	    ((and (char=? (string-ref s 0) #\.)
		  (char-numeric? (string-ref s 1)))
	     (digits 2 #t #f))
	    (else #f)))
    (define (digits i dot? e?)
      (cond ((>= i len) dot?)
	    ((char-numeric? (string-ref s i))
	     (digits (+ i 1) dot? e?))
	    ((and (char=? (string-ref s i) #\e)
		  (not e?))
	     (exponent (+ i 1)))
	    ((and (char=? (string-ref s i) #\.)
		  (not dot?))
	     (digits (+ i 1) #t #f))
	    (else #f)))
    (define (exponent i)
      (cond ((>= i len) #f)
	    ((char-numeric? (string-ref s i))
	     (digits (+ i 1) #t #t))
	    ((or (char=? (string-ref s i) #\+)
		 (char=? (string-ref s i) #\-))
	     (exponent2 (+ i 1)))
	    (else #f)))
    (define (exponent2 i)
      (cond ((>= i len) #f)
	    ((char-numeric? (string-ref s i))
	     (digits (+ i 1) #t #t))
	    (else #f)))
    
    (start)))

(define-simple-type :float-string (:string) float-string?)

(define-method &really-string->number ((s :float-string) radix exact?)
  (if (and (= radix 10)
	   (not exact?))
      (string->float s)
      (next-method)))
