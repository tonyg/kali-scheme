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

(define-structure sort-package sort-interface
  (open lmsort-package vhsort-package vmsort-package sorted?-package
	delndups-package scheme)
  (files sort)
  (optimize auto-integrate))


;;; sort? - sorted predicates

(define-structure sorted?-package sorted?-interface
  (open scheme
	vector-utils)
  (files sortp)
  (optimize auto-integrate))

;;; delndups - deleting neighbor duplicates

(define-structure delndups-package delndups-interface
  (open scheme
	receiving
	formats
	vector-utils)
  (files delndups)
  (optimize auto-integrate))

;; binary search

(define-structure binary-searches binary-searches-interface
  (open scheme
	vector-utils)
  (files vbinsearch))

;;; lmsort - list merge sort

(define-structure lmsort-package lmsort-interface
  (open scheme
	receiving
	(subset signals (error)))
  (files lmsort)
  (optimize auto-integrate))

;;; vmsort - vector merge sort

(define-structure vmsort-package vmsort-interface
  (open scheme
	receiving
	vector-utils
	visort-internals-package)
  (files vmsort)
  (optimize auto-integrate))

;;; vhsort - vector heap sort

(define-structure vhsort-package vhsort-interface
  (open scheme
	receiving
	vector-utils)
  (files vhsort)
  (optimize auto-integrate))

;;; visort - vector insertion sort

(define-structures ((visort-package visort-interface)
		    (visort-internals-package visort-internals-interface))
  (open scheme
	vector-utils)
  (files visort)
  (optimize auto-integrate))

(define-structure vector-utils (export vector-copy
				       vector-portion-copy
				       vector-portion-copy!
				       vector-start+end
				       vectors-start+end-2)
  (open scheme)
  (files vector-util))
