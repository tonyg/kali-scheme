
; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; This file contains the definitions of the node tree data structure.

;---------------------------------------------------------------------------
; Records to represent variables.

(define-record-type variable
  ((name)        ; Source code name for variable (used for debugging only)
   (id)          ; Unique numeric identifier     (used for debugging only)
   (type)        ; Type for variable's value
   )
  (binder        ; LAMBDA node which binds this variable
   (refs '())    ; List of leaf nodes n for which (REFERENCE-VARIABLE n) = var.
   (flag #f)     ; Useful slot, used by shapes, COPY-NODE, NODE->VECTOR, etc.
                 ; all users must leave this is #F
   (flags '())   ; For various annotations, e.g. IGNORABLE
   (generate #f) ; For whatever code generation wants
   ))

(define-record-discloser type/variable
  (lambda (var)
    (node-hash var)
    (list 'variable (variable-name var) (variable-id var))))

(define (make-variable name type)
  (variable-maker name (new-id) type))

(define (make-global-variable name type)
  (let ((var (make-variable name type)))
    (set-variable-binder! var #f)
    var))

(define (global-variable? var)
  (not (variable-binder var)))

; Every variable has a unique numeric identifier that is used for printing.

(define *variable-id* 0)

(define (new-id)
  (let ((id *variable-id*))
    (set! *variable-id* (+ 1 *variable-id*))
    id))

(define (erase-variable var)
  (cond ((eq? (variable-id var) '<erased>)
	 (bug "variable ~S already erased" var))
	(else
	 (set-variable-id! var '<erased>))))

(define *node-hash-table* #f)

(define (reset-node-id)
  (set! *variable-id* 0)
  (set! *node-hash-table* (make-table)))

(define (node-hash var-or-lambda)
  (let ((id (if (variable? var-or-lambda)
		(variable-id var-or-lambda)
		(lambda-id var-or-lambda))))
    (table-set! *node-hash-table* id var-or-lambda)))

(define (node-unhash n)
  (table-ref *node-hash-table* n))

; The index of VAR in the variables bound by its binder.

(define (variable-index var)
  (let ((binder (variable-binder var)))
    (if (not binder)
	(bug "VARIABLE-INDEX called on global variable ~S" var)
	(do ((i 0 (+ i 1))
	     (vs (lambda-variables binder) (cdr vs)))
	    ((eq? (car vs) var)
	     i)))))

; Copy an old variable.

(define (copy-variable old)
  (let ((var (make-variable (variable-name old) (variable-type old))))
    (set-variable-flags! var (variable-flags old))
    var))

; An unused variable is either #F or a variable with no references.

(define (used? var)
  (and var
       (not (null? (variable-refs var)))))

(define (unused? var)
  (not (used? var)))

; known values for top-level variables

(define (flag-accessor flag)
  (lambda (var)
    (let ((p (flag-assq flag (variable-flags var))))
      (if p (cdr p) #f))))

(define (flag-setter flag)
  (lambda (var value)
    (set-variable-flags! var
			 (cons (cons flag value)
			       (variable-flags var)))))

(define (flag-remover flag)
  (lambda (var)
    (set-variable-flags! var (filter (lambda (x)
				       (or (not (pair? x))
					   (not (eq? (car x) flag))))
				     (variable-flags var)))))
  
(define variable-known-value (flag-accessor 'known-value))
(define add-variable-known-value! (flag-setter 'known-value))
(define remove-variable-known-value! (flag-remover 'known-value))

(define variable-simplifier (flag-accessor 'simplifier))
(define add-variable-simplifier! (flag-setter 'simplifier))
(define remove-variable-simplifier! (flag-remover 'simplifier))

(define variable-known-lambda (flag-accessor 'known-lambda))
(define note-known-global-lambda! (flag-setter 'known-lambda))

;----------------------------------------------------------------------------
; The main record for the node tree

(define-record-type node
  ((variant)           ; One of LAMBDA, CALL, REFERENCE, LITERAL
   )
  ((parent empty)      ; Parent node
   (index '<free>)     ; Index of this node in parent
   (simplified? #f)    ; True if it has already been simplified.
   (flag #f)           ; Useful flag, all users must leave this is #F
   stuff-0             ; Variant components - each type of node has a different
   stuff-1             ; use for these fields
   stuff-2
   stuff-3
   ))

(define-record-discloser type/node
  (lambda (node)
    `(node ,(node-variant node)
	   . ,(case (node-variant node)
		((lambda)
		 (node-hash node)
		 (list (lambda-name node) (lambda-id node)))
		((call)
		 (list (primop-id (call-primop node))))
		((reference)
		 (let ((var (reference-variable node)))
		   (list (variable-name var) (variable-id var))))
		((literal)
		 (list (literal-value node)))
		(else
		 '())))))

(define make-node node-maker)

;--------------------------------------------------------------------------
; EMPTY is used to mark empty parent and child slots in nodes.

(define empty
  (list 'empty))

(define (empty? obj) (eq? obj empty))

(define (proclaim-empty probe)
  (cond ((not (empty? probe))
         (bug "not empty - ~S" probe))))

;----------------------------------------------------------------------------
; This walks the tree rooted at NODE and removes all pointers that point into
; this tree from outside.

(define (erase node)
  (let label ((node node))
    (cond ((empty? node)
           #f)
          (else
           (case (node-variant node)
             ((lambda)
              (label (lambda-body node)))
             ((call)
              (walk-vector label (call-args node))))
           (really-erase node)))))

; This does the following:
; Checks that this node has not already been removed from the tree.
;
; Reference nodes are removed from the refs list of the variable they reference.
;
; For lambda nodes, the variables are erased, non-CONT lambdas are removed from
; the *LAMBDAS* list (CONT lambdas are never on the list).
;
; Literal nodes whose values have reference lists are removed from those
; reference lists.

(define (really-erase node)
  (cond ((empty? node)
         #f)
        (else
         (cond ((eq? (node-index node) '<erased>)
                (bug "node erased twice ~S" node))
               ((reference-node? node)
		(let ((var (reference-variable node)))
		  (set-variable-refs! var
				      (delq! node (variable-refs var)))))
               ((lambda-node? node)
                (for-each (lambda (v)
			    (if v (erase-variable v)))
			  (lambda-variables node))
                (if (neq? (lambda-type node) 'cont)
                    (delete-lambda node))
                (set-lambda-variables! node '()))  ; safety
               ((literal-node? node)
                (let ((refs (literal-refs node)))
                  (if refs
		      (set-literal-reference-list!
		       refs
                       (delq! node (literal-reference-list refs)))))))
;	 (erase-type (node-type node))
         (set-node-index! node '<erased>))))

;---------------------------------------------------------------------------
; CONNECTING AND DISCONNECTING NODES
;
; There are two versions of each of these routines, one for value nodes
; (LAMBDA, REFERENCE, or LITERAL), and one for call nodes.

; Detach a node from the tree.

(define (detach node)
  (vector-set! (call-args (node-parent node))
	       (node-index node)
	       empty)
  (set-node-index! node #f)
  (set-node-parent! node empty)
  node)

(define (detach-body node)
  (set-lambda-body! (node-parent node) empty)
  (set-node-index! node #f)
  (set-node-parent! node empty)
  node)

; Attach a node to the tree.

(define (attach parent index child)
  (proclaim-empty (node-parent child))
  (proclaim-empty (vector-ref (call-args parent) index))
  (vector-set! (call-args parent) index child)
  (set-node-parent! child parent)
  (set-node-index! child index)
  (values))

(define (attach-body parent call)
  (proclaim-empty (node-parent call))
  (proclaim-empty (lambda-body parent))
  (set-lambda-body! parent call)
  (set-node-parent! call parent)
  (set-node-index! call '-1)
  (values))

; NODES is an alternating series ... lambda, call, lambda, call, ...
; that is connected into a sequence.  Each call becomes the body of the
; previous lambda and each lambda becomes the (single) exit of the previous
; call.

(define (connect-sequence . all-nodes)
  (if (not (null? all-nodes))
      (let loop ((last (car all-nodes)) (nodes (cdr all-nodes)))
	(if (not (null? nodes))
	    (let ((next (car nodes)))
	      (cond ((and (lambda-node? last)
			  (call-node? next))
		     (attach-body last next))
		    ((and (call-node? last)
			  (lambda-node? next)
			  (= 1 (call-exits last)))
		     (attach last 0 next))
		    (else
		     (bug "bad node sequence ~S" all-nodes)))
	      (loop next (cdr nodes)))))))

; Replace node in tree with value of applying proc to node.
; Note the fact that a change has been made at this point in the tree.

(define (move node proc)
  (let ((parent (node-parent node))
        (index (node-index node)))
    (detach node)
    (let ((new (proc node)))
      (attach parent index new)
      (mark-changed new))))

(define (move-body node proc)
  (let ((parent (node-parent node)))
    (detach-body node)
    (let ((new (proc node)))
      (attach-body parent new)
      (mark-changed new))))

; Put CALL into the tree as the body of lambda-node PARENT, making the current
; body of PARENT the body of lambda-node CONT.

(define (insert-body call cont parent)
  (move-body (lambda-body parent)
             (lambda (old-call)
               (attach-body cont old-call)
               call)))

; Replace old-node with new-node, noting that a change has been made at this
; point in the tree.

(define (replace old-node new-node)
  (let ((index (node-index old-node))
        (parent (node-parent old-node)))
    (mark-changed old-node)
    (erase (detach old-node))
    (attach parent index new-node)
    (set-node-simplified?! new-node #f)
    (values)))

(define (replace-body old-node new-node)
  (let ((parent (node-parent old-node)))
    (mark-changed old-node)
    (erase (detach-body old-node))
    (attach-body parent new-node)
    (set-node-simplified?! new-node #f)
    (values)))

; Starting with the parent of NODE, set the SIMPLIFIED? flags of the
; ancestors of NODE to be #F.

(define (mark-changed node)
  (do ((p (node-parent node) (node-parent p)))
      ((or (empty? p)
           (not (node-simplified? p))))
    (set-node-simplified?! p #f)))

;-------------------------------------------------------------------------
; Syntax for defining the different types of nodes.

(define-syntax define-node-type
  (lambda (form rename compare)
    (let ((id (cadr form))
	  (slots (cddr form)))
      (let ((pred (concatenate-symbol id '- 'node?)))
	`(begin (define (,pred x)
		  (eq? ',id (node-variant x)))
		. ,(do ((i 0 (+ i 1))
			(s slots (cdr s))
			(r '() (let ((n (concatenate-symbol id '- (car s)))
				     (f (concatenate-symbol 'node-stuff- i)))
				 `((define-node-field ,n ,pred ,f)
				   . ,r))))
		       ((null? s) (reverse r))))))))

; These are used to rename the NODE-STUFF fields of particular node variants.

(define-syntax define-node-field
  (lambda (form rename compare)
    (let ((id (cadr form))
	  (predicate (caddr form))
	  (field (cadddr form)))
      `(begin
	 (define (,id node)
	   (,field (enforce ,predicate node)))
	 (define (,(concatenate-symbol 'set- id '!) node val)
	   (,(concatenate-symbol 'set- field '!)
	    (enforce ,predicate node)
	    val))))))

;-------------------------------------------------------------------------
; literals

(define-node-type literal
  value  ; the value
  type   ; the type of the value
  refs   ; either #F or a literal-reference record; only a few types of literal
  )      ; literal values require reference lists

(define-record-type literal-reference
  ()
  ((list '())  ; list of literal nodes that refer to a particular value
   ))

(define make-literal-reference-list literal-reference-maker)

(define (make-literal-node value type)
  (let ((node (make-node 'literal)))
    (set-literal-value! node value)
    (set-literal-type!  node type)
    (set-literal-refs!  node #f)
    node))

(define (copy-literal-node node)
  (let ((new (make-node 'literal))
        (refs (literal-refs node)))
    (set-literal-value! new (literal-value node))
    (set-literal-type!  new (literal-type  node))
    (set-literal-refs!  new refs)
    (if refs (set-literal-reference-list!
	      refs
	      (cons new (literal-reference-list refs))))
    new))

(define (make-marked-literal value refs)
  (let ((node (make-node 'literal)))
    (set-literal-value!   node value)
    (set-literal-refs!    node refs)
    (set-literal-reference-list! refs
				 (cons node (literal-reference-list refs)))
    node))

;-------------------------------------------------------------------------
; These just contain an identifier.

(define-node-type reference
  variable
  )

(define (make-reference-node variable)
  (let ((node (make-node 'reference)))
    (set-reference-variable! node variable)
    (set-variable-refs! variable (cons node (variable-refs variable)))
    node))

; Literal and reference nodes are leaf nodes as they do not contain any other
; nodes.

(define (leaf-node? n)
  (or (literal-node? n)
      (reference-node? n)))

;--------------------------------------------------------------------------
; Call nodes

(define-node-type call
  primop     ; the primitive being called
  args       ; vector of child nodes
  exits      ; the number of arguments that are continuations
  source     ; source info
  )

; Create a call node with primop P, N children and EXITS exits.

(define (make-call-node primop n exits)
  (let ((node (make-node 'call)))
    (set-call-primop! node primop)
    (set-call-args!   node (make-vector n empty))
    (set-call-exits!  node exits)
    (set-call-source! node #f)
    node))

(define (call-arg call index)
  (vector-ref (call-args call) index))

(define (call-arg-count call)
  (vector-length (call-args call)))

;----------------------------------------------------------------------------
; LAMBDA NODES

(define-node-type lambda
  body       ; the call-node that is the body of the lambda
  variables  ; a list of variable records with #Fs for ignored positions
  source     ; source code for the lambda (if any)
  data       ; a LAMBDA-DATA record (lambdas have more associated data than
  )          ; the other node types.)

(define-subrecord lambda lambda-data lambda-data
  ((name)          ; symbol          (for debugging only)
   id              ; unique integer  (for debugging only)
   (type))         ; PROC, KNOWN-PROC, CONT, or JUMP (maybe ESCAPE at some point)
  ((block #f)      ; either a basic-block (for flow analysis) or a code-block
                   ; (for code generation).
   (env #f)        ; a record containing lexical environment data
   (protocol #f)   ; calling protocol from the source language
   (prev #f)       ; previous node on *LAMBDAS* list
   (next #f)       ; next node on *LAMBDAS* list
   ))

; Doubly linked list of all non-CONT lambdas
(define *lambdas* #f)

(define (initialize-lambdas)
  (set! *lambdas* (make-lambda-node '*lambdas* 'cont '()))
  (link-lambdas *lambdas* *lambdas*))

(define (link-lambdas node1 node2)
  (set-lambda-prev! node2 node1)
  (set-lambda-next! node1 node2))

(define (add-lambda node)
  (let ((next (lambda-next *lambdas*)))
    (link-lambdas *lambdas* node)
    (link-lambdas node next)))

(define (delete-lambda node)
  (link-lambdas (lambda-prev node) (lambda-next node))
  (set-lambda-prev! node #f)
  (set-lambda-next! node #f))

(define (walk-lambdas proc)
  (do ((n (lambda-next *lambdas*) (lambda-next n)))
      ((eq? n *lambdas*))
    (proc n))
  (values))

(define (make-lambda-list)
  (do ((n (lambda-next *lambdas*) (lambda-next n))
       (l '() (cons n l)))
      ((eq? n *lambdas*)
       l)))

(define (add-lambdas nodes)
  (for-each add-lambda nodes))

;    Create a lambda node.  NAME is used as the name of the lambda node's
; self variable.  VARS is a list of variables.  The VARIABLE-BINDER slot
; of each variable is set to be the new lambda node.

(define (make-lambda-node name type vars)
  (let ((node (make-node 'lambda))
        (data (lambda-data-maker name (new-id) type)))
    (set-lambda-body!      node empty)
    (set-lambda-variables! node vars)
    (set-lambda-data!      node data)
    (set-lambda-source!    node #f)
    (for-each (lambda (var)
		(if var (set-variable-binder! var node)))
	      vars)
    (if (neq? type 'cont)
	(add-lambda node))
    node))

; Change the type of lambda-node NODE to be TYPE.  This may require adding or
; deleting NODE from the list *LAMBDAS*.

(define (change-lambda-type node type)
  (let ((has (lambda-type node)))
    (cond ((neq? type (lambda-type node))
	   (set-lambda-type! node type)
	   (cond ((eq? type 'cont)
		  (delete-lambda node))
		 ((eq? has 'cont)
		  (add-lambda node)))))
    (values)))

(define (lambda-variable-count node)
  (length (lambda-variables node)))

(define (calls-known? node)
  (neq? (lambda-type node) 'proc))

(define (set-calls-known?! node)
  (set-lambda-type! node 'known-proc))

(define (proc-lambda? node)
  (or (eq? 'proc (lambda-type node))
      (eq? 'known-proc (lambda-type node))))

