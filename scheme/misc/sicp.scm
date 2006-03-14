; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Compatibility mode for use with Abelson & Sussman's book,
; Structure and Interpretation of Computer Programs.

; Requires ERROR, MAKE-TABLE, TABLE-REF, and TABLE-SET!.

; Streams

(define-syntax cons-stream
  (syntax-rules ()
    ((cons-stream head tail)
     (cons head (delay tail)))))

(define stream-car car)
(define (stream-cdr s) (force (cdr s)))
(define the-empty-stream '(list <the-empty-stream>))
(define (stream-null? s) (eq? s the-empty-stream))

; GET and PUT

(define (make-property-module)
  (define symbol-properties-table #f)

  (define (put symbol indicator value)
    (let* ((probe (table-ref symbol-properties-table symbol))
	   (table (if probe
		      probe
		      (let ((table (make-table)))
			(table-set! symbol-properties-table symbol table)
			table))))
      (table-set! table indicator value)))

  (define (get symbol indicator)
    (let ((probe (table-ref symbol-properties-table symbol)))
      (if probe
	  (table-ref probe indicator)
	  #f)))

  (set! symbol-properties-table (make-table))

  (cons get put))

(define property-module (make-property-module))
(define get (car property-module))
(define put (cdr property-module))

