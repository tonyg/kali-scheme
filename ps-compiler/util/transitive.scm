
; General transitive closure

; (transitive <nodes>                ; a list of nodes
;             <node-parents>         ; returns the parents of a node
;             <node-children>        ; returns the children of a node
;             <node-elements>        ; returns the elements of a node
;             <set-node-elements!>   ; sets the elements of a node
;             <node-kills-elements>  ; the node ignores these elements if it
;                                    ;  has them or receives them from a parent
;             <element-temp>         ; two procedures that provide a temporary
;             <set-element-temp!>    ;  slot in elements
;             )
;
; Only one of <node-parents> or <node-children> needs to be specified.  The
; other should be #F.
; If <node-kills-elements> is #F then no elements are killed.
;
; Every node sends to its children all (<node-elements> node) that are not in
; (<node-kill-elements> child).  This process iterates until (<node-elements>
; node) is stable for all nodes.
;------------------------------

; 1) put the distinct elements into a vector and mark them with their place
; 2) make a record for each thing, including sets for elements and kills
; 3) set the children fields of the thing records
; 4) do the transitive closure
; 5) clean up and translate the element sets back into lists

(define (transitive user-things parents children elts set-elts! kills
		    elt-hash set-elt-hash!)
  (let* ((elt-unhash-vec
	  (element-hasher user-things elts elt-hash set-elt-hash!))
	 (elt-hash (make-element-hash elt-hash))
	 (things (map (lambda (user-thing)
			(make-thing user-thing elts set-elts! kills
				    elt-hash elt-unhash-vec))
		      user-things)))
    (if parents
	(set-thing-kids-using-parents!  things parents  elts)
	(set-thing-kids-using-children! things children elts))
    (real-transitive-closure! things kills)
    (for-each (lambda (thing)
		(record-results thing elt-unhash-vec set-elts!))
	      things)
    (do ((i 0 (+ i 1)))
	((>= i (vector-length elt-unhash-vec)))
      (set-elt-hash! (vector-ref elt-unhash-vec i) #f))
    (values)))

;------------------------------------------------------------
; Counting the elements and assigning numbers to them

(define-record-type element-hash
  (number  ; the element-hash record is just a way of tagging this number
   )       ; with a unique predicate
  ())

(define (element-hasher things elts elt-hash set-elt-hash!)
  (let loop ((to-do '()) (ts things) (all-elts '()) (count 0))
    (cond ((null? to-do)
	   (if (null? ts)
	       (real-element-hasher all-elts count)
	       (loop (elts (car ts)) (cdr ts) all-elts count)))
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

;------------------------------------------------------------
; Adding records to the things

(define-record-type thing
  (data            ; user's data
   (elt-set)       ; element set
   kill-set        ; kill set
   )
  (
   (kids '())
   (changed? #f)
   ))
  
(define (make-thing user-thing elts set-elts! kills elt-hash elt-vec)
  (let ((new (thing-maker user-thing
			  (make-element-set (elts user-thing)
					    elt-hash elt-vec)
			  (if kills
			      (make-element-set (kills user-thing)
						elt-hash elt-vec)
			      #f))))
    (set-elts! user-thing new)
    new))

(define (make-element-set elts elt-hash elt-vec)
  (let loop ((elts elts) (set (make-empty-integer-set (vector-length elt-vec))))
    (if (null? elts)
	set
	(loop (cdr elts)
	      (cond ((elt-hash (car elts))
		     => (lambda (hash)
			  (add-to-integer-set! set hash)))
		    (else set))))))

;------------------------------------------------------------
; Setting the kids field of the things

(define (set-thing-kids-using-children! things children thing-slot)
  (for-each
   (lambda (thing)
     (set-thing-kids! thing
		      (map (lambda (kid)
			     (let ((t (thing-slot kid)))
			       (if (not (thing? t))
				   (missing-thing-error kid "child" thing))
			       t))
			   (children (thing-data thing)))))
   things))

(define (set-thing-kids-using-parents! things parents thing-slot)
  (for-each
   (lambda (thing)
     (for-each (lambda (parent)
		 (let ((parent-thing (thing-slot parent)))
		   (if (not (thing? parent-thing))
		       (missing-thing-error parent-thing "parent" thing))
		   (set-thing-kids! parent-thing
				    (cons thing
					  (thing-kids parent-thing)))))
	       (parents (thing-data thing))))
   things))

(define (missing-thing-error thing relationship relation)
  (error (format #f "Transitive - ~S, ~A of ~S, not in list of nodes"
		 thing relationship (thing-data relation))))

;------------------------------------------------------------
; Turn the element sets into lists of elements.

(define (record-results thing elt-unhash-vec set-elts!)
  (set-elts! (thing-data thing)
	     (map-over-integer-set
	      (lambda (i) (vector-ref elt-unhash-vec i))
	      (thing-elt-set thing))))

;------------------------------------------------------------
; The actual algorithm - keeps passing elements around until the changes
; stop.

(define (real-transitive-closure! things kills)
  (let ((op (if kills update-thing update-thing-no-kill)))
    (let loop ((to-do things))
      (cond ((not (null? to-do))
	     (let ((elt-set (thing-elt-set (car to-do))))
	       (set-thing-changed?! (car to-do) #f)
	       (let kids-loop ((ts (thing-kids (car to-do)))
			       (to-do (cdr to-do)))
		 (cond ((null? ts)
			(loop to-do))
		       ((and (op (car ts) elt-set)
			     (not (thing-changed? (car ts))))
			(set-thing-changed?! (car ts) #t)
			(kids-loop (cdr ts) (cons (car ts) to-do)))
		       (else
			(kids-loop (cdr ts) to-do))))))))))

; The weird function INTEGER-SET-SUBTRACT&IOR-WITH-TEST! takes three integer
; sets, subtracts the second from the first and inclusive OR's the result
; with the third.  It returns the resulting set and a flag which is #T if
; the result is not the same as the original third set.  The inclusive OR
; may be destructive.

(define (update-thing thing elt-set)
  (receive (set change?)
      (integer-set-subtract&ior-with-test! elt-set
					   (thing-kill-set thing)
					   (thing-elt-set thing))
    (set-thing-elt-set! thing set)
    change?))

(define (update-thing-no-kill thing elt-set)
  (receive (set change?)
      (integer-set-ior-with-test! elt-set
				  (thing-elt-set thing))
    (set-thing-elt-set! thing set)
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
             
(define (test-transitive graph)
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
    (transitive vertices                             ; user-things
		#f                                   ; parents
		(lambda (v) (vector-ref v 3))        ; children
		(lambda (v) (vector-ref v 1))        ; elts
		(lambda (v x) (vector-set! v 1 x))   ; set-elts!
		(if (every? (lambda (n) (null? (caddr n))) graph)
		    #f
		    (lambda (v) (vector-ref v 2)))   ; kills
		(lambda (e) (vector-ref e 1))        ; elt-hash
		(lambda (e x) (vector-set! e 1 x)))  ; set-elt-hash!
    (map (lambda (v)
	   (list (vector-ref v 0)
		 (map (lambda (e) (vector-ref e 0))
		      (vector-ref v 1))))
	 vertices)))
