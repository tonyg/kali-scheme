; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.
; copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; (make-array <initial-value> <bound1> ...)
; (array-shape <array>)
; (array-ref <array> <index1> ...)
; (array-set! <array> <value> <index1> ...)
; (make-shared-array <array> <linear-map> <bound1> ...)
; (copy-array <array>)
; (array->vector <array>)
; (array <bounds> . <elements>)
;
; All arrays are zero based.
;
; The <linear-map> argument to MAKE-SHARED-ARRAY is a linear function
; that maps indices into the shared array into a list of indices into
; the original array.  The array returned by MAKE-SHARED-ARRAY shares
; storage with the original array.
;
; (array-ref (make-shared-array a f i1 i2 ... iN) j1 j2 ... jM)
;   <==>
; (apply array-ref a (f j1 j2 ... jM))
;
; ARRAY->VECTOR returns a vector containing the elements of an array
; in row-major order.

; An array consists of a vector containing the bounds of the array,
; a vector containing the elements of the array, and a linear map
; expressed as a vector of coefficients and one constant.
; If the map is #(c1 c2 ... cN C0) then the index into the vector of
; elements for (array-ref a i1 i2 ... iN) is
; (+ (* i1 c1) (* i2 c2) ... (* iN cN) C0).

; Interface due to Alan Bawden (except for requiring zero-based arrays)
; Implementation by Richard Kelsey.

(define-record-type array :array
  (really-make-array bounds map elements)
  array?
  (bounds   array-bounds)       ; vector of array bounds
  (map      array-map)          ; vector of coefficients + one constant
  (elements array-elements))    ; vector of actual elements

(define-record-discloser :array
  (lambda (array)
    (cons 'array (array-shape array))))

(define (array-shape array)
  (vector->list (array-bounds array)))

; Calculate the index into an array's element vector that corresponds to
; INDICES.  MAP is the array's linear map.

(define (fast-array-index indices map)
  (let ((size (- (vector-length map) 1)))
    (do ((i 0 (+ i 1))
	 (j (vector-ref map size)
	    (+ j (* (vector-ref indices i)
		    (vector-ref map i)))))
	((>= i size) j))))

; The same thing with bounds checking added.

(define (array-index array indices)
  (let ((bounds       (array-bounds array))
	(coefficients (array-map array)))
    (let loop ((is indices)
	       (i 0)
	       (index (vector-ref coefficients (vector-length bounds))))
      (cond ((null? is)
	     (if (= i (vector-length bounds))
		 index
		 (error "wrong number of array indices" array indices)))
	    ((>= i (vector-length bounds))
	     (error "wrong number of array indices" array indices))
	    (else
	     (let ((j (car is)))
	       (if (and (>= j 0)
			(< j (vector-ref bounds i)))
		   (loop (cdr is)
			 (+ i 1)
			 (+ index (* j (vector-ref coefficients i))))
		   (error "array index out of range" array indices))))))))

(define (array-ref array . indices)
  (vector-ref (array-elements array) (array-index array indices)))

(define (array-set! array value . indices)
  (vector-set! (array-elements array) (array-index array indices) value))

; This is mostly error checking.

(define (make-array initial bound1 . bounds)
  (let* ((all-bounds (cons bound1 bounds))
	 (bounds (make-vector (length all-bounds)))
	 (size (do ((bs all-bounds (cdr bs))
		    (i 0 (+ i 1))
		    (s 1 (* s (car bs))))
		   ((null? bs) s)
		 (let ((b (car bs)))
		   (vector-set! bounds i b)
		   (if (not (and (integer? b)
				 (exact? b)
				 (< 0 b)))
		       (error "illegal array bounds" all-bounds))))))
    (really-make-array bounds
		       (bounds->map bounds)
		       (make-vector size initial))))

(define (array bounds . elts)
  (let* ((array (apply make-array #f bounds))
	 (elements (array-elements array))
	 (size (vector-length elements)))
    (if (not (= (length elts) size))
	(error "ARRAY got the wrong number of elements" bounds elts))
    (do ((i 0 (+ i 1))
	 (elts elts (cdr elts)))
	((null? elts))
      (vector-set! elements i (car elts)))
    array))

; Determine the linear map that corresponds to a simple array with the
; given bounds.

(define (bounds->map bounds)
  (do ((i (- (vector-length bounds) 1) (- i 1))
       (s 1 (* s (vector-ref bounds i)))
       (l '() (cons s l)))
      ((< i 0)
       (list->vector (reverse (cons 0 (reverse l)))))))

; This is mostly error checking.  Two different procedures are used to
; check that the shared array does not extend past the original.  The
; full check does a complete check, but, because it must check every corner
; of the shared array, it gets very slow as the number of dimensions
; goes up.  The simple check just verifies that the all elements of
; the shared array map to elements in the vector of the original.

(define (make-shared-array array linear-map . bounds)
  (let ((map (make-shared-array-map array linear-map bounds)))
    (if (if (<= (length bounds) maximum-full-bounds-check)
	    (full-array-bounds-okay? linear-map bounds (array-bounds array))
	    (simple-array-bounds-okay? map bounds (vector-length
						   (array-elements array))))
	(really-make-array (list->vector bounds)
			   map
			   (array-elements array))
	(error "shared array out of bounds" array linear-map bounds))))

(define maximum-full-bounds-check 5)

; Check that every corner of the array specified by LINEAR and NEW-BOUNDS
; is within OLD-BOUNDS.  This checks every corner of the new array.

(define (full-array-bounds-okay? linear new-bounds old-bounds)
  (let ((old-bounds (vector->list old-bounds)))
    (let label ((bounds (reverse new-bounds)) (args '()))
      (if (null? bounds)
	  (let loop ((res (apply linear args)) (bounds old-bounds))
	    (cond ((null? res)
		   (null? bounds))
		  ((and (not (null? bounds))
			(<= 0 (car res))
			(< (car res) (car bounds)))
		   (loop (cdr res) (cdr bounds)))
		  (else #f)))
	  (and (label (cdr bounds) (cons 0 args))
	       (label (cdr bounds) (cons (- (car bounds) 1) args)))))))

; Check that the maximum and minimum possible vector indices possible with
; the given bounds and map would fit in an array of the given size.

(define (simple-array-bounds-okay? map bounds size)
  (do ((map (vector->list map) (cdr map))
       (bounds bounds (cdr bounds))
       (min 0 (if (> 0 (car map))
		  (+ min (* (car map) (- (car bounds) 1)))
		  min))
       (max 0 (if (< 0 (car map))
		  (+ max (* (car map) (- (car bounds) 1)))
		  max)))
      ((null? bounds)
       (and (>= 0 (+ min (car map)))
	    (< size (+ max (car map)))))))

; Determine the coefficients and constant of the composition of
; LINEAR-MAP and the linear map of ARRAY.  BOUNDS is used only to
; determine the rank of LINEAR-MAP's domain.
;
; The coefficients are determined by applying first LINEAR-MAP and then
; ARRAY's map to the vectors (1 0 0 ... 0), (0 1 0 ... 0), ..., (0 ... 0 1).
; Applying them to (0 ... 0) gives the constant of the composition.

(define (make-shared-array-map array linear-map bounds)
  (let* ((zero (map (lambda (ignore) 0) bounds))
	 (do-vector (lambda (v)
		      (or (apply-map array (apply linear-map v))
			  (error "bad linear map for shared array"
				 linear-map array bounds))))
	 (base (do-vector zero)))
    (let loop ((bs bounds) (ces '()) (unit (cons 1 (cdr zero))))
      (if (null? bs)
	  (list->vector (reverse (cons base ces)))
	  (loop (cdr bs)
		(cons (- (do-vector unit) base) ces)
		(rotate unit))))))

; Apply ARRAY's linear map to the indices in the list VALUES and
; return the resulting vector index.  #F is returned if VALUES is not
; the correct length or if any of its elements are out of range.

(define (apply-map array values)
  (let ((map (array-map array))
	(bounds (array-bounds array)))
    (let loop ((values values)
	       (i 0)
	       (index (vector-ref map (vector-length bounds))))
      (cond ((null? values)
	     (if (= i (vector-length bounds))
		 index
		 #f))
	    ((>= i (vector-length bounds))
	     #f)
	    (else
	     (let ((j (car values)))
	       (if (and (>= j 0)
			(< j (vector-ref bounds i)))
		   (loop (cdr values)
			 (+ i 1)
			 (+ index (* j (vector-ref map i))))
		   #f)))))))

; Return LIST with its last element moved to the front.

(define (rotate list)
  (let ((l (reverse list)))
    (cons (car l) (reverse (cdr l)))))

; Copy an array, shrinking the vector if this is subarray that does not
; use all of the original array's elements.

(define (copy-array array)
  (really-make-array (array-bounds array)
		     (bounds->map (array-bounds array))
		     (array->vector array)))

; Make a new vector and copy the elements into it.  If ARRAY's map is
; the simple map for its bounds, then the elements are already in the
; appropriate order and we can just copy the element vector.

(define (array->vector array)
  (let* ((size (array-element-count array))
	 (new (make-vector size)))
    (if (and (= size (vector-length (array-elements array)))
	     (equal? (array-map array) (bounds->map (array-bounds array))))
	(copy-vector (array-elements array) new)
	(copy-elements array new))
    new))

(define (array-element-count array)
  (let ((bounds (array-bounds array)))
    (do ((i 0 (+ i 1))
	 (s 1 (* s (vector-ref bounds i))))
	((>= i (vector-length bounds))
	 s))))
  
(define (copy-vector from to)
  (do ((i (- (vector-length to) 1) (- i 1)))
      ((< i 0))
    (vector-set! to i (vector-ref from i))))

; Copy the elements of ARRAY into the vector TO.  The copying is done one
; row at a time.  POSN is a vector containing the index of the row that
; we are currently copying.  After the row is copied, POSN is updated so
; that the next row can be copied.  A little more cleverness would make
; this faster by replacing the call to FAST-ARRAY-INDEX with some simple
; arithmetic on J.

(define (copy-elements array to)
  (let ((bounds   (array-bounds array))
	(elements (array-elements array))
	(map      (array-map array)))
    (let* ((size (vector-length bounds))
	   (posn (make-vector size 0))
	   (step-size (vector-ref bounds (- size 1)))
	   (delta (vector-ref map (- size 1))))
      (let loop ((i 0))
	(do ((i2 i (+ i2 1))
	     (j (fast-array-index posn map) (+ j delta)))
	    ((>= i2 (+ i step-size)))
	  (vector-set! to i2 (vector-ref elements j)))
	(cond ((< (+ i step-size) (vector-length to))
	       (let loop2 ((k (- size 2)))
		 (cond ((= (+ (vector-ref posn k) 1) (vector-ref bounds k))
			(vector-set! posn k 0)
			(loop2 (- k 1)))
		       (else
			(vector-set! posn k (+ 1 (vector-ref posn k))))))
	       (loop (+ i step-size))))))))

; Testing.
; (define a1 (make-array 0 4 5))
;  0  1  2  3
;  4  5  6  7
;  8  9 10 11
; 12 13 14 15
; 16 17 18 19
; (make-shared-array-map a1 (lambda (x) (list x x)) '(3))
; 0 5 10, #(5 0)
; (make-shared-array-map a1 (lambda (x) (list 2 (- 4 x))) '(3))
; 18 14 10 #(-4 18)
; (make-shared-array-map a1 (lambda (x y) (list (+ x 1) y)) '(2 4))
;  1  2
;  5  6
;  9 10
; 13 14
; #(1 4 1)

