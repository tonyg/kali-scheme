; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.

; Definitions are (<variable> . <value>) pairs, where <value> can be any
; Scheme value.  This code walks the values looking for sharing and for
; closures.  Shared values are collected in a list and additional definitions
; are introduced for the bindings in the environments of closures and for
; close-compiled versions of any primitives in non-call position.  References
; to closure-bound variables are replaced with references to the newly-created
; package variables.

(define (flatten-definitions definitions)
  (set! *shared* '())
  (set! *definitions* '())
  (set! *immutable-value-table* (make-value-table))
  (set! *closed-compiled-primitives* (make-symbol-table))
  (let loop ((defs definitions) (flat '()))
    (cond ((not (null? defs))
	   (let ((var (caar defs))
		 (value (cdar defs)))
	     (if (and (variable-set!? var)
		      (closure? value))
		 (let ((new (generate-top-variable (variable-name var))))
		   (loop `((,var . ,new)
			   (,new . ,value)
			   . ,defs)
			 flat))
		 (loop (cdr defs)
		       (cons (cons var (flatten-value value))
			     flat)))))
	  ((null? *definitions*)
	   (let ((forms (really-make-forms flat *shared*)))
	     (set! *shared* #f)       ; safety
	     (set! *closed-compiled-primitives* #f)
	     (set! *immutable-value-table* #f)
	     forms))
	  (else
	   (let ((defs *definitions*))
	     (set! *definitions* '())
	     (loop defs flat))))))

; <Definitions> is a list of (<variable> . <value>) pairs.
; <Shared> is a list of all shared objects, each of which must end up being
; bound to a variable.

(define (really-make-forms definitions shared)
  (for-each (lambda (defn)
	      (let ((var (car defn))
		    (shared (value-shared (cdr defn))))
		(if (and (not (variable-set!? var))
			 shared
			 (not (shared-variable shared)))
		    (set-shared-variable! shared var))))
	    definitions)
  (map definition->form
       (append definitions
	       (shared-values->definitions shared))))

(define variable-set!? (structure-ref forms variable-set!?))

(define (shared-values->definitions shared)
  (do ((shared shared (cdr shared))
       (defns '() (if (shared-variable (value-shared (car shared)))
		      defns
		      (let ((var (generate-top-variable #f)))
			(set-shared-variable! (value-shared (car shared)) var)
			(cons (cons var (car shared)) defns)))))
      ((null? shared)
       defns)))

(define (definition->form definition)
  (let* ((var (car definition))
	 (value (cdr definition))
	 (shared (value-shared value))
	 (value (if (or (not shared)
			(eq? var (shared-variable shared)))
		    value
		    (shared-variable shared)))
	 (clean (clean-value! value)))
    ((structure-ref forms make-form)
       var
       (if (or (node? clean)
	       (variable? clean))
	   clean
	   (make-literal-node clean))
       (if (closure? value)              
	   (cdr (shared-saved (closure-temp value))) ; free vars
	   (stored-value-free-vars clean)))))

(define (make-literal-node value)
  (make-node op/literal value))

(define (make-name-node value)
  (make-node op/name value))

(define *shared* '())

(define (add-shared! value)
  (set! *shared* (cons value *shared*)))

(define *definitions* '())

(define (add-package-definition! value id)
  (let ((var (generate-top-variable id)))
    (set! *definitions*
	  (cons (cons var value)
		*definitions*))
    var))

(define (generate-top-variable maybe-id)
  (let ((var (make-global-variable (concatenate-symbol
				      (if maybe-id
					  (schemify maybe-id)
					  'top.)
				      (next-top-id))
				   type/undetermined)))
    (set-variable-flags! var
			 (cons 'generated-top-variable
			       (variable-flags var)))
    var))

(define *next-top-id* 0)

(define (next-top-id)
  (let ((id *next-top-id*))
    (set! *next-top-id* (+ 1 *next-top-id*))
    id))

(define (generated-top-variable? var)
  (memq? 'generated-top-variable (variable-flags var)))

(define (stored-value-free-vars value)
  (let ((vars '()))
    (let label ((value value))
      (cond ((variable? value)
	     (cond ((not (variable-flag value))
		    (set-variable-flag! value #t)
		    (set! vars (cons value vars)))
		   (else
		    ;(breakpoint "marked variable")  ; why did I care?
		    (values))))
	    ((vector? value)
	     (do ((i 0 (+ i 1)))
		 ((= i (vector-length value)))
	       (label (vector-ref value i))))
	    ((pair? value)
	     (label (car value))
	     (label (cdr value)))))
    (for-each (lambda (var)
		(set-variable-flag! var #f))
	      vars)
    vars))

;----------------------------------------------------------------
; Finding shared data structures.

(define-record-type shared
  ()
  (saved
   (shared? #f)
   (variable #f)))

(define make-shared shared-maker)

(define (value-shared value)
  (cond ((pair? value)
	 (car value))
	((vector? value)
	 (if (= 0 (vector-length value))
	     #f
	     (vector-ref value 0)))
	((closure? value)
	 (closure-temp value))
	(else
	 #f)))

(define (clean-value! value)
  (cond ((pair? value)
	 (cons (clean-sub-value! (shared-saved (car value)))
	       (clean-sub-value! (cdr value))))
	((vector? value)
	 (if (= 0 (vector-length value))
	     value
	     (let ((new (make-vector (vector-length value))))
	       (vector-set! new 0 (clean-sub-value!
				   (shared-saved (vector-ref value 0))))
	       (do ((i 1 (+ i 1)))
		   ((= i (vector-length value)))
		 (vector-set! new i (clean-sub-value! (vector-ref value i))))
	       new)))
	((closure? value)
	 (car (shared-saved (closure-temp value)))) ; flattened version of node
	((node? value)
	 (if (name-node? value)
	     (name-node->variable value)
	     (bug "bad definition value: ~S" value)))
	(else
	 value)))

(define name-node? (node-predicate 'name))

(define (clean-sub-value! value)
  (cond ((pair? value)
	 (let ((shared (car value)))
	   (cond ((shared-shared? shared)
		  (shared-variable shared))
		 (else
		  (set-car! value (clean-sub-value! (shared-saved shared)))
		  (set-cdr! value (clean-sub-value! (cdr value)))
		  value))))
	((vector? value)
	 (cond ((= 0 (vector-length value))
		value)
	       ((shared-shared? (vector-ref value 0))
		(shared-variable (vector-ref value 0)))
	       (else
		(vector-set! value 0 (clean-sub-value!
				      (shared-saved (vector-ref value 0))))
		(do ((i 1 (+ i 1)))
		    ((= i (vector-length value)))
		  (vector-set! value i (clean-sub-value! (vector-ref value i))))
		value)))
	((closure? value)
	 (shared-variable (closure-temp value)))
	(else
	 value)))

(define (flatten-value value)
  (cond ((immutable? value)
	 (flatten-immutable-value value))
	((primitive? value)
	 (primitive->name-node value))
	(else
	 (flatten-value! value)
	 value)))

(define (flatten-value! value)
  (cond ((pair? value)
	 (check-shared! (car value) flatten-pair! value))
	((vector? value)
	 (if (not (= 0 (vector-length value)))
	     (check-shared! (vector-ref value 0) flatten-vector! value)))
	((closure? value)
	 (check-shared! (closure-temp value) flatten-closure! value))))

(define (check-shared! shared flatten! value)
  (cond ((not (shared? shared))
	 (flatten! value))
	((not (shared-shared? shared))
	 (set-shared-shared?! shared #t)
	 (add-shared! value))))

(define *immutable-value-table* #f)

(define (flatten-immutable-value value)
  (cond ((pair? value)
	 (or (shared-immutable-value value car)
	     (let ((p (cons (car value) (cdr value))))
	       (table-set! *immutable-value-table* value p)
	       (flatten-pair! p)
	       p)))
	((vector? value)
	 (if (= 0 (vector-length value))
	     value
	     (or (shared-immutable-value value (lambda (x) (vector-ref x 0)))
		 (let ((v (copy-vector value)))
		   (table-set! *immutable-value-table* value v)
		   (flatten-vector! v)
		   v))))
	; no immutable closures
	(else
	 value)))  ; no sub-values

(define (shared-immutable-value value accessor)
  (cond ((table-ref *immutable-value-table* value)
	 => (lambda (copy)
	      (cond ((not (shared-shared? (accessor copy)))
		     (set-shared-shared?! (accessor copy) #t)
		     (add-shared! copy)
		     copy))))
	(else
	 #f)))

(define (flatten-pair! pair)
  (let ((temp (car pair))
	(shared (make-shared)))
    (set-car! pair shared)
    (set-shared-saved! shared (flatten-value temp))
    (set-cdr! pair (flatten-value (cdr pair)))))

(define (flatten-vector! vector)
  (let ((temp (vector-ref vector 0))
	(shared (make-shared)))
    (vector-set! vector 0 shared)
    (set-shared-saved! shared (flatten-value temp))
    (do ((i 1 (+ i 1)))
	((= i (vector-length vector)))
      (vector-set! vector i (flatten-value (vector-ref vector i))))))

; Make top-level definitions for the bindings in the closure and then substitute
; the defined variables within the closure's code.  The define variables are
; saved in the bindings in case they are shared with other closures (both for
; efficiency and because SET! requires it).

(define (flatten-closure! closure)
  (let ((shared (make-shared)))
    (for-each flatten-closure-binding! (closure-env closure))
    (set-closure-temp! closure shared)
    (set-shared-shared?! shared #t)     ; closures always need definitions
    (add-shared! closure)
    (receive (exp free)
	(substitute-in-expression (closure-node closure))
      (set-shared-saved! shared (cons exp free))
      (for-each clear-closure-binding! (closure-env closure)))))

(define (clear-closure-binding! pair)
  (node-set! (car pair) 'substitute #f))

; PAIR is (<name-node> . <value>) if it hasn't been seen before and
; (<name-node> . <substitute-name-node>) if it has.

(define (flatten-closure-binding! pair)
  (let* ((name (car pair))
	 (subst (if (node? (cdr pair))
		    (cdr pair)
		    (let ((subst (make-name-node-subst name (cdr pair))))
		      (set-cdr! pair subst)
		      subst))))
    (node-set! name 'substitute subst)))

(define (make-name-node-subst name value)		      
  (let ((var (add-package-definition! value (node-form name)))
	(subst (make-similar-node name (node-form name))))
    (node-set! subst 'binding (make-binding #f var #f))
    subst))

(define op/literal (get-operator 'literal))
(define op/name    (get-operator 'name))

;----------------------------------------------------------------
(define *closed-compiled-primitives* #f)

(define (make-primitive-node primitive call?)
  (if (and call?
	   (primitive-expands-in-place? primitive))
      (make-node op/primitive primitive)
      (let ((name-node (primitive->name-node primitive)))
	(note-variable-use! (name-node->variable name-node))
	name-node)))

(define (name-node->variable name-node)
  (let ((binding (node-ref name-node 'binding)))
    (cond ((not (binding? binding))
	   (bug "unbound variable ~S" (node-form name-node)))
	  ((primitive? (binding-static binding))
	   (primitive->name-node (binding-static binding)))
	  (else
	   (binding-place binding)))))

(define (primitive->name-node primitive)
  (let ((id (primitive-id primitive)))
    (or (table-ref *closed-compiled-primitives* id)
	(let* ((var (add-package-definition!
		     (make-top-level-closure
		      (expand (primitive-source primitive)
			      prescheme-compiler-env))
		     id))
	       (binding (make-binding #f var #f))
	       (node (make-node op/name id)))
	  (node-set! node 'binding (make-binding #f var #f))
	  (table-set! *closed-compiled-primitives* id node)
	  (set-variable-flags! var (cons 'closed-compiled-primitive
					 (variable-flags var)))
	  node))))

(define op/primitive (get-operator 'primitive))

;----------------------------------------------------------------

(define max-key-depth 5)

(define (value-table-hash-function obj)
  (let recur ((obj obj) (depth 0))
    (cond ((= depth max-key-depth)
	   0)
	  ((symbol? obj) (string-hash (symbol->string obj)))
	  ((integer? obj)
	   (if (< obj 0) (- -1 obj) obj))
	  ((char? obj) (+ 333 (char->integer obj)))
	  ((eq? obj #f) 3001)
	  ((eq? obj #t) 3003)
	  ((null? obj) 3005)
	  ((pair? obj)
	   (+ 3007
	      (recur (car obj) (- depth 1))
	      (* 3 (recur (cdr obj) (- depth 1)))))
	  ((vector? obj)
	   (let loop ((i 0) (hash (+ 3009 (vector-length obj))))
	     (if (or (= i (vector-length obj))
		     (= 0 (- depth i)))
		 hash
		 (loop (+ i 1) (+ hash (* i (recur (vector-ref obj i)
						   (- depth i))))))))
	  (else (error "value cannot be used as a table key" obj)))))

(define make-value-table
  (make-table-maker eq? value-table-hash-function))
