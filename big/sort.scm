;;; Copyright (c) 1985 Yale University
;;;     Authors: N Adams, R Kelsey, D Kranz, J Philbin, J Rees.

;;; This material was developed by the T Project at the Yale
;;; University Computer Science Department.  Permission to copy this
;;; software, to redistribute it, and to use it for any purpose is
;;; granted, subject to the following restric- tions and
;;; understandings.
;;; 1. Any copy made of this software must include this copyright
;;;    notice in full.
;;; 2. Users of this software agree to make their best efforts (a) to return
;;;    to the T Project at Yale any improvements or extensions that they make,
;;;    so that these may be included in future releases; and (b) to inform
;;;    the T Project of noteworthy uses of this software.
;;; 3. All materials developed as a consequence of the use of this software
;;;    shall duly acknowledge such use, in accordance with the usual standards
;;;    of acknowledging credit in academic research.
;;; 4. Yale has made no warrantee or representation that the operation of
;;;    this software will be error-free, and Yale is under no obligation to
;;;    provide any services, by way of maintenance, update, or otherwise.
;;; 5. In conjunction with products arising from the use of this material,
;;;    there shall be no use of the name of the Yale University nor of any
;;;    adaptation thereof in any advertising, promotional, or sales literature
;;;    without prior written consent from Yale in each case.
;;;

;;; We gratefully acknowledge Bob Nix

;;;  SORT:ONLINE-MERGE-SORT!
;;;  =======================
;;;  On-Line Merge sort, a fast and stable algorithm for sorting a list.
;;;  This is a very neat algorithm!  Consider the following code:
;;;
;;;    (DEFINE (MERGE-SORT L)
;;;        (IF (NULL? (CDR L))
;;;            L
;;;            (MERGE (MERGE-SORT (FIRST-HALF-OF L))
;;;                   (MERGE-SORT (SECOND-HALF-OF L)))))
;;;
;;;  The nested calls to MERGE above form a binary tree, with MERGE's of
;;;  singleton lists at the leaves, and a MERGE of two lists of size N/2 at
;;;  the top.  The algorithm below traverses this MERGE-tree in post-order,
;;;  moving from the lower left hand corner to the right.
;;;
;;;  This algorithm sorts N objects with about NlgN+2N comparisons and exactly
;;;  lgN conses.  The algorithm used is a version of mergesort that is
;;;  amenable to Lisp's data accessing primitives.  The first phase of the
;;;  algorithm is an "addition" phase in which each element X is added to
;;;  a list of lists of sorted runs B in much the same manner as a one is
;;;  added to a binary number.  If the first "digit" of B is 0, i.e. the first
;;;  list in B is NIL, then the element to be added becomes the first digit
;;;  of B.  If that digit is non empty then you merge the digit with X and
;;;  recurse on the rest of B -- setting the first digit of B to be zero.
;;;  For example:
;;;
;;;   Reversed      LIST B
;;;   Binary #     Each sublist is sorted.
;;;
;;;    0000        ()
;;;    1000        ((x))
;;;    0100        (()  (x x))
;;;    1100        ((x) (x x))
;;;    0010        (()  ()    (x x x x))
;;;    1010        ((x) ()    (x x x x))
;;;    0110        (()  (x x) (x x x x))
;;;    1110        ((x) (x x) (x x x x))
;;;    0001        (()  ()    ()        (x x x x x x x x))
;;;    1001        ((x) ()    ()        (x x x x x x x x))
;;;
;;;  The algorithm then merges the sublists of these lists into
;;;  one list, and returns that list.
;;;
;;;  To see the algorithm in action, trace LIST-MERGE!.
;;;

;;; Returns list L sorted using OBJ-< for comparisons.

(define (sort-list l obj-<)
  (cond ((or (null? l)
             (null? (cdr l)))
         l)
        (else
         (online-merge-sort! (append l '())    ; copy-list
			     obj-<))))

;;; Returns list L sorted using OBJ-< for comparisons.
;;; L is destructively altered.

(define (sort-list! l obj-<)
  (cond ((or (null? l)
             (null? (cdr l)))
         l)
        (else
         (online-merge-sort! l obj-<))))

;;; The real sort procedure.  Elements of L are added to B, a list of sorted
;;; lists as defined above.  When all elements of L have been added to B
;;; the sublists of B are merged together to get the desired sorted list.

(define (online-merge-sort! l obj-<)
  (let ((b (cons '() '())))
    (let loop ((l l))
      (cond ((null? l)
             (do ((c (cddr b) (cdr c))
                  (r (cadr b) (list-merge! (car c) r obj-<)))
                 ((null? c)
                  r)))
            (else
             (let ((new-l (cdr l)))
               (set-cdr! l '())
               (add-to-sorted-lists l b obj-<)
               (loop new-l)))))))

;;; X is a list that is merged into B, the list of sorted lists.

(define (add-to-sorted-lists x b obj-<)
  (let loop ((x x) (b b))
    (let ((l (cdr b)))
      (cond ((null? l)
             (set-cdr! b (cons x '())))
            ((null? (car l))
             (set-car! l x))
            (else
             (let ((y (list-merge! x (car l) obj-<)))
                (set-car! l '())
                (loop y l)))))))

;;; Does a stable side-effecting merge of L1 and L2.

(define (list-merge! l1 l2 obj-<)
  (cond ((null? l1) l2)
        ((null? l2) l1)
        ((obj-< (car l1) (car l2))
         (real-list-merge! l2 (cdr l1) obj-< l1)
         l1)
        (else
         (real-list-merge! l1 (cdr l2) obj-< l2)
         l2)))

;;; Does the real work of LIST-MERGE!.  L1 is assumed to be non-empty.

(define (real-list-merge! l1 l2 obj-< prev)
  (let loop ((a l1) (b l2) (prev prev))
    (cond ((null? b)
           (set-cdr! prev a))
          ((obj-< (car a) (car b))
           (set-cdr! prev a)
           (loop b (cdr a) a))
          (else
           (set-cdr! prev b)
           (loop a (cdr b) b)))))
