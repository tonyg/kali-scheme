; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; A little interpreter.

(define (eval form p)
  (run-forms (list form) p #f))

; Programs

(define (run-forms forms p file)
  (let ((forms (scan-forms forms p file))
	(env (package->environment p)))
    (if (not (null? forms))
	(let-fluid $source-file-name file
	  (lambda ()
	    (do ((forms forms (cdr forms)))
		((null? (cdr forms))
		 (run-processed-form (car forms) env))
	      (run-processed-form (car forms) env)))))))

(define (run-processed-form form p)
  (if (define? form)
      (set-contents! (package-ensure-defined! p (define-lhs form))
		     (run (define-rhs form) p))
      (run form p)))

(define operator/define (get-operator 'define 'syntax))


; Main dispatch for a single expression.

(define (run exp env)
  (cond ((name? exp)
	 (run-variable exp env))
	((pair? exp)
	 (cond ((name? (car exp))
		(let ((den (lookup env (car exp))))
		  (cond ((operator? den)
			 ((interpreter-for-operator den) exp env))
			((transform? den)
			 (run (transform den exp (lambda (name)
						   (probe-env env name)))
			      env))
			(else
			 (run-call exp env)))))
	       ((operator? (car exp))  ;Provision for 2-pass compiler
	        ((interpreter-for-operator (car exp)) exp env))
	       (else
		(run-call exp env))))
        ((literal? exp) exp)
        ((location? exp) (contents exp))
        (else (error "invalid expression" exp))))

(define (run-variable exp env)
  (let ((probe (lookup-variable env exp)))
    (if (location? probe)
	(contents probe)
	(environment-ref (car probe) (cdr probe)))))

(define (run-call exp env)
  (let ((proc (run (car exp) env))) ;Doing this first aids debugging
    (apply proc
	   (map (lambda (arg-exp)
		  (run arg-exp env))
		(cdr exp)))))


; Special operators

(define interpreters
  (make-compilator-table (lambda (form env)
			   (run-call form env))))

(define define-interpreter (car interpreters))
(define interpreter-for-operator (cdr interpreters))

(define-interpreter 'quote 'syntax
  (lambda (exp env) (desyntaxify (cadr exp))))

(define-interpreter 'lambda 'syntax
  (lambda (exp env)
    (lambda args
      (run-body (cddr exp) (bind (cadr exp) args env)))))

(define (run-body body env)
  (scan-body
      body
      (lambda (name) (probe-env env name))
      (lambda (name) (lookup env name))
      (lambda (defs exps)
	(if (null? defs)
	    (run-begin exps env)
	    ;; *** not hygienic ***
	    (run `(letrec ,(map cdr defs) ,@exps)
		 env)))))

(define-interpreter 'begin 'syntax
  (lambda (exp env)
    (run-begin (cdr exp) env)))

(define (run-begin exp-list env)
  (if (null? exp-list)
      (syntax-error "null begin")
      (let loop ((exp-list exp-list))
	(if (null? (cdr exp-list))
	    (run (car exp-list) env)
	    (begin (run (car exp-list) env)
		   (loop (cdr exp-list)))))))

(define-interpreter 'set! 'syntax
  (lambda (exp env)
    (let ((probe (lookup-assigned env (cadr exp)))
	  (val (run (caddr exp) env)))
      (if (location? probe)
	  (set-contents! probe val)
	  (environment-set! (car probe) (cdr probe) val)))))

(define-interpreter 'if 'syntax
  (lambda (exp env)
    (if (null? (cdddr exp))
	(if (run (cadr exp) env)
	    (run (caddr exp) env)) ;hack
	(if (run (cadr exp) env)
	    (run (caddr exp) env)
	    (run (cadddr exp) env)))))

(let ((bad (lambda (form env)
	     (error "not valid in expression context" form))))
  (define-interpreter 'define 'syntax bad)
  (define-interpreter 'define-syntax 'syntax bad))


; Primitive procedures

;(define-interpreter 'primitive-procedure 'syntax
;  (lambda (expr env)
;    (or (table-ref primitive-procedures (cadr expr))
;        (lambda args
;          (error "unimplemented primitive procedure" expr)))))

(define-interpreter 'unassigned 'procedure
  (lambda (expr env)
    (check-nargs= expr 0)
    *unassigned*))

(define *unassigned* (list '*unassigned*))  ;For LETREC

(define-interpreter 'unspecific 'procedure
  (lambda (expr env) (if #f #f)))	;For COND

(define (define-a-primitive name proc)
  (define-interpreter name 'procedure
    (lambda (expr env)
      (apply proc (map (lambda (arg) (run arg env)) (cdr expr))))))

(define-syntax define-some-primitives
  (syntax-rules ()
    ((define-some-primitives name ...)
     (begin (define-a-primitive 'name name) ...))))

(define-some-primitives
  eq? car cdr cons
  vector-ref pair?)

(define (check-nargs= expr n)
  (if (not (= (length (cdr expr)) n))
      (error "wrong number of arguments to primitive" expr n)))


; Environments

(define (package->environment p) p) ;?

(define (lookup env name)
  (really-lookup env name package-lookup))

(define (probe-env env name)
  (really-lookup env name probe-package))


(define (lookup-variable env name)
  (really-lookup env name cons))

(define (lookup-assigned env name)
  (really-lookup env name cons))

(define (really-lookup env name p-lookup)
  (if (procedure? env)
      (env name p-lookup)
      (p-lookup env name)))

(define (bind1 name arg env)
  (let ((loc (make-undefined-location name)))
    (set-location-defined?! loc #t)
    (if (not (eq? arg *unassigned*))	;Hack for letrec
	(set-contents! loc arg))
    (lambda (a-name p-lookup)
      (if (eq? a-name name)
	  loc
	  (really-lookup env a-name p-lookup)))))

(define (bind names args env)
  (cond ((null? names)
	 (if (null? args)
	     env
	     (error "too many arguments" args)))
	((not (pair? names))
	 (bind1 names args env))
	((null? args)
	 (error "too few arguments" names))
	(else
	 (bind1 (car names) (car args)
		(bind (cdr names) (cdr args) env)))))


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


(define (eval-from-file forms p file)	;Scheme48 thing
  (run-forms forms p file))
