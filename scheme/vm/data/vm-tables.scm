; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; String hash tables for managing three tables:
;  the symbol table : string -> symbol
;  exported-bindings : string -> shared-binding
;  imported-bindings : string -> shared-binding
;
; The hash buckets are linked throught the symbol-next and shared-binding-next
; fields.  The values in the links are converted to fixnums to prevent the GC
; from tracing them.

; Size of the hash vectors (is this a reasonable size?).

(define hash-table-slots 1024)

(define hash-table-size (vm-vector-size hash-table-slots))

; The various hash-table values all have their own link fields.

(define hash-table-entry-size 0)

(define (value->link value)
  (assert (not (fixnum? value)))
  (if (stob? value)
      (descriptor->fixnum value)
      value))

(define (link->value link)
  (assert (or (false? link)
	      (fixnum? link)))
  (if (fixnum? link)
      (fixnum->stob link)
      link))

(define (hash-table-ref table i)
  (link->value (vm-vector-ref table i)))

(define (hash-table-index string)
  (bitwise-and (vm-string-hash string)
	       (- hash-table-slots 1)))

; All buckets are initially false.

(define (make-hash-table+gc)
  (let ((table (vm-make-vector+gc hash-table-slots)))
    (natural-for-each (lambda (index)
			(vm-vector-set! table index false))
		      hash-table-slots)
    table))

; Return a procedure for adding FOO's to tables.

(define (table-adder foo-string set-foo-next!)
  (lambda (table foo)
    (let ((index (hash-table-index (foo-string foo))))
      (set-foo-next! foo (vm-vector-ref table index))
      (vm-vector-set! table index (value->link foo)))))

; Return a function for looking up strings in a Foo table.  A new Foo is
; made if none is found.

(define (table-searcher foo-string foo-next make-foo)
  (lambda (table string key)
    (let* ((index (hash-table-index string))
           (bucket (hash-table-ref table index)))
      (let loop ((foo bucket))
        (cond ((vm-eq? foo false)
               (let ((new (make-foo string (value->link bucket) key)))
                 (vm-vector-set! table index (value->link new))
                 new))
              ((vm-string=? string (foo-string foo))
	       foo)
              (else
	       (loop (link->value (foo-next foo)))))))))

; Same thing, except we remove the entry if it is found.

(define (table-remover foo-string foo-next set-foo-next!)
  (lambda (table string)
    (let* ((index (hash-table-index string))
           (bucket  (hash-table-ref table index)))
      (let loop ((previous-foo false) (foo bucket))
	(cond ((vm-eq? foo false)
	       (unspecific))
	      ((not (vm-string=? string (foo-string foo)))
	       (loop foo (link->value (foo-next foo))))
	      ((vm-eq? previous-foo false)
	       (vm-vector-set! table index (foo-next foo)))
	      (else
	       (set-foo-next! previous-foo (foo-next foo))))))))

; Return a procedure that will apply PROC to every element of TABLE.

(define (table-walker foo-next)
  (lambda (proc table)
    (natural-for-each (lambda (index)
			(let loop ((entry (hash-table-ref table index)))
			  (if (not (vm-eq? entry false))
			      (begin
				(proc entry)
				(loop (link->value (foo-next entry)))))))
		      hash-table-slots)))

; Return a procedure that will apply PROC to every element of TABLE
; until PROC returns #f.

(define (table-while-walker foo-next)
  (lambda (proc table)
    (natural-for-each-while (lambda (index)
			      (let loop ((entry (hash-table-ref table index)))
				(if (and (not (vm-eq? entry false))
					 (proc entry))
				    (loop (link->value (foo-next entry))))))
			    hash-table-slots)))

; Copy a table, retaining all entries.

(define (table-tracer foo-next set-foo-next! trace-value)
  (make-table-copier trace-value
		     (entry-tracer foo-next
				   set-foo-next!
				   trace-value)))
  
; Copy a table, dropping unreferenced entries.

(define (table-cleaner foo-next set-foo-next! extant? trace-value)
  (make-table-copier trace-value
		     (entry-cleaner foo-next
				    set-foo-next!
				    extant?
				    trace-value)))
  
; Copy a table.

(define (make-table-copier trace-value entry-tracer)
  (lambda (table)
    (let ((table (trace-value table)))
      (natural-for-each
       (lambda (index)
	 (vm-vector-set! table
			 index
			 (entry-tracer (vm-vector-ref table index))))
       hash-table-slots)
      table)))

; Loop down a list of foos tracing all of them.  We reverse the
; list in doing so.

(define (entry-tracer foo-next set-foo-next! trace-value)
  (lambda (foo-link)
    (let loop ((foo-link foo-link) (done-link false))
      (let ((foo (link->value foo-link)))
	(if (false? foo)
	    done-link
	    (let* ((new-foo (trace-value foo))
		   (next-link (foo-next new-foo)))
	      (set-foo-next! new-foo done-link)
	      (loop next-link
		    (value->link new-foo))))))))

; Loop down a list of foos removing any untraced ones.  We reverse the
; list in doing so.

(define (entry-cleaner foo-next set-foo-next! extant? trace-value)
  (lambda (foo-link)
    (let loop ((foo-link foo-link) (okay-link false))
      (let ((foo (link->value foo-link)))
	(cond ((false? foo)
	       okay-link)
	      ((extant? foo)
	       (let* ((new-foo (trace-value foo))
		      (next-link (foo-next new-foo)))
		 (set-foo-next! new-foo okay-link)
		 (loop next-link
		       (value->link new-foo))))
	      (else
	       (loop (foo-next foo)
		     okay-link)))))))

; Add DELTA to all hidden pointers.

(define (table-relocator foo-next set-foo-next!)
  (lambda (table delta)
    (if (vm-vector? table)
	(natural-for-each
	 (lambda (index)
	   (let ((bucket (hash-table-ref table index)))
	     (if (not (false? bucket))
		 (let ((bucket (+ bucket delta)))
		   (vm-vector-set! table index (value->link bucket))
		   (let loop ((entry bucket))
		     (let ((next (link->value (foo-next entry))))
		       (if (not (false? next))
			   (let ((next (+ next delta)))
			     (set-foo-next! entry (value->link next))
			     (loop next)))))))))
	 hash-table-slots))))
