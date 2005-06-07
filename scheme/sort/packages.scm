;;; Package defs for the Scheme Underground sorting package,
;;; in the Scheme 48 module language.

;;; The general sort package:

(define-structure sorting sorting-interface
  (open scheme
	list-merge-sort
	vector-heap-sort
	vector-merge-sort
	sorted
	delete-neighbor-duplicates)
  (files sort)
  (optimize auto-integrate))

(define-structure sorted sorted-interface
  (open scheme
	vector-utils)
  (files sortp)
  (optimize auto-integrate))

(define-structure delete-neighbor-duplicates delete-neighbor-duplicates-interface
  (open scheme
	vector-utils)
  (files delndups)
  (optimize auto-integrate))

(define-structure binary-searches binary-searches-interface
  (open scheme
	vector-utils)
  (files vbinsearch))

(define-structure list-merge-sort list-merge-sort-interface
  (open scheme
	(subset signals (error)))
  (files lmsort)
  (optimize auto-integrate))

(define-structure vector-merge-sort vector-merge-sort-interface
  (open scheme
	vector-utils
	vector-insertion-sort-internal)
  (files vmsort)
  (optimize auto-integrate))

(define-structure vector-heap-sort vector-heap-sort-interface
  (open scheme
	vector-utils)
  (files vhsort)
  (optimize auto-integrate))

(define-structures ((vector-insertion-sort vector-insertion-sort-interface)
		    (vector-insertion-sort-internal
		     vector-insertion-sort-internal-interface))
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
