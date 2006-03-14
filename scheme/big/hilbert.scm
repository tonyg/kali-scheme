; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Hilbert vectors are like vectors that grow as large as they need to.
; That is, they can be indexed by arbitrarily large nonnegative integers.

; The implementation allows for arbitrarily large gaps by arranging
; the entries in a tree.

; So-called because they live in an infinite-dimensional vector
; space...

(define hilbert-log 8)
(define hilbert-node-size (arithmetic-shift 1 hilbert-log))
(define hilbert-mask (- hilbert-node-size 1))
(define minus-hilbert-log (- 0 hilbert-log))

(define-record-type hilbert :hilbert
  (make-hilbert height root)
  (height hilbert-height set-hilbert-height!)
  (root hilbert-root set-hilbert-root!))

(define-record-discloser :hilbert
  (lambda (h)
    '(sparse-vector)))

(define (make-sparse-vector)
  (make-hilbert 1 (make-vector hilbert-node-size #f)))


(define (sparse-vector-ref hilbert index)
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
	       
(define (sparse-vector-set! hilbert index value)
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
	(vector-set! new-root 0 (hilbert-root hilbert))
	(set-hilbert-root! hilbert new-root)
	(set-hilbert-height! hilbert (+ (hilbert-height hilbert) 1))
	(let ((index (arithmetic-shift index minus-hilbert-log)))
	  (make-node-if-necessary (make-higher-if-necessary hilbert index)
				  (bitwise-and index hilbert-mask))))))

(define (make-node-if-necessary node index)
  (or (vector-ref node index)
      (let ((new (make-vector hilbert-node-size #f)))
	(vector-set! node index new)
	new)))

; For debugging

(define (sparse-vector->list h)
  (let recur ((node (hilbert-root h))
              (height (hilbert-height h))
              (more '()))
    (if (= height 0)
        (if (or node (pair? more))
            (cons node more)
            '())
        (do ((i (- hilbert-node-size 1) (- i 1))
             (more more (recur (if node
                                   (vector-ref node i)
                                   #f)
                               (- height 1) more)))
            ((< i 0) more)))))
