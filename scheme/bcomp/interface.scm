; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Interfaces

(define-record-type interface :interface
  (really-make-interface ref walk clients name)
  interface?
  (ref ref-method)
  (walk walk-method)
  (clients interface-clients)
  (name interface-name set-interface-name!))

(define-record-discloser :interface
  (lambda (int) (list 'interface (interface-name int))))

(define (interface-ref int name)
  ((ref-method int) name))

(define (for-each-declaration proc int)
  ((walk-method int) proc))

(define (note-reference-to-interface! int thing)
  (let ((pop (interface-clients int)))
    (if pop
	(add-to-population! thing pop)
	;; If it's compound, we really ought to descend into its components
	)))

; If name is #f, then the interface is anonymous, so we don't need to
; make a population.

(define (make-interface ref walk name)
  (really-make-interface ref
			 walk
			 (make-population)
			 name))

; Simple interfaces (export (name type) ...)

(define (make-simple-interface name items)
  (let ((table (make-symbol-table)))
    (for-each (lambda (item)
		(if (pair? item)
		    (let ((name (car item))
			  (type (cadr item)))
		      (if (or (null? name) (pair? name))
			  ;; Allow ((name1 name2 ...) type)
			  (for-each (lambda (name)
				      (table-set! table name type))
				    name)
			  (table-set! table name type)))
		    (table-set! table item undeclared-type)))
	      items)
    (make-table-immutable! table)
    (really-make-simple-interface table name)))

(define (really-make-simple-interface table name)
  (make-interface (lambda (name) (table-ref table name))
		  (lambda (proc) (table-walk proc table))
		  name))


; Compoune interfaces

(define (make-compound-interface name . ints)
  (let ((int
	 (make-interface (lambda (name)
			   (let loop ((ints ints))
			     (if (null? ints)
				 #f
				 (or (interface-ref (car ints) name)
				     (loop (cdr ints))))))
			 (lambda (proc)
			   (for-each (lambda (int)
				       (for-each-declaration proc int))
				     ints))
			 name)))
    (for-each (lambda (i)
		(note-reference-to-interface! i int))
	      ints)
    int))


(define (note-interface-name! int name)
  (if (and name (not (interface-name int)))
      (set-interface-name! int name)))
