; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


; Determining if two nodes are functionally identical.

(define (node-equal? n1 n2)
  (if (call-node? n1)
      (and (call-node? n2)
           (call-node-eq? n1 n2))
      (value-node-eq? n1 n2)))

; Compare two call nodes.  The arguments to the nodes are compared
; starting from the back to do leaf nodes first (usually).

(define (call-node-eq? n1 n2)
  (and (= (call-arg-count n1) (call-arg-count n2))
       (= (call-exits     n1) (call-exits     n2))
       (eq? (call-primop n1) (call-primop n2))
       (let ((v1 (call-args n1))
             (v2 (call-args n2)))
         (let loop ((i (- (vector-length v1) '1)))
           (cond ((< i '0)
                  #t)
                 ((node-equal? (vector-ref v1 i) (vector-ref v2 i))
                  (loop (- i '1)))
                 (else
                  #f))))))

; Compare two value nodes.  Reference nodes are the same if they refer to the
; same variable or if they refer to corresponding variables in the two node
; trees.  Primop and literal nodes must be identical.  Lambda nodes are compared
; by their own procedure.

(define (value-node-eq? n1 n2)
  (cond ((neq? (node-variant n1) (node-variant n2))
	 #f)
	((reference-node? n1)
	 (let ((v1 (reference-variable n1))
	       (v2 (reference-variable n2)))
	   (or (eq? v1 v2) (eq? v1 (variable-flag v2)))))
	((literal-node? n1)
	 (and (eq? (literal-value n1) (literal-value n2))
	      (eq? (literal-type  n1) (literal-type  n2))))
	((lambda-node? n1)
	 (lambda-node-eq? n1 n2))))

; Lambda nodes are identical if they have identical variable lists and identical
; bodies.  The variables of N1 are stored in the flag fields of the variables of
; N2 for the use of VALUE-NODE-EQ?.

(define (lambda-node-eq? n1 n2)
  (let ((v1 (lambda-variables n1))
        (v2 (lambda-variables n2)))
    (let ((ok? (let loop ((v1 v1) (v2 v2))
                 (cond ((null? v1)
                        (if (null? v2)
                            (call-node-eq? (lambda-body n1) (lambda-body n2))
                            #f))
                       ((null? v2) #f)
                       ((variable-eq? (car v1) (car v2))
                        (loop (cdr v1) (cdr v2)))
                       (else #f)))))
      (map (lambda (v) (if v (set-variable-flag! v #f))) v2)
      ok?)))

(define (variable-eq? v1 v2)
  (cond ((not v1)
         (not v2))
        ((not v2) #f)
        ((eq? (variable-type v1) (variable-type v2))
         (set-variable-flag! v2 v1)
         #t)
        (else #f)))




