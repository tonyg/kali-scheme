; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The symbol table, which is just a string table full of symbols.

(define *the-symbol-table*)

(define-consing-primitive intern (string->)
  (lambda (ignore)
    (+ vm-symbol-size
       hash-table-entry-size))
  (let ((searcher (table-searcher vm-symbol->string
				  vm-symbol-next
				  vm-make-symbol)))
    (lambda (string key)
      (searcher *the-symbol-table* string key)))
  return)

; Using the regular set-...-next! procedures in the cleanup procedure is
; unfortunate, because they go through the write barrier.  Of course, we
; could disable that for these setters, since the symbol table has to be
; checked every GC anyway.

; Copy the table and remove any unreachable symbols.  This is exported for
; use when writing an image.

(define s48-copy-symbol-table
  (let ((cleaner! (table-cleaner vm-symbol-next vm-set-symbol-next!)))
    (lambda ()
      (let ((new (s48-trace-value *the-symbol-table*)))
	(cleaner! new)
	new))))

(add-post-gc-cleanup!
  (lambda ()
    (set! *the-symbol-table* (s48-copy-symbol-table))))

; There is no symbol table in images created by the static linker.

(define (install-symbols!+gc symbol-table)
  (if (eq? symbol-table false)
      (build-symbol-table+gc)
      (set! *the-symbol-table* symbol-table)))

; Create the symbol table and then add to it all currently-extant symbols.

(define (build-symbol-table+gc)
  (set! *the-symbol-table* (make-hash-table (ensure-space hash-table-size)))
  (let ((symbols (let ((maybe (s48-find-all (enum stob symbol))))
		   (if (eq? maybe false)
		       (begin
			 (collect)
			 (let ((maybe (s48-find-all (enum stob symbol))))
			   (if (eq? maybe false)
			       (error "insufficient heap space to build symbol table"))
			   maybe))
		       maybe))))
    (natural-for-each (lambda (i)
			(symbol-table-add! *the-symbol-table*
					   (vm-vector-ref symbols i)))
		      (vm-vector-length symbols))))

(define symbol-table-add! (table-adder vm-symbol->string vm-set-symbol-next!))
