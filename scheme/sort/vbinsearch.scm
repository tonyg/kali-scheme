;;; The SRFI-32 sort package -- binary search			-*- Scheme -*-
;;; Copyright (c) 1998 by Olin Shivers.
;;; This code is in the public domain.
;;; Olin Shivers 98/11

;;; Returns the index of the matching element.
;;; (vector-binary-search < car 4 '#((1 . one) (3 . three)
;;;                                  (4 . four) (25 . twenty-five)))
;;;   => 2

(define (vector-binary-search key< elt->key key v . maybe-start+end)
  (let-vector-start+end (start end) vector-binary-search v maybe-start+end
    (let lp ((left start) (right end))	; Search V[left,right).
      (and (< left right)
	   (let* ((m (quotient (+ left right) 2))
		  (elt (vector-ref v m))
		  (elt-key (elt->key elt)))
	     (cond ((key< key elt-key) (lp left m))
		   ((key< elt-key key) (lp (+ m 1) right))
		   (else m)))))))

(define (vector-binary-search3 compare v . maybe-start+end)
  (let-vector-start+end (start end) vector-binary-search v maybe-start+end
    (let lp ((left start) (right end))	; Search V[left,right).
      (and (< left right)
	   (let* ((m (quotient (+ left right) 2))
		  (sign (compare (elt->key (vector-ref v m)))))
	     (cond ((> sign 0) (lp left m))
		   ((< sign 0) (lp (+ m 1) right))
		   (else m)))))))

;;; These procedures are completely R5RS except for the LET-VECTOR-START+END
;;; macro, which parses out the optional START/END arguments from the rest
;;; argument.
