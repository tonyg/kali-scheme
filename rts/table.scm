; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file table.scm.

; Implements the table procedures make-table, table-ref, and table-set!. 
; John D. Ramsdell --- December 1989.
; [With major modification by JAR over the years.]

; Copyright 1991 by The MITRE Corporation.
; Permission to use, copy, modify, and distribute this software and
; its documentation for any purpose and without fee is hereby granted,
; provided that the above copyright notice appear in all copies.  The
; MITRE Corporation makes no representations about the suitability of
; this software for any purpose.  It is provided "as is" without
; expressed or implied warranty.

; A table is a record containing the data as well as information used to
; interpret the data.  Procedure make-table creates an empty table in
; which the data is kept as an alist and searched using assq.  When the
; table grows large enough, the table is reorganized into a hash table.
; The data is kept in a vector of alists.  The hash function selects the
; alist and then assq is used to continue the search.  When a hash table
; has more entries than the size of the vector used to store the data,
; the table is reorganized by increasing the size of the vector by a
; constant factor.  There is no means for shrinking a table.

(define-record-type table table-type
  (really-make-table hash-function size data ref set)
  table?
  (hash-function table-hash-function)
  (size table-size set-table-size!)
  (data table-data set-table-data!)
  (ref table-ref-procedure set-table-ref-procedure!)
  (set table-set-procedure set-table-set-procedure!))

; make-table creates an empty table initialized for linear search.
(define (make-table . maybe-hash-function)
  (let ((hash (if (null? maybe-hash-function)
		  default-table-hash-function
		  (car maybe-hash-function))))
    (really-make-table hash
		       0		; Number of entries
		       null-entry	; Data
		       linear-table-ref	; Reference procedure
		       linear-table-set! ; Update procedure
		       )))

(define (table-ref table key)
  ((table-ref-procedure table) table key))

(define (table-set! table key val)
  ((table-set-procedure table) table key val))


; Parameters.
(define linear-table-size-limit 15)	; Table size which introduces hashing.
(define table-size-limit 100000)	; Print error when size exceeds this.

(define (next-table-size count)		; Figure out next good size for table.
  (let ((new-size (+ (* count 3) 1)))
    (if (>= new-size table-size-limit)
	(error "requested table size is too large" new-size))
    new-size))


; Speed & size hack?!  See how well it works out, then revert to
; a-lists if it doesn't.

(define null-entry #f)

(define (new-entry key val others)	;(cons (cons key val) others)
  (let ((v (make-vector 3 #f)))
    (vector-set! v 0 key)
    (vector-set! v 1 val)
    (vector-set! v 2 others)
    v))

(define (get-entry entries key)		; (let ((probe (assq key entries))))
  (if (vector? entries)			;   (if probe (cdr probe) #f)
      (if (eq? key (vector-ref entries 0))
	  (vector-ref entries 1)
	  (get-entry (vector-ref entries 2) key))
      #f))

; Returns #t if there is *no* entry already.

(define (set-entry! entries key val)
  (if (vector? entries)
      (if (eq? key (vector-ref entries 0))
	  (begin (vector-set! entries 1 val)
		 #f)
	  (set-entry! (vector-ref entries 2) key val))
      #t))

(define (for-each-entry proc entries)
  (if (vector? entries)
      (let ((data (vector-ref entries 1)))
	(if data (proc (vector-ref entries 0) data))
	(for-each-entry proc (vector-ref entries 2)))))


; Linear tables

(define (linear-table-ref table key)
  (get-entry (table-data table) key))

(define (linear-table-set! table key val)
  (let ((entries (table-data table)))
    (if (set-entry! entries key val)
	(begin (set-table-data! table (new-entry key val entries))
	       (linear-table-inc! table)))))

(define (linear-table-inc! table)
  (let ((count (+ (table-size table) 1)))
    (if (< count linear-table-size-limit)
	(set-table-size! table count)	; Update table count.
	(let ((data (table-data table)))
	  ;; Transform table into one using hashing.
	  (set-table-ref-procedure! table hash-table-ref)
	  (set-table-set-procedure! table hash-table-set!)
	  (table-expand-table! table (next-table-size count))
	  (table-enter-entries! table data)))))

(define (table-expand-table! table size) ; Expand hash vector and
  (set-table-size! table 0)		; zero data count.
  (set-table-data! table (make-vector size null-entry)))

(define (table-enter-entries! table entries)
  (for-each-entry (lambda (key val)
		    (hash-table-set! table key val))
		  entries))

; Hash tables.

(define (hash-table-ref table key)
  (let ((data (table-data table)))
    (get-entry (vector-ref data
			   (remainder ((table-hash-function table) key)
				      (vector-length data)))
	       key)))

(define (hash-table-set! table key val)
  (let* ((data (table-data table))
	 (h (remainder ((table-hash-function table) key)
		       (vector-length data)))
	 (entries (vector-ref data h)))
    (if (set-entry! entries key val)
	(begin (vector-set! data h
			    (new-entry key val entries))
	       (hash-table-inc! table)))))

(define (hash-table-inc! table)
  (let* ((count (+ (table-size table) 1))
	 (data (table-data table))
	 (size (vector-length data)))
    (if (< count size)
	(set-table-size! table count)	; Update table count.
	(begin		
	  ;; Make bigger hash vector.
	  (table-expand-table! table (next-table-size count))
	  (do ((i 0 (+ i 1)))
	      ((>= i size))		; Reenter data.
	    (table-enter-entries! table (vector-ref data i)))))))

; table-walk - do (proc key value) for each entry in table

(define (table-walk proc table)
  (let ((data (table-data table)))
    (if (eq? (table-ref-procedure table) linear-table-ref)
	(for-each-entry proc data)
	(let ((size (vector-length data)))
	  (do ((i 0 (+ i 1)))
	      ((>= i size) #t)
	    (for-each-entry proc (vector-ref data i)))))))

; The default hash function only on works on things that would work in
; a CASE expression.  Even then, numbers don't really "work," since
; they are compared using eq?.

(define (default-table-hash-function obj)
  (cond ((symbol? obj) (string-hash (symbol->string obj)))
	((integer? obj) (abs obj))
	((char? obj) (+ 333 (char->integer obj)))
	((eq? obj #f) 3001)
	((eq? obj #t) 3003)
	((null? obj) 3005)
	(else (error "value cannot be used as a table key" obj))))
