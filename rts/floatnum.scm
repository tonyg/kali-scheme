; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; Inexact rational arithmetic using hacked-in floating point numbers.

(define floatnum-type
  (make-extended-number-type "Floatnum" '(code-vec)))

(define floatnum? (extended-number-predicate floatnum-type))
(define make-floatnum (extended-number-constructor floatnum-type '(code-vec)))
(define floatnum-code-vec (extended-number-accessor floatnum-type 'code-vec))

(define float-vec (make-vector 4))

(define (fixnum->floatnum fix)
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
	  (fixnum->floatnum x)
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


; Methods

(define floatnum-family (make-family 'floatnum 12))  ; higher than rationals
                                                     ; so that 1.1 is a float

(define (define-float-method table proc)
  (define-method table floatnum-family proc))

; Methods on floatnums

(define (true-for-floatnums n)
  (if (floatnum? n) #t (fail)))

(define-float-method rational?-table true-for-floatnums)
(define-float-method real?-table     true-for-floatnums)
(define-float-method complex?-table  true-for-floatnums)
(define-float-method number?-table   true-for-floatnums)

; Methods on complexes in terms of real-part and imag-part

(define-float-method exact?-table
  (lambda (z)
    (if (floatnum? z)
	#f
	(fail))))

(define-float-method inexact->exact-table
  (lambda (z)
    (if (and (floatnum? z)
	     (integral-floatnum? z)
	     (in-fixnum-range? z))
	(floatnum->fixnum z)
	(fail))))  ; ain't no way

(define-float-method exact->inexact-table
  (lambda (z)
    (if (rational? z)
	(x->float z)
	(fail))))

(define (when-floatnum proc)
  (lambda (n)
    (if (floatnum? n) (proc n) (fail))))

(define (when-floatable proc)
  (lambda (n)
    (if (rational? n) (proc n) (fail))))

(define-float-method integer?-table (when-floatnum integral-floatnum?))

(define (when-floatnums proc)
  (lambda (m n)
    (if (and (or (floatnum? m) (floatnum? n))
	     (real? m)
	     (real? n))
	(proc m n)
	(fail))))

(define-float-method plus-table (when-floatnums float+))
(define-float-method minus-table (when-floatnums float-))
(define-float-method *-table (when-floatnums float*))
(define-float-method /-table (when-floatnums float/))
(define-float-method quotient-table (when-floatnums float-quotient))
(define-float-method remainder-table (when-floatnums float-remainder))
(define-float-method =-table (when-floatnums float=))
(define-float-method <-table (when-floatnums float<))
(define-float-method floor-table (when-floatnum float-floor))

(define-float-method exp-table (when-floatable float-exp))
(define-float-method log-table (when-floatable float-log))
(define-float-method sqrt-table (when-floatable float-sqrt))

(define-float-method number->string-table
  (lambda (n radix)
    (if (and (floatnum? n) (= radix 10))
	(float->string n)
	(fail))))

(define-float-method string->number-table
  (lambda (s radix exact?)
    (if (and (= radix 10)
	     (float-string? s)
	     (not exact?))
	(string->float s)
	(fail))))

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
