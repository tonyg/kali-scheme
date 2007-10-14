; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-syntax define-external-enum-type-with-unknowns
  (syntax-rules ()
    ((define-external-enum-type-with-unknowns
       ?type-name
       (?enumerand ...) ; The C code knows about the order
       ?unknown-type-name ?:unknown-type-name
       ?make-unknown ?unknown-predicate? ?unknown-accessor
       ?offset ; C code knows this
       ?predicate? ?->raw ?raw->)
    (begin

      (define-record-type ?unknown-type-name ?:unknown-type-name
	(?make-unknown number)
	?unknown-predicate?
	(number ?unknown-accessor))

      (define-record-discloser ?:unknown-type-name
	(lambda (r)
	  (list '?unknown-type-name
		(?unknown-accessor r))))

      (define-enumeration ?type-name
	(?enumerand ...)
	set)

      (define all (enum-set-complement (set)))
      (define index (enum-set-indexer all))
      (define set-type (enum-set-type all))

      (define (?predicate? thing)
	(or (and (symbol? thing)
		 (enum-set-member? thing all))
	    (?unknown-predicate? thing)))

      (define (?->raw val)
	(if (?unknown-predicate? val)
	    (+ (?unknown-accessor val) ?offset)
	    (index val)))

      (define (?raw-> raw)
	(if (>= raw ?offset)
	    (?make-unknown (- raw ?offset))
	    (vector-ref (enum-set-type-values set-type) raw)))
      ))))
