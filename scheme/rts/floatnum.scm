; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Inexact rational arithmetic using hacked-in floating point numbers.

(define floatnum? double?)

(define-enumeration flop
  (+ - * / = <
   fixnum->float
   string->float
   float->string
   exp log sin cos tan asin acos atan1 atan2 sqrt
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
    (let ((res (make-double)))
      (floperate op (x->float a) (x->float b) res)
      res)))

(define (float&float->boolean op)
  (lambda (a b)
    (floperate op (x->float a) (x->float b))))

(define (float1 op)
  (lambda (float)
    (floperate op float)))

(define (float2 op)
  (lambda (a b)
    (floperate op a b)))

(define (float->float op)
  (lambda (a)
    (let ((res (make-double)))
      (floperate op (x->float a) res)
      res)))

(define (string->float string)
  (let ((res (make-double)))
    (or (floperate (enum flop string->float) string res)
	(error "not enough memory for STRING->FLOAT string buffer" string))))

; Call the VM to get a string

(define (float->string float)
  (let* ((res (make-string 40 #\space))
	 (len (floperate (enum flop float->string)
			 float
			 res)))
    (substring res 0 len)))

(define (x->float x)
  (cond ((double? x)
	 x)
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
  (let ((res (make-double)))
    (if (floperate (enum flop fixnum->float) k res)
	res
	#f)))

(define (float->exact-integer x)
  (or (float->fixnum x)
      (let ((d (fixnum->float definitely-a-fixnum)))
	(+ (* definitely-a-fixnum
	      (float->exact-integer (float-quotient x d)))
	   (float->fixnum (loophole :double		; outsmarted ourselves
				    (float-remainder x d)))))))

(define definitely-a-fixnum (expt 2 23))    ;Be conservative

(define integral-floatnum? (float1 (enum flop integer?)))
(define float->fixnum      (float1 (enum flop float->fixnum)))

(define float+ (float&float->float (enum flop +)))
(define float- (float&float->float (enum flop -)))
(define float* (float&float->float (enum flop *)))
(define float/ (float&float->float (enum flop /)))
(define float-quotient (float&float->float (enum flop quotient)))
(define float-remainder (float&float->float (enum flop remainder)))
(define float-atan1 (float->float (enum flop atan1)))
(define float-atan2 (float&float->float (enum flop atan2)))

(define float= (float2 (enum flop =)))
(define float< (float2 (enum flop <)))

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
  (define (atan a . maybe-b)
    (cond ((null? maybe-b)
	   (float-atan1 a))
	  ((null? (cdr maybe-b))
	   (float-atan2 a (car maybe-b)))
	  (else
	   (error "too many arguments to ATAN" (cons a maybe-b)))))
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

(define-method &integer? ((x :double))
  (integral-floatnum? x))

(define-method &rational? ((n :double)) #t)

(define-method &exact? ((x :double)) #f)

(define-method &inexact->exact ((x :double))
  (float->exact x))

(define-method &exact->inexact ((x :rational))
  (x->float x))		;Should do this only if the number is within range.

(define-method &floor ((x :double)) (float-floor x))

; beware infinite regress
(define-method &numerator ((x :double)) (float-numerator x))
(define-method &denominator ((x :double)) (float-denominator x))

(define (define-floatnum-method mtable proc)
  (define-method mtable ((m :rational) (n :rational)) (proc m n)))

;; the numerical tower sucks
(define (define-floatnum-comparison mtable proc float-proc)
  (define-method mtable ((m :double) (n :double)) (float-proc m n))
  (define-method mtable ((m :double) (n :rational))
    (proc (float->exact m) n))
  (define-method mtable ((m :rational) (n :double))
    (proc m (float->exact n))))

(define-floatnum-method &+ float+)
(define-floatnum-method &- float-)
(define-floatnum-method &* float*)
(define-floatnum-method &/ float/)
(define-floatnum-method &quotient float-quotient)
(define-floatnum-method &remainder float-remainder)
(define-floatnum-comparison &= = float=)
(define-floatnum-comparison &< < float<)
(define-floatnum-method &atan2 float-atan2)

(define-method &exp   ((x :rational)) (float-exp   x))
(define-method &log   ((x :rational)) (float-log   x))
(define-method &sqrt  ((x :rational)) (float-sqrt  x))
(define-method &sin   ((x :rational)) (float-sin   x))
(define-method &cos   ((x :rational)) (float-cos   x))
(define-method &tan   ((x :rational)) (float-tan   x))
(define-method &acos  ((x :rational)) (float-acos  x))
(define-method &atan1 ((x :rational)) (float-atan1 x))

(define-method &number->string ((n :double) radix)
  (if (= radix 10)
      (float->string n)
      (next-method)))

; Recognizing a floating point number.  This doesn't know about `#'.

(define (float-string? s)
  (let ((len (string-length s)))
    (define (start)
      (and (< 1 len)
	   (let ((first (string-ref s 0))
		 (second (string-ref s 1)))
	     (if (char-numeric? first)
		 (digits 1 #f #f)
		 (case first
		   ((#\+ #\-)
		    (and (char-numeric? second)
			 (digits 2 #f #f)))
		   ((#\.)
		    (and (char-numeric? second)
			 (digits 2 #t #f)))
		   (else #f))))))

    ; Read digits until the end or an `e' or a `.'.  E-OR-DOT? is true if
    ; we have seen either, E? is true if we've seen an `e'.
    (define (digits i e-or-dot? e?)
      (if (= i len)
	  e-or-dot?
	  (let ((next (string-ref s i)))
	    (if (char-numeric? next)
		(digits (+ i 1) e-or-dot? e?)
		(case next
		  ((#\e #\E)
		   (and (not e?)
			(exponent (+ i 1) #f)))
		  ((#\.)
		   (and (not e-or-dot?)
			(digits (+ i 1) #t #f)))
		  (else #f))))))

    ; Read in an exponent.  If SIGN? is true then we have already got the sign.
    (define (exponent i sign?)
      (and (< i len)
	   (let ((next (string-ref s i)))
	     (if (char-numeric? next)
		 (digits (+ i 1) #t #t)
		 (case next
		   ((#\+ #\-)
		    (and (not sign?)
			 (exponent (+ i 1) #t)))
		   (else #f))))))
    (start)))

(define-simple-type :float-string (:string) float-string?)

(define-method &really-string->number ((s :float-string) radix exact?)
  (if (and (= radix 10)
	   (not exact?))
      (string->float s)
      (next-method)))
