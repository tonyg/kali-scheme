; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


; Finding where to put phi-functions.
;
; First call:
; (GRAPH->SSA-GRAPH! <root-node> <node-successors> <node-temp> <set-node-temp!>)
;
; Then:
; (FIND-JOINS <nodes> <node-temp>)
;  will return the list of nodes N for which there are (at least) two paths
;  ... N_0 M_0 ... M_i N and ... N_1 P_0 ... P_j N such that N_0 and N_1
;  are distinct members of <nodes> and the M's and P's are disjoint sets.
;
; Algorithm from:
;   Efficiently computing static single assignment form and the control
;     dependence graph,
;   Ron Cytron, Jeanne Ferrante, Barry K. Rosen, Mark N. Wegman, and
;     F. Kenneth Zadeck,
;   ACM Transactions on Programming Languages and Systems 1991 13(4)
;   pages 451-490

(define-record-type ssa-node :node
  (really-make-node data use-uid predecessors dominator dominated
		    seen-mark join-mark)
  node?
  (data node-data)                ; user's stuff
  (use-uid node-use-uid)          ; distinguishes between different invocations
  (successors node-successors     ; parents
	      set-node-successors!)
  (predecessors node-predecessors ;  and children in the graph
		set-node-predecessors!)
  (dominator node-dominator       ; parent ;; initialize for goofy dominator code
	     set-node-dominator!)
  (dominated node-dominated       ;   and children in the dominator tree
	     set-node-dominated!)
  (frontier node-frontier         ; dominator frontier
	    set-node-frontier!)
  (seen-mark node-seen-mark       ; two markers used in
	     set-node-seen-mark!)
  (join-mark node-join-mark       ;  the ssa algorithm
	     set-node-join-mark!))

(define (make-node data use-uid)
  (really-make-node data
		    use-uid
		    '()       ; predecessors
		    #f        ; dominator
		    '()       ; dominated
		    -1        ; see-mark
		    -1))      ; join-mark

(define (graph->ssa-graph! root successors temp set-temp!)
  (let ((graph (real-graph->ssa-graph root successors temp set-temp!)))
    (find-dominators! (car graph)
		      node-successors node-predecessors
		      node-dominator set-node-dominator!)
    (for-each (lambda (node)
		(let ((dom (node-dominator node)))
		  (set-node-dominated! dom (cons node (node-dominated dom)))))
	      (cdr graph))   ; root has no dominator
    (find-frontiers! (car graph))
    (values)))

; Turn the user's graph into a NODE graph.

(define (real-graph->ssa-graph root successors temp set-temp!)
  (let ((uid (next-uid))
	(nodes '()))
    (let recur ((data root))
      (let ((node (temp data)))
	(if (and (node? node)
		 (= uid (node-use-uid node)))
	    node
	    (let ((node (make-node data uid)))
	      (set! nodes (cons node nodes))
	      (set-temp! data node)
	      (let ((succs (map recur (successors data))))
		(for-each (lambda (succ)
			    (set-node-predecessors! succ
						    (cons node (node-predecessors succ))))
			  succs)
		(set-node-successors! node succs))
	      node))))
    (if (any (lambda (node)
	       (not (eq? node (temp (node-data node)))))
	     nodes)
	(breakpoint "graph made incorrectly"))
    (reverse! nodes)))  ; root ends up at front

; Find the dominance frontiers of the nodes in a graph.

(define (find-frontiers! node)
  (let ((frontier (let loop ((succs (node-successors node)) (frontier '()))
               (if (null? succs)
                   frontier
                        (loop (cdr succs)
                             (if (eq? node (node-dominator (car succs)))
                                 frontier
                                (cons (car succs) frontier)))))))
    (let loop ((kids (node-dominated node)) (frontier frontier))
      (cond ((null? kids)
         (set-node-frontier! node frontier)
      frontier)
      (else
            (let kid-loop ((kid-frontier (find-frontiers! (car kids)))
                      (frontier frontier))
               (if (null? kid-frontier)
            (loop (cdr kids) frontier)
              (kid-loop (cdr kid-frontier)
                      (if (eq? node (node-dominator (car kid-frontier)))
                          frontier
                                (cons (car kid-frontier) frontier))))))))))

(define (find-joins nodes temp)
  (for-each (lambda (n)
	      (if (not (node? (temp n)))
		  (begin
		    (breakpoint "node not seen before ~s" n)
		    n)))
	    nodes)
  (map node-data (really-find-joins (map temp nodes))))

(define (really-find-joins nodes)
  (let ((marker (next-uid)))
    (for-each (lambda (n)
		(set-node-seen-mark! n marker))
	      nodes)
    (let loop ((to-do nodes) (joins '()))
      (if (null? to-do)
	  joins
	  (let frontier-loop ((frontier (node-frontier (car to-do)))
			      (to-do (cdr to-do))
			      (joins joins))
	    (cond ((null? frontier)
		   (loop to-do joins))
		  ((eq? marker (node-join-mark (car frontier)))
		   (frontier-loop (cdr frontier) to-do joins))
		  (else
		   (let ((node (car frontier)))
		     (set-node-join-mark! node marker)
		     (frontier-loop (cdr frontier)
				    (if (eq? marker (node-seen-mark node))
					to-do
					(begin
					  (set-node-seen-mark! node marker)
					  (cons node to-do)))
				    (cons node joins))))))))))

; Integers as UID's

(define *next-uid* 0)

(define (next-uid)
  (let ((uid *next-uid*))
    (set! *next-uid* (+ uid 1))
    uid))

;----------------------------------------------------------------
; Testing

;(define-record-type data
;  (name)
;  (kids
;   temp))
;
;(define-record-discloser type/data
;  (lambda (data)
;    (list 'data (data-name data))))
;
;(define (make-test-graph spec)
;  (let ((vertices (map (lambda (d)
;                         (data-maker (car d)))
;                       spec)))
;    (for-each (lambda (data vertex)
;                (set-data-kids! vertex (map (lambda (s)
;                                              (first (lambda (v)
;                                                       (eq? s (data-name v)))
;                                                     vertices))
;                                            (cdr data))))
;              spec
;              vertices)
;    vertices))

;(define g1 (make-test-graph '((a b) (b c d) (c b e) (d d e) (e))))
;(graph->ssa-graph (car g1) data-kids data-temp set-data-temp!)
;(find-joins (list (list-ref g1 0)) data-temp)
