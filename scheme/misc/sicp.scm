; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Compatibility mode for use with Abelson & Sussman's book,
; Structure and Interpretation of Computer Programs.

; Requires ERROR, MAKE-TABLE, TABLE-REF, and TABLE-SET!.


; Incompatible change to the meanings of AND and OR

(define (and . rest)
  (let loop ((rest rest))
    (if (null? rest)
	#t
	(if (car rest)
	    (loop (cdr rest))
	    #f))))

(define (or . rest)
  (let loop ((rest rest))
    (if (null? rest)
	#f
	(let ((temp (car rest)))
	  (if temp
	      temp
	      (loop (cdr rest)))))))

; Misc. nonsense

(define-syntax sequence
  (syntax-rules ()
    ((sequence form ...) (begin form ...))))

(define mapcar map)
(define mapc for-each)

(define (1+ x) (+ x 1))
(define (-1+ x) (- x 1))

(define t #t)
(define nil #f)
(define (atom? x) (not (pair? x)))

(define (print x)
  (write x)
  (newline))
(define princ display)
(define prin1 write)

; Streams

(define-syntax cons-stream
  (syntax-rules ()
    ((cons-stream head tail)
     (cons head (delay tail)))))

(define head car)
(define (tail s) (force (cdr s)))
(define the-empty-stream '<the-empty-stream>)
(define (empty-stream? s) (eq? s the-empty-stream))

; EXPLODE and IMPLODE

(define (explode thing)
  (map (lambda (c) (string->symbol (string c)))
       (string->list (cond ((symbol? thing)
			    (symbol->string thing))
			   ((number? thing)
			    (number->string thing))
			   (else
			    (error "invalid argument to explode" thing))))))

(define (implode l)
  (string->symbol (list->string (map (lambda (s)
				       (string-ref (symbol->string s) 0))
				     l))))

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

; Need these special forms:
;   collect make-environment access the-environment

; The following are among the procedures defined in MIT's student
; system; I don't know how many are actually needed for the book:
;   ascii char nth nthcdr tyo vector-cons
;   accumulate filter map-stream append-streams
