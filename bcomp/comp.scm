; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file comp.scm.

;;;; The byte-code compiler

; This is a two-phase compiler.  The first phase does macro expansion,
; variable resolution, and instruction selection, and computes the
; size of the code vector.  The second phase (assembly) creates the
; code vector, "template" (literals vector), and debugging data
; structures.

; The output of the first phase (the COMPILE- and INSTRUCTION-
; routines) and the input to the second phase (SEGMENT->TEMPLATE) is a
; "segment."  A segment is a pair (size . proc) where size is the size
; of the code segment in bytes, and proc is a procedure that during
; phase 2 will store the segment's bytes into the code vector.

; A "cenv" maps lexical variables to <level, offset> pairs.  Level is
; the variable's distance from the root of the environment; 0 means
; outermost level, and higher numbers mean deeper lexical levels.  The
; offset is the position of the variable within its level's
; environment vector.

; Optimizations are marked with +++, and may be flushed if desired.


(define (compile-top exp cenv depth cont)
  (compile exp (initial-cenv cenv) depth cont))


; Main dispatch for compiling a single expression.

(define (compile exp cenv depth cont)
  (let* ((node (classify exp cenv))
	 (node (if *type-check?*
		   (type-check node cenv)
		   node)))
    ((operator-table-ref compilators (node-operator-id node))
     node
     cenv
     depth
     cont)))
(define *type-check?* #t) ;for compiler timings


; Specialists

(define compilators
  (make-operator-table (lambda (node cenv depth cont)
			 (generate-trap cont
					"not valid in expression context"
					(schemify node)))
		       (lambda (frob)  ;for let-syntax, with-aliases, etc.
			 (lambda (node cenv depth cont)
			   (call-with-values (lambda () (frob node cenv))
			     (lambda (form cenv)
			       (compile form cenv depth cont)))))))

(define (define-compilator name proc)
  (operator-define! compilators name proc))

(define-compilator 'literal
  (lambda (node cenv depth cont)
    (let ((obj (node-form node)))
      (if (eq? obj #f)
	  ;; +++ hack for bootstrap from Schemes that don't distinguish #f/()
	  (deliver-value (instruction op/false) cont)
	  (compile-constant obj depth cont)))))

(define-compilator 'quote
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))
      cenv				;ignored
      (let ((obj (cadr exp)))
	(compile-constant obj depth cont)))))

(define (compile-constant obj depth cont)
  (if (ignore-values-cont? cont)
      empty-segment			;+++ dead code
      (deliver-value (instruction-with-literal op/literal obj)
		     cont)))

; Variable reference

(define-compilator 'name
  (lambda (node cenv depth cont)
    (let* ((binding (name-node-binding node cenv))
	   (name (node-form node)))
      (deliver-value (if (and (binding? binding)
			      (pair? (binding-place binding)))
			 (let* ((level+over (binding-place binding))
				(back (- (environment-level cenv)
					 (car level+over)))
				(over (cdr level+over)))
			   (case back
			     ((0) (instruction op/local0 over)) ;+++
			     ((1) (instruction op/local1 over)) ;+++
			     ((2) (instruction op/local2 over)) ;+++
			     (else (instruction op/local back over))))
			 (instruction-with-location
			        op/global
				(get-location binding cenv name value-type)))
		     cont))))

; Assignment

(define-compilator (list 'set! syntax-type)
  (lambda (node cenv depth cont)
    (let* ((exp (node-form node))
	   (lhs-node (classify (cadr exp) cenv))
	   (name (node-form lhs-node))
	   ;; Error if not a name node...
	   (binding (name-node-binding lhs-node cenv)))
      (sequentially
       (compile (caddr exp) cenv depth (named-cont name))
       (deliver-value
	(if (and (binding? binding) (pair? (binding-place binding)))
	    (let ((level+over (binding-place binding)))
	      (instruction op/set-local!
			   (- (environment-level cenv) (car level+over))
			   (cdr level+over)))
	    (instruction-with-location op/set-global!
	      (get-location binding cenv name usual-variable-type)))
	cont)))))

; Conditional

(define-compilator (list 'if syntax-type)
  (lambda (node cenv depth cont)
    (let ((exp (node-form node))
	  (alt-label (make-label))
	  (join-label (make-label)))
      (sequentially
       ;; Test
       (compile (cadr exp) cenv depth (fall-through-cont (schemify node) 1))
       (instruction-using-label op/jump-if-false alt-label)
       ;; Consequent
       (compile (caddr exp) cenv depth cont)
       (if (fall-through-cont? cont)
	   (instruction-using-label op/jump join-label)
	   empty-segment)
       ;; Alternate
       (attach-label alt-label
		     (compile (cadddr exp) cenv depth cont))
       (attach-label join-label
		     empty-segment)))))


(define-compilator (list 'begin syntax-type)
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))
      (compile-begin (cdr exp) cenv depth cont))))

(define (compile-begin exp-list cenv depth cont)
  (if (null? exp-list)
      (generate-trap cont "null begin")
      (let ((dummy `(begin ,@(map schemify exp-list))));for debugging database
	(let loop ((exp-list exp-list) (i 1))
	  (if (null? (cdr exp-list))
	      (compile (car exp-list) cenv depth cont)
	      (careful-sequentially
		 (compile (car exp-list) cenv depth
			  (ignore-values-cont dummy i))
		 (loop (cdr exp-list) (+ i 1))
		 depth
		 cont))))))


; Compile a call

(define (compile-call node cenv depth cont)
  (let* ((exp (node-form node))
	 (proc-node (car exp)))
    (if (and (lambda-node? proc-node)
	     (let ((formals (cadr (node-form proc-node))))
	       (and (not (n-ary? formals))
		    (= (length (cdr exp))
		       (number-of-required-args formals)))))
	(compile-redex proc-node (cdr exp) cenv depth cont)
	(let ((new-node (maybe-transform-call proc-node node cenv)))
	  (if (eq? new-node node)
	      (compile-unknown-call node cenv depth cont)
	      (compile new-node cenv depth cont))))))

(define-compilator 'call compile-call)


; A redex is a call of the form ((lambda (x1 ... xn) body ...) e1 ... en).

(define lambda-node? (node-predicate 'lambda))

(define (compile-redex proc-node args cenv depth cont)
  (let* ((proc-exp (node-form proc-node))
	 (formals (cadr proc-exp))
	 (body (cddr proc-exp)))
    (if (null? formals)
	(compile-body body cenv depth cont) ;+++
	(maybe-push-continuation
	 (sequentially 
	  (push-all-with-names args formals cenv 0)
	  (compile-lambda-code formals body cenv (cont-name cont)))
	 depth
	 cont))))

; Compile a call to a computed procedure.

(define (compile-unknown-call node cenv depth cont)
  (let ((exp (node-form node)))
    (let ((call (sequentially (push-arguments node cenv 0)
			      (compile (car exp)
				       cenv
				       (length (cdr exp))
				       (fall-through-cont (schemify node) 0))
			      (instruction op/call (length (cdr exp))))))
      (maybe-push-continuation call depth cont))))

(define (maybe-push-continuation code depth cont)
  (if (return-cont? cont) 
      code
      (let ((label (make-label)))
	(sequentially (instruction-using-label op/make-cont
					       label
					       depth)
		      (note-source-code (cont-source-info cont)
					code)
		      (attach-label label
				    (cont-segment cont))))))

; Continuation is implicitly fall-through.

(define (push-arguments node cenv depth)
  (let ((exp (schemify node)))
    (let recur ((args (cdr (node-form node))) (depth depth) (i 1))
      (if (null? args)
	  empty-segment
	  (sequentially (compile (car args) cenv depth
				 (fall-through-cont exp i))
			(instruction op/push)
			(recur (cdr args) (+ depth 1) (+ i 1)))))))

(define (push-all-with-names exp-list names cenv depth)
  (if (null? exp-list)
      empty-segment
      (sequentially (compile (car exp-list)
			     cenv depth
			     (named-cont (car names)))
		    (instruction op/push)
                    (push-all-with-names (cdr exp-list)
					 (cdr names)
					 cenv
					 (+ depth 1)))))
     
; OK, now that you've got all that under your belt, here's LAMBDA.

(define-compilator (list 'lambda syntax-type)
  (lambda (node cenv depth cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (deliver-value
       (instruction-with-template op/closure
				  (compile-lambda exp cenv (if name #t #f))
				  name)
       cont))))

(define (compile-lambda exp cenv body-name)
  (let* ((formals (cadr exp))
	 (nargs (number-of-required-args formals)))
    (sequentially
     ;; Check number of arguments
     (if (n-ary? formals)
	 (if (pair? formals)
	     (instruction op/check-nargs>= nargs)
	     empty-segment)		;+++ (lambda x ...) needs no check
	 (instruction op/check-nargs= nargs))
     (compile-lambda-code formals (cddr exp) cenv body-name))))

; name isn't the name of the procedure, it's the name to be given to
; the value that the procedure will return.

(define (compile-lambda-code formals body cenv name)
  (if (null? formals)
      (compile-body body		;+++ Don't make null environment
		    cenv
		    0
		    (return-cont name))
      ;; (if (node-ref node 'no-inferior-lambdas) ...)
      (sequentially
       (let ((nargs (number-of-required-args formals)))
	 (if (n-ary? formals)
	     (sequentially
	      (instruction op/make-rest-list nargs)
	      (instruction op/push)
	      (instruction op/make-env (+ nargs 1)))
	     (instruction op/make-env nargs)))
       (let* ((vars (normalize-formals formals))
	      (cenv (bind-vars (reverse vars) cenv)))
	 (note-environment
	  vars
	  (compile-body body
			cenv
			0
			(return-cont name)))))))

(define compile-letrec
  (let ((operator/lambda     (get-operator 'lambda syntax-type))
	(operator/set!	     (get-operator 'set!   syntax-type))
	(operator/unassigned (get-operator 'unassigned)))
    (lambda (node cenv depth cont)
      ;; (if (node-ref node 'pure-letrec) ...)
      (let* ((exp (node-form node))
	     (specs (cadr exp))
	     (body (cddr exp)))
	(compile-redex (make-node operator/lambda
				  `(lambda ,(map car specs)
				     ,@(map (lambda (spec)
					      (make-node operator/set!
							 `(set! ,@spec)))
					    specs)
				     (,(make-node operator/lambda
						  `(lambda () ,@body)))))
		       (map (lambda (spec)
			      (make-node operator/unassigned
					 `(unassigned)))
			    specs)
		       cenv depth cont)))))

(define-compilator (list 'letrec syntax-type) compile-letrec)

; --------------------
; Deal with internal defines (ugh)

(define (compile-body body cenv depth cont)
  (scan-body body
	     cenv
	     (lambda (defs exps)
	       (if (null? defs)
		   (compile-begin exps cenv depth cont)
		   (compile-letrec
			(make-node operator/letrec
				   `(letrec ,(map (lambda (node)
						    (cdr (node-form node)))
						  defs)
				      ,@exps))
			cenv depth cont)))))

(define operator/letrec (get-operator 'letrec))

; --------------------
; Compile-time continuations
;
; A compile-time continuation is a pair (segment . name).  Segment is
; one of the following:
;   a return instruction - invoke the current full continuation.
;   empty-segment - fall through to subsequent instructions.
;   an ignore-values instruction - ignore values, then fall through.
; If name is non-#f, then the value delivered to subsequent
; instructions will be assigned to a variable.  If the value being
; assigned is a lambda, we can give that lambda that name, for
; debugging purposes.

(define (make-cont seg source-info) (cons seg source-info))
(define cont-segment car)
(define cont-source-info cdr)

; Eventually we may be able to optimize jumps to jumps.  Can't yet.
;(define (make-jump-cont jump cont)
;  (if (fall-through-cont? cont)
;      (make-cont jump (cont-name cont))
;      cont))

(define return-cont-segment (instruction (enum op return)))

(define (return-cont name)
  (make-cont return-cont-segment name))

(define (return-cont? cont)
  (eq? (cont-segment cont) return-cont-segment))

; Fall through into next instruction

(define (fall-through-cont exp i)
  (make-cont empty-segment (cons i exp)))

(define (fall-through-cont? cont)
  (not (return-cont? cont)))

; Ignore return value, then fall through

(define ignore-values-segment
  (instruction (enum op ignore-values)))

(define (ignore-values-cont exp i)
  (make-cont ignore-values-segment (cons i exp)))

(define (ignore-values-cont? cont)
  (eq? (cont-segment cont) ignore-values-segment))

; Value is in *val*; deliver it to its continuation.
; No need to generate an ignore-values instruction in this case.

(define (deliver-value segment cont)
  (if (ignore-values-cont? cont)	;+++
      segment
      (sequentially segment (cont-segment cont))))

; For putting names to lambda expressions:

(define (named-cont name)
  (make-cont empty-segment name))

(define (cont-name cont)
  (if (pair? (cont-source-info cont))
      #f
      (cont-source-info cont)))

; --------------------
; Compile-time environments

(define (bind-vars names cenv)
  (let ((level (+ (environment-level cenv) 1)))
    (lambda (name)
      (if (eq? name *level-key*)
	  level
	  (let loop ((over 1) (names names))
	    (cond ((null? names)
		   (lookup cenv name))
		  ((eq? name (car names))
		   (make-binding usual-variable-type
				 (cons level over)))
		  (else (loop (+ over 1) (cdr names)))))))))

(define (initial-cenv cenv)
  (bind1 *level-key* -1 cenv))

(define (environment-level cenv)
  (lookup cenv *level-key*))

(define *level-key* (string->symbol "Lexical nesting level"))

; Find lookup result that was cached by classifier

(define (name-node-binding node cenv)
  (or (node-ref node 'binding)
      (node-form node)))  ; = (lookup cenv (node-form node))


; --------------------
; Utilities

; Produce something for source code that contains a compile-time error.

(define (generate-trap cont . stuff)
  (apply warn stuff)
  (sequentially (instruction-with-literal op/literal (cons 'error stuff))
		(deliver-value (instruction op/trap)
			       cont)))

; Make a segment smaller, if it seems necessary, by introducing an
; extra template.  A segment is "too big" if it accesses more literals
; than the size of the operand in a literal-accessing instruction.
; The number of literals is unknowable given current representations,
; so we conservatively shrink the segment when its size exceeds 2
; times the largest admissible operand value, figuring that it takes
; at least 2 instruction bytes to use a literal.

(define (careful-sequentially seg1 seg2 depth cont)
  (if (and (= depth 0)
	   (> (+ (segment-size seg1) (segment-size seg2))
	      large-segment-size))
      (if (> (segment-size seg1) (segment-size seg2))
	  (sequentially (maybe-push-continuation
			 (sequentially 
			  (instruction-with-template
			   op/closure
			   (sequentially seg1
					 (instruction op/return))
			   #f)
			  (instruction op/call 0))
			 0
			 (fall-through-cont #f #f))
			seg2)
	  (sequentially seg1
			(maybe-push-continuation
			 (sequentially 
			  (instruction-with-template
			   op/closure
			   (if (return-cont? cont)
			       seg2
			       (sequentially seg2
					     (instruction op/return)))
			   #f)
			  (instruction op/call 0))
			 0
			 cont)))
      (sequentially seg1 seg2)))

(define large-segment-size (* byte-limit 2))


(define op/call (enum op call))
(define op/check-nargs= (enum op check-nargs=))
(define op/check-nargs>= (enum op check-nargs>=))
(define op/closure (enum op closure))
(define op/false (enum op false))
(define op/global (enum op global))
(define op/ignore-values (enum op ignore-values))
(define op/jump (enum op jump))
(define op/jump-if-false (enum op jump-if-false))
(define op/literal (enum op literal))
(define op/local (enum op local))
(define op/local0 (enum op local0))
(define op/local1 (enum op local1))
(define op/local2 (enum op local2))
(define op/make-cont (enum op make-cont))
(define op/make-env (enum op make-env))
(define op/make-rest-list (enum op make-rest-list))
(define op/push (enum op push))
(define op/return (enum op return))
(define op/set-global! (enum op set-global!))
(define op/set-local! (enum op set-local!))
(define op/trap (enum op trap))
(define op/unspecific (enum op unspecific))



; --------------------
; Type checking.  This only handles calls; special forms have their
; own checks.

(define (type-check node cenv)
  (let ((form (node-form node)))
    (if (and (pair? form)
	     (node? (car form)))
	(let ((proc-type (node-type (car form) cenv)))
	  (if (eq? proc-type syntax-type)
	      node
	      (let ((args (cdr form)))
		;; or (map (lambda (arg) (classify arg cenv)) (cdr form))
		(if (fixed-arity-procedure-type? proc-type)
		    (if (= (procedure-type-arity proc-type) (length args))
			(if (not (compatible-type-lists?
				  (map (lambda (arg) (node-type arg cenv))
				       args)
				  (procedure-type-argument-types proc-type)))
			    (report-type-error "argument type error"
					       node proc-type))
			(report-type-error "wrong number of arguments"
					   node proc-type))
		    (if (not (compatible-types? proc-type any-procedure-type))
			;; Could also check args for one-valuedness.
			(report-type-error "non-procedure in operator position"
					   node proc-type)))
		;; (make-similar-node node (cons (car form) args))
		node)))
	node)))

(define (report-type-error message node proc-type)
  (warn message
	(schemify node)
	`(procedure: ,proc-type))
  #f)


; Type system loophole

(define-compilator (list 'loophole syntax-type)
  (lambda (node cenv depth cont)
    (compile (caddr (node-form node)) cenv depth cont)))
