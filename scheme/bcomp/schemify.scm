; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; schemify

; This is only used for producing error and warning messages.

; Flush nodes and generated names in favor of something a little more
; readable.  Eventually, (schemify node env) ought to produce an
; s-expression that has the same semantics as node, when node is fully
; expanded.

(define (schemify node . maybe-env)
  (if (node? node)
      (schemify-node node
		     (if (null? maybe-env)
			 #f
			 (car maybe-env)))
      (schemify-sexp node)))
		     

(define schemifiers
  (make-operator-table (lambda (node env)
			 (let ((form (node-form node)))
			   (if (list? form)
			       (let ((op (car form)))
				 (cons (cond ((operator? op)
					      (operator-name op))
					     ((node? op)
					      (schemify-node op env))
					     (else
					      (schemify-sexp op)))
				       (schemify-nodes (cdr form) env)))
			       form)))))

(define (schemify-node node env)
  ((operator-table-ref schemifiers
		       (node-operator-id node))
     node
     env))

(define (schemify-nodes nodes env)
  (map (lambda (node)
	 (schemify-node node env))
       nodes))

(define (define-schemifier name type proc)
  (operator-define! schemifiers name type proc))

(define-schemifier 'name 'leaf
  (lambda (node env)
    (if env
	(name->qualified (node-form node)
			 env)
	(desyntaxify (node-form node)))))

(define-schemifier 'quote syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      `(quote ,(cadr form)))))

(define-schemifier 'call 'internal
  (lambda (node env)
    (map (lambda (node)
	   (schemify-node node env))
	 (node-form node))))

(define-schemifier 'lambda syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      `(lambda ,(schemify-formals (cadr form) env)
	 ,(schemify-node (caddr form) env)))))

(define (schemify-formals formals env)
  (cond ((node? formals)
	 (schemify-node formals env))
	((pair? formals)
	 (cons (schemify-node (car formals) env)
	       (schemify-formals (cdr formals) env)))
	(else
	 (schemify-sexp formals))))  ; anything besides '() ?

; let-syntax, letrec-syntax...

(define-schemifier 'letrec syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      `(letrec ,(map (lambda (spec)
		       (schemify-nodes spec env))
		     (cadr form))
	 ,@(map (lambda (f) (schemify-node f env))
		(cddr form))))))

(define-schemifier 'loophole syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      (list 'loophole
	    (type->sexp (cadr form) #t)
	    (schemify-node (caddr form) env)))))

(define-schemifier 'lap syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      `(lap
	,(cadr form)
	,(schemify-nodes (caddr form) env)
	. ,(cdddr form)))))

;----------------

(define (schemify-sexp thing)
  (cond ((name? thing)
	 (desyntaxify thing))
	((pair? thing)
	 (let ((x (schemify-sexp (car thing)))
	       (y (schemify-sexp (cdr thing))))
	   (if (and (eq? x (car thing))
		    (eq? y (cdr thing)))
	       thing			;+++
	       (cons x y))))
	((vector? thing)
	 (let ((new (make-vector (vector-length thing) #f)))
	   (let loop ((i 0) (same? #t))
	     (if (>= i (vector-length thing))
		 (if same? thing new)	;+++
		 (let ((x (schemify-sexp (vector-ref thing i))))
		   (vector-set! new i x)
		   (loop (+ i 1)
			 (and same? (eq? x (vector-ref thing i)))))))))
	(else thing)))

