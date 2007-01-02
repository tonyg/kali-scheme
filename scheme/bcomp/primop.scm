; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Primops.

(define-record-type primop :primop
  (make-primop name type closed compilator)
  primop?
  (name primop-name)
  (type primop-type)
  (closed primop-closed)
  (compilator primop-compilator))

(define-record-discloser :primop
  (lambda (primop)
    `(primop ,(primop-name primop))))

(define primop-table (make-symbol-table))

; This is used to add definitions of the primitives to a package.

(define (walk-primops proc)
  (table-walk (lambda (name primop)
		(proc name (primop-type primop) primop))
	      primop-table))

(define (define-compiler-primitive name type compilator closed)
  (table-set! primop-table
	      name
	      (make-primop name (or type value-type) closed compilator)))

(define (get-primop name)
  (or (table-ref primop-table name)
      (error "unknown compiler primitive" name)))
