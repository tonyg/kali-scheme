; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.


; Code to determine the separation vertices of a graph

; NODES is a list of nodes
; (TO node) returns a list of the nodes which are connected to this one
; (SLOT-NODE node) and (SET-SLOT! node value) are used by the algorithm to
; associate data with nodes (in the absence of a tables).

(define (separation-vertices nodes to slot set-slot!)
  (cond ((null? nodes)
	 (values '() '()))
	((null? (cdr nodes))
	 (values nodes (list nodes)))
	(else
	 (receive (separators components)
	     (real-separation-vertices (make-vertices nodes to slot set-slot!))
	   (for-each (lambda (n) (set-slot! n #f)) nodes)
	   (values separators components)))))

(define-record-type vertex :vertex
  (really-make-vertex data edges dfs-index)
  vertex?
  (data vertex-data)                  ; user's data
  (edges vertex-edges                 ; list of edges from this vertex
	 set-vertex-edges!)    
  (dfs-index vertex-dfs-index         ; ordering from depth-first-search
	     set-vertex-dfs-index!)    
  (level vertex-level                 ; value used in algorithm...
	 set-vertex-level!)
  (parent vertex-parent               ; parent of this node in DFS tree
	  set-vertex-parent!))

(define (make-vertex data)
  (really-make-vertex data '() 0))

(define-record-type edge :edge
  (make-edge from to unused?)
  (from edge-from)             ; two (unordered) vertices
  (to edge-to)
  (unused? edge-unused         ; used to mark edges that have been traversed
	   set-edge-unused!))

(define (make-edge from to)
  (really-make-edge from to #t))

(define (other-vertex edge v)
  (if (eq? v (edge-from edge))
      (edge-to edge)
      (edge-from edge)))

(define (maybe-add-edge from to)
  (if (and (not (eq? from to))
	   (not (any? (lambda (e)
			(or (eq? to (edge-from e))
			    (eq? to (edge-to e))))
		      (vertex-edges from))))
      (let ((e (make-edge from to)))
	(set-vertex-edges! from (cons e (vertex-edges from)))
	(set-vertex-edges! to   (cons e (vertex-edges to))))))

(define (make-vertices nodes to slot set-slot!)
  (let ((vertices (map (lambda (n)
			 (let ((v (make-vertex n)))
			   (set-slot! n v)
			   v))
		       nodes)))
    (for-each (lambda (n)
		(for-each (lambda (n0)
			    (maybe-add-edge (slot n) (slot n0)))
			  (to n)))
	      nodes)
    vertices))
    
; The numbers are the algorithm step numbers from page 62 of Graph Algorithms,
; Shimon Even, Computer Science Press, 1979.
; Them           Us
;  L(v)     (vertex-level v)
;  k(v)     (vertex-dfs-index v)
;  f(v)     (vertex-parent v)
;  S        stack
;  s        start

(define (real-separation-vertices vertices)
  (do-vertex (car vertices) 0 '() (car vertices) '() '()))

; 2

(define (do-vertex v i stack start v-res c-res)
  (let ((i (+ i 1)))
    (set-vertex-level! v i)
    (set-vertex-dfs-index! v i)
    (find-unused-edge v i (cons v stack) start v-res c-res)))

; 3

(define (find-unused-edge v i stack start v-res c-res)
  (let ((e (first edge-unused? (vertex-edges v))))
    (if e
	(do-edge e v i stack start v-res c-res)
	(no-unused-edge v i stack start v-res c-res))))

; 4

(define (do-edge e v i stack start v-res c-res)
  (let ((u (other-vertex e v)))
    (set-edge-unused?! e #f)
    (cond ((= 0 (vertex-dfs-index u))
	   (set-vertex-parent! u v)
	   (do-vertex u i stack start v-res c-res))
	  (else
	   (if (> (vertex-level v)
		  (vertex-dfs-index u))
	       (set-vertex-level! v (vertex-dfs-index u)))
	   (find-unused-edge v i stack start v-res c-res)))))

; 5

(define (no-unused-edge v i stack start v-res c-res)
  (let* ((parent (vertex-parent v))
	 (p-dfs-index (vertex-dfs-index parent)))
    (cond ((= 1 p-dfs-index)
	   (gather-nonseparable-with-start v i stack start v-res c-res))
	  ((< (vertex-level v) p-dfs-index)
	   (if (< (vertex-level v)
		  (vertex-level parent))
	       (set-vertex-level! parent (vertex-level v)))
	   (find-unused-edge parent i stack start v-res c-res))
	  (else
	   (gather-nonseparable v i stack start v-res c-res)))))

; 7

(define (gather-nonseparable v i stack start v-res c-res)
  (let* ((parent (vertex-parent v))
	 (data (vertex-data parent)))
    (receive (vertices stack)
	(pop-down-to stack v)
      (find-unused-edge parent
			i
			stack
			start
			(if (not (memq? data v-res))
			    (cons data v-res)
			    v-res)
			(cons (cons data (map vertex-data vertices)) c-res)))))

; 9

(define (gather-nonseparable-with-start v i stack start v-res c-res)
  (receive (vertices stack)
      (pop-down-to stack v)
    (let* ((data (vertex-data start))
	   (c-res (cons (cons data (map vertex-data vertices)) c-res)))
      (if (not (any? edge-unused? (vertex-edges start)))
	  (values v-res c-res)
	  (find-unused-edge start
			    i
			    stack
			    start
			    (if (not (memq? data v-res))
				(cons data v-res)
				v-res)
			    c-res)))))

(define (pop-down-to stack v)
  (do ((stack stack (cdr stack))
       (res '() (cons (car stack) res)))
      ((eq? v (car stack))
       (values (cons v res) (cdr stack)))))

(define (test-separation-vertices graph)
  (let ((nodes (map (lambda (n)
		      (vector (car n) #f #f))
		    graph)))
    (for-each (lambda (data node)
		(vector-set! node 1 (map (lambda (s)
					   (first (lambda (v)
						    (eq? s (vector-ref v 0)))
						  nodes))
					 (cdr data))))
	      graph
	      nodes)
    (receive (separation-vertices components)
	(separation-vertices nodes
			     (lambda (v) (vector-ref v 1))
			     (lambda (v) (vector-ref v 2))
			     (lambda (v val) (vector-set! v 2 val)))
      (values (map (lambda (v) (vector-ref v 0)) separation-vertices)
	      (map (lambda (l)
		     (map (lambda (v) (vector-ref v 0))
			  l))
		   components)))))
			   
