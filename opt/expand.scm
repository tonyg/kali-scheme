; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Front end for Scheme 48 compilation, optimization, or whatever.

; Entry point for debugging or casual use.

(define (expand-form form p)
  (let-fluid $assignments #f
    (lambda ()
      (expand-begin (scan-forms (list form) p #f)
		    (package->environment p)))))


; After processing a package body, we change the types of all
; unassigned top-level variables from (VARIABLE <type>) to <type>.

(define (expand-stuff stuff p)
  (let* ((table (make-table))
	 (new-stuff
	  (let-fluid $assignments (cons p table)
	    (lambda ()
	      (map (lambda (filename+nodes)
		     (let ((filename (car filename+nodes))
			   (nodes (cdr filename+nodes)))
		       (let-fluid $source-file-name filename
			 (lambda ()
			   (cons filename
				 (expand-forms nodes p))))))
		   stuff)))))
    (for-each (lambda (filename+nodes)
		(for-each (lambda (node)
			    (if (define-node? node)
				(maybe-update-known-type node p table)))
			  (cdr filename+nodes)))
	      new-stuff)
    new-stuff))

(set-optimizer! 'expand expand-stuff)

(define (maybe-update-known-type node p table)
  (let ((lhs (cadr (node-form node))))
    (if (not (table-ref table lhs))
	(let* ((new-type (node-type (caddr (node-form node))
				    (package->environment p)))
	       (compat (compatible-types? new-type value-type)))
	  (if compat
	      (package-define! p lhs (if (eq? compat 'definitely)
					 new-type
					 value-type))
	      (warn "ill-typed right-hand side"
		    (schemify node)
		    new-type))))))

(define define-node? (node-predicate 'define syntax-type))
(define lambda-node? (node-predicate 'lambda))


; Expand a package body.

(define (expand-forms scanned-forms p)
  (let ((env (package->environment p)))
    (map (lambda (node)
	   (expand node env))
	 scanned-forms)))


; --------------------
; Expand a single form.

(define (expand form env)
  (let ((node (classify form env)))
    (if (already-expanded? node)
	node
	((get-expander (node-operator-id node)) node env))))

(define expanders
  (make-operator-table (lambda (node env)
			 (let ((form (node-form node)))
			   (make-expanded node
					  (cons (car form)
						(map (lambda (arg-exp)
						       (expand arg-exp env))
						     (cdr form))))))))

(define (define-expander name proc)
  (operator-define! expanders name proc))

(define (get-expander id)
  (operator-table-ref expanders id))

(define-expander 'literal
  (lambda (node env)
    (set-expanded node)))

(define-expander 'name
  (lambda (node env)
    (note-reference! node)
    node))

(define-expander 'call
  (lambda (node env)
    (let ((exp (node-form node)))
      (let ((proc-node (expand (car exp) env)))
	(note-operator! proc-node)
	(make-expanded node
		       (cons proc-node
			     (map (lambda (arg-exp)
				    (expand arg-exp env))
				  (cdr exp))))))))


; Special operators

(define-expander (list 'quote syntax-type)
  (lambda (node env)
    (set-expanded node)))

(define-expander (list 'lambda syntax-type)
  (lambda (node env)
    (set-fluid! $inferior-lambdas? #t)
    (let-fluid $inferior-lambdas? #f
      (lambda ()
	(let* ((exp (node-form node))
	       (formals (cadr exp)))
	  (with-lexicals (normalize-formals formals) env
	    (lambda (env lexicals)
	      (let ((node (make-expanded node
					 (list (car exp)
					       formals
					       (expand-body (cddr exp) env)))))
		(if (not (fluid $inferior-lambdas?))
		    (node-set! node 'no-inferior-lambdas #t))
		node))))))))

(define with-lexicals
  (let ((operator/name (get-operator 'name)))
    (lambda (vars env proc)
      (let* ((lexicals (map make-lexical vars))
	     (var-nodes (map (lambda (formal lexical)
			       (let ((var-node
				      (make-node operator/name formal)))
				 (node-set! var-node 'lexical lexical)
				 var-node))
			     vars
			     lexicals))
	     (node (proc (bind vars var-nodes env) lexicals)))
	(node-set! node 'var-nodes var-nodes)
	node))))


(define-expander (list 'letrec syntax-type)
  (lambda (node env)
    (set-fluid! $inferior-lambdas? #t)	;foo
    (let* ((exp (node-form node))
	   (specs (cadr exp))
	   (body (cddr exp)))
      (with-lexicals (map car specs) env
	(lambda (env lexicals)
	  (let* ((specs (map (lambda (spec)
			       (list (car spec)
				     (expand (cadr spec) env)))
			     specs))
		 (node (make-expanded node
				      (list (car exp)
					    specs
					    (expand-body body env)))))
	    (if (and (every (lambda (spec)
			      (lambda-node? (cadr spec)))
			    specs)
		     (every (lambda (lexical)
			      (and (= (lexical-assignment-count lexical) 0)
				   (= (lexical-reference-count lexical)
				      (lexical-operator-count lexical))))
			    lexicals))
		(node-set! node 'pure-letrec #t))
	    node))))))

(define $inferior-lambdas? (make-fluid #t))

(define expand-body
  (let ((operator/letrec (get-operator 'letrec syntax-type)))
    (lambda (body env)
      (scan-body body
		 env
		 (lambda (defs exps)	;defs is a list of define nodes
		   (if (null? defs)
		       (expand-begin exps env)
		       (expand (make-node
				   operator/letrec
				   `(letrec ,(map (lambda (def)
						    (cdr (node-form def)))
						  defs)
				      ,@exps))
			       env)))))))

(define expand-begin
  (let ((op (get-operator 'begin syntax-type)))
    (lambda (exp-list env)
      (let ((nodes (map (lambda (exp) (expand exp env))
			exp-list)))
	(if (null? (cdr nodes))
	    (car nodes)
	    (set-expanded (make-node op (cons 'begin nodes))))))))

(define-expander (list 'set! syntax-type)
  (lambda (node env)
    (let ((exp (node-form node)))
      (let ((lhs (classify (cadr exp) env))
	    (rhs (expand (caddr exp) env)))
	(if (name-node? lhs)
	    (begin (if (node-ref lhs 'lexical)
		       (note-assignment! lhs)
		       (note-top-level-assignment! (node-form lhs)))
		   (make-expanded node (list (car exp) lhs rhs)))
	    (expand (syntax-error "invalid assignment" (node-form node))
		    env))))))

(define name-node? (node-predicate 'name 'leaf))

(define (name-node-binding node cenv)
  (or (node-ref node 'binding)
      (lookup cenv (node-form node))))

(define-expander (list 'define syntax-type)
  (lambda (node env)
    (let ((form (node-form node)))
      (make-expanded node
		 (list (car form)
		       (cadr form)
		       (expand (caddr form) env))))))

(define-expander (list 'if syntax-type)
  (lambda (node env)
    (let ((exp (node-form node)))
      (make-expanded node
		     (list (car exp)
			   (expand (cadr exp) env)
			   (expand (caddr exp) env)
			   (expand (cadddr exp) env))))))

(define-expander (list 'primitive-procedure syntax-type)
  (lambda (node env)
    (set-expanded node)))


; --------------------
; Expanded nodes

(define (make-expanded node form)
  (set-expanded (make-similar-node node form)))

(define (set-expanded node)
  (node-set! node 'expanded #t)
  node)

(define (already-expanded? node)
  (node-ref node 'expanded))

; --------------------
; Keep track of which defined top-level variables are assigned

(define $assignments (make-fluid #f))

(define (note-top-level-assignment! name)
  (let ((package+table (fluid $assignments)))
    (if package+table
	(if (generated? name)
	    (if (eq? (generated-env name) (car package+table))
		(table-set! (cdr package+table) (generated-symbol name) #t))
	    (table-set! (cdr package+table) name #t)))))


; --------------------
; Lexical information structures record the number of times that a
; variable is used.

(define (make-lexical name)
  (vector 0 0 0))

(define (lexical-accessor j)
  (lambda (lex)
    (vector-ref lex j)))

(define (lexical-incrementator j)
  (lambda (node)
    (let ((v (node-ref node 'lexical)))
      (if v
	  (vector-set! v j (+ (vector-ref v j) 1))))))

(define lexical-reference-count  (lexical-accessor 0))
(define lexical-operator-count   (lexical-accessor 1))
(define lexical-assignment-count (lexical-accessor 2))

(define note-reference!  (lexical-incrementator 0))
(define note-operator!   (lexical-incrementator 1))
(define note-assignment! (lexical-incrementator 2))

