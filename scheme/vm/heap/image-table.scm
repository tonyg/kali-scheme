; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A hash table for writing images.  Objects to be written out are saved in
; the table.

; This needs to be here because the Pre-Scheme compiler does not currently
; support polymorphic data types (so the table cannot be polymorphic, so
; we need to have this before we can define tables).

(define-record-type image-location :image-location
  (really-make-image-location new-descriptor next)
  (new-descriptor integer image-location-new-descriptor)
  (next           integer image-location-next set-image-location-next!))

(define (make-image-location new-descriptor)
  (really-make-image-location new-descriptor 0))

;----------------------------------------------------------------

(define-record-type table :table
  (really-make-table keys values count size)
  (keys   (^ integer)        table-keys   set-table-keys!)
  (values (^ image-location) table-values set-table-values!)
  (count  integer            table-count  set-table-count!)
  (size   integer            table-size   set-table-size!))

(define initial-table-size (shift-left 1 12))

; MAKE-VECTOR uses the initial value only to determine the type of the vector.
; It doesn't do the fill (which is really dumb).

(define (make-table)
  (let ((keys (make-vector (+ initial-table-size 1) 0)))
    (vector+length-fill! keys (+ initial-table-size 1) 0)
    (really-make-table keys
		       (make-vector initial-table-size (null-pointer))
		       0
		       initial-table-size)))

(define (deallocate-table table)
  (let ((keys (table-keys table))
	(values (table-values table)))
    (do ((i 0 (+ i 1)))
	((= i (table-size table)))
      (if (not (= (vector-ref keys i) 0))
	  (deallocate (vector-ref values i))))
    (deallocate keys)
    (deallocate values)
    (deallocate table)))

; The if we run out of memory we mark the table as unusable.  The image-writing
; code does the same.

(define (table-okay? table)
  (< 0 (table-size table)))

(define (break-table! table)
  (set-table-size! table 0))

; Assumes SIZE is a power of two.
; I have no idea how effective this hash function is.

(define (hash key size)
  (bitwise-and (bitwise-xor key
			    (bitwise-xor (shift-left key 1)
					 (arithmetic-shift-right key 10)))
	       (- size 1)))

; Double the size of the table.

(define (table-grow table)
  (let ((new-size (* (table-size table) 2))
	(old-size (table-size table))
	(old-keys (table-keys table))
	(old-values (table-values table)))
    (let ((new-keys (make-vector (+ new-size 1) 0))
	  (new-values (make-vector new-size (null-pointer))))
      (cond ((or (null-pointer? new-keys)
		 (null-pointer? new-values))
	     (if (not (null-pointer? new-keys))
		 (deallocate new-keys))
	     (if (not (null-pointer? new-values))
		 (deallocate new-values))
	     (break-table! table))
	    (else
	     (set-table-keys!   table new-keys)
	     (set-table-values! table new-values)
	     (set-table-size!   table new-size)
	     (set-table-count!  table 0)
	     (vector+length-fill! new-keys	 ; MAKE-VECTOR doesn't fill
				  (+ new-size 1)
				  0)
	     (do ((i 0 (+ i 1)))
		 ((= i old-size))
	       (let ((key (vector-ref old-keys i)))
		 (if (not (= key 0))
		     (table-insert! new-size
				    new-keys
				    new-values
				    key
				    (vector-ref old-values i)))))
	     (deallocate old-keys)
	     (deallocate old-values))))))

(define (table-insert! size keys values key value)
  (let loop ((i (hash key size)))
    (cond ((not (= (vector-ref keys i) 0))
	   (loop (+ i 1)))
	  ((= i size)
	   (loop 0))
	  (else
	   (vector-set! keys   i key)
	   (vector-set! values i value)))))

(define (table-find table key found not-found)
  (let ((start (hash key (table-size table)))
	(keys (table-keys table)))
    (let loop ((i start))
      (let ((next (vector-ref keys i)))
	(cond ((= key next)
	       (found i))
	      ((not (= next 0))
	       (loop (+ i 1)))
	      ((= i (table-size table))
	       (loop 0))
	      (else
	       (not-found i)))))))

(define (table-set! table key value)
  (if (table-okay? table)
      (table-find table
		  key
		  (lambda (i)
		    ; A. this should not happen
		    ; B. if it were to, who would delete the old value?
		    (vector-set! (table-values table) i value))
		  (lambda (i)
		    (vector-set! (table-keys   table) i key)
		    (vector-set! (table-values table) i value)
		    (set-table-count! table (+ (table-count table) 1))
		    (if (= (table-count table)
			   (quotient (table-size table) 3))
			(table-grow table))
		    (unspecific)))
      (unspecific)))

(define (table-ref table key)
  (if (table-okay? table)
      (table-find table
		  key
		  (lambda (i)
		    (vector-ref (table-values table) i))
		  (lambda (i)
		    (null-pointer)))
      (null-pointer)))


