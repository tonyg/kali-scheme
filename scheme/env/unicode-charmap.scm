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
				     (- (+ 1
					   *uppercase-index-width*
					   *lowercase-index-width*
					   *titlecase-index-width*
					   *general-category-bits*))))))))

(define (unicode-char-upper-case? c)
  (let ((encoding (char-info-encoding c)))
    (not
     (zero?
      (bitwise-and 1
		   (arithmetic-shift encoding
				     (- (+ *uppercase-index-width*
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

; Now replace the ASCII-only procedures by these

(set-char-map-procedures! unicode-char-alphabetic?
			  unicode-char-numeric?
			  unicode-char-whitespace?
			  unicode-char-upper-case?
			  unicode-char-lower-case?
			  unicode-char-upcase
			  unicode-char-downcase)

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
