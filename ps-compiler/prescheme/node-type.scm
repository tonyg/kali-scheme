; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


; Types and nodes together

; Instantiate TYPE and replace the types in NODE with their corresponding
; value.  LOCATION is where NODE will be applied, and is used to get the actual
; types of the arguments.

(define (instantiate-type&value type node location)
  (let ((has (instantiate-type-scheme type
				      -1
				      (lambda () (fix-types node))))
	(wants (call->proc-type (node-parent location))))
    (identity (unify! has wants 'simplifying))))
;	  (format #t "~%Reconstructing ")
;	  (pp-cps call)
;	  (format #t " has   ~S~% wants ~S~%"
;		  (instantiate has)
;		  (instantiate wants))
;	  (breakpoint "reconstructing ~S" call)
;	  (unify! has wants 'simplifying)

; This is used to replace all references in NODE to polymorphic type variables
; with the current value of the type variable.
; Youch!  Very inefficient - may make many copies of the same type.

(define (fix-types node)
  (let label ((node node))
    (case (node-variant node)
      ((lambda)
       (for-each fix-variable (lambda-variables node))
       (label (lambda-body node)))
      ((call)
       (walk-vector label (call-args node)))
      ((literal)
       (let ((value (literal-value node)))
	 (if (or (uvar? value)
		 (other-type? value))
	     (set-literal-value! node (copy-type value))))))))

(define (fix-variable var)
  (set-variable-type! var (copy-type (variable-type var))))

(define (call->proc-type call)
  (let ((end (if (or (calls-this-primop? call 'call)
		     (calls-this-primop? call 'tail-call))
		 2        ; no protocol to ignore
		 3)))     ; protocol to ignore
    (make-arrow-type (do ((i (- (vector-length (call-args call)) 1) (- i 1))
			  (ts '() (cons (maybe-instantiate
					 (node-type (call-arg call i)))
					ts)))
			 ((< i end)
			  ts))
		     (let ((cont (call-arg call 0)))
		       (if (reference-node? cont)
			   (variable-type (reference-variable cont))
			   (make-tuple-type (map variable-type
						 (lambda-variables cont))))))))

(define (maybe-instantiate type)
  (if (type-scheme? type)
      (instantiate-type-scheme type -1)
      type))

(define (make-monomorphic! var)
  (let ((type (type-scheme-type (variable-type var))))
    (for-each (lambda (ref)
		(if (not (called-node? ref))
		    (error
		     "polymorphic procedure ~S used as value, cannot be made monomorphic"
		     (variable-name var))
		    (unify! type
			    (call->proc-type (node-parent ref))
			    'make-monomorphic!)))
	      (variable-refs var))
    (set-variable-type! var type)))