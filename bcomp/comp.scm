; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file comp.scm.

;;;; The byte-code compiler

; This is a two-phase compiler.  The first phase does macro expansion,
; variable resolution, and instruction selection, and computes the
; necessary size for the code vector.  The second phase (assembly) then
; creates the code vector, "template" (literals vector), and debugging
; data.

; The output of the first phase (the COMPILE- and INSTRUCTION-
; routines) and the input to the second phase (SEGMENT->TEMPLATE) is a
; "segment."  A segment is a pair (size . proc) where size is the size
; of the code segment in bytes, and proc is a procedure that during
; phase 2 will store the appropriate bytes into the code vector.

; A "cenv" is a compile-time representation of what the lexical
; variable environment will look like at run time.

; Optimizations are marked with +++, and may be flushed if desired.


; Main dispatch for compiling a single expression.

(define (compile exp cenv depth cont)
  (cond ((name? exp)
         (compile-variable exp cenv depth cont))
	((pair? exp)
	 (cond ((name? (car exp))
		(let ((op (clookup cenv (car exp))))
		  (cond ((operator? op)
			 ((get-compilator op) exp cenv depth cont))
			((transform? op)
			 (compile (transform op exp (lambda (name)
						      (probe-cenv cenv name)))
				  cenv depth cont))
			(else
			 (compile-unknown-call exp cenv depth cont)))))
	       ((operator? (car exp))  ;Provision for 2-pass compiler
	        ((get-compilator (car exp)) exp cenv depth cont))
	       (else
		(compile-call exp cenv depth cont))))
        ((literal? exp)
         (compile-literal exp depth cont))
        ((location? exp)  ;Provision for 2-pass compiler, etc. (?)
         (compile-location exp depth cont))
        (else
	 (generate-trap cont "invalid expression" exp))))

(define (compile-variable exp cenv depth cont)
  (let ((den (clookup-variable cenv exp)))
    (if (env-ref? den)
	(let ((back (env-ref-back den cenv))
	      (over (env-ref-over den)))
	  (deliver-value (case back
			   ((0) (instruction op/local0 over)) ;+++
			   ((1) (instruction op/local1 over)) ;+++
			   ((2) (instruction op/local2 over)) ;+++
			   (else (instruction op/local back over)))
			 cont))
	(deliver-value (instruction-with-variable op/global exp den #f)
		       cont))))

(define (compile-literal obj depth cont)
  (if (eq? obj #f)
      ;; +++ kludge for bootstrap from Schemes that don't distinguish #f/()
      (deliver-value (instruction op/false) cont)
      (compile-constant obj depth cont)))


; Defining and using special operators.

(define compilators
  (make-compilator-table (lambda (form cenv depth cont)
			   (generate-trap cont
					  "not valid in expression context"
					  form))))

(define define-compilator (car compilators))
(define get-compilator (cdr compilators))


; The special operators.

(define-compilator 'quote 'syntax
  (lambda (exp cenv depth cont)
    cenv ;ignored
    (let ((obj (cadr exp)))
      (compile-constant obj depth cont))))

(define (compile-constant obj depth cont)
  (if (ignore-values-cont? cont)
      empty-segment			;+++ dead code
      (deliver-value (instruction-with-literal op/literal (desyntaxify obj))
		     cont)))

(define-compilator 'begin 'syntax
  (lambda (exp cenv depth cont)
    (compile-begin (cdr exp) cenv depth cont)))

(define (compile-begin exp-list cenv depth cont)
  (if (null? exp-list)
      (generate-trap cont "null begin")
      (let ((dummy `(begin ,@exp-list))) ;for debugging database
	(let loop ((exp-list exp-list) (i 1))
	  (if (null? (cdr exp-list))
	      (compile (car exp-list) cenv depth cont)
	      (careful-sequentially
		 (compile (car exp-list) cenv depth
			  (ignore-values-cont dummy i))
		 (loop (cdr exp-list) (+ i 1))
		 depth
		 cont))))))

(define-compilator 'set! 'syntax
  (lambda (exp cenv depth cont)
    (let ((name (cadr exp))
          (val (caddr exp)))
      (sequentially
       (compile val cenv depth (named-cont name))
       (deliver-value
	(let ((den (clookup-assigned cenv name)))
	  (cond ((env-ref? den)
		 (instruction op/set-local!
			      (env-ref-back den cenv)
			      (env-ref-over den)))
		(else
		 (instruction-with-variable op/set-global! name den #t))))
	cont)))))

(define-compilator 'if 'syntax
  (lambda (exp cenv depth cont)
    (let* ((alt-segment
            (if (null? (cdddr exp))
		(deliver-value (instruction op/unspecific) cont)
		(compile (cadddr exp) cenv depth cont)))
           (con-segment
            (sequentially
             (compile (caddr exp) cenv depth cont)
             (if (fall-through-cont? cont)
                 (instruction-with-offset op/jump
					  (segment-size alt-segment))
                 empty-segment))))  ;+++ Eliminate dead code.
      (sequentially
       (compile (cadr exp) cenv depth (fall-through-cont exp 1))
       (instruction-with-offset op/jump-if-false (segment-size con-segment))
       con-segment
       alt-segment))))


; Compile a call

(define (compile-call exp cenv depth cont)
  (let ((proc-exp (car exp))
	(args (cdr exp)))
    (if (and (lambda-expression? proc-exp cenv)
	     (acceptable-arguments? (cadr proc-exp) args))
	(compile-redex proc-exp args cenv depth cont) ;+++ don't cons a closure
	(compile-unknown-call exp cenv depth cont))))

(define (lambda-expression? exp cenv)
  (and (pair? exp)
       (if (name? (car exp))
	   (eq? (clookup cenv (car exp))
		operator/lambda)
	   ;; Hack for 2-pass compiler
	   (eq? (car exp) operator/lambda))))

(define operator/lambda (get-operator 'lambda 'syntax))

(define (acceptable-arguments? formals args)
  (let ((want-nargs (number-of-required-args formals))
	(have-nargs (length args)))
    (if (if (n-ary? formals)
	    (>= have-nargs want-nargs)
	    (= have-nargs want-nargs))
	#t
	(begin (warn "wrong number of arguments in ((lambda ...) ...)"
		     formals args)))))


; A redex is a call of the form ((lambda (x1 ... xn) body ...) e1 ... en),
; otherwise known as a "let".

(define (compile-redex proc-exp args cenv depth cont)
  (let ((formals (cadr proc-exp)))
    (maybe-push-continuation
      (sequentially 
        (push-all-with-names args formals cenv 0)
	(compile-lambda-code formals (cddr proc-exp) cenv (cont-name cont)))
      depth
      cont)))

(define (compile-unknown-call exp cenv depth cont)
  (let ((call (sequentially (push-arguments exp cenv 0)
			    (compile (car exp)
				     cenv
				     (length (cdr exp))
				     (fall-through-cont exp 0))
			    (instruction op/call (length (cdr exp))))))
    (maybe-push-continuation call depth cont)))

(define (maybe-push-continuation code depth cont)
  (if (return-cont? cont) 
      code
      (sequentially (instruction-with-offset&byte op/make-cont
						  (segment-size code)
						  depth)
		    (note-source-code (cont-source-info cont)
				      code)
		    (cont-segment cont))))

; Continuation is implicitly fall-through.

(define (push-arguments exp cenv depth)
  (let recur ((args (cdr exp)) (depth depth) (i 1))
    (if (null? args)
	empty-segment
	(sequentially (compile (car args) cenv depth (fall-through-cont exp i))
		      (instruction op/push)
		      (recur (cdr args) (+ depth 1) (+ i 1))))))

(define (push-all-with-names exp-list names cenv depth)
  (if (null? exp-list)
      empty-segment
      (sequentially (compile (car exp-list)
			     cenv depth
			     (named-cont (car names)))
		    (instruction op/push)
                    (push-all-with-names (cdr exp-list) (cdr names)
					 cenv (+ depth 1)))))

; OK, now that you've got all that under your belt, here's LAMBDA.

(define-compilator 'lambda 'syntax
  (lambda (exp cenv depth cont)
    (deliver-value
      (instruction-with-template op/closure
				 (compile-lambda exp cenv)
				 (cont-name cont))
      cont)))

(define (compile-lambda exp cenv)
  (let* ((formals (cadr exp))
	 (nargs (number-of-required-args formals)))
    (sequentially
     ;; Check number of arguments
     (if (n-ary? formals)
	 (if (pair? formals)
	     (instruction op/check-nargs>= nargs)
	     empty-segment)		;+++ (lambda x ...) needs no check
	 (instruction op/check-nargs= nargs))
     (compile-lambda-code formals (cddr exp) cenv #f))))

; name isn't the name of the procedure, it's the name to be given to
; the value that the procedure will return.

(define (compile-lambda-code formals body cenv name)
  (if (null? formals)
      (compile-body body		;+++ Don't make null environment
		    cenv
		    0
		    (return-cont name))
      (sequentially
       (let ((nargs (number-of-required-args formals)))
	 (if (n-ary? formals)
	     (sequentially
	      (instruction op/make-rest-list nargs)
	      (instruction op/push)
	      (instruction op/make-env (+ nargs 1)))
	     (instruction op/make-env nargs)))
       (let* ((vars (normalize-formals formals))
	      (cenv (bind-vars vars cenv)))
	 (note-environment
	  vars
	  (compile-body body
			cenv
			0
			(return-cont name)))))))


; Utility to produce something for code that contained a compile-time
; error.

(define (generate-trap cont . stuff)
  (apply warn stuff)
  (sequentially (instruction-with-literal op/literal (cons 'error stuff))
		(deliver-value (instruction op/trap)
			       cont)))

; Continuations
;
; A (compile-time) continuation is a pair (jump . name).  Jump is a
; segment; this will be either the empty segment, meaning that we will
; fall into subsequent instructions, or a short instruction that is
; guaranteed not to fall through.
;
; If there is a name, then the value delivered to subsequent
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
  (if (name? (cont-source-info cont))
      (cont-source-info cont)
      #f))



; Deal with internal defines (ugh)

(define (compile-body body cenv depth cont)
  (scan-body
      body
      (lambda (name) (probe-cenv cenv name)) ;probe
      (lambda (name) (clookup cenv name))
      (lambda (defs exps)
	(if (null? defs)
	    (compile-begin exps cenv depth cont)
	    ;; *** not hygienic ***
	    (compile `(letrec ,(map cdr defs) ,@exps)
		     cenv depth cont)))))


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


; For two-pass compiler...

(define-compilator 'local 'syntax
  (lambda (exp cenv depth cont)
    (compile-variable (cadr exp) cenv depth cont)))

(define-compilator 'call 'syntax
  (lambda (exp cenv depth cont)
    (compile-call (cdr exp) cenv depth cont)))

; For ##

(define (compile-location den depth cont)
  (deliver-value (instruction-with-literal op/global den)
		 cont))


; --------------------
; Compile-time environments

;        clookup : env * name -> denotation
;
;        denotation = location + operator + transform + package + (back * over)

(define (env-ref? den)  ;Asked of something returned by clookup-variable
  (integer? (car den)))

(define (env-ref-back den cenv) (car den))
(define (env-ref-over den) (cdr den))

(define (bind-vars names cenv)
  (cons  (reverse names) cenv))

; Look up a name in a compile-time environment.
; Four version: for comparison in macros; for operator position; for variable
; reference; and for variable assignment.

(define (probe-cenv cenv name)
  (really-clookup cenv name probe-package))

(define (clookup cenv name)
  (really-clookup cenv name package-lookup))

(define (clookup-variable cenv name)
  (really-clookup cenv name package-check-variable))

(define (clookup-assigned cenv name)
  (really-clookup cenv name package-check-assigned))


(define (really-clookup cenv name p-lookup)
  (let next-rib ((cenv cenv)
		 (back 0))
    (if (pair? cenv)
	(let loop ((rib (car cenv)) (over 1))
	  (cond ((null? rib)
		 (next-rib (cdr cenv) (+ back 1))) ;Not here, try outer env.
		((eq? name (car rib))
		 (cons back over))
		(else (loop (cdr rib) (+ over 1)))))
	(p-lookup cenv name))))



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
