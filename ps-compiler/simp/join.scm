; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


; Call JOIN-SUBSTITUTE on all variable/value pairs.

(define (substitute-join-arguments lambda-proc call)
  (let ((vec (call-args call))
        (vars (lambda-variables lambda-proc)))
    (do ((vars vars (cdr vars))
         (i    1   (+ i 1))
         (c?   #f  (or (join-substitute (car vars) (vector-ref vec i))
                        c?)))
        ((null? vars) c?))))

; Does VAL take only one argument and is that argument passed to $TEST?
; Is VAR applied to constants?
; Then two possiblities are checked for:
;   Does the tree rooted at the least-common-ancestor of VAR's references
;   contain no side-effects and necessarily passed control to VAR?
;  or
;   Does the join point contain no side-effects above the test?
;
; If so, make the transformation described below.

(define (join-substitute var val)
  (let ((ref (and (lambda-node? val)
		  (simple-test-procedure val))))
    (and ref
	 (applied-to-useful-value? var ref)
	 (let ((lca (least-common-ancestor (variable-refs var))))
	   (cond ((or (suitable-join-conditional? lca var)
		      (suitable-join-point? val (node-parent ref)))
		  (really-join-substitute var val lca (node-parent ref))
		  #t)
		 (else #f))))))

; Check that VAL (a lambda-node) takes one argument, is jumped to, tests its
; argument, and that all references to the argument are at or below the test.

(define (simple-test-procedure val)
  (let ((vars (lambda-variables val)))
    (if (or (null? vars)
	    (not (null? (cdr vars)))
	    (not (car vars))
	    (not (calls-known? val))
	    (neq? 'jump (lambda-type val)))
	#f
	(let* ((var (car vars))
	       (ref (any simple-cond-ref (variable-refs var))))
	  (if (and ref (all-refs-below? var (node-parent ref)))
	      ref
	      #f)))))

(define (simple-cond-ref ref)
  (if (primop-conditional? (call-primop (node-parent ref)))
      ref
      #f))

(define (all-refs-below? var node)
  (set-node-flag! node #t)
  (set-node-flag! (variable-binder var) #t)
  (let ((res (every? (lambda (r)
		       (eq? node (marked-ancestor r)))
		     (variable-refs var))))
    (set-node-flag! node #f)
    (set-node-flag! (variable-binder var) #f)
    res))

; Is VAR applied to something that can be used to simplify the conditional?

(define (applied-to-useful-value? var ref)
  (let ((call (node-parent ref))
	(index (node-index ref)))
    (any? (lambda (r)
	    (simplify-conditional? call index (call-arg (node-parent r) 1)))
	  (variable-refs var))))

; CALL is the least-common-ancestor of the references to VAR.  Check that
; the tree rooted at CALL contains no side-effects and that the control flow
; necessarily passes to VAR.  (Could check for undefined-effect here...)
; could do check that jumped-to proc if not VAR jumped to VAR eventually

(define (suitable-join-conditional? call var)
  (let label ((call call))
    (cond ((call-side-effects? call)
	   #f)
	  ((= 0 (call-exits call))
	   (and (eq? 'jump (primop-id (call-primop call)))
		(eq? var (reference-variable (called-node call)))))
	  (else
	   (let loop ((i 0))
	     (cond ((>= i (call-exits call))
		    #t)
		   ((not (label (lambda-body (call-arg call i))))
		    #f)
		   (else
		    (loop (+ i 1)))))))))

; #t if CALL performs side-effects.  The continuations to CALL are ignored.

(define (call-side-effects? call)
  (or (primop-side-effects (call-primop call))
      (let loop ((i (call-exits call)))
	(cond ((>= i (call-arg-count call))
	       #f)
	      ((side-effects? (call-arg call i))
	       #t)
	      (else
	       (loop (+ i 1)))))))

; The alternative to the above test: does the join point contain no side-effects
; above the test?

(define (suitable-join-point? join test)
  (let label ((call (lambda-body join)))
    (cond ((eq? call test)
	   #t)
	  ((call-side-effects? call)
	   #f)
	  (else
	   (let loop ((i 0))
	     (cond ((>= i (call-exits call))
		    #t)
		   ((not (label (lambda-body (call-arg call i))))
		    #f)
		   (else
		    (loop (+ i 1)))))))))
	     
; (let ((j (lambda (v)                  ; VAR VAL
;            .a.
;            ($test c1 c2 ... v ...)    ; TEST
;            .b.)))
;   .c.
;   (... (j x) ...)                     ; CALL
;   .d.)
; ==>
; .c.
; (.a.
;  (let ((v1 (lambda (x) c1[x/v]))
;        (v2 (lambda (x) c2[x/v])))
;    (... ((lambda (v)
;            ($test (lambda () (v1 v)) (lambda () (v2 v)) ... v ...))
;          x)
;     ...))
;  .b.)
; .d.
;
; CALL is the least common ancestor of the references to VAR, which is bound to
; VAL, a procedure.  TEST is a conditional that tests the argument passed to
; VAL.
;
; (lambda-body VAL) is moved to where CALL is.
; In the body of VAL, TEST is replaced by a LET that binds TEST's continuations
; and then executes CALL.  TEST's continuations are replaced by calls to
; the variables bound by the LET.
; Finally, references to VAR are replaced by a procedure whose body is TEST,
; which is the point of the whole exercise.

(define (really-join-substitute var val call test)
  (let ((value-var (car (lambda-variables val))))
    (receive (cont-call conts)
	(move-continuations test call value-var)
      (let ((test-parent (node-parent test))
	    (val-parent (node-parent val))
	    (val-index (node-index val)))
	(parameterize-continuations conts value-var)
	(detach-body test)
	(move-body cont-call
		   (lambda (cont-call)
		     (attach-body test-parent cont-call)
		     (detach-body (lambda-body val))))
	(attach-body val test)
	(mark-changed (call-arg test 1)) ; marks test as changed.
	(mark-changed cont-call)
	(substitute var val #t)
	(attach val-parent val-index (make-literal-node #f #f))
	(values)))))

; Move the continuations of CALL to a LET call just above TO.  Returns a list
; of the variables now bound to the continuations and the continuations
; themselves.

(define (move-continuations call to arg-var)
  (let ((count (call-exits call)))
    (let loop ((i (- count 1)) (vs '()) (es '()))
      (cond ((< i 0)
	     (let ((new-call (make-call-node (get-primop (enum primop let))
					     (+ count 1)
					     1))
		   (new-proc (make-lambda-node 'j 'cont vs)))
	       (attach-call-args new-call (cons new-proc es))
	       (insert-body new-call new-proc (node-parent to))
	       (values new-call es)))
	    (else
	     (let ((var (make-variable 'e (node-type (call-arg call i))))
		   (cont (detach (call-arg call i))))
	       (let-nodes ((new-cont () c1)
			   (c1 (jump 0 (* var) (* arg-var))))
		 (attach call i new-cont))
	       (change-lambda-type cont 'jump)
	       (loop (- i 1) (cons var vs) (cons cont es))))))))

; Add a new variable to each of CONTS and substitute a reference to the correct
; variable for each reference to VAR within CONTS.

(define (parameterize-continuations conts var)
  (for-each (lambda (n)
	      (let ((var (copy-variable var)))
		(set-lambda-variables! n (cons var (lambda-variables n)))
		(set-variable-binder! var n)
		(set-node-flag! n #t)))
	    conts)
  (let ((backstop (variable-binder var)))
    (set-node-flag! backstop #t)
    (walk-refs-safely
     (lambda (n)
       (let ((cont (marked-ancestor n)))
	 (if (not (eq? cont backstop))
	     (replace n (make-reference-node (car (lambda-variables cont)))))))
     var)
    (set-node-flag! backstop #f)
    (for-each (lambda (n) (set-node-flag! n #f)) conts)
    (values)))
