; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file xnum.scm.

;;;; Extended number support

(define-simple-type :extended-number (:number) extended-number?)

(define-record-type extended-number-type :extended-number-type
  (really-make-extended-number-type field-names supers priority predicate id)
  extended-number-type?
  (field-names extended-number-type-field-names)
  (supers      extended-number-type-supers)
  (priority    extended-number-type-priority)
  (predicate   extended-number-predicate)
  (id	       extended-number-type-identity))

(define-record-discloser :extended-number-type
  (lambda (e-n-t)
    (list 'extended-number-type (extended-number-type-identity e-n-t))))

(define (make-extended-number-type field-names supers id)
  (letrec ((t (really-make-extended-number-type
	       field-names
	       supers
	       (+ (apply max
			 (map type-priority
			      (cons :extended-number supers)))
		  10)
	       (lambda (x)
		 (and (extended-number? x)
		      (eq? (extended-number-type x) t)))
	       id)))
    t))

(define (extended-number-type x) (extended-number-ref x 0))


; DEFINE-EXTENDED-NUMBER-TYPE macro

(define-syntax define-extended-number-type
  (syntax-rules ()
    ((define-extended-number-type ?type (?super ...)
       (?constructor ?arg1 ?arg ...)
       ?predicate
       (?field ?accessor)
       ...)
     (begin (define ?type
	      (make-extended-number-type '(?field ...)
					 (list ?super ...)
					 '?type))
	    (define ?constructor
	      (let ((args '(?arg1 ?arg ...)))
		(if (equal? args
			    (extended-number-type-field-names ?type))
		    (let ((k (+ (length args) 1)))
		      (lambda (?arg1 ?arg ...)
			(let ((n (make-extended-number k #f))
			      (i 1))
			  (extended-number-set! n 0 ?type)
			  (extended-number-set! n 1 ?arg1)
			  (begin (set! i (+ i 1))
				 (extended-number-set! n i ?arg))
			  ...
			  n)))
		    (error "ill-formed DEFINE-EXTENDED-NUMBER-TYPE" '?type))))
	    (define (?predicate x)
	      (and (extended-number? x)
		   (eq? (extended-number-type x) ?type)))
	    (define-extended-number-accessors ?accessor ...)))))

(define-syntax define-extended-number-accessors
  (syntax-rules ()
    ((define-extended-number-accessors ?accessor)
     (define (?accessor n) (extended-number-ref n 1)))
    ((define-extended-number-accessors ?accessor1 ?accessor2)
     (begin (define (?accessor1 n) (extended-number-ref n 1))
	    (define (?accessor2 n) (extended-number-ref n 2))))
    ((define-extended-number-accessors ?accessor1 ?accessor2 ?accessor3)
     (begin (define (?accessor1 n) (extended-number-ref n 1))
	    (define (?accessor2 n) (extended-number-ref n 2))
	    (define (?accessor3 n) (extended-number-ref n 3))))))

(define-method &type-priority ((t :extended-number-type))
  (extended-number-type-priority t))

(define-method &type-predicate ((t :extended-number-type))
  (extended-number-predicate t))

; Make all the numeric instructions be extensible.

(define (make-opcode-generic! opcode mtable)
  (let ((perform (method-table-get-perform mtable)))
    (extend-opcode! opcode
		    (lambda (lose)
		      (set-final-method! mtable
					 (lambda (next-method . args)
					   (apply lose args)))
		      (lambda args
			((perform) args))))))
		      
(define-syntax define-opcode-extension
  (syntax-rules ()
    ((define-opcode-extension ?name ?table-name)
     (begin (define ?table-name (make-method-table '?name))
	    (make-opcode-generic! (enum op ?name) ?table-name)))))

(define-opcode-extension +              &+)
(define-opcode-extension -              &-)
(define-opcode-extension *              &*)
(define-opcode-extension /              &/)
(define-opcode-extension =              &=)
(define-opcode-extension <              &<)
(define-opcode-extension quotient       &quotient)
(define-opcode-extension remainder      &remainder)
  
(define-opcode-extension integer?       &integer?)
(define-opcode-extension rational?      &rational?)
(define-opcode-extension real?          &real?)
(define-opcode-extension complex?       &complex?)
(define-opcode-extension number?        &number?)
(define-opcode-extension exact?         &exact?)

(define-opcode-extension exact->inexact &exact->inexact)
(define-opcode-extension inexact->exact &inexact->exact)
(define-opcode-extension real-part      &real-part)
(define-opcode-extension imag-part      &imag-part)

(define-opcode-extension floor          &floor)
(define-opcode-extension numerator      &numerator)
(define-opcode-extension denominator    &denominator)

(define-opcode-extension make-rectangular &make-rectangular)

(define-opcode-extension exp   &exp)
(define-opcode-extension log   &log)
(define-opcode-extension sin   &sin)
(define-opcode-extension cos   &cos)
(define-opcode-extension tan   &tan)
(define-opcode-extension asin  &asin)
(define-opcode-extension acos  &acos)
(define-opcode-extension atan1 &atan1)
(define-opcode-extension atan2 &atan2)
(define-opcode-extension sqrt  &sqrt)

; >, <=, and >= are all extended using the table for <.

(let ((perform (method-table-get-perform &<)))
  (extend-opcode! (enum op >)
		  (lambda (lose)
		    (lambda (x y)
		      ((perform) (list y x)))))
  (extend-opcode! (enum op <=)
		  (lambda (lose)
		    (lambda (x y)
		      (not ((perform) (list y x))))))
  (extend-opcode! (enum op >=)
		  (lambda (lose)
		    (lambda (x y)
		      (not ((perform) (list x y)))))))
		      
; Default methods.

(define-method &integer?  (x) #f)
(define-method &rational? (x) (integer? x))
(define-method &real?     (x) (rational? x))
(define-method &complex?  (x) (real? x))
(define-method &number?   (x) (complex? x))

(define-method &real-part ((x :real)) x)

(define-method &imag-part ((x :real))
  (if (exact? x) 0 (exact->inexact 0)))

(define-method &floor ((n :integer)) n)

(define-method &numerator ((n :integer)) n)

(define-method &denominator ((n :integer))
  (if (exact? n) 1 (exact->inexact 1)))

; Make sure this has very low priority, so that it's only tried as a
; last resort.

(define-method &/ (m n)
  (if (and (integer? m) (integer? n))
      (if (= 0 (remainder m n))
	  (quotient m n)
	  (let ((z (abs (quotient n 2))))
	    (set-exactness (quotient (if (< m 0)
					 (- m z)
					 (+ m z))
				     n)
			   #f)))
      (next-method)))

(define-method &sqrt ((n :integer))
  (if (>= n 0)
      (non-negative-integer-sqrt n)	;Dubious
      (next-method)))

(define (non-negative-integer-sqrt n)
  (cond ((<= n 1)    ; for both 0 and 1
	 n)
	;; ((< n 0)
	;;  (make-rectangular 0 (integer-sqrt (- 0 n))))
	(else
	 (let loop ((m (quotient n 2)))
	   (let ((m1 (quotient n m)))
	     (cond ((< m1 m)
		    (loop (quotient (+ m m1) 2)))
		   ((= n (* m m))
		    m)
		   (else
		    (exact->inexact m))))))))

(define-simple-type :exact (:number)
  (lambda (n) (and (number? n) (exact? n))))

(define-simple-type :inexact (:number)
  (lambda (n) (and (number? n) (inexact? n))))


; Whattakludge.

; Replace the default method (which in the initial image always returns #f).

(define-method &really-string->number (s radix xact?)
  (let ((len (string-length s)))
    (cond ((<= len 1) #f)
	  ((char=? (string-ref s (- len 1)) #\i)
	   (parse-rectangular s radix xact?))
	  ((string-position #\@ s)
	   => (lambda (at)
		(let ((r (really-string->number (substring s 0 at)
						radix xact?))
		      (theta (really-string->number (substring s (+ at 1) len)
						    radix xact?)))
		  (if (and (real? r) (real? theta))
		      (make-polar r theta)))))
	  ((string-position #\/ s)
	   => (lambda (slash)
		(let ((m (string->integer (substring s 0 slash) radix))
		      (n (string->integer (substring s (+ slash 1) len)
					  radix)))
		  (if (and m n)
		      (set-exactness (/ m n) xact?)
		      #f))))
	  ((string-position #\# s)
	   (if xact?
	       #f
	       (really-string->number
		   (list->string (map (lambda (c) (if (char=? c #\#) #\5 c))
				      (string->list s)))
		   radix
		   xact?)))
	  ((string-position #\. s)
	   => (lambda (dot)
		(parse-decimal s radix xact? dot)))
	  (else #f))))

(define (parse-decimal s radix xact? dot)
  ;; Talk about kludges.  This is REALLY kludgey.
  (let* ((len (string-length s))
	 (j (if (or (char=? (string-ref s 0) #\+)
		    (char=? (string-ref s 0) #\-))
		1
		0))
	 (m (if (= dot j)
		0
		(string->integer (substring s j dot)
				 radix)))
	 (n (if (= dot (- len 1))
		0
		(string->integer (substring s (+ dot 1) len)
				 radix))))
    (if (and m n)
	(let ((n (+ m (/ n (expt radix
				 (- len (+ dot 1)))))))
	  (set-exactness (if (char=? (string-ref s 0) #\-)
			     (- 0 n)
			     n)
			 xact?))
	#f)))

(define (parse-rectangular s radix xact?)
  (let ((len (string-length s)))
    (let loop ((i (- len 2)))
      (if (< i 0)
	  #f
	  (let ((c (string-ref s i)))
	    (if (or (char=? c #\+)
		    (char=? c #\-))
		(let ((x (if (= i 0)
			     0
			     (really-string->number (substring s 0 i)
						    radix xact?)))
		      (y (if (= i (- len 2))
			     (if (char=? c #\+) 1 -1)
			     (really-string->number (substring s i (- len 1))
						    radix xact?))))
		  (if (and (real? x) (real? y))
		      (make-rectangular x y)
		      #f))
		(loop (- i 1))))))))

(define (set-exactness n xact?)
  (if (exact? n)
      (if xact? n (exact->inexact n))
      ;; ?what to do? (if xact? (inexact->exact n) n)
      n))

; Utility

(define (string-position c s)
  (let loop ((i 0))
    (if (>= i (string-length s))
	#f
	(if (char=? c (string-ref s i))
	    i
	    (loop (+ i 1))))))
