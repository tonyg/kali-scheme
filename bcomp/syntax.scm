; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Syntactic stuff: transforms and operators.



(define usual-operator-type
  (procedure-type any-arguments-type value-type #f))

; --------------------
; Operators (= special operators and primitives)

(define-record-type operator :operator
  (make-operator type nargs uid name)
  operator?
  (type operator-type set-operator-type!)
  (nargs operator-nargs)
  (uid operator-uid)
  (name operator-name))

(define-record-discloser :operator
  (lambda (s)
    (list 'operator
	  (operator-name s)
	  (type->sexp (operator-type s) #t))))

(define (get-operator name . type-option)
  (let ((type (if (null? type-option) #f (car type-option)))
	(probe (table-ref operators-table name)))
    (if (operator? probe)
	(let ((previous-type (operator-type probe)))
	  (cond ((not type))
		((symbol? type)		; 'leaf or 'internal
		 (if (not (eq? type previous-type))
		     (warn "operator type inconsistency" name type previous-type)))
		((same-type? type previous-type))
		((subtype? type previous-type)
		 (signal 'note
			 "improving operator type"
			 name
			 (type->sexp previous-type 'foo)
			 (type->sexp type 'foo))
		 (set-operator-type! probe type))
		((not (subtype? previous-type type))
		 (warn "operator type inconsistency"
		       name
		       (type->sexp previous-type 'foo)
		       (type->sexp type 'foo))))
	  probe)
	(let* ((uid *operator-uid*)
	       (op (make-operator (or type
				      (begin (signal 'note
						     "default type for operator"
						     name)
					     usual-operator-type))
				  (if (and (not (symbol? type))
					   (fixed-arity-procedure-type? type))
				      (procedure-type-arity type)
				      #f)
				  uid
				  name)))
	  (if (>= uid number-of-operators)
	      (warn "too many operators" (operator-name op) (operator-type op)))
	  (set! *operator-uid* (+ *operator-uid* 1))
	  (table-set! operators-table (operator-name op) op)
	  (vector-set! the-operators uid op)
	  op))))

(define *operator-uid* 0)

(define operators-table (make-table))

(define number-of-operators 200)  ;Fixed-size limits bad, but speed good
(define the-operators (make-vector number-of-operators #f))

; --------------------
; Operator tables (for fast dispatch)

(define (make-operator-table default . mumble-option)
  (let ((v (make-vector number-of-operators default)))
    (if (not (null? mumble-option))
	(define-usual-suspects v (car mumble-option)))
    v))

(define operator-table-ref vector-ref)

(define (operator-lookup table op)
  (operator-table-ref table (operator-uid op)))

(define (operator-define! table name proc)
  (vector-set! table
	       (operator-uid (if (pair? name)
				 (get-operator (car name) (cadr name))
				 (get-operator name)))
	       proc))


; --------------------
; Nodes

(define-record-type node :node
  (really-make-node uid form plist)
  node?
  (uid node-operator-id)
  (form node-form)
  (plist node-plist set-node-plist!))

(define-record-discloser :node
  (lambda (n) (list (operator-name (node-operator n)) (node-form n))))

(define (make-node operator form)
  (really-make-node (operator-uid operator) form '()))

(define (node-ref node key)
  (let ((probe (assq key (node-plist node))))
    (if probe (cdr probe) #f)))

(define (node-set! node key value) ;gross
  (if value
      (let ((probe (assq key (node-plist node))))
	(if probe
	    (set-cdr! probe value)
	    (set-node-plist! node (cons (cons key value) (node-plist node)))))
      (let loop ((l (node-plist node)) (prev #f))
	(cond ((null? l) 'lose)
	      ((eq? key (caar l))
	       (if prev
		   (set-cdr! prev (cdr l))
		   (set-node-plist! node (cdr l))))
	      (else (loop (cdr l) l))))))

(define (node-operator node)
  (vector-ref the-operators (node-operator-id node)))


(define (node-predicate name . type-option)
  (let ((id (operator-uid (apply get-operator name type-option))))
    (lambda (node)
      (= (node-operator-id node) id))))

(define (make-similar-node node form)
  (if (equal? form (node-form node))
      node
      (make-node (node-operator node) form)))

; --------------------
; Generated names

; Generated names (more like syntactic closures, actually).
; The parent is always another name (perhaps generated).
; The parent chain provides an access path, should one ever be needed.
; That is: transform = (binding-static (lookup env parent)).

(define-record-type generated :generated
  (make-generated symbol token env parent-name)
  generated?
  (symbol      generated-symbol)
  (token       generated-token)
  (env	       generated-env)
  (parent-name generated-parent-name))

(define-record-discloser :generated
  (lambda (name) (list 'generated (generated-symbol name))))

(define (generate-name symbol env parent-name)    ;for opt/inline.scm
  (make-generated symbol #f env parent-name))

(define (name->symbol name)
  (if (symbol? name)
      name
      (let ((uid *generated-uid*))
	(set! *generated-uid* (+ *generated-uid* 1))
	(string->symbol (string-append (symbol->string (generated-symbol name))
				       "##"
				       (number->string uid))))))
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
	((operator? thing)
	 (warn "operator in quotation" thing)
	 (operator-name thing))  ;Foo
	(else
	 (warn "invalid datum in quotation" thing)
	 thing)))

(define (make-immutable thing)
  (make-immutable! thing)
  thing)

; --------------------
; Transforms

; A transform represents a source-to-source rewrite rule: either a
; macro or an in-line procedure.

(define-record-type transform :transform
  (really-make-transform xformer env type aux-names source id)
  transform?
  (xformer   transform-procedure)
  (env	     transform-env)
  (type	     transform-type)
  (aux-names transform-aux-names)
  (source    transform-source)    ;for reification
  (id	     transform-id))

(define (make-transform thing env type source id)
  (let ((type (if (or (pair? type) (symbol? type))
		  (sexp->type type #t)
		  type)))
    (if (pair? thing)
	(really-make-transform (car thing) env type (cdr thing) source id)
	(really-make-transform thing env type #f source id))))

(define-record-discloser :transform
  (lambda (m) (list 'transform (transform-id m))))

(define (maybe-transform t exp env-of-use)
  (let* ((token (cons #f #f))
	 (new-env (bind-aliases token t env-of-use))
	 (rename (make-name-generator (transform-env t)
				      token
				      (node-form (car exp))))
	 (compare
	  (lambda (name1 name2)
	    (or (eqv? name1 name2)
		(and (name? name1)
		     (name? name2)
		     (same-denotation? (lookup new-env name1)
				       (lookup new-env name2)))))))
    (values ((transform-procedure t) exp rename compare)
	    new-env
	    token)))

(define (bind-aliases token t env-of-use)
  (let ((env-of-definition (transform-env t)))
    (if (procedure? env-of-definition)
	(lambda (name)
	  (if (and (generated? name)
		   (eq? (generated-token name) token))
	      (lookup env-of-definition (generated-symbol name))
	      (lookup env-of-use name)))
	env-of-use)))  ;Lose

(define (make-name-generator env token parent-name)
  (let ((alist '()))			;list of (symbol . generated)
    (lambda (symbol)
      (if (symbol? symbol)
	  (let ((probe (assq symbol alist)))
	    (if probe
		(cdr probe)
		(let ((new-name (make-generated symbol token env parent-name)))
		  (set! alist (cons (cons symbol new-name)
				    alist))
		  new-name)))
	  (error "non-symbol argument to rename procedure"
		 symbol parent-name)))))

(define (same-denotation? x y)
  (or (equal? x y)
      (and (binding? x)
	   (binding? y)
	   (eq? (binding-place x) (binding-place y)))))


; --------------------
; Bindings: the things that are returned by LOOKUP.

; Representation is #(type place operator-or-transform-or-#f).
; For top-level bindings, place is usually a location.

(define binding? vector?)
(define (binding-type b) (vector-ref b 0))
(define (binding-place b) (vector-ref b 1))
(define (binding-static b) (vector-ref b 2))

(define (set-binding-place! b place) (vector-set! b 1 place))

(define (really-make-binding type place static)
  (let ((b (make-vector 3 place)))
    (vector-set! b 0 type)
    (vector-set! b 2 static)
    b))

(define (make-binding type-or-static place . static-option)
  (cond ((not (null? static-option))
	 (really-make-binding type-or-static place (car static-option)))
	((operator? type-or-static)
	 (really-make-binding (operator-type type-or-static)
			      place type-or-static))
	((transform? type-or-static)
	 (really-make-binding (transform-type type-or-static)
			      place type-or-static))
	(else
	 (really-make-binding type-or-static place #f))))

(define (clobber-binding! b type place static)
  (vector-set! b 0 type)
  (if place
      (set-binding-place! b place))
  (vector-set! b 2 static))

(define (binding-transform b)
  (let ((foo (binding-static b)))
    (if (transform? foo) foo #f)))

; Return a binding that's similar to the given one, but has its type
; replaced with the given type.

(define (impose-type type b integrate?)
  (if (eq? type syntax-type)
      b
      (really-make-binding (if (eq? type undeclared-type)
			       (let ((type (binding-type b)))
				 (if (variable-type? type)
				     (variable-value-type type)
				     type))
			       type)
			   (binding-place b)
			   (if integrate?
			       (binding-static b)
			       #f))))

; Return a binding that's similar to the given one, but has any
; procedure integration or other static information removed.
; But don't remove static information for macros (or structures,
; interfaces, etc.)

(define (forget-integration b)
  (if (and (binding-static b)
	   (subtype? (binding-type b) any-values-type))
      (really-make-binding (binding-type b)
			   (binding-place b)
			   #f)
      b))

; --------------------
; Expression classifier.  Returns a node.

(define (classify form env)
  (cond ((node? form)
	 (if (and (name-node? form)
		  (not (node-ref form 'binding)))
	     (classify-name (node-form form) env)
	     form))
	((name? form)
	 (classify-name form env))
        ((pair? form)
	 (let ((op-node (classify (car form) env)))
	   (if (name-node? op-node)
	       (let ((probe (node-ref op-node 'binding)))
		 (if (binding? probe)
		     (let ((s (binding-static probe)))
		       (cond ((operator? s)
			      (classify-operator-form s op-node form env))
			     ((and (transform? s)
				   (eq? (binding-type probe) syntax-type))
			      ;; Non-syntax transforms (i.e. procedure
			      ;; integrations) get done by MAYBE-TRANSFORM-CALL.
			      (classify-macro-application
			               s (cons op-node (cdr form)) env))
			     (else
			      (classify-call op-node form env))))
		     (classify-call op-node form env)))
	       (classify-call op-node form env))))
	((literal? form)
	 (classify-literal form))
	(else
	 (classify (syntax-error "invalid expression" form) env))))

(define call-node? (node-predicate 'call 'internal))
(define name-node? (node-predicate 'name 'leaf))

(define classify-literal
  (let ((op (get-operator 'literal 'leaf)))
    (lambda (exp)
      (make-node op exp))))

(define classify-call
  (let ((operator/call (get-operator 'call 'internal)))
    (lambda (proc-node exp env)
      (make-node operator/call
		 (if (eq? proc-node (car exp))
		     exp		;+++
		     (cons proc-node (cdr exp)))))))

; An environment is a procedure that takes a name and returns one of
; the following:
;
;  1. A binding record.
;  2. A node, which is taken to be a substitution for the name.
;  3. Another name, meaning that the first name is unbound.  The name
;     returned will be a symbol even if the original name was generated.
;
; In case 1, CLASSIFY caches the binding as the node's BINDING property.
; In case 2, it simply returns the node.

(define (classify-name name env)
  (let ((binding (lookup env name)))
    (if (node? binding)
	binding
	(let ((node (make-node operator/name name)))
	  (if (not (unbound? binding))
	      (node-set! node 'binding binding))
	  node))))

(define operator/name (get-operator 'name 'leaf))

; Expand a macro or in-line procedure application

(define (classify-macro-application t form env-of-use)
  (classify-transform-application
       t form env-of-use
       (lambda () 
	 (classify (syntax-error "use of macro doesn't match definition"
				 (schemify form))
		   env-of-use))))


(define classify-transform-application
  (let ((operator/with-aliases (get-operator 'with-aliases syntax-type)))
    (lambda (t form env-of-use lose)
      (call-with-values (lambda () (maybe-transform t form env-of-use))
	(lambda (new-form new-env token)
	  (cond ((eq? new-form form)
		 (lose))
		((eq? new-env env-of-use)
		 (classify new-form new-env))
		(else
		 (make-node operator/with-aliases
			    `(with-aliases ,(car form)
					   ,token
					   ,new-form)))))))))

(define (maybe-transform-call proc-node node env)
  (if (name-node? proc-node)
      (let ((b (or (node-ref proc-node 'binding)
		   (lookup env (node-form proc-node)))))
	(if (binding? b)
	    (let ((s (binding-static b)))
	      (cond ((transform? s)
		     (classify-transform-application s
						     (node-form node)
						     env
						     (lambda () node)))
		    ;; ((operator? s) (make-node s (node-form node)))
		    (else node)))
	    node))
      node))


; --------------------
; Specialist classifiers for particular operators

(define (classify-operator-form op op-node form env)
  ((operator-table-ref classifiers (operator-uid op))
   op op-node form env))

(define classifiers
  (make-operator-table (lambda (op op-node form env)
			 (if (let ((nargs (operator-nargs op)))
			       (or (not nargs)
				   (= nargs (length (cdr form)))))
			     (make-node op (cons op-node (cdr form)))
			     (classify-call op-node form env)))))

(define (define-classifier name proc)
  (operator-define! classifiers (list name syntax-type) proc))

; Remove generated names from quotations.

(define-classifier 'quote
  (lambda (op op-node exp env)
    (make-node op (list op-node (desyntaxify (cadr exp))))))

; Convert one-armed IF to two-armed IF.

(define-classifier 'if
  (lambda (op op-node exp env)
    (make-node op
	       (cons op-node
		     (if (null? (cdddr exp))
			 (append (cdr exp) (list (unspecific-node)))
			 (cdr exp))))))

; Rewrite (define (name . vars) body ...)
;  as (define foo (lambda vars body ...)).

(define-classifier 'define
  (let ((operator/lambda (get-operator 'lambda syntax-type))
	(operator/unassigned (get-operator 'unassigned 
					   (proc () value-type)))) ;foo
    (lambda (op op-node form env)
      (make-node op
		 (let ((pat (cadr form)))
		   (cons op-node
			 (if (pair? pat)
			     (list (car pat)
				   (make-node operator/lambda
					      `(lambda ,(cdr pat)
						 ,@(cddr form))))
			     (list pat
				   (if (null? (cddr form))
				       (make-node operator/unassigned
						  `(unassigned))
				       (caddr form))))))))))

(define unspecific-node
  (let ((op (get-operator 'unspecific
			  (proc () unspecific-type))))
    (lambda ()
      (make-node op '(unspecific)))))


; For the module system:

(define-classifier 'structure-ref
  (lambda (op op-node form env)
    (let ((struct-node (classify (cadr form) env))
	  (lose (lambda ()
		  (classify (syntax-error "invalid structure reference" form)
			    env))))
      (if (and (name? (caddr form))
	       (name-node? struct-node))
	  (let ((b (node-ref struct-node 'binding)))
	    (if (and (binding? b) (binding-static b)) ; (structure? ...)
		(classify (generate-name (desyntaxify (caddr form))
					 (binding-static b)
					 (node-form struct-node))
			  env)
		(lose)))
	  (lose)))))

; Magical Scheme 48 internal thing, mainly for use by the
; DEFINE-PACKAGE macro.

(define-classifier '%file-name%
  (let ((operator/quote (get-operator 'quote syntax-type)))
    (lambda (op op-node form env)
      (make-node operator/quote `',(fluid $source-file-name)))))


; To do:
;  Check syntax of others special forms

; --------------------
; Environments

(define (lookup env name)
  (env name))

(define (bind1 name binding env)
  (lambda (a-name)
    (if (eq? a-name name)
	binding
	(lookup env a-name))))

; corollary

(define (bind names bindings env)
  (cond ((null? names) env)
	(else
	 (bind1 (car names)
		(car bindings)
		(bind (cdr names) (cdr bindings) env)))))

(define (bindrec names env->bindings env)
  (set! env (bind names
		  (env->bindings (lambda (a-name) (env a-name)))
		  env))
  env)


; --------------------
; Utilities

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


(define (syntax? d)
  (cond ((operator? d)
	 (eq? (operator-type d) syntax-type))
	((transform? d)
	 (eq? (transform-type d) syntax-type))
	(else #f)))

(define (name? thing)
  (or (symbol? thing)
      (generated? thing)))

(define unbound? name?)


; Name of file being compiled

(define $source-file-name (make-fluid #f))


; --------------------
; LET-SYNTAX and friends

(define (define-usual-suspects table mumble)

  (operator-define! table (list 'let-syntax syntax-type)
    (mumble (lambda (node env)
	      (let* ((form (node-form node))
		     (specs (cadr form)))
		(values (caddr form)
			(bind (map car specs)
			      (map (lambda (spec)
				     (make-binding syntax-type
						   (list 'let-syntax)
						   (process-syntax (cadr spec)
								   env
								   (car spec)
								   env)))
				   specs)
			      env))))))

  (operator-define! table (list 'letrec-syntax syntax-type)
    (mumble (lambda (node env)
	      (let* ((form (node-form node))
		     (specs (cadr form)))
		(values (caddr form)
			(bindrec (map car specs)
				 (lambda (new-env)
				   (map (lambda (spec)
					  (make-binding
					     syntax-type
					     (list 'letrec-syntax)
					     (process-syntax (cadr spec)
							     new-env
							     (car spec)
							     new-env)))
					specs))
				 env))))))

  (operator-define! table 'with-aliases
    (mumble (lambda (node env)
	      (let ((form (node-form node)))
		(values (cadddr form)
			(bind-aliases (caddr form)
				      (binding-static
				           (node-ref (cadr form) 'binding))
				      env)))))))

(define (process-syntax form env name env-or-whatever)
  (let ((thing ((evaluator-for-syntax env)
		;; Bootstrap kludge to macro expand SYNTAX-RULES
		(let ((probe (lookup env (car form))))
		  (if (and (binding? probe)
			   (binding-transform probe))
		      ((transform-procedure (binding-transform probe))
		       form (lambda (x) x) eq?)
		      form)))))
    (make-transform thing env-or-whatever syntax-type form name)))

(define (get-funny name)
  (lambda (env)
    (let ((binding (lookup env name)))
      (if (binding? binding)
	  (binding-static binding)
	  (error "no binding of funny name" env binding)))))

(define funny-name/evaluator-for-syntax
  (string->symbol ".evaluator-for-syntax."))

(define evaluator-for-syntax 
  (get-funny funny-name/evaluator-for-syntax))

;(define funny-name/environment-for-syntax
;  (string->symbol ".environment-for-syntax."))
;
;(define environment-for-syntax 
;  (get-funny funny-name/environment-for-syntax))


; --------------------
; The horror of internal defines

; The continuation argument to SCAN-BODY takes two arguments: a list
; of definition nodes, and a list of other things (nodes and
; expressions).

(define (scan-body forms env cont)
  (if (or (null? forms)
	  (null? (cdr forms)))
      (cont '() forms)			;+++ tiny compiler speedup?
      (scan-body-forms forms env '()
		       (lambda (defs exps env)
			 (cont defs exps)))))

(define (scan-body-forms forms env defs cont)
  (if (null? forms)
      (cont defs '() env)
      (let ((node (classify (car forms) env))
	    (forms (cdr forms)))
	(cond ((define-node? node)
	       (scan-body-forms forms
				(let ((name (cadr (node-form node))))
				  (bind1 name
					 ;; Shadow, and don't cache lookup
					 (make-node operator/name name)
					 env))
				(cons node defs)
				cont))
	      ((begin-node? node)
	       (scan-body-forms (cdr (node-form node))
				env
				defs
				(lambda (new-defs exps env)
				  (cond ((null? exps)
					 (scan-body-forms forms
							  env
							  new-defs
							  cont))
					((eq? new-defs defs)
					 (cont defs
					       (append exps forms)
					       env))
					(else (body-lossage node))))))
	      (else
	       (cont defs (cons node forms) env))))))

(define (body-lossage node)
  (syntax-error "definitions and expressions intermixed"
		(schemify node)))


(define begin-node? (node-predicate 'begin syntax-type))
(define define-node? (node-predicate 'define syntax-type))


; --------------------
; Flush nodes in favor of something a little more readable

(define (schemify node)
  (if (node? node)
      (or (node-ref node 'schemify)
	  (let ((form ((operator-table-ref schemifiers (node-operator-id node))
		       node)))
	    (node-set! node 'schemify form)
	    form))
      (desyntaxify node)))

(define schemifiers
  (make-operator-table (lambda (node)
			 (let ((form (node-form node)))
			   (if (list? form)
			       (map schemify form)
			       form)))))

(define (define-schemifier name proc)
  (operator-define! schemifiers name proc))

(define-schemifier 'name
  (lambda (node)
    (desyntaxify (node-form node))))

(define-schemifier 'quote
  (lambda (node) (list 'quote (cadr (node-form node)))))

(define-schemifier (list 'letrec syntax-type)
  (lambda (node)
    (let ((form (node-form node)))
      `(letrec ,(map (lambda (spec)
		       `(,(car spec) ,(schemify (cadr spec))))
		     (cadr form))
	 ,@(map schemify (cddr form))))))

; --------------------
; Variable types

(define (variable-type type)
  (list 'variable type))

(define (variable-type? type)
  (and (pair? type) (eq? (car type) 'variable)))
(define variable-value-type cadr)

; Used in two places:
; 1. GET-LOCATION checks to see if the context of use (either variable
;    reference or assignment) is compatible with the declared type.
; 2. CHECK-STRUCTURE checks to see if the reconstructed type is compatible
;    with any type declared in the interface.

(define (compatible-types? have-type want-type)
  (if (variable-type? want-type)
      (and (variable-type? want-type)
	   (same-type? (variable-value-type have-type)
		       (variable-value-type want-type)))
      (if (variable-type? have-type)
	  (meet? (variable-value-type have-type)
		 want-type))))


; Usual type for Scheme variables.

(define usual-variable-type (variable-type value-type))


(define undeclared-type ':undeclared)    ;cf. really-export macro
