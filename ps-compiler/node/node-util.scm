 
; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.

; This file contains miscellaneous utilities for accessing and modifying the
; node tree.

; Get the root of the tree containing node.

(define (node-base node)
  (do ((p node (node-parent p)))
      ((not (node? (node-parent p)))
       p)))

; Find the procedure node that contains NODE.  Go up one parent at a time
; until a lambda node is found, then go up two at a time, skipping the
; intervening call nodes.

(define (containing-procedure node)
  (do ((node (node-parent node) (node-parent node)))
      ((lambda-node? node)
       (do ((node node (node-parent (node-parent node))))
	   ((proc-lambda? node) node)))))

; Trivial calls are those whose parents are call nodes.

(define (trivial? call)
  (call-node? (node-parent call)))

(define (nontrivial? call)
  (lambda-node? (node-parent call)))

(define (nontrivial-ancestor call)
  (let loop ((call call))
    (if (or (not (node? (node-parent call)))
	    (nontrivial? call))
	call
	(loop (node-parent call)))))

(define (calls-this-primop? call id)
  (eq? id (primop-id (call-primop call))))

; Return the variable to which a value is bound by LET or LETREC.

(define (bound-to-variable node)
  (let ((parent (node-parent node)))
    (case (primop-id (call-primop parent))
      ((let)
       (if (n= 0 (node-index node))
	   (list-ref (lambda-variables (call-arg parent 0))
		     (- (node-index node) 1))
	   #f))
      ((letrec2)
       (if (< 1 (node-index node))
	   (list-ref (lambda-variables
		      (variable-binder
		       (reference-variable (call-arg parent 1))))
		     (- (node-index node) 1))
	   #f))
      (else #f))))

; Return a list of all the reference to lambda-node L's value that call it.
; If not all can be identified then #F is returned.

(define (find-calls l)
  (let ((refs (cond ((bound-to-variable l)
		     => variable-refs)
		    ((called-node? l)
		     (list l))
		    (else
		     #f))))
    (cond ((and refs (every? called-node? refs))
	   refs)
	  ((calls-known? l)
	   (bug "cannot find calls for known lambda ~S" l))
	  (else #f))))

; Walk (or map) a tree-modifying procedure down a variable's references.

(define (walk-refs-safely proc var)
  (for-each proc (copy-list (variable-refs var))))

; Return #t if the total primop-cost of NODE is less than SIZE.

(define (small-node? node size)
  (let label ((call (lambda-body node)))
    (set! size (- size (primop-cost call)))
    (if (>= size 0)
	(walk-vector (lambda (n)
		       (cond ((lambda-node? n)
			      (label (lambda-body n)))
			     ((call-node? n)
			      (label n))))
		     (call-args call))))
  (>= size 0))

; True if executing NODE involves side-effects.

(define (side-effects? node . permissible)
  (let ((permissible (cons #f permissible)))
    (let label ((node node))
      (cond ((not (call-node? node))
	     #f)
	    ((and (= 0 (call-exits node))
		  (memq (primop-side-effects (call-primop node))
			permissible))
	     (let loop ((i (- (call-arg-count node) 1)))
	       (cond ((< i 0) #f)
		     ((label (call-arg node i)) #t)
		     (else (loop (- i 1))))))
	    (else
	     #t)))))

; A conservative check - is there only one SET-CONTENTS call for the owner and
; are all calls between CALL and the LETREC call that binds the owner calls to
; SET-CONTENTS?

;(define (single-letrec-set? call)
;  (let ((owner (call-arg call set/owner)))
;    (and (reference-node? owner)
;	 (every? (lambda (ref)
;		   (or (eq? (node-parent ref) call)
;		       (not (set-reference? ref))))
;		 (variable-refs (reference-variable owner))))))
    
;(define (set-reference? node)
;  (and (eq? 'set-contents
;	    (primop-id (call-primop (node-parent node))))
;       (= (node-index node) set/owner)))

;-------------------------------------------------------------------------------

(define the-undefined-value (list '*undefined-value*))

(define (undefined-value? x)
  (eq? x the-undefined-value))

(define (undefined-value-node? x)
  (and (literal-node? x)
       (undefined-value? (literal-value x))))

(define (make-undefined-literal)
  (make-literal-node the-undefined-value #f))

;-------------------------------------------------------------------------------
; Finding the lambda node called by CALL, JUMP, or RETURN

(define (called-node? node)
  (and (node? (node-parent node))
       (eq? node (called-node (node-parent node)))))
						   
(define (called-node call)
  (cond ((and (primop-procedure? (call-primop call))
	      (primop-call-index (call-primop call)))
	 => (lambda (i)
	      (call-arg call i)))
	(else '#f)))

(define (called-lambda call)
  (get-lambda-value (call-arg call (primop-call-index (call-primop call)))))

(define (get-lambda-value value)
  (cond ((lambda-node? value)
	 value)
	((reference-node? value)
	 (get-variable-lambda (reference-variable value)))
	(else
	 (error "peculiar procedure in ~S" value))))

(define (get-variable-lambda variable)
  (if (global-variable? variable)
      (or (variable-known-lambda variable)
	  (error "peculiar procedure variable ~S" variable))
      (let* ((binder (variable-binder variable))
	     (index (node-index binder))
	     (call (node-parent binder))
	     (lose (lambda ()
		     (error "peculiar procedure variable ~S" variable))))
	(case (primop-id (call-primop call))
	  ((let)
	   (if (= 0 index)
	       (get-lambda-value (call-arg call (+ 1 (variable-index variable))))
	       (lose)))
	  ((letrec1)
	   (if (= 0 index)
	       (get-letrec-variable-lambda variable)
	       (lose)))
	  ((call)
	   (if (and (= 1 index)
		    (= 0 (variable-index variable))) ; var is a continuation var
	       (get-lambda-value (call-arg call 0))
	       (lose)))
	  (else
	   (lose))))))

; Some of the checking can be removed once I know the LETREC code works.

(define (get-letrec-variable-lambda variable)
  (let* ((binder (variable-binder variable))
	 (call (lambda-body binder)))
    (if (and (eq? 'letrec2 (primop-id (call-primop call)))
	     (reference-node? (call-arg call 1))
	     (eq? (car (lambda-variables binder))
		  (reference-variable (call-arg call 1))))
	(call-arg call (+ 1 (variable-index variable)))
	(error "LETREC is incorrectly organized ~S" (node-parent binder)))))

;(define (get-cell-variable-lambda variable)
;  (let ((ref (first set-reference? (variable-refs variable))))
;    (if (and ref
;	     (eq? 'letrec
;		  (literal-value (call-arg (node-parent ref) set/type))))
;	(get-lambda-value (call-arg (node-parent ref) set/value))
;	(error "peculiar lambda cell ~S" variable))))
  
;-------------------------------------------------------------------------------
; Attaching and detaching arguments to calls

; Make ARGS the arguments of call node PARENT.  ARGS may contain #f.

(define (attach-call-args parent args)
  (let ((len (call-arg-count parent)))
    (let loop ((args args) (i 0))
      (cond ((null? args)
             (if (< i (- len 1))
                 (bug '"too few arguments added to node ~S" parent))
             (values))
            ((>= i len)
             (bug '"too many arguments added to node ~S" parent))
            (else
	     (if (car args)
		 (attach parent i (car args)))
	     (loop (cdr args) (+ 1 i)))))))

; Remove all of the arguments of NODE.

(define (remove-call-args node)
  (let ((len (call-arg-count node)))
    (do ((i 1 (+ i 1)))
        ((>= i len))
      (if (not (empty? (call-arg node i)))
          (erase (detach (call-arg node i)))))
    (values)))

; Replace the arguments of call node NODE with NEW-ARGS.

(define (replace-call-args node new-args)
  (let ((len (length new-args)))
    (remove-call-args node)
    (if (n= len (call-arg-count node))
        (let ((new (make-vector len empty))
              (old (call-args node)))
          (set-call-args! node new)))
    (attach-call-args node new-args)))

; Remove all arguments to CALL that are EMPTY?.  COUNT is the number of
; non-EMPTY? arguments.

(define (remove-null-arguments call count)
  (let ((old (call-args call))
        (new (make-vector count empty)))
    (let loop ((i 0) (j 0))
      (cond ((>= j count)
	     (values))
            ((not (empty? (vector-ref old i)))
             (set-node-index! (vector-ref old i) j)
             (vector-set! new j (vector-ref old i))
             (loop (+ i 1) (+ j 1)))
            (else
             (loop (+ i 1) j))))
    (set-call-args! call new)
    (values)))

; Remove all but the first COUNT arguments from CALL.

(define (shorten-call-args call count)
  (let ((old (call-args call))
        (new (make-vector count empty)))
    (vector-replace new old count)
    (do ((i (+ count 1) (+ i 1)))
        ((>= i (vector-length old)))
      (erase (vector-ref old i)))
    (set-call-args! call new)
    (values)))

; Insert ARG as the INDEXth argument to CALL.

(define (insert-call-arg call index arg)
  (let* ((old (call-args call))
         (len (vector-length old))
         (new (make-vector (+ 1 len) empty)))
    (vector-replace new old index)
    (do ((i index (+ i 1)))
        ((>= i len))
      (vector-set! new (+ i 1) (vector-ref old i))
      (set-node-index! (vector-ref old i) (+ i 1)))
    (set-call-args! call new)
    (attach call index arg)
    (values)))

; Remove the INDEXth argument to CALL.

(define (remove-call-arg call index)
  (let* ((old (call-args call))
         (len (- (vector-length old) 1))
         (new (make-vector len)))
    (vector-replace new old index)
    (if (node? (vector-ref old index))
        (erase (detach (vector-ref old index))))
    (do ((i index (+ i 1)))
        ((>= i len))
      (vector-set! new i (vector-ref old (+ i 1)))
      (set-node-index! (vector-ref new i) i))
    (set-call-args! call new)
    (if (< index (call-exits call))
        (set-call-exits! call (- (call-exits call) 1)))
    (values)))

; Add ARG to the end of CALL's arguments.

(define (append-call-arg call arg)
  (insert-call-arg call (call-arg-count call) arg)) 

; Replace CALL with the body of its continuation.

(define (remove-body call)
  (if (n= 1 (call-exits call))
      (bug "removing a call with ~D exits" (call-exits call))
      (replace-body call (detach-body (lambda-body (call-arg call 0))))))

; Avoiding N-Ary Procedures
; These are used in the expansion of the LET-NODES macro.

(define (attach-two-call-args node a0 a1)
  (attach node 0 a0)
  (attach node 1 a1))

(define (attach-three-call-args node a0 a1 a2)
  (attach node 0 a0)
  (attach node 1 a1)
  (attach node 2 a2))

(define (attach-four-call-args node a0 a1 a2 a3)
  (attach node 0 a0)
  (attach node 1 a1)
  (attach node 2 a2)
  (attach node 3 a3))

(define (attach-five-call-args node a0 a1 a2 a3 a4)
  (attach node 0 a0)
  (attach node 1 a1)
  (attach node 2 a2)
  (attach node 3 a3)
  (attach node 4 a4))

;-------------------------------------------------------------------------------
; Bind VARS to VALUES using letrec at CALL.  If CALL is already a letrec
; call, just add to it, otherwise make a new one.

(define (put-in-letrec vars values call)
  (cond ((eq? 'letrec2 (primop-id (call-primop call)))
	 (let ((binder (node-parent call)))
	   (mark-changed call)
	   (for-each (lambda (var)
		       (set-variable-binder! var binder))
		     vars)
	   (set-lambda-variables! binder
				  (append (lambda-variables binder) vars))
	   (for-each (lambda (value)
		       (append-call-arg call value))
		     values)))
	(else
	 (move-body
	  call
	  (lambda (call)
	    (receive (letrec-call letrec-cont)
		(make-letrec vars values)
	      (attach-body letrec-cont call)
	      letrec-call))))))

(define (make-letrec vars vals)
  (let ((cont (make-lambda-node 'c 'cont '())))
    (let-nodes ((call (letrec1 1 l2))
		(l2 ((x #f) . vars) (letrec2 1 cont (* x) . vals)))
      (values call cont))))

;-------------------------------------------------------------------------------
; Changing lambda-nodes' variable lists

(define (remove-lambda-variable l-node index)
  (remove-variable l-node (list-ref (lambda-variables l-node) index)))

(define (remove-variable l-node var)
  (if (used? var)
      (bug '"cannot remove referenced variable ~s" var))
  (erase-variable var)
  (let ((vars (lambda-variables l-node)))
    (if (eq? (car vars) var)
        (set-lambda-variables! l-node (cdr vars))
        (do ((vars vars (cdr vars)))
            ((eq? (cadr vars) var)
             (set-cdr! vars (cddr vars)))))))

; Remove all of L-NODES' unused variables.

(define (remove-unused-variables l-node)
  (set-lambda-variables! l-node
			 (filter! (lambda (v)
				    (cond ((used? v)
					   #t)
					  (else
					   (erase-variable v)
					   #f)))
				  (lambda-variables l-node))))

;------------------------------------------------------------------------------
; Substituting Values For Variables

; Substitute VAL for VAR.  If DETACH? is true then VAL should be detached
; and so can be used instead of a copy for the first substitution.
;
; If VAL is a reference to a variable named V, it was probably introduced by
; the CPS conversion code.  In that case, the variable is renamed with the
; name of VAR.  This helps considerably when debugging the compiler.

(define (substitute var val detach?)
  (if (and (reference-node? val)
           (eq? 'v (variable-name (reference-variable val)))
	   (not (global-variable? (reference-variable val))))
      (set-variable-name! (reference-variable val)
           (variable-name var)))
  (let ((refs (variable-refs var)))
    (set-variable-refs! var '())
    (cond ((not (null? refs))
           (for-each (lambda (ref)
		       (replace ref (copy-node-tree val)))
		     (if detach? (cdr refs) refs))
           (if detach? (replace (car refs) (detach val))))
          (detach?
           (erase (detach val))))))

; Walk the tree NODE replacing references to variables in OLD-VARS with
; the corresponding variables in NEW-VARS.  Uses VARIABLE-FLAG to mark
; the variables being replaced.

(define (substitute-vars-in-node-tree node old-vars new-vars)
  (for-each (lambda (old new)
	      (set-variable-flag! old new))
	    old-vars
	    new-vars)
  (let tree-walk ((node node))
    (cond ((lambda-node? node)
           (walk-vector tree-walk (call-args (lambda-body node))))
          ((call-node? node)
           (walk-vector tree-walk (call-args node)))
          ((and (reference-node? node)
                (variable-flag (reference-variable node)))
           => (lambda (new)
                (replace node (make-reference-node new))))))
  (for-each (lambda (old) 
	      (set-variable-flag! old #f))
	    old-vars))

; Replaces the call node CALL with VALUE.
; (<proc> <exit> . <args>) => (<exit> <value>)

(define (replace-call-with-value call value)
  (cond ((n= 1 (call-exits call))
         (bug '"can only substitute for call with one exit ~s" call))
        (else
	 (let ((cont (detach (call-arg call 0))))
	   (set-call-exits! call 0)
	   (replace-call-args call (if value (list cont value) (list cont)))
	   (set-call-primop! call (get-primop (enum primop let)))))))

;------------------------------------------------------------------------------
; Copying Node Trees

; Copy the node-tree NODE.  This dispatches on the type of NODE.
; Variables which have been copied have the copy in the node-flag field.

(define (copy-node-tree node)
  (let ((new (cond ((lambda-node? node)
		    (copy-lambda node))
		   ((reference-node? node)
		    (let ((var (reference-variable node)))
		      (cond ((and (variable-binder var)
				  (variable-flag var))
			     => make-reference-node)
			    (else
			     (make-reference-node var)))))
		   ((call-node? node)
		    (copy-call node))
		   ((literal-node? node)
		    (copy-literal-node node)))))
    new))

; Copy a lambda node and its variables.  The variables' copies are put in
; their VARIABLE-FLAG while the lambda's body is being copied.

(define (copy-lambda node)
  (let* ((vars (map (lambda (var)
		      (if var
			  (let ((new (copy-variable var)))
			    (set-variable-flag! var new)
			    new)
			  #f))
		    (lambda-variables node)))
         (new-node (make-lambda-node (lambda-name node)
				     (lambda-type node)
				     vars)))
    (attach-body new-node (copy-call (lambda-body node)))
    (set-lambda-protocol! new-node (lambda-protocol node))
    (set-lambda-source! new-node (lambda-source node))
    (for-each (lambda (var)
		(if var (set-variable-flag! var #f)))
	      (lambda-variables node))
    new-node))

(define (copy-call node)
  (let ((new-node (make-call-node (call-primop node)
				  (call-arg-count node)
				  (call-exits node))))
    (do ((i 0 (+ i 1)))
        ((>= i (call-arg-count node)))
      (attach new-node i (copy-node-tree (call-arg node i))))
    (set-call-source! new-node (call-source node))
    new-node))

;------------------------------------------------------------------------------
; Checking the scoping of identifers

; Mark all ancestors of N with FLAG

(define (mark-ancestors n flag)
  (do ((n n (node-parent n)))
      ((not (node? n)) (values))
    (set-node-flag! n flag)))

; Does N have an ancestor with a non-#f flag?

(define (marked-ancestor? n)
  (do ((n n (node-parent n)))
      ((or (not (node? n))
           (node-flag n))
       (node? n))))

; Does N have an ancestor with a #f flag?

(define (unmarked-ancestor? n)
  (do ((n n (node-parent n)))
      ((or (not (node? n))
           (not (node-flag n)))
       (node? n))))

; Is ANC? an ancestor of NODE?

(define (node-ancestor? anc? node)
  (set-node-flag! anc? #t)
  (let ((okay? (marked-ancestor? node)))
    (set-node-flag! anc? #f)
    okay?))

; Find the lowest ancestor of N that has a non-#f flag

(define (marked-ancestor n)
  (do ((n n (node-parent n)))
      ((or (not (node? n))
           (node-flag n))
       (if (node? n) n #f))))

; Mark the ancestors of START with #f, stopping when END is reached

(define (unmark-ancestors-to start end)
  (do ((node start (node-parent node)))
      ((eq? node end))
    (set-node-flag! node #f)))

; Return the lowest node that is above all NODES

(define (least-common-ancestor nodes)
  (mark-ancestors (car nodes) #t)
  (let loop ((nodes (cdr nodes)) (top (car nodes)))
    (cond ((null? nodes)
           (mark-ancestors top #f)
           top)
          (else
           (let ((new (marked-ancestor (car nodes))))
             (unmark-ancestors-to top new)
             (loop (cdr nodes) new))))))

; Can TO be moved to FROM without taking variables out of scope.
; This first marks all of the ancestors of FROM, and then unmarks all of the
; ancestors of TO.  The net result is to mark every node that is above FROM but
; not above TO.  Then if any reference-node below FROM references a variable
; with a marked binder, that node, and thus FROM itself, cannot legally be
; moved to TO.

; This is not currently used anywhere, and it doesn't know about trivial
; calls.

(define (hoistable-node? from to)
  (let ((from (if (call-node? from)
		  (node-parent (nontrivial-ancestor from))
		  from)))
    (mark-ancestors (node-parent from) #t)
    (mark-ancestors to #f)
    (let ((okay? (let label ((n from))
                   (cond ((lambda-node? n)
                          (let* ((vec (call-args (lambda-body n)))
                                 (c (vector-length vec)))
                            (let loop ((i 0))
                              (cond ((>= i c) #t)
                                    ((label (vector-ref vec i))
                                     (loop (+ i 1)))
                                    (else #f)))))
                         ((reference-node? n)
                          (let ((b (variable-binder (reference-variable n))))
                            (or (not b) (not (node-flag b)))))
                         (else #t)))))
      (mark-ancestors (node-parent from) #f)
      okay?)))

; Mark all of the lambda nodes which bind variables referenced below NODE.

(define (mark-binders node)
  (let label ((n node))
    (cond ((lambda-node? n)
           (walk-vector label (call-args (lambda-body n))))
          ((reference-node? n)
           (let ((b (variable-binder (reference-variable n))))
             (if b (set-node-flag! b #f))))))
  (values))


;------------------------------------------------------------------------------
; For each lambda-node L this sets (PARENT L) to be the enclosing PROC node
; of L and, if L is a PROC node, sets (KIDS L) to be the lambda nodes it
; encloses.

(define (find-scoping lambdas parent set-parent! kids set-kids!)
  (receive (procs others)
      (partition-list proc-lambda? lambdas)
    (for-each (lambda (l)
		(set-parent! l #f)
		(set-kids!   l '()))
	      procs)
    (for-each (lambda (l)
		(set-parent! l #f))
	      others)
    (letrec ((set-lambda-parent!
	      (lambda (l)
		(cond ((parent l)
		       => identity)
		      ((proc-ancestor l)
		       => (lambda (p)
			    (let ((p (if (proc-lambda? p)
					 p
					 (set-lambda-parent! p))))
			      (set-kids! p (cons l (kids p)))
			      (set-parent! l p)
			      p)))
		      (else #f)))))
      (for-each set-lambda-parent! lambdas))
    (values procs others)))

(define (proc-ancestor node)
  (let ((p (node-parent node)))
    (if (not (node? p))
        #f
        (let ((node (do ((p p (node-parent p)))
			((lambda-node? p)
			 p))))
          (do ((node node (node-parent (node-parent node))))
              ((proc-lambda? node)
	       node))))))

(define (no-free-references? node)
  (if (call-node? node)
      (error "NO-FREE-REFERENCES only works on value nodes: ~S" node))
  (let label ((node node))
    (cond ((reference-node? node)
	   (let ((b (variable-binder (reference-variable node))))
	     (or (not b)
		 (node-flag b))))
	  ((lambda-node? node)
	   (set-node-flag! node #t)
	   (let* ((vec (call-args (lambda-body node)))
		  (res (let loop ((i (- (vector-length vec) 1)))
			 (cond ((< i 0) #t)
			       ((not (label (vector-ref vec i))) #f)
			       (else (loop (- i 1)))))))
	     (set-node-flag! node #f)
	     res))
	  (else
	   #t))))

(define (node-type node)
  (cond ((literal-node? node)
	 (literal-type node))
	((reference-node? node)
	 (variable-type (reference-variable node)))
	((lambda-node? node)
	 (lambda-node-type node))
	((and (call-node? node)
	      (primop-trivial? (call-primop node)))
	 (trivial-call-return-type node))
	(else
	 (error "node ~S does not represent a value" node))))

;----------------------------------------------------------------
; Debugging utilities

(define (show-simplified node)
  (let loop ((n node) (r '()))
    (if (node? n)
	(loop (node-parent n) (cons (node-simplified? n) r))
	(reverse r))))

(define (show-flag node)
  (let loop ((n node) (r '()))
    (if (node? n)
        (loop (node-parent n) (cons (node-flag n) r))
	(reverse r))))

(define (reset-simplified node)
  (let loop ((n node))
    (cond ((node? n)
	   (set-node-simplified?! n #f)
	   (loop (node-parent n))))))
