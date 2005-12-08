; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Unicode version of env/rts/charmap.scm, replaces relevant procedures there.

(define *encoding-table-block-mask*
  (- (arithmetic-shift 1 *encoding-table-block-bits*) 1))

; access the compact table
(define (char-info-encoding c)
  (let ((scalar-value (char->scalar-value c)))
    (vector-ref *scalar-value-info-encodings*
		(+ (vector-ref *scalar-value-info-indices*
			       (arithmetic-shift scalar-value (- *encoding-table-block-bits*)))
		   (bitwise-and scalar-value
				*encoding-table-block-mask*)))))
    

(define *scalar-value-encoding-general-category-mask*
  (- (arithmetic-shift 1 *general-category-bits*) 1))

(define (char-general-category c)
  (vector-ref general-categories
	      (bitwise-and (char-info-encoding c)
			   *scalar-value-encoding-general-category-mask*)))

(define (unicode-char-alphabetic? c)
  (eq? (primary-category letter)
       (general-category-primary-category (char-general-category c))))

(define (unicode-char-numeric? c)
  (eq? (primary-category number)
       (general-category-primary-category (char-general-category c))))

(define (unicode-char-whitespace? c)
  (or (eq? (general-category space-separator)
	   (char-general-category c))
      (let ((scalar-value (char->scalar-value c)))
	;; space, horizontal tab, line feed (= newline), vertical tab,
	;; form feed, and carriage return
	(and (>= scalar-value 9)
	     (<= scalar-value 13)))))

(define (unicode-char-lower-case? c)
  (let ((encoding (char-info-encoding c)))
    (not
     (zero?
      (bitwise-and 1
		   (arithmetic-shift encoding
				     (- (+ *uppercase-index-width*
					   *lowercase-index-width*
					   *titlecase-index-width*
					   *general-category-bits*))))))))

(define (unicode-char-upper-case? c)
  (let ((encoding (char-info-encoding c)))
    (not
     (zero?
      (bitwise-and 1
		   (arithmetic-shift encoding
				     (- (+ 1
					   *uppercase-index-width*
					   *lowercase-index-width*
					   *titlecase-index-width*
					   *general-category-bits*))))))))

(define (lookup-by-offset-index scalar-value offset-index offsets)
  (scalar-value->char
   (+ scalar-value (vector-ref offsets offset-index))))

(define *uppercase-mask* (- (arithmetic-shift 1 *uppercase-index-width*) 1))

(define (unicode-char-upcase c)
  (let ((scalar-value (char->scalar-value c))
	(encoding (char-info-encoding c)))
    (lookup-by-offset-index
     scalar-value
     (bitwise-and *uppercase-mask*
		  (arithmetic-shift encoding
				    (- (+ *lowercase-index-width* 
					  *titlecase-index-width*
					  *general-category-bits*))))
     *uppercase-offsets*)))

(define *lowercase-mask* (- (arithmetic-shift 1 *lowercase-index-width*) 1))

(define (unicode-char-downcase c)
  (let ((scalar-value (char->scalar-value c))
	(encoding (char-info-encoding c)))
  (lookup-by-offset-index
   scalar-value
   (bitwise-and *lowercase-mask*
		(arithmetic-shift encoding
				  (- (+ *titlecase-index-width* *general-category-bits*))))
   *lowercase-offsets*)))

(define (char-foldcase c)
  (char-downcase (char-upcase c)))

; Now replace the ASCII-only procedures by these

(set-char-map-procedures! unicode-char-alphabetic?
			  unicode-char-numeric?
			  unicode-char-whitespace?
			  unicode-char-upper-case?
			  unicode-char-lower-case?
			  unicode-char-upcase
			  unicode-char-downcase
			  char-foldcase)

; Unicode bonus material

(define (char-title-case? c)
  (eq? (general-category titlecase-letter)
       (char-general-category c)))

(define *titlecase-mask* (- (arithmetic-shift 1 *titlecase-index-width*) 1))

(define (char-titlecase c)
  (let ((scalar-value (char->scalar-value c))
	(encoding (char-info-encoding c)))
    (lookup-by-offset-index
     scalar-value
     (bitwise-and *titlecase-mask*
		  (arithmetic-shift encoding (- *general-category-bits*)))
     *titlecase-offsets*)))

; check if the mapping in UnicodeDate.txt is not authoritative, and we
; should use the one in SpecialCasing.txt
(define (unicode-char-specialcasing? c)
  (let ((encoding (char-info-encoding c)))
    (not
     (zero?
      (bitwise-and 1
		   (arithmetic-shift encoding
				     (- (+ 2
					   *uppercase-index-width*
					   *lowercase-index-width*
					   *titlecase-index-width*
					   *general-category-bits*))))))))

(define (prepend-specialcasing-reverse start length c r)
  (let loop ((j 0)
	     (r r))
    (if (>= j length)
	r
	(loop (+ j 1)
	      (cons (string-ref *specialcasings* (+ start j))
		    r)))))

(define (string-xcase char-xcase prepend-specialcasing-xcase/reverse
		      s)
  (let ((size (string-length s)))
    (let loop ((i 0) (r '()))
      (if (>= i size)
	  (list->string (reverse r))
	  (let ((c (string-ref s i)))
	    (loop (+ 1 i)
		  (if (unicode-char-specialcasing? c)
		      (prepend-specialcasing-xcase/reverse c r s i size)
		      (cons (char-xcase c) r))))))))

(define (prepend-specialcasing-upcase/reverse c r s i size)
  (let ((specialcasing
	 (table-ref *specialcasing-table* (char->scalar-value c))))
    (prepend-specialcasing-reverse (specialcasing-uppercase-start specialcasing)
				   (specialcasing-uppercase-length specialcasing)
				   c r)))

(define (string-upcase s)
  (string-xcase char-upcase prepend-specialcasing-upcase/reverse s))

(define (prepend-specialcasing-downcase/reverse c r s i size)
  (let ((specialcasing
	 (table-ref *specialcasing-table* (char->scalar-value c))))
    (if (and (specialcasing-final-sigma? specialcasing)
	     (or (and (< (+ 1 i) size) ; a letter follows
		      (unicode-char-alphabetic? (string-ref s (+ 1 i))))
		 (or (zero? i) ; it's the only letter in the word
		     (not (unicode-char-alphabetic? (string-ref s (- i 1)))))))
	(cons (char-downcase c) r)
	(prepend-specialcasing-reverse (specialcasing-lowercase-start specialcasing)
				       (specialcasing-lowercase-length specialcasing)
				       c r))))

(define (string-downcase s)
  (string-xcase char-downcase prepend-specialcasing-downcase/reverse s))

(define (string-foldcase s)
  ;; map to uppercase, then back to lowercase char-by-char
  (let* ((ucase (string-upcase s))
	 (size (string-length ucase)))
    (let loop ((i 0))
      (if (< i size)
	  (begin
	    (string-set! ucase i (char-downcase (string-ref ucase i)))
	    (loop (+ 1 i))))
      ucase)))

(define (string-ci-comparator cs-comp)
  (lambda (a-string b-string)
    (cs-comp (string-foldcase a-string) (string-foldcase b-string))))

(define string-ci=? (string-ci-comparator string=?))
(define string-ci<? (string-ci-comparator string<?))

(set-string-ci-procedures! string-ci=? string-ci<?)

; Titlecase

(define (char-cased? c)
  (or (char-lower-case? c)
      (char-upper-case? c)
      (char-title-case? c)))

(define u+00ad (scalar-value->char #x00ad)) ; SOFT HYPHEN (SHY)
(define u+2019 (scalar-value->char #x2019)) ; RIGHT SINGLE QUOTATION MARK

(define (char-case-ignorable? c)
  ;; Mike suspects this list is not complete
  (or (char=? c #\')
      (char=? c u+00ad)
      (char=? c u+2019)
      (let ((cat (char-general-category c)))
	(or (eq? cat (general-category non-spacing-mark))
	    (eq? cat (general-category enclosing-mark))
	    (eq? cat (general-category formatting-character))
	    (eq? cat (general-category modifier-symbol))))))

(define (string-titlecase s)
  (let ((size (string-length s)))
    (let loop ((i 0) (r '()))
      ;; looking for a letter
      (if (>= i size)
	  (list->string (reverse r))
	  (let ((c (string-ref s i)))
	    (if (char-cased? c)
		(let casing-loop
		    ((j (+ 1 i))
		     (r (if (unicode-char-specialcasing? c)
			    (let ((specialcasing
				   (table-ref *specialcasing-table* (char->scalar-value c))))
			      (prepend-specialcasing-reverse
			       (specialcasing-titlecase-start specialcasing)
			       (specialcasing-titlecase-length specialcasing)
			       c r))
			    (cons (char-titlecase c) r))))
		  (if (>= j size)
		      (list->string (reverse r))
		      (let ((c (string-ref s j)))
			(cond
			 ((char-case-ignorable? c)
			  (casing-loop (+ j 1) (cons c r)))
			 ((char-cased? c)
			  (casing-loop (+ j 1)
				       (if (unicode-char-specialcasing? c)
					   (prepend-specialcasing-downcase/reverse c r s j size)
					   (cons (char-downcase c) r))))
			 (else
			  (loop j r))))))
		(loop (+ 1 i) (cons c r))))))))

