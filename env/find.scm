; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; work in progress

; packages features package-commands table

;(define (find name)
;  (for-each-structure
;   (lambda (name s)
;     (let ((den (package-lookup ??? (structure-package s) name)))
;       (if ?)))))

(define (for-each-structure proc)
  (let ((p (config-package)))
    (for-each-binding
     (lambda (name den)
       (if (and (symbol? den)
		(let ((loc (package-lookup-location p name #f)))
		  (and (location-defined? loc)
		       (location-assigned? loc)
		       (structure? (contents loc)))))
	   (proc name (contents loc))))
     p)
    (newline)))

(define (for-each-binding proc p)
  (let ((table (make-table)))
    (for-each-definition
      (lambda (name binding)
	(table-set! table name #t)
	(proc name ...))
      p)
    (for-each (lambda (s)
		(for-each-export
		     (lambda (name type binding)
		       (if (not (table-ref table name))
			   (proc name binding)))
		     s))
	      (package-opens p))))