; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Simple code analysis to determine whether it's a good idea to
; in-line calls to a given procedure.

; Hook into the byte code compiler.

(set-optimizer! 'auto-integrate
  (lambda (stuff p)
    (let ((out (current-noise-port)))
      ; (set-package-integrate?! p #t) ; this proc is only called when this is #T
      (newline out)
      (display "Analyzing... " out) (force-output out)
      (let* ((names '())
	     (stuff
	      (map (lambda (filename+nodes)
		     (let ((filename (car filename+nodes))
			   (nodes (cdr filename+nodes)))
		       (set! names
			     (append (analyze-forms nodes p) names))
		       (cons filename nodes)))
		   stuff)))
	(cond ((not (null? names))
	       (newline out)
	       (display "Calls will be compiled in line: " out)
	       (write (reverse names) out))
	      (else
	       (display "no in-line procedures" out)))
	(newline out)
	stuff))))

(define (analyze-forms scanned-nodes p)
  (let ((inlines '()))
    (for-each (lambda (node)
		(let ((lhs (analyze-form node p)))
		  (if lhs
		      (set! inlines (cons lhs inlines)))))
	      scanned-nodes)
    inlines))

(define (analyze-form node p)		;Return LHS iff calls will be inlined.
  (if (define-node? node)
      (let ((form (node-form node)))
	(let ((lhs (cadr form))
	      (rhs (caddr form)))
	  (let ((type (package-lookup-type p lhs)))
	    (if (variable-type? type)
		(require "not assigned" lhs #f)
		(let ((method (inlinable-rhs? rhs type p lhs)))
		  (if method
		      (begin (package-define! p lhs method)
			     (if (transform? method)
				 lhs
				 #f))
		      #f))))))
      #f))

(define lambda-node? (node-predicate 'lambda))
(define name-node? (node-predicate 'name))
(define loophole-node? (node-predicate 'loophole))


(define (inlinable-rhs? node type p lhs)
  (cond ((lambda-node? node)
	 (if (simple-lambda? node lhs p)
	     (make-inline-transform node type p lhs)
	     #f))
	((name-node? node)
	 (let ((name (node-form node)))
	   (if (and (require "symbol rhs" (list lhs name)
		      (symbol? name))
		    (require "rhs bound" (list lhs name)
		      (binding? (package-lookup-type p name)))
		    (require "rhs unassigned" (list lhs name)
		      (not (variable-type? (package-lookup-type p name))))
		    (require "definitely procedure" (list lhs name)
		      (procedure-type? (package-lookup-type p name))))
	       (make-inline-transform node type p lhs)
	       #f)))
	((loophole-node? node)
	 (inlinable-rhs? (caddr (node-form node)) type p lhs))
	((primitive-procedure-node? node)
	 (get-operator (cadr (node-form node))))
	(else #f)))

(define primitive-procedure-node? (node-predicate 'primitive-procedure))


; We elect to integrate a procedure definition when
;  1. The procedure in not n-ary,
;  2. Every parameter is used exactly once and not assigned, and
;  3. The analysis phase says that the body is acceptable (see below). 

(define (simple-lambda? node id p)
  (let* ((exp (node-form node))
	 (formals (cadr exp))
	 (body (caddr exp))
	 (var-nodes (node-ref node 'var-nodes)))
    (and (require "not n-ary" id
	   (not (n-ary? formals)))
	 (require "unique references" id
	   (every (lambda (var-node)
		    (let ((usage (node-ref var-node 'usage)))
		      (and (= (usage-reference-count usage) 1)
			   (= (usage-assignment-count usage) 0))))
		  var-nodes))
	 (require "good analysis" id
	   (simple? (caddr exp)
		    (bind formals
			  (map (lambda (name)
				 (make-node operator/name name))
			       formals)
			  (package->environment p))
		    ret)))))

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

(define (simple? node env ret?)
  ((operator-table-ref analyzers (node-operator-id node))
   (node-form node)
   env ret?))

(define (simple-list? exp-list env)
  (if (null? exp-list)
      'empty
      (let ((s1 (simple? (car exp-list) env no-ret)))
	(if (eq? s1 'empty)
	    (simple-list? (cdr exp-list) env)
	    (if s1
		(and (simple-list? (cdr exp-list) env)
		     #t)
		#f)))))


; Particular operators

(define analyzers
  (make-operator-table (lambda (exp env ret?)
			 (simple-list? (cdr exp) env))))

(define (define-analyzer name proc)
  (operator-define! analyzers name #f proc))

(define-analyzer 'literal
  (lambda (exp env ret?)
    (if (require "repeatable literal" #f
	  (simple-literal? exp))
	'empty
	#f)))

(define-analyzer 'name
  (lambda (exp env ret?)
    ;; (if (node-ref node 'usage) #t 'empty)
    ;;   ... (not (generated? exp)) ugh ...
    #t))

(define-analyzer 'quote
  (lambda (exp env ret?)
    (if (require "repeatable quotation" #f
	  (simple-literal? (cadr exp)))
	'empty
	#f)))

(define-analyzer 'lambda
  (lambda (exp env ret?) #f))

(define-analyzer 'letrec
  (lambda (exp env ret?) #f))

; SET! loses because we might move a variable reference past a SET! on the
; variable.  This can't happen if the SET! is the last thing done.

(define-analyzer 'set!
  (lambda (exp env ret?)
    (and ret?
	 (simple? (caddr exp) env no-ret))))

(define-analyzer 'loophole
  (lambda (exp env ret?)
    (simple? (caddr exp) env ret?)))

; Can't always fully in-line things like (lambda (a b c) (if a b c))

(define-analyzer 'if
  (lambda (exp env ret?)
    (and (eq? (simple? (caddr exp) env ret?) 'empty)
	 (eq? (simple? (cadddr exp) env ret?) 'empty)
	 (simple? (cadr exp) env no-ret))))

(define-analyzer 'begin
  (lambda (exp env ret?)
    (let loop ((exps (cdr exp)))
      (if (null? (cdr exps))
	  (if (simple? (car exps) env ret?) #t #f)
	  (and (simple? (car exps) env no-ret)
	       (loop (cdr exps)))))))

(define-analyzer 'call
  (lambda (exp env ret?)
    ;; Retry transforming calls in hopes of finding procedures that
    ;; have become integrable as a result of the ongoing analysis of
    ;; this package.
    (let ((proc (car exp)))
      (if (name-node? proc)
	  (let* ((node (make-node (get-operator 'call) exp))
		 (new-node (maybe-transform-call proc node env)))
	    (if (eq? new-node node)
		(really-simple-call? exp env ret?)
		(simple? (expand new-node env) env ret?)))
	  (really-simple-call? exp env ret?)))))

(define (really-simple-call? exp env ret?)
  (let ((proc (car exp)))
    (and (require "non-local non-tail call" proc
	   (or (and ret? (simple? proc env no-ret))	;tail calls are ok
	       (lexical-node? proc)))		;so are calls to arguments
	 (simple-list? exp env))))

(define (lexical-node? node)
  (not (node-ref node 'binding)))

(define no-ret #f)

(define ret #t)

(define (simple-literal? x) ;Things that TRANSPORT won't copy.
  (or (integer? x)
      (boolean? x)
      (null? x)
      (char? x)
      (symbol? x)))


; --------------------
; Once we know that we want something to be inlined, the following things
; actually makes use of the fact.  For procedures for which all
; arguments can be substituted unconditionally, we make a transform
; (a macro, really) that performs the substitution.

(define (make-inline-transform node type p name)
  (let* ((free (free-top-level-variables node))
	 (form (make-substitution-template node p free))
	 (aux-names (map (lambda (free)
			   (do ((free free (generated-parent-name free)))
			       ((not (generated? free)) free)))
			 free)))
    (make-transform (inline-transform form aux-names)
		    p			;env ?
		    type
		    `(inline-transform ',form ',aux-names)
		    name)))

; Create something that can be passed to SUBSTITUTE.  Must be valid as
; a quotation.

(define (make-substitution-template node p free)
  (let ((env (package->environment p)))
    (clean-node node
		(map (lambda (free)
		       (cons free (name->qualified free env)))
		     free))))

; This routine is obligated to return an S-expression.
; It's better not to rely on the constancy of node id's, so 
; the output language is a sort of quasi-Scheme.  Any form that's a list
; has an operator name in its car.

(define (clean-node node env)
  (cond ((name-node? node)
	 (clean-lookup env (node-form node)))
	((quote-node? node)
	 `(quote ,(cadr (node-form node))))
	((lambda-node? node)
	 (clean-lambda node env))
	((call-node? node)
	 (cons 'call
	       (map (lambda (node) (clean-node node env))
		    (node-form node))))
	((loophole-node? node)		;Uck
	 (let ((args (cdr (node-form node))))
	   `(loophole ,(schemify (car args))
		      ,(clean-node (cadr args) env))))
	;; LETREC had better not occur, since we ain't prepared for it
	((pair? (node-form node))
	 (cons (operator-name (node-operator node))
	       (map (lambda (subnode)
		      (clean-node subnode env))
		    (cdr (node-form node)))))
	(else (node-form node))))	;literal

(define quote-node? (node-predicate 'quote))
(define call-node? (node-predicate 'call))


(define (clean-lambda node env)
  (let* ((exp (node-form node))
	 (formals (cadr exp))
	 (env (append (map (lambda (name)
			     (cons name
				   (unused-name env name)))
			   (normalize-formals formals))
		      env)))
    `(lambda ,(let recur ((foo formals))
		(cond ((name? foo) (clean-lookup env foo))
		      ((pair? foo)
		       (cons (recur (car foo))
			     (recur (cdr foo))))
		      (else foo)))
       ,(clean-node (caddr exp) env))))

(define (clean-lookup env name)
  (cdr (assq name env)))		;Must be there.
  
; I'm aware that this is pedantic.

(define (unused-name env name)
  (let ((sym (if (generated? name)
		 (generated-symbol name)
		 name)))
    (do ((i 0 (+ i 1))
	 (name sym
	       (string->symbol (string-append (symbol->string sym)
					      (number->string i)))))
	((not (assq name env)) name))))

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
