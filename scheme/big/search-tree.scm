; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Red-Black binary search trees as described in Introduction to Algorithms
; by Cormen, Leiserson, and Rivest.  Look there if you want to understand
; the algorithm.
;
; These are like tables in that the value of a key defaults to #f.
;
; (make-search-tree key-= key-<)      -> tree
;
; (search-tree? value)                -> boolean
;
; (search-tree-ref tree key)          -> value
;
; (search-tree-set! tree key value)
;
; (search-tree-modify! tree key proc)
;   == (search-tree-set! tree key (proc (search-tree-ref tree key)))
;
; (search-tree-max tree)              -> key + value
; (pop-search-tree-max! tree)         -> key + value  (removes entry)
;
; (search-tree-min tree)              -> key + value
; (pop-search-tree-min! tree)         -> key + value  (removes entry)
;
; (walk-search-tree proc tree)
;   applies PROC in order to all key + value pairs with a non-#f value

(define-record-type tree :tree
  (make-tree lookup nil root)
  search-tree?
  (lookup tree-lookup)
  (nil    tree-nil)                  ; node marker for missing leaf nodes
  (root   tree-root set-tree-root!))

(define (make-search-tree = <)
  (let ((nil (make-node #f #f #f)))
    (set-node-red?! nil #f)
    (make-tree (make-lookup = <) nil #f)))

(define-record-type node :node
  (really-make-node key value parent red? left right)
  node?
  (key    node-key    set-node-key!)
  (value  node-value  set-node-value!)
  (parent node-parent set-node-parent!)    ; #f in the root node
  (red?   node-red?   set-node-red?!)      ; for balancing the tree
  (left   node-left   set-node-left!)      ; left and
  (right  node-right  set-node-right!))    ;          right subtrees

(define (make-node key value parent)
  (really-make-node key value parent #t #f #f))

(define-record-discloser :node
  (lambda (node)
    (list 'node (node-key node))))

; Lookup up KEY and return its value.

(define (search-tree-ref tree key)
  (call-with-values
   (lambda ()
     ((tree-lookup tree) tree key))
   (lambda (node parent left?)
     (if node
	 (node-value node)
	 #f))))

; Adding and modifying entries.

(define (search-tree-set! tree key value)
  (search-tree-modify! tree key (lambda (ignore) value)))

(define (search-tree-modify! tree key proc)
  (call-with-values
   (lambda ()
     ((tree-lookup tree) tree key))
   (lambda (node parent left?)
     (let ((new-value (proc (if node (node-value node) #f))))
       (cond ((and node new-value)
	      (set-node-value! node new-value))
	     (new-value
	      (really-insert! tree parent left? (make-node key new-value parent)))
	     (node
	      (really-delete! tree node)))))))
  
; Min and max entries.

(define (search-tree-max tree)
  (real-search-tree-max tree #f))

(define (pop-search-tree-max! tree)
  (real-search-tree-max tree #t))

(define (real-search-tree-max tree delete?)
  (let ((node (tree-root tree)))
    (if node
	(let loop ((node node))
	  (cond ((node-right node)
		 => loop)
		(else
		 (if delete?
		     (really-delete! tree node))
		 (values (node-key node) (node-value node)))))
	(values #f #f))))

(define (search-tree-min tree)
  (real-search-tree-min tree #f))

(define (pop-search-tree-min! tree)
  (real-search-tree-min tree #t))

(define (real-search-tree-min tree delete?)
  (let ((node (tree-root tree)))
    (if node
	(let loop ((node node))
	  (cond ((node-left node)
		 => loop)
		(else
		 (if delete?
		     (really-delete! tree node))
		 (values (node-key node) (node-value node)))))
	(values #f #f))))

(define (walk-search-tree proc tree)
  (let recur ((node (tree-root tree)))
    (cond (node
	   (recur (node-left node))
	   (proc (node-key node) (node-value node))
	   (recur (node-right node))))))

; Lookup up an entry.  Easy.
;
; Hack of checking common case reduced lookup time in a 1000 element search
; tree by a third.

(define (make-lookup tree-= tree-<)
  (if (and (eq? tree-= =)
	   (eq? tree-< <))
      default-lookup
      (lambda (tree key)
	(let loop ((node (tree-root tree))
		   (parent #f)
		   (left? #f))
	  (cond ((not node)
		 (values #f parent left?))
		((tree-= (node-key node) key)
		 (values node #f #f))
		((tree-< key (node-key node))
		 (loop (node-left node) node #t))
		(else
		 (loop (node-right node) node #f)))))))

(define (default-lookup tree key)
  (let loop ((node (tree-root tree))
	     (parent #f)
	     (left? #f))
    (cond ((not node)
	   (values #f parent left?))
	  ((= (node-key node) key)
	   (values node #f #f))
	  ((< key (node-key node))
	   (loop (node-left node) node #t))
	  (else
	   (loop (node-right node) node #f)))))

;----------------------------------------------------------------
; Little utilities.

; Parameterized node access

(define (node-child node left?)
  (if left?
      (node-left node)
      (node-right node)))

(define (set-node-child! node left? child)
  (if left?
      (set-node-left! node child)
      (set-node-right! node child)))

; Empty leaf slots are considered black.

(define (node-black? node)
  (not (and node (node-red? node))))

; The next node (used in REALLY-DELETE!)

(define (successor node)
  (cond ((node-right node)
	 => (lambda (node)
	      (let loop ((node node))
		(cond ((node-left node)
		       => loop)
		      (else node)))))
	(else
	 (let loop ((node node) (parent (node-parent node)))
	   (if (and parent
		    (eq? node (node-right parent)))
	       (loop parent (node-parent parent))
	       parent)))))

;----------------------------------------------------------------
; Add NODE as the LEFT? child of PARENT and balance the tree.

(define (really-insert! tree parent left? node)
  (if (not parent)
      (set-tree-root! tree node)
      (set-node-child! parent left? node))
  (fixup-insertion! node tree))

; Balance the tree after NODE has been inserted.

(define (fixup-insertion! node tree)
  (let loop ((node node))
    (let ((parent (node-parent node)))
      (if (and parent (node-red? parent))
	  (let* ((grand (node-parent parent))
		 (left? (eq? parent (node-left grand)))
		 (y (node-child grand (not left?))))
	    (cond ((node-black? y)
		   (let* ((node (cond ((eq? node (node-child parent (not left?)))
				       (rotate! parent left? tree)
				       parent)
				      (else node)))
			  (parent (node-parent node))
			  (grand (node-parent parent)))
		     (set-node-red?! parent #f)
		     (set-node-red?! grand  #t)
		     (rotate! grand (not left?) tree)
		     (loop node)))
		  (else
		   (set-node-red?! parent #f)
		   (set-node-red?! y      #f)
		   (set-node-red?! grand  #t)
		   (loop grand)))))))
  (set-node-red?! (tree-root tree) #f))

;        A                                 B
;       / \    =(rotate! A #f tree)=>     / \
;      B   k                             i   A
;     / \      <=(rotate! B #t tree)=       / \
;    i   j                                 j   k

(define (rotate! node left? tree)
  (let* ((y (node-child node (not left?)))
	 (y-left (node-child y left?))
	 (parent (node-parent node)))
    (set-node-child! node (not left?) y-left)
    (if y-left
	(set-node-parent! y-left node))
    (replace! parent y node tree)
    (set-node-child! y left? node)
    (set-node-parent! node y)))
		
; Replace CHILD (of PARENT) with NEW-CHILD

(define (replace! parent new-child child tree)
    (set-node-parent! new-child parent)
    (cond ((eq? child (tree-root tree))
	   (set-tree-root! tree new-child))
	  ((eq? child (node-left parent))
	   (set-node-left! parent new-child))
	  (else
	   (set-node-right! parent new-child))))

; Remove NODE from tree.

(define (really-delete! tree node)
  (let* ((y (cond ((or (not (node-left node))
		       (not (node-right node)))
		   node)
		  (else
		   (let ((y (successor node)))
		     (set-node-key!   node (node-key y))
		     (set-node-value! node (node-value y))
		     y))))
	 (x (or (node-left y)
		(node-right y)
		(let ((x (tree-nil tree)))
		  (set-node-right! y x)
		  x)))
	 (parent (node-parent y)))
    (replace! parent x y tree)
    (if (not (node-red? y))
	(fixup-delete! x tree))
    (let ((nil (tree-nil tree)))
      (cond ((node-parent nil)
	     => (lambda (p)
		  (if (eq? (node-right p) nil)
		      (set-node-right! p #f)
		      (set-node-left! p #f))
		  (set-node-parent! (tree-nil tree) #f)))
	    ((eq? nil (tree-root tree))
	     (set-tree-root! tree #f))))))

(define (fixup-delete! x tree)
  (let loop ((x x))
    (if (or (eq? x (tree-root tree))
	    (node-red? x))
	(set-node-red?! x #f)
	(let* ((parent (node-parent x))
	       (left? (eq? x (node-left parent)))
	       (w (node-child parent (not left?)))
	       (w (cond ((node-red? w)
			 (set-node-red?! w #f)
			 (set-node-red?! parent #t)
			 (rotate! parent left? tree)
			 (node-child (node-parent x) (not left?)))
			(else
			 w))))
	  (cond ((and (node-black? (node-left w))
		      (node-black? (node-right w)))
		 (set-node-red?! w #t)
		 (loop (node-parent x)))
		(else
		 (let ((w (cond ((node-black? (node-child w (not left?)))
				 (set-node-red?! (node-child w left?) #f)
				 (set-node-red?! w #t)
				 (rotate! w (not left?) tree)
				 (node-child (node-parent x) (not left?)))
				(else
				 w))))
		   (let ((parent (node-parent x)))
		     (set-node-red?! w (node-red? parent))
		     (set-node-red?! parent #f)
		     (set-node-red?! (node-child w (not left?)) #f)
		     (rotate! parent left? tree)
		     (set-node-red?! (tree-root tree) #f)))))))))
		 
; Verify that the coloring is correct
;
;(define (okay-tree? tree)
;  (receive (okay? red? count)
;      (let recur ((node (tree-root tree)))
;        (if (not node)
;            (values #t #f 0)
;            (receive (l-ok? l-r? l-c)
;                (recur (node-left node))
;              (receive (r-ok? r-r? r-c)
;                  (recur (node-right node))
;                (values (and l-ok?
;                             r-ok?
;                             (not (and (node-red? node)
;                                       (or l-r? r-r?)))
;                             (= l-c r-c))
;                        (node-red? node)
;                        (if (node-red? node)
;                            l-c
;                            (+ l-c 1)))))))
;    okay?))
;
;
;(define (walk-sequences proc list)
;  (let recur ((list list) (r '()))
;    (if (null? list)
;        (proc (reverse r))
;        (let loop ((list list) (done '()))
;          (if (not (null? list))
;              (let ((next (car list)))
;                (recur (append (reverse done) (cdr list)) (cons next r))
;                (loop (cdr list) (cons next done))))))))
;           
;(define (tree-test n)
;  (let ((iota (do ((i n (- i 1))
;                   (l '() (cons i l)))
;                  ((<= i 0) l))))
;    (walk-sequences (lambda (in)
;                      (walk-sequences (lambda (out)
;                                        (do-tree-test in out))
;                                      iota))
;                    iota)
;    #t))
;
;(define (do-tree-test in out)
;  (let ((tree (make-search-tree = <)))
;    (for-each (lambda (i)
;                (search-tree-set! tree i (- 0 i)))
;              in)
;    (if (not (okay-tree? tree))
;        (breakpoint "tree ~S is not okay" in))
;    (if (not (tree-ordered? tree (length in)))
;        (breakpoint "tree ~S is not ordered" in))
;    (for-each (lambda (i)
;                (if (not (= (search-tree-ref tree i) (- 0 i)))
;                    (breakpoint "looking up ~S in ~S lost" i in)))
;              in)
;    (do ((o out (cdr o)))
;        ((null? o))
;      (search-tree-set! tree (car o) #f)
;      (if (not (okay-tree? tree))
;          (breakpoint "tree ~S is not okay after deletions ~S" in out)))))
;
;(define (tree-ordered? tree count)
;  (let ((l '()))
;    (walk-search-tree (lambda (key value)
;                        (set! l (cons (cons key value) l)))
;                      tree)
;    (let loop ((l l) (n count))
;      (cond ((null? l)
;             (= n 0))
;            ((and (= (caar l) n)
;                  (= (cdar l) (- 0 n)))
;             (loop (cdr l) (- n 1)))
;            (else #f)))))
;
;(define (do-tests tester)
;  (do ((i 0 (+ i 1)))
;      (#f)
;    (tester i)
;    (format #t " done with ~D~%" i)))
;
;(define (another-test n)
;  (let ((iota (do ((i n (- i 1))
;                   (l '() (cons i l)))
;                  ((<= i 0) l))))
;    (walk-sequences (lambda (in)
;                      (do ((i 1 (+ i 1)))
;                          ((> i n))
;                        (let ((tree (make-search-tree = <)))
;                          (for-each (lambda (i)
;                                      (search-tree-set! tree i (- 0 i)))
;                                    in)
;                          (if (not (okay-tree? tree))
;                              (breakpoint "tree ~S is not okay" in))
;                          (if (not (tree-ordered? tree (length in)))
;                              (breakpoint "tree ~S is not ordered" in))
;                          (for-each (lambda (i)
;                                      (if (not (= (search-tree-ref tree i) (- 0 i)))
;                                          (breakpoint "looking up ~S in ~S lost" i in)))
;                                    in)
;                          (search-tree-set! tree i #f)
;                          (if (not (okay-tree? tree))
;                              (breakpoint "tree ~S is not okay after deletion ~S"
;                                          in i))
;                          (for-each (lambda (j)
;                                      (let ((ref (search-tree-ref tree j)))
;                                        (if (not (eq? ref (if (= j i) #f (- 0 j))))
;                                            (breakpoint "looking up ~S in ~S lost" i in))))
;                                    in))))
;                    iota)))
