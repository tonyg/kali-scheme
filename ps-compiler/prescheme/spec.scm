; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.

; Protocol specifications are lists of representations.

(set-compiler-parameter! 'lambda-node-type
			 (lambda (node)
			   (let ((vars (lambda-variables node)))
			     (case (lambda-type node)
			       ((cont jump)
				(make-arrow-type (map variable-type vars)
						 type/unknown)) ; what to do?
			       ((proc known-proc)
				(make-arrow-type (map variable-type (cdr vars))
						 (variable-type (car vars))))
			       (else
				(error "unknown type of lambda node ~S" node))))))

(set-compiler-parameter! 'true-value #t)
(set-compiler-parameter! 'false-value #f)

; Tail-calls with goto-protocols cause the lambda node to be annotated
; as tail-called.
; Calls with a tuple argument need their argument spread out into separate
; variables.

(define (determine-lambda-protocol lambda-node call-refs)
  (set-lambda-protocol! lambda-node #f)
  (for-each (lambda (r)
	      (let ((call (node-parent r)))
		(cond ((goto-protocol? (literal-value (call-arg call 2)))
		       (if (not (calls-this-primop? call 'unknown-tail-call))
			   (bug "GOTO marker in non-tail-all ~S" call))
		       (set-lambda-protocol! lambda-node 'tail-called)))
		(unknown-call->known-call call)))
	    call-refs)
  (set-calls-known?! lambda-node))

(set-compiler-parameter! 'determine-lambda-protocol determine-lambda-protocol)

(define (unknown-call->known-call call)
  (remove-call-arg call 2)  ; remove the protocol
  (set-call-primop! call
		    (case (primop-id (call-primop call))
		      ((unknown-call)
		       (get-primop (enum primop call)))
		      ((unknown-tail-call)
		       (get-primop (enum primop tail-call)))
		      (else
		       (bug "odd primop in call ~S" call)))))

; CONT is the continuation passed to PROCS.

(define (determine-continuation-protocol cont procs)
  (for-each (lambda (proc)
	      (let ((cont-var (car (lambda-variables proc))))
		(walk-refs-safely
		 (lambda (ref)
		   (let ((call (node-parent ref)))
		     (unknown-return->known-return call cont-var cont)))
		 cont-var)))
	    procs))

(set-compiler-parameter! 'determine-continuation-protocol
			 determine-continuation-protocol)

; If the return is actually a tail-recursive call we change it to
; a non-tail-recursive one (since we have identified the continuation)
; and insert the appropriate continuation.

(define (unknown-return->known-return call cont-var cont)
  (case (primop-id (call-primop call))
    ((unknown-return)
     (set-call-primop! call (get-primop (enum primop return))))
    ((unknown-tail-call tail-call)
     (let* ((vars (map copy-variable (lambda-variables cont)))
	    (args (map make-reference-node vars)))
       (let-nodes ((cont vars (return 0 (* cont-var) . args)))
	 (replace (call-arg call 0) cont)
	 (set-call-primop! call
			   (if (calls-this-primop? call 'tail-call)
			       (get-primop (enum primop call))
			       (get-primop (enum primop unknown-call))))
	 (set-call-exits! call 1)
	 (if (and (calls-this-primop? call 'unknown-call)
		  (goto-protocol? (literal-value (call-arg call 2))))
	     (set-literal-value! (call-arg call 2) #f)))))
    (else
     (bug "odd return primop ~S" (call-primop call)))))

(define normal-protocol #f)
(define goto-protocol 'goto)

(define (goto-protocol? x)
  (eq? x goto-protocol))

(set-compiler-parameter! 'lookup-primop get-prescheme-primop)

(set-compiler-parameter! 'type/unknown type/unknown)

(set-compiler-parameter! 'type-eq? type-eq?)

