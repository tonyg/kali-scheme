; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


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
  (let ((node (type-check (classify exp cenv) cenv)))
    ((operator-table-ref compilators (node-operator-id node))
     node
     cenv
     depth
     cont)))

; Specialists

(define compilators
  (make-operator-table (lambda (node cenv depth cont)
			 (generate-trap cont
					"not valid in expression context"
					(schemify node cenv)))
		       (lambda (frob)  ;for let-syntax, with-aliases, etc.
			 (lambda (node cenv depth cont)
			   (call-with-values (lambda () (frob node cenv))
			     (lambda (forms cenv)
			       (compile-body forms cenv depth cont)))))))

(define (define-compilator name type proc)
  (operator-define! compilators name type proc))

(define-compilator 'literal #f
  (lambda (node cenv depth cont)
    (let ((obj (node-form node)))
      (if (eq? obj #f)
	  ;; +++ hack for bootstrap from Schemes that don't distinguish #f/()
	  (deliver-value (instruction (enum op false)) cont)
	  (compile-constant obj depth cont)))))

(define-compilator 'quote syntax-type
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))
      cenv				;ignored
      (let ((obj (cadr exp)))
	(compile-constant obj depth cont)))))

(define (compile-constant obj depth cont)
  (if (ignore-values-cont? cont)
      empty-segment			;+++ dead code
      (deliver-value (instruction-with-literal (enum op literal) obj)
		     cont)))

; Variable reference

(define-compilator 'name #f
  (lambda (node cenv depth cont)
    (let* ((binding (name-node-binding node cenv))
	   (name (node-form node)))
      (deliver-value
         (if (and (binding? binding)
		  (pair? (binding-place binding)))
	     (let* ((level+over (binding-place binding))
		    (back (- (environment-level cenv)
			     (car level+over)))
		    (over (cdr level+over)))
	       (if (>= over byte-limit)
		   (instruction (enum op big-local)
				back
				(high-byte over)
				(low-byte over))
		   (case back
		     ((0) (instruction (enum op local0) over)) ;+++
		     ((1) (instruction (enum op local1) over)) ;+++
		     ((2) (instruction (enum op local2) over)) ;+++
		     (else (instruction (enum op local) back over)))))
	     (instruction-with-location
	      (enum op global)
	      (get-location binding cenv name value-type)))
	 cont))))

; Assignment

(define-compilator 'set! syntax-type
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
	      (instruction (enum op set-local!)
			   (- (environment-level cenv) (car level+over))
			   (high-byte (cdr level+over))
			   (low-byte (cdr level+over))))
	    (instruction-with-location (enum op set-global!)
	      (get-location binding cenv name usual-variable-type)))
	cont)))))

; Conditional

(define-compilator 'if syntax-type
  (lambda (node cenv depth cont)
    (let ((exp (node-form node))
	  (alt-label (make-label))
	  (join-label (make-label)))
      (sequentially
       ;; Test
       (compile (cadr exp) cenv depth (fall-through-cont node 1))
       (instruction-using-label (enum op jump-if-false) alt-label)
       ;; Consequent
       (compile (caddr exp) cenv depth cont)
       (if (fall-through-cont? cont)
	   (instruction-using-label (enum op jump) join-label)
	   empty-segment)
       ;; Alternate
       (attach-label alt-label
		     (compile (cadddr exp) cenv depth cont))
       (attach-label join-label
		     empty-segment)))))

(define-compilator 'begin syntax-type
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))
      (compile-begin (cdr exp) cenv depth cont))))

(define compile-begin
  (let ((operator/begin (get-operator 'begin)))
    (lambda (exp-list cenv depth cont)
      (if (null? exp-list)
	  (generate-trap cont "null begin")
	  (let ((dummy
		 (make-node operator/begin ;For debugging database
			    `(begin ,@exp-list))))
	    (let loop ((exp-list exp-list) (i 1))
	      (if (null? (cdr exp-list))
		  (compile (car exp-list) cenv depth cont)
		  (careful-sequentially
		   (compile (car exp-list) cenv depth
			    (ignore-values-cont dummy i))
		   (loop (cdr exp-list) (+ i 1))
		   depth
		   cont))))))))


; Compile a call

(define (compile-call node cenv depth cont)
  (if (node-ref node 'type-error)
      (compile-unknown-call node cenv depth cont)
      (let ((proc-node (classify (car (node-form node)) cenv)))
	(if (and (lambda-node? proc-node)
		 (not (n-ary? (cadr (node-form proc-node)))))
	    (compile-redex proc-node (cdr (node-form node)) cenv depth cont)
	    (let ((new-node (maybe-transform-call proc-node node cenv)))
	      (if (eq? new-node node)
		  (compile-unknown-call node cenv depth cont)
		  (compile new-node cenv depth cont)))))))

(define-compilator 'call #f compile-call)


; A redex is a call of the form ((lambda (x1 ... xn) body ...) e1 ... en).

(define lambda-node? (node-predicate 'lambda))

(define (compile-redex proc-node args cenv depth cont)
  (let* ((proc-exp (node-form proc-node))
	 (formals (cadr proc-exp))
	 (body (cddr proc-exp)))
    (cond ((not (= (length formals)
		   (length args)))
	   (generate-trap cont
			  "wrong number of arguments"
			  (cons (schemify proc-node cenv)
				(map (lambda (arg)
				       (schemify arg cenv))
				     args))))
	  ((null? formals)
	   (compile-body body cenv depth cont)) ;+++
	  (else
	   (maybe-push-continuation
	    (sequentially 
	     (push-all-with-names args formals cenv 0)
	     (compile-lambda-code formals body cenv (cont-name cont)))
	    depth
	    cont)))))

; Compile a call to a computed procedure.

(define (compile-unknown-call node cenv depth cont)
  (let ((exp (node-form node)))
    (let ((call (sequentially (push-arguments node cenv 0)
			      (compile (car exp)
				       cenv
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
		      (note-source-code (cont-source-info cont)
					code)
		      (attach-label label
				    (cont-segment cont))))))

; Continuation is implicitly fall-through.

(define (push-arguments node cenv depth)
  (let recur ((args (cdr (node-form node))) (depth depth) (i 1))
    (if (null? args)
	empty-segment
	(sequentially (compile (car args) cenv depth
			       (fall-through-cont node i))
		      (instruction (enum op push))
		      (recur (cdr args) (+ depth 1) (+ i 1))))))

(define (push-all-with-names exp-list names cenv depth)
  (if (null? exp-list)
      empty-segment
      (sequentially (compile (car exp-list)
			     cenv depth
			     (named-cont (car names)))
		    (instruction (enum op push))
                    (push-all-with-names (cdr exp-list)
					 (cdr names)
					 cenv
					 (+ depth 1)))))
     
; OK, now that you've got all that under your belt, here's LAMBDA.

(define-compilator 'lambda syntax-type
  (lambda (node cenv depth cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (deliver-value
       (instruction-with-template (enum op closure)
				  (compile-lambda exp
						  cenv
						  ;; Hack for constructors.
						  ;; Cf. disclose method
						  ;; (if name #t #f)
						  #f)
				  name)
       cont))))

(define (compile-lambda exp cenv body-name)
  (let* ((formals (cadr exp))
	 (nargs (number-of-required-args formals))
	 (fast-protocol? (and (<= nargs maximum-stack-args)
			      (not (n-ary? formals)))))
    (sequentially
     ;; Insert protocol
     (cond (fast-protocol?
	    (instruction (enum op protocol) nargs))
	   ((<= nargs available-stack-space)
	    (instruction (enum op protocol)
			 (if (n-ary? formals)
			     two-byte-nargs+list-protocol
			     two-byte-nargs-protocol)
			 (high-byte nargs)
			 (low-byte nargs)))
	   (else
	    (error "compiler bug: too many formals"
		   (schemify exp))))
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
       (let* ((nargs (number-of-required-args formals))
	      (nargs (if (n-ary? formals)
			 (+ nargs 1)
			 nargs)))
	 (instruction (enum op make-env)
		      (high-byte nargs)
		      (low-byte nargs)))
       (let* ((vars (normalize-formals formals))
	      (cenv (bind-vars (reverse vars) cenv)))
	 (note-environment
	  vars
	  (compile-body body
			cenv
			0
			(return-cont name)))))))

; Same as the above, except with a flat instead of nested environment.
; (flat-lambda formals free-vars body)

(define-compilator 'flat-lambda syntax-type
  (lambda (node cenv depth cont)
    (let* ((exp (node-form node))
	   (name (cont-name cont))
	   (free (caddr exp))
	   (free-names (map node-form free))
	   (reverse-var-locs (lookup-variables free cenv))
	   (template (compile-lambda `(lambda ,(cadr exp)
					. ,(cdddr exp))
				     (bind-flat-vars free-names cenv)
				     #f)))
      (deliver-value
       (if (null? free)   ; not an optimization, but a hack around the
	   (sequentially  ; somewhat odd assembler interface for templates
	    (instruction-with-template (enum op literal) template name)
	    (instruction (enum op push))
	    (instruction (enum op false))
	    (instruction (enum op make-stored-object) 2 (enum stob closure)))
	   (sequentially
	    (instruction (enum op flat-closure) (length free))
	    (apply instruction (reverse (cdr reverse-var-locs)))
	    (note-environment (reverse free-names)
			      (instruction-with-template
			       (car reverse-var-locs)
			       template
			       name))))
       cont))))

(define (lookup-variables vars cenv)
  (let loop ((vars vars) (locs '()))
    (if (null? vars)
        locs
        (let* ((name (node-form (car vars)))
	       (binding (lookup cenv name)))
          (if (and (binding? binding)
		   (pair? (binding-place binding)))
	      (let* ((level+over (binding-place binding))
		     (back (- (environment-level cenv)
			      (car level+over)))
	             (over (cdr level+over)))
		(loop (cdr vars) (cons over (cons back locs))))
              (error "variable in flat-lambda list is not local" (car vars)))))))

(define compile-letrec
  (let ((operator/lambda     (get-operator 'lambda syntax-type))
	(operator/set!	     (get-operator 'set!   syntax-type))
	(operator/call	     (get-operator 'call))
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
				     ,(make-node
				       operator/call
				       `(,(make-node operator/lambda
						     `(lambda () ,@body))))))
		       (map (lambda (spec)
			      (make-node operator/unassigned
					 `(unassigned)))
			    specs)
		       cenv depth cont)))))

(define-compilator 'letrec syntax-type compile-letrec)

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
; A compile-time continuation is a pair (segment . source-info).
; Segment is one of the following:
;   a return instruction - invoke the current full continuation.
;   empty-segment - fall through to subsequent instructions.
;   an ignore-values instruction - ignore values, then fall through.
; Source-info is one of:
;   #f - we don't know anything
;   symbol - value delivered to subsequent instructions will be assigned to
;     a variable with this name.  If the value being assigned is a lambda, we
;     can give that lambda that name.
;   (i . node) - the value being computed is the i'th subexpression of the node.

; Need to add:
; continuation-stack-size
; maximum-stack-cell
; Doing a push requires checking the current stack size against the maximum
; stack size.
; Or, just have a max stack depth cell and leave it up to its creator to check
; it against the last one.
; Ops that need to check are those that push: op/push, make-continuation,
; make-environment, etc.  Check in the VM.  Doesn't look too bad.

(define (make-cont seg source-info) (cons seg source-info))
(define cont-segment car)
(define cont-source-info cdr)

; We could probably be able to optimize jumps to jumps.
;(define (make-jump-cont label cont)
;  (if (fall-through-cont? cont)
;      (make-cont label (cont-name cont))
;      cont))

(define return-cont-segment (instruction (enum op return)))

(define (return-cont name)
  (make-cont return-cont-segment name))

(define (return-cont? cont)
  (eq? (cont-segment cont) return-cont-segment))

; Fall through into next instruction while compiling the I'th part of NODE.

(define (fall-through-cont node i)
  (make-cont empty-segment (cons i node)))

(define (fall-through-cont? cont)
  (not (return-cont? cont)))

; Ignore return value, then fall through

(define ignore-values-segment
  (instruction (enum op ignore-values)))

(define (ignore-values-cont node i)
  (make-cont ignore-values-segment (cons i node)))

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
  (really-bind-vars names cenv 1))

(define (bind-flat-vars names cenv)
  (really-bind-vars names cenv 0))

(define (really-bind-vars names cenv start)
  (let ((level (+ (environment-level cenv) 1)))
    (lambda (name)
      (if (eq? name funny-name/lexical-level)
	  level
	  (let loop ((over start) (names names))
	    (cond ((null? names)
		   (lookup cenv name))
		  ((eq? name (car names))
		   (make-binding usual-variable-type (cons level over) #f))
		  (else (loop (+ over 1) (cdr names)))))))))

(define (initial-cenv cenv)
  (bind1 funny-name/lexical-level -1 cenv))

(define (environment-level cenv)
  (lookup cenv funny-name/lexical-level))

(define funny-name/lexical-level (string->symbol "Lexical nesting level"))

; Find lookup result that was cached by classifier

(define (name-node-binding node cenv)
  (or (node-ref node 'binding)
      (node-form node)))  ; = (lookup cenv (node-form node))


; --------------------
; Utilities

; Produce something for source code that contains a compile-time error.

(define (generate-trap cont . stuff)
  (apply warn stuff)
  (sequentially (instruction-with-literal (enum op literal)
					  (cons 'error stuff))
		(deliver-value (instruction (enum op trap))
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
	  (sequentially (shrink-segment seg1 (fall-through-cont #f #f))
			seg2)
	  (sequentially seg1
			(shrink-segment seg2 cont)))
      (sequentially seg1 seg2)))

(define large-segment-size (* byte-limit 2))

(define (shrink-segment seg cont)
  (maybe-push-continuation
   (sequentially (instruction-with-template
		  (enum op closure)
		  (sequentially
		   (instruction (enum op protocol) 0)
		   (if (return-cont? cont)
		       seg
		       (sequentially seg
				     (instruction (enum op return)))))
		  #f)
		 (instruction (enum op call) 0))
   0
   cont))

; --------------------
; Type checking.  This gets called on all nodes.

(define (type-check node cenv)
  (if *type-check?*
      (let ((form (node-form node)))
	(if (pair? form)
	    (let ((proc-node (car form)))
	      (if (node? proc-node)
		  (let ((proc-type (node-type proc-node cenv)))
		    (cond ((procedure-type? proc-type)
			   (if (restrictive? proc-type)
			       (let* ((args (if (eq? *type-check?* 'heavy)
						(map (lambda (exp)
						       (classify exp cenv)) 
						     (cdr form))
						(cdr form)))
				      (args-type (make-some-values-type
						  (map (lambda (arg)
							 (meet-type
							  (node-type arg cenv)
							  value-type))
						       args)))
				      (node (make-similar-node node
							       (cons proc-node
								     args))))
				 (if (not (meet? args-type
						 (procedure-type-domain proc-type)))
				     (diagnose-call-error node proc-type cenv))
				 node)
			       node))
			  ((not (meet? proc-type any-procedure-type))
			   ;; Could also check args for one-valuedness.
			   (let ((message "non-procedure in operator position"))
			     (warn message
				   (schemify node cenv)
				   `(procedure: ,proc-type))
			     (node-set! node 'type-error message))
			   node)
			  (else node)))
		  node))
	    node))
      node))

(define (set-type-check?! check?)
  (set! *type-check?* check?))

(define *type-check?* 'heavy)


(define (diagnose-call-error node proc-type cenv)
  (let ((message
	 (cond ((not (fixed-arity-procedure-type? proc-type))
		"invalid arguments")
	       ((= (procedure-type-arity proc-type)
		   (length (cdr (node-form node))))
		"argument type error")
	       (else
		"wrong number of arguments"))))
    (warn message
	  (schemify node cenv)
	  `(procedure wants:
		      ,(rail-type->sexp (procedure-type-domain proc-type)
					#f))
	  `(arguments are: ,(map (lambda (arg)
				   (type->sexp (node-type arg cenv) #t))
				 (cdr (node-form node)))))
    (node-set! node 'type-error message)))


; Type system loophole

(define-compilator 'loophole syntax-type
  (lambda (node cenv depth cont)
    (compile (caddr (node-form node)) cenv depth cont)))
