; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.

;;; Find immediate dominators in a directed graph
;;; Mark Reinhold (mbr@research.nj.nec.com)/3 February 1995
; Debugging code removed and everything reluctantly Scheme-ized by
; R. Kelsey, St. Valentine's Day, 1995

; This fast dominator code is based upon Lengauer and Tarjan, "A Fast
; Algorithm for Finding Dominators in a Flowgraph," ACM TOPLAS 1:1, pp.
; 121--141, July 1979.  It runs in time $O(|E|\log|V|)$, where $|E|$ is the
; number of edges and $|V|$ is the number of vertices.  A smaller time bound
; of $O(|E|\alpha(|E|,|V|))$, where $\alpha$ is the inverse of Ackerman's
; function, can be achieved with more complex versions of the internal link!
; and eval! procedures.
;
; The client provides a rooted, directed graph by passing a root node,
; successor and predecessor functions, and auxiliary procedures for accessing
; and setting a slot in each node.  The dominator code creates a shadow of
; the client's graph using the vertex record type defined below.  To keep
; things clear, the client's graph is considered to contain "nodes," while
; the shadow graph contains "vertices."

(define-record-type vertex :vertex
  (really-make-vertex node semi bucket ancestor debug)
  vertex?
  (node vertex-node)        ; Corresponding node in client's graph
  (semi vertex-semi         ; A number for this vertex, w, as follows:
	set-vertex-semi!)   ;   After w is numbered, but before its semidominator
			    ;      is computed: w's DFS number
		            ;   After w's semidominator is computed:
		            ;      the number of its semidominator
  (parent vertex-parent     ; Parent of this vertex in DFS spanning tree
	  set-vertex-parent!)
  (pred vertex-pred         ; Parents
	set-vertex-pred!)
  (label vertex-label       ; Label in spanning forest, initially this vertex
	 set-vertex-label!)
  (bucket vertex-bucket     ; List of vertices whose semidominator is this vertex
	  set-vertex-bucket!)
  (dom vertex-dom           ; A vertex, as follows:
       set-vertex-dom!)     ;   After step 3: If the semidominator of this
    		            ;      vertex, w, is its immediate dominator, then
		            ;      this slot contains that vertex; otherwise,
		            ;      this slot is a vertex v whose number is
		            ;      smaller than w's and whose immediate dominator
                            ;      is also w's immediate dominator
		            ;   After step 4: The immediate dominator of this
                            ;      vertex
  (ancestor vertex-ancestor ; An ancestor of this vertex in the spanning forest
	    set-vertex-ancestor!)
  (debug vertex-debug       ; Debug field ##
	 set-vertex-debug!))

(define (make-vertex node semi)
  (really-make-vertex node
		      semi
		      '()	; bucket
		      #f	; ancestor
		      #f))	; debug

(define (push-vertex-bucket! inf elt)
  (set-vertex-bucket! inf (cons elt (vertex-bucket inf))))


(define (find-dominators-quickly! root		; root node
				  succ		; maps a node to its children
				  pred		; maps a node to its parents
				  slot		; result slot accessor
				  set-slot!)	; result slot setter
  ;; Compute the dominator tree of the given rooted, directed graph;
  ;; when done, the slot of each node will contain its immediate dominator.
  ;; Requires that each slot initially contain #f.

  (define (dfs root)
    (let ((n 0) (vertices '()))
      (let go ((node root) (parent #f))
	(let ((v (make-vertex node n)))
	  (set-slot! node v)
	  (set! n (+ n 1))
	  (set-vertex-parent! v parent)
	  (set-vertex-label! v v)
	  (set! vertices (cons v vertices))
	  (for-each (lambda (node)
		      (if (not (slot node))
			  (go node v)))
		    (succ node))))
      
      (let ((vertex-map (list->vector (reverse! vertices))))
	(do ((i 0 (+ i 1)))
	    ((= i (vector-length vertex-map)))
	  (let ((v (vector-ref vertex-map i)))
	    (set-vertex-pred! v (map slot (pred (vertex-node v))))))
	(values n vertex-map))))

  (define (compress! v)
    (let ((a (vertex-ancestor v)))
      (if (vertex-ancestor a)
	  (begin
	    (compress! a)
	    (if (< (vertex-semi (vertex-label a))
		   (vertex-semi (vertex-label v)))
		(set-vertex-label! v (vertex-label a)))
	    (set-vertex-ancestor! v (vertex-ancestor (vertex-ancestor v)))))))
    
  (define (eval! v)
    (cond ((not (vertex-ancestor v))
	   v)
	  (else
	   (compress! v)
	   (vertex-label v))))
  
  (define (link! v w)
    (set-vertex-ancestor! w v))
  
  (receive (n vertex-map) (dfs root)	; Step 1
    (do ((i (- n 1) (- i 1)))
	((= i 0))
      (let ((w (vector-ref vertex-map i)))
	
	(for-each (lambda (v)		; Step 2
		    (let ((u (eval! v)))
		      (if (< (vertex-semi u)
			     (vertex-semi w))
			  (set-vertex-semi! w
					    (vertex-semi u)))))
		  (vertex-pred w))
	(push-vertex-bucket! (vector-ref vertex-map (vertex-semi w)) w)
	(link! (vertex-parent w) w)
	
	(for-each (lambda (v)		; Step 3
		    ;; T&L delete v from the bucket list at this point,
		    ;; but there is no reason to do so
		    (let ((u (eval! v)))
		      (set-vertex-dom! v
				       (if (< (vertex-semi u)
					      (vertex-semi v))
					   u
					   (vertex-parent w)))))
		  (vertex-bucket (vertex-parent w)))))
    
    (do ((i 1 (+ i 1)))			; Step 4
	((= i n))
      (let ((w (vector-ref vertex-map i)))
	(if (not (eq? (vertex-dom w)
		      (vector-ref vertex-map (vertex-semi w))))
	    (set-vertex-dom! w
			     (vertex-dom (vertex-dom w))))))
    (set-vertex-dom! (slot root) #f)
    
    ;(show-nodes root succ slot)	; ## debug
    
    (do ((i 0 (+ i 1)))			; Set dominator pointers
	((= i n))
      (let ((w (vector-ref vertex-map i)))
	(let ((d (vertex-dom w)))
	  (set-slot! (vertex-node w) (if d (vertex-node d) #f)))))))


;;; The fast dominator algorithm is difficult to prove correct, so the
;;; following slow code is provided in order to check its results.  The slow
;;; algorithm, which runs in time $O(|E||V|)$, is adapted from Aho and Ullman,
;;; _The Theory of Parsing, Translation, and Compiling_, Prentice-Hall, 1973,
;;; p. 916.


(define (find-dominators-slowly! root succ pred slot set-slot!)

  (define vertex-succ vertex-pred)
  (define set-vertex-succ! set-vertex-pred!)
  (define vertex-mark vertex-ancestor)
  (define set-vertex-mark! set-vertex-ancestor!)

  (define (dfs root)
    (let ((n 0) (vertices '()))
      (let go ((node root) (parent #f))
	(let ((v (make-vertex node n)))
	  (set-slot! node v)
	  (set! n (+ n 1))
	  (set! vertices (cons v vertices))
	  (set-vertex-parent! v #f)
	  (set-vertex-label! v #f)
	  (for-each (lambda (node)
		      (if (not (slot node))
			  (go node v)))
		    (succ node))))

      (for-each (lambda (v)
		  (set-vertex-succ! v (map slot (succ (vertex-node v)))))
		vertices)
      (values n (reverse! vertices))))

  (receive (n vertices) (dfs root)

    (define (inaccessible v)
      ;; Determine set of vertices that are inaccessible if vertex v is ignored
      (set-vertex-mark! v #t)
      (let go ((w (car vertices)))
	(set-vertex-mark! w #t)
	(for-each (lambda (u)
		    (if (not (vertex-mark u))
			(go u)))
		  (vertex-succ w)))
      (filter (lambda (w)
		(cond
		 ((vertex-mark w)
		  (set-vertex-mark! w #f)
		  #f)
		 (else #t)))
	      vertices))

    (for-each (lambda (v) (set-vertex-dom! v (car vertices)))
	      (cdr vertices))

    (for-each (lambda (v)
		(let ((dominated-by-v (inaccessible v)))
		  (for-each (lambda (w)
			      (if (eq? (vertex-dom w) (vertex-dom v))
				  (set-vertex-dom! w v)))
			    dominated-by-v)))
	      (cdr vertices))
    (set-vertex-dom! (car vertices) #f)

					;(show-nodes root succ slot)	; ## debug

    (for-each (lambda (v)
		(set-slot! (vertex-node v)
			   (let ((d (vertex-dom v)))
			     (if d (vertex-node d) #f))))
	      vertices)))


(define (time-thunk thunk) (thunk))

(define (find-and-check-dominators! root succ pred slot set-slot!)
  (let ((set-fast-slot! (lambda (x v) (set-car! (slot x) v)))
	(fast-slot (lambda (x) (car (slot x))))
	(set-slow-slot! (lambda (x v) (set-cdr! (slot x) v)))
	(slow-slot (lambda (x) (cdr (slot x)))))

    (let go ((node root))
      (set-slot! node (cons #f #f))
      (for-each (lambda (node)
		  (if (not (slot node))
		      (go node)))
		(succ node)))

    (let ((fast (time-thunk
		 (lambda ()
		   (find-dominators-quickly!
		    root succ pred fast-slot set-fast-slot!))))
	  (slow (time-thunk (lambda ()
			      (find-dominators-slowly!
			       root succ pred slow-slot set-slow-slot!)))))

					;     (format #t "** find-and-check-dominators!: fast ~a, slow ~a~%" fast slow)	; ##
      (let go ((node root))
	(if (not (eq? (fast-slot node) (slow-slot node)))
	    (bug "Dominator algorithm error"))
	(set-slot! node (fast-slot node))
	(for-each (lambda (node)
		    (if (pair? (slot node)) ; ## Assumes nodes are not pairs
			(go node)))
		  (succ node))))))


(define *check?* #t)

(define (find-dominators! . args)
  (apply (if *check?*
	     find-and-check-dominators!
	     find-dominators-quickly!)
	 args))

