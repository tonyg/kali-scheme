; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; String hash tables for managing three tables:
;  the symbol table : string -> symbol
;  exported-bindings : string -> shared-binding
;  imported-bindings : string -> shared-binding

; Size of the hash vectors (is this a reasonable size?).

(define hash-table-slots 1024)

(define hash-table-size (vm-vector-size hash-table-slots))

; All hash table values are required to have their own link fields.

(define hash-table-entry-size 0)

(define (hash-table-index string)
  (bitwise-and (vm-string-hash string)
	       (- hash-table-slots 1)))

; All buckets are initially false.

(define (make-hash-table key)
  (let ((table (vm-make-vector hash-table-slots key)))
    (natural-for-each (lambda (index)
			(vm-vector-set! table index false))
		      hash-table-slots)
    table))

; Return a procedure for adding FOO's to tables.

(define (table-adder foo-string set-foo-next!)
  (lambda (table foo)
    (let ((index (hash-table-index (foo-string foo))))
      (set-foo-next! foo (vm-vector-ref table index))
      (vm-vector-set! table index foo))))

; Return a function for looking up strings in a Foo table.  A new Foo is
; made if none is found.

(define (table-searcher foo-string foo-next make-foo)
  (lambda (table string key)
    (let* ((index (hash-table-index string))
           (bucket (vm-vector-ref table index)))
      (let loop ((foo bucket))
        (cond ((vm-eq? foo false)
               (let ((new (make-foo string bucket key)))
                 (vm-vector-set! table index new)
                 new))
              ((vm-string=? string (foo-string foo))
	       foo)
              (else
	       (loop (foo-next foo))))))))

; Same thing, except we remove the entry if it is found.

(define (table-remover foo-string foo-next set-foo-next!)
  (lambda (table string)
    (let* ((index (hash-table-index string))
           (bucket (vm-vector-ref table index)))
      (let loop ((previous-foo false) (foo bucket))
	(cond ((vm-eq? foo false)
	       (unspecific))
	      ((not (vm-string=? string (foo-string foo)))
	       (loop foo (foo-next foo)))
	      ((vm-eq? previous-foo false)
	       (vm-vector-set! table index (foo-next foo)))
	      (else
	       (set-foo-next! previous-foo (foo-next foo))))))))

; Return a procedure that will apply PROC to every element of TABLE.

(define (table-walker foo-next)
  (lambda (proc table)
    (natural-for-each (lambda (index)
			(let loop ((entry (vm-vector-ref table index)))
			  (if (not (vm-eq? entry false))
			      (begin
				(proc entry)
				(loop (foo-next entry))))))
		      hash-table-slots)))

; Copy a table, treating the entries weakly.

(define (table-cleaner foo-next set-foo-next!)
  (let ((entry-cleaner (entry-cleaner foo-next set-foo-next!)))
    (lambda (table)
      (let ((table (s48-trace-value table)))
	(natural-for-each
	  (lambda (index)
	    (vm-vector-set! table
			    index
			    (entry-cleaner (vm-vector-ref table index))))
	  hash-table-slots)))))
  
(define (entry-cleaner foo-next set-foo-next!)
  (lambda (foo)
    (let loop ((foo foo) (okay false))
      (if (vm-eq? foo false)
	  okay
	  (loop (foo-next foo)
		(if (s48-extant? foo)
		    (let ((new-foo (s48-trace-value foo)))
		      (set-foo-next! new-foo okay)
		      new-foo)
		    okay))))))

