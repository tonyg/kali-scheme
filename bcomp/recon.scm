; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Rudimentary type reconstruction, hardly worthy of the name.

; Currently, NODE-TYPE is called in two places.  One is to determine
; the type of the right-hand side of a DEFINE for a variable that is
; never assigned, so uses of the variable can be checked later.  The
; other is when compiling a call, to check types of arguments and
; produce warning messages.

; This is heuristic, to say the least.  It's not clear what the right
; interface or formalism is for Scheme; I'm still experimenting.

; Obviously we can't do Hindley-Milner inference.  Not only does
; Scheme have subtyping, but it also has dependent types up the wazoo.
; For example, the following is perfectly correct Scheme:
;
;   (define (foo x y) (if (even? x) (car y) (vector-ref y 3)))


(define (node-type node env)
  (reconstruct node env '()))

(define (reconstruct node env constrained)
  (if (node? node)
      ((operator-table-ref reconstructors (node-operator-id node))
       node env constrained)
      any-values-type))

(define reconstructors
  (make-operator-table (lambda (node env constrained)
			 (reconstruct-call node env constrained))))

(define (define-reconstructor name type proc)
  (operator-define! reconstructors (list name type) proc))

(define-reconstructor 'lambda syntax-type
  (lambda (node env constrained)
    (let* ((form (node-form node))
	   (formals (cadr form))
	   (vars (normalize-formals formals))
	   (var-nodes (map (lambda (formal)
			     (make-node operator/name formal))
			   vars))
	   (cod (reconstruct (caddr form)
			     (bind vars var-nodes env)
			     var-nodes)))
      (procedure-type (if (n-ary? formals)
			  any-values-type ;lose
			  (make-some-values-type
			       (map (lambda (node)
				      (or (node-ref node 'type-constraint)
					  value-type))
				    var-nodes)))
		      cod))))

(define operator/name (get-operator 'name))

(define-reconstructor 'name 'leaf
  (lambda (node env constrained)
    (let ((probe (node-ref node 'binding)))
      (if (binding? probe)
	  (binding-type probe)
	  (or (node-ref node 'type-constraint)
	      value-type)))))

(define (reconstruct-call node env constrained)
  (constrain-call! node any-values-type env constrained)
  (let ((op-type (reconstruct (car (node-form node)) env constrained)))
    (if (procedure-type? op-type)
	(procedure-type-codomain op-type)
	any-values-type)))


(define-reconstructor 'literal 'leaf
  (lambda (node env constrained)
    (constant-type (node-form node))))

(define-reconstructor 'quote syntax-type
  (lambda (node env constrained)
    (constant-type (cadr (node-form node)))))

(define-reconstructor 'if syntax-type
  (lambda (node env constrained)
    (let ((con-type (reconstruct (caddr (node-form node)) env '()))
	  (alt-type (reconstruct (cadddr (node-form node)) env '())))
      (if (equal? con-type alt-type)
	  con-type
	  ;; (if (and (some-values-type? con-type) ...) (meet ...))
	  any-values-type))))

(define-reconstructor 'begin syntax-type
  (lambda (node env constrained)
    (reconstruct (last (node-form node)) env constrained)))

(define-reconstructor 'set! syntax-type
  (lambda (node env constrained) unspecific-type))

(define-reconstructor 'letrec syntax-type
  (lambda (node env constrained)
    any-values-type
    ;; (reconstruct (last (node-form node)) (bind ... env) constrained)
    ))

(define-reconstructor 'primitive-procedure syntax-type
  (lambda (node env constrained)
    (operator-type (get-operator (cadr (node-form node))))))    ;mumble

(define-reconstructor 'loophole syntax-type
  (lambda (node env constrained)
    (node->type (cadr (node-form node)))))

(define (node->type node)
  (if (node? node)
      (let ((form (node-form node)))
	(if (pair? form)
	    (map node->type form)
	    (desyntaxify form)))
      (desyntaxify node)))

(define-reconstructor 'define syntax-type
  (lambda (node env constrained)
    'definition))

(define-reconstructor 'define-syntax syntax-type
  (lambda (node env constrained)
    'definition))

; Upward constraint propagation.  This is what lets us conclude, e.g.,
; that (lambda (x) (car x)) has type (proc (pair) value).

(define (constrain-call! node want-type env constrained)
  (if (pair? constrained)  ;+++
      (let ((exp (node-form node)))
	(constrain! (car exp)
		    (procedure-type (make-some-values-type
				     (map (lambda (arg) value-type)
					  (cdr exp)))
				    want-type)
		    env
		    constrained)
	(let ((op-type (reconstruct (car exp) env constrained)))
	  (if (fixed-arity-procedure-type? op-type)
	      (for-each (lambda (want-type arg)
			  (constrain! arg want-type env constrained))
			(procedure-type-argument-types op-type)
			(cdr exp)))))))

; Weak type requirements: e.g. we don't look inside of IF's.

(define (constrain! arg want-type env constrained)
  (cond ((not (node? arg)))
	((name-node? arg)
	 (let ((binding (lookup env (node-form arg))))
	   (if (node? binding)
	       (if (memq binding constrained)
		   (let ((have-type (node-ref binding 'type-constraint)))
		     (if have-type
			 (if (compatible-types? want-type have-type)
			     (if (eq? (compatible-types? have-type want-type)
				      'maybe)
				 (node-set! binding 'type-constraint
					    want-type)))
			 (node-set! binding 'type-constraint want-type)))))))
	((call-node? arg)
	 (constrain-call! arg want-type env constrained))
	((begin-node? arg)
	 ;; This is unsound if there can be a throw out of some subform
	 ;; other than the final one.
	 (do ((exps (cdr (node-form arg)) (cdr exps)))
	     ((null? (cdr exps))
	      (constrain! (car exps) want-type env constrained))
	   (constrain! (car exps) any-values-type env constrained)))))


(define call-node? (node-predicate 'call))
(define name-node? (node-predicate 'name))
(define begin-node? (node-predicate 'begin))





; --------------------
; Primitive procedures:

(define-reconstructor 'values any-procedure-type
  (lambda (node env constrained)
    (make-some-values-type (map (lambda (node)
				  (reconstruct node env constrained))
				(cdr (node-form node))))))

(define-reconstructor 'call-with-values
		      (proc ((proc () any-values-type)
			     any-procedure-type)
			    any-values-type)
  (lambda (node env constrained)
    (let ((proc-type (reconstruct (caddr (node-form node)) env constrained)))
      (if (procedure-type? proc-type)
	  (procedure-type-codomain proc-type)
	  any-values-type))))

(define (reconstruct-apply node env constrained)
  (let ((proc-type (reconstruct (cadr (node-form node)) env constrained)))
    (if (procedure-type? proc-type)
	(procedure-type-codomain proc-type)
	any-values-type)))

(define-reconstructor 'apply any-procedure-type
  reconstruct-apply)

(define zero-type 'zero)       ;Hmm.
(define escape-type 'escape)   ;Hmm.  See whether any errors crop up.

(define-reconstructor 'primitive-catch
		      (proc ((proc (escape-type) any-values-type))
			    any-values-type)
  reconstruct-apply)


; --------------------
; Types of simple primitives.

(define (declare-operator-type ops type)
  (if (list? ops)
      (for-each (lambda (op) (get-operator op type))
		ops)
      (get-operator ops type)))

(declare-operator-type 'with-continuation
		       (proc (escape-type (proc () any-values-type))
			     zero-type))
 
(declare-operator-type 'eq?
		       (proc (value-type value-type) boolean-type))

(declare-operator-type '(number? integer? rational? real? complex? exact?
				 char? eof-object?)
		       (proc (value-type) boolean-type))

(declare-operator-type '(exact->inexact
			 inexact->exact
			 floor numerator denominator
			 real-part imag-part
			 exp log sin cos tan asin acos atan sqrt
			 angle magnitude
			 bitwise-not)
		       (proc (number-type) number-type))

(declare-operator-type '(+ * - /)
		       (procedure-type any-values-type number-type))

(declare-operator-type '(= <)
		       (procedure-type any-values-type boolean-type))

(declare-operator-type '(quotient
			 remainder
			 make-polar make-rectangular
			 bitwise-and bitwise-ior bitwise-xor
			 arithmetic-shift)
		       (proc (number-type number-type) number-type))

(declare-operator-type '(char=? char<?)
		       (proc (char-type char-type) boolean-type))

(declare-operator-type 'char->ascii
		       (proc (char-type) number-type))

(declare-operator-type 'ascii->char
		       (proc (number-type) char-type))


(define (last x)
  (if (null? (cdr x))
      (car x)
      (last (cdr x))))


(define (constant-type x)
  (cond ((number? x) number-type)
	((boolean? x) boolean-type)
	((pair? x) pair-type)
	((string? x) 'string)
	((char? x) char-type)
	((null? x) 'null)
	((symbol? x) 'symbol)
	((vector? x) 'vector)
	(else value-type)))
