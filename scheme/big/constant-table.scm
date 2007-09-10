; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Looking up in an eq? table that's known in advance.

; Keys can't be #f.

; Based on the idea described in Will Clinger's paper at the
; 2005 Scheme Workshop.

(define-record-type constant-table :constant-table 
  (really-make-constant-table hash-function
			      keys values
			      max-work)
  constant-table?
  (hash-function constant-table-hash-function)
  (keys constant-table-keys)
  (values constant-table-values)
  (max-work constant-table-max-work))

(define (table-bits entry-count)
  ;; upper approximation for log2(entry-count)
  (inexact->exact (round (* 2 (log (+ entry-count 1))))))

(define (shorten-vector vec size)
  (if (= size (vector-length vec))
      vec
      (do ((new (make-vector size))
	   (i 0 (+ 1 i)))
	  ((= i size) new)
	(vector-set! new i (vector-ref vec i)))))

(define (make-constant-table entries-alist hash-function)
  (let* ((entry-count (length entries-alist))
	 (bit-count (table-bits entry-count))
	 (modulus (expt 2 bit-count))
	 (size (* 2 modulus))
	 (mask (- modulus 1))
	 (keys (make-vector size #f))
	 (values (make-vector size #f))
	 (max-index 0)
	 (max-work 0))

    (define (compute-index key)
      (let loop ((index (bitwise-and (hash-function key) mask))
		 (work 0))
	(if (not (vector-ref keys index))
	    (begin
	      (set! max-work (max work max-work))
	      (set! max-index (max index max-index))
	      index)
	    (loop (+ 1 index) (+ 1 work)))))

    (for-each (lambda (p)
		(let ((key (car p))
		      (val (cdr p)))
		  (let ((index (compute-index key)))
		    (vector-set! keys index key)
		    (vector-set! values index val))))
	      entries-alist)

    (really-make-constant-table
     (lambda (key)
       (bitwise-and (hash-function key) mask))
     (shorten-vector keys (+ 1 max-index))
     (shorten-vector values (+ 1 max-index))
     max-work)))

(define (constant-table-lookup table key)
  (let ((hash ((constant-table-hash-function table) key))
	(keys (constant-table-keys table)))
    (let ((max-index (min (+ hash (constant-table-max-work table))
			  (- (vector-length keys) 1))))
      (let loop ((index hash))
	(cond
	 ((> index max-index) #f)
	 ((eq? key (vector-ref keys index))
	  (vector-ref (constant-table-values table) index))
	 (else
	  (loop (+ 1 index))))))))
