; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Transmogrify code to produce flat lexical environments.
;
; This takes two passes.  The first finds the SET! variables so that cells can
; be added for them.  The second pass adds a list of free variables to each
; non-call-position LAMBDA node.  We don't need the free list for LET's.

(set-optimizer! 'flat-environments
  (lambda (forms package)
    (map (lambda (form)
           (flatten-form (force-node form)))
         forms)))

(define (flatten-form node)
  (mark-set-variables! node)  ; we need to introduce cells for SET! variables
  (if (define-node? node)
      (let ((form (node-form node)))
	(make-similar-node node
			   `(define ,(cadr form)
			      ,(flatten-node (caddr form)
					     (install-new-set!)))))
      (flatten-node node (install-new-set!))))

; Main dispatch
; This returns a new node and a list of free lexical variables.

(define (flatten-node node free)
  ((operator-table-ref flatteners (node-operator-id node))
     node
     free))

; Particular operators

(define flatteners
  (make-operator-table
   (lambda (node free)
     (make-similar-node node
			(cons (car (node-form node))
			      (flatten-list (cdr (node-form node))
					    free))))))

(define (define-flattener name proc)
  (operator-define! flatteners name #f proc))

(define (flatten-list nodes free)
  (map (lambda (node)
	 (flatten-node node free))
       nodes))

(define (no-free-vars node free)
  node)

(define-flattener 'literal             no-free-vars)
(define-flattener 'quote               no-free-vars)
(define-flattener 'primitive-procedure no-free-vars)

; LAMBDA's get changed to FLAT-LAMBDA's if the lexical environment is
; non-empty.
; (FLAT-LAMBDA -formals- -free-vars- -body-)

(define-flattener 'lambda
  (lambda (node free)
    (flatten-lambda node caddr free #t)))

(define-flattener 'flat-lambda
  (lambda (node free)
    (flatten-lambda node cadddr free #t)))

(define (flatten-lambda node get-body free closure?)
  (let ((exp (node-form node))
	(my-free (install-new-set!)))
    (let* ((formals (cadr exp))
	   (body (convert-lambda-body formals (get-body exp) my-free)))
      (install-set! free)
      (set-union! free my-free)
      (if closure?
	  (make-node operator/flat-lambda
		     (list 'flat-lambda
			   formals
			   (set->list my-free)
			   body))
	  (make-node operator/lambda
		     (list 'lambda
			   formals
			   body))))))
  
; Flatten the body and make cells for any SET! variables.

(define (convert-lambda-body formals body free)
  (let* ((var-nodes (normalize-formals formals))
	 (body (flatten-node body free)))
    (set-difference! free var-nodes)
    (add-cells body var-nodes)))

(define (add-cells exp vars)
  (do ((vars vars (cdr vars))
       (cells '() (if (assigned? (car vars))
		      (cons (make-make-cell (car vars)) cells)
		      cells)))
      ((null? vars)
       (if (null? cells)
	   exp
	   (make-node operator/begin
		      `(begin
			 ,@(reverse cells)
			 ,exp))))))

; Lexical nodes are free and may have cells.

(define-flattener 'name
  (lambda (node free)
    (if (node-ref node 'binding)
	node
	(begin
	  (set-add-element! free node)
	  (if (assigned? node)
	      (make-cell-ref node)
	      node)))))

(define-flattener 'set!
  (lambda (node free)
    (let* ((exp (node-form node))
	   (var (cadr exp))
	   (value (flatten-node (caddr exp) free)))
      (if (assigned? var)
	  (begin
	    (set-add-element! free var)
	    (make-cell-set! var value))
	  (make-similar-node node
			     (list 'set! var value))))))

(define-flattener 'call
  (lambda (node free)
    (let ((proc (car (node-form node)))
	  (args (cdr (node-form node))))
      (make-similar-node node
			 (cons (cond ((and (lambda-node? proc)
					   (not (n-ary? (cadr (node-form proc)))))
				      (flatten-lambda proc caddr free #f))
				     ((and (flat-lambda-node? proc)
					   (not (n-ary? (cadr (node-form proc)))))
				      (flatten-lambda proc cadddr free #f))
				     (else
				      (flatten-node proc free)))
			       (flatten-list args free))))))

(define-flattener 'loophole
  (lambda (node free)
    (let ((form (node-form node)))
      (make-similar-node node
			 (list (car form)
			       (cadr form)
			       (flatten-node (caddr form) free))))))
  
; Use LET & SET! for LETRECs that have non-LAMBDA values.

(define-flattener 'letrec
  (lambda (node free)
    (flatten-letrec node caddr free)))

(define-flattener 'pure-letrec
  (lambda (node free)
    (flatten-letrec node cadddr free)))

(define (flatten-letrec node get-body free)
  (let ((form (node-form node)))
    (let ((vars (map car (cadr form)))
	  (vals (map cadr (cadr form)))
	  (body (get-body form)))
      (cond ((null? vars)
	     (flatten-node body free))			;+++
	    ((and (every (lambda (node)
			   (or (lambda-node? node)
			       (flat-lambda-node? node)))
			 vals)
		  (not (any assigned? vars)))
	     (flatten-pure-letrec vars vals body free))	;+++
	    (else
	     (flatten-impure-letrec vars vals body free))))))

(define (flatten-pure-letrec vars vals body free)
  (let* ((vals-free (install-new-set!))
	 (vals (flatten-list vals vals-free)))
    (set-difference! vals-free vars)
    (install-set! free)
    (let ((body (flatten-node body free)))
      (set-difference! free vars)
      (set-union! free vals-free)
      (make-node operator/pure-letrec
		 `(pure-letrec ,(map list vars vals)
			       ,(set->list vals-free)
			       ,body)))))

(define (flatten-impure-letrec vars vals body free)
  (for-each (lambda (var)
	      (node-set! var 'assigned 'maybe))
	    vars)
  (let ((vals (flatten-list vals free))
	(temps (map (lambda (var)
		      (make-node operator/name var))
		    vars))
	(body (flatten-node body free)))
    (set-difference! free vars)
    (make-node
     operator/call
     (cons
      (make-node operator/lambda
       `(lambda ,vars
	  ,(make-node
	    operator/call
	    (cons
	     (make-node operator/lambda
			`(lambda ,temps
			   ,(make-node operator/begin
				       `(begin ,@(map make-cell-set!
						      vars
						      temps)
					       ,body))))
	     vals))))
      (map (lambda (ignore)
	     (make-unassigned-cell))
	   vars)))))

; Pick out the lexical variables from the list of free variables in the
; LAP form.

(define-flattener 'lap
  (lambda (node free)
    (for-each (lambda (var)
		(if (not (node-ref var 'binding))
		    (set-add-element! free var)))
	      (caddr (node-form node)))
    node))

;----------------
; Is name-node NODE SET! anywhere?

(define (assigned? node)
  (node-ref node 'assigned))

; Gather the info needed by ASSIGNED?.

(define (mark-set-variables! node)
  ((operator-table-ref mark-sets (node-operator-id node))
   node))

; Particular operators

(define mark-sets
  (make-operator-table
   (lambda (node)
     (for-each mark-set-variables! (cdr (node-form node))))))

(define (define-set-marker name proc)
  (operator-define! mark-sets name #f proc))

(define (no-sets node) #f)

(define-set-marker 'literal             no-sets)
(define-set-marker 'quote               no-sets)
(define-set-marker 'name                no-sets)
(define-set-marker 'primitive-procedure no-sets)
(define-set-marker 'lap                 no-sets)

(define-set-marker 'lambda
  (lambda (node)
    (mark-set-variables! (caddr (node-form node)))))

(define-set-marker 'flat-lambda
  (lambda (node)
    (mark-set-variables! (cadddr (node-form node)))))

(define-set-marker 'set!
  (lambda (node)
    (let* ((exp (node-form node))
	   (var (cadr exp)))
      (if (not (node-ref var 'binding))
	  (node-set! var 'assigned #t))
      (mark-set-variables! (caddr exp)))))

(define-set-marker 'loophole
  (lambda (node)
    (mark-set-variables! (caddr (node-form node)))))

(define-set-marker 'call
  (lambda (node)
    (for-each mark-set-variables! (node-form node))))

(define-set-marker 'letrec
  (lambda (node)
    (mark-letrec-sets node caddr)))

(define-set-marker 'pure-letrec
  (lambda (node)
    (mark-letrec-sets node cadddr)))

(define (mark-letrec-sets node get-body)
  (let ((form (node-form node)))
    (for-each (lambda (spec)
		(mark-set-variables! (cadr spec)))
	      (cadr form))
    (mark-set-variables! (get-body form))))

;----------------
; Cell manipulation calls.

(define (make-make-cell var)
  (make-node operator/set!
	     (list 'set!
		   var
		   (make-primop-call (make-cell-primop) var))))

(define (make-unassigned-cell)
  (make-primop-call (make-cell-primop)
		    (make-node (get-operator 'unassigned)
			       '(unassigned))))

; LETREC-bound cells need an additional check.

(define (make-cell-ref var)
  (if (eq? 'maybe (node-ref var 'assigned))
      (make-primop-call (unassigned-check-primop)
			(really-make-cell-ref var))
      (really-make-cell-ref var)))

(define (really-make-cell-ref var)
  (make-primop-call (cell-ref-primop) var))

(define (make-cell-set! var value)
  (make-primop-call (cell-set!-primop) var value))

(define (make-primop-call primop . args)
  (make-node operator/call
	     (cons (make-node operator/literal
			      primop)
		   args)))

;----------------
; The operators and primops used here.

(define operator/flat-lambda (get-operator 'flat-lambda))
(define operator/lambda      (get-operator 'lambda))
(define operator/name        (get-operator 'name))
(define operator/letrec      (get-operator 'letrec))
(define operator/pure-letrec (get-operator 'pure-letrec))
(define operator/begin       (get-operator 'begin))
(define operator/literal     (get-operator 'literal))
(define operator/call        (get-operator 'call))
(define operator/unassigned  (get-operator 'unassigned))
(define operator/set!        (get-operator 'set!))

(define define-node?      (node-predicate 'define))
(define lambda-node?      (node-predicate 'lambda))
(define flat-lambda-node? (node-predicate 'flat-lambda))

; We get loaded before these are defined, so we have to delay the lookups.

(define-syntax define-primop
  (syntax-rules ()
    ((define-primop name id temp)
     (begin
       (define temp #f)
       (define (name)
	 (or temp
	     (begin
	       (set! temp (get-primop 'id))
	       temp)))))))

(define-primop make-cell-primop make-cell temp0)
(define-primop cell-ref-primop  cell-ref  temp1)
(define-primop cell-set!-primop cell-set! temp2)
(define-primop unassigned-check-primop unassigned-check temp3)

;----------------
; Set operations on lists.
;
; These use side effects to make union and difference O(n).  Name nodes are
; marked with the set they are in.  These marks are only valid for one set
; at any given time.

(define (install-new-set!)
  (list 'set))

(define (install-set! set)
  (for-each (lambda (var)
	      (node-set! var 'set-owner set))
	    (cdr set)))

(define set->list cdr)

(define (set-add-element! set var)
  (if (not (eq? set (node-ref var 'set-owner)))
      (begin
	(node-set! var 'set-owner set)
	(set-cdr! set (cons var (cdr set))))))

(define (set-union! set other-set)
  (for-each (lambda (var)
	      (set-add-element! set var))
	    (set->list other-set)))

(define (set-difference! set vars)
  (for-each clear-var-set! vars)
  (set-cdr! set (clean-var-list (cdr set))))

(define (clean-var-list list)
  (cond ((null? list)
	 list)
	((node-ref (car list) 'set-owner)
	 (cons (car list)
	       (clean-var-list (cdr list))))
	(else
	 (clean-var-list (cdr list)))))

(define (clear-var-set! var)
  (node-set! var 'set-owner #f))
