
; Optimizations are marked with +++, and may be flushed if desired.

(define (compile-expression node depth frame cont)
  (compile node depth frame cont))

; Main dispatch for compiling a single expression.

(define (compile node depth frame cont)
  (let ((node (type-check node)))
    ((operator-table-ref compilators (node-operator-id node))
     node
     depth
     frame
     cont)))

; Specialists

(define compilators
  (make-operator-table
    (lambda (node depth frame cont)
      (generate-trap depth
		     frame
		     cont
		     "not valid in expression context"
		     (schemify node)))))

(define (define-compilator name type proc)
  (operator-define! compilators name type proc))

;----------------------------------------------------------------

(define-compilator 'literal 'leaf
  (lambda (node depth frame cont)
    (compile-constant (node-form node) depth frame cont)))

(define-compilator 'quote syntax-type
  (lambda (node depth frame cont)
    (compile-constant (cadr (node-form node)) depth frame cont)))

(define (compile-constant obj depth frame cont)
  (deliver-constant-value (cond ((eq? obj #f)
				 ;; +++ hack for bootstrap from Schemes that don't
				 ;; distinguish #f/()
				 (instruction (enum op false)))
				((small-integer? obj)
				 (integer-literal-instruction obj))
				(else
				 (stack-indirect-instruction
				  (template-offset frame depth)
				  (literal->index frame obj))))
			  cont))

(define (small-integer? obj)
  (and (integer? obj)
       (exact? obj)
       (<= 0 (+ obj 128))
       (< (+ obj 128) byte-limit)))

; PreScheme does not have signed bytes so we bias OBJ

(define (integer-literal-instruction obj)
  (instruction (enum op integer-literal)
	       (+ obj 128)))

(define-compilator 'unspecific (proc () unspecific-type)
  (lambda (node depth frame cont)
    (deliver-constant-value (instruction (enum op unspecific))
			    cont)))

(define-compilator 'unassigned (proc () unspecific-type)
  (lambda (node depth frame cont)
    (deliver-constant-value (instruction (enum op unassigned))
			    cont)))

(define (deliver-constant-value segment cont)
  (deliver-value (if (ignore-values-cont? cont)
		     empty-segment
		     segment)
		 cont))

;----------------------------------------------------------------
; Variable reference

(define-compilator 'name 'leaf
  (lambda (node depth frame cont)
    (let* ((binding (name-node-binding node))
	   (name (node-form node)))
      (deliver-value
        (if (pair? binding)
	    (compile-local-name node name binding depth)
	    (let ((offset (template-offset frame depth))
		  (index (binding->index frame binding name value-type)))
	      (instruction (enum op global)
			   (high-byte offset)
			   (low-byte offset)
			   (high-byte index)
			   (low-byte index))))
	cont))))

(define (compile-local-name node name binding depth)
  (let ((stack-offset (index->offset (car binding) depth))
	(rest (cdr binding)))
    (cond ((null? rest)		; in this frame
	   (stack-ref-instruction stack-offset))
	  ((null? (cdr rest))
	   (stack-indirect-instruction stack-offset (car rest)))
	  (else
	   (error "variable has too many indirections" name binding)))))

(define (index->offset index depth)
  (- (- depth 1)
     index))

;----------------------------------------------------------------
; Hacked versions of the above for peephole optimization of pushes.
; This tries to compile NODE with an implicit push instruction preceeding
; (if PRE?) or following (if not PRE?).  The instructions with implicit
; pushes are:
;   push-false
;   push+stack-ref
;   stack-ref+push
;   push+stack-indirect
;   stack-indirect+push
; This codes finds the cases where literals or lexical references can be
; compiled into one of these.

(define (maybe-compile-with-push node depth frame pre?)
  (cond ((literal-node? node)
	 (constant-with-push (node-form node) depth frame pre?))
	((quote-node? node)
	 (constant-with-push (cadr (node-form node)) depth frame pre?))
	((name-node? node)
	 (let ((binding (name-node-binding node)))
	   (if (and (pair? binding)
		    (not (node-ref node 'check-unassigned)))
	       (lexical-ref-with-push binding depth pre?)
	       #f)))
	(else
	 #f)))

(define (constant-with-push obj depth frame pre?)
  (cond ((eq? obj #f)
	 (if pre?
	     #f
	     (instruction (enum op push-false))))
	((small-integer? obj)
	 ; PreScheme does not have signed bytes so we bias OBJ
	 (instruction (if pre?
			  (enum op push+integer-literal)
			  (enum op integer-literal+push))
		      (+ obj 128)))
	(else
	 (push+stack-indirect-instruction (template-offset frame depth)
					  (literal->index frame obj)
					  pre?))))

(define (lexical-ref-with-push binding depth pre?)
  (let ((stack-offset (index->offset (car binding) depth))
	(rest (cdr binding)))
    (cond ((null? rest)		; in this frame
	   (push+stack-ref-instruction stack-offset pre?))
	  ((null? (cdr rest))
	   (push+stack-indirect-instruction stack-offset (car rest) pre?))
	  (else
	   #f))))

(define (push+stack-ref-instruction index pre?)
  (if (< index byte-limit)
      (instruction (if pre?
		       (enum op push+stack-ref)
		       (enum op stack-ref+push))
		   index)
      #f))

(define (push+stack-indirect-instruction index-in-stack index pre?)
  (if (and (< index byte-limit)
	   (< index-in-stack byte-limit))
      (instruction (if pre?
		       (enum op push+stack-indirect)
		       (enum op stack-indirect+push))
		   index-in-stack
		   index)
      #f))

;----------------------------------------------------------------
; Assignment

(define-compilator 'set! syntax-type
  (lambda (node depth frame cont)
    (let* ((exp (node-form node))
	   (lhs-node (cadr exp))
	   (name (node-form lhs-node))
	   (binding (name-node-binding lhs-node)))
      (sequentially
        (compile (caddr exp) depth frame (named-cont name))
	(deliver-value
	  (if (pair? binding)
	      (let ((stack-offset (index->offset (car binding) depth))
		    (rest (cdr binding)))
		(if (null? rest)		; in this frame
		    (stack-set!-instruction stack-offset)
		    (error "SET! on a closed-over variable" name)))
	      (let ((offset (template-offset frame depth))
		    (index (binding->index frame
					   binding
					   name
					   usual-variable-type)))
		(instruction (enum op set-global!)
			     (high-byte offset)
			     (low-byte offset)
			     (high-byte index)
			     (low-byte index))))
	  cont)))))

;----------------------------------------------------------------
; IF and BEGIN

(define-compilator 'if syntax-type
  (lambda (node depth frame cont)
    (let ((exp (node-form node))
	  (alt-label (make-label))
	  (join-label (make-label)))
      (sequentially
       ;; Test
       (compile (cadr exp) depth frame (fall-through-cont node 1))
       (instruction-using-label (enum op jump-if-false) alt-label)
       ;; Consequent
       (compile (caddr exp) depth frame cont)
       (if (fall-through-cont? cont)
	   (instruction-using-label (enum op jump) join-label)
	   empty-segment)
       ;; Alternate
       (attach-label alt-label
		     (compile (cadddr exp) depth frame cont))
       (attach-label join-label
		     empty-segment)))))

(define-compilator 'begin syntax-type
  (lambda (node depth frame cont)
    (let ((exp-list (cdr (node-form node))))
      (if (null? exp-list)
	  (generate-trap depth frame cont "null begin")
	  (let ((dummy
		 (make-node operator/begin ;For debugging database
			    `(begin ,@exp-list))))
	    (let loop ((exp-list exp-list) (i 1))
	      (if (null? (cdr exp-list))
		  (compile (car exp-list) depth frame cont)
		  (sequentially
		    (compile (car exp-list)
			     depth
			     frame
			     (ignore-values-cont dummy i))
		    (loop (cdr exp-list) (+ i 1))))))))))

;----------------------------------------------------------------
; Calls

(define (compile-call node depth frame cont)
  (if (node-ref node 'type-error)
      (compile-unknown-call node depth frame cont)
      (let ((proc-node (car (node-form node))))
	(cond ((name-node? proc-node)
	       (compile-name-call node proc-node depth frame cont))
	      ((and (lambda-node? proc-node)
		    (not (n-ary? (cadr (node-form proc-node)))))
	       (compile-redex proc-node (cdr (node-form node))
			      depth
			      frame
			      cont))
 	      ((and (literal-node? proc-node)
 		    (primop? (node-form proc-node)))
 	       (let ((primop (node-form proc-node)))
 		 (if (primop-compilator primop)
 		     ((primop-compilator primop) node depth frame cont)
 		     (error "compiler bug: primop has no compilator"
 			    primop
 			    (schemify node)))))
	      (else
	       (compile-unknown-call node depth frame cont))))))

(define-compilator 'call 'internal compile-call)

(define (compile-name-call node proc-node depth frame cont)
  (let ((binding (name-node-binding proc-node)))
    (if (binding? binding)
	(let ((static (binding-static binding)))
	  (cond ((primop? static)
		 (if (primop-compilator static)
		     ((primop-compilator static) node depth frame cont)
		     (compile-unknown-call node depth frame cont)))
		((transform? static)
		 (let* ((form (node-form node))
			(new (apply-inline-transform static
						     form
						     (node-form proc-node))))
		   (if (eq? new form)
		       (compile-unknown-call node depth frame cont)
		       (compile new depth frame cont))))
		(else
		 (compile-unknown-call node depth frame cont))))
	(compile-unknown-call node depth frame cont))))

; Compile a call to a computed procedure.

(define (compile-unknown-call node depth frame cont)
  (receive (before depth label after)
      (maybe-push-continuation depth frame cont node)
    (let* ((exp (node-form node))
	   (nargs (length (cdr exp))))
      (sequentially before
		    (push-arguments node depth frame)
		    (compile (car exp)
			     (+ depth nargs)
			     frame
			     (fall-through-cont node 0))
		    (call-instruction nargs label)
		    after))))

; A redex is a call of the form ((lambda (x1 ... xn) body ...) e1 ... en).

(define (compile-redex proc-node args depth frame cont)
  (let* ((proc-exp (node-form proc-node))
	 (formals (cadr proc-exp))
	 (body (caddr proc-exp)))
    (cond ((not (= (length formals)
		   (length args)))
	   (generate-trap depth
			  frame
			  cont
			  "wrong number of arguments"
			  (cons (schemify proc-node)
				(map schemify args))))
	  ((null? formals)
	   (compile body depth frame cont)) ;+++
	  (else
	   (let* ((nargs (length args))
		  (body-depth (+ depth nargs)))
	     (set-frame-locations! formals body-depth)
	     (sequentially
	       (push-all-with-names args formals depth frame)
	       (compile-inline-body nargs
				    (map name-node->symbol formals)
				    body
				    body-depth
				    frame
				    cont)))))))

(define (compile-inline-body nargs formals body depth frame cont)
  (sequentially 
    (note-environment
      formals
      (- depth nargs)
      (compile body depth frame cont))
    (if (return-cont? cont)
	empty-segment
	(deliver-value (instruction (enum op pop-n)
				    (high-byte nargs)
				    (low-byte nargs))
		       cont))))

;----------------------------------------------------------------
; (PURE-LETREC ((<var> <val>) ...) (<free var> ...) <body>)
; These are LETREC's where the values are all LAMBDA's.  They are produced by
; opt/flatten.scm. 

(define-compilator 'pure-letrec syntax-type
  (lambda (node depth frame cont)
    (let* ((exp (node-form node))
           (specs (cadr exp))
           (free-vars (caddr exp))
           (body (cadddr exp))
           (count (length specs))
	   (old-locations (map name-node-binding free-vars)))
      (receive (env-code ignore-free-vars-in-order)
          (compile-recursive-environment free-vars
					 depth
					 (template-offset frame depth)
					 (letrec-template-maker specs frame))
	(for-each (lambda (node location)
		    (node-set! node 'binding location))
		  free-vars
		  old-locations)
	(set-lexical-offsets! (map car specs) depth)
	(depth-check! frame (+ depth 1))
	(sequentially
	  env-code
	  (instruction (enum op push))
	  (compile-inline-body 1
			       (list (map name-node->symbol
					  (append (map car specs)
						  free-vars)))
			       body
			       (+ depth 1)
			       frame
			       cont))))))

; After getting the free variable list (to pass to NOTE-ENVIRONMENT) this
; compiles the values in SPECS, all of which are lambdas.  It returns the
; template indexes of the resulting templates.

(define (letrec-template-maker specs frame)
  (lambda (free-vars-in-order)
    (let ((all-vars (append (map car specs)
			    free-vars-in-order)))
      (map (lambda (spec)
	     (receive (proc-code proc-frame)
		 (compile-lambda (unflatten-form (cadr spec))
				 all-vars
				 (node-form (car spec))
				 #f
				 frame)
	       (literal->index frame
			       (segment->template proc-code proc-frame))))
	   specs))))

;----------------------------------------------------------------
; We don't pass the incremented depth to MAKE-RETURN-POINT because the
; return-pointer is not included in a continuation's size.
;
; Returns before-segment new-depth label after-segment.
;
; In all of these NODE is the expression whose value will be returned to the
; continuation.  It is saved for debugging assistance.

(define (maybe-push-continuation depth frame cont node)
  (if (return-cont? cont)
      (values empty-segment depth #f empty-segment)
      (push-continuation depth frame cont node)))

(define (push-continuation depth frame cont node)
  (if (return-cont? cont)
      (error "making a return point in tail position"))
  (let ((protocol (continuation-protocol (if (ignore-values-cont? cont)
                                             0
                                             1)
                                         (if (ignore-values-cont? cont)
                                             #t
                                             #f))))
    (really-push-continuation depth frame protocol node cont)))

(define (push-continuation-no-protocol depth frame node cont)
  (really-push-continuation depth frame empty-segment node cont))

(define (really-push-continuation depth frame protocol node cont)
  (depth-check! frame (+ depth 1))
  (let ((label (make-label))
	(protocol (if (keep-source-code?)
		      (note-source-code (fixup-source node
						      (cont-source-info cont))
					protocol
					frame)
		      protocol)))
    (values (instruction (enum op push-false))
	    (+ depth 1)
	    label
	    (sequentially (continuation-data #f depth (template-offset frame depth))
			  (attach-label label protocol)))))

(define (fixup-source node destination)
  ;; Abbreviate this somehow?
  (if node
      (if (pair? destination)
	  (cons (schemify node)
		(cons (car destination)
		      (schemify (cdr destination))))
	  (list (schemify node)))
      #f))

;----------------------------------------------------------------
; Pushing arguments.
;
; This is a mess because we try to merge push instructions with other common
; ones.  There are three entry points:

(define (push-arguments node depth frame)
  (let ((args (cdr (node-form node))))
    (if (null? args)
	empty-segment
	(really-push-arguments args depth frame node #t))))

(define (push-all-but-last nodes depth frame debug-info)
  (really-push-arguments nodes depth frame debug-info #f))

(define (push-all-with-names nodes names depth frame)
  (really-push-arguments nodes depth frame names #t))

(define (push-argument node index depth frame)
  (depth-check! frame (+ depth 1))
  (receive (code pushed?)
      (compile-argument (list-ref (node-form node) (+ index 1))
			depth
			frame
			node
			(+ index 1)
			#f
			#t)
    (if pushed?
	code
	(sequentially code push-instruction))))

; The main loop.

(define (really-push-arguments nodes depth frame debug-info push-last?)
  (let recur ((args nodes) (i 1) (pre-push? #f))
    (receive (arg-code pushed?)
	(compile-argument (car args) (+ depth (- i 1)) frame debug-info i
			  pre-push?
			  (if (null? (cdr args))
			      push-last?
			      #t))
      (cond ((null? (cdr args))
	     (depth-check! frame (+ depth (if push-last? i (- i 1))))
	     (if (and push-last? (not pushed?))
		 (sequentially arg-code push-instruction)
		 arg-code))
	    (else
	     (sequentially arg-code
			   (recur (cdr args) (+ i 1) (not pushed?))))))))

(define (compile-argument node depth frame debug-info index before? after?)
  (cond ((and before?			;+++
	      (maybe-compile-with-push node depth frame #t))
	 => (lambda (code)
	      (values code #f)))
	((and after?			;+++
	      (maybe-compile-with-push node depth frame #f))
	 => (lambda (code)
	      (values (pre-push code before?)
		      #t)))
	(else
	 (values (pre-push (compile node
				    depth
				    frame
				    (if (pair? debug-info)
					(named-cont (node-form (car debug-info)))
					(fall-through-cont debug-info index)))
			   before?)
		 #f))))

(define (pre-push code do-it?)
  (if do-it?
      (sequentially push-instruction code)
      code))

(define push-instruction
  (instruction (enum op push)))

;----------------------------------------------------------------
; We have two sizes of these because the big size is very rare and
; signficantly slower (because the argument count cannot be encoded in
; the protocol).

(define (call-instruction nargs label)
  (if label
      (if (> nargs maximum-stack-args)				;+++
	  (instruction-using-label (enum op big-call)
				   label
				   (high-byte nargs)
				   (low-byte nargs))
	  (instruction-using-label (enum op call)
				   label
				   nargs))
      (if (> nargs maximum-stack-args)				;+++
	  (instruction (enum op big-call)
		       0
		       0
		       (high-byte nargs)
		       (low-byte nargs))
	  (instruction (enum op tail-call)
		       nargs))))
  
(define (stack-ref-instruction index)
  (if (>= index byte-limit)					;+++
      (instruction (enum op big-stack-ref)
		   (high-byte index)
		   (low-byte index))
      (instruction (enum op stack-ref)
		   index)))

(define (stack-set!-instruction index)
  (if (>= index byte-limit)					;+++
      (instruction (enum op big-stack-set!)
		   (high-byte index)
		   (low-byte index))
      (instruction (enum op stack-set!)
		   index)))

(define (stack-indirect-instruction index-in-stack index)
  (if (and (< index byte-limit)					;+++
	   (< index-in-stack byte-limit))
      (instruction (enum op stack-indirect)
		   index-in-stack
		   index)
      (instruction (enum op big-stack-indirect)
		   (high-byte index-in-stack)
		   (low-byte index-in-stack)
		   (high-byte index)
		   (low-byte index))))

;----------------------------------------------------------------
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

(define (plain-fall-through-cont)
  (make-cont 'fall-through #f))

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

;----------------------------------------------------------------
; Utilities

; Find lookup result that was cached by classifier

(define (name-node-binding node)
  (or (node-ref node 'binding)
      (node-form node)))

; Produce something for source code that contains a compile-time error.

(define (generate-trap depth frame cont . stuff)
  (apply warn stuff)
  (sequentially
    (stack-indirect-instruction (template-offset frame depth)
				(literal->index frame (cons 'error stuff)))
    (deliver-value (instruction (enum op trap))
		   cont)))

;----------------------------------------------------------------
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
  (lambda (node depth frame cont)
    (compile (caddr (node-form node)) depth frame cont)))

; Node predicates and operators.

(define lambda-node?      (node-predicate 'lambda      syntax-type))
(define flat-lambda-node? (node-predicate 'flat-lambda syntax-type))
(define name-node?        (node-predicate 'name        'leaf))
(define literal-node?     (node-predicate 'literal     'leaf))
(define quote-node?       (node-predicate 'quote       syntax-type))

(define operator/lambda     (get-operator 'lambda syntax-type))
(define operator/set!	    (get-operator 'set!   syntax-type))
(define operator/call	    (get-operator 'call   'internal))
(define operator/begin      (get-operator 'begin  syntax-type))

; Should this be in its own spot?

(define-syntax receive
  (syntax-rules ()
    ((receive ?vars ?producer . ?body)
     (call-with-values (lambda () ?producer)
       (lambda ?vars . ?body)))))
