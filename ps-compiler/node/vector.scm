; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


;----------------------------------------------------------------------------
; STORING NODE TREES IN VECTORS
;----------------------------------------------------------------------------

; The use of OTHER and GLOBAL depends on whether NODE->VECTOR or VECTOR->NODE

(define-record-type vec
 (vector    ; an expanding vector (NODE->VECTOR) or just a vector (VECTOR->NODE)
  (index)   ; the index of the next empty slot or the next thing to read
  locals    ; vector of local variables (VECTOR->NODE only)
  )
 ())

(define make-vec vec-maker)

; Add value as the next thing in the VEC.

(define (add-datum vec value)
  (xvector-set! (vec-vector vec) (vec-index vec) value)
  (set-vec-index! vec (+ 1 (vec-index vec))))

;   Convert a node into a vector
;
; literal       => QUOTE <literal> <rep>
; reference     => <index of the variable's name in vector> if lexical, or
;                  GLOBAL <variable> if it isn't
; lambda        => LAMBDA <stuff> #vars <variable names+reps> <call>
; call          => CALL <source> <primop> <exits> <number of args> <args>

; Preserve the node as a vector.

(define (node->vector node)
  (let ((vec (make-vec (make-xvector #f) 0 #f)))
    (real-node->vector node vec)
    (xvector->vector (vec-vector vec))))
  
; The main dispatch

(define (real-node->vector node vec)
  (case (node-variant node)
    ((literal)
     (literal->vector node vec))
    ((reference)
     (reference->vector node vec))
    ((lambda)
     (lambda->vector node vec))
    ((call)
     (add-datum vec 'call)
     (call->vector node vec))
    (else
     (bug "node->vector got funny node ~S" node))))

; VARIABLE-FLAGs are used to mark variables with their position in the
; vector.

(define (lambda->vector node vec)
  (add-datum vec 'lambda)
  (add-datum vec (lambda-name node))
  (add-datum vec (lambda-type node))
  (add-datum vec (lambda-protocol node))
  (add-datum vec (lambda-source node))
  (add-datum vec (lambda-variable-count node))
  (for-each (lambda (var)
	      (cond ((not var)
		     (add-datum vec #f))
		    (else
		     (set-variable-flag! var (vec-index vec))
		     (add-datum vec (variable-name var))
		     (add-datum vec (variable-type var)))))
	    (lambda-variables node))
  (call->vector (lambda-body node) vec)
  (for-each (lambda (var)
	      (if var
		  (set-variable-flag! var #f)))
	    (lambda-variables node)))

; If VAR is bound locally, then put the index of the variable within the vector
; into the vector.

(define (reference->vector node vec)
  (let ((var (reference-variable node)))
    (cond ((not (variable-binder var))
           (add-datum vec 'global)
           (add-datum vec var))
          ((integer? (variable-flag var))
           (add-datum vec (variable-flag var)))
          (else
           (bug "variable ~S has no vector location" var)))))

(define (literal->vector node vec)
  (let ((value (literal-value node)))
    (add-datum vec 'quote)
    (add-datum vec (literal-value node))
    (add-datum vec (literal-type node))))

; This counts down so that the continuation will be done after the arguments.
; Why does this matter?

(define (call->vector node vec)
  (let* ((args (call-args node))
         (len (vector-length args)))
    (add-datum vec (call-source node))
    (add-datum vec (call-primop node))
    (add-datum vec (call-exits node))
    (add-datum vec len)
    (do ((i (- len 1) (- i 1)))
        ((< i 0))
      (real-node->vector (vector-ref args i) vec))))

;----------------------------------------------------------------------------
; TURNING VECTORS BACK INTO NODES
;----------------------------------------------------------------------------

(define (vector->node vector)
  (if (not (vector? vector))
      (bug "VECTOR->NODE got funny value ~S~%" vector)
      (let ((vec (make-vec vector -1 (make-vector (vector-length vector)))))
	(real-vector->node vec))))

(define (vector->leaf-node vector)
  (case (vector-ref vector 0)
    ((quote global)
     (vector->node vector))
    (else #f)))

; Pop the next thing off of the vector (which is really a (<vector> . <index>)
; pair).

(define (get-datum vec)
  (let ((i (+ (vec-index vec) 1)))
    (set-vec-index! vec i)
    (vector-ref (vec-vector vec) i)))

; This prevents the (unecessary) resimplification of recreated nodes.

(define (real-vector->node vec)
  (let ((node (totally-real-vector->node vec)))
    (set-node-simplified?! node #t)
    node))

; Dispatch on the next thing in VEC.

(define (totally-real-vector->node vec)
  (let ((exp (get-datum vec)))
    (cond ((integer? exp)
           (make-reference-node (vector-ref (vec-locals vec) exp)))
          (else
           (case exp
             ((lambda)
              (vector->lambda-node vec))
             ((quote)
              (let* ((value (get-datum vec))
                     (rep   (get-datum vec)))
                (make-literal-node value rep)))
             ((global)
	      (make-reference-node (get-datum vec)))
	     ((call)
	      (vector->call-node vec))
	     ((import)  ; global variable from a separate compilation
	      (make-reference-node (lookup-imported-variable (get-datum vec))))
             (else
              (no-op
               (bug '"real-vector->node got an unknown code ~S" exp))))))))

(define (vector->lambda-node vec)
  (let* ((name     (get-datum vec))
         (type     (get-datum vec))
	 (protocol (get-datum vec))
	 (source   (get-datum vec))
         (count    (get-datum vec))
         (vars (do ((i 0 (+ i 1))
                    (v '() (cons (vector->variable vec) v)))
                   ((>= i count) v)))
         (node (make-lambda-node name type (reverse! vars))))
    (set-lambda-protocol! node protocol)
    (set-lambda-source! node source)
    (attach-body node (vector->call-node vec))
    (set-node-simplified?! (lambda-body node) #t)
    node))

; Replace a variable name with a new variable.

(define (vector->variable vec)
  (let ((name (get-datum vec)))
    (if name
        (let ((var (make-variable name (get-datum vec))))
          (vector-set! (vec-locals vec) (+ -1 (vec-index vec)) var)
          var)
        #f)))

(define (vector->call-node vec)
  (let* ((source (get-datum vec))
	 (primop (let ((p (get-datum vec)))
		   (if (primop? p)
		       p
		       (lookup-primop p))))
	 (exits  (get-datum vec))
         (count  (get-datum vec))
         (node (make-call-node primop count exits)))
    (do ((i (- count 1) (- i 1)))
        ((< i 0))
      (attach node i (real-vector->node vec)))
    (set-call-source! node source)
    node))





