; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Inexact rational arithmetic using hacked-in floating point numbers.

(define-extended-number-type :floatnum (:rational)
  (make-floatnum datum)
  floatnum?
  (datum floatnum-datum))

(define (make-float-datum) (make-code-vector 8 0))

(define-enumeration flop
  (+ - * / = <
   fixnum->float
   string->float
   float->string
   exp log sin cos tan asin acos atan sqrt
   floor
   integer?
   float->fixnum
   quotient
   remainder))

(define-syntax floperate
  (syntax-rules ()
    ((floperate ?which ?x)
     (vm-extension (+ ?which 100) ?x))
    ((floperate ?which ?x ?y)
     (vm-extension (+ ?which 100) (cons ?x ?y)))
    ((floperate ?which ?x ?y ?z)
     (vm-extension (+ ?which 100) (vector ?x ?y ?z)))))

(define (float&float->float op)
  (lambda (a b)
    (let ((float1 (x->float a))
	  (float2 (x->float b))
	  (res (make-float-datum)))
      (floperate op
		 (floatnum-datum float1)
		 (floatnum-datum float2)
		 res)
      (make-floatnum res))))

(define (float&float->boolean op)
  (lambda (a b)
    (let ((float1 (x->float a))
	  (float2 (x->float b)))
      (floperate op
		 (floatnum-datum float1)
		 (floatnum-datum float2)))))

(define (float1 op)
  (lambda (float)
    (floperate op (floatnum-datum float))))

(define (float->float op)
  (lambda (a)
    (let ((float (x->float a))
	  (res (make-float-datum)))
      (floperate op (floatnum-datum float) res)
      (make-floatnum res))))

(define (string->float string)
  (let ((res (make-float-datum)))
    (floperate (enum flop string->float) string res)
    (make-floatnum res)))

(define (float->string float)
  (let* ((res (make-string 40 #\space))
	 (len (floperate (enum flop float->string)
			 (floatnum-datum float)
			 res))
	 (str (substring res 0 len)))
    (let loop ((i 0))
      (cond ((>= i (string-length str))
	     (string-append str "."))
	    ((or (char=? (string-ref str i) #\e)
		 (char=? (string-ref str i) #\.))
	     str)
	    (else
	     (loop (+ i 1)))))))

(define (x->float x)
  (cond ((floatnum? x) x)
	((integer? x)
	 (exact-integer->float (if (exact? x)
				   x
				   (inexact->exact x))))
	((rational? x)
	 ;; This loses when num or den overflows flonum range
	 ;; but x doesn't.
	 (float/ (numerator x) (denominator x)))
	(else
	 (error "cannot coerce to a float" x))))

; Conversion to/from exact integer

(define (exact-integer->float k)
  (or (fixnum->float k)
      (float+ (float* (fixnum->float definitely-a-fixnum)
		      (quotient k definitely-a-fixnum))
	      (fixnum->float (remainder k definitely-a-fixnum)))))

(define (fixnum->float k)    ;Returns #f is k is a bignum
  (let ((res (make-float-datum)))
    (if (floperate (enum flop fixnum->float) k res)
	(make-floatnum res)
	#f)))

(define (float->exact-integer x)
  (or (float->fixnum x)
      (let ((d (fixnum->float definitely-a-fixnum)))
	(+ (* definitely-a-fixnum
	      (float->exact-integer (float-quotient x d)))
	   (float->fixnum (float-remainder x d))))))

(define definitely-a-fixnum (expt 2 23))    ;Be conservative

(define integral-floatnum? (float1 (enum flop integer?)))
(define float->fixnum      (float1 (enum flop float->fixnum)))

(define float+ (float&float->float (enum flop +)))
(define float- (float&float->float (enum flop -)))
(define float* (float&float->float (enum flop *)))
(define float/ (float&float->float (enum flop /)))
(define float-quotient (float&float->float (enum flop quotient)))
(define float-remainder (float&float->float (enum flop remainder)))
(define float-atan (float&float->float (enum flop atan)))

(define float= (float&float->boolean (enum flop =)))
(define float< (float&float->boolean (enum flop <)))

(define float-exp (float->float (enum flop exp)))
(define float-log (float->float (enum flop log)))
(define float-sin (float->float (enum flop sin)))
(define float-cos (float->float (enum flop cos)))
(define float-tan (float->float (enum flop tan)))
(define float-asin (float->float (enum flop asin)))
(define float-acos (float->float (enum flop acos)))
(define float-sqrt (float->float (enum flop sqrt)))
(define float-floor (float->float (enum flop floor)))

; This lets you do ,open floatnum to get faster invocation
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

(define (float-fraction-length x)
  (let ((two (exact-integer->float 2)))
    (do ((x x (float* x two))
	 (i 0 (+ i 1)))
	((integral-floatnum? x) i)
      (if (> i 1000) (error "I'm bored." x)))))

(define (float-denominator x)
  (expt (exact-integer->float 2) (float-fraction-length x)))

(define (float-numerator x)
  (float* x (float-denominator x)))

(define (float->exact x)
  (if (integral-floatnum? x)
      (float->exact-integer x)		;+++
      (let ((lose (lambda ()
		    (call-error "no exact representation"
				inexact->exact x)))
	    (q (expt 2 (float-fraction-length x))))
	(if (exact? q)
	    (let ((e (/ (float->exact-integer
			     (float* x (exact-integer->float q)))
			q)))
	      (if (exact? e)
		  e
		  (lose)))
	    (lose)))))


; Methods on floatnums

(define-method &integer? ((x :floatnum))
  (integral-floatnum? x))

(define-method &rational? ((n :floatnum)) #t)

(define-method &exact? ((x :floatnum)) #f)

(define-method &inexact->exact ((x :floatnum))
  (float->exact x))

(define-method &exact->inexact ((x :rational))
  (x->float x))		;Should do this only if the number is within range.

(define-method &floor ((x :floatnum)) (float-floor x))

; beware infinite regress
(define-method &numerator ((x :floatnum)) (float-numerator x))
(define-method &denominator ((x :floatnum)) (float-denominator x))

(define (define-floatnum-method mtable proc)
  (define-method mtable ((m :rational) (n :rational)) (proc m n)))

(define-floatnum-method &+ float+)
(define-floatnum-method &- float-)
(define-floatnum-method &* float*)
(define-floatnum-method &/ float/)
(define-floatnum-method &quotient float-quotient)
(define-floatnum-method &remainder float-remainder)
(define-floatnum-method &= float=)
(define-floatnum-method &< float<)

(define-method &numerator ((x :rational)) (float-numerator x))
(define-method &denominator ((x :rational)) (float-denominator x))

(define-method &exp ((x :rational)) (float-exp x))
(define-method &log ((x :rational)) (float-log x))
(define-method &sqrt ((x :rational)) (float-sqrt x))
(define-method &sin ((x :rational)) (float-sin x))
(define-method &cos ((x :rational)) (float-cos x))
(define-method &tan ((x :rational)) (float-tan x))
(define-method &acos ((x :rational)) (float-acos x))

(define-floatnum-method &atan float-atan)

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
