; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

;Date: Thu, 4 Nov 93 14:34:04 EST
;Subject: binary search trees
;From: kelsey@research.nj.nec.com
;
;If you want to add the Hilbert tables I think you should change
;the name and add some documentation.  Neither the name nor the
;only comment in the file are particulary informative.  They are
;not infinite dimensional vectors, just arbitrarily large one
;dimensional ones.
;
;How about make-big-vector etc.?


; Hilbert vectors are like vectors that grow as large as they need to.
; That is, they can be indexed by arbitrarily large nonnegative integers.

; The implementation allows for arbitrarily large gaps by arranging
; the entries in a tree.

; So-called because they live in an infinite-dimensional vector
; space...

; ,open bitwise define-record-types


(define hilbert-log 8)
(define hilbert-node-size (arithmetic-shift 1 hilbert-log))
(define hilbert-mask (- hilbert-node-size 1))
(define minus-hilbert-log (- 0 hilbert-log))



(define-record-type hilbert :hilbert
  (really-make-hilbert height root)
  (height hilbert-height set-hilbert-height!)
  (root hilbert-root set-hilbert-root!))

(define (make-hilbert)
  (really-make-hilbert 1 (make-vector hilbert-node-size #f)))


(define (hilbert-ref hilbert index)
  (let recur ((height (hilbert-height hilbert))
	      (index index))
    (if (= height 1)
	(let ((root (hilbert-root hilbert)))
	  (if (< index (vector-length root))
	      (vector-ref root index)
	      #f))
	(let ((node (recur (- height 1)
			   (arithmetic-shift index minus-hilbert-log))))
	  (if node
	      (vector-ref node (bitwise-and index hilbert-mask))
	      #f)))))
	       
(define (hilbert-set! hilbert index value)
  (vector-set!
   (let recur ((height (hilbert-height hilbert))
	       (index index))
     (if (= height 1)
	 (make-higher-if-necessary hilbert index)
	 (let ((index (arithmetic-shift index minus-hilbert-log)))
	   (make-node-if-necessary
	    (recur (- height 1) index)
	    (bitwise-and index hilbert-mask)))))
   (bitwise-and index hilbert-mask)
   value))

(define (make-higher-if-necessary hilbert index)
  (if (< index hilbert-node-size)
      (hilbert-root hilbert)
      (let ((new-root (make-vector hilbert-node-size #f)))
	(write `(higher ,index)) (newline)
	(vector-set! new-root 0 (hilbert-root hilbert))
	(set-hilbert-root! hilbert new-root)
	(set-hilbert-height! hilbert (+ (hilbert-height hilbert) 1))
	(let ((index (arithmetic-shift index minus-hilbert-log)))
	  (make-node-if-necessary (make-higher-if-necessary hilbert index)
				  (bitwise-and index hilbert-mask))))))

(define (make-node-if-necessary node index)
  (or (vector-ref node index)
      (let ((new (make-vector hilbert-node-size #f)))
	;; (write `(wider ,index)) (newline)
	(vector-set! node index new)
	new)))




; For debugging
;(define (hilbert->list h)
;  (let recur ((node (hilbert-root h))
;              (height (hilbert-height h))
;              (more '()))
;    (if (= height 0)
;        (if (or node (pair? more))
;            (cons node more)
;            '())
;        (do ((i (- hilbert-node-size 1) (- i 1))
;             (more more (recur (if node
;                                   (vector-ref node i)
;                                   #f)
;                               (- height 1) more)))
;            ((< i 0) more)))))
