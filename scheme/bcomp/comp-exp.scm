; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Compiling expressions.

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

; Optimizations are marked with +++, and may be flushed if desired.

(define (compile-expression node depth cont)
  (compile node 0 depth cont))

; Main dispatch for compiling a single expression.

(define (compile node level depth cont)
  (let ((node (type-check node)))
    ((operator-table-ref compilators (node-operator-id node))
     node
     level
     depth
     cont)))

; Specialists

(define compilators
  (make-operator-table
    (lambda (node level depth cont)
      (generate-trap cont
		     "not valid in expression context"
		     (schemify node)))))

(define (define-compilator name type proc)
  (operator-define! compilators name type proc))

(define-compilator 'literal 'leaf
  (lambda (node level depth cont)
    (let ((obj (node-form node)))
      (if (eq? obj #f)
	  ;; +++ hack for bootstrap from Schemes that don't distinguish #f/()
	  (deliver-value (instruction (enum op false)) cont)
	  (compile-constant obj depth cont)))))

(define-compilator 'unspecific (proc () unspecific-type)
  (lambda (node level depth cont)
    (deliver-value (instruction (enum op unspecific))
		   cont)))

(define-compilator 'unassigned (proc () unspecific-type)
  (lambda (node level depth cont)
    (deliver-value (instruction (enum op unassigned))
		   cont)))

(define-compilator 'quote syntax-type
  (lambda (node level depth cont)
    (let ((exp (node-form node)))
      level				;ignored
      (let ((obj (cadr exp)))
	(compile-constant obj depth cont)))))

(define (compile-constant obj depth cont)
  (if (ignore-values-cont? cont)
      empty-segment			;+++ dead code
      (deliver-value (instruction-with-literal (enum op literal) obj)
		     cont)))

; Variable reference

(define-compilator 'name 'leaf
  (lambda (node level depth cont)
    (let* ((binding (name-node-binding node))
	   (name (node-form node)))
      (deliver-value
         (if (pair? binding)
	     (let ((back (- level (car binding)))
		   (over (cdr binding)))
	       (if (or (>= back byte-limit)
		       (>= over byte-limit))
		   (instruction (enum op big-local)
				(high-byte back)
				(low-byte back)
				(high-byte over)
				(low-byte over))
		   (case back
		     ((0) (instruction (enum op local0) over)) ;+++
		     ((1) (instruction (enum op local1) over)) ;+++
		     ((2) (instruction (enum op local2) over)) ;+++
		     (else (instruction (enum op local) back over)))))
	     (instruction-with-location (enum op global)
					binding
					name
					value-type))
	 cont))))

; Assignment

(define-compilator 'set! syntax-type
  (lambda (node level depth cont)
    (let* ((exp (node-form node))
	   (lhs-node (cadr exp))
	   (name (node-form lhs-node))
	   ;; Error if not a name node...
	   (binding (name-node-binding lhs-node)))
      (sequentially
       (compile (caddr exp) level depth (named-cont name))
       (deliver-value
	(if (pair? binding)
	    (let ((back (- level (car binding)))
		  (over (cdr binding)))
	      (instruction (enum op set-local!)
			   (high-byte back)
			   (low-byte back)
			   (high-byte over)
			   (low-byte over)))
	    (instruction-with-location (enum op set-global!)
				       binding
				       name
				       usual-variable-type))
	cont)))))

; Conditional

(define-compilator 'if syntax-type
  (lambda (node level depth cont)
    (let ((exp (node-form node))
	  (alt-label (make-label))
	  (join-label (make-label)))
      (sequentially
       ;; Test
       (compile (cadr exp) level depth (fall-through-cont node 1))
       (instruction-using-label (enum op jump-if-false) alt-label)
       ;; Consequent
       (compile (caddr exp) level depth cont)
       (if (fall-through-cont? cont)
	   (instruction-using-label (enum op jump) join-label)
	   empty-segment)
       ;; Alternate
       (attach-label alt-label
		     (compile (cadddr exp) level depth cont))
       (attach-label join-label
		     empty-segment)))))

(define-compilator 'begin syntax-type
  (lambda (node level depth cont)
    (let ((exp-list (cdr (node-form node))))
      (if (null? exp-list)
	  (generate-trap cont "null begin")
	  (let ((dummy
		 (make-node operator/begin ;For debugging database
			    `(begin ,@exp-list))))
	    (let loop ((exp-list exp-list) (i 1))
	      (if (null? (cdr exp-list))
		  (compile (car exp-list) level depth cont)
		  (sequentially
		    (compile (car exp-list) level depth
			     (ignore-values-cont dummy i))
		    (loop (cdr exp-list) (+ i 1))))))))))

; Compile a call

(define (compile-call node level depth cont)
  (if (node-ref node 'type-error)
      (compile-unknown-call node level depth cont)
      (let ((proc-node (car (node-form node))))
	(cond ((name-node? proc-node)
	       (compile-name-call node proc-node level depth cont))
	      ((and (lambda-node? proc-node)
		    (not (n-ary? (cadr (node-form proc-node)))))
	       (compile-redex proc-node (cdr (node-form node)) level depth cont))
 	      ((and (literal-node? proc-node)
 		    (primop? (node-form proc-node)))
 	       (let ((primop (node-form proc-node)))
 		 (if (primop-compilator primop)
 		     ((primop-compilator primop) node level depth cont)
 		     (error "compiler bug: primop has no compilator"
 			    primop
 			    (schemify node)))))
	      (else
	       (compile-unknown-call node level depth cont))))))

(define (compile-name-call node proc-node level depth cont)
  (let ((binding (name-node-binding proc-node)))
    (if (binding? binding)
	(let ((static (binding-static binding)))
	  (cond ((primop? static)
		 (if (primop-compilator static)
		     ((primop-compilator static) node level depth cont)
		     (compile-unknown-call node level depth cont)))
		((transform? static)
		 (let* ((form (node-form node))
			(new (apply-inline-transform static
						     form
						     (node-form proc-node))))
		   (if (eq? new form)
		       (compile-unknown-call node level depth cont)
		       (compile new level depth cont))))
		(else
		 (compile-unknown-call node level depth cont))))
	(compile-unknown-call node level depth cont))))

(define-compilator 'call 'internal compile-call)

; A redex is a call of the form ((lambda (x1 ... xn) body ...) e1 ... en).

(define (compile-redex proc-node args level depth cont)
  (let* ((proc-exp (node-form proc-node))
	 (formals (cadr proc-exp))
	 (body (caddr proc-exp)))
    (cond ((not (= (length formals)
		   (length args)))
	   (generate-trap cont
			  "wrong number of arguments"
			  (cons (schemify proc-node)
				(map schemify args))))
	  ((null? formals)
	   (compile body level depth cont)) ;+++
	  (else
	   (maybe-push-continuation
	    (sequentially 
	     (push-all-with-names args formals level 0)
	     (compile-lambda-code formals body level (cont-name cont) #f))
	    depth
	    cont)))))

; Compile a call to a computed procedure.

(define (compile-unknown-call node level depth cont)
  (let ((exp (node-form node)))
    (let ((call (sequentially (push-arguments node level 0)
			      (compile (car exp)
				       level
				       (length (cdr exp))
				       (fall-through-cont node 0))
			      (let ((nargs (length (cdr exp))))
				(if (> nargs maximum-stack-args)
				    (instruction (enum op big-call)
						 (high-byte nargs)
						 (low-byte nargs))
				    (instruction (enum op call) nargs))))))
      (maybe-push-continuation call depth cont))))

(define (maybe-push-continuation code depth cont)
  (if (return-cont? cont) 
      code
      (let ((label (make-label)))
	(sequentially (if (>= depth byte-limit)
			  (instruction-using-label (enum op make-big-cont)
						   label
						   (high-byte depth)
						   (low-byte depth))
			  (instruction-using-label (enum op make-cont)
						   label
						   depth))
		      (if (keep-source-code?)
			  (note-source-code (fixup-source (cont-source-info cont))
					    code)
			  code)
		      ;; Header and/or other data goes here.
		      ;; But what about the stack-check code?
		      ;; What about alignment?
		      (attach-label label
				    (if (accept-values-cont? cont)
					empty-segment
					(instruction
					  (enum op protocol)
					  (if (ignore-values-cont? cont)
					      ignore-values-protocol
					      1))))))))

(define (fixup-source info)
  ;; Abbreviate this somehow?
  (if (pair? info)
      (cons (car info)
	    (schemify (cdr info)))
      ;; Name might be generated...
      info))

; Continuation is implicitly fall-through.

(define (push-arguments node level depth)
  (let recur ((args (cdr (node-form node))) (depth depth) (i 1))
    (if (null? args)
	empty-segment
	(sequentially (compile (car args) level depth
			       (fall-through-cont node i))
		      (instruction (enum op push))
		      (recur (cdr args) (+ depth 1) (+ i 1))))))

(define (push-all-with-names exp-list names level depth)
  (if (null? exp-list)
      empty-segment
      (sequentially (compile (car exp-list)
			     level depth
			     (named-cont (node-form (car names))))
		    (instruction (enum op push))
                    (push-all-with-names (cdr exp-list)
					 (cdr names)
					 level
					 (+ depth 1)))))
     
; LETREC.

(define-compilator 'letrec syntax-type
  (lambda (node level depth cont)
    (let* ((exp (node-form node))
	   (specs (cadr exp))
	   (body (caddr exp))
	   (body (make-node operator/begin
			    `(begin
			       ,@(map (lambda (spec)
					(make-node operator/set!
						   `(set! ,@spec)))
				      specs)
			       ,body))))
      (if (null? specs)
	  (compile body level depth cont) ;+++
	  (maybe-push-continuation
	    (sequentially
	      (apply sequentially
		     (map (lambda (spec)
			    (sequentially
			      (instruction (enum op unassigned))
			      (instruction (enum op push))))
			  specs))
	      (compile-lambda-code (map car specs)
				   body
				   level
				   (cont-name cont)
				   #f))
	    depth
	    cont)))))

; (PURE-LETREC ((<var> <val>) ...) (<free var> ...) <body>)
; These are LETREC's where the values are all LAMBDA's.  They are produced by
; opt/flatten.scm. 

(define-compilator 'pure-letrec syntax-type
  (lambda (node level depth cont)
    (let* ((exp (node-form node))
	   (specs (cadr exp))
	   (free-vars (caddr exp))
	   (body (cadddr exp))
	   (count (length specs)))
      (call-with-values
        (lambda ()
	  (compile-flat-environment free-vars level))
	(lambda (env-code free-vars)
	  (maybe-push-continuation
	    (sequentially
	     (instruction-with-literal (enum op literal) (fluid $env-key))
	     (if (empty-segment? env-code)
		 (instruction (enum op make-stored-object)
			      1
			      (enum stob vector))
		 env-code)
	     (instruction (enum op letrec-closures)
			  (high-byte count)
			  (low-byte count))
	     (letrec-lambda-code specs free-vars)
	     (letrec-body-code body specs level (cont-name cont)))
	    depth
	    cont))))))

(define (empty-segment? segment)
  (= 0 (segment-size segment)))

(define (letrec-lambda-code specs free-vars)
  (let ((vars (map car specs))
	(vals (map (lambda (spec)
		     (node-form (cadr spec)))
		   specs)))
    (set-lexical-offsets! vars 0)
    (apply sequentially
	   (map (lambda (var val)
		  (really-compile-flat-lambda (node-form var)
					      (cadr val)	; vars
					      (cadddr val)	; body
					      free-vars
					      1))		; depth
		vars
		vals))))

(define (letrec-body-code body specs level name)
  (let ((vars (map car specs))
	(level (+ level 1)))
    (set-lexical-offsets! (reverse vars) level)
    (note-environment
     (map name-node->symbol vars)
     (compile body level 0 (return-cont name)))))

; --------------------
; Compile-time continuations
;
; A compile-time continuation is a pair (kind . source-info).
; Kind is one of the following:
;   'return        - invoke the current full continuation.
;   'fall-through  - fall through to subsequent instructions.
;   'ignore-values - ignore values, then fall through.
;   'accept-values - pass values to continuation
; Source-info is one of:
;   #f - we don't know anything
;   symbol - value delivered to subsequent instructions will be assigned to
;     a variable with this name.  If the value being assigned is a lambda, we
;     can give that lambda that name.
;   (i . node) - the value being computed is the i'th subexpression of the node.

(define (make-cont kind source-info) (cons kind source-info))
(define cont-kind car)
(define cont-source-info cdr)

; We could probably be able to optimize jumps to jumps.
;(define (make-jump-cont label cont)
;  (if (fall-through-cont? cont)
;      (make-cont label (cont-name cont))
;      cont))

(define (return-cont name)
  (make-cont 'return name))

(define (return-cont? cont)
  (eq? (cont-kind cont) 'return))

; Fall through into next instruction while compiling the I'th part of NODE.

(define (fall-through-cont node i)
  (make-cont 'fall-through (cons i node)))

(define (fall-through-cont? cont)
  (not (return-cont? cont)))

; Ignore return value, then fall through

(define ignore-values-segment
  (instruction ignore-values-protocol))

(define (ignore-values-cont node i)
  (make-cont 'ignore-values (cons i node)))

(define (ignore-values-cont? cont)
  (eq? (cont-kind cont) 'ignore-values))

(define (accept-values-cont node i)
  (make-cont 'accept-values (cons i node)))

(define (accept-values-cont? cont)
  (eq? (cont-kind cont) 'accept-values))

; Value is in *val*; deliver it to its continuation.

(define (deliver-value segment cont)
  (if (return-cont? cont)
      (sequentially segment
		    (instruction (enum op return)))
      segment))			; just fall through to next instruction

; For putting names to lambda expressions:

(define (named-cont name)
  (make-cont 'fall-through name))

(define (cont-name cont)
  (if (pair? (cont-source-info cont))
      #f
      (cont-source-info cont)))

; Find lookup result that was cached by classifier

(define (name-node-binding node)
  (or (node-ref node 'binding)
      (node-form node)))

; --------------------
; Utilities

; Produce something for source code that contains a compile-time error.

(define (generate-trap cont . stuff)
  (apply warn stuff)
  (sequentially (instruction-with-literal (enum op literal)
					  (cons 'error stuff))
		(deliver-value (instruction (enum op trap))
			       cont)))

; --------------------
; Type checking.  This gets called on all nodes.

(define (type-check node)
  (if *type-check?*
      (let ((form (node-form node)))
	(if (pair? form)
	    (let ((proc-node (car form)))
	      (if (node? proc-node)
		  (let ((proc-type (node-type proc-node)))
		    (cond ((procedure-type? proc-type)
			   (if (restrictive? proc-type)
			       (let* ((args (cdr form))
				      (args-type (make-some-values-type
						  (map (lambda (arg)
							 (meet-type
							  (node-type arg)
							  value-type))
						       args)))
				      (node (make-similar-node node
							       (cons proc-node
								     args))))
				 (if (not (meet? args-type
						 (procedure-type-domain proc-type)))
				     (diagnose-call-error node proc-type))
				 node)
			       node))
			  ((not (meet? proc-type any-procedure-type))
			   ;; Could also check args for one-valuedness.
			   (let ((message "non-procedure in operator position"))
			     (warn message
				   (schemify node)
				   `(procedure: ,proc-type))
			     (node-set! node 'type-error message))
			   node)
			  (else node)))
		  node))
	    node))
      node))

(define (set-type-check?! check?)
  (set! *type-check?* check?))

(define *type-check?* #t)


(define (diagnose-call-error node proc-type)
  (let ((message
	 (cond ((not (fixed-arity-procedure-type? proc-type))
		"invalid arguments")
	       ((= (procedure-type-arity proc-type)
		   (length (cdr (node-form node))))
		"argument type error")
	       (else
		"wrong number of arguments"))))
    (warn message
	  (schemify node)
	  `(procedure wants:
		      ,(rail-type->sexp (procedure-type-domain proc-type)
					#f))
	  `(arguments are: ,(map (lambda (arg)
				   (type->sexp (node-type arg) #t))
				 (cdr (node-form node)))))
    (node-set! node 'type-error message)))


; Type system loophole

(define-compilator 'loophole syntax-type
  (lambda (node level depth cont)
    (compile (caddr (node-form node)) level depth cont)))

; Node predicates and operators.

(define lambda-node?      (node-predicate 'lambda syntax-type))
(define flat-lambda-node? (node-predicate 'flat-lambda syntax-type))
(define name-node?        (node-predicate 'name 'leaf))
(define literal-node?     (node-predicate 'literal 'leaf))

(define operator/lambda     (get-operator 'lambda syntax-type))
(define operator/set!	    (get-operator 'set!   syntax-type))
(define operator/call	    (get-operator 'call   'internal))
(define operator/begin      (get-operator 'begin  syntax-type))
