; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Miniature package system.  This links mini-eval up to the output of
; the package reifier.

(define (make-simple-package opens eval for-syntax . name-option)

  (define operators (make-table))
  (define locations (make-table))

  (define (lookup-operator name)
    (table-ref operators name))

  (define (lookup name)
    (or (table-ref locations name)
	(table-ref operators name)
	(let loop ((opens opens))
	  (if (null? opens)
	      (error "unbound" name)
	      (or (structure-lookup (car opens) name)
		  (loop (cdr opens)))))))

  (define (define-operator! name op)
    (table-set! operators name op))

  (define (define-location! name loc)
    (table-set! locations name loc))

  (table-set! operators '%%lookup-operator%% lookup-operator)
  (table-set! operators '%%lookup%% lookup)  ;for make-structure, below

  (table-set! operators '%%define-operator%% define-operator!)
  (table-set! operators '%%define-location%% define-location!)

  (table-set! operators '%%define%%		;for mini-eval
	      (lambda (name val)
		(or (table-ref locations name)
		    (define-location! name (make-location val name)))))

  (lambda (name)
    (let ((den (lookup name)))
      (if (location? den)
	  (if (and (location-defined? den)
		   (location-assigned? den))
	      (contents den)
	      (error "undefined" name))
	  (begin (warn "not a variable" name)
		 den)))))

(define (package-define! p name op)
  ((p '%%define-operator%%) name op))

(define (probe-package p name) name)

(define (package-lookup p name)
  ((p '%%lookup-operator%%) name))

(define (package-ensure-defined! p name)
  (package-define! p name (make-location 'defined name)))

(define (set-package-name! . rest) 'lose)

; Structures

(define (make-structure package signature name)
  (let ((lookup (package '%%lookup%%))
	(names (map (lambda (x) (if (pair? x) (cadr x) x)) signature)))
    (cons name
	  (lambda (name)
	    (if (memq name names)
		(lookup name)
		#f)))))

(define (structure-lookup struct name)
  ((cdr struct) name))

(define structure-name car)

; Etc.

(define (make-simple-signature names)
  names)

(define (get-operator . rest)   (cons 'get-operator rest))
(define (make-transform . rest) (cons 'make-transform rest))
(define (usual-transform-procedure . rest)
  (cons 'usual-transform-procedure rest))

(define (integrate-all-primitives! . rest) 'lose)
(define (structure-package s) 'lose)

(define (transform-for-structure-ref . rest)
  (cons 'transform-for-structure-ref rest))
(define (make-procedure-for-inline-transform . rest)
  (cons 'make-procedure-for-inline-transform rest))
