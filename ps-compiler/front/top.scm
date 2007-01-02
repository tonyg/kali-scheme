; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


; Debugging aids

(define *bad-ids*     '())
(define *all-procs?*  #f)
(define *checkpoints* '())

(define all-checkpoints
  '(node-made
    simplify1
    protocols
    simplify2
    node->vector
    pre-simplify-proc
    envs-added
    ))

(define (debug-breakpoint loc id data)
  (if (and (memq? loc *checkpoints*)
           (or (not id)
               *all-procs?*
               (memq? id *bad-ids*)))
      (breakpoint "~S at ~S is ~S" id loc data)))

(define (add-checks . locs)
  (receive (okay wrong)
           (partition-list (lambda (l) (memq? l all-checkpoints))
                           locs)
    (set! *checkpoints* (union okay *checkpoints*))
    (for-each (lambda (l)
		(format #t '"~&~S is not a checkpoint~%" l))
	      wrong)
    *checkpoints*))

(define (clear-checks . locs)
  (set! *checkpoints*
	(if (null? locs)
	    '()
	    (set-difference *checkpoints* locs))))

(define (add-procs . locs)
  (if (null? locs)
      (set! *all-procs?* #t)
      (set! *bad-ids* (union locs *bad-ids*))))

(define (clear-procs . locs)
  (cond ((null? locs)
         (set! *all-procs?* #f)
         (set! *bad-ids* '()))
        (else
         (set! *bad-ids*
	       (if (null? locs)
		   '()
		   (set-difference *bad-ids* locs))))))

(define add-check   add-checks)
(define clear-check clear-checks)
(define add-proc    add-procs)
(define clear-proc  clear-procs)

;------------------------------------------------------------------------------

(define *remove-cells?* #f)
(define *flow-values?*  #f)

(define (simplify-all node id)
  (debug-breakpoint 'node-made id node)
  (simplify-node node)
  (debug-breakpoint 'simplify1 id node)
  (determine-protocols)
  (debug-breakpoint 'protocols id node)
  (if (integrate-jump-procs! node)
      (simplify-node node))
  (cond (*remove-cells?*
         (remove-cells-from-tree node (make-lambda-list))
         (simplify-node node)))
  (cond (*flow-values?*
         (flow-values node (make-lambda-list))
         (simplify-node node)))
  (debug-breakpoint 'simplify2 id node)
  (values))

(define (determine-protocols)
  (walk-lambdas (lambda (l)
		  (cond ((and (eq? 'proc (lambda-type l))
			      (node? (node-parent l))
			      (find-calls l))
			 => (lambda (calls)
			      (determine-lambda-protocol l calls)))))))
