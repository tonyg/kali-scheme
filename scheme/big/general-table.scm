; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Hash table package that allows for different hash and comparison functions.
;
; The fields in a table are:
;  size - the number of entries
;  data - an a-list or vector of a-lists
;  ref  - a procedure: [table index] -> value
;  set  - a procedure: [table index new-value] -> void
;
; In small tables the data is stored in an a-list and no hashing is used.
; In large tables the data is stored in a vector of a-lists, with hashing
; used to index into the vector.  When a small table grows large the REF
; and SET fields are replaced with vector-oriented versions.

(define-record-type table :table
  (really-make-table size data ref set)
  table?
  (size table-size set-table-size!)
  (data table-data set-table-data!)
  (ref table-ref-procedure set-table-ref-procedure!)
  (set table-set!-procedure set-table-set!-procedure!))

(define (table-ref table key)
  ((table-ref-procedure table) table key))

(define (table-set! table key value)
  ((table-set!-procedure table) table key value))

; This number is a guess
(define linear-table-size-limit 15)

(define (next-table-size count)		; Figure out next good size for table.
  (+ (* count 3) 1))

; A table-maker is a thunk that returns a new, empty table each time it is
; called.  There are four functions involved:
;   assoc : [key alist] -> entry or #f
;   ref-proc : [table key] -> entry or #f
;   x->hash-table! : [assoc hash-function] -> void
;   set!-proc : [table key value] -> void
; X->HASH-TABLE! replaces the data, ref, and set fields of the table, making
; it into a hash table.

(define (make-table-maker comparison-function hash-function)
  (assoc->table-maker (make-assoc comparison-function)
		      hash-function))

(define (assoc->table-maker assoc hash-function)
  (let* ((ref-proc (make-linear-table-ref assoc))
	 (x->hash-table! (make->hash-table assoc hash-function))
	 (set!-proc (make-linear-table-set! assoc x->hash-table!)))
    (lambda ()
      (really-make-table 0 null-entry ref-proc set!-proc))))

;----------------
; A-lists.  These are currently actual a-lists, because ASSQ is a VM primitive
; and thus very fast.

(define null-entry '())  ; #f

(define (new-entry key val others)
  ;(vector key val others)
  (cons (cons key val) others))

; This abstraction is violated at times.
(define entry-value cdr)
(define entry-key car)
(define set-entry-value! set-cdr!)

; ENTRIES is known to contain ENTRY.

(define (delete-entry! entries entry)
  (if (eq? entry (car entries))
      (cdr entries)
      (begin
	(let loop ((entries entries))
	  (if (eq? entry
		   (cadr entries))
	      (set-cdr! entries (cddr entries))
	      (loop (cdr entries))))
	entries)))

(define (make-assoc pred)
  (if (eq? pred eq?)
      assq
      (lambda (thing alist)
	(let loop ((alist alist))
	  (cond ((null? alist)
		 #f)
		((pred thing (caar alist))
		 (car alist))
		(else
		 (loop (cdr alist))))))))

; Using actual a-lists allows us to use ASSQ instead of the following.

;(define eq?-assoc
;  (lambda (thing alist)
;    (let loop ((alist alist))
;      (cond ((not alist)
;             #f)
;            ((eq? thing (vector-ref alist 0))
;             alist)
;            (else
;             (loop (vector-ref alist 2)))))))

;----------------
; Turn some version of ASSOC into a table reference procedure for a-list
; tables.

(define (make-linear-table-ref assoc)
  (lambda (table key)
    (let ((probe (assoc key (table-data table))))
      (if probe (entry-value probe) #f))))

; Turn some version of ASSOC and a hash function into a table set! procedure
; for a-list tables.  If the table gets too big it is turned into a hash table.

(define (make-linear-table-set! assoc x->hash-table!)
  (lambda (table key value)
    (let* ((data (table-data table))
	   (probe (assoc key data)))
      (cond (probe
	     (if value
		 (set-entry-value! probe value)
		 (begin
		   (set-table-data! table (delete-entry! data probe))
		   (set-table-size! table (- (table-size table) 1)))))
	    (value
	     (set-table-data! table (new-entry key value data))
	     (let ((size (table-size table)))
	       (if (< size linear-table-size-limit)
		   (set-table-size! table (+ size 1))
		   (x->hash-table! table size))))))))

; Return a function to transform linear tables into hash tables.

(define (make->hash-table assoc hash-function)
  (let ((hash-table-ref (make-hash-table-ref assoc hash-function))
	(hash-table-set! (make-hash-table-set! assoc hash-function)))
    (lambda (table size)
      (let ((data (table-data table)))
	(set-table-ref-procedure! table hash-table-ref)
	(set-table-set!-procedure! table hash-table-set!)
	(table-expand-table! table (next-table-size size))
	(table-enter-alist! table data)))))

(define (make-hash-table-ref assoc hash-function)
  (lambda (table key)
    (let* ((data (table-data table))
	   (h (remainder (hash-function key)
			 (vector-length data)))
	   (alist (vector-ref data h))
	   (probe (assoc key alist)))
      (if probe (entry-value probe) #f))))
	       
(define (make-hash-table-set! assoc hash-function)
  (lambda (table key value)
    (let* ((data (table-data table))
	   (h (remainder (hash-function key)
			 (vector-length data)))
	   (alist (vector-ref data h))
	   (probe (assoc key alist)))
      (cond (probe
	     (if value
		 (set-entry-value! probe value)
		 (begin
		   (vector-set! data h (delete-entry! alist probe))
		   (set-table-size! table (- (table-size table) 1)))))
	    (value
	     (vector-set! data h (new-entry key value alist))
	     (let ((size (+ (table-size table) 1)))
	       (if (< size (vector-length data))
		   (set-table-size! table size)
		   (expand-hash-table! table size))))))))

(define (expand-hash-table! table size)
  (let ((data (table-data table)))
    (table-expand-table! table (next-table-size size))
    (do ((i 0 (+ i 1)))
	((>= i (vector-length data)))
      (table-enter-alist! table (vector-ref data i)))))

(define (table-enter-alist! table alist)
  (let ((set!-proc (table-set!-procedure table)))
    (do ((alist alist (cdr alist)))
	((null? alist))
      (set!-proc table (caar alist) (cdar alist)))))

; Reset the size and data of a table.  The size will be incremented as
; the entries are added back into the table.

(define (table-expand-table! table vector-size)
  (set-table-size! table 0)
  (set-table-data! table (make-vector vector-size null-entry)))

(define (table-walk proc table)
  (really-table-walk (lambda (v)
		       (proc (entry-key v) (entry-value v)))
		     table))
		       
(define (really-table-walk proc table)
  (let ((data (table-data table)))
    (cond ((null? data))
	  ((pair? data)
	   (alist-walk proc data))
	  (else
	   (do ((i 0 (+ i 1)))
	       ((>= i (vector-length data)))
	     (alist-walk proc (vector-ref data i)))))))

(define (alist-walk proc alist)
  (do ((alist alist (cdr alist)))
      ((null? alist))
    (proc (car alist))))

(define (make-table-immutable! table)
  (really-table-walk make-immutable! table)
  (make-immutable! (table-data table))
  (make-immutable! table))

(define (table->entry-list table)
  (let ((list '()))
    (table-walk (lambda (k v)
		  (set! list (cons v list)))
		table)
    list))

; Actual tables

; The default hash function only on works on things that would work in
; a CASE expression.  Even then, numbers don't really "work," since
; they are compared using eq?.

(define (default-table-hash-function obj)
  (cond ((symbol? obj) (string-hash (symbol->string obj)))
	((integer? obj)
	 (if (< obj 0) (- -1 obj) obj))
	((char? obj) (+ 333 (char->integer obj)))
	((eq? obj #f) 3001)
	((eq? obj #t) 3003)
	((null? obj) 3005)
	(else (error "value cannot be used as a table key" obj))))

(define eqv?-assoc (make-assoc eqv?))

(define (default-table-assoc key alist)
  (if (number? key)
      (eqv?-assoc key alist)
      (assq key alist)))

; (define string-hash (structure-ref features string-hash))

(define (symbol-hash symbol)
  (string-hash (symbol->string symbol)))

(define make-table
  (let ((make-usual-table (assoc->table-maker default-table-assoc
					      default-table-hash-function)))
    (lambda hash-function-option
      (if (null? hash-function-option)
	  (make-usual-table)
	  ((assoc->table-maker default-table-assoc
			       (car hash-function-option)))))))

(define make-string-table  (make-table-maker string=? string-hash))
(define make-symbol-table  (make-table-maker eq?      symbol-hash))
(define make-integer-table (make-table-maker =	      abs))
