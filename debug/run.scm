; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; A little interpreter.

(define (eval form p)
  (run-forms (list form) p #f))

; Programs

(define (run-forms forms p file)
  (let ((nodes (scan-forms forms p file)))
    (if (not (null? nodes))
	(let-fluid $source-file-name file
	  (lambda ()
	    (let ((env (package->environment p)))
	      (do ((nodes nodes (cdr nodes)))
		  ((null? (cdr nodes))
		   (run-processed-form (car nodes) env))
		(run-processed-form (car nodes) env))))))))

(define (run-processed-form node env)
  (if (define-node? node)
      (let* ((form (node-form node))
	     (loc (cdr (env (cadr form))))
	     (value (run (caddr form) env)))
	(set-location-defined?! loc #t)
	(set-contents! loc value))
      (run node env)))

(define define-node? (node-predicate 'define))


; Main dispatch for a single expression.

(define (run exp env)
  (let ((node (classify exp env)))
    ((operator-table-ref interpreters (node-operator-id node)) node env)))

(define interpreters
  (make-operator-table (lambda (node env)
			 (run-call (node-form node) env))))

(define (define-interpreter name proc)
  (operator-define! interpreters name proc))

(define-interpreter 'name
  (lambda (node env)
    (let ((binding (name-node-binding node env)))
      (cond ((binding? binding)		;(type . location)
	     (if (and (compatible-types? (binding-type binding) 'value)
		      (location? (binding-place binding)))
		 (let ((loc (cdr binding)))
		   (if (location-defined? loc)
		       (contents loc)
		       (error "uninitialized variable" (schemify node))))
		 (error "invalid variable reference" (schemify node))))
	    ((unbound? binding)
	     (error "unbound variable" (schemify node)))
	    (else
	     (error "peculiar binding" node binding))))))

(define (name-node-binding node env)
  (or (node-ref node 'binding)
      (lookup env (node-form node))))

(define-interpreter 'literal
  (lambda (node env)
    (node-form node)))

(define-interpreter 'call
  (lambda (node env)
    (run-call (node-form node) env)))

(define (run-call exp env)
  (let ((proc (run (car exp) env)))	;Doing this first aids debugging
    (apply proc
	   (map (lambda (arg-exp)
		  (run arg-exp env))
		(cdr exp)))))

(define-interpreter '(quote syntax)
  (lambda (node env)
    (cadr (node-form node))))

(define-interpreter '(lambda syntax)
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


(define-interpreter '(begin syntax)
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

(define-interpreter '(set! syntax)
  (lambda (node env)
    (let* ((exp (node-form node))
	   (probe (name-node-binding (classify (cadr exp) env) env)))
      (cond ((and (binding? probe)
		  (location? (cdr probe)))
	     (if (and (location-defined? (binding-place probe))
		      (variable-type? (binding-type probe)))
		 (set-contents! (cdr probe) (run (caddr exp) env))
		 (error "invalid assignment" (schemify node))))
	    ((unbound? probe) (error "unbound variable" exp))
	    (else (error "peculiar assignment" exp))))))

(define-interpreter '(if syntax)
  (lambda (node env)
    (let ((exp (node-form node)))
      (if (null? (cdddr exp))
	  (if (run (cadr exp) env)
	      (run (caddr exp) env))	;hack
	  (if (run (cadr exp) env)
	      (run (caddr exp) env)
	      (run (cadddr exp) env))))))

; (reverse specs) in order to try to catch unportabilities

(define-interpreter '(letrec syntax)
  (lambda (node env)
    (let ((exp (node-form node)))
      (run-letrec (cadr exp) (cddr exp) env))))

(define (run-letrec specs body env)
  (let* ((bindings (map (lambda (spec)
			  (make-binding usual-variable-type
					(make-undefined-location (car spec))))
			specs))
	 (env (bind (map car specs)
		    bindings
		    env)))
    (for-each (lambda (binding val)
		(set-location-defined?! (cdr binding) #t)
		(set-contents! (cdr binding) val))
	      bindings
	      (map (lambda (spec) (run (cadr spec) env)) specs))
    (run-body body env)))


(let ((bad (lambda (node env)
	     (error "not valid in expression context" (node-form node)))))
  (define-interpreter '(define syntax) bad)
  (define-interpreter '(define-syntax syntax) bad))


; Primitive procedures

(define-interpreter '(primitive-procedure syntax)
  (lambda (node env)
    (let ((name (cadr (node-form node))))
      (or (table-ref primitive-procedures name)
	  (lambda args
	    (error "unimplemented primitive procedure" name))))))

(define primitive-procedures (make-table))

(define (define-a-primitive name proc)
  (table-set! primitive-procedures name proc)
  (define-interpreter name
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
    (bind1 name (make-binding usual-variable-type loc) env)))

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


; LOAD  (copied from mini-eval.scm)

(define (load filename . package-option)
  (let ((package (if (null? package-option)
		     (interaction-environment)
		     (car package-option))))
    (call-with-input-file filename
      (lambda (port)
	(let ((out (current-output-port)))
	  (display filename out) (force-output out)
	  (let loop ()
	    (let ((form (read port)))
	      (cond ((eof-object? form))
		    (else
		     (run-forms (list form) package filename)
		     (loop)))))
	  (newline out))))))


(define (eval-from-file forms p file)	;Scheme 48 internal thing
  (run-forms forms p file))


; (scan-structures (list s) (lambda (p) #t) (lambda (stuff) #f))
