; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Parse UnicodeData.txt and various other files from the Unicode
; consortium, and generate character classification and conversion
; tables from it.

(define (string-split string at)
  (let ((count (string-length string)))
    (let loop ((index 0)
	       (rev-result '()))
      (cond
       ((>= index count)
	(reverse (cons "" rev-result)))
       ((string-index string at index)
	=> (lambda (found)
	     (loop (+ 1 found)
		   (cons (substring string index found)
			 rev-result))))
       (else
	(reverse (cons (substring string index count)
		       rev-result)))))))
	  

(define (split-unicode-data-record line)
  (string-split line #\;))

(define (maybe-code-point text default)
  (if (zero? (string-length text))
      default
      (string->number text 16)))

(define-record-type code-point-info :code-point-info
  (make-code-point-info code-point
			name
			general-category
			combining-class
			bidirectional-category-id
			decomposition-id
			decimal-digit-value
			digit-value
			numeric-value
			mirrored?
			unicode-1.0-name
			iso-10646-comment
			uppercase-code-point
			lowercase-code-point
			titlecase-code-point)
  code-point-info?
  ;; number
  (code-point code-point-info-code-point)
  ;; string
  (name code-point-info-name)
  ;; :GENERAL-CATEGORY
  (general-category code-point-info-general-category)
  ;; number
  (combining-class code-point-info-combining-class)
  ;; symbol
  (bidirectional-category-id code-point-info-bidirectional-category-id)
  ;; #f or string
  (decomposition-id code-point-info-decomposition-id)
  ;; number
  (decimal-digit-value code-point-info-decimal-digit-value)
  ;; number
  (digit-value code-point-info-digit-value)
  ;; number
  (numeric-value code-point-info-numeric-value)
  ;; boolean
  (mirrored? code-point-info-mirrored?)
  ;; string
  (unicode-1.0-name code-point-info-unicode-1.0-name)
  ;; string
  (iso-10646-comment code-point-info-iso-10646-comment)
  ;; number
  (uppercase-code-point code-point-info-uppercase-code-point)
  ;; number
  (lowercase-code-point code-point-info-lowercase-code-point)
  ;; number
  (titlecase-code-point code-point-info-titlecase-code-point))

(define-record-discloser :code-point-info
  (lambda (r)
    (list 'code-point-info
	  (code-point-info-code-point r)
	  (code-point-info-name r)
	  (code-point-info-general-category r)
	  (code-point-info-combining-class r)
	  (code-point-info-bidirectional-category-id r)
	  (code-point-info-decomposition-id r)
	  (code-point-info-decimal-digit-value r)
	  (code-point-info-digit-value r)
	  (code-point-info-numeric-value r)
	  (code-point-info-mirrored? r)
	  (code-point-info-unicode-1.0-name r)
	  (code-point-info-iso-10646-comment r)
	  (code-point-info-uppercase-code-point r)
	  (code-point-info-lowercase-code-point r)
	  (code-point-info-titlecase-code-point r))))

(define (unicode-data-record->info line)
  (destructure (((code-point-hex
		  name
		  general-category-id
		  combining-class-id
		  bidirectional-category-text
		  decomposition-id
		  decimal-digit-value-text
		  digit-value-text
		  numeric-value-text
		  mirrored-y/n
		  unicode-1.0-name
		  iso-10646-comment
		  uppercase-code-point-hex
		  lowercase-code-point-hex
		  titlecase-code-point-hex)
		 (split-unicode-data-record line)))
    (let ((code-point (maybe-code-point code-point-hex #f)))
      (let ((uppercase-code-point (maybe-code-point uppercase-code-point-hex code-point))
	    (lowercase-code-point (maybe-code-point lowercase-code-point-hex code-point))
	    (titlecase-code-point (maybe-code-point titlecase-code-point-hex code-point)))
	(make-code-point-info code-point
			      name
			      (id->general-category general-category-id)
			      (string->number combining-class-id)
			      (string->symbol bidirectional-category-text)
			      (if (zero? (string-length decomposition-id))
				  #f
				  decomposition-id)
			      (string->number decimal-digit-value-text)
			      (string->number digit-value-text)
			      (string->number numeric-value-text)
			      (string=? mirrored-y/n "Y")
			      unicode-1.0-name
			      iso-10646-comment
			      uppercase-code-point
			      lowercase-code-point
			      titlecase-code-point)))))

; for EXPANDED-CODE-POINT-INFO-SOURCE
(define (code-point-info-with-code-point+name info code-point name)
  (make-code-point-info code-point
			name
			(code-point-info-general-category info)
			(code-point-info-combining-class info)
			(code-point-info-bidirectional-category-id info)
			(code-point-info-decomposition-id info)
			(code-point-info-decimal-digit-value info)
			(code-point-info-digit-value info)
			(code-point-info-numeric-value info)
			(code-point-info-mirrored? info)
			(code-point-info-unicode-1.0-name info)
			(code-point-info-iso-10646-comment info)
			code-point code-point code-point)) ; kludge

; expand the code-point ranges that look like this:
; 3400;<CJK Ideograph Extension A, First>;Lo;0;L;;;;;N;;;;;
; 4DB5;<CJK Ideograph Extension A, Last>;Lo;0;L;;;;;N;;;;;

; returns a thunk that returns the infos from consecutive calls,
; then #f

(define (expanded-code-point-info-source infos)
  (let ((first-info #f)
	(code-point #f)
	(last-code-point #f)
	(name-base #f))
    (lambda ()
      (let again ()
	(cond
	 (first-info
	  (if (<= code-point last-code-point)
	      (begin
		(set! code-point (+ 1 code-point))
		(code-point-info-with-code-point+name
		 first-info
		 (- code-point 1)
		 name-base)) ; kludge for speed; should be:
	                     ; (string-append name-base (number->string code-point 16))
	      (begin
		(set! first-info #f)
		(again))))
	 ((null? infos)
	  #f)
	 (else
	  (let* ((info (car infos))
		 (name (code-point-info-name info)))
	    (cond
	     ((and (string-prefix? "<" name)
		   (string-suffix? ", First>" name))
	      (set! first-info info)
	      (set! code-point (code-point-info-code-point info))
	      (set! last-code-point (code-point-info-code-point (cadr infos)))
	      (set! name-base (string-append
			       (substring name
					  1 ; (string-length "<")
					  (- (string-length name)
					     8 ; (string-length ", First>")
					     ))
			       "-<code point>")) ; kludge, see above
	      (set! infos (cddr infos))
	      (again))
	     (else
	      (set! infos (cdr infos))
	      info)))))))))

(define (for-each-expanded-code-point-info proc infos)
  (let ((source (expanded-code-point-info-source infos)))
    (let loop ()
      (let ((info (source)))
	(if info
	    (begin
	      (proc info)
	      (loop)))))))
	    
(define (read-line port)
  (let loop ((l '()))
    (let ((c (read-char port)))
      (if (eof-object? c)
          c
          (if (char=? c #\newline)
              (list->string (reverse l))
              (loop (cons c l)))))))

(define (parse-unicode-data filename)
  (call-with-input-file filename
    (lambda (port)
      (let loop ((rev-infos '()))
	(let ((thing (read-line port)))
	  (if (eof-object? thing)
	      (reverse rev-infos)
	      (loop (cons (unicode-data-record->info thing) rev-infos))))))))

; Mapping the relevant info (general category + case mappings) into a
; compact array

(define (mapping-offsets infos accessor)
  (let loop ((infos infos)
	     (offsets '()))
    (if (null? infos)
	(list->vector offsets)
	(let* ((info (car infos))
	       (code-point (code-point-info-code-point info))
	       (other (accessor info))
	       (offset (- other code-point)))
	  (if (member offset offsets)
	      (loop (cdr infos) offsets)
	      (loop (cdr infos) (cons offset offsets)))))))

(define (vector-index vector value)
  (let ((count (vector-length vector)))
    (let loop ((i 0))
      (cond
       ((>= i count) #f)
       ((equal? value (vector-ref vector i)) i)
       (else (loop (+ 1 i)))))))

(define (code-point-info->case+general-category-encoding
	 info
	 special-lowercase-table special-uppercase-table
	 uppercase-offsets lowercase-offsets titlecase-offsets
	 uppercase-index-width lowercase-index-width titlecase-index-width)
  (let ((code-point (code-point-info-code-point info)))
    (let ((uppercase-index (vector-index uppercase-offsets
					 (- (code-point-info-uppercase-code-point info)
					    code-point)))
	  (lowercase-index (vector-index lowercase-offsets
					 (- (code-point-info-lowercase-code-point info)
					    code-point)))
	  (titlecase-index (vector-index titlecase-offsets
					 (- (code-point-info-titlecase-code-point info)
					    code-point)))
	  (uppercase? (or (eq? (general-category uppercase-letter)
			       (code-point-info-general-category info))
			  (table-ref special-uppercase-table code-point)))
	  (lowercase? (or (eq? (general-category lowercase-letter)
			       (code-point-info-general-category info))
			  (table-ref special-lowercase-table code-point))))

      (bitwise-ior
       (arithmetic-shift
	(bitwise-ior
	 (arithmetic-shift (bitwise-ior
			    (arithmetic-shift
			     (bitwise-ior
			      (arithmetic-shift (bitwise-ior (if uppercase? 2 0)
							     (if lowercase? 1 0))
						uppercase-index-width)
			      uppercase-index)
			     lowercase-index-width)
			    lowercase-index)
			   titlecase-index-width)
	 titlecase-index)
	*general-category-bits*)
       (general-category-index (code-point-info-general-category info))))))

(define (code-point-encoding-uppercase? encoding
					uppercase-index-width lowercase-index-width titlecase-index-width)
  (not
   (zero?
    (bitwise-and 1
		 (arithmetic-shift encoding
				   (- (+ 1
					 uppercase-index-width
					 lowercase-index-width
					 titlecase-index-width
					 *general-category-bits*)))))))

(define (code-point-encoding-lowercase? encoding
					uppercase-index-width lowercase-index-width titlecase-index-width)
  (not
   (zero?
    (bitwise-and 1
		 (arithmetic-shift encoding
				   (- (+ uppercase-index-width
					 lowercase-index-width
					 titlecase-index-width
					 *general-category-bits*)))))))

(define (lookup-by-offset-index code-point offset-index offsets)
  (+ code-point (vector-ref offsets offset-index)))

(define (code-point-encoding-uppercase-code-point code-point encoding
						  uppercase-offsets
						  uppercase-index-width lowercase-index-width titlecase-index-width)
  (lookup-by-offset-index
   code-point
   (bitwise-and (- (arithmetic-shift 1 uppercase-index-width) 1)
		(arithmetic-shift encoding
				  (- (+ lowercase-index-width titlecase-index-width *general-category-bits*))))
   uppercase-offsets))

(define (code-point-encoding-lowercase-code-point code-point encoding
						  lowercase-offsets
						  uppercase-index-width lowercase-index-width titlecase-index-width)
  (lookup-by-offset-index
   code-point
   (bitwise-and (- (arithmetic-shift 1 lowercase-index-width) 1)
		(arithmetic-shift encoding
				  (- (+ titlecase-index-width *general-category-bits*))))
   lowercase-offsets))

(define (code-point-encoding-titlecase-code-point code-point encoding
						  titlecase-offsets
						  uppercase-index-width lowercase-index-width titlecase-index-width)
  (lookup-by-offset-index
   code-point
   (bitwise-and (- (arithmetic-shift 1 titlecase-index-width) 1)
		(arithmetic-shift encoding (- *general-category-bits*)))
   titlecase-offsets))

(define *code-point-encoding-general-category-mask*
  (- (arithmetic-shift 1 *general-category-bits*) 1))

(define (code-point-encoding-general-category encoding)
  (vector-ref general-categories
	      (bitwise-and encoding *code-point-encoding-general-category-mask*)))
						  

(define (max-code-point infos)
  (let loop ((max 0) (infos infos))
    (cond
     ((null? infos) max)
     ((> (code-point-info-code-point (car infos))
	 max)
      (loop (code-point-info-code-point (car infos)) (cdr infos)))
     (else (loop max (cdr infos))))))

; returns a THUNK that will return for each code-point in sequence
; (PROC <code-point>) or DEFAULT if there's no info.

; assumes INFOS are sorted

(define (make-consecutive-info-source source make-default proc)
  (let ((next-info #f)
	(last-code-point -1))
    (lambda ()

      (define (upto info)
	(if (< last-code-point (code-point-info-code-point info))
	    (begin
	      (set! next-info info)
	      (proc (make-default last-code-point)))
	    (begin
	      (set! next-info #f)
	      ;; scalar values only
	      (if (eq? (code-point-info-general-category info)
		       (general-category surrogate))
		  (proc (make-default last-code-point))
		  (proc info)))))

      (set! last-code-point (+ 1 last-code-point))

      (cond
       ((or next-info (source)) => upto)
       (else #f)))))

(define (parse-proplist-for-upper/lowercase filename)
  (call-with-input-file filename
    (lambda (port)
      (let ((uppercase (make-integer-table)) (lowercase (make-integer-table)))
	(let loop ()
	  (let ((thing (read-line port)))
	    (if (eof-object? thing)
		(values uppercase lowercase)
		(call-with-values
		    (lambda ()
		      (extract-upper/lowercase thing))
		  (lambda (uppers lowers)
		    (for-each (lambda (u)
				(table-set! uppercase u #t))
			      uppers)
		    (for-each (lambda (l)
				(table-set! lowercase l #t))
			      lowers)
		    (loop))))))))))

(define (extract-upper/lowercase line)
  (cond
   ((string-prefix? "#" line)
    (values '() '()))
   ((string-contains line "Other_Uppercase")
    (values (proplist-line-range line)
	    '()))
   ((string-contains line "Other_Lowercase")
    (values '()
	    (proplist-line-range line)))
   (else
    (values '() '()))))

(define (proplist-line-range line)
  (let* ((i1 (string-skip line char-set:hex-digit))
	 (first (string->number (substring line 0 i1) 16)))
    (if (char=? #\. (string-ref line i1))
	(let* ((i2 (string-skip line #\. i1))
	       (i3 (string-skip line char-set:hex-digit i2))
	       (last (string->number (substring line i2 i3) 16)))
	  (let loop ((last last) (range '()))
	    (if (= last first)
		(cons last range)
		(loop (- last 1) (cons last range)))))
	(list first))))

(define (make-scalar-value-case+general-category-encoding-tables
	 infos 
	 special-lowercase-table special-uppercase-table
	 block-bits)

  (let ((uppercase-offsets (mapping-offsets infos code-point-info-uppercase-code-point))
	(lowercase-offsets (mapping-offsets infos code-point-info-lowercase-code-point))
	(titlecase-offsets (mapping-offsets infos code-point-info-titlecase-code-point)))
    (let ((uppercase-index-width (bits-necessary (vector-length uppercase-offsets)))
	  (lowercase-index-width (bits-necessary (vector-length lowercase-offsets)))
	  (titlecase-index-width (bits-necessary (vector-length titlecase-offsets)))

	  (block-size (expt 2 block-bits)))

      (call-with-values
	  (lambda ()
	    (compute-compact-table
	     (make-consecutive-info-source
	      (expanded-code-point-info-source infos)
	      (lambda (code-point)
		(make-code-point-info code-point
				      "<unassigned>"
				      (general-category unassigned)
				      #f #f #f #f #f #f #f #f #f
				      code-point code-point code-point))
	      (lambda (info)
		(code-point-info->case+general-category-encoding
		 info
		 special-lowercase-table special-uppercase-table
		 uppercase-offsets lowercase-offsets titlecase-offsets
		 uppercase-index-width lowercase-index-width titlecase-index-width)))
	     block-size))
	(lambda (indices encodings)
	  (values indices encodings
		  uppercase-offsets lowercase-offsets titlecase-offsets))))))

; saves a couple of kilobyes, but probably not worthwhile

(define (write-vector-code/rll name vector port)
  (write `(define ,name (make-vector ,(vector-length vector)))
	 port)
  (newline port)
  (let loop ((values (vector->list vector))
	     (index 0))
    (cond
     ((null? values))
     ((or (null? (cdr values))
	  (not (equal? (car values) (cadr values))))
      (write `(vector-set! ,name ,index ,(car values))
	     port)
      (newline port)
      (loop (cdr values) (+ 1 index)))
     (else
      (let ((value (car values)))
	(let inner-loop ((values values)
			 (last-index index))
	  (cond
	   ((or (null? values)
	       (not (equal? (car values) value)))
	    (write
	     `(do ((i ,index (+ 1 i)))
		  ((>= i ,last-index))
		(vector-set! ,name i ,value))
	     port)
	    (newline port)
	    (loop values last-index))
	   (else
	    (inner-loop (cdr values) (+ 1 last-index))))))))))

(define (create-unicode-tables unicode-data-filename
			       proplist-filename
			       category-output-file
			       srfi-14-base-output-file)
  (let ((infos (parse-unicode-data unicode-data-filename)))
    (call-with-values
	(lambda ()
	  (parse-proplist-for-upper/lowercase proplist-filename))
      (lambda (special-uppercase-table special-lowercase-table)
	(write-unicode-category-tables infos special-uppercase-table special-lowercase-table 
				       category-output-file)
	(write-srfi-14-base-char-sets infos srfi-14-base-output-file)))))

(define (write-unicode-category-tables infos special-uppercase-table special-lowercase-table
				       output-file)
  (let ((block-bits 8))			; better than 9, at least
    (call-with-values
	(lambda ()
	  (make-scalar-value-case+general-category-encoding-tables
	   infos
	   special-uppercase-table special-lowercase-table
	   block-bits))
      (lambda (indices
	       encodings
	       uppercase-offsets lowercase-offsets titlecase-offsets)

	(call-with-output-file output-file
	  (lambda (port)
	    (display "; Automatically generated by WRITE-UNICODE-CATEGORY-TABLES; do not edit."
		     port)
	    (newline port)
	    (newline port)

	    (write `(define *encoding-table-block-bits* ,block-bits)
		   port)
	    (newline port)
	    (newline port)

	    (write `(define *uppercase-index-width*
		      ,(bits-necessary (vector-length uppercase-offsets)))
		   port)
	    (newline port)
	    (write `(define *lowercase-index-width*
		      ,(bits-necessary (vector-length lowercase-offsets)))
		   port)
	    (newline port)
	    (write `(define *titlecase-index-width*
		      ,(bits-necessary (vector-length titlecase-offsets)))
		   port)
	    (newline port)
	    (newline port)

	    (write `(define *scalar-value-info-indices* ',indices)
		   port)
	    (newline port)
	    (write `(define *scalar-value-info-encodings* ',encodings)
		   port)
	    (newline port)
	    (newline port)

	    (write `(define *uppercase-offsets* ',uppercase-offsets)
		   port)
	    (newline port)
	    (write `(define *lowercase-offsets* ',lowercase-offsets)
		   port)
	    (newline port)
	    (write `(define *titlecase-offsets* ',titlecase-offsets)
		   port)
	    (newline port)))))))

(define (write-srfi-14-base-char-sets infos output-file)
  (call-with-output-file output-file
    (lambda (port)

      (display "; Automatically generated by WRITE-SRFI-14-BASE-CHAR-SETS; do not edit."
	       port)
      (newline port)
      (newline port)

      (let-syntax
	  ((general-category-predicate
	    (syntax-rules ()
	      ((general-category-predicate ?name)
	       (lambda (info)
		 (eq? (code-point-info-general-category info)
		      (general-category ?name))))))
	   (primary-category-predicate
	    (syntax-rules ()
	      ((primary-category-predicate ?name)
	       (lambda (info)
		 (eq? (general-category-primary-category
		       (code-point-info-general-category info))
		      (primary-category ?name)))))))


	(write-srfi-14-base-char-set-definition
	 'char-set:lower-case
	 (general-category-predicate lowercase-letter)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:upper-case
	 (general-category-predicate uppercase-letter)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:title-case
	 (general-category-predicate titlecase-letter)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:letter
	 (primary-category-predicate letter)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:digit
	 (general-category-predicate decimal-digit-number)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:mark
	 (primary-category-predicate mark)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:separator
	 (primary-category-predicate separator)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:punctuation
	 (primary-category-predicate punctuation)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:symbol
	 (primary-category-predicate symbol)
	 infos port)
	(write-srfi-14-base-char-set-definition
	 'char-set:space-separator
	 (general-category-predicate space-separator)
	 infos port)))))

(define (write-srfi-14-base-char-set-definition name pred infos port)
  (write (srfi-14-base-char-set-definition name pred infos)
	 port)
  (newline port))

(define (ranges->range-vector ranges)
  (let* ((range-count (length ranges))
	 (range-vector (make-vector (* 2 (length ranges)))))

    (let loop ((i 0) (ranges ranges))
      (if (< i range-count)
	  (begin
	    (vector-set! range-vector (* 2 i) (caar ranges))
	    (vector-set! range-vector (+ 1 (* 2 i)) (cdar ranges))
	    (loop (+ 1 i) (cdr ranges)))))
    range-vector))

(define (srfi-14-base-char-set-definition name pred infos)
  (let ((accumulator (make-ranges-accumulator pred)))
    (for-each-expanded-code-point-info accumulator infos)
    `(define ,name
       (range-vector->char-set
	',(ranges->range-vector (accumulator 'ranges))))))

(define (make-ranges-accumulator pred)
  (let ((rev-ranges '())
	(current-left #f)
	(current-right #f))
    ;; assumes the characters arrive with ascending scalar values
    (lambda (message)
      (cond
       ((not (code-point-info? message))
	(if current-left
	    (reverse (cons (cons current-left current-right)
			   rev-ranges))
	    (reverse rev-ranges)))
       ((pred message)
	(let ((scalar-value (code-point-info-code-point message)))
	  (cond
	   ((not current-left)
	    (set! current-left scalar-value)
	    (set! current-right (+ 1 scalar-value)))
	   ((= scalar-value current-right)
	    (set! current-right (+ 1 current-right)))
	   (else
	    (set! rev-ranges
		  (cons (cons current-left current-right)
			rev-ranges))
	    (set! current-left scalar-value)
	    (set! current-right (+ 1 scalar-value))))))))))

; for debugging

(define (test-code-point-case+general-category-encoding-tables
	 infos special-uppercase-table special-lowercase-table
	 block-bits
	 indices encodings
	 uppercase-offsets lowercase-offsets titlecase-offsets)

  (let ((lower-mask (- (arithmetic-shift 1 block-bits) 1))
	(uppercase-index-width (bits-necessary (vector-length uppercase-offsets)))
	(lowercase-index-width (bits-necessary (vector-length lowercase-offsets)))
	(titlecase-index-width (bits-necessary (vector-length titlecase-offsets))))

    (for-each-expanded-code-point-info
     (lambda (info)
       (let* ((code-point (code-point-info-code-point info))
	      (base-index (vector-ref indices
				      (arithmetic-shift code-point (- block-bits))))
	      (index (+ base-index (bitwise-and code-point lower-mask)))
	      (encoding (vector-ref encodings index)))

	 (if (not (eq? (code-point-info-general-category info)
		       (general-category surrogate)))
	     (begin

	       (if (not (eq? (code-point-info-general-category info)
			     (code-point-encoding-general-category encoding)))
		   (error "general category mismatch"
			  info
			  (code-point-encoding-general-category encoding)))

	       (let ((uppercase-code-point
		      (code-point-encoding-uppercase-code-point
		       code-point encoding
		       uppercase-offsets
		       uppercase-index-width lowercase-index-width titlecase-index-width))
		     (lowercase-code-point
		      (code-point-encoding-lowercase-code-point
		       code-point encoding
		       lowercase-offsets
		       uppercase-index-width lowercase-index-width titlecase-index-width))
		     (titlecase-code-point
		      (code-point-encoding-titlecase-code-point
		       code-point encoding
		       titlecase-offsets
		       uppercase-index-width lowercase-index-width titlecase-index-width))
		     (uppercase?
		      (code-point-encoding-uppercase?
		       encoding
		       uppercase-index-width lowercase-index-width titlecase-index-width))
		     (lowercase?
		      (code-point-encoding-lowercase?
		       encoding
		       uppercase-index-width lowercase-index-width titlecase-index-width)))

		 (if (not (= (code-point-info-uppercase-code-point info)
			     uppercase-code-point))
		     (error "uppercase mismatch" info uppercase-code-point))

		 (if (not (= (code-point-info-lowercase-code-point info)
			     lowercase-code-point))
		     (error "lowercase mismatch" info lowercase-code-point))
	   
		 (if (not (= (code-point-info-titlecase-code-point info)
			     titlecase-code-point))
		     (error "titlecase mismatch" info titlecase-code-point))

		 (if (not (eq? (or (table-ref special-uppercase-table code-point)
				   (eq? (code-point-info-general-category info)
					(general-category uppercase-letter)))
			       uppercase?))
		     (error "uppercase? mismatch" info titlecase-code-point))
		 (if (not (eq? (or (table-ref special-lowercase-table code-point)
				   (eq? (code-point-info-general-category info)
					(general-category lowercase-letter)))
			       lowercase?))
		     (error "lowercase? mismatch" info titlecase-code-point))
		 )))))
     infos)))

(define (find-code-point-info code-point infos)
  (find (lambda (info)
	  (= code-point (code-point-info-code-point info)))
	infos))
