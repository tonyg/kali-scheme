; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; Evaluator for nodes.

; This doesn't handle n-ary procedures.

; (NAME-NODE-BINDING name-node) is used as an EQ? key in local environments,
; and passed as-is to the global-environment arguments.

; Exports:
; (EVAL-NODE node global-ref global-set! eval-primitive)
; CLOSURE? (CLOSURE-NODE closure) (CLOSURE-ENV closure)
; (UNSPECIFIC? thing)

(define (eval-node node global-ref global-set! eval-primitive)
  (eval node (make-env '()
		       (make-eval-data global-ref
				       global-set!
				       eval-primitive))))

(define-record-type eval-data :eval-data
  (make-eval-data global-ref global-set! eval-primitive)
  eval-data?
  (global-ref     eval-data-global-ref)
  (global-set!    eval-data-global-set!)
  (eval-primitive eval-data-eval-primitive))

; Environments

(define-record-type env :env
  (make-env alist eval-data)
  env?
  (alist env-alist)
  (eval-data env-eval-data))

(define (env-ref env name-node)
  (let ((cell (assq name-node (env-alist env))))
    (if cell
	(cdr cell)
	((eval-data-global-ref (env-eval-data env)) name-node))))

(define (env-set! env name-node value)
  (let ((cell (assq name-node (env-alist env))))
    (if cell
	(set-cdr! cell value)
	((eval-data-global-set! (env-eval-data env))
	 name-node
	 value))))

(define (extend-env env ids vals)
  (make-env (append (map cons ids vals)
		    (env-alist env))
	    (env-eval-data env)))

(define (eval-primitive primitive args env)
  ((eval-data-eval-primitive (env-eval-data env)) primitive args))

; Closures

(define-record-type closure :closure
  (make-closure node env)
  closure?
  (node closure-node)
  (env real-closure-env)
  (temp closure-temp set-closure-temp!))

(define (closure-env closure)                   ; exported
  (env-alist (real-closure-env closure)))

(define (make-top-level-closure exp)
  (make-closure exp the-empty-env))

(define the-empty-env (make-env '() #f))

; Main dispatch

(define (eval node env)
  ((operator-table-ref evaluators (node-operator-id node))
   node
   env))

; Particular operators

(define evaluators
  (make-operator-table
   (lambda (node env)
     (error "no evaluator for node ~S" node))))

(define (define-evaluator name proc)
  (operator-define! evaluators name #f proc))

(define (eval-list nodes env)
  (map (lambda (node)
	 (eval node env))
       nodes))

(define-evaluator 'literal
  (lambda (node env)
    (node-form node)))

(define-evaluator 'unspecific
  (lambda (node env)
    (unspecific)))

(define-evaluator 'unassigned
  (lambda (node env)
    (unspecific)))

(define-evaluator 'real-external
  (lambda (node env)
    (let* ((exp (node-form node))
	   (type (expand-type-spec (cadr (node-form (caddr exp))))))
      (make-external-value (node-form (cadr exp))
			   type))))

(define-evaluator 'quote
  (lambda (node env)
    (cadr (node-form node))))

(define-evaluator 'lambda
  (lambda (node env)
    (make-closure node env)))

(define (apply-closure closure args)
  (let ((node (closure-node closure))
	(env (real-closure-env closure)))
    (eval (caddr (node-form node))
	  (extend-env env (cadr (node-form node)) args))))

(define-evaluator 'name
  (lambda (node env)
    (env-ref env node)))

(define-evaluator 'set!
  (lambda (node env)
    (let ((exp (node-form node)))
      (env-set! env (cadr exp) (eval (caddr exp) env))
      (unspecific))))

(define-evaluator 'call
  (lambda (node env)
    (eval-call (car (node-form node))
	       (cdr (node-form node))
	       env)))

(define-evaluator 'goto
  (lambda (node env)
    (eval-call (cadr (node-form node))
	       (cddr (node-form node))
	       env)))

(define (eval-call proc args env)
  (let ((proc (eval proc env))
	(args (eval-list args env)))
    (if (closure? proc)
	(apply-closure proc args)
	(eval-primitive proc args env))))

(define-evaluator 'begin
  (lambda (node env)
    (let ((exps (cdr (node-form node)))) 
      (if (null? exps)
	  (unspecific)
	  (let loop ((exps exps))
	    (cond ((null? (cdr exps))
		   (eval (car exps) env))
		  (else
		   (eval (car exps) env)
		   (loop (cdr exps)))))))))

(define-evaluator 'if 
  (lambda (node env)
    (let* ((form (node-form node))
	   (test (cadr form))
	   (arms (cddr form)))
      (cond ((eval test env)
	     (eval (car arms) env))
	    ((null? (cdr arms))
	     (unspecific))
	    (else
	     (eval (cadr arms) env))))))

(define-evaluator 'loophole
  (lambda (node env)
    (eval (caddr (node-form node)) env)))

(define-evaluator 'letrec
  (lambda (node env)
    (let ((form (node-form node)))
      (let ((vars (map car (cadr form)))
	    (vals (map cadr (cadr form)))
	    (body (caddr form)))
	(let ((env (extend-env env
			       vars
			       (map (lambda (ignore)
				      (unspecific))
				    vars))))
	  (for-each (lambda (var val)
		      (env-set! env var (eval val env)))
		    vars
		    vals)
	  (eval body env))))))

(define (unspecific? x)
  (eq? x (unspecific)))

; Used by our clients but not by us.

(define (constant? x)
  (or (number? x)
      (symbol? x)
      (external-constant? x)
      (external-value? x)
      (boolean? x)))
