; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; General transitive closure

; (make-graph-from-predecessors nodes node-parents node-temp set-node-temp!)
;            -> graph
; (make-graph-from-successors nodes node-kids node-temp set-node-temp!)
;            -> graph
;
; (transitive-or! graph elements set-elements! element-temp set-element-temp!)
; (transitive-or-with-kill! graph elements set-elements! node-kills element-temp set-element-temp!)
; (transitive-or-with-pass! graph elements set-elements! node-passes element-temp set-element-temp!)
;
; (transitive-and! graph elements set-elements! element-temp set-element-temp!)
; (transitive-and-with-kill! graph elements set-elements! node-kills element-temp set-element-temp!)
; (transitive-and-with-pass! graph elements set-elements! node-passes element-temp set-element-temp!)

;----------------

(define (make-graph-from-predecessors user-nodes user-node-parents node-temp set-node-temp!)
  (let ((nodes (make-nodes user-nodes set-node-temp!)))
    (connect-nodes-using-parents! nodes user-node-parents node-temp)
    (for-each (lambda (node)
		(set-node-temp! (node-data node) #f))
	      nodes)
    (make-graph nodes)))

(define (make-graph-from-successors user-nodes user-node-kids node-temp set-node-temp!)
  (let ((nodes (make-nodes user-nodes set-node-temp!)))
    (connect-nodes-using-children! nodes user-node-kids node-temp)
    (for-each (lambda (node)
		(set-node-temp! (node-data node) #f))
	      nodes)
    (make-graph nodes)))

(define (make-nodes user-nodes set-node-temp!)    
  (map (lambda (data)
	 (let ((node (node-maker data '() '())))
	   (set-node-temp! data node)
	   node))
       user-nodes))

(define-record-type graph
  (nodes         ; list of nodes
   )
  ())

(define make-graph graph-maker)
  
(define-record-type node
  (data          ; user's data
   (parents)     ; predecessors
   (kids))       ; successors
  (elt-set       ; elements
   kill-set      ; elements that are not passed
   changed?      ; change flag for iteration
   ))

;------------------------------
; Six false fronts for the real procedure.

(define (transitive-or! graph elts set-elts! elt-hash set-elt-hash!)
  (do-it graph elts set-elts! #f #f elt-hash set-elt-hash!
	 (transitive-or-closure! or-update-node)))

(define (transitive-or-with-kill! graph elts set-elts! kill-elts elt-hash set-elt-hash!)
  (do-it graph elts set-elts! kill-elts #f elt-hash set-elt-hash!
	 (transitive-or-closure! or-update-node-with-kill)))

(define (transitive-or-with-pass! graph elts set-elts! pass-elts elt-hash set-elt-hash!)
  (do-it graph elts set-elts! pass-elts #t elt-hash set-elt-hash!
	 (transitive-or-closure! or-update-node-with-kill)))

(define (transitive-and! graph elts set-elts! elt-hash set-elt-hash!)
  (do-it graph elts set-elts! #f #f elt-hash set-elt-hash!
	 (transitive-and-closure! and-update-node)))

(define (transitive-and-with-kill! graph elts set-elts! kill-elts elt-hash set-elt-hash!)
  (do-it graph elts set-elts! kill-elts #f elt-hash set-elt-hash!
	 (transitive-and-closure! and-update-node-with-kill)))

(define (transitive-and-with-pass! graph elts set-elts! pass-elts elt-hash set-elt-hash!)
  (do-it graph elts set-elts! pass-elts #t elt-hash set-elt-hash!
	 (transitive-and-closure! and-update-node-with-kill)))

(define (do-it graph elts set-elts! kill-elts pass? elt-hash set-elt-hash! op)
  (let* ((nodes (graph-nodes graph))
	 (elt-unhash-vec (add-elements! nodes elts kill-elts pass?
					elt-hash set-elt-hash!)))
    (op nodes)
    (record-results! nodes elt-unhash-vec set-elts!)
    (do ((i 0 (+ i 1)))
	((= i (vector-length elt-unhash-vec)))
      (set-elt-hash! (vector-ref elt-unhash-vec i) #f))
    (values)))

;----------------
; Setting the kids field of the nodes

(define (connect-nodes-using-children! nodes children node-slot)
  (for-each
   (lambda (node)
     (set-node-kids! node
		      (map (lambda (kid)
			     (let ((t (node-slot kid)))
			       (if (not (node? t))
				   (missing-node-error kid "child" node))
			       (set-node-parents! t
						   (cons node
							 (node-parents t)))
			       t))
			   (children (node-data node)))))
   nodes))

(define (connect-nodes-using-parents! nodes parents node-slot)
  (for-each
   (lambda (node)
     (set-node-parents! node
			 (map (lambda (parent)
				(let ((t (node-slot parent)))
				  (if (not (node? t))
				      (missing-node-error t "parent" node))
				  (set-node-kids! t
						   (cons node
							 (node-kids t)))
				  t))
			      (parents (node-data node)))))
   nodes))

(define (missing-node-error node relationship relation)
  (error (format #f "Transitive - ~S, ~A of ~S, not in list of nodes"
		 node relationship (node-data relation))))

;----------------

(define (add-elements! nodes node-elements node-kills pass?
		       element-temp set-element-temp!)
  (let ((unhash-vec (element-hasher nodes node-elements element-temp set-element-temp!))
	(element-hash (make-element-hash element-temp)))
    (for-each (lambda (node)
		(set-node-elt-set! node
				   (make-element-set (node-elements (node-data node))
						     element-hash)))
	      nodes)
    (if node-kills
	(for-each (lambda (node)
		    (let ((kill-set (make-element-set (node-kills (node-data node))
						      element-hash)))
		      (set-node-kill-set! node (if pass?
						   (integer-set-not kill-set)
						   kill-set))))
		  nodes))
    unhash-vec))

(define (make-element-set elts elt-hash)
  (let loop ((elts elts) (set (make-empty-integer-set)))
    (if (null? elts)
	set
	(loop (cdr elts)
	      (cond ((elt-hash (car elts))
		     => (lambda (hash)
			  (add-to-integer-set set hash)))
		    (else set))))))

;----------------
; Counting the elements and assigning numbers to them

(define-record-type element-hash
  (number  ; the element-hash record is just a way of tagging this number
   )       ; with a unique predicate
  ())

(define (element-hasher nodes elts elt-hash set-elt-hash!)
  (let loop ((to-do '()) (ts nodes) (all-elts '()) (count 0))
    (cond ((null? to-do)
	   (if (null? ts)
	       (real-element-hasher all-elts count)
	       (loop (elts (node-data (car ts))) (cdr ts) all-elts count)))
	  ((element-hash? (elt-hash (car to-do)))
	   (loop (cdr to-do) ts all-elts count))
	  (else
	   (set-elt-hash! (car to-do) (element-hash-maker count))
	   (loop (cdr to-do) ts (cons (car to-do) all-elts) (+ count 1))))))

(define (real-element-hasher elts count)
  (let ((unhash-vec (make-vector count)))
    (do ((i (- count 1) (- i 1))
	 (elts elts (cdr elts)))
	((null? elts))
      (vector-set! unhash-vec i (car elts)))
    unhash-vec))

(define (make-element-hash elt-hash)
  (lambda (elt)
    (let ((hash (elt-hash elt)))
      (if (element-hash? hash)
	  (element-hash-number hash)
	  #f))))

;----------------
; Turn the element sets into lists of elements and clean up stray pointers
; at the same time.

(define (record-results! nodes elt-unhash-vec set-elts!)
  (for-each (lambda (node)
	      (set-elts! (node-data node)
			 (map-over-integer-set
			  (lambda (i) (vector-ref elt-unhash-vec i))
			  (node-elt-set node)))
	      (set-node-elt-set! node #f)
	      (set-node-kill-set! node #f))
	    nodes))

;----------------
; The OR algorithm - keeps passing elements around until the changes stop.

(define (transitive-or-closure! op)
  (lambda (nodes)
    (for-each (lambda (node)
		(set-node-changed?! node #t))
	      nodes)
    (let loop ((to-do nodes))
      (if (not (null? to-do))
	  (let* ((node (car to-do))
		 (elt-set (node-elt-set node)))
	    (set-node-changed?! node #f)
	    (let kids-loop ((ts (node-kids node))
			    (to-do (cdr to-do)))
	      (cond ((null? ts)
		     (loop to-do))
		    ((and (op (car ts) elt-set)
			  (not (node-changed? (car ts))))
		     (set-node-changed?! (car ts) #t)
		     (kids-loop (cdr ts) (cons (car ts) to-do)))
		    (else
		     (kids-loop (cdr ts) to-do)))))))))

; The weird function INTEGER-SET-SUBTRACT&IOR-WITH-TEST! takes three integer
; sets, subtracts the second from the first and inclusive OR's the result
; with the third.  It returns the resulting set and a flag which is #T if
; the result is not the same as the original third set.  The inclusive OR
; may be destructive.

(define (or-update-node-with-kill node elt-set)
  (receive (set change?)
      (integer-set-subtract&ior-with-test! elt-set
					   (node-kill-set node)
					   (node-elt-set node))
    (set-node-elt-set! node set)
    change?))

(define (or-update-node node elt-set)
  (receive (set change?)
      (integer-set-ior-with-test! elt-set
				  (node-elt-set node))
    (set-node-elt-set! node set)
    change?))

; Implementations using simpler, nondestructive operations (these might be
; done more efficiently if they had access to the underlying representation
; of integer sets).

(define (integer-set-subtract&ior-with-test! set1 set2 set3)
  (let ((result (integer-set-ior set3 (integer-set-subtract set1 set2))))
    (values result (not (integer-set-equal? set3 result)))))

(define (integer-set-ior-with-test! set1 set3)
  (let ((result (integer-set-ior set3 set1)))
    (values result (not (integer-set-equal? set3 result)))))

;----------------
; The AND algorithm - keeps a to-do list of nodes whose parents' elements
; have changed, instead of a list of nodes whose elements have changed.

(define (transitive-and-closure! op)
  (lambda (nodes)
    (let loop ((to-do (filter (lambda (node)
				(if (not (null? (node-parents node)))
				    (begin
				      (set-node-changed?! node #t)
				      #t)
				    #f))
			      nodes)))
      (if (not (null? to-do))
	  (let ((node (car to-do)))
	    (set-node-changed?! node #f)
	    (if (op node)
		(let kids-loop ((ts (node-kids node))
				(to-do (cdr to-do)))
		  (cond ((null? ts)
			 (loop to-do))
			((node-changed? (car ts))
			 (kids-loop (cdr ts) to-do))
			(else
			 (set-node-changed?! (car ts) #t)
			 (kids-loop (cdr ts) (cons (car ts) to-do)))))
		(loop (cdr to-do))))))))
  
; These are the same as for OR except that we AND together the parents'
; elt-sets instead of using the one provided.

(define (and-update-node-with-kill node)
  (receive (set change?)
      (integer-set-subtract&ior-with-test! (parents-elt-set node)
					   (node-kill-set node)
					   (node-elt-set node))
    (set-node-elt-set! node set)
    change?))
  
(define (and-update-node node)
  (receive (set change?)
      (integer-set-ior-with-test! (parents-elt-set node)
				  (node-elt-set node))
    (set-node-elt-set! node set)
    change?))

(define (parents-elt-set node)
  (do ((parents (cdr (node-parents node))
		(cdr parents))
       (elts (node-elt-set (car (node-parents node)))
	     (integer-set-and elts (node-elt-set (car parents)))))
      ((null? parents)
       elts)))

;------------------------------------------------------------
; Testing

; GRAPH is ((<symbol>           name
;            (element*)         elements
;            (element*)         kills
;            . <symbol*>)*)     children
;
'((node1 (elt1 elt2) ()     node2)
  (node2 (elt3)      (elt2) node1 node3)
  (node3 ()          ()     ))

'((a (1) () b)
  (b ()  () ))

'((a (1 2 3 4) (1) b)
  (b ()        (2) c)
  (c ()        (3) d)
  (d (5)       (4) a))
             
(define (test-transitive graph down? or? pass?)
  (let* ((elts '())
	 (get-elt (lambda (sym)
		    (cond ((first (lambda (v)
				    (eq? sym (vector-ref v 0)))
				  elts)
			   => identity)
			  (else
			   (let ((new (vector sym #f)))
			     (set! elts (cons new elts))
			     new)))))
	 (vertices (map (lambda (n)
			  (vector (car n)
				  (map get-elt (cadr n))
				  (map get-elt (caddr n))
				  #f #f))
			graph)))
    (for-each (lambda (data vertex)
		(vector-set! vertex 3 (map (lambda (s)
					     (first (lambda (v)
						      (eq? s (vector-ref v 0)))
						    vertices))
					   (cdddr data))))
	      graph
	      vertices)
    (let ((the-graph ((if down?
			  make-graph-from-successors
			  make-graph-from-predecessors)
		      vertices
		      (lambda (x) (vector-ref x 3))
		      (lambda (x) (vector-ref x 4))
		      (lambda (x v) (vector-set! x 4 v)))))
      (if (every? (lambda (n) (null? (caddr n))) graph)
	  ((if or? transitive-or! transitive-and!)
	   the-graph
	   (lambda (v) (vector-ref v 1))        ; elts
	   (lambda (v x) (vector-set! v 1 x))   ; set-elts!
	   (lambda (e) (vector-ref e 1))        ; elt-hash
	   (lambda (e x) (vector-set! e 1 x)))  ; set-elt-hash!
          ((if or?					    
	       (if pass?
		   transitive-or-with-pass!
		   transitive-or-with-kill!)
	       (if pass?
		   transitive-and-with-pass!
		   transitive-and-with-kill!))
	   the-graph
	   (lambda (v) (vector-ref v 1))         ; elts
	   (lambda (v x) (vector-set! v 1 x))    ; set-elts!
	   (lambda (v) (vector-ref v 2))         ; kills
	   (lambda (e) (vector-ref e 1))         ; elt-hash
	   (lambda (e x) (vector-set! e 1 x))))) ; set-elt-hash!
    (map (lambda (v)
	   (list (vector-ref v 0)
		 (map (lambda (e) (vector-ref e 0))
		      (vector-ref v 1))))
	 vertices)))
