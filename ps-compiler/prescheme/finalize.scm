; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


; 1) Find all of the uvars that have no binding.
; 2) Flow types forwards from known sources.
; 3) For uvars still without types, flow backward from known sinks.
; 4) Flow both ways to the remaining uvars (we have no other data for them).
; 4) Anything left is completely unconnected, so set it to type/null.
; 5) Add any necessary coercions.

(define (add-type-coercions reducer)
  (let ((uvars (filter (lambda (uvar)
			 (not (uvar-binding uvar)))
		       (constrained-uvars))))
    (let loop ((uvars uvars) (count (length uvars)))
      (let* ((uvars (flow-sinks-backwards (flow-sources-forwards uvars)))
	     (new-count (length uvars)))
	(if (< new-count count)
	    (loop uvars new-count)
	    (for-each (lambda (uvar)
			(set-uvar-binding! uvar type/null))
		      uvars)))))
  (for-each (lambda (uvar)
	      (if (not (uvar? (uvar-binding uvar)))
		  (add-coercions uvar reducer)))
	    (constrained-uvars)))

(define (add-coercions uvar defns)
  (let ((type (uvar-binding uvar)))
    (for-each (lambda (relation)
		(let ((wants-type (maybe-follow-uvar (relation-sink relation))))
		  (if (not (eq? type wants-type))
		      (insert-coercion (relation-site relation)
				       type
				       wants-type
				       defns))))
	      (uvar-sinks uvar))
    (for-each (lambda (relation)
		(let ((source (relation-source relation)))
		  (if (and (not (uvar? source))
			   (not (eq? type source)))
		      (insert-coercion (relation-site relation)
				       source
				       type
				       defns))))
	      (uvar-sources uvar))))

(define (insert-coercion site from to defns)
  (receive (ref set!)
      (cond ((pair? site) (values car set-car!))
	    ; ((form? site) (values form-value set-form-value!))
	    (else
	     (error "coercion has a funny site ~S" site)))
    (let ((exp (ref site)))
      ; (format #t " coercing ~S from ~S to ~S~%" exp from to)
      (cond ((type> from to)
	     (format #t "Warning: losing coercion ~S from ~S to ~S~%" exp from to)
	     (receive (form-id node)
		 (find-pair site defns)
	       (if form-id
		   (begin
		     (format #t " in ~S~% in definition of ~S~%"
			     (schemify node)
			     form-id))))
;	     (breakpoint "Losing coercion")
	     (values))
	    ((or (quote-node? exp)
		 (literal-node? exp))
	     (node-set! exp 'type to))
	    (else
	     (set! site (make-coercion from to exp)))))))

(define (make-coercion from to exp)
  (cond ((eq? to type/float64)
	 (really-make-coercion 'int->float
			       (if (eq? from type/int32)
				   exp
				   (make-coercion from type/int32 exp))))
	((eq? to type/int32)
	 (cond ((eq? from type/int8)
		(really-make-coercion 'sign-extend exp))
	       ((or (eq? from type/int8u)
		    (eq? from type/int7u))
		(really-make-coercion 'zero-extend exp))
	       (else
		(bug "don't now how to convert from ~S to ~S" from to))))
	((and (or (eq? to type/int8u)
		  (eq? to type/int8))
	      (eq? from type/int7u))
	 exp)
	(else
	 (bug "don't now how to convert from ~S to ~S" from to))))

(define (really-make-coercion primop-id exp)
  (make-node op/call (list (make-literal-node
			    (get-prescheme-primop primop-id))
			   exp)))

(define quote-node? (node-predicate 'quote))
(define literal-node? (node-predicate 'literal))

(define op/literal (get-operator 'literal))
(define op/call (get-operator 'call))

(define (make-literal-exp value)
  (make-node op/literal value))

;----------------------------------------------------------------

(define (flow-sources-forwards uvars)
  (for-each (lambda (uvar)
	      (let ((type (reduce (uvar-sources uvar)
				  (lambda (relation type)
				    (let ((other (maybe-follow-uvar
						  (relation-source relation))))
				      (cond ((uvar? other)
					     type)
					    ((not type)
					     other)
					    (else
					     (type-max type other)))))
				  (if (uvar-arith-op? uvar)
				      type/int32 ; minimum arithmetic type
				      #f))))
		(set-uvar-temp! uvar (if type (list type) '()))))
	    uvars)
  (receive (ref set!)
      (make-base-type-table)
    (transitive uvars
		(lambda (uvar)
		  (filter-map (lambda (relation)
				(let ((other (maybe-follow-uvar
					      (relation-source relation))))
				  (if (uvar? other)
				      other
				      #f)))
			      (uvar-sources uvar)))
		#f
		uvar-temp
		set-uvar-temp!
		#f
		ref
		set!))
  (filter (lambda (uvar)
	    (let ((types (uvar-temp uvar)))
	      (cond ((null? types)
		     (set-uvar-binding! uvar #f)
		     #t)
		    (else
		     (set-uvar-binding! uvar
					(reduce (cdr types) type-max (car types)))
		     #f))))
	  uvars))

;----------------------------------------------------------------
; Find the smallest type that the uvar's sinks want.

(define (flow-sinks-backwards uvars)
  (for-each (lambda (uvar)
	      (set-uvar-binding! uvar (uvar-marker-maker)))
	    uvars)
  (for-each (lambda (uvar)
	      (let loop ((rels (uvar-sinks uvar)) (type #f) (users '()))
		(if (null? rels)
		    (begin
		      (set-uvar-temp! uvar (if type (list type) '()))
		      (set-uvar-users! uvar users))
		    (let ((other (relation-sink (car rels))))
		      (if (and (uvar? other)
			       (uvar-marker? (uvar-binding other)))
			  (loop (cdr rels) type (cons other users))
			  (loop (cdr rels)
				(type-min type (maybe-follow-uvar other))
				users))))))
	    uvars)
  (receive (ref set!)
      (make-base-type-table)
    (transitive uvars
		uvar-users
		#f
		uvar-temp
		set-uvar-temp!
		#f
		ref
		set!))
  (filter (lambda (uvar)
	    (let ((types (uvar-temp uvar)))
	      (cond ((null? types)
		     (set-uvar-binding! uvar #f)
		     #t)
		    (else
		     (set-uvar-binding! uvar
					(reduce (cdr types) type-min (car types)))
		     #f))))
	  uvars))

(define-subrecord uvar uvar-marker uvar-binding
  ()
  (users))

(define (type-min t1 t2)
  (if (type> t1 t2) t2 t1))

(define (type-max t1 t2)
  (if (type> t2 t1) t2 t1))

(define (reduce l f a)
  (if (null? l)
      a
      (reduce (cdr l) f (f (car l) a))))

;----------------------------------------------------------------

(define (find-pair pair reducer)
  (call-with-current-continuation
    (lambda (exit)
      (reducer (lambda (id node ignore)
		 (let recur ((node node))
		   (if (node? node)
		       (let ((form (node-form node)))
			 (if (pair? form)
			     (do ((form form (cdr form)))
				 ((not (pair? form)))
			       (if (eq? pair form)
				   (exit id node))
			       (recur (car form)))
			     (recur form))))))
	       #f)
      (values #f #f))))