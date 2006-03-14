; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.

; Type checking nodes.

; Entry point

; Because NODE is not the car of a pair, this depends on lambdas not being
; coerceable and literal nodes being coerced in place (instead of having a
; call inserted).

(define (infer-definition-type node name)
  (set! *currently-checking* name)
  (let ((res (cond ((literal-node? node)
		    (infer-literal-type node name))
		   ((lambda-node? node)
		    (infer-type node 0))
		   ((name-node? node)
		    (get-global-type (binding-place (node-ref node 'binding))))
		   (else
		    (bug "definition value is not a value node ~S" node)))))
    (set! *currently-checking* #f)
    res))

(define (infer-literal-type node name)
  (let ((value (node-form node)))
    (cond ((vector? value)
	   (let ((uvar (make-uvar name -1)))
	     (do ((i 0 (+ i 1)))
		 ((>= i (vector-length value)))
	       (unify! uvar (type-check-thing (vector-ref value i)) value))
	     (make-pointer-type (maybe-follow-uvar uvar))))
	  (else
	   (infer-type node 0)))))

(define (type-check-thing thing)
  (if (variable? thing)
      (get-package-variable-type thing)
      (literal-value-type thing)))

(define literal-operator (get-operator 'literal))

(define (make-literal-node value)
  (make-node literal-operator value))

; Get the type of the variable - if it is a type-variable, then create a new
; one and relate the two; if it is a polymorphic pattern, instantiate it.

(define (get-package-variable-type var)
  (let ((rep (variable-type var)))
    (cond ((eq? rep type/undetermined)
	   (let ((type (make-uvar (variable-name var) -1)))
	     (set-variable-type! var type)
	     (set-uvar-source! type var)
	     type))
	  ((type-scheme? rep)
	   (instantiate-type-scheme rep -1))
	  (else
	   rep))))

; Exported

(define (get-variable-type var)
  (let ((rep (variable-type var)))
    (cond ((eq? rep type/undetermined)
	   (bug "lexically bound variable ~S has no type" var))
	  ((type-scheme? rep)
	   (instantiate-type-scheme rep -1))
	  (else
	   rep))))

;----------------------------------------------------------------

(define (infer-type node depth)
  (infer-any-type node depth #f))

(define (infer-any-type node depth return?)
  (let ((type ((operator-table-ref inference-rules (node-operator-id node))
	       node
	       depth
	       return?)))
    (set-node-type! node type)
    (maybe-follow-uvar type)))

(define inference-rules
  (make-operator-table
   (lambda (node depth return?)
     (error "no type inference for node ~S" node))))

(define (define-inference-rule name proc)
  (operator-define! inference-rules name #f proc))

(define-inference-rule 'literal
  (lambda (node depth return?)
    (infer-literal (node-form node) node)))

(define-inference-rule 'quote
  (lambda (node depth return?)
    (infer-literal (cadr (node-form node)) node)))

(define (infer-literal value node)
  (literal-value-type value))

(define (literal-value-type value)
  (or (maybe-literal-value-type value)
      (error "don't know type of literal ~S" value)))
      
(define (maybe-literal-value-type value)
  (cond ((boolean? value)
	 type/boolean)
	((char? value)
	 type/char)
	((and (integer? value)
	      (exact? value))
	 type/integer)
	((real? value)
	 type/float)
	((string? value)
	 type/string)
	(((structure-ref eval-node unspecific?) value)
	 type/unit)			; was type/null
	((input-port? value)
	 type/input-port)
	((output-port? value)
	 type/output-port)
	((external-value? value)
	 (external-value-type value))
	((external-constant? value)
         type/integer)
	(else
	 #f)))

(define-inference-rule 'unspecific
  (lambda (node depth return?)
    type/unit))

(define-inference-rule 'lambda
  (lambda (node depth return?)
    (let* ((uid (unique-id))
	   (exp (node-form node))
	   (var-types (map (lambda (name-node)
			     (initialize-name-node-type name-node uid depth))
			   (cadr exp)))
	   (result (infer-any-type (caddr exp) depth #t)))
      ; stash the return type
      (set-lambda-node-return-type! node result)
      (make-arrow-type var-types result))))
	
; Create a new type variable for VAR.

(define (initialize-name-node-type node uid depth)
  (let ((uvar (make-uvar (node-form node) depth uid)))
    (set-node-type! node uvar)
    (set-uvar-source! uvar node)
    uvar))

; Get the type of the variable - if it is a type-variable, then create a new
; one and relate the two; if it is a polymorphic pattern, instantiate it.
; How to pass the source?

(define-inference-rule 'name
  (lambda (node depth return?)
    (let ((type (if (node-ref node 'binding)
		    (get-global-type (binding-place (node-ref node 'binding)))
		    (node-type node))))
      (if (not type)
	  (bug "name node ~S has no type" node))
      (if (type-scheme? type)
	  (instantiate-type-scheme type depth)
	  type))))

(define-inference-rule 'primitive
  (lambda (node depth return?)
    (let ((type (get-global-type (cdr (node-form node)))))
      (if (type-scheme? type)
	  (instantiate-type-scheme type depth)
	  type))))

; If no type is present, create a type variable.

(define (get-global-type value)
  (if (location? value)
      (literal-value-type (contents value))
      (let ((has (maybe-follow-uvar (variable-type value))))
	(cond ((not (eq? has type/undetermined))
	       has)
	      (else
	       (let ((type (make-uvar (variable-name value) -1)))
		 (set-variable-type! value type)
		 (set-uvar-source! type value)
		 type))))))

(define-inference-rule 'set!
  (lambda (node depth return?)
    (let* ((exp (node-form node))
	   (type (infer-type (caddr exp) depth))
	   (binding (node-ref (cadr exp) 'binding)))
      (if (not binding)
	  (error "SET! on a local variable ~S" (schemify node)))
      (unify! type (variable-type (binding-place binding)) node)
      type/null)))

(define-inference-rule 'call
  (lambda (node depth return?)
    (rule-for-calls (node-form node) node depth return?)))

(define-inference-rule 'goto
  (lambda (node depth return?)
    (rule-for-calls (cdr (node-form node)) node depth return?)))

(define (rule-for-calls proc+args node depth return?)
  (let ((proc (car proc+args))
	(args (cdr proc+args)))
    (cond ((lambda-node? proc)
	   (rule-for-let node depth proc args return?))
	  ((primitive-node? proc)
	   (rule-for-primitives node depth (node-form proc) args return?))
	  (else
	   (rule-for-unknown-calls node depth proc+args return?)))))

(define name-node? (node-predicate 'name))
(define lambda-node? (node-predicate 'lambda))
(define literal-node? (node-predicate 'literal))
(define primitive-node? (node-predicate 'primitive))

(define (rule-for-let node depth proc args return?)
  (let ((depth (+ depth 1))
	(uid (unique-id))
	(proc (node-form proc)))
    (do ((names (cadr proc) (cdr names))
	 (vals args (cdr vals)))
	((null? names))
      (let ((type (schemify-type (infer-type (car vals) depth) depth)))
	(if (type-scheme? type)
	    (set-node-type! (car names) type)
	    (unify! (initialize-name-node-type (car names) uid depth)
		    type
		    node))))
    (infer-any-type (caddr proc) depth return?)))

(define (rule-for-primitives node depth primitive args return?)
  ((primitive-inference-rule primitive)
     args node depth return?))

(define (rule-for-unknown-calls node depth proc+args return?)
  (let ((proc-type (infer-type (car proc+args) depth))
	(arg-types (infer-types (cdr proc+args) depth))
	(return-type (if return?
			 (make-tuple-uvar 'result depth)
			 (make-uvar 'result depth))))
    (unify! proc-type
	    (make-arrow-type arg-types return-type)
	    node)
;    (if (= 244 (uvar-id return-type))
;	(breakpoint "rule-for-unknown-calls"))
    (maybe-follow-uvar return-type)))
  
(define (infer-types nodes depth)
  (map (lambda (node)
	 (infer-type node depth))
       nodes))

(define-inference-rule 'begin
  (lambda (node depth return?)
    (let loop ((exps (cdr (node-form node))) (type type/unit))
      (if (null? exps)
	  type
	  (loop (cdr exps)
		(infer-any-type (car exps)
				depth
				(or (not (null? (cdr exps)))
				    return?)))))))

(define-inference-rule 'if
  (lambda (node depth return?)
    (let ((args (cdr (node-form node))))
      (let ((test-type (infer-type (car args) depth))
	    (true-type (infer-any-type (cadr args) depth return?))
	    (false-type (infer-any-type (caddr args) depth return?)))
	(unify! test-type type/boolean node)
	(unify! true-type false-type node)
	true-type))))

; Unions haven't been completely implemented yet.
;
;(define-inference-rule 'type-case
;  (lambda (node depth return?)
;    (let ((args (cdr (node-form node))))
;      (let ((type-id (cadr (node-form (cadr args))))
;            (uvar (make-uvar 'v depth)))
;        (let ((union-type (make-pointer-type (get-union-type type-id)))
;              (cont-types (get-union-deconstruction-types type-id uvar)))
;          (check-arg-type args 0 union-type depth node)
;          (check-arg-types args 2 cont-types depth node)
;          uvar)))))

(define-inference-rule 'letrec
  (lambda (node depth return?)
    (let ((form (node-form node))
	  (depth (+ depth 1))
	  (uid (unique-id)))
      (let ((names (map car (cadr form)))
	    (vals (map cadr (cadr form))))
	(for-each (lambda (name)
		    (initialize-name-node-type name uid depth))
		  names)
	(do ((names names (cdr names))
	     (vals vals (cdr vals)))
	    ((null? names))
	  (if (not (lambda-node? (car vals)))
	      (error "LETREC value is not a LAMBDA: ~S" (schemify node)))
	  (unify! (infer-type (car vals) depth)
		  (node-type (car names))
		  node))
	(for-each (lambda (name)
		    (let ((type (schemify-type (node-type name) depth)))
		      (if (type-scheme? type)
			  (set-node-type! name type))))
		  names)
	(infer-any-type (caddr form) depth return?)))))

;--------------------------------------------------

(define (node-type node)
  (maybe-follow-uvar (node-ref node 'type)))

(define (set-node-type! node type)
  (node-set! node 'type type))

(define (lambda-node-return-type node)
  (node-ref node 'return-type))

(define (set-lambda-node-return-type! node type)
  (node-set! node 'return-type type))

;--------------------------------------------------
; Utility procedures used by the inferencers of the various primops.

; Check that the INDEX'th argument of CALL has type TYPE.

(define (check-arg-type args index type depth exp)
  (if (null? args)
      (begin
	(format #t "Wrong number of arguments in ~S~% " (schemify exp))
	(if *currently-checking*
	    (format #t "~% while reconstructing the type of '~S'" *currently-checking*))
	(error "type problem")))
  (unify! (infer-type (list-ref args index) depth)
	  type
	  exp))

