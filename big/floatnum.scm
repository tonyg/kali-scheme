; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; Real arithmetic using hacked-in floating point numbers.

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

(define (float&float->float op float1 float2)
  (let ((res (make-code-vector 8 0)))
    (vector-set! float-vec 3 res)
    (float&float op float1 float2)
    (make-floatnum res)))

(define (float&float->boolean op float1 float2)
  (= 0 (float&float op float1 float2)))

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
	((or (not (integer? x))
	     (extended-number? x))
	 (error "cannot coerce to a float" x))
	(else
	 (fixnum->floatnum x))))

(define (float+ a b) (float&float->float 0 (x->float a) (x->float b)))
(define (float- a b) (float&float->float 1 (x->float a) (x->float b)))
(define (float* a b) (float&float->float 2 (x->float a) (x->float b)))
(define (float/ a b) (float&float->float 3 (x->float a) (x->float b)))
(define (float= a b) (float&float->boolean 4 (x->float a) (x->float b)))
(define (float< a b) (float&float->boolean 5 (x->float a) (x->float b)))


; Methods

(define floatnum-family (make-family 'floatnum 10))  ; higher than rationals
                                                     ; so that 1.1 is a float

(define (define-float-method table proc)
  (define-method table floatnum-family proc))

; Methods on floatnums

(define (true-for-floatnums n)
  (if (floatnum? n) #t (fail)))

(define-float-method real?-table    true-for-floatnums)
(define-float-method complex?-table true-for-floatnums)
(define-float-method number?-table  true-for-floatnums)

(define (when-floatnum proc)
  (lambda (n) (if (floatnum? n) (proc n) (fail))))

; Methods on complexes in terms of real-part and imag-part

(define-float-method exact?-table
  (lambda (z)
    (if (floatnum? z)
	#f
	(fail))))

(define-float-method inexact->exact-table
  (lambda (z)
    (fail)))  ; ain't no way

(define-float-method exact->inexact-table
  (lambda (z)
    (if (floatnum? z)
	z
	(fail))))

(define (when-reals proc)
  (lambda (m n)
    (if (and (real? m) (real? n)
	     (not (and (rational? m) (rational? n))))
	(proc m n)
	(fail))))

(define-float-method plus-table (when-reals float+))
(define-float-method minus-table (when-reals float-))
(define-float-method *-table (when-reals float*))
(define-float-method /-table (when-reals float/))
(define-float-method =-table (when-reals float=))
(define-float-method <-table (when-reals float<))

(define-float-method number->string-table
  (lambda (n radix)
    (if (and (real? n) (not (rational? n)) (= radix 10))
	(float->string n)
	(fail))))

(define-float-method string->number-table
  (lambda (s radix exact?)
    (if (or (not (= radix 10))
	    ; exact?
	    (not (float-string? s)))
	(fail)
	(string->float s))))

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
