; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; These are the four entry points (cf. rts/eval.scm):

; EVAL

(define (eval form package)
  (compile-and-run (list form) package #f))

; LOAD-INTO - load file into package.

(define (load-into filename package)
  (compile-and-run (read-forms filename package #f)
		   package
		   filename))

; Evaluate forms as if they came from the given file.

(define (eval-from-file forms package filename)
  (if filename
      ((fluid-cell-ref $note-file-package)
        filename package))
  (compile-and-run forms package filename))

; LOAD

(define (load filename . package-option)
  (let ((package (if (null? package-option)
		     (interaction-environment)
		     (car package-option))))
    (load-into filename package)))

;----------------

(define (compile-and-run forms package maybe-filename)
  (let* ((env (if maybe-filename
		  (bind-source-file-name maybe-filename
					 (package->environment package))
		  (package->environment package)))
	 (nodes (map (lambda (form)
		       (expand-scanned-form form env))
		     (scan-forms forms env))))
    (if (not (null? nodes))
	(run-nodes nodes env))))

(define (run-nodes nodes env)
  (do ((nodes nodes (cdr nodes)))
      ((null? (cdr nodes))
       (run-node (car nodes) env))
    (run-node (car nodes) env)))

(define (run-node node env)
  (cond ((define-node? node)
	 (let* ((form (node-form node))
		(loc (binding-place (lookup env (cadr form))))
		(value (run (caddr form) env)))
	   (set-location-defined?! loc #t)
	   (set-contents! loc value)))
	((not (define-syntax-node? node))
	 (run node env))))

; Main dispatch for a single expression.

(define (run node env)
  ((operator-table-ref interpreters (node-operator-id node))
     node
     env))

(define interpreters
  (make-operator-table (lambda (node env)
			 (run-call (node-form node) env))))

(define (define-interpreter name type proc)
  (operator-define! interpreters name type proc))

(define-interpreter 'name #f
  (lambda (node env)
    (let ((binding (name-node-binding node env)))
      (cond ((binding? binding)
	     (if (and (compatible-types? (binding-type binding) value-type)
		      (location? (binding-place binding)))
		 (let ((loc (binding-place binding)))
		   (if (location-defined? loc)
		       (contents loc)
		       (error "uninitialized variable" (schemify node env))))
		 (error "invalid variable reference" (schemify node env))))
	    ((unbound? binding)
	     (error "unbound variable" (schemify node env)))
	    (else
	     (error "peculiar binding" node binding))))))

(define (name-node-binding node env)
  (or (node-ref node 'binding)
      (lookup env (node-form node))))

(define-interpreter 'literal #f
  (lambda (node env)
    (node-form node)))

(define-interpreter 'call #f
  (lambda (node env)
    (run-call (node-form node) env)))

(define (run-call exp env)
  (let ((proc (run (car exp) env)))	;Doing this first aids debugging
    (apply proc
	   (map (lambda (arg-exp)
		  (run arg-exp env))
		(cdr exp)))))

(define-interpreter 'quote syntax-type
  (lambda (node env)
    (cadr (node-form node))))

(define-interpreter 'lambda syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (make-interpreted-closure (cadr exp) (cddr exp) env))))

(define (make-interpreted-closure formals body env)
  (lambda args
    (run-body body (bind-vars formals args env))))

(define (run-body body env)
  (scan-body
      body
      env
      (lambda (defs exps)
	(if (null? defs)
	    (run-begin exps env)
	    (run-letrec (map (lambda (def) (cdr (node-form def))) defs)
			exps
			env)))))


(define-interpreter 'begin syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (run-begin (cdr exp) env))))

(define (run-begin exp-list env)
  (if (null? exp-list)
      (syntax-error "null begin")
      (let loop ((exp-list exp-list))
	(if (null? (cdr exp-list))
	    (run (car exp-list) env)
	    (begin (run (car exp-list) env)
		   (loop (cdr exp-list)))))))

(define-interpreter 'set! syntax-type
  (lambda (node env)
    (let* ((exp (node-form node))
	   (probe (name-node-binding (cadr exp) env)))
      (cond ((and (binding? probe)
		  (location? (binding-place probe)))
	     (if (and (location-defined? (binding-place probe))
		      (variable-type? (binding-type probe)))
		 (set-contents! (binding-place probe)
				(run (caddr exp) env))
		 (error "invalid assignment" (schemify node env))))
	    ((unbound? probe) (error "unbound variable" exp))
	    (else (error "peculiar assignment" exp))))))

(define-interpreter 'if syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (if (null? (cdddr exp))
	  (if (run (cadr exp) env)
	      (run (caddr exp) env))	;hack
	  (if (run (cadr exp) env)
	      (run (caddr exp) env)
	      (run (cadddr exp) env))))))

; (reverse specs) in order to try to catch unportabilities

(define-interpreter 'letrec syntax-type
  (lambda (node env)
    (let ((exp (node-form node)))
      (run-letrec (cadr exp) (cddr exp) env))))

(define (run-letrec specs body env)
  (let* ((bindings (map (lambda (spec)
			  (make-binding usual-variable-type
					(make-undefined-location (car spec))
					#f))
			specs))
	 (env (bind (map car specs)
		    bindings
		    env)))
    (for-each (lambda (binding val)
		(let ((loc (binding-place binding)))
		  (set-location-defined?! loc #t)
		  (set-contents! loc val)))
	      bindings
	      (map (lambda (spec) (run (cadr spec) env)) specs))
    (run-body body env)))


(let ((bad (lambda (node env)
	     (error "not valid in expression context" (node-form node)))))
  (define-interpreter 'define syntax-type bad)
  (define-interpreter 'define-syntax syntax-type bad))


; Primitive procedures

(define-interpreter 'primitive-procedure syntax-type
  (lambda (node env)
    (let ((name (cadr (node-form node))))
      (or (table-ref primitive-procedures name)
	  (lambda args
	    (error "unimplemented primitive procedure" name))))))

(define primitive-procedures (make-table))

(define (define-a-primitive name proc)
  (table-set! primitive-procedures name proc)
  (define-interpreter name any-procedure-type
    (lambda (node env)
      (apply proc (map (lambda (arg) (run arg env))
		       (cdr (node-form node)))))))

(define-a-primitive 'unspecific
  (lambda () (if #f #f)))		;For COND

(define-syntax define-some-primitives
  (syntax-rules ()
    ((define-some-primitives name ...)
     (begin (define-a-primitive 'name name) ...))))

(define-some-primitives
  + - * quotient remainder = <
  eq? car cdr cons
  pair?
  vector? vector-ref string? string-ref
  symbol?
  char<? char=?)


; --------------------
; Environments

(define (bind-var name arg env)
  (let ((loc (make-undefined-location name)))
    (set-location-defined?! loc #t)
    (set-contents! loc arg)
    (bind1 name (make-binding usual-variable-type loc #f) env)))

(define (bind-vars names args env)
  (cond ((null? names)
	 (if (null? args)
	     env
	     (error "too many arguments" args)))
	((not (pair? names))
	 (bind-var names args env))
	((null? args)
	 (error "too few arguments" names))
	(else
	 (bind-var (car names) (car args)
		   (bind-vars (cdr names) (cdr args) env)))))

; (scan-structures (list s) (lambda (p) #t) (lambda (stuff) #f))
