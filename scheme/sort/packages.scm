;;; Package defs for the Scheme Underground sorting package,
;;; in the Scheme48 module language.

;;; Packages defined:
;;;   sort-package
;;;   sorted?-package
;;;   lmsort-package
;;;   vmsort-package
;;;   vhsort-package
;;;   vqsort-package
;;;   visort-package

;;; The general sort package:
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

(define-structure sort-package sort-interface
  (open lmsort-package vhsort-package vmsort-package sorted?-package
	delndups-package scheme)
  (files sort)
  (optimize auto-integrate))


;;; sort? - sorted predicates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-sorted? l <		   -> boolean
;;; vector-sorted? v < [start end] -> boolean

(define-structure sorted?-package sorted?-interface
  (open let-vecopt scheme)
  (files sortp)
  (optimize auto-integrate))

;;; delndups - deleting neighbor duplicates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-delete-neighbor-dups l = -> list
;;; vector-delete-neighbor-dups v = [start end] -> vector

(define-structure delndups-package delndups-interface
  (open let-vecopt scheme)
  (files delndups)
  (optimize auto-integrate))



;;; lmsort - list merge sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; list-merge-sort  l <	-> list
;;; list-merge-sort! l <	-> list
;;; list-merge  lis lis <	-> list
;;; list-merge! lis lis <	-> list

(define-structure lmsort-package lmsort-interface
  (open receiving list-lib scheme)
  (files lmsort)
  (optimize auto-integrate))



;;; vmsort - vector merge sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; vector-merge-sort  v < [start end temp]			-> vector
;;; vector-merge-sort! v < [start end temp]			-> unspecific
;;; vector-merge  v1 v2 < [start1 end1 start2 end2]		-> vector
;;; vector-merge! v v1 v2 < [start0 start1 end1 start2 end2]	-> unspecific

(define-structure vmsort-package vmsort-interface
  (open vector-hax let-opt visort-internals-package scheme)
  (files vmsort)
  (optimize auto-integrate))



;;; vhsort - vector heap sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; heap-sort  v < [start end] -> vector
;;; heap-sort! v <	       -> unspecific
;;; heap-sort->list! v <       -> list

(define-structure vhsort-package vhsort-interface
  (open vector-hax let-vecopt receiving scheme)
  (files vhsort)
  (optimize auto-integrate))



;;; vqsort - vector quick sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; quick-sort  v < [start end] -> vector
;;; quick-sort! v < [start end]	-> unspecific

(define-structure vqsort-package vqsort-interface
  (open visort-internals-package vector-hax let-vecopt receiving scheme)
  (files vqsort)
  (optimize auto-integrate))


;;; visort - vector insertion sort
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; insert-sort  v < [start end] -> vector
;;; insert-sort! v < [start end] -> unspecific

(define-structures ((visort-package visort-interface)
		    (visort-internals-package visort-internals-interface))
  (open vector-hax let-vecopt scheme)
  (files visort)
  (optimize auto-integrate))
