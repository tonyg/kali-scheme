; Copyright (c) 1994 Richard Kelsey.  See file COPYING.

; Type checking nodes.

; Hacked to propogate data-type information.

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
  (receive (type true false)
      (infer-tail-type node depth #f)
    type))

; This returns both a type for the expression and two lists of type assertions
; for variables.  Those in the first list apply if the expression evaluates to
; true and those that apply if the predicate evaluates to false.  An assertion
; has two parts: an expression E (which currently must be a variable) whose
; type is a union, and the set of variants of that union which the variable
; might have.  If NODE cannot have a particular boolean value the corresponding
; list of assertions is #F.

; RETURN? is true if NODE is syntactically tail-recursive in some procedure
; or a non-final expression in a body, in which case NODE is allowed to have
; a tuple type.

(define (infer-tail-type node depth return?)
  (receive (type true false)
      ((operator-table-ref inference-rules (node-operator-id node))
         node
	 depth
	 return?)
    (set-node-type! node type)
    (values (maybe-follow-uvar type) true false)))

; Return a true value: no true assertions and value is never false.

(define (true-value value)
  (values value '() #f))

(define (unknown-value value)
  (values value '() '()))

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
  (if value
      (true-value (or (maybe-literal-value-type value)
		      (error "don't know type of literal ~S" value)))
      (values type/boolean #f '())))  ; always false, but no actual assertions

(define (maybe-literal-value-type value)
  (cond ((boolean? value)
	 type/boolean)
	((char? value)
	 type/char)
	((integer? value)
	 type/integer)
	((string? value)
	 type/string)
	(((structure-ref eval-node unspecific?) value)
	 type/null)
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
    (unknown-value type/null)))

(define-inference-rule 'lambda
  (lambda (node depth return?)
    (let* ((uid (unique-id))
	   (exp (node-form node))
	   (var-types (map (lambda (name-node)
			     (initialize-name-node-type name-node uid depth))
			   (cadr exp)))
	   (result (infer-tail-type (caddr exp) depth #t)))
      ; stash the return type
      (set-lambda-node-return-type! node result)
      (true-value (make-arrow-type var-types result)))))
	
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
	  (unknown-value type)))))

(define-inference-rule 'primitive
  (lambda (node depth return?)
    (let ((type (get-global-type (cdr (node-form node)))))
      (true-value (if (type-scheme? type)
		      (instantiate-type-scheme type depth)
		      type)))))

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
      (unknown-value type/null))))

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
    (infer-tail-type (caddr proc) depth return?)))

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
    (unknown-value (maybe-follow-uvar return-type))))
  
(define (infer-types nodes depth)
  (map (lambda (node)
	 (infer-type node depth))
       nodes))

(define-inference-rule 'begin
  (lambda (node depth return?)
    (let ((exps (cdr (node-form node))))
      (if (null? exps)
	  (unknown-value type/unit)
	  (let loop ((exps exps))
	    (if (null? (cdr exps))
		(infer-tail-type (car exps) depth return?)
		(begin
		  (infer-tail-type (car exps) depth #t)
		  (loop (cdr exps)))))))))

; It would be nice if we could just try to unify the two arms and return
; type/unit if we lost, but unification has side-effects.

(define-inference-rule 'if
  (lambda (node depth return?)
    (let ((args (cdr (node-form node))))
      (receive (test-type test-true-asserts test-false-asserts)
	  (infer-tail-type (car args) depth #f)
	(unify! test-type type/boolean node)
	(receive (cons-type cons-true-asserts cons-false-asserts)
	    (with-assertions test-true-asserts
	      (lambda ()
		(infer-tail-type (cadr args) depth return?)))
	  (receive (alt-type alt-true-asserts alt-false-asserts)
	    (with-assertions test-false-asserts
	      (lambda ()
		(infer-tail-type (caddr args) depth return?)))
	    (values (cond ((eq? cons-type type/null)
			   alt-type)
			  ((eq? alt-type type/null)
			   cons-type)
			  (else
			   (unify! cons-type alt-type node)
			   cons-type))
		    (assertion-or (assertion-and test-true-asserts
						 cons-true-asserts)
				  (assertion-and test-false-asserts
						 alt-true-asserts))
		    (assertion-or (assertion-and test-true-asserts
						 cons-false-asserts)
				  (assertion-and test-false-asserts
						 alt-false-asserts)))))))))

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
	  (unify! (infer-type (car vals) depth)
		  (node-type (car names))
		  node))
	(for-each (lambda (name)
		    (let ((type (schemify-type (node-type name) depth)))
		      (if (type-scheme? type)
			  (set-node-type! name type))))
		  names)
	(infer-tail-type (caddr form) depth return?)))))

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

