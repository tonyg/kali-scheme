; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
	     (compile-lambda-code formals body level (cont-name cont)))
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
		      (attach-label label
				    (cont-segment cont))))))

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
     
; OK, now that you've got all that under your belt, here's LAMBDA.

(define-compilator 'lambda syntax-type
  (lambda (node level depth cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (deliver-value
       (sequentially
	 (instruction (enum op closure))
	 (template (compile-lambda exp level #f)
		   (if (name? name)
		       (name->symbol name)
		       #f))
	 (instruction 0)) ; last byte of closure instruction, 0 means use
                          ; *env* for environment
       cont))))

(define (compile-lambda exp level body-name)
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
     (compile-lambda-code formals (caddr exp) level body-name))))

; name isn't the name of the procedure, it's the name to be given to
; the value that the procedure will return.

(define (compile-lambda-code formals body level name)
  (if (null? formals)		;+++ Don't make null environment
      (compile body level 0 (return-cont name))
      ;; (if (node-ref node 'no-inferior-lambdas) ...)
      (sequentially
       (let* ((nargs (number-of-required-args formals))
	      (nargs (if (n-ary? formals)
			 (+ nargs 1)
			 nargs)))
	 (instruction (enum op make-env)
		      (high-byte nargs)
		      (low-byte nargs)))
       (let ((vars (normalize-formals formals))
	     (level (+ level 1)))
	 (set-lexical-offsets! (reverse vars) level)
	 (note-environment
	  (map name-node->symbol vars)
	  (compile body level 0 (return-cont name)))))))

(define (name-node->symbol node)
  (let ((form (node-form node)))
    (cond ((name? form)
	   (name->symbol form))
	  ((symbol? form)
	   form)
	  (else
	   #f))))

; Give each name node in NAMES a binding record that has the names lexical
; level and offset.

(define (set-lexical-offsets! names level)
  (let loop ((over 1) (names names))
    (if (not (null? names))
	(begin
	  (node-set! (car names) 
		     'binding
		     (cons level over))
	  (loop (+ over 1) (cdr names))))))

(define-compilator 'flat-lambda syntax-type
  (lambda (node level depth cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (let ((vars (cadr exp))
	    (free (caddr exp))
 	    (body (cadddr exp)))
 	(deliver-value (compile-flat-lambda name vars body free level)
 		       cont)))))
 
; The MAKE-FLAT-ENV instruction is designed to allow us to make nested flat
; environments (i.e. flat environments consisting of a linked chain of vectors)
; but this code doesn't generate them.  The nested environments would avoid
; the need for offsets larger than a byte.  The current code cannot handle
; large environments.
; When we're done we have to restore the old locations of the free variables.

(define (compile-flat-lambda name vars body free level)
  (let* ((alist (sort-list (get-variables-offsets free level)
			   (lambda (p1 p2)
			     (< (cadr p1)
				(cadr p2)))))
	 (free (map car alist))
	 (old-locations (map name-node-binding free)))
    (set-lexical-offsets! free 0)  ; 0 is the level
    (let ((code (sequentially
		 (instruction (enum op false)) ; either the super env or the env
		 (if (null? free)
		     empty-segment
		     (apply instruction (enum op make-flat-env)
			    1   ; add in *val*
			    (+ (length free) 1)
			    (variable-env-data (map cdr alist))))
		 (instruction (enum op closure))
		 (note-environment (reverse (map node-form free))
				   (template (compile-lambda `(lambda ,vars
								,body)
							     0
							     #f)
					     (if (name? name)
						 (name->symbol name)
						 #f)))
		 (instruction 1)))) ; last byte of closure instruction, 1 means
                                    ; use *val* as environment, instead of *env*
      (for-each (lambda (node location)
		  (node-set! node 'binding location))
		free
		old-locations)
      code)))

; Looks up VARS in CENV and returns an alist of (<name> . (<level> <over>))
; pairs.

(define (get-variables-offsets vars level)
  (let loop ((vars vars) (locs '()))
    (if (null? vars)
	locs
	(let ((binding (name-node-binding (car vars))))
	  (if (pair? binding)
	      (let ((back (- level (car binding)))
		    (over (cdr binding)))
		(if (< byte-limit over)
		    (error "lexical environment limit exceeded; complain to implementors"))
		(loop (cdr vars)
		      (cons (cons (car vars)
				  (cons back over))
			    locs)))
	      (error "variable in flat-lambda list is not local"
		     (car vars)))))))

; Addresses is a list of (level . over) pairs, sorted by level.
; This returns the reverse of the following data:
;   <back for level>
;   <number of variables from this level>
;   <over of 1st var> ...
;   <back for next level>
;   ...
; If a <back> is too large we insert some empty levels.

(define (variable-env-data addresses)
  (let level-loop ((addresses addresses) (last-level 0) (data '()))
    (if (null? addresses)
	(reverse data)
	(let ((level (caar addresses)))
	  (let loop ((addresses addresses) (overs '()))
	    (if (or (null? addresses)
		    (not (= level (caar addresses))))
		(level-loop addresses
			    level
			    (append overs
				    (list (length overs))
				    (let loop ((delta (- level last-level))
					       (back '()))
				      (if (<= delta byte-limit)
					  (cons delta back)
					  (loop (- delta byte-limit)
						`(0 ,byte-limit . ,back))))
				    data))
		(loop (cdr addresses)
		      (cons (cdar addresses) overs))))))))
	  
; We should probably just use the sort from big-scheme.

(define (sort-list xs less?)
  (letrec ((insert (lambda (x xs)
		     (if (null? xs)
			 (list x)
			 (if (less? (car xs) x)
			     (cons (car xs)
				   (insert x (cdr xs)))
			     (cons x xs))))))
    (let sort ((xs xs))    
      (if (null? xs)
	  '()
	  (insert (car xs)
		  (sort (cdr xs)))))))

; LETREC.

(define-compilator 'letrec syntax-type
  (lambda (node level depth cont)
    ;; (if (node-ref node 'pure-letrec) ...)
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
	      (compile-lambda-code (map car specs) body level (cont-name cont)))
	    depth
	    cont)))))

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

(define lambda-node?  (node-predicate 'lambda syntax-type))
(define name-node?    (node-predicate 'name 'leaf))
(define literal-node? (node-predicate 'literal 'leaf))

(define operator/lambda     (get-operator 'lambda syntax-type))
(define operator/set!	    (get-operator 'set!   syntax-type))
(define operator/call	    (get-operator 'call   'internal))
(define operator/begin      (get-operator 'begin  syntax-type))
