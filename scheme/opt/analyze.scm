; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Simple code analysis to determine whether it's a good idea to
; in-line calls to a given procedure.

; Hook into the byte code compiler.

(set-optimizer! 'auto-integrate
  (lambda (forms package)
    (let ((out (current-noise-port)))
      (newline out)
      (display "Analyzing... " out) (force-output out)
      (let* ((forms (find-usages (map force-node forms) package))
	     (names (analyze-forms forms package)))
	(cond ((not (null? names))
	       (newline out)
	       (display "Calls will be compiled in line: " out)
	       (write (reverse names) out))
	      (else
	       (display "no in-line procedures" out)))
	(newline out)
	forms))))

(define (analyze-forms scanned-nodes package)
  (let ((inlines '()))
    (for-each (lambda (node)
		(let ((lhs (analyze-form node package)))
		  (if lhs
		      (set! inlines (cons lhs inlines)))))
	      scanned-nodes)
    inlines))

(define (analyze-form node package)	;Return LHS iff calls will be inlined.
  (if (define-node? node)
      (let ((form (node-form node)))
	(let ((lhs (node-form (cadr form)))
	      (rhs (caddr form)))
	  (let ((type (package-lookup-type package lhs)))
	    (if (variable-type? type)
		(require "not assigned" lhs #f)
		(let ((method (inlinable-rhs? rhs type package lhs)))
		  (if method
		      (begin (package-add-static! package lhs method)
			     (if (transform? method)
				 lhs
				 #f))
		      #f))))))
      #f))

(define (inlinable-rhs? node type package lhs)
  (cond ((lambda-node? node)
	 (if (simple-lambda? node lhs package)
	     (make-inline-transform node type package lhs)
	     #f))
	((name-node? node)
	 (let ((name (node-form node)))
	   (if (and (require "symbol rhs" (list lhs name)
		      (symbol? name))
		    (require "rhs bound" (list lhs name)
		      (binding? (package-lookup-type package name)))
		    (require "rhs unassigned" (list lhs name)
		      (not (variable-type? (package-lookup-type package name))))
		    (require "definitely procedure" (list lhs name)
		      (procedure-type? (package-lookup-type package name))))
	       (make-inline-transform node type package lhs)
	       #f)))
	((loophole-node? node)
	 (inlinable-rhs? (caddr (node-form node)) type package lhs))
;These should already be taken care of.
;	((primitive-procedure-node? node)
;	 (get-operator (cadr (node-form node))))
	(else
	 #f)))

; We elect to integrate a procedure definition when
;  1. The procedure in not n-ary,
;  2. Every parameter is used exactly once and not assigned, and
;  3. The analysis phase says that the body is acceptable (see below). 

(define (simple-lambda? node id package)
  (let* ((exp (node-form node))
	 (formals (cadr exp))
	 (body (caddr exp))
	 (var-nodes (normalize-formals formals)))
    (and (require "not n-ary" id
	   (not (n-ary? formals)))
	 (require "unique references" id
	   (every (lambda (var-node)
		    (let ((usage (node-ref var-node 'usage)))
		      (and (= (usage-reference-count usage) 1)
			   (= (usage-assignment-count usage) 0))))
		  var-nodes))
	 (require "good analysis" id
	   (simple? (caddr exp) ret)))))

(define operator/name (get-operator 'name 'leaf))

; --------------------
; SIMPLE? takes an alpha-converted expression and returns either
;  - #f, meaning that the procedure in which the expression occurs
;    has no chance of being fully inlinable, so we might as well give up,
;  - #t, if there's no problem, or
;  - 'empty, if there's no problem AND there are no lexical variable
;    references at or below this node.
; Foul situations are:
;  - complex quotations (we don't want to make multiple copies of them)
;  - a LAMBDA occurs (too much overhead, presumably)
;  - a call that is not to a primitive and not a tail call


; Main dispatch for analyzer
; The name node analyzer needs the node; all others can get by with the
; expression.

(define (simple? node ret?)
  ((operator-table-ref analyzers (node-operator-id node))
     (if (name-node? node)
	 node
	 (node-form node))
     ret?))

(define (simple-list? exp-list)
  (if (null? exp-list)
      'empty
      (let ((s1 (simple? (car exp-list) no-ret)))
	(cond ((eq? s1 'empty)
	       (simple-list? (cdr exp-list)))
	      ((and s1
		    (simple-list? (cdr exp-list)))
	       #t)
	      (else
	       #f)))))

; Particular operators

(define analyzers
  (make-operator-table (lambda (exp ret?)
			 (simple-list? (cdr exp)))))

(define (define-analyzer name proc)
  (operator-define! analyzers name #f proc))

(define-analyzer 'literal
  (lambda (exp ret?)
    (if (require "repeatable literal" #f
	  (simple-literal? exp))
	'empty
	#f)))

(define-analyzer 'unspecific
  (lambda (exp ret?)
    #t))

; It's too awkward to try to inline references to unbound variables.
; By special dispensation, this one analyzer receives the node instead of the
; expression.  It needs the node to look up the binding record.

(define-analyzer 'name
  (lambda (node ret?)
    ;; (if (node-ref node 'usage) #t 'empty)
    ;;   ... (not (generated? exp)) ugh ...
    (not (eq? (node-ref node 'binding)
	      'unbound))))

(define-analyzer 'quote
  (lambda (exp ret?)
    (if (require "repeatable quotation" #f
	  (simple-literal? (cadr exp)))
	'empty
	#f)))

(define-analyzer 'lambda
  (lambda (exp ret?) #f))

(define-analyzer 'letrec
  (lambda (exp ret?) #f))

(define-analyzer 'pure-letrec
  (lambda (exp ret?) #f))

(define-analyzer 'lap
  (lambda (exp ret?) #f))

; SET! loses because we might move a variable reference past a SET! on the
; variable.  This can't happen if the SET! is the last thing done.
; It's too awkward to try to inline references to unbound variables.

(define-analyzer 'set!
  (lambda (exp ret?)
    (and ret?
	 (not (eq? (node-ref (cadr exp) 'binding)
		   'unbound))
	 (simple? (caddr exp) no-ret))))

(define-analyzer 'loophole
  (lambda (exp ret?)
    (simple? (caddr exp) ret?)))

; Can't always fully in-line things like (lambda (a b c) (if a b c))

(define-analyzer 'if
  (lambda (exp ret?)
    (and (eq? (simple? (caddr exp) ret?) 'empty)
	 (eq? (simple? (cadddr exp) ret?) 'empty)
	 (simple? (cadr exp) no-ret))))

(define-analyzer 'begin
  (lambda (exp ret?)
    (let loop ((exps (cdr exp)))
      (if (null? (cdr exps))
	  (if (simple? (car exps) ret?) #t #f)
	  (and (simple? (car exps) no-ret)
	       (loop (cdr exps)))))))

(define-analyzer 'call
  (lambda (exp ret?)
    (let ((static (static-value (car exp))))
      (if (transform? static)
          (let ((new-node
                 (apply-inline-transform static
                                         exp
                                         (node-form (car exp)))))
            (if (eq? new-node exp)
                (really-simple-call? exp ret?)
                (simple? new-node ret?)))
	  (really-simple-call? exp ret?)))))

; Return the static value of FORM, if any.

(define (static-value form)
  (if (and (node? form)
	   (name-node? form))
      (let ((probe (node-ref form 'binding)))
	(if (binding? probe)
	    (binding-static probe)
	    #f))
      #f))

(define (really-simple-call? exp ret?)
  (let ((proc (car exp)))
    (and (require "non-local non-tail call" proc
	   (or (and ret? (simple? proc no-ret))	;tail calls are ok
               (primitive-proc? proc)))         ;as are calls to primitives
	 (simple-list? exp))))

; Calls to primitives and lexically bound variables are okay.

(define (primitive-proc? proc)
  (cond ((literal-node? proc)
	 (primop? (node-form proc)))
	((name-node? proc)
	 (let ((binding (node-ref proc 'binding)))
	   (and (binding? binding)
		(primop? (binding-static binding)))))
	(else
	 #f)))

(define no-ret #f)

(define ret #t)

(define (simple-literal? x) ;Things that TRANSPORT won't copy.
  (or (integer? x)
      (boolean? x)
      (null? x)
      (char? x)
      (symbol? x)))

; --------------------
; debugging hack

(define (require reason id x)
  (if (and *debug?* (not x))
      (begin (write id)
	     (display " lost because ")
	     (display reason)
	     (display " failed")
	     (newline)))
  x)

(define *debug?* #f)

; utility

(define (package-lookup-type p name)
  (let ((probe (package-lookup p name)))
    (if (binding? probe)
	(binding-type probe)
	#f)))

; --------------------

(define lambda-node? (node-predicate 'lambda))
(define name-node? (node-predicate 'name))
(define loophole-node? (node-predicate 'loophole))
(define define-node? (node-predicate 'define syntax-type))
(define literal-node? (node-predicate 'literal 'leaf))

;----------------
;(define (foo f p)
;  (analyze-forms (alpha-forms (scan-file f p) p)))
;
;
;(define (tst e p)
;  (inlinable-rhs? (alpha e p) #f))
;
;(define b (make-compiler-base))
;
;(define p (make-simple-package (list b) eval #f))
;
;; (define b-stuff (alpha-structure b))
;
