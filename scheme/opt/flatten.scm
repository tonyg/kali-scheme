; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Transmogrify code to produce flat lexical environments.
;
; This takes two passes.  The first finds the SET! variables so that cells can
; be added for them.  The second pass adds a list of free variables to each
; non-call-position LAMBDA node.  We don't need the free list for LET's.

(set-optimizer! 'flat-environments
  (lambda (forms package)
    (map (lambda (form)
	   (flatten-form (force form)))
	 forms)))

(define (flatten-form node)
  (mark-set-variables! node)  ; we need to introduce cells for SET! variables
  (if (define-node? node)
      (let ((form (node-form node)))
	(make-similar-node node
			   `(define ,(cadr form)
			      ,(call-with-values
				(lambda ()
				  (flatten-node (caddr form)))
				(lambda (node ignore)
				  node)))))
      (call-with-values
       (lambda ()
	 (flatten-node node))
       (lambda (node ignore)
	 node))))

; Main dispatch
; This returns a new node and a list of free lexical variables.

(define (flatten-node node)
  ((operator-table-ref flatteners (node-operator-id node))
     node))

; Particular operators

(define flatteners
  (make-operator-table
   (lambda (node)
     (call-with-values
      (lambda ()
	(flatten-list (cdr (node-form node))))
      (lambda (exps free)
	(values (make-similar-node node
				   (cons (car (node-form node))
					 exps))
		free))))))

(define (define-flattener name proc)
  (operator-define! flatteners name #f proc))

(define (flatten-list nodes)
  (let loop ((nodes nodes) (new '()) (free '()))
    (if (null? nodes)
	(values (reverse new) free)
	(call-with-values
	 (lambda ()
	   (flatten-node (car nodes)))
	 (lambda (node more-free)
	   (loop (cdr nodes)
		 (cons node new)
		 (union more-free free)))))))

(define (no-free-vars node)
  (values node '()))

(define-flattener 'literal             no-free-vars)
(define-flattener 'quote               no-free-vars)
(define-flattener 'primitive-procedure no-free-vars)
(define-flattener 'lap                 no-free-vars)

; LAMBDA's get changed to FLAT-LAMBDA's if the lexical environment is
; non-empty.
; (FLAT-LAMBDA -formals- -free-vars- -body-)

(define-flattener 'lambda
  (lambda (node)
    (flatten-lambda node #t)))

(define (flatten-lambda node closure?)
  (let ((exp (node-form node)))
    (call-with-values
     (lambda ()
       (convert-lambda-body node))
     (lambda (body free)
       (values (if closure?
		   (make-node operator/flat-lambda
			      (list 'flat-lambda
				    (cadr exp)
				    free
				    body))
		   (make-similar-node node
				      (list (car exp)
					    (cadr exp)
					    body)))
	       free)))))
	  
; Flatten the body and make cells for any SET! variables.

(define (convert-lambda-body node)
  (let* ((exp (node-form node))
	 (var-nodes (normalize-formals (cadr exp))))
    (call-with-values
     (lambda ()
       (flatten-node (caddr exp)))
     (lambda (body free)
       (values (add-cells body var-nodes)
	       (set-difference free var-nodes))))))

(define (add-cells exp vars)
  (do ((vars vars (cdr vars))
       (cells '() (if (assigned? (car vars))
		      (cons (make-cell (car vars)) cells)
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
  (lambda (node)
    (if (node-ref node 'binding)
	(values node '())
	(values (if (assigned? node)
		    (make-cell-ref node)
		    node)
		(list node)))))

(define-flattener 'set!
  (lambda (node)
    (let* ((exp (node-form node))
	   (var (cadr exp)))
      (call-with-values
       (lambda ()
	 (flatten-node (caddr exp)))
       (lambda (value free)
	 (if (assigned? var)
	     (values (make-cell-set! var value)
		     (union (list var) free))
	     (values (make-similar-node node
					(list 'set! var value))
		     free)))))))

(define-flattener 'call
  (lambda (node)
    (let ((form (node-form node)))
      (call-with-values
       (lambda ()
	 (flatten-list (cdr form)))
       (lambda (args free)
	 (call-with-values
	  (lambda ()
	    (if (lambda-node? (car form))
		(flatten-lambda (car form) #f)
		(flatten-node (car form))))
	  (lambda (proc more-free)
	    (values (make-similar-node node
				       (cons proc args))
		    (union free more-free)))))))))

(define-flattener 'loophole
  (lambda (node)
    (let ((form (node-form node)))
      (call-with-values
       (lambda ()
	 (flatten-node (caddr form)))
       (lambda (new free)
	 (values (make-similar-node node
				    (list (car form)
					  (cadr form)
					  new))
		 free))))))
  
; Use LET & SET! for LETREC.

(define-flattener 'letrec
  (lambda (node)
    (let* ((form (node-form node))
	   (vars (map car (cadr form)))
	   (vals (map cadr (cadr form))))
      (for-each (lambda (var)
		  (node-set! var 'assigned #t))
		vars)
      (call-with-values
       (lambda ()
	 (flatten-list (cons (caddr form) vals)))
       (lambda (nodes free)
	 (values (make-node operator/call
		    (cons (make-node operator/lambda
			     `(lambda ,vars
				,(make-node operator/begin
				   `(begin ,@(map make-cell-set!
						  vars
						  (cdr nodes))
					   ,(car nodes)))))
			  (map (lambda (ignore)
				 (make-unassigned-cell))
			       vars)))
		 (set-difference free vars)))))))

(define-flattener 'lap
  (lambda (node)
    (caddr (node-form node))))

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
    (let ((form (node-form node)))
      (for-each (lambda (spec)
		  (mark-set-variables! (cadr spec)))
		(cadr form))
      (mark-set-variables! (caddr form)))))

;----------------
; Cell manipulation calls.

(define (make-cell var)
  (make-node operator/set!
	     (list 'set!
		   var
		   (make-primop-call (make-cell-primop) var))))

(define (make-unassigned-cell)
  (make-primop-call (make-cell-primop)
		    (make-node (get-operator 'unassigned)
			       '(unassigned))))

(define (make-cell-ref var)
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
(define operator/begin       (get-operator 'begin))
(define operator/literal     (get-operator 'literal))
(define operator/call        (get-operator 'call))
(define operator/unassigned  (get-operator 'unassigned))
(define operator/set!        (get-operator 'set!))

(define define-node? (node-predicate 'define))
(define lambda-node? (node-predicate 'lambda))

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

;----------------
; Set operations on lists.

(define (union vars1 vars2)
  (if (null? vars2)
      vars1
      (let recur ((vars1 vars1))
	(cond ((null? vars1)
	       vars2)
	      ((memq (car vars1) vars2)
	       (recur (cdr vars1)))
	      (else
	       (cons (car vars1) (recur (cdr vars1))))))))

(define (set-difference vars1 vars2)
  (let recur ((vars1 vars1))
    (cond ((null? vars1)
	   '())
	  ((memq (car vars1) vars2)
	   (recur (cdr vars1)))
	  (else
	   (cons (car vars1) (recur (cdr vars1)))))))
