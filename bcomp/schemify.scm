; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; schemify


; Flush nodes in favor of something a little more readable.
; Eventually, this ought to produce s-expressions with the property that
; have the same semantics as the original node.

(define (schemify node . env-option)
  (schemify1 node (if (null? env-option) #f (car env-option))))

(define (schemify1 node env)
  (if (node? node)
      (or (node-ref node 'schemify)
	  (let ((form ((operator-table-ref schemifiers (node-operator-id node))
		       node env)))
	    (node-set! node 'schemify form)
	    form))
      (schemify-sexp node env)))

(define schemifiers
  (make-operator-table (lambda (node env)
			 (let ((form (node-form node)))
			   (if (list? form)
			       (map (lambda (f) (schemify1 f env)) form)
			       form)))))

(define (define-schemifier name type proc)
  (operator-define! schemifiers (list name type) proc))

(define-schemifier 'name 'leaf
  (lambda (node env)
    (name->qualified (node-form node) env)))

(define-schemifier 'quote syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      (list (schemify1 (car form) env) (cadr form)))))

; Convert an alias (generated name) to S-expression form ("qualified name").
;
; As an optimization, we elide intermediate steps in the lookup path
; when possible.  E.g.
;     #(>> #(>> #(>> define-record-type define-accessors)
;		define-accessor)
;	   record-ref)
; is replaced with
;     #(>> define-record-type record-ref)

(define (name->qualified name env)
  (if env
      (if (generated? name)
	  (if (same-denotation? (lookup env name)
				(lookup env (generated-symbol name)))
	      (generated-symbol name)	;+++
	      (make-qualified
	       (let recur ((name (generated-parent-name name)))
		 (if (generated? name)
		     (let ((parent (generated-parent-name name)))
		       (if (let ((b1 (lookup env name))
				 (b2 (lookup env parent)))
			     (or (same-denotation? b1 b2)
				 (and (binding? b1)
				      (binding? b2)
				      (let ((s1 (binding-static b1))
					    (s2 (binding-static b2)))
					(and (transform? s1)
					     (transform? s2)
					     (eq? (transform-env s1)
						  (transform-env s2)))))))
			   (recur parent) ;+++
			   `#(>> ,(recur parent)
				 ,(generated-symbol name))))
		     name))
	       (generated-symbol name)))
	  name)
      (desyntaxify name)))

; lambda, let-syntax, letrec-syntax...

(define-schemifier 'letrec syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      `(letrec ,(map (lambda (spec)
		       `(,(car spec) ,(schemify1 (cadr spec) env)))
		     (cadr form))
	 ,@(map (lambda (f) (schemify1 f env))
		(cddr form))))))

(define (schemify-sexp thing env)
  (cond ((name? thing)
	 (name->qualified thing env))
	((pair? thing)
	 (let ((x (schemify-sexp (car thing) env))
	       (y (schemify-sexp (cdr thing) env)))
	   (if (and (eq? x (car thing))
		    (eq? y (cdr thing)))
	       thing			;+++
	       (cons x y))))
	((vector? thing)
	 (let ((new (make-vector (vector-length thing) #f)))
	   (let loop ((i 0) (same? #t))
	     (if (>= i (vector-length thing))
		 (if same? thing new)	;+++
		 (let ((x (schemify-sexp (vector-ref thing i) env)))
		   (vector-set! new i x)
		   (loop (+ i 1)
			 (and same? (eq? x (vector-ref thing i)))))))))
	(else thing)))


; Qualified names

(define (make-qualified transform-name sym)
  (vector '>> transform-name sym))

(define (qualified? thing)
  (and (vector? thing)
       (= (vector-length thing) 3)
       (eq? (vector-ref thing 0) '>>)))

(define (qualified-parent-name q) (vector-ref q 1))
(define (qualified-symbol q) (vector-ref q 2))
