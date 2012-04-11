; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.


(define (simplify-jump call)
  (cond ((lambda-node? (call-arg call 0))
	 (set-call-primop! call (get-primop (enum primop let)))
	 (set-call-exits! call 1)
	 (set-node-simplified?! call #f))
	(else
	 (default-simplifier call))))

(define simplify-return simplify-jump)

; If the procedure is a lambda-node:
;   1. note that we know where the continuation lambda is used (and turn any
;      tail-calls using it into regular calls)
;   2. change the primop to LET
;   3. the procedure is now the continuation
;   4. the continuation is now a jump lambda
;   5. change the primop used to call the continuation to jump
;   6. swap the cont and proc.
;   (CALL <cont> (LAMBDA (c . vars) ...) . args))
;     =>
;   (LET (LAMBDA (c . vars) ...) <cont> . args)
; If the continuation just returns somewhere else, replace UNKNOWN-CALL
; with UNKNOWN-TAIL-CALL.

(define (simplify-known-call call)
  (let ((proc (call-arg call 1))
	(cont (call-arg call 0)))
    (cond ((lambda-node? proc)
	   (determine-continuation-protocol cont (list proc))
	   (set-call-primop! call (get-primop (enum primop let)))
	   (change-lambda-type proc 'cont)
	   (change-lambda-type cont 'jump)
	   (for-each (lambda (ref)
		       (set-call-primop! (node-parent ref)
					 (get-primop (enum primop jump))))
		     (variable-refs (car (lambda-variables proc))))
	   (move cont
		 (lambda (cont)
		   (detach proc)
		   (attach call 1 cont)
		   proc)))
	  ((trivial-continuation? cont)
	   (replace cont (detach (call-arg (lambda-body cont) 0)))
	   (set-call-primop! call (get-primop (enum primop tail-call)))
	   (set-call-exits! call 0))
	  (else
	   (default-simplifier call)))))

;  (CALL (CONT (v1 ... vN) (RETURN c v1 ... vN)) ...args...)

(define (trivial-continuation? cont)
  (let ((body (lambda-body cont)))
    (and (calls-this-primop? body 'return)
	 (= (length (lambda-variables cont))
	    (- (call-arg-count body ) 1))
	 (let loop ((vars (lambda-variables cont)) (i 1))
	   (cond ((null? vars)
		  #t)
		 ((and (reference-node? (call-arg body i))
		       (eq? (car vars)
			    (reference-variable (call-arg body i))))
		  (loop (cdr vars) (+ i 1)))
		 (else #f))))))

; The same as the above, except that the continuation is a reference node
; and not a lambda, so we substitute it for the proc's continuation variable.

(define (simplify-known-tail-call call)
  (let ((proc (call-arg call 1))
	(cont (call-arg call 0)))
    (cond ((lambda-node? proc)
	   (set-call-primop! call (get-primop (enum primop let)))
	   (change-lambda-type proc 'cont)
	   (substitute (car (lambda-variables proc)) cont #t)
	   (set-lambda-variables! proc (cdr (lambda-variables proc)))
	   (remove-call-arg call 0)
	   (set-call-exits! call 1)  ; must be after REMOVE-CALL-ARG
	   (mark-changed proc))
	  (else
	   (default-simplifier call)))))

(define (simplify-test call)
  (simplify-arg call 2)
  (let ((value (call-arg call 2)))
    (cond ((literal-node? value)
	   (fold-conditional call (if (eq? false-value (literal-value value))
				      1
				      0)))
	  ((reference-node? value)
	   (simplify-variable-test call (reference-variable value)))
	  ((collapse-multiple-zero-bit-tests call)
	   )
	  (else
	   (default-simplifier call)))))

(define (simplify-variable-test call var)
  (cond ((flag-assq 'test (variable-flags var))
	 => (lambda (pair)
	      (fold-conditional call (cdr pair))))
	(else
	 (let ((pair (cons 'test 0))
	       (flags (variable-flags var)))
	   (set-variable-flags! var (cons pair flags))
	   (simplify-arg call 0)
	   (set-cdr! pair 1)
	   (simplify-arg call 1)
	   (set-variable-flags! var flags)))))

(define (fold-conditional call index)
  (replace-body call (detach-body (lambda-body (call-arg call index)))))

; (if (and (= 0 (bitwise-and 'j x))
;          (= 0 (bitwise-and 'j y)))
;     ...)
; =>
; (if (= 0 (bitwise-and (bitwise-or x y) 'j))
;     ...)
; This comes up in the Scheme48 VM.

(define (collapse-multiple-zero-bit-tests test)
  (receive (mask first-arg)
      (zero-bit-test (call-arg test 2))
    (if mask
	(let ((false-exit (call-arg test 1))
	      (true-exit (call-arg test 0)))
	  (simplify-lambda-body true-exit)
	  (simplify-lambda-body false-exit)
	  (let ((call (lambda-body true-exit)))
	    (if (and (eq? 'test (primop-id (call-primop call)))
		     (node-equal? false-exit (call-arg call 1)))
		(receive (new-mask second-arg)
		    (zero-bit-test (call-arg call 2))
		  (if (and new-mask (= mask new-mask))
		      (fold-zero-bit-tests test first-arg second-arg
					   (call-arg call 0))
		      #f))
		#f)))
	#f)))

; = and bitwise-and always have any literal node as arg1
;
; 1. call to =
; 2. first arg is literal 0
; 3. second arg is call to and
; 4. first arg of and-call is numeric literal
; 5. second arg of and-call has no side-effects (reads are okay)
;  Returns #f or the two arguments to bitwise-and.

(define (zero-bit-test call)
  (if (eq? '= (primop-id (call-primop call)))
      (let ((literal-0 (call-arg call 0))
	    (bitwise-and-call (call-arg call 1)))
	(if (and (literal-node? literal-0)
		 (number? (literal-value literal-0))
		 (= 0 (literal-value literal-0))
		 (call-node? bitwise-and-call)
		 (eq? 'bitwise-and (primop-id (call-primop bitwise-and-call)))
		 (literal-node? (call-arg bitwise-and-call 0))
		 (number? (literal-value (call-arg bitwise-and-call 0)))
		 (not (side-effects? (call-arg bitwise-and-call 1) 'read)))
	    (values (literal-value (call-arg bitwise-and-call 0))
		    (call-arg bitwise-and-call 1))
	    (values #f #f)))
      (values #f #f)))

(define (fold-zero-bit-tests test first-arg second-arg true-cont)
  (detach second-arg)
  (replace (call-arg test 0) (detach true-cont))
  (move first-arg
	(lambda (first-arg)
	  (let-nodes ((call (bitwise-ior 0 first-arg second-arg)))
	    call))))

(define (expand-test call)
  (bug "Trying to expand a call to TEST (~D) ~S"
       (node-hash (node-parent (nontrivial-ancestor call)))))

; TEST can be simplified using any literal value.
; The check for reference nodes is a heuristic.  It will only help if the
; two tests end up being sequential.

(define (simplify-test? call index value)
  (cond ((literal-node? value)
	 #t)
	((reference-node? value)
	 (any? (lambda (r)
		 (eq? 'test (primop-id (call-primop (node-parent r)))))
	       (variable-refs (reference-variable value))))
	(else
	 #f)))

(define (simplify-unknown-call call)
  (simplify-args call 0)
  (let ((proc (call-arg call 1)))
    (cond ((lambda-node? proc)
	   (determine-lambda-protocol proc (list proc))
	   (mark-changed proc))
	  ((and (reference-node? proc)
		(variable-simplifier (reference-variable proc)))
           => (lambda (proc)
		(proc call))))))

; Simplify a cell.  A set-once cell is one that is set only once and does
; not escape.  If such a cell is set to a value that can be hoisted (without
; moving variables out of scope) to the point the cell is created the cell
; is replace with the value.

; This should make use of the type of the cell.

(define (simplify-allocation call)
  (set-node-simplified?! call #t)
  (simplify-args call 0)    ; simplify all arguments, including continuation
  (let ((var (car (lambda-variables (call-arg call 0)))))
    (if (every? cell-use? (variable-refs var))
	(receive (uses sets)
	    (partition-list (lambda (n)
			      (eq? 'contents
				   (primop-id (call-primop (node-parent n)))))
			    (variable-refs var))
	  (simplify-cell-part call uses sets)))))

(define (cell-use? ref)
  (let ((call (node-parent ref)))
    (case (primop-id (call-primop call))
      ((contents)
       #t)
      ((set-contents)
       (= (node-index ref) set/owner))
      (else
       #f))))

(define (simplify-cell-part call my-uses my-sets)
  (cond ((null? my-uses)
         (for-each (lambda (n) (remove-body (node-parent n)))
		   my-sets))
        ((null? my-sets)
         (for-each (lambda (n)
		     (replace-call-with-value
		      (node-parent n)
		      (make-undefined-literal)))
		   my-uses))
;        ((null? (cdr my-sets))
;	  (set-literal-value! (call-arg call 1) 'single-set)
;	  (really-simplify-single-set call (car my-sets) my-uses))
	(else
	 (if (neq? 'small (literal-value (call-arg call 1)))
	     (set-literal-value! (call-arg call 1) 'small)))))
	     
