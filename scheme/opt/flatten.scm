; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Transmogrify code to produce flat lexical environments.

; Hook into the byte code compiler.

; This code is not up-to-date.

;(set-optimizer! 'flat-environments
;  (lambda (stuff p)
;;    (display "Making flat environments... ")
;;    (force-output (current-output-port))
;    (let ((stuff (map (lambda (filename+nodes)
;                        (let ((filename (car filename+nodes))
;                              (nodes (cdr filename+nodes)))
;                          (cons filename
;                                (map flatten-form nodes))))
;                      stuff)))
;;      (newline)
;      stuff)))

(define (flatten-form node)
  (if (define-node? node)
      (let ((form (node-form node)))
	(make-similar-node node `(define ,(cadr form)
				   ,(call-with-values
				     (lambda ()
				       (flatten (caddr form) #t))
				     (lambda (node ignore)
				       node)))))
      (call-with-values
       (lambda ()
	 (flatten node #t))
       (lambda (node ignore)
	 node))))

; Main dispatch
; This returns a new node and a list of free lexical variables.
; TOP? is true if the lexical environment is empty (in which case we
; can use regular closures).

(define (flatten node top?)
  ((operator-table-ref flatteners (node-operator-id node))
   node
   top?))

; Particular operators

(define flatteners
  (make-operator-table
   (lambda (node top?)
     (call-with-values
      (lambda ()
	(flatten-list (cdr (node-form node)) top?))
      (lambda (exps free)
	(values (make-similar-node node (cons (car (node-form node)) exps))
		free))))))

(define (define-flattener name proc)
  (operator-define! flatteners name #f proc))

(define (flatten-list nodes top?)
  (let loop ((nodes nodes) (new '()) (free '()))
    (if (null? nodes)
	(values (reverse new) free)
	(call-with-values
	 (lambda ()
	   (flatten (car nodes) top?))
	 (lambda (node more-free)
	   (loop (cdr nodes) (cons node new) (union more-free free)))))))

(define-flattener 'literal
  (lambda (node top?)
    (values node '())))

(define-flattener 'quote
  (lambda (node top?)
    (values node '())))

; LAMBDA's get changed to FLAT-LAMBDA's if the lexical environment is
; non-empty.
; (FLAT-LAMBDA -formals- -free-vars- -body-)

(define-flattener 'lambda
  (lambda (node top?)
    (let ((exp (node-form node)))
      (call-with-values
       (lambda ()
	 (convert-lambda-body node top?))
       (lambda (body free)
	 (values (if top?
		     (make-similar-node node (list (car exp) (cadr exp) body))
		     (make-node (get-operator 'flat-lambda)
				(list 'flat-lambda
				      (cadr exp)
				      free
				      body)))
		 free))))))
	  
; Flatten the body and make cells for any SET! variables.

(define (convert-lambda-body node top?)
  (let ((exp (node-form node))
	(var-nodes (node-ref node 'var-nodes)))
    (call-with-values
     (lambda ()
       (flatten (caddr exp) (and top? (null? var-nodes))))
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
	   (make-node (get-operator 'begin)
		      `(begin
			 ,@(reverse cells)
			 ,exp))))))

; Lexical nodes are free and may have cells.

(define-flattener 'name
  (lambda (node top?)
    (if (node-ref node 'usage)
	(values (if (assigned? node)
		    (make-cell-ref node)
		    node)
		(list node))
	(values node '()))))

(define-flattener 'set!
  (lambda (node top?)
    (let* ((exp (node-form node))
	   (var (cadr exp)))
      (call-with-values
       (lambda ()
	 (flatten (caddr exp) top?))
       (lambda (value free)
	 (if (and (node-ref var 'usage)
		  (assigned? var))
	     (values (make-cell-set! var value)
		     (union (list var) free))
	     (values (make-similar-node node (list 'set! var value))
		     free)))))))

(define-flattener 'call
  (lambda (node top?)
    (let ((form (node-form node)))
      (call-with-values
       (lambda ()
	 (flatten-list (cdr form) top?))
       (lambda (args free)
	 (call-with-values
	  (lambda ()
	    (flatten (car form) #t))  ; no closure needed for LET
	  (lambda (proc more-free)
	    (values (make-similar-node node
				       (cons proc args))
		    (union more-free free)))))))))

(define-flattener 'loophole
  (lambda (node top?)
    (let ((form (node-form node)))
      (call-with-values
       (lambda ()
	 (flatten (caddr form) top?))
       (lambda (new free)
	 (values (make-similar-node node
				    (list (car form)
					  (cadr form)
					  new))
		 free))))))
  
(define-flattener 'primitive-procedure
  (lambda (node top?)
    (values node '())))

(define-flattener 'define-syntax
  (lambda (node env)
    (values node '())))

; If all lambdas and none set!:
;  ((flat-lambda (-vars- -free-vars-) ()
;     (set! -f- (lambda ...))
;     (set! -g- (lambda ...))
;     ...)
;   #f #f ... -free-vars-)
; This makes the right data structures but does a stack->heap copy.
; Which is more or less what happens in any case.
; Except that -free-vars- must include the free variables of the body.
; Yuck.  Add a flat-letrec special form?
;
; (push (vector #f ... -free-vars-))
; (stack-ref ..)   ; get the env
; (literal (template ...))
; make closure
; push
; (stack-ref ..)   ; get the env
; stob-set!        ; put the closure in the env
; ...
; 

; Use LET & SET! for the general case.

(define-flattener 'letrec
  (lambda (node top?)
    (let ((form (node-form node))
	  (var-nodes (node-ref node 'var-nodes)))
      (flatten (let-n-set form var-nodes) top?))))

;      (if (and (not (any assigned? var-nodes))
;               (every (lambda (p)
;                        (lambda-node? (cadr p)))))
;          (flat-letrec node form var-nodes)

(define (let-n-set form var-nodes)
  (let ((vars (map car (cadr form)))
	(vals (map cadr (cadr form))))
    (let ((proc (make-node (get-operator 'lambda)
		  `(lambda ,vars
		     ,(make-node (get-operator 'begin)
				 `(begin
				    ,@(map (lambda (var val)
					     (make-node (get-operator 'set!)
							`(set! ,var ,val)))
					   var-nodes
					   vals)
				    . ,(cddr form)))))))
      (node-set! proc 'var-nodes var-nodes)
      (for-each (lambda (node)
		  (node-set! node 'assigned? #t))   ; so they'll get cells
		var-nodes)
      (make-node (get-operator 'call)
		 `(,proc
		   . ,(map (lambda (ignore)
			     (make-node (get-operator 'literal) #f))
			   vars))))))

; Is name-node NODE SET! anywhere.

(define (assigned? node)
  (let ((usage (node-ref node 'usage)))
    (or (and usage (< 0 (usage-assignment-count usage)))
	(node-ref node 'assigned?))))

; Cell manipulation calls.

(define (make-cell var)
  (make-node (get-operator 'set!)
	     (list 'set!
		   var
		   (make-node (get-operator 'make-cell)
			      (list 'make-cell var)))))

(define (make-cell-ref var)
  (make-node (get-operator 'cell-ref)
	     (list 'cell-ref var)))

(define (make-cell-set! var value)
  (make-node (get-operator 'cell-set!)
	     (list 'cell-set! var value)))

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
