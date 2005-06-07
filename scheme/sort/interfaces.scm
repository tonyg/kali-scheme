;;; Interface defs for the Scheme Underground sorting package,
;;; in the Scheme 48 module language.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-delete-neighbor-dups = l -> list
;;; vector-delete-neighbor-dups = v [start end] -> vector
;;; vector-delete-neighbor-dups! = v [start end] -> vector
;;;

(define-interface delete-neighbor-duplicates-interface
  (export (list-delete-neighbor-dups
	   (proc ((proc (:value :value) :boolean)
		  :value)
		 :value))
	  (vector-delete-neighbor-dups
	   (proc ((proc (:value :value) :boolean)
		  :vector
		  &opt
		  :exact-integer :exact-integer)
		 :vector))
	  (vector-delete-neighbor-dups!
	   (proc ((proc (:value :value) :boolean)
		  :vector
		  &opt
		  :exact-integer :exact-integer)
		 :vector))))

;;;    vector-binary-search  elt< elt->key key v [start end] -> integer-or-false
;;;    vector-binary-search3 c                 v [start end] -> integer-or-false

(define-interface binary-searches-interface
  (export vector-binary-search
	  vector-binary-search3))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-sorted? l <		   -> boolean
;;; vector-sorted? v < [start end] -> boolean

(define-interface sorted-interface
  (export (list-sorted? (proc (:value (proc (:value :value) :boolean)) :boolean))
	  (vector-sorted? (proc ((proc (:value :value) :boolean)
				 :vector 
				 &opt :exact-integer :exact-integer)
				:boolean))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-merge-sort  < l -> list
;;; list-merge-sort! < l -> list
;;; list-merge  < lis lis -> list
;;; list-merge! < lis lis -> list

(define-interface list-merge-sort-interface
  (export ((list-merge-sort list-merge-sort!)
	   (proc ((proc (:value :value) :boolean) :value) :value))
	  ((list-merge list-merge!)
	   (proc ((proc (:value :value) :boolean) :value :value) :value))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; vector-merge-sort  < v [start end temp]			-> vector
;;; vector-merge-sort! < v [start end temp]			-> unspecific
;;; vector-merge  < v1 v2 [start1 end1 start2 end2]		-> vector
;;; vector-merge! < v v1 v2 [start0 start1 end1 start2 end2]	-> unspecific

(define-interface vector-merge-sort-interface
  (export
   (vector-merge-sort  (proc ((proc (:value :value) :boolean)
			      :vector 
			      &opt
			      :exact-integer :exact-integer
			      :vector)
			     :vector))
   (vector-merge-sort! (proc ((proc (:value :value) :boolean)
			      :vector 
			      &opt
			      :exact-integer :exact-integer
			      :vector)
			     :unspecific))
   (vector-merge (proc ((proc (:value :value) :boolean)
			:vector :vector
			&opt
			:exact-integer :exact-integer
			:exact-integer :exact-integer)
		       :vector))
   (vector-merge! (proc ((proc (:value :value) :boolean)
			 :vector :vector :vector
			 &opt
			 :exact-integer :exact-integer :exact-integer
			 :exact-integer :exact-integer)
			:unspecific))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; vector-heap-sort  < v [start end] -> vector
;;; vector-heap-sort! < v        -> unspecific

(define-interface vector-heap-sort-interface
  (export (vector-heap-sort (proc ((proc (:value :value) :boolean)
				   :vector 
				   &opt :exact-integer :exact-integer)
				  :vector))
	  (vector-heap-sort! (proc ((proc (:value :value) :boolean) :vector) :unspecific))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; vector-insert-sort  < v [start end] -> vector
;;; vector-insert-sort! < v [start end] -> unspecific
;;;
;;; internal:
;;; %vector-insert-sort! < v start end -> unspecific

(define-interface vector-insertion-sort-interface
  (export (vector-insert-sort (proc ((proc (:value :value) :boolean)
				     :vector 
				     &opt :exact-integer :exact-integer)
				    :vector))
	  (vector-insert-sort! (proc ((proc (:value :value) :boolean)
				      :vector 
				      &opt :exact-integer :exact-integer)
				     :unspecific))))

(define-interface vector-insertion-sort-internal-interface
  (export (%vector-insert-sort! (proc ((proc (:value :value) :boolean)
				       :vector 
				       :exact-integer :exact-integer)
				      :unspecific))))

;;; The general sort interface:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-sorted? < l      -> boolean
;;;
;;; list-merge  < l1 l2	  -> list
;;; list-merge! < l1 l2	  -> list
;;;
;;; list-sort         < l -> list			
;;; list-sort!        < l -> list			
;;; list-stable-sort  < l -> list			
;;; list-stable-sort! < l -> list			
;;; 
;;; list-delete-neighbor-dups l = -> list
;;; 
;;; vector-sorted? < v [start end]	-> boolean
;;;
;;; vector-merge  < v1 v2 [start1 end1 start2 end2]         -> vector
;;; vector-merge! < v v1 v2 [start start1 end1 start2 end2] -> unspecific
;;;
;;; vector-sort  < v [start end] 	-> vector
;;; vector-sort! < v 			-> unspecific
;;;
;;; vector-stable-sort  < v [start end] -> vector
;;; vector-stable-sort! < v             -> unspecific		
;;;
;;; vector-delete-neighbor-dups v = [start end] -> vector

(define-interface sorting-interface
  (compound-interface
   sorted-interface
   (export
    
    ((list-merge list-merge!)
     (proc ((proc (:value :value) :boolean) :value :value) :value))
    
    ((list-sort list-sort! list-stable-sort list-stable-sort!)
     (proc ((proc (:value :value) :boolean) :value) :value))
    
    (vector-merge  (proc ((proc (:value :value) :boolean)
			  :vector :vector
			  &opt
			  :exact-integer :exact-integer
			  :exact-integer :exact-integer)
			 :vector))
    
    (vector-merge! (proc ((proc (:value :value) :boolean) 
			  :vector :vector :vector
			  &opt
			  :exact-integer :exact-integer :exact-integer
			  :exact-integer :exact-integer)
			 :unspecific))
    
    ((vector-sort vector-stable-sort)
     (proc ((proc (:value :value) :boolean)
	    :vector 
	    &opt
	    :exact-integer :exact-integer)
	   :vector))

    ((vector-sort! vector-stable-sort!)
     (proc ((proc (:value :value) :boolean) :vector) :unspecific))

    (list-delete-neighbor-dups
     (proc ((proc (:value :value) :boolean)
	    :value)
	   :value))
    (vector-delete-neighbor-dups
     (proc ((proc (:value :value) :boolean)
	    :vector
	    &opt
	    :exact-integer :exact-integer)
	   :vector)))))
