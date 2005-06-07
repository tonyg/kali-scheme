; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

; We cache the no-env version because that's the one used to generate the
; sources in the debugging info (which takes up a lot of space).

(define (schemify-node node env)
  (or (and (not env)
	   (node-ref node 'schemify))
      (let ((form ((operator-table-ref schemifiers (node-operator-id node))
		     node
		     env)))
	(if (not env)
	    (node-set! node 'schemify form))
	form)))

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
	(let ((form (node-form node)))
          (if (or #f (node? form))
              (schemify-node form env)
              (desyntaxify form))))))

(define-schemifier 'quote syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      `(quote ,(cadr form)))))

(define-schemifier 'call 'internal
  (lambda (node env)
    (map (lambda (node)
	   (schemify-node node env))
	 (node-form node))))

; We ignore the list of free variables in flat lambdas.

(define (schemify-lambda node env)
  (let ((form (node-form node)))
    `(lambda ,(schemify-formals (cadr form) env)
       ,(schemify-node (last form) env))))

(define-schemifier 'lambda syntax-type schemify-lambda)
(define-schemifier 'flat-lambda syntax-type schemify-lambda)

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
      (schemify-letrec (cadr form) (caddr form) env))))

(define-schemifier 'pure-letrec syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      (schemify-letrec (cadr form) (cadddr form) env))))

(define (schemify-letrec specs body env)
  `(letrec ,(map (lambda (spec)
		   (schemify-nodes spec env))
		 specs)
     ,(schemify-node body env)))

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

