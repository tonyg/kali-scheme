;;; Interface defs for the Scheme Underground sorting package,
;;; in the Scheme48 module language.

;;; The delete neighbor-dups interface
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-delete-neighbor-dups l = -> list
;;; vector-delete-neighbor-dups v = [start end] -> vector

(define-interface delndups-interface
  (export (list-delete-neighbor-dups (proc (:value (proc (:value) :boolean))
					   :value))
	  (vector-delete-neighbor-dups
	   (proc (:vector (proc (:value) :boolean)
			  &opt :exact-integer :exact-integer)
		 :vector))))


;;; The general sort interface:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-sorted? l <      -> boolean
;;;
;;; list-merge  l1 l2 <	  -> list
;;; list-merge! l1 l2 <	  -> list
;;;
;;; list-sort         l <  -> list			
;;; list-sort!        l <  -> list			
;;; list-stable-sort  l < -> list			
;;; list-stable-sort! l < -> list			
;;; 
;;; list-delete-neighbor-dups l = -> list
;;; 
;;; vector-sorted? v < [start end]	-> boolean
;;;
;;; vector-merge  v1 v2 < [start1 end1 start2 end2]         -> vector
;;; vector-merge! v v1 v2 < [start start1 end1 start2 end2] -> unspecific
;;;
;;; vector-sort  v < [start end] 	-> vector
;;; vector-sort! v < 			-> unspecific
;;;
;;; vector-stable-sort  v < [start end] -> vector
;;; vector-stable-sort! v <             -> unspecific		
;;;
;;; vector-delete-neighbor-dups v = [start end] -> vector

(define-interface sort-interface
  (compound-interface
    (export
   
     (list-sorted? (proc (:value (proc (:value :value) :boolean)) :boolean))

     ((list-merge list-merge!)
      (proc (:value :value (proc (:value :value) :boolean)) :value))

     ((list-sort list-sort! list-stable-sort list-stable-sort!)
      (proc (:value (proc (:value :value) :boolean)) :value))

     (vector-sorted? (proc (:vector (proc (:value :value) :boolean)) :boolean))

     (vector-merge  (proc (:vector :vector
				   (proc (:value :value) :boolean)
				   &opt :exact-integer :exact-integer
				   :exact-integer :exact-integer)
			  :vector))

     (vector-merge! (proc (:vector :vector :vector
				   (proc (:value :value) :boolean)
				   &opt :exact-integer :exact-integer :exact-integer
				   :exact-integer :exact-integer)
			  :unspecific))
			
     ((vector-sort vector-stable-sort)
      (proc (:vector (proc (:value :value) :boolean)
		     &opt :exact-integer :exact-integer)
	    :vector))

     ((vector-sort! vector-stable-sort!)
      (proc (:vector (proc (:value :value) :boolean)) :unspecific)))

    delndups-interface))


;;; sort? - sorted predicates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-sorted? l <		   -> boolean
;;; vector-sorted? v < [start end] -> boolean

(define-interface sorted?-interface
  (export (list-sorted? (proc (:value (proc (:value :value) :boolean)) :boolean))
	  (vector-sorted? (proc (:vector (proc (:value :value) :boolean)
					 &opt :exact-integer :exact-integer)
				:boolean))))


;;; lmsort - list merge sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-merge-sort  l <	-> list
;;; list-merge-sort! l <	-> list
;;; list-merge  lis lis <	-> list
;;; list-merge! lis lis <	-> list

(define-interface lmsort-interface
  (export ((list-merge-sort list-merge-sort!)
	   (proc (:value (proc (:value :value) :boolean)) :value))
	  ((list-merge list-merge!)
	   (proc (:value :value (proc (:value :value) :boolean)) :value))))


;;; vmsort - vector merge sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; vector-merge-sort  v < [start end temp]			-> vector
;;; vector-merge-sort! v < [start end temp]			-> unspecific
;;; vector-merge  v1 v2 < [start1 end1 start2 end2]		-> vector
;;; vector-merge! v v1 v2 < [start0 start1 end1 start2 end2]	-> unspecific

(define-interface vmsort-interface
  (export
   (vector-merge-sort  (proc (:vector (proc (:value :value) :boolean)
				      &opt :exact-integer :exact-integer
				           :vector)
			     :vector))
   (vector-merge-sort! (proc (:vector (proc (:value :value) :boolean)
				      &opt :exact-integer :exact-integer
				           :vector)
			     :unspecific))
   (vector-merge (proc (:vector :vector
			(proc (:value :value) :boolean)
			&opt :exact-integer :exact-integer
			     :exact-integer :exact-integer)
		       :vector))
   (vector-merge! (proc (:vector :vector :vector
			 (proc (:value :value) :boolean)
			 &opt :exact-integer :exact-integer :exact-integer
			      :exact-integer :exact-integer)
			:unspecific))))


;;; vhsort - vector heap sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; heap-sort  v < [start end] -> vector
;;; heap-sort! v <	       -> unspecific
;;; heap-sort->list! v <       -> list

(define-interface vhsort-interface
  (export (heap-sort (proc (:vector (proc (:value :value) :boolean)
				    &opt :exact-integer :exact-integer)
			   :vector))
	  (heap-sort! (proc (:vector (proc (:value :value) :boolean)) :unspecific))
	  (heap-sort->list! (proc (:vector (proc (:value :value) :boolean)) :value))))

;;; vqsort - vector quick sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; quick-sort  v < [start end] -> vector
;;; quick-sort! v < [start end]	-> unspecific

(define-interface vqsort-interface
  (export (quick-sort (proc (:vector (proc (:value :value) :boolean)
				     &opt :exact-integer :exact-integer)
			    :vector))
	  (quick-sort! (proc (:vector (proc (:value :value) :boolean)
				      &opt :exact-integer :exact-integer)
			     :unspecific))))

;;; visort - vector insertion sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; insert-sort  v < [start end] -> vector
;;; insert-sort! v < [start end] -> unspecific
;;;
;;; internal:
;;; %insert-sort! v < start end -> unspecific

(define-interface visort-interface
  (export (insert-sort (proc (:vector (proc (:value :value) :boolean)
				      &opt :exact-integer :exact-integer)
			     :vector))
	  (insert-sort! (proc (:vector (proc (:value :value) :boolean)
				       &opt :exact-integer :exact-integer)
			      :unspecific))))

(define-interface visort-internals-interface
  (export (%insert-sort! (proc (:vector (proc (:value :value) :boolean)
					:exact-integer :exact-integer)
			       :unspecific))))
