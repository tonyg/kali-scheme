; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

(define (node-type node)
  (reconstruct node 'fast any-values-type))

(define (reconstruct-type node env)
  (reconstruct node '() any-values-type))

(define (reconstruct node constrained want-type)
  ((operator-table-ref reconstructors (node-operator-id node))
    node
    constrained
    want-type))

(define (examine node constrained want-type)
  (if (pair? constrained)
      (reconstruct node constrained want-type)
      want-type))

(define reconstructors
  (make-operator-table (lambda (node constrained want-type)
                         (reconstruct-call (node-form node)
					   constrained
					   want-type))))

(define (define-reconstructor name type proc)
  (operator-define! reconstructors name type proc))

(define-reconstructor 'lambda syntax-type
  (lambda (node constrained want-type)
    (reconstruct-lambda node constrained want-type #f)))

(define-reconstructor 'flat-lambda syntax-type
  (lambda (node constrained want-type)
    (reconstruct-lambda node constrained want-type #f)))

(define (reconstruct-lambda node constrained want-type called?)
  (if (eq? constrained 'fast)
      any-procedure-type
      (let* ((form (node-form node))
	     (want-result (careful-codomain want-type))
	     (formals (cadr form))
	     (alist (map (lambda (node)
			   (cons node value-type))
			 (normalize-formals formals)))
	     (cod (reconstruct (last form)	; works for normal and flat
			       (if called?
				   (append alist constrained)
				   alist)
			       want-result)))
	(procedure-type (if (n-ary? formals)
			    any-values-type ;lose
			    (make-some-values-type (map cdr alist)))
			cod
			#t))))

(define (careful-codomain proc-type)
  (if (procedure-type? proc-type)
      (procedure-type-codomain proc-type)
      any-values-type))

(define-reconstructor 'name 'leaf
  (lambda (node constrained want-type)
    (if (eq? constrained 'fast)
        (reconstruct-name node)
        (let ((z (assq node constrained)))
          (if z
              (let ((type (meet-type (cdr z) want-type)))
                (begin (set-cdr! z type)
                       type))
              (reconstruct-name node))))))

(define (reconstruct-name node)
  (let ((probe (node-ref node 'binding)))
    (if (binding? probe)
        (let ((type (binding-type probe)))
          (cond ((variable-type? type)
		 (variable-value-type type))
                ((subtype? type value-type)
		 type)
                (else
		 value-type)))
        value-type)))

(define-reconstructor 'call 'internal
  (lambda (node constrained want-type)
    (let ((form (node-form node)))
      (cond ((proc->reconstructor (car form))
	     => (lambda (recon)
		  (recon (cdr form) constrained want-type)))
	    (else
	     (reconstruct-call form constrained want-type))))))

; See if PROC is a primop or a variable bound to a primop, and then return
; that primops reconstructor, if it has one.

(define (proc->reconstructor proc)      
  (cond ((name-node? proc)
	 (let ((probe (node-ref proc 'binding)))
	   (if (and probe
		    (binding? probe)
		    (primop? (binding-static probe)))
	       (table-ref primop-reconstructors
			  (binding-static probe))
	       #f)))
	((literal-node? proc)
	 (if (primop? (node-form proc))
	     (table-ref primop-reconstructors
			(node-form proc))
	     #f))
	(else #f)))

(define (reconstruct-call form constrained want-type)
  (let* ((want-op-type (procedure-type any-arguments-type
				       want-type
				       #f))
	 (op-type (if (lambda-node? (car form))
		      (reconstruct-lambda (car form)
					  constrained
					  want-op-type
					  #t)
		      (reconstruct (car form)
				   constrained
				   want-op-type)))
	 (args (cdr form))
	 (lose (lambda ()
		 (for-each (lambda (arg)
			     (examine arg constrained value-type))
			   args))))
    (if (procedure-type? op-type)
	(begin (if (restrictive? op-type)
		   (let loop ((args args)
			      (dom (procedure-type-domain op-type)))
		     (if (not (or (null? args)
				  (empty-rail-type? dom)))
			 (begin (examine (car args)
					 constrained
					 (head-type dom))
				(loop (cdr args) (tail-type dom)))))
		   (lose))
	       (procedure-type-codomain op-type))
	(begin (lose)
	       any-values-type))))

(define-reconstructor 'literal 'leaf
  (lambda (node constrained want-type)
    (constant-type (node-form node))))

(define-reconstructor 'quote syntax-type
  (lambda (node constrained want-type)
    (constant-type (cadr (node-form node)))))

(define-reconstructor 'unspecific #f
  (lambda (node constrained wnat-type)
    unspecific-type))

(define-reconstructor 'unassigned #f
  (lambda (node constrained wnat-type)
    unspecific-type))

(define-reconstructor 'if syntax-type
  (lambda (node constrained want-type)
    (let ((form (node-form node)))
      (examine (cadr form) constrained value-type)
      ;; Fork off two different constrain sets
      (let ((con-alist (fork-constraints constrained))
            (alt-alist (fork-constraints constrained)))
        (let ((con-type (reconstruct (caddr form) con-alist want-type))
              (alt-type (reconstruct (cadddr form) alt-alist want-type)))
          (if (pair? constrained)
              (for-each (lambda (c1 c2 c)
                          (set-cdr! c (join-type (cdr c1) (cdr c2))))
                        con-alist
                        alt-alist
                        constrained))
          (join-type con-type alt-type))))))

(define (fork-constraints constrained)
  (if (pair? constrained)
      (map (lambda (x) (cons (car x) (cdr x)))
           constrained)
      constrained))-

(define-reconstructor 'begin syntax-type
  (lambda (node constrained want-type)
    ;; This is unsound - there might be a throw out of some subform
    ;; other than the final one.
    (do ((forms (cdr (node-form node)) (cdr forms)))
        ((null? (cdr forms))
         (reconstruct (car forms) constrained want-type))
      (examine (car forms) constrained any-values-type))))

(define-reconstructor 'set! syntax-type
  (lambda (node constrained want-type)
    (examine (caddr (node-form node)) constrained value-type)
    unspecific-type))

(define-reconstructor 'letrec syntax-type
  (lambda (node constrained want-type)
    (let ((form (node-form node)))
      (if (eq? constrained 'fast)
          (reconstruct (caddr form) 'fast want-type)
          (let ((alist (map (lambda (spec)
			      (cons (car spec)
				    (reconstruct (cadr spec)
						 constrained
						 value-type)))
                            (cadr form))))
            (reconstruct (caddr form)
			 (append alist constrained)
                         want-type))))))

(define-reconstructor 'loophole syntax-type
  (lambda (node constrained want-type)
    (let ((args (cdr (node-form node))))
      (examine (cadr args) constrained any-values-type)
      (car args))))

(define (node->type node)
  (if (node? node)
      (let ((form (node-form node)))
        (if (pair? form)
            (map node->type form)
            (desyntaxify form)))
      (desyntaxify node)))

(define-reconstructor 'define syntax-type
  (lambda (node constrained want-type)
    ':definition))

(define-reconstructor 'lap syntax-type
  (lambda (node constrained want-type)
    any-procedure-type))

(define name-node?    (node-predicate 'name    'leaf))
(define lambda-node?  (node-predicate 'lambda  syntax-type))
(define literal-node? (node-predicate 'literal 'leaf))
; --------------------
; Primops.
;
; Most primops just have the types assigned in comp-prim.scm.

(define primop-reconstructors (make-symbol-table))

(define (define-primop-reconstructor name proc)
  (table-set! primop-reconstructors name proc))

(define-reconstructor 'primitive-procedure syntax-type
  (lambda (node constrained want-type)
    (primop-type (get-primop (cadr (node-form node))))))

(define-primop-reconstructor 'values
  (lambda (args constrained want-type)
    (make-some-values-type (map (lambda (node)
                                  (meet-type
                                   (reconstruct node constrained value-type)
                                   value-type))
				args))))

(define-primop-reconstructor 'call-with-values
  (lambda (args constrained want-type)
    (if (= (length args) 2)
	(let ((thunk-type (reconstruct (car args)
				       constrained
				       (procedure-type empty-rail-type
						       any-values-type
						       #f))))
	  (careful-codomain
	   (reconstruct (cadr args)
			constrained
			(procedure-type (careful-codomain thunk-type)
					any-values-type
					#f))))
	error-type)))

(define (reconstruct-apply args constrained want-type)
  (if (not (null? args))
      (let ((proc-type (reconstruct (car args)
				    constrained
				    any-procedure-type)))
	(for-each (lambda (arg) (examine arg constrained value-type))
		  (cdr args))
	(careful-codomain proc-type))
      error-type))

(define-primop-reconstructor 'apply reconstruct-apply)

(define-primop-reconstructor 'primitive-catch reconstruct-apply)

(define (constant-type x)
  (cond ((number? x)
         (meet-type (if (exact? x) exact-type inexact-type)
                    (cond ((integer? x) integer-type)
                          ((rational? x) rational-type)
                          ((real? x) real-type)
                          ((complex? x) complex-type)
                          (else number-type))))
        ((boolean? x) boolean-type)
        ((pair? x) pair-type)
        ((string? x) string-type)
        ((char? x) char-type)
        ((null? x) null-type)
        ((symbol? x) symbol-type)
        ((vector? x) vector-type)
        (else value-type)))

