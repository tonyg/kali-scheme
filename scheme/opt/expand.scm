; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Front end for Scheme 48 compilation, optimization, or whatever.

; Entry point for expanding single forms

(define (expand-form form p)
  (let-fluid $package-usages #f
    (lambda ()
      (expand-begin (scan-forms (list form) p #f)
		    (package->environment p)))))

; When used as an optimizer we want to get better type information.

(set-optimizer! 'expand
		(lambda (stuff p)
		  (really-expand-stuff stuff p #t)))

; When called on random code (say from the REPL) type information may
; need to be superceded so we can't save it.

(set-optimizer! 'simple-expand
		(lambda (stuff p)
		  (really-expand-stuff stuff p #f)))

(define (expand-stuff stuff p . maybe-update?)
  (really-expand-stuff stuff p (if (null? maybe-update?)
				   #t
				   (car maybe-update?))))

; If were processing a package body, we change the types of all
; unassigned top-level variables from (VARIABLE <type>) to <type>.
;
; The forms are returned in a topologically sorted list.

(define (really-expand-stuff stuff p update?)
  (let* ((table (make-table))
	 (env (package->environment p))
	 (new-stuff
	  (let-fluid $package-usages (cons p table)
	    (lambda ()
	      (map (lambda (filename+nodes)
		     (let ((filename (car filename+nodes))
			   (nodes (cdr filename+nodes)))
		       (cons filename
			     (let ((env (bind-source-file-name filename env)))
			       (map (lambda (node)
				      (expand node env))
				    nodes)))))
		   stuff)))))
    (if update?
	(for-each (lambda (filename+nodes)
		    (for-each (lambda (node)
				(if (define-node? node)
				    (maybe-update-known-type node p table)))
			      (cdr filename+nodes)))
		  new-stuff))
    (sort-stuff new-stuff update?)))

(set-optimizer! 'expand expand-stuff)

(define (maybe-update-known-type node p table)
  (let* ((lhs (cadr (node-form node)))
	 (usage (get-package-usage table lhs)))
    (node-set! node 'usage usage)
    (if (= 0 (usage-assignment-count usage))
	(let ((new-type (reconstruct-type (caddr (node-form node))
					  (package->environment p))))
	  (if (subtype? new-type any-values-type)
	      (package-define! p lhs (if (subtype? new-type value-type)
					 new-type
					 value-type))
	      (warn "ill-typed right-hand side"
		    (schemify node)
		    (type->sexp new-type #t)))))))

(define lambda-node? (node-predicate 'lambda))
(define quote-node? (node-predicate 'quote))
(define literal-node? (node-predicate 'literal))
(define call-node? (node-predicate 'call))


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
						     (cdr form))))))
		       (lambda (frob)  ;for let-syntax, with-aliases, etc.
			 (lambda (node env)
			   (call-with-values (lambda () (frob node env))
			     (lambda (forms env)
			       (expand-body forms env)))))))

(define (define-expander name type proc)
  (operator-define! expanders name type proc))

(define (get-expander id)
  (operator-table-ref expanders id))

(define-expander 'literal #f
  (lambda (node env)
    (set-expanded node)))

(define-expander 'name #f
  (lambda (node env)
    (note-reference! node)
    node))

(define-expander 'call #f
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

(define-expander 'quote syntax-type
  (lambda (node env)
    (set-expanded node)))

(define-expander 'lambda syntax-type
  (lambda (node env)
    (set-fluid! $inferior-lambdas? #t)
    (let-fluid $inferior-lambdas? #f
      (lambda ()
	(let* ((exp (node-form node))
	       (formals (cadr exp)))
	  (with-usages (normalize-formals formals) env
	    (lambda (env usages)
	      (let ((node (make-expanded node
					 (list (car exp)
					       formals
					       (expand-body (cddr exp) env)))))
		(if (not (fluid $inferior-lambdas?))
		    (node-set! node 'no-inferior-lambdas #t))
		node))))))))

(define with-usages
  (let ((operator/name (get-operator 'name)))
    (lambda (vars env proc)
      (let* ((usages (map make-usage vars))
	     (var-nodes (map (lambda (formal usage)
			       (let ((var-node
				      (make-node operator/name formal)))
				 (node-set! var-node 'usage usage)
				 var-node))
			     vars
			     usages))
	     (node (proc (bind vars var-nodes env) usages)))
	(node-set! node 'var-nodes var-nodes)
	node))))


(define-expander 'letrec syntax-type
  (lambda (node env)
    (set-fluid! $inferior-lambdas? #t)	;foo
    (let* ((exp (node-form node))
	   (specs (cadr exp))
	   (body (cddr exp)))
      (with-usages (map car specs) env
	(lambda (env usages)
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
		     (every (lambda (usage)
			      (and (= (usage-assignment-count usage) 0)
				   (= (usage-reference-count usage)
				      (usage-operator-count usage))))
			    usages))
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

(define-expander 'set! syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (let ((lhs (classify (cadr exp) env))
	    (rhs (expand (caddr exp) env)))
	(if (name-node? lhs)
	    (begin (note-assignment! lhs)
		   (make-expanded node (list (car exp) lhs rhs)))
	    (expand (syntax-error "invalid assignment" (node-form node))
		    env))))))

(define name-node? (node-predicate 'name 'leaf))

(define (name-node-binding node cenv)
  (or (node-ref node 'binding)
      (lookup cenv (node-form node))))

(define-expander 'define syntax-type
  (lambda (node env)
    (let ((form (node-form node)))
      (make-expanded node
		     (list (car form)
			   (cadr form)
			   (expand (caddr form) env))))))

(define-expander 'if syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (make-expanded node
		     (list (car exp)
			   (expand (cadr exp) env)
			   (expand (caddr exp) env)
			   (expand (cadddr exp) env))))))

(define-expander 'primitive-procedure syntax-type
  (lambda (node env)
    (set-expanded node)))

(define-expander 'define-syntax syntax-type
  (lambda (node env)
    (set-expanded node)))

(define-expander 'lap syntax-type  ; argh.  There needs to be a way to put
  (lambda (node env)               ; this in the assembler file.
    (set-expanded node)))

(define-expander 'loophole syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (make-expanded node
		     (list (car exp)
			   (cadr exp)         ; don't expand the type
			   (expand (caddr exp) env))))))

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
; Usage information structures record the number of times that a
; variable is used.

(define (make-usage name)
  (vector 0 0 0))

(define (usage-accessor j)
  (lambda (usage)
    (vector-ref usage j)))

(define (usage-incrementator j)
  (lambda (node)
    (let ((v (or (node-ref node 'usage)
		 (package-usage node))))
      (if v
	  (vector-set! v j (+ (vector-ref v j) 1))))))

(define usage-reference-count  (usage-accessor 0))
(define usage-operator-count   (usage-accessor 1))
(define usage-assignment-count (usage-accessor 2))

(define note-reference!  (usage-incrementator 0))
(define note-operator!   (usage-incrementator 1))
(define note-assignment! (usage-incrementator 2))

; --------------------
; Keep track of which defined top-level variables are assigned

(define $package-usages (make-fluid #f))

(define (package-usage node)
  (let ((package+table (fluid $package-usages)))
    (if (and (name-node? node)
	     package+table)
	(let ((name (node-form node)))
	  (if (generated? name)
	      (if (eq? (generated-env name) (car package+table))
		  (get-package-usage (cdr package+table) (generated-symbol name))
		  #f)
	      (get-package-usage (cdr package+table) name)))
	#f)))
	    
(define (get-package-usage table name)
  (or (table-ref table name)
      (let ((usage (make-usage name)))
	(table-set! table name usage)
	usage)))
