; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.


; Simplifying LET nodes, i.e. calls to the LET primop.

; 1. Change the procedure to a JUMP procedure if necessary.
; 2. Check that the right number of arguments are present.
; 3. Substitute any values that can be substituted without reference to
;    how they are used in the body; then remove the call if it is no
;    longer necessary.
; 4. Try harder.

(define (simplify-let call)
  (let ((proc (call-arg call 0)))
    (if (eq? (lambda-type proc) 'jump)
	(change-lambda-type proc 'cont))
    (cond ((n= (length (lambda-variables proc))
	       (- (call-arg-count call) 1))
	   (bug "wrong number of arguments in ~S" call))
	  ((or (null? (lambda-variables proc))
	       (substitute-let-arguments proc call quick-substitute))
	   (remove-body call))
	  (else
	   (really-simplify-let proc call)))))

; A value can be quickly substituted if it is a leaf node or if it has no
; side-effects and is used only once.

(define (quick-substitute var val)
  (or (literal-node? val)
      (reference-node? val)
      (and (not (side-effects? val))
	   (null? (cdr (variable-refs var))))))

; Simplify the arguments and then repeatedly simplify the body of PROC
; and try substituting the arguments.
; If all the arguments can be substituted the call node is removed.
;
; SUBSTITUTE-JOIN-ARGUMENTS copies arguments in an attempt to remove
; conditionals via constant folding.

(define (really-simplify-let proc call)
  (simplify-args call 1)
  (let loop ()
    (set-node-simplified?! proc #t)
    (simplify-lambda-body proc)
    (cond ((substitute-let-arguments proc call slow-substitute)
	   (remove-body call))
          ((substitute-join-arguments proc call)
           (loop))
          ((not (node-simplified? proc))
           (loop)))))

(define *duplicate-lambda-size* '-1)   ; don't duplicate anything
(define *duplicate-jump-lambda-size* 1) ; duplicate one call

(define (slow-substitute var val)
  (cond ((or (literal-node? val) (reference-node? val))
	 #t)
	((call-node? val)
	 (let ((refs (variable-refs var)))
	   (and (not (null? refs))
		(null? (cdr refs))
		(or (not (side-effects? val 'allocate))
		    (and (not (side-effects? val 'allocate 'read))
			 (not-used-between? val (car refs)))))))
	((every? called-node? (variable-refs var))
	 (simplify-known-cont-calls (variable-refs var) val)
	 (or (null? (cdr (variable-refs var)))
	     (case (lambda-type val)
	       ((proc known-proc)
		(small-node? val *duplicate-lambda-size*))
	       ((jump)
		(small-node? val *duplicate-jump-lambda-size*))
	       (else
		#f))))
        (else #f)))

; This only detects the following situation:
; (let (lambda (... var ...) (primop ... var ...))
;      ... value ...)
; where the reference to VAR is contained within nested, non-writing calls
; This depends on there being no simple calls with WRITE side-effects

(define (not-used-between? call ref)
  (let ((top (lambda-body (call-arg (node-parent call) 0))))
    (let loop ((call (node-parent ref)))
      (cond ((eq? call top) #t)
	    ((or (not (call-node? call))
		 (eq? 'write (primop-side-effects (call-primop call))))
	     #f)
	    (else (loop (node-parent call)))))))

(define (simplify-known-cont-calls refs l-node)
  (case (lambda-type l-node)
    ((proc)
     (determine-lambda-protocol l-node refs))
    ((cont)
     (bug "CONT lambda bound by LET ~S" l-node)))
  (if (calls-known? l-node)
      (simplify-known-lambda l-node)))

; ($some-RETURN <proc> . <args>)
;   =>
; ($JUMP <proc> . <args>)

; could check argument reps as well

(define (add-return-mark call l-node arg-count)
  (if (not (= (call-arg-count call) (+ arg-count 1)))
      (bug '"call ~S to join ~S has the wrong number of arguments"
           call l-node))
  (set-call-primop! call (get-primop (enum primop jump))))

; Removed arguments to a lambda-node in call position.
; If any arguments are actually removed
; REMOVE-NULL-ARGUMENTS shortens the argument vector.

(define (substitute-let-arguments node call gone-proc)
  (let* ((vec (call-args call))
         (c (do ((vars (lambda-variables node) (cdr vars))
                 (i 1 (+ i 1))
                 (c 0 (if (keep-var-val (car vars) (vector-ref vec i) gone-proc)
			  c
			  (+ 1 c))))
                ((null? vars) c))))
    (cond ((= (+ c 1) (call-arg-count call)) #t)
          ((= c 0)                           #f)
          (else
           (remove-unused-variables node)
           (remove-null-arguments call (- (call-arg-count call) c))
           #f))))

(define (keep-var-val var val gone-proc)
  (cond ((and (unused? var)
	      (or (not (call-node? val))
		  (not (side-effects? val 'allocate 'read))))
         (erase (detach val))
         #f)
        ((gone-proc var val)
         (substitute var val #t)
         #f)
        (else '#t)))

; VAL is simple enough to be substituted in more than one location if
; its body is a call with all leaf nodes.
; -- no longer used --
;(define (simple-lambda? val)
;  (vector-every? (lambda (n)
;                   (and (not (lambda-node? n))
;                 (call-args (lambda-body val))))

(define (called-anywhere? var)
  (any? called-node? (variable-refs var)))

