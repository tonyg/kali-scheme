; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.

; (cps-call <primop> <exits> <first-arg-index> <args> <cps>) ->
;                   <call-node> + <top-call-node> + <bottom-lambda-node>
;
; (cps-sequence <nodes> <cps>) -> <last-node> + <top-call> + <bottom-lambda>
;
; (<cps> <node>) -> <value-node> + <top-call-node> + <bottom-lambda-node>

(define (cps-call primop exits first-arg-index args cps)
  (let ((call (make-call-node primop
			      (+ (length args) first-arg-index)
			      exits))
	(arguments (make-arg-nodes args first-arg-index cps)))
    (let loop ((args arguments) (first #f) (last #f))
      (if (null? args)
	  (values call first last)
	  (let ((arg (car args)))
	    (attach call (arg-index arg) (arg-value arg))
	    (if (and last (arg-first arg))
		(attach-body last (arg-first arg)))
	    (loop (cdr args)
		  (or first (arg-first arg))
		  (or (arg-last arg) last)))))))
	    
; Record to hold information about arguments to calls.

(define-record-type arg :arg
  (make-arg index rank value first last)
  arg?
  (index arg-index)  ; The index of this argument in the call.
  (rank arg-rank)    ; The estimated cost of executing this node at run time.
  (value arg-value)  ; What CPS returned for this argument.
  (first arg-first)
  (last arg-last))

; Convert the elements of EXP into nodes (if they aren't already) and put
; them into an ARG record.  Returns the list of ARG records sorted
; by ARG-RANK.

(define (make-arg-nodes exp start cps)
  (do ((index start (+ index 1))
       (args exp (cdr args))
       (vals '() (cons (receive (value first last)
			   (cps (car args))
			 (make-arg index (node-rank first) value first last))
		       vals)))
      ((null? args)
       (sort-list vals
                  (lambda (v1 v2)
		    (> (arg-rank v1) (arg-rank v2)))))))

; Complexity analysis used to order argument evaluation.  More complex
; arguments are to be evaluated first.  This just counts reference nodes.
; It is almost certainly a waste of time.

(define (node-rank first)
  (if (not first)
      0
      (complexity-analyze-vector (call-args first))))

(define (complexity-analyze node)
  (cond ((empty? node)
         0)
        ((reference-node? node)
         1)
        ((lambda-node? node)
	 (if (not (empty? (lambda-body node)))
	     (complexity-analyze-vector (call-args (lambda-body node)))
	     0))
	((call-node? node)
	 (complexity-analyze-vector (call-args node)))
        (else
         0)))

(define (complexity-analyze-vector vec)
  (do ((i 0 (+ i 1))
       (q 0 (+ q (complexity-analyze (vector-ref vec i)))))
      ((>= i (vector-length vec))
       q)))

;----------------------------------------------------------------
; (cps-sequence <nodes> <values-cps>) ->
;   <last-node> + <top-call> + <bottom-lambda>
; <values-cps> is the same as the <cps> used above, except that it returns
; a list of value nodes instead of exactly one.

(define (cps-sequence nodes values-cps)
  (if (null? nodes)
      (bug "CPS: empty sequence"))
  (let loop ((nodes nodes) (first #f) (last #f))
    (if (null? (cdr nodes))
	(values (car nodes) first last)
	(receive (exp-first exp-last)
	    (cps-sequent (car nodes) values-cps)
	  (if (and last exp-first)
	      (attach-body last exp-first))
	  (loop (cdr nodes) (or first exp-first) (or exp-last last))))))

(define (cps-sequent node values-cps)	  
  (receive (vals exp-first exp-last)
      (values-cps node)
    (receive (calls other)
	(partition-list call-node? vals)
      (map erase other)
      (if (null? calls)
	  (values exp-first exp-last)
	  (insert-let calls exp-first exp-last)))))

(define (insert-let calls exp-first exp-last)
  (let* ((vars (map (lambda (call)
		      (make-variable 'v (trivial-call-return-type call)))
		    calls))
	 (cont (make-lambda-node 'c 'cont vars))
	 (call (make-call-node (get-primop (enum primop let))
			       (+ 1 (length calls))
			       1)))
    (attach-call-args call (cons cont calls))
    (cond (exp-first
	   (attach-body exp-last call)
	   (values exp-first cont))
	  (else
	   (values call cont)))))

