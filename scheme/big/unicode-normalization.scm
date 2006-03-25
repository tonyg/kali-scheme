; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; returns index of value (must be number) in vector
(define (binary-search vec val)
  (let ((size (vector-length vec)))
    (let loop ((low 0)			; inclusive
	       (high size))		; exclusive
      (cond
       ((< low (- high 1))
	(let* ((pos (quotient (+ low high) 2)) ; always in
	       (at (vector-ref vec pos)))
	  (cond
	   ((= val at) pos)
	   ((< val at)
	    (loop low pos))
	   (else
	    (loop pos high)))))
       ((< low high)
	(if (= val (vector-ref vec low))
	    low
	    #f))
       (else #f)))))

(define *normalization-info-block-mask*
  (- (arithmetic-shift 1 *normalization-info-block-bits*) 1))

(define (sv-normalization-info s)
  (vector-ref *normalization-info-encodings*
	      (+ (vector-ref *normalization-info-indices* 
			     (arithmetic-shift s (- *normalization-info-block-bits*)))
		 (bitwise-and s
			      *normalization-info-block-mask*))))

(define (sv-combining-class s)
  (bitwise-and (sv-normalization-info s) #xff))

(define (sv-has-canonical-decomposition? s)
  (not (zero? (bitwise-and (sv-normalization-info s) #x100))))

(define (sv-has-compatibility-decomposition? s)
  (not (zero? (bitwise-and (sv-normalization-info s) #x200))))

;; Hangul constants
;; from Unicode Standard Annex #15

(define jamo-syllable-start #xAC00)
(define jamo-initial-consonant-start #x1100)
(define jamo-initial-consonant-count 19)
(define jamo-initial-consonant-end (+ jamo-initial-consonant-start jamo-initial-consonant-count))
(define jamo-trailing-consonant-start #x11A7)
(define jamo-trailing-consonant-count 28)
(define jamo-trailing-consonant-end (+ jamo-trailing-consonant-start jamo-trailing-consonant-count))
(define jamo-vowel-start #x1161)
(define jamo-vowel-count 21)
(define jamo-vowel-end (+ jamo-vowel-start jamo-vowel-count))

;; number of syllables with a given initial consonant
(define jamo-syllable-per-count
  (* jamo-vowel-count jamo-trailing-consonant-count))
(define jamo-syllable-count
  (* jamo-initial-consonant-count jamo-syllable-per-count))
(define jamo-syllable-end (+ jamo-syllable-start jamo-syllable-count))

(define (sv-jamo-initial-consonant? sv)
  (and (>= sv jamo-initial-consonant-start)
       (< sv jamo-initial-consonant-end)))
(define (sv-jamo-trailing-consonant? sv)
  (and (>= sv jamo-trailing-consonant-start)
       (< sv jamo-trailing-consonant-end)))
(define (sv-jamo-vowel? sv)
  (and (>= sv jamo-vowel-start)
       (< sv jamo-vowel-end)))

;; assumes SV-HAS-CANONICAL-DECOMPOSITION? has returned #t
(define (sv-canonical-decomposition-encoding s)
  (vector-ref *canonical-decompositions*
	      (binary-search *canonical-decomposition-scalar-values* s)))

(define (string-nfd s)
  (decompose #f s))

(define (string-nfkd s)
  (decompose #t s))

(define (decompose compat? s)
  (let ((size (string-length s)))
    (let loop ((i 0)
	       (rev-chars '()))
      (if (>= i size)
	  (reorder-according-to-combining-class!
	   (list->string (reverse rev-chars)))
	  (let* ((c (string-ref s i))
		 (sv (char->scalar-value c)))
	    (if (sv-hangul-syllable? sv)
		(loop (+ 1 i)
		      (prepend-reverse-jamo-decomposition sv rev-chars))
		(loop (+ 1 i)
		      (append (reverse-decomposition compat? sv) rev-chars))))))))

(define (sv-hangul-syllable? sv)
  (and (>= sv  jamo-syllable-start)
       (< sv jamo-syllable-end)))

(define (prepend-reverse-jamo-decomposition sv rev-chars)
  (let* ((offset (- sv jamo-syllable-start))
	 (l (+ jamo-initial-consonant-start
	       (quotient offset jamo-syllable-per-count)))
	 (v (+ jamo-vowel-start
	       (quotient (modulo offset jamo-syllable-per-count)
			 jamo-trailing-consonant-count)))
	 (t (+ jamo-trailing-consonant-start
	       (modulo offset jamo-trailing-consonant-count)))
	 (either-way
	  (cons (scalar-value->char v)
		(cons (scalar-value->char l)
		      rev-chars))))
    (if (= t jamo-trailing-consonant-start)
	either-way
	(cons (scalar-value->char t) either-way))))

(define (reverse-decomposition compat? sv)
  (let recur ((sv sv))
    (cond
     ((and compat? (sv-has-compatibility-decomposition? sv))
      (let* ((pos (binary-search *compatibility-scalar-values* sv))
	     (end (vector-ref *compatibility-indices* (+ pos 1))))
	(let loop ((index (vector-ref *compatibility-indices* pos))
		   (rev '()))
	  (if (>= index end)
	      rev
	      (loop (+ 1 index)
		    (append (recur (vector-ref *compatibility-decompositions* index))
			    rev))))))
     ((sv-has-canonical-decomposition? sv)
      (let ((enc (sv-canonical-decomposition-encoding sv)))
	(cond
	 ;; it's either a number with one or two concatenated 16-bit numbers from
	 ;; the BMP
	 ((number? enc)
	  (let ((rest (recur (bitwise-and #xffff enc)))
		(second (bitwise-and #xffff (arithmetic-shift enc -16))))
	    (if (zero? second)
		rest
		(append (recur second) rest))))
	 ;; ... or a 1-element list or pair of scalar values
	 ((null? (cdr enc))
	  ;; 1 element
	  (recur (car enc)))
	 ;; 2 is max
	 (else
	  (append (recur (cdr enc))
		  (recur (car enc)))))))
     (else
      (list (scalar-value->char sv))))))

; bubble-sort decompositions accoring to combining class
; returns the modified string
(define (reorder-according-to-combining-class! s)
  (let ((size (string-length s)))
    (let repeat ()
      (let loop ((i 0)
		 (swapped? #f))
	(cond
	 ((< (+ i 1) size)
	  (let ((sv-i (char->scalar-value (string-ref s i)))
		(sv-i+1 (char->scalar-value (string-ref s (+ i 1)))))
	    (let ((cc-i (sv-combining-class sv-i))
		  (cc-i+1 (sv-combining-class sv-i+1)))
	      (if (and (not (zero? cc-i))
		       (not (zero? cc-i+1))
		       (< cc-i+1 cc-i))
		  (begin
		    (string-set! s i (scalar-value->char sv-i+1))
		    (string-set! s (+ i 1) (scalar-value->char sv-i))
		    (loop (+ 1 i) #t))
		  (loop (+ 1 i) swapped?)))))
	 (swapped? (repeat))
	 (else s))))))

(define (compose-2 sv-1 sv-2)
  (let ((encoding (bitwise-ior (arithmetic-shift sv-2 16)
			       sv-1)))
    (cond
     ((binary-search *composition-encodings* encoding)
      => (lambda (index)
	   (vector-ref *composition-scalar-values* index)))
     (else #f))))


(define (compose! s)
  (let ((size (string-length s)))
    (let loop ((p 0)	; output index for finished combined character
	       (p2 1)	      ; output index for uncombined characters
	       (i 0)		  ; input index for starting character
	       (j 1))	  ; input index for characters to be combined 
      (if (< i size)
	  (let* ((sv-i (char->scalar-value (string-ref s i)))
		 (cc-i (sv-combining-class sv-i)))
	    (if (zero? cc-i)
		(if (= j size)
		    (begin
		      ;; we're done combining with sv-i; skip past
		      ;; combining sequences in both input and output
		      (string-set! s p (scalar-value->char sv-i))
		      (substring s 0 (min size (max (+ p 1) p2))))
		    (let* ((sv-j (char->scalar-value (string-ref s j)))
			   (cc-j (sv-combining-class sv-j)))
		      (cond
		       ((and (= j (+ i 1))
			     (sv-jamo-initial-consonant? sv-i)
			     (sv-jamo-vowel? sv-j))
			;; need Hangul composition
			(if (and (< (+ j 1) size)
				 (sv-jamo-trailing-consonant?
				  (char->scalar-value (string-ref s (+ j 1)))))
			    ;; 3-char composition
			    (let ((composite
				   (+ jamo-syllable-start
				      (* (- sv-i jamo-initial-consonant-start)
					 jamo-syllable-per-count)
				      (* (- sv-j jamo-vowel-start)
					 jamo-trailing-consonant-count)
				      (- (char->scalar-value (string-ref s (+ j 1)))
					 jamo-trailing-consonant-start))))
			      (string-set! s i (scalar-value->char composite))
			      (loop p p2 i (+ j 2)))
			    ;; 2-char composition
			    (let ((composite
				   (+ jamo-syllable-start
				      (* (- sv-i jamo-initial-consonant-start)
					 jamo-syllable-per-count)
				      (* (- sv-j jamo-vowel-start)
					 jamo-trailing-consonant-count))))
			      (string-set! s i (scalar-value->char composite))
			      (loop p p2 i (+ j 1)))))
		       ((let ((previous-cc (sv-combining-class (char->scalar-value (string-ref s (- j 1))))))
			  ;; check if blocked
			  (and (<= previous-cc cc-j)
			       (compose-2 sv-i sv-j)))
			;; we can combine; store result temporarily at i;
			;; advance past the combining mark
			=> (lambda (combined)
			     (string-set! s i (scalar-value->char combined))
			     (loop p p2 i (+ j 1))))
		       ((zero? cc-j)
			;; both are combining class 0; we're done
			;; combining with sv-i; skip past combining sequences
			;; in both input and output
			(string-set! s p (scalar-value->char sv-i))
			(loop p2 (+ p2 1) j (+ 1 j)))
		       (else
			(let skip ((j j) (p2 p2))
			  (if (< j size)
			      (let ((sv-j (char->scalar-value (string-ref s j))))
				(if (= (sv-combining-class sv-j) cc-j)
				    (begin
				      (string-set! s p2 (scalar-value->char sv-j))
				      (skip (+ j 1) (+ p2 1)))
				    (loop p p2 i j)))
			      (loop p p2 i j)))))))
		(loop (+ p 1) (+ p2 1) (+ i 1) (+ j 1))))
	  (substring s 0 (min size p2))))))

(define (string-nfc s)
  (compose! (string-nfd s)))

(define (string-nfkc s)
  (compose! (string-nfkd s)))