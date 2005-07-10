; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

;;;; number->string and string->number

; NUMBER->STRING

(define-generic real-number->string &number->string)

(define (number->string number . maybe-radix)
  (let ((radix (if (null? maybe-radix)
		   10
		   (car maybe-radix))))
    (if (and (number? number)
	     (or (null? maybe-radix)
		 (and (null? (cdr maybe-radix))
		      (integer? radix)
		      (exact? radix)
		      (< 0 radix))))
	(real-number->string number radix)
	(apply call-error
	       "invalid argument"
	       'number->string
	       number
	       maybe-radix))))

(define-method &number->string (n radix)
  (call-error "invalid argument"
	      'number->string
	      n
	      radix))

(define-method &number->string ((n :exact-integer) radix)
  (integer->string n radix))

(define integer->string    ;Won't necessarily work if n is inexact
  (let ()

    (define (integer->string n radix)
      (let ((magnitude
	     (if (= n 0)
		 (list #\0)
		 (let recur ((n n) (l '()))
		   (if (= n 0)
		       l
		       (recur (quotient n radix)
			      (cons (integer->digit (abs (remainder n radix)))
				    l)))))))
	(list->string (if (>= n 0)
			  magnitude
			  (cons #\- magnitude)))))

    (define (integer->digit n)
      (ascii->char (+ n (if (< n 10)
			    zero
			    a-minus-10))))

    (define zero (char->ascii #\0))
    (define a-minus-10 (- (char->ascii #\a) 10))

    integer->string))


; STRING->NUMBER

; This just strips off any # prefixes and hands the rest off to
; really-string->number, which is generic.

(define (string->number string . options)
  (if (not (string? string))
      (apply call-error "invalid argument"
	     'string->number
	     string options))
  (let* ((radix (cond ((null? options) 10)
		      ((null? (cdr options)) (car options))
		      ;; Revised^3 Scheme compatibility
		      (else (cadr options))))
	 (radix (case radix
		  ((2 8 10 16) radix)
		  ((b) 2) ((o) 8) ((d) 10) ((x) 16)    ;R3RS only?
		  (else (call-error "invalid radix"
				    'string->number
				    string radix))))
	 (len (string-length string)))
    (let loop ((pos 0) (exactness? #f) (exact? #t) (radix? #f) (radix radix))
      (cond ((>= pos len)
	     #f)
	    ((char=? (string-ref string pos) #\#)
	     (let ((pos (+ pos 1)))
	       (if (>= pos len)
		   #f
		   (let ((radix-is
			  (lambda (radix)
			    (if radix?
				#f
				(loop (+ pos 1) exactness? exact? #t radix))))
			 (exactness-is
			  (lambda (exact?)
			    (if exactness?
				#f
				(loop (+ pos 1) #t exact? radix? radix)))))
		     (case (char-downcase (string-ref string pos))
		       ((#\b) (radix-is 2))
		       ((#\o) (radix-is 8))
		       ((#\d) (radix-is 10))
		       ((#\x) (radix-is 16))
		       ((#\e) (exactness-is #t))
		       ((#\i) (exactness-is #f))
		       (else #f))))))
	    (else
	     (really-string->number
	        (substring string pos len)
		radix
		(if exactness?
		    exact?
		    (let loop ((pos pos))
		      (cond ((>= pos len) #t) ;exact
			    ((char=? (string-ref string pos) #\.)
			     (if (not (= radix 10))
				 (warn "non-base-10 number has decimal point"
				       string))
			     #f)	;inexact
			    ((char=? (string-ref string pos) #\#)
			     #f)
			    ((and (= radix 10)
				  (case (char-downcase (string-ref string pos))
				    ;; One day, we have to include #\s #\f #\d #\l.
				    ;; We don't now because STRING->FLOAT actually does the
				    ;; wrong thing for these currently, so we'd rather barf.
				    ((#\e) #t)
				    (else #f)))
			     #f)
			    (else (loop (+ pos 1))))))))))))

(define-generic really-string->number &really-string->number)

(define-method &really-string->number (string radix xact?) #f)


; Read exact integers

(define-simple-type :integer-string (:string)
  (lambda (s)
    (and (string? s)
	 (let loop ((i (- (string-length s) 1)))
	   (if (< i 0)
	       #t
	       (let ((c (string-ref s i)))
		 (and (or (char-numeric? c)
			  (and (char>=? c #\a)
			       (char<=? c #\f))
			  (and (char>=? c #\A)
			       (char<=? c #\F))
			  (and (= i 0)
			       (or (char=? c #\+) (or (char=? c #\-)))))
		      (loop (- i 1)))))))))

(define-method &really-string->number ((string :integer-string) radix xact?)
  (let ((n (string->integer string radix)))
    (if n
	(set-exactness n xact?)
	(next-method)))) ; we might have something like 1e10

(define (set-exactness n xact?)
  (if (exact? n)
      (if xact? n (exact->inexact n))
      (if xact? (inexact->exact n) n)))

(define string->integer
  (let ()

    (define (string->integer string radix)
      (cond ((= (string-length string) 0) #f)
	    ((char=? (string-ref string 0) #\+)
	     (do-it string 1 1 radix))
	    ((char=? (string-ref string 0) #\-)
	     (do-it string 1 -1 radix))
	    (else
	     (do-it string 0 1 radix))))

    (define (do-it string pos sign radix)
      (let* ((len (string-length string)))
	(if (>= pos len)
	    #f
	    (let loop ((n 0) (pos pos))
	      (if (>= pos len)
		  n
		  (let ((d (digit->integer (string-ref string pos)
					   radix)))
		    (if d
			(loop (+ (* n radix) (* sign d))
			      (+ pos 1))
			#f)))))))

    (define (digit->integer c radix)
      (cond ((char-numeric? c)
	     (let ((n (- (char->ascii c) zero)))
	       (if (< n radix) n #f)))
	    ((<= radix 10) #f)
	    (else
	     (let ((n (- (char->ascii (char-downcase c)) a-minus-ten)))
	       (if (and (>= n 10) (< n radix)) n #f)))))
    
    (define zero (char->ascii #\0))
    (define a-minus-ten (- (char->ascii #\a) 10))

    string->integer))
