; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


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

; Copy the table and remove any unreachable symbols.

(let ((cleaner (table-cleaner vm-symbol-next
			      vm-set-symbol-next!
			      s48-extant?
			      s48-trace-value)))
  (add-post-gc-cleanup!
   (lambda (major? in-trouble?)
     (set! *the-symbol-table* (cleaner *the-symbol-table*)))))

; For the image writer.

(define (s48-symbol-table)
  *the-symbol-table*)

; There is no symbol table in images created by the static linker.

(define (install-symbols!+gc symbol-table)
  (if (eq? symbol-table false)
      (build-symbol-table+gc)
      (set! *the-symbol-table* symbol-table)))

; Create the symbol table and then add to it all currently-extant symbols.

(define (build-symbol-table+gc)
  (set! *the-symbol-table* (make-hash-table+gc))
  (let ((symbols (let ((maybe (s48-find-all (enum stob symbol))))
		   (if (eq? maybe false)
		       (begin
			 (s48-collect #t)
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
