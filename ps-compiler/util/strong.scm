; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


; Code to find the strongly connected components of a graph.
; (TO <vertex>) are the vertices that have an edge to <vertex>.
; (SLOT <vertex>) and (SET-SLOT! <vertex> <value>) is a settable slot
; used by the algorithm.
;
; The components are returned in a backwards topologically sorted list.

(define (strongly-connected-components vertices to slot set-slot!)
  (make-vertices vertices to slot set-slot!)
  (let loop ((to-do vertices) (index 0) (stack #t) (comps '()))
    (let ((to-do (find-next-vertex to-do slot)))
      (cond ((null? to-do)
	     (for-each (lambda (n) (set-slot! n #f)) vertices)
	     comps)
	    (else
	     (call-with-values
	      (lambda () 
		(do-vertex (slot (car to-do)) index stack comps))
	      (lambda (index stack comps)
		(loop to-do index stack comps))))))))

(define (find-next-vertex vertices slot)
  (do ((vertices vertices (cdr vertices)))
      ((or (null? vertices)
           (= 0 (vertex-index (slot (car vertices)))))
       vertices)))

(define-record-type vertex
  (data            ; user's data
   )
  ((edges   '())   ; list of vertices
   (stack    #f)   ; next vertex on the stack
   (index    0)    ; time at which this vertex was reached in the traversal
   (parent   #f)   ; a vertex pointing to this one
   (lowpoint #f)   ; lowest index in this vertices strongly connected component
   ))
  
(define (make-vertices vertices to slot set-slot!)
  (let ((maybe-slot (lambda (n)
		      (let ((s (slot n)))
			(if (vertex? s)
			    s
			    (error "graph edge points to non-vertex" n))))))
    (for-each (lambda (n)
		(set-slot! n (vertex-maker n)))
	      vertices)
    (for-each (lambda (n)
		(set-vertex-edges! (slot n) (map maybe-slot (to n))))
	      vertices)
    (values)))

; The numbers are the algorithm step numbers from page 65 of Graph Algorithms,
; Shimon Even, Computer Science Press, 1979.

; 2

(define (do-vertex vertex index stack comps)
  (let ((index (+ index '1)))
    (set-vertex-index!    vertex index)
    (set-vertex-lowpoint! vertex index)
    (set-vertex-stack!    vertex stack)
    (get-strong vertex index vertex comps)))

; 3

(define (get-strong vertex index stack comps)
  (if (null? (vertex-edges vertex))
      (end-vertex    vertex index stack comps)
      (follow-edge vertex index stack comps)))

; 7

(define (end-vertex vertex index stack comps)
  (call-with-values
   (lambda ()
     (if (= (vertex-index vertex) (vertex-lowpoint vertex))
	 (unwind-stack vertex stack comps)
	 (values stack comps)))
   (lambda (stack comps)
     (cond ((vertex-parent vertex)
	    => (lambda (parent)
		 (if (> (vertex-lowpoint parent) (vertex-lowpoint vertex))
		     (set-vertex-lowpoint! parent (vertex-lowpoint vertex)))
		 (get-strong parent index stack comps)))
	   (else
	    (values index stack comps))))))

(define (unwind-stack vertex stack comps)
  (let loop ((n stack) (c '()))
    (let ((next (vertex-stack n))
          (c (cons (vertex-data n) c)))
      (set-vertex-stack! n #f)
      (if (eq? n vertex)
          (values next (cons c comps))
          (loop next c)))))

; 4

(define (follow-edge vertex index stack comps)
  (let* ((next (pop-vertex-edge! vertex))
         (next-index (vertex-index next)))
    (cond ((= next-index 0)
           (set-vertex-parent! next vertex)
           (do-vertex next index stack comps))
          (else
           (if (and (< next-index (vertex-index vertex))
                    (vertex-stack next)
                    (< next-index (vertex-lowpoint vertex)))
               (set-vertex-lowpoint! vertex next-index))
           (get-strong vertex index stack comps)))))

(define (pop-vertex-edge! vertex)
  (let ((edges (vertex-edges vertex)))
    (set-vertex-edges! vertex (cdr edges))
    (car edges)))

; GRAPH is ((<symbol> . <symbol>*)*)
             
;(define (test-strong graph)
;  (let ((vertices (map (lambda (n)
;                         (vector (car n) #f #f))
;                       graph)))
;    (for-each (lambda (data vertex)
;                (vector-set! vertex 1 (map (lambda (s)
;                                             (first (lambda (v)
;                                                      (eq? s (vector-ref v 0)))
;                                                    vertices))
;                                           (cdr data))))
;              graph
;              vertices)
;    (map (lambda (l)
;           (map (lambda (n) (vector-ref n 0)) l))
;         (strongly-connected-components vertices
;                                        (lambda (v) (vector-ref v 1))
;                                        (lambda (v) (vector-ref v 2))
;                                        (lambda (v val)
;                                          (vector-set! v 2 val))))))







