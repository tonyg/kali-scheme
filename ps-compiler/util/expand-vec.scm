; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


; Vectors of infinite length.  These work as do regular vectors except that
; they have no fixed size.  XVECTOR-LENGTH is one more than the highest index
; that has been passed to XVECTOR-SET!.
;
; (MAKE-XVECTOR <fill>)
; (XVECTOR-LENGTH <xvec>)
; (XVECTOR-REF <xvec> <index>)
; (XVECTOR-SET! <xvec> <index> <value>)
; (XVECTOR->VECTOR <xvec>)

; The actual record fields are:
;  default  - the fill value
;  length   - total length of internal vector(s)
;  contents - either:
;      a: vector of entries, or
;      b: vector of vectors of entries, vector has #f's after the last needed
;         sub-vector, each sub-vector has length XVEC-MAX-CONTENTS-SIZE
;  max      - maximum index used in a XVECTOR-SET!


(define-record-type expanding-vector :expanding-vector
  (make-expanding-vector default length contents max)
  expanding-vector?
  (default expanding-vector-default)
  (length expanding-vector-length set-expanding-vector-length!)
  (contents expanding-vector-contents set-expanding-vector-contents!)
  (max expanding-vector-max set-expanding-vector-max!))

(define (make-xvector default)
  (make-expanding-vector default 0 '#() -1))

; Maximum size of any internal vector

(define xvec-max-contents-size 1024)

; XVECTOR-LENGTH returns 1 + the maximum index passed to XVECTOR-SET!

(define (xvector-length xvec)
  (+ 1 (expanding-vector-max xvec)))

; If INDEX points outside current storage, the default is returned, otherwise
; the value is extracted from the one-or-two-level contents vector.

(define (xvector-ref xvec index)
  (let ((length (expanding-vector-length xvec)))
    (cond ((>= index length)
	   (expanding-vector-default xvec))
	  ((<= length xvec-max-contents-size)
	   (vector-ref (expanding-vector-contents xvec) index))
	  (else
	   (vector-ref (vector-ref (expanding-vector-contents xvec)
				   (quotient index xvec-max-contents-size))
		       (remainder index xvec-max-contents-size))))))

; Same as the above, setting instead of extracting.  INDEX is checked agains
; the maximum index, and the contents may have to be expanded.

(define (xvector-set! xvec index value)
  (let ((length (expanding-vector-length xvec)))
    (if (> index (expanding-vector-max xvec))
	(set-expanding-vector-max! xvec index))
    (cond ((>= index length)
	   (expand-expanding-vector xvec)
	   (xvector-set! xvec index value))
	  ((<= length xvec-max-contents-size)
	   (vector-set! (expanding-vector-contents xvec) index value))
	  (else
	   (vector-set! (vector-ref (expanding-vector-contents xvec)
				    (quotient index xvec-max-contents-size))
			(remainder index xvec-max-contents-size)
			value)))))

; Copy an expanding vector into a regular vector.

(define (xvector->vector xvec)
  (let ((new (make-vector (xvector-length xvec))))
    (do ((i 0 (+ i 1)))
        ((>= i (xvector-length xvec)))
      (vector-set! new i (xvector-ref xvec i)))
    new))

; Expand an expanding vector.  A new length and contents are produced and
; put in XVEC.  If LENGTH is below the maximum for a contents vector it is
; doubled in size.  If the two-level structure is being used, a new sub-
; vector is added to it.

(define (expand-expanding-vector xvec)
  (let ((length   (expanding-vector-length   xvec))
	(contents (expanding-vector-contents xvec))
	(default  (expanding-vector-default  xvec))
	(finish (lambda (length contents)
		  (set-expanding-vector-length!   xvec length)
		  (set-expanding-vector-contents! xvec contents))))
    (if (< length xvec-max-contents-size)
	(let ((new (expand-vector contents default)))
	  (finish (vector-length new) new))
	(let ((contents (cond ((= length xvec-max-contents-size)
			       (let ((new (make-vector 4 #f)))
				 (vector-set! new 0 contents)
				 new))
			      ((vector-ref contents
					   (- (vector-length contents) 1))
			       (expand-vector contents #f))
			      (else
			       contents))))
	  (vector-set! contents
		       (quotient length xvec-max-contents-size)
		       (make-vector xvec-max-contents-size default))
	  (finish (+ length xvec-max-contents-size) contents)))))

; Make a new vector twice the length of OLD and copy the contents of OLD into
; it.  DEFAULT is used to fill in the other slots.

(define (expand-vector old default)
  (let* ((length (vector-length old))
	 (new (make-vector (if (= 0 length) 4 (* 2 length)) default)))
    (do ((i 0 (+ i 1)))
	((>= i length))
      (vector-set! new i (vector-ref old i)))
    new))
	  
