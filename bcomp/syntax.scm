; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Syntactic stuff: transforms and operators.

(define (syntax? d)
  (cond ((operator? d)
	 (eq? (operator-type d) 'syntax))
	((transform? d)
	 (eq? (transform-type d) 'syntax))
	(else #f)))

(define (name? thing)
  (or (symbol? thing)
      (generated? thing)))

(define unbound? name?)


; --------------------
; Operators (= special operators and primitives)

(define-record-type operator type/operator
  (make-operator type name uid transform)  ; transform ?
  operator?
  (type operator-type)
  (name operator-name)
  (transform operator-transform set-operator-transform!)
  (uid operator-uid-maybe set-operator-uid-maybe!))

(define-record-discloser type/operator
  (lambda (s) (list 'operator (operator-name s))))

(define (get-operator name . type-option)
  (let ((type (if (null? type-option) #f (car type-option)))
	(probe (table-ref operators-table name)))
    (if (operator? probe)
	(let ((previous-type (operator-type probe)))
	  (if (and type (not (equal? previous-type type)))
	      (warn "operator type inconsistency" name type previous-type))
	  probe)
	(let ((op (make-operator (or type 'value) name #f #f)))
	  (operator-uid op)
	  op))))

(define (operator-uid op)
  (or (operator-uid-maybe op)
      (let ((uid *operator-uid*))
	(if (>= uid number-of-operators)
	    (warn "too many operators" (operator-name op) (operator-type op)))
	(set! *operator-uid* (+ *operator-uid* 1))
	(table-set! operators-table (operator-name op) op)
	(set-operator-uid-maybe! op uid)
	uid)))

(define *operator-uid* 0)

(define operators-table (make-table))

(define number-of-operators 200)  ;Fixed-size limits bad, but speed good

(define (make-compilator-table default)
  (let ((v (make-vector number-of-operators default)))
    (cons (lambda (name type proc)
	    (vector-set! v (operator-uid (get-operator name type)) proc))
	  (lambda (s)
	    (vector-ref v (operator-uid s))))))


; --------------------
; Transforms

; A transform represents a source-to-source rewrite rule: either a
; macro or an in-line procedure.

(define-record-type transform type/transform
  (make-transform xformer env type aux-names source id)
  transform?
  (xformer   transform-procedure)
  (env	     transform-env)
  (type	     transform-type)      ;'syntax (macro) or 'procedure (in-line)
  (aux-names transform-aux-names)
  (source    transform-source)    ;for reification
  (id	     transform-id))

(define-record-discloser type/transform
  (lambda (m) (list 'transform (transform-id m))))

(define (maybe-transform den exp lookup)
  ((transform-procedure den)
   exp
   (make-name-generator (transform-env den) (car exp))
   (lambda (name1 name2)
     (and (name? name1)
	  (name? name2)
	  (same-denotation? (lookup name1)
			    (lookup name2))))))

(define (transform den exp lookup)
  (let ((new-exp (maybe-transform den exp lookup)))
    (if (eq? new-exp exp)
	(syntax-error "use of macro doesn't match definition"
		      exp)
	new-exp)))

(define (make-name-generator env parent-name)
  (let ((alist '()))			;list of name * generated
    (lambda (symbol)
      (if (symbol? symbol)
	  (let ((probe (assq symbol alist)))
	    (if probe
		(cdr probe)
		(let ((new-name (generate-name symbol env parent-name)))
		  (set! alist (cons (cons symbol new-name)
				    alist))
		  new-name)))
	  (error "non-symbol argument to rename procedure"
		 symbol parent-name)))))

(define (same-denotation? x y)
  (or (equal? x y)
      (and (transform? x)
	   (transform? y)
	   (same-transform? x y))))

(define (same-transform? x y)
  (and (eq? (transform-id x) (transform-id y))
       (eq? (transform-env x) (transform-env y))
       (equal? (transform-source x) (transform-source y))))

; --------------------
; Generated names

; Generated names (more like syntactic closures, actually).
; The parent is always another name (perhaps generated).
; The parent chain provides an access path, should one ever be needed.
; That is: transform = (lookup env parent).

(define-record-type generated type/generated
  (make-generated symbol env parent-name pname)
  generated?
  (symbol      generated-symbol)
  (env	       generated-env)
  (parent-name generated-parent-name)
  (pname       generated-pname set-generated-pname!))

(define-record-discloser type/generated
  (lambda (name) (list 'generated (generated-symbol name))))

(define (generate-name symbol env parent-name)
  (make-generated symbol env parent-name #f))

(define (name->symbol name)
  (if (symbol? name)
      name
      (or (generated-pname name)
	  (let ((uid *generated-uid*))
	    (set! *generated-uid* (+ *generated-uid* 1))
	    (let ((pname
		   (string->symbol
		    (string-append (symbol->string (generated-symbol name))
				   "##"
				   (number->string uid)))))
	      (set-generated-pname! name pname)
	      pname)))))
(define *generated-uid* 0)

(define (name-hash name)
  (cond ((symbol? name)
	 (string-hash (symbol->string name)))
	((generated? name)
	 (name-hash (generated-symbol name)))
	(else (error "invalid name" name))))


; Used by QUOTE to turn generated names back into symbols

(define (desyntaxify thing)
  (cond ((or (boolean? thing) (null? thing) (number? thing)
	     (symbol? thing) (char? thing))
	 thing)
	((string? thing)
	 (make-immutable thing))
	((generated? thing) (desyntaxify (generated-symbol thing)))
	((pair? thing)
	 (make-immutable
	  (let ((x (desyntaxify (car thing)))
		(y (desyntaxify (cdr thing))))
	    (if (and (eq? x (car thing))
		     (eq? y (cdr thing)))
		thing
		(cons x y)))))
	((vector? thing)
	 (make-immutable
	  (let ((new (make-vector (vector-length thing) #f)))
	    (let loop ((i 0) (same? #t))
	      (if (>= i (vector-length thing))
		  (if same? thing new)
		  (let ((x (desyntaxify (vector-ref thing i))))
		    (vector-set! new i x)
		    (loop (+ i 1)
			  (and same? (eq? x (vector-ref thing i))))))))))
	((location? thing) thing)
	((operator? thing) (operator-name thing))  ;Foo
	(else
	 (warn "invalid datum in quotation" thing)
	 thing)))

(define (make-immutable thing)
  (make-immutable! thing)
  thing)

; --------------------
; Utilities

; Definitions

(define operator/define (get-operator 'define 'syntax))
(define operator/lambda (get-operator 'lambda 'syntax))
(define operator/unassigned (get-operator 'unassigned 'procedure))

(define (normalize-definition form)
  (let ((pat (cadr form)))
    (if (pair? pat)
	(make-define (car pat)
		     `(,operator/lambda ,(cdr pat) ,@(cddr form)))
	(make-define pat
		     (if (null? (cddr form))
			 `(,operator/unassigned)
			 (caddr form))))))

(define (make-define var rhs)
  `(,operator/define ,var ,rhs))
(define (define? form)
  (and (pair? form) (eq? (car form) operator/define)))
(define define-lhs cadr)
(define define-rhs caddr)

(define (literal? exp)
  (or (number? exp) (char? exp) (string? exp) (boolean? exp)))

(define (number-of-required-args formals)
  (do ((l formals (cdr l))
       (i 0 (+ i 1)))
      ((not (pair? l)) i)))

(define (n-ary? formals)
  (cond ((null? formals) #f)
	((pair? formals) (n-ary? (cdr formals)))
	(else #t)))

(define (normalize-formals formals)
  (cond ((null? formals) '())
        ((pair? formals)
	 (cons (car formals) (normalize-formals (cdr formals))))
        (else (list formals))))


; Name of file being compiled

(define $source-file-name (make-fluid #f))
