; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

(define (simplify-letrec1 call)
  (let* ((cont (call-arg call 0))
	 (next (lambda-body cont))
	 (var (car (lambda-variables cont))))
    (if (not (and (calls-this-primop? next 'letrec2)
		  (= 1 (length (variable-refs var)))
		  (eq? next (node-parent (car (variable-refs var))))
		  (= 1 (node-index (car (variable-refs var))))))
	(error "badly formed LETREC ~S ~S" call (node-parent call)))
    (simplify-args call 0)
    (check-letrec-scoping call cont next)
    (if (every? unused? (cdr (lambda-variables cont)))
	(replace-body call (detach-body (lambda-body (call-arg next 0)))))))

(define (check-letrec-scoping letrec1 binder letrec2)
  (let ((values (sub-vector->list (call-args letrec2) 2))
	(body (call-arg letrec2 0)))
    (for-each (lambda (n) (set-node-flag! n 'okay)) values)
    (set-node-flag! body 'okay)
    (for-each (lambda (var)
		(for-each (lambda (ref)
			    (set-node-flag! (marked-ancestor ref) 'lose))
			  (variable-refs var)))
	      (cdr (lambda-variables binder)))
    (let ((non-recur (filter (lambda (p)
			       (eq? (node-flag (car p)) 'okay))
			     (map cons values (cdr (lambda-variables binder))))))
      (for-each (lambda (n) (set-node-flag! n #f)) values)
      (set-node-flag! body #f)
      (if (not (null? non-recur))
	  (letrec->let (map car non-recur)
		       (map cdr non-recur)
		       letrec1 binder letrec2)))))

(define (letrec->let vals vars letrec1 binder letrec2)
  (for-each detach vals)
  (remove-null-arguments letrec2
			 (- (vector-length (call-args letrec2))
			    (length vals)))
  (set-lambda-variables!
   binder
   (filter (lambda (v) (not (memq v vars)))
	   (lambda-variables binder)))
  (move-body letrec1
	     (lambda (letrec1)
	       (let-nodes ((call (let 1 l1 . vals))
			   (l1 vars letrec1))
		 call))))

; (return (lambda (a) ...) x)
;  =>
; (let (lambda (a) ...) x)

(define (simplify-ps-return call)
  (let ((cont (call-arg call 0))
	(value (call-arg call 1)))
    (cond ((not (lambda-node? cont))
	   (default-simplifier call))
	  (else
	   (set-call-primop! call (get-primop (enum primop let)))
	   (set-call-exits! call 1)
	   (set-node-simplified?! call #f)))))

(make-primop 'dispatch         #f #f default-simplifier (lambda (call) 1) #f)
(make-primop 'let              #f #f simplify-let       (lambda (call) 1) #f)
(make-primop 'letrec1          #f #f (lambda (call)
				       (simplify-letrec1 call))  (lambda (call) 1) #f)
(make-primop 'letrec2          #f #f default-simplifier (lambda (call) 1) #f)
(make-primop 'undefined-value  #t #f default-simplifier
	     (lambda (call) 1)
	     (lambda (call) type/null))
(make-primop 'undefined-effect #t #f default-simplifier
	     (lambda (call) 1)
	     (lambda (call) type/null))
(make-primop 'global-ref       #t 'read default-simplifier
	     (lambda (call) 1)
	     (lambda (call)
	       (variable-type (reference-variable (call-arg call 0)))))

;(make-primop 'allocate         #f #f 'allocate simplify-allocation (lambda (call) 1))

(make-primop 'global-set!      #f 'write default-simplifier
	     (lambda (call) 1) #f)

(make-proc-primop 'call 'write simplify-known-call
		  (lambda (call) 1) 1)
(make-proc-primop 'tail-call 'write simplify-known-tail-call
		  (lambda (call) 1) 1)
(make-proc-primop 'return #f   simplify-ps-return  (lambda (call) 1) 0)
(make-proc-primop 'jump   #f   simplify-jump       (lambda (call) 1) 0)
(make-proc-primop 'throw  #f   default-simplifier  (lambda (call) 1) 0)

; This delays simplifying the arguments until we see if the procedure
; is a lambda-node.

(define (simplify-unknown-call call)
  (simplify-arg call 1)   ; simplify the procedure
  (let ((proc (call-arg call 1)))
    (cond ((lambda-node? proc)
	   (determine-lambda-protocol proc (list proc))
	   (mark-changed proc))
	  ((and (reference-node? proc)
		(variable-simplifier (reference-variable proc)))
           => (lambda (proc)
		(proc call)))
	  (else
	   (simplify-args call 0)))))  ; simplify all arguments

(make-proc-primop 'unknown-call 'write simplify-unknown-call
		  (lambda (call) 1) 1)
(make-proc-primop 'unknown-tail-call 'write simplify-unknown-call
		  (lambda (call) 1) 1)
(make-proc-primop 'unknown-return #f default-simplifier
		  (lambda (call) 1) 0)

(define (simplify-unspecific call)
  (let ((node (make-undefined-literal)))
    (set-literal-type! node type/null)
    (replace call node)))

(define-scheme-primop unspecific #f type/null simplify-unspecific)

(define-scheme-primop uninitialized-value type/null)
(define-scheme-primop null-pointer?       type/boolean)
(define-scheme-primop null-pointer        type/boolean) ; type can't be right

(define-scheme-primop eq? type/boolean)  ; should have a simplifier

;(define (exp->type exp)
;  (if (quote-exp? exp)
;      (real-exp->type (quote-exp-value exp))
;      (error "can't turn ~S into a type" exp)))
;
;(define (real-exp->type exp)
;  (let ((lose (lambda () (error "can't turn ~S into a type" exp))))
;    (let label ((exp exp))
;      (cond ((pair? exp)
;             (case (car exp)
;               ((pointer)
;                (make-pointer-type (label (cadr exp))))
;               ((arrow)
;                (make-arrow-type (map label (cadr exp)) (caddr exp)))
;               (else
;                (lose))))
;            ((and (symbol? exp)
;                  (lookup-type exp))
;             => identity)
;            (else
;             (lose))))))

(define-scheme-cond-primop test simplify-test expand-test simplify-test?)

;(define-primitive-expander 'unspecific 0
;  (lambda (source args cenv)
;    (make-quote-exp the-undefined-value type/unknown)))


