; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.

; Substituting new variables for old in expressions.

(define *free-exp-vars* #f)

(define (substitute-in-expression exp)
  (set! *free-exp-vars* '())
  (set! *letrec-datas* '())
  (let* ((exp (substitute-in-exp exp))
	 (free *free-exp-vars*))
    (set! *free-exp-vars* '())
    (for-each (lambda (var)
		(set-variable-flag! var #f))
	      free)
    (values exp free)))

(define global-marker (list 'global))

(define (note-binding-use! binding)
  (let ((var (binding-place binding)))
    (if (variable? var)
	(note-variable-use! var))))

(define (note-variable-use! var)
  (cond ((not (eq? (variable-flag var) global-marker))
	 (set! *free-exp-vars* (cons var *free-exp-vars*))
	 (set-variable-flag! var global-marker))))

; Main dispatch

(define (substitute-in-exp node)
  ((operator-table-ref substitutions (node-operator-id node))
   node))

; Particular operators

(define substitutions
  (make-operator-table
   (lambda (node)
     (error "no substitution for node ~S" node))))

(define (default-substitution node)
  (make-similar-node node
		     (cons (car (node-form node))
			   (map substitute-in-exp (cdr (node-form node))))))

(define (define-substitution name proc)
  (operator-define! substitutions name #f proc))

(define-substitution 'literal identity)
(define-substitution 'quote identity)
(define-substitution 'unspecific identity)

(define-substitution 'real-external
  (lambda (node)
    (let* ((exp (node-form node))
	   (type (expand-type-spec (cadr (node-form (caddr exp))))))
      (make-literal-node (make-external-value (node-form (cadr exp))
					      type)))))

(define op/literal (get-operator 'literal))

(define (make-literal-node x)
  (make-node op/literal x))

; We copy the names because the same node may occur in multiple places
; in the tree.

(define-substitution 'lambda
  (lambda (node)
    (let* ((new-names (copy-names (cadr (node-form node))))
	   (body (substitute-in-exp (caddr (node-form node)))))
      (make-similar-node node
			 (list (car (node-form node))
			       new-names
                               body)))))

(define (copy-names names)
  (map (lambda (name)
	 (let ((new (make-similar-node name (node-form name))))
	   (node-set! name 'substitute new)
	   new))
       names))

(define-substitution 'name
  (lambda (node)
    (substitute-name-node node #f)))

(define (substitute-name-node node call?)
  (let ((node (name-node-substitute node)))
    (let ((binding (node-ref node 'binding)))
      (cond ((not binding)
	     (note-name-use! node)
	     node)
	    ((not (binding? binding))
	     (bug "unbound variable ~S" (node-form node)))
	    ((primitive? (binding-static binding))
	     (make-primitive-node (binding-static binding) call?))
	    ((location? (binding-place binding))
	     (let ((value (contents (binding-place binding))))
	       (if (constant? value)
		   (make-literal-node value)
		   (identity
		   (bug "name ~S has non-constant location ~S" node value)))))
	    (else
	     (note-binding-use! binding)
	     node)))))

(define (name-node-substitute node)
  (let loop ((node node) (first? #t))
    (cond ((node-ref node 'substitute)
	   => (lambda (node)
		(loop node #f)))
	  ((and first? (not (node-ref node 'binding)))
	   (user-error "unbound variable ~S" (node-form node)))
	  (else
	   node))))

(define-substitution 'set!
  (lambda (node)
    (let* ((exp (node-form node))
	   (name (substitute-name-node (cadr exp) #f))
	   (binding (node-ref name 'binding)))
      (if (not (binding? binding))
	  (user-error "SET! on local variable ~S" (node-form (cadr exp))))
      ((structure-ref forms note-variable-set!!)
        (binding-place binding))
      (note-binding-use! binding)
      (make-similar-node node
			 (list (car exp)
                               name
                               (substitute-in-exp (caddr exp)))))))

(define-substitution 'call
  (lambda (node)
    (let ((proc (car (node-form node)))
	  (args (cdr (node-form node))))
      (make-similar-node node
			 (cons (if (name-node? proc)
				   (substitute-name-node proc #t)
				   (substitute-in-exp proc))
			       (map substitute-in-exp args))))))

; Flush GOTO when it is used with a primitive.

(define-substitution 'goto
  (lambda (node)
    (let ((proc (cadr (node-form node)))
	  (args (cddr (node-form node))))
      (if (and (name-node? proc)
	       (bound-to-primitive? proc))
	  (make-node (get-operator 'call)
		     (cons (substitute-name-node proc #t)
			   (map substitute-in-exp args)))
	  (make-similar-node node
			     (cons 'goto
				   (cons (if (name-node? proc)
					     (substitute-name-node proc #t)
					     (substitute-in-exp proc))
					 (map substitute-in-exp args))))))))

(define name-node? (node-predicate 'name))

(define (bound-to-primitive? node)
  (let ((node (name-node-substitute node)))
    (let ((binding (node-ref node 'binding)))
      (and binding
	   (primitive? (binding-static binding))))))

(define-substitution 'begin default-substitution)
(define-substitution 'if    default-substitution)

; drop the loophole part

(define-substitution 'loophole
  (lambda (node)
    (substitute-in-exp (caddr (node-form node)))))

;----------------------------------------------------------------
; Breaking LETREC's down to improve type inference and make compilation
; easier.

(define-substitution 'letrec
  (lambda (node)
    (let* ((exp (node-form node))
	   (vars (map car (cadr exp)))
	   (vals (map cadr (cadr exp))))
      (receive (names datas)
	  (copy-letrec-names vars vals exp)
	(for-each (lambda (data value)
		    (expand-letrec-value data value datas exp))
		  datas
		  vals)
	(let ((sets (strongly-connected-components datas
						   letrec-data-uses
						   letrec-data-seen?
						   set-letrec-data-seen?!)))
	  ;; so we don't keep track of which vars are referenced in the body
	  (for-each (lambda (d)
		      (set-letrec-data-seen?! d #t))
		    datas)
	  (do ((sets sets (cdr sets))
	       (exp (substitute-in-exp (caddr exp))
		    (build-letrec (car sets) exp)))
	      ((null? sets)
	       (for-each (lambda (n)
			   (node-set! n 'letrec-data #f))
			 names)
	       exp)))))))

(define-record-type letrec-data
  (name      ; the name node for which this data exists
   marker    ; a unique marker for this LETREC
   cell?     ; variable is SET! or its value is not a (lambda ...).  This is
             ; always #F until I can think of a reason to allow otherwise.
   )
  (value     ; the expanded value of this variable
   uses      ; a list of variables that VALUE uses
   seen?     ; #T if this has been seen before during the current expansion
   ))

(define (copy-letrec-names names vals marker)
  (let ((names (map (lambda (name value)
		      (let ((new (make-similar-node name (node-form name)))
			    (cell? #f)) ; we no longer allow SET! on LETREC vars.
			(node-set! new 'letrec-data
				   (letrec-data-maker new marker cell?))
			(node-set! name 'substitute new)
			new))
		    names
		    vals)))
    (values names (map (lambda (name) (node-ref name 'letrec-data)) names))))

(define lambda-node? (node-predicate 'lambda))

; List of LETREC bound variables currently in scope.

(define *letrec-datas* '())

(define (note-name-use! name)
  (let ((data (node-ref name 'letrec-data)))
    (cond ((and data (not (letrec-data-seen? data)))
	   (set-letrec-data-seen?! data #t)
	   (set! *letrec-datas* (cons data *letrec-datas*))))))

; Expand VALUE and determine which of DATAS it uses.

(define (expand-letrec-value data value datas mark)
  (let ((old-letrec-vars *letrec-datas*))
    (set! *letrec-datas* '())
    (for-each (lambda (d) (set-letrec-data-seen?! d #f)) datas)
    (set-letrec-data-value! data (substitute-in-exp value))
    (receive (ours others)
	(partition-list (lambda (data)
			  (eq? (letrec-data-marker data) mark))
			*letrec-datas*)
      (set! *letrec-datas* (append others old-letrec-vars))
      (set-letrec-data-uses! data ours))))

; If there is only one variable and its value doesn't reference it, then
; use a LET instead of a LETREC.  Variables whose value is either set! or
; not a lambda have explicit cells introduced.

(define (build-letrec datas body)
  (if (and (null? (cdr datas))
	   (not (memq? (car datas)
		       (letrec-data-uses (car datas)))))
      (make-let-node (map letrec-data-name datas)
		     (map letrec-data-value datas)
		     body)
      (receive (cells normal)
	  (partition-list letrec-data-cell? datas)
	(make-let-node (map letrec-data-name cells)
		       (map (lambda (ignore) (unspecific-node))
			    cells)
		       (make-letrec-node (map letrec-data-name normal)
					 (map letrec-data-value normal)
					 (make-begin-node
					  (append (map letrec-data->set! cells)
						  (list body))))))))

(define op/unspecific (get-operator 'unspecific))
(define op/set! (get-operator 'set!))

(define (unspecific-node)
  (make-node op/unspecific '()))

(define (letrec-data->set! data)
  (make-node op/set!
	     (list 'set!
		   (letrec-data-name data)
		   (letrec-data-value data))))

(define (make-let-node names values body)
  (if (null? names)
      body
      (make-node op/call
		 (cons (make-node op/lambda
				  (list 'lambda names body))
		       values))))

(define (make-letrec-node names values body)
  (if (null? names)
      body
      (make-node op/letrec
		 (list 'letrec
		       (map list names values)
		       body))))

(define (make-begin-node nodes)
  (if (null? (cdr nodes))
      (car nodes)
      (make-node op/begin (cons 'begin nodes))))

(define op/call   (get-operator 'call))
(define op/lambda (get-operator 'lambda))
(define op/letrec (get-operator 'letrec))
(define op/begin  (get-operator 'begin))

;----------------------------------------------------------------
; A version of MAKE-SIMILAR-NODE that actually makes a new node.
; I wish this could keep the old node's list of properties.

(define (make-similar-node node form)
  (make-node (node-operator node) form))
