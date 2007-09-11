; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; The (rnrs enums (6)) library.

(define (make-enum-type members)
  (let* ((table (make-constant-table
		 (map cons members (iota (length members)))
		 symbol-hash))
	 (find-index
	  (lambda (symbol)
	    (constant-table-lookup table symbol))))
    (big:make-enum-set-type 'r6rs-enum
			    (lambda (symbol)
			      (and (find-index symbol) #t))
			    (list->vector members)
			    find-index)))


(define (iota n)
  (let loop ((n n) (r '()))
    (if (zero? n)
	r
	(loop (- n 1)
	      (cons (- n 1) r)))))

(define (make-enum-set type symbols)
  (big:elements->enum-set type symbols))

(define (enum-type->enum-set et)
  (big:enum-set-negation
   (big:elements->enum-set et '())))

(define (make-enumeration symbols)
  (enum-type->enum-set (make-enum-type symbols)))

(define (enum-set-universe es)
  (enum-type->enum-set (big:enum-set-type es)))

(define (enum-set-indexer es)
  (lambda (symbol)
    (big:enum-set-type-element-index (big:enum-set-type es) symbol)))

(define (enum-set-constructor es)
  (let ((et (big:enum-set-type es)))
    (lambda (symbols)
      (make-enum-set et symbols))))

(define enum-set->list big:enum-set->list)

(define (enum-set-member? symbol es)
  (big:enum-set-member? es symbol))

(define (enum-set-subset? es1 es2)
  (if (eq? (big:enum-set-type es1)
	   (big:enum-set-type es2))
      (big:enum-set-subset? es1 es2)
      ;; slow case
      (every? (lambda (member)
		(enum-set-member? member es2))
	      (enum-set->list es1))))

(define (enum-set=? es1 es2)
  (if (eq? (big:enum-set-type es1)
	   (big:enum-set-type es2))
      (big:enum-set=? es1 es2)
      ;; slow case
      (and (enum-set-subset? es1 es2)
	   (enum-set-subset? es2 es1))))

(define enum-set-union big:enum-set-union)
(define enum-set-intersection big:enum-set-intersection)
(define enum-set-difference big:enum-set-difference)
(define enum-set-complement big:enum-set-negation)

(define (enum-set-projection es1 es2)
  (if (eq? (big:enum-set-type es1)
	   (big:enum-set-type es2))
      es1
      (let ((et2 (big:enum-set-type es2)))
	(big:elements->enum-set et2
				(filter (lambda (element)
					  (and (big:enum-set-type-member? et2 element)
					       (enum-set-member? element es2)))
					(enum-set->list es1))))))


(define-syntax define-type-name-keyword
  (lambda (form0 rename0 compare0)
    (let ((%define-syntax (rename0 'define-syntax))
	  (%lambda (rename0 'lambda))
	  (%desyntaxify (rename0 'desyntaxify))
	  (%code-quote (rename0 'code-quote))
	  (%quote (rename0 'quote)))
    `(,%define-syntax ,(cadr form0)
       (,%lambda (form rename compare)
	 (let ((id (,%desyntaxify (cadr form))))
	   (let loop ((members ',(cddr form0)))
	     (cond
	      ((null? members) form)
	      ((eq? (car members) id) (list (,%code-quote ,%quote) id))
	      (else (loop (cdr members)))))))))))

(define-syntax define-enumeration
  (syntax-rules ()
    ((define-enumeration ?type-name
       (?member ...)
       ?constructor)
     (begin
       (define-type-name-keyword ?type-name ?member ...)
       (define type (make-enum-type '(?member ...)))
       (define (make elements)
	 (big:elements->enum-set type elements))
       (big:define-enum-set-maker ?constructor make ?type-name)))))
