; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

;;;; number->string and string->number

(define number->string-table (make-method-table 'number->string))
(define string->number-table (make-method-table 'string->number))

(define-default-method number->string-table
  (lambda (n radix)
    (if (exact? n)
	(if (integer? n)
	    (integer->string n radix)
	    (fail))
	(let ((s (number->string (inexact->exact n) radix)))
	  (if (integer? n)
	      (string-append s ".")
	      (string-append "#i" s))))))

(define-default-method string->number-table
  (lambda (string radix exact?)
    (string->integer string radix exact?)))

(define really-number->string (make-generic number->string-table))
(define really-string->number (make-generic string->number-table))

; Number->string

(define (number->string n . radix-or-format-option)
  (really-number->string n
			 (cond ((null? radix-or-format-option) 10)
			       ((pair? (car radix-or-format-option)) 10) ;R3RS
			       (else (car radix-or-format-option)))))

; Assumes n is exact

(define integer->string
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

    (define zero (char->integer #\0))
    (define a-minus-10 (- (char->integer #\a) 10))

    integer->string))

; String->number
; This just strips off any # prefixes and hands the rest off to
; really-string->number, which is generic.

(define (string->number string . options)
  (let* ((radix (cond ((null? options) 10)
		      ((null? (cdr options)) (car options))
		      ;; Revised^3 Scheme compatibility
		      (else (cadr options))))
	 (radix (case radix
		  ((2 b) 2) ((8 o) 8) ((10 d) 10) ((16 x) 16)
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
			    (else (loop (+ pos 1))))))))))))

(define string->integer
  (let ()

    (define (string->integer string radix xact?)
      (cond ((= (string-length string) 0) #f)
	    ((char=? (string-ref string 0) #\+)
	     (do-it string 1 1 radix xact?))
	    ((char=? (string-ref string 0) #\-)
	     (do-it string 1 -1 radix xact?))
	    (else
	     (do-it string 0 1 radix xact?))))

    (define (do-it string pos sign radix xact?)
      (let* ((len (string-length string))
	     (len (if (char=? (string-ref string (- len 1)) #\.)
		      (- len 1)    ;Allow nnnnn.
		      len)))
	(if (>= pos len)
	    #f
	    (let loop ((n 0) (pos pos))
	      (if (>= pos len)
		  (if (or xact? (not (exact? n))) n (exact->inexact n))
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
	    ((char=? c #\#) 5)  ;and warn if exact?
	    ((<= radix 10) #f)
	    (else
	     (let ((n (- (char->ascii (char-downcase c)) a-minus-ten)))
	       (if (and (>= n 10) (< n radix)) n #f)))))
    
    (define zero (char->ascii #\0))
    (define a-minus-ten (- (char->ascii #\a) 10))

    string->integer))
