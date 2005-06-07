; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.

; Code to turn PROC lambdas into JUMP lambdas.

; FIND-JUMP-PROCS returns two lists.  The first contains lists of the form
;    ((<proc-lambda> ...) (<var> ...) <continuation>)
; indicating that these lambda nodes, bound to the given variables, are all
; called with <continuation> as their only continuation.  The second list
; is of procedures that are called only by each other.  The procedures in
; the second list are deleted.  Those in the first list are converted to
; JUMP lambdas
;
; INTEGRATE-JUMP-PROCS! returns #T if any change is made to the program.

(define (integrate-jump-procs!)
  (receive (hits useless)
      (find-jump-procs (filter proc-lambda?
			       (make-lambda-list))
		       find-calls)
    (remove-unused-procedures! useless)
    (for-each (lambda (p)
		(procs->jumps (cdr p)
			      (map bound-to-variable (cdr p))
			      (car p)))
	      hits)
    (or (not (null? hits))
	(not (null? useless)))))

; We want to find subsets of ALL-PROCS such that all elements of a subset
; are always called with the same continuation.  (PROC->USES <proc>) returns
; the references to <proc> that are calls, or #f if there are references that
; are not calls.
;
; We proceed as follows:
;  1. Partition the procs depending on whether all their calls are known or not.
;  2. Build a call graph:
;       Nodes represent either procedures or continuations.  If there is a
;       tail-recursive call to procedure B in procedure A, then there is an
;       edge from A to B.  For continuation C such that there is a call in
;       procedure A to procedure B with that continuation, there are edges
;       from A to C and from C to B.
;         In other words, it is a call graph where the edges that represent
;       non-tail-recursive calls are replaced by two edges, with a node for
;       the continuation in between.
;         There is a special root node (representing `outside'), that has
;       edges to the nodes representing procedures whose call sites have not
;       been identified.
;  3. Determine the dominance frontiers in the graph.
;  4. Find the nodes in the graph that are reachable from more than one
;     continuation (the joins).
;  5. Starting from each node that represents a continuation (the joins,
;     procs whose calls aren't known, and the continuations themselves),
;     find the set of nodes reachable from that node without going through
;     some other continuation node.

(define (find-jump-procs all-procs proc->uses)
  (for-each (lambda (l)
	      (set-lambda-block! l (make-node l #f)))
	    all-procs)
  (receive (known unknown)
      (partition-list calls-known? all-procs)
    (let ((root (make-node #f #f))
	  (conts-cell (list '()))
	  (known-blocks (map lambda-block known))
	  (procs-cell (list (map lambda-block unknown))))
      (note-calls! known conts-cell procs-cell proc->uses)
      (let ((unknown-blocks (car procs-cell))
	    (conts (car conts-cell)))
	(set-node-successors! root unknown-blocks)
	(graph->ssa-graph! root node-successors node-temp set-node-temp!)
	(let ((joins (find-joins (append conts unknown-blocks) node-temp)))
	  (for-each (lambda (n)
		      (set-node-join?! n #t))
		    joins)
	  (let* ((mergable (filter-map find-mergable
				       (append joins unknown-blocks conts)))
		 (useless  (filter (lambda (p)
				     (not (or (node-join? (lambda-block p))
					      (node-merged? (lambda-block p)))))
				   known)))
	    (for-each (lambda (p)
			(set-lambda-block! p #f))
		      all-procs)
	    (values mergable useless)))))))

; Make a call graph with extra nodes inserted for continuations:
;
;  If F calls G tail-recursively, add an edge F->G
;  If F calls G ... with continuation K, add a node K and edges F->K, K->G ...
;
; Then FIND-JOINS will return a list of the nodes that are passed two or
; more distinct continuations.  The rest can be merged with their callers.
;
; Need a root node, so make one that points to all procs with unknown calls.

(define-record-type node :node
  (really-make-node proc cont successors join? merged?)
  node?
  (proc node-proc)             ; lambda node (or #f for continuation holders)
  (cont node-cont)             ; lambda node (or #f for procs)
  (successors node-successors set-node-successors!)
  (temp node-temp set-node-temp!)
  (join? node-join? set-node-join?!)
  (merged? node-merged? set-node-merged?!))

(define (make-node proc cont)
  (really-make-node proc cont '() #f #f))

(define-record-discloser :node
  (lambda (node)
    (list 'node (node-proc node) (node-cont node))))

(define (add-child! parent child)
  (if (not (memq? child (node-successors parent)))
      (set-node-successors! parent
			    (cons child
				  (node-successors parent)))))

; Walk KNOWN-PROCS adding edges to the call graph.

(define (note-calls! known-procs conts-cell procs-cell proc->uses)
  (for-each (lambda (proc)
	      (for-each (lambda (ref)
			  (note-call! (lambda-block proc)
				      ref
				      conts-cell procs-cell))
			(proc->uses proc)))
	    known-procs))

; Add an edge from the node containing REF to PROC-NODE.  Tail calls add an
; edge directly from the calling node, non-tail calls add an edge from the
; successor to the calling node that represents the call's continuation.

(define (note-call! proc-node ref conts-cell procs-cell)
  (let ((caller (get-lambda-block (containing-procedure ref) procs-cell)))
    (add-child! (if (calls-this-primop? (node-parent ref) 'tail-call)
		    caller
		    (get-cont-block caller
				    (call-arg (node-parent ref) 0)
				    conts-cell))
		proc-node)))

; Get the block for lambda-node PROC, making a new one if necessary.

(define (get-lambda-block proc procs-cell)
  (let ((block (lambda-block proc)))
    (if (node? block)
	block
	(let ((new (make-node proc #f)))
	  (set-lambda-block! proc new)
	  (set-car! procs-cell (cons new (car procs-cell)))
	  new))))

; Get the successor to CALLER containing CONT, making it if necessary.

(define (get-cont-block caller cont conts-cell)
  (or (any (lambda (node)
	     (and (node-cont node)
		  (node-equal? cont (node-cont node))))
	   (node-successors caller))
      (let ((cont-node (make-node #f cont)))
	(set-car! conts-cell (cons cont-node (car conts-cell)))
	(add-child! caller cont-node)
	cont-node)))

;----------------

(define (find-mergable node)
  (let ((mergable (really-find-mergable node)))
    (if (null? mergable)
	#f
	(cons (or (node-cont node)
		  (car (variable-refs
			 (car (lambda-variables (node-proc node))))))
	      mergable))))

(define (really-find-mergable node)    
  (let recur ((nodes (node-successors node)) (res '()))
    (if (null? nodes)
	res
	(recur (cdr nodes)
	       (let ((node (car nodes)))
		 (cond ((or (node-join? node)    ; gets two or more continuations
			    (node-merged? node)  ; already merged
			    (node-cont node))    ; different continuation
			res)
;		       ((node-cont node)          ; not a lambda
;			(recur (node-successors node) res))
		       (else
			(set-node-merged?! node #t)
			(recur (node-successors node)
			       (cons (node-proc node) res)))))))))

;----------------
; Part 2.  PROCS is a list of procedures that are only called by each other;
; with no entry point they are useless and can be removed.

(define (remove-unused-procedures! procs)
  (for-each (lambda (proc)
	      (let ((var (bound-to-variable proc)))
		(if (not var)
		    (bug "known procedure has no variable ~S" proc))
		(format #t "Removing unused procedure: ~S~%"
			(variable-name var)) ; would LAMBDA-NAME be better?
		(mark-changed (node-parent proc))
		(detach-bound-value var proc)
		(erase proc)))
	    procs))

;----------------
; Part 3.  Turn JUMP-PROCS from procs to jumps.  CONT is the continuation they
; all receive, and is also turned into a jump.

; This creates a LETREC to bind all CONT and any of JUMP-PROCS that are
; passed CONT directly and are bound above the LCA of all calls to JUMP-PROCS
; that use CONT.  Then every jump-proc is changed from a proc lambda to a
; jump lambda and has its continuation removed.  Returns are replaced with
; jumps to CONT.  If CONT is not a variable some protocol adjustment may be
; required.

(define (procs->jumps jump-procs vars cont)
  (receive (called-vars called-procs lca)
      (find-cont-uses cont vars jump-procs)
    (let ((proc (containing-procedure cont))
	  (lca (if (call-node? lca) lca (node-parent lca)))
	  (cvar (if (lambda-node? cont)
		    (make-variable 'w (node-type cont))
		    #f)))
      (receive (called-vars called-procs)
	  (bound-above? lca called-vars called-procs)
	(for-each detach-bound-value called-vars called-procs)
	(cond ((lambda-node? cont)
	       (determine-continuation-protocol cont jump-procs)
	       (let ((cont-copy (copy-node-tree cont)))
		 (change-lambda-type cont-copy 'jump)
		 (put-in-letrec (cons cvar
				      called-vars)
				(cons cont-copy
				      called-procs)
				lca)))
	      (else
	       (put-in-letrec called-vars called-procs lca))))
      (for-each proc-calls->jumps jump-procs)
      (for-each (lambda (p)
		  (let* ((v (car (lambda-variables p)))
			 (refs (variable-refs v)))
		    (set-variable-refs! v '())
		    (for-each (lambda (r)
				(if (lambda-node? cont)
				    (return->jump (node-parent r) cvar)
				    (replace r (make-reference-node
						(car (lambda-variables proc))))))
			      refs)
		    (remove-variable p v)))
		jump-procs)
      (values))))

; Returns those of VARS and VALS where there is a call to the variable that
; passes CONT as a continuation, or where the variable is not bound.  The
; third value returned is the least-common-ancestor of all calls to VARS
; that use CONT.
;
; Why exclude uncalled variables just because they are bound?

(define (find-cont-uses cont vars vals)
  (let loop ((vars vars) (vals vals) (r-vars '()) (r-vals '()) (uses '()))
    (if (null? vars)
	(values r-vars
		r-vals
		(least-common-ancestor uses))
	(let ref-loop ((refs (variable-refs (car vars))) (my-uses uses))
	  (cond ((not (null? refs))
		 (ref-loop (cdr refs)
			   (if (node-equal? cont
					    (call-arg (node-parent (car refs))
						      0))
			       (cons (car refs) my-uses)
			       my-uses)))
; Why was this here?  It breaks for some examples.
;                ((and (variable-binder (car vars))
;                      (eq? my-uses uses))
;                 (loop (cdr vars) (cdr vals) r-vars r-vals uses))
		(else
		 (loop (cdr vars) (cdr vals)
		       (cons (car vars) r-vars)
		       (cons (car vals) r-vals)
		       my-uses)))))))

; Return the list of VARS and VALS where the variable is either global
; or bound above CALL.

(define (bound-above? call vars vals)
  (set-node-flag! call #t)
  (let loop ((vars vars) (vals vals) (r-vars '()) (r-vals '()))
    (cond ((null? vars)
	   (set-node-flag! call #f)
	   (values r-vars r-vals))
	  ((and (variable-binder (car vars))
		(marked-ancestor (variable-binder (car vars))))
	   (loop (cdr vars) (cdr vals) r-vars r-vals))
	  (else
	   (loop (cdr vars) (cdr vals)
		 (cons (car vars) r-vars)
		 (cons (car vals) r-vals))))))

(define (detach-bound-value var node)
  (if (variable-binder var)
      (let ((binder (variable-binder var))
	    (parent (node-parent node))
	    (index (node-index node)))
	(set-lambda-variables! binder (delq! var (lambda-variables binder)))
	(detach node)
	(remove-call-arg parent index))))
  
; Turn all calls to PROC into jumps.

(define (proc-calls->jumps proc)
  (for-each (lambda (n)
	      (call->jump (node-parent n)))
	    (find-calls proc))
  (change-lambda-type proc 'jump))

; Change a call to a jump by changing the primop and removing the continuation.

(define (call->jump call)
  (case (primop-id (call-primop call))
    ((call tail-call)
     (set-call-primop! call (get-primop (enum primop jump)))
     (remove-call-arg call 0))
    (else
     (bug "odd call primop ~S" (call-primop call)))))

; Change a return to a jump.  VAR is a variable bound to JUMP, the lambda
; being jumped to.

(define (return->jump call var)
  (case (primop-id (call-primop call))
    ((return)
     (set-call-primop! call (get-primop (enum primop jump)))
     (replace (call-arg call 0) (make-reference-node var)))
    (else
     (bug "odd return primop ~S" (call-primop call)))))
