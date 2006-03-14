; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; --------------------
; Operators (= special operators and primitives)

(define-record-type operator :operator
  (make-operator type nargs uid name)
  operator?
  (type operator-type set-operator-type!)
  (nargs operator-nargs)
  (uid operator-uid)
  (name operator-name))

(define-record-discloser :operator
  (lambda (s)
    (list 'operator
	  (operator-name s)
	  (if (symbol? (operator-type s))
	      (operator-type s)
	      (type->sexp (operator-type s) #t)))))

(define usual-operator-type
  (procedure-type any-arguments-type value-type #f))

(define (get-operator name . type-option)
  (let ((type (if (null? type-option) #f (car type-option)))
	(probe (table-ref operators-table name)))
    (if (operator? probe)
	(let ((previous-type (operator-type probe)))
	  (cond ((not type))
		((not previous-type)
		 (set-operator-type! probe type))
		((symbol? type)		; 'leaf or 'internal
		 (if (not (eq? type previous-type))
		     (warn "operator type inconsistency" name type previous-type)))
		((subtype? type previous-type)  ;Improvement
		 (set-operator-type! probe type))
		((not (subtype? previous-type type))
		 (warn "operator type inconsistency"
		       name
		       (type->sexp previous-type 'foo)
		       (type->sexp type 'foo))))
	  probe)
	(let* ((uid *operator-uid*)
	       (op (make-operator type
				  (if (and type
					   (not (symbol? type))
					   (fixed-arity-procedure-type? type))
				      (procedure-type-arity type)
				      #f)
				  uid
				  name)))
	  (if (>= uid number-of-operators)
	      (warn "too many operators" (operator-name op) (operator-type op)))
	  (set! *operator-uid* (+ *operator-uid* 1))
	  (table-set! operators-table (operator-name op) op)
	  (vector-set! the-operators uid op)
	  op))))

(define *operator-uid* 0)

(define operators-table (make-table))

(define number-of-operators 400)  ;Fixed-size limits bad, but speed good
(define the-operators (make-vector number-of-operators #f))

; --------------------
; Operator tables (for fast dispatch)

(define (make-operator-table default)
  (make-vector number-of-operators default))

(define operator-table-ref vector-ref)

(define (operator-lookup table op)
  (operator-table-ref table (operator-uid op)))

(define (operator-define! table name type proc)
  (vector-set! table
	       (operator-uid (get-operator name type))
	       proc))

; --------------------
; Nodes

; A node is an annotated expression (or definition or other form).
; The FORM component of a node is an S-expression of the same form as
; the S-expression representation of the expression.  E.g. for
; literals, the form is the literal value; for variables the form is
; the variable name; for IF expressions the form is a 4-element list
; (ignored test con alt).  Nodes also have a tag identifying what kind
; of node it is (literal, variable, if, etc.) and a property list.

(define-record-type node :node
  (really-make-node uid form plist)
  node?
  (uid node-operator-id)
  (form node-form)
  (plist node-plist set-node-plist!))

(define-record-discloser :node
  (lambda (n) (list (operator-name (node-operator n)) (node-form n))))

(define (make-node operator form)
  (really-make-node (operator-uid operator) form '()))

(define (node-ref node key)
  (let ((probe (assq key (node-plist node))))
    (if probe (cdr probe) #f)))

(define (node-set! node key value) ;gross
  (if value
      (let ((probe (assq key (node-plist node))))
	(if probe
	    (set-cdr! probe value)
	    (set-node-plist! node (cons (cons key value) (node-plist node)))))
      (let loop ((l (node-plist node)) (prev #f))
	(cond ((null? l) 'lose)
	      ((eq? key (caar l))
	       (if prev
		   (set-cdr! prev (cdr l))
		   (set-node-plist! node (cdr l))))
	      (else (loop (cdr l) l))))))

(define (node-operator node)
  (vector-ref the-operators (node-operator-id node)))

(define (node-predicate name . type-option)
  (let ((id (operator-uid (apply get-operator name type-option))))
    (lambda (node)
      (= (node-operator-id node) id))))

(define (make-similar-node node form)
  (if (equal? form (node-form node))
      node
      (make-node (node-operator node) form)))

; Top-level nodes are often delayed.

(define (force-node node)
  (if (node? node)
      node
      (force node)))
