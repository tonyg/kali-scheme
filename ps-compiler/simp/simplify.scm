; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


;   Post-CPS optimizer.  All simplifications are done by changing the
; structure of the node tree.
;
; There are two requirements for the simplifiers:
;    1) Only the node being simplified and its descendents may be changed.
;    2) If a node is changed the NODE-SIMPLIFIED? flag of that node and all
;       its ancestors must be set to false.

; No way to simplify literal or reference nodes.

(define (simplify-node node)
  (cond ((call-node? node)
	 (simplify-call node))
	((lambda-node? node)
	 (simplify-lambda-body node))))

(define (simplify-global-reference ref)
  (let ((value (variable-known-value (reference-variable ref))))
    (if value
	(replace ref (vector->node value)))))

(define (simplify-lambda-body lambda-node)
  (let loop ()
    (let ((node (lambda-body lambda-node)))
      (cond ((not (node-simplified? node))
	     (set-node-simplified?! node #t)
             (simplify-call node)
             (loop))))))

(define (default-simplifier call)
  (simplify-args call 0))

; Utility used by many simplifiers - simplify the specified children.

(define (simplify-args call start)
  (let* ((vec (call-args call))
         (len (vector-length vec)))
    (do ((i start (+ i '1)))
        ((>= i len))
      (really-simplify-arg vec i))))

; Keep simplifying a node until it stops changing.

(define (simplify-arg call index)
  (really-simplify-arg (call-args call) index))

(define (really-simplify-arg vec index)
  (let loop ((node (vector-ref vec index)))
    (cond ((not (node-simplified? node))
	   (set-node-simplified?! node #t)
	   (case (node-variant node)
	     ((reference)
	      (if (global-variable? (reference-variable node))
		  (simplify-global-reference node)))
	     ((call)
	      (simplify-call node))
	     ((lambda)
	      (simplify-lambda-body node)))
	   (loop (vector-ref vec index))))))

; Remove any unused arguments to L-NODE
; Could substitute identical arguments as well...

(define (simplify-known-lambda l-node)
  (let ((unused (filter (lambda (var) (not (used? var)))
			(if (eq? 'proc (lambda-type l-node))
			    (cdr (lambda-variables l-node))
			    (lambda-variables l-node)))))
    (if (not (null? unused))
	(let ((refs (find-calls l-node)))
	  (for-each (lambda (var)
		      (let ((index (+ 1 (variable-index var))))
			(for-each (lambda (ref)
				    (remove-ith-argument (node-parent ref)
							 index
							 var))
				  refs)
			(remove-variable l-node var)))
		    unused)))))
    
; VAR is used to get the appropriate representation

(define (remove-ith-argument call index var)
  (let ((value (detach (call-arg call index))))
    (remove-call-arg call index)
    (move-body call
	       (lambda (call)
		 (let-nodes ((c1 (let 1 l1 value))
			     (l1 (var) call))
		   c1)))))
