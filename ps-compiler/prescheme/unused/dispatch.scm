; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


(define (dispatches->computed-gotos forms)
  (walk (lambda (f)
	  (if (and (form-var f)
		   (eq? 'initialize (form-type f)))
	      (let ((value (thunk-value (form-node f))))
		(if (and value
			 (call-node? value)
			 (eq? 'make-dispatch-table
			      (primop-id (call-primop value))))
		    (dispatch->computed-goto f value)))))
	forms)
  forms)

; FORM defines a dispatch vector, created in MAKE-CALL.  This replaces the
; dispatch vector with a procedure that performs a computed goto.
; The references to VAR, the variable to which the dispatch
; vector is bound, are analysed to find all of the uses of the vector and all
; of the dispatch procedures added to it.  These procedures are put in the
; appropriate slot of VEC, a model of the dispatch vector.
;   Once all of the references are known, the dispatch vector is replaced with
; a procedure and all of the dispatches are replaced with calls.  The dispatch
; procedures from VEC become the continuations of the computed-goto call.

(define (dispatch->computed-goto form make-call)
  (initialize-lambdas)
  (let* ((var (form-var form))
	 (size-node (call-arg make-call 0))
	 (size (if (and (literal-node? size-node)
			(integer? (literal-value size-node))
			(>= (literal-value size-node) 0))
		   (literal-value size-node)
		   (error "dispatch vector ~A's size is not a positive integer"
			  (variable-name (form-var form)))))
	 (vec (make-vector size #f)))
    (analyze-dispatch-vector var vec)
    (add-lambdas (form-lambdas form))
    (let ((goto-proc (make-computed-goto var vec size make-call)))
      (for-each (lambda (ref)
		  (let ((call (node-parent ref)))
		    (set-call-primop! call (get-primop (enum primop unknown-call)))
		    (insert-call-arg call 2 (make-literal-node 1 #f))))
		(variable-refs var))
      (set-form-node! form goto-proc (make-lambda-list))
      (set-form-type! form 'lambda)
      (values)))))

; Possible problems with a call to DEFINE-DISPATCH are:
; The index is not an integer constant in the right range.
; The procedure value cannot be moved.

(define (analyze-dispatch-vector var vec)
  (walk-refs-safely
   (lambda (ref)
     (let ((call (node-parent ref)))
       (if (and (eq? 'define-dispatch! (primop-id (call-primop call)))
		(= 1 (node-index ref)))
	   (let ((index (if (literal-node? (call-arg call 2))
			    (literal-value (call-arg call 2))
			    #f))
		 (proc (get-procedure-value (call-arg call 3)))
		 (form (node-flag (node-base ref))))
	     (if (or (not (integer? index))
		     (< index 0)
		     (>= index (vector-length vec))
		     (not proc))
		 (error "problem with DEFINE-DISPATCH! in ~S"
			(form-source form)))
	     (receive (ours theirs)
		 (partition-list (lambda (n)
				   (eq? proc (node-base n)))
				 (form-lambdas form))
	       (replace-body call (detach-body (lambda-body (call-arg call 0))))
	       (set-form-lambdas! form theirs)
	       (add-lambdas ours)
	       (vector-set! vec index proc))))))
   var))

(define (get-procedure-value node)
  (cond ((lambda-node? node)
	 (if (no-free-references? node)
	     (detach node)
	     #f))
	((global-reference? node)
	 (detach node))
	(else #f)))

; make computed goto call, put the exits in the call

(define (make-computed-goto var vec size make-call)
  (receive (goto-proc goto-call default-var cont-var)
           (make-dispatch-procedure make-call size)
    (do ((i 0 (+ i 1)))
	((>= i size))
      (let ((cont (if (vector-ref vec i)
		      (make-goto-continuation (vector-ref vec i) cont-var)
		      (make-goto-default default-var))))
	(attach goto-call i cont)))
    (simplify-lambda-body goto-proc)
    goto-proc))

; ($MAKE-DISPATCH-TABLE <cont> <size> <default>)
;   =>
; (LAMBDA (C I)
;   ((LAMBDA (D)
;      ($COMPUTED-GOTO <empty> ... <empty> I))
;    (LAMBDA ()
;      (LET ((X ($PROC-CALL <default>)))
;        ($UNKNOWN-RETURN C X)))))
;
; The new procedure, the goto call, D, the default procedure, and C are all
; returned.

(define (make-dispatch-procedure call size)
  (let ((default (detach (call-arg call 1)))
	(goto-call (make-call-node (get-prescheme-primop 'computed-goto)
				   (+ size 1)
				   size)))
    (if (not default)
	(error '"cannot move dispatch default ~S" (call-arg call 1)))
    (let-nodes ((l1 ((c type/unknown) (i type/unknown)) (let 1 l2 def))
		(l2 ((d type/unknown)) goto-call)
		(def () (unknown-call 1 l3 default '(0 #f)))
		(l3 ((x type/unknown)) (unknown-return 0 (* c) '1 (* x))))
      (attach goto-call size (make-reference-node i))
      (change-lambda-type l1 'proc)  ; LET-NODES only makes CONT lambdas
      (set-lambda-protocol! l1 1) ; indicate number of non-cont arguments
      (erase (node-base call))
      (values l1 goto-call d c))))

; Return a continuation that calls DEFAULT-PROC (to which DEFAULT-VAR is bound).

(define (make-goto-default default-var)
  (let-nodes ((l1 () (jump 0 (* default-var))))
    l1))

; Make a continuation that calls PROC and then returns to CONT-VAR.
;
;  (LAMBDA ()
;    (CALL (LAMBDA (V) (UNKNOWN-RETURN <cont-var> V))
;          <proc>))

(define (make-goto-continuation proc-node cont-var)
  (let-nodes ((l1 () c1)
	      (c1 (unknown-call 1 l2 proc-node '(0 #f)))
	      (l2 ((v type/unknown))
		  (unknown-return 0 (* cont-var) '1 (* v))))
    l1))

;------------------------------------------------------------------------------
; Defining the primops used in all this.

; (MAKE-DISPATCH-TABLE <size> <default>)

(define-scheme-primop make-dispatch-table allocate)

; ($DEFINE-DISPATCH! <table> <index> <procedure>)

(define-nonsimple-scheme-primop define-dispatch! write)

; ($DISPATCH <cont> <table>)

(define-nonsimple-scheme-primop dispatch)

; (COMPUTED-GOTO <exit0> <exit1> ... <exitN> <dispatch-value>)

(define-nonsimple-scheme-primop computed-goto)
