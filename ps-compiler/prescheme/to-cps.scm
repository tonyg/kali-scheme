; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.

; Convert a byte-code-compiler node into a cps node.

; Entry point.

(define (x->cps node name)
  (receive (value first-call last-lambda)
      (cps node)
    (if first-call
	(bug "(X->CPS ~S) got a non-value" node))
    (maybe-add-name! value name)
    value))

;----------------------------------------------------------------
; (CPS <node>)
;   -> <value> <first-call> <last-lambda>
; <value> is the CPSed value of <node>.  If <node> contains no non-trivial
; calls, <first-call> and <last-lambda> are both #f.  Otherwise they are
; the first of the non-trivial calls and the continuation of the last.

(define (cps node)
  (receive (value first-call last-lambda)
      (real-cps node)
    (let ((value (cond ((not (list? value))
			value)
		       ((or (null? value)
			    (not (null? (cdr value))))
			(bug "value expression did not return one value ~S"
			     (schemify node)))
		       (else
			(car value)))))
      (values value first-call last-lambda))))
    
; Same as above except that <value> is a list of values.

(define (values-cps node)
  (receive (value first-call last-lambda)
      (real-cps node)
    (values (if (list? value)
		value
		(list value))
	    first-call
	    last-lambda)))

(define (real-cps node)
  ((operator-table-ref cps-converters
		       (node-operator-id node))
     node))

(define cps-converters
  (make-operator-table
   (lambda (node id)
     (error "no cps-converter for node ~S" node))))

(define (define-cps-converter name proc)
  (operator-define! cps-converters name #f proc))

;----------------------------------------------------------------
; (TAIL-CPS <node> <continuation-variable>)
;   -> <first-call>

(define (tail-cps node cont-var)
  ((operator-table-ref tail-cps-converters (node-operator-id node))
     node
     cont-var))

(define tail-cps-converters
  (make-operator-table
    (lambda (node cont-var)
      (error "no tail-cps-converter for node ~S" node))))

(define (define-tail-cps-converter name proc)
  (operator-define! tail-cps-converters name #f proc))

; Use PROC in the CPS table and give it a wrapper that makes a return for use
; in the TAIL-CPS table.

(define (define-value-cps-converter name proc)
  (operator-define! cps-converters name #f
		    (lambda (node)
		      (values (proc node) #f #f)))
  (operator-define! tail-cps-converters name #f
		    (lambda (node cont-var)
		      (make-return cont-var (proc node)))))

; El Hacko Grande: we use the name of the CONT-VAR to determine whether
; it is a return or a join point.

(define (join? var)
  (case (variable-name var)
    ((c) #f)
    ((j) #t)
    (else
     (bug "funny continuation variable name ~S" var))))

(define (make-return cont-var value)
  (really-make-return cont-var (list value)))

(define (make-multiple-value-return cont-var values)
  (really-make-return cont-var values))

(define (really-make-return cont-var values)
  (let ((return (make-call-node
		 (get-primop (if (join? cont-var)
				 (enum primop jump)
				 (enum primop unknown-return)))
		 (+ 1 (length values))
		 0)))
    (attach-call-args return (cons (make-reference-node cont-var) values))
    return))

;----------------------------------------------------------------
; Constants are easy.

(define-value-cps-converter 'literal
  (lambda (node)
    (cps-literal (node-form node) node)))

(define-value-cps-converter 'quote
  (lambda (node)
    (cps-literal (cadr (node-form node)) node)))

(define (cps-literal value node)
  (make-literal-node value (node-type node)))

(define-value-cps-converter 'unspecific
  (lambda (node)
    (make-unspecific)))

(define (make-unspecific)
  (make-call-node (get-prescheme-primop 'unspecific) 0 0))

; Used for primitives in non-call position.  The CDR of the form is a
; variable that will be bound to the primitive's closed-compiled value.

(define-value-cps-converter 'primitive
  (lambda (node)
    (make-reference-node (cdr (node-form node)))))

;----------------------------------------------------------------

(define-value-cps-converter 'lambda
  (lambda (node)
    (let ((form (node-form node))
	  (cont-var (make-variable 'c (lambda-node-return-type node)))
	  (vars (map (lambda (name)
		       (let ((var (make-variable (name-node->symbol name)
						 (node-type name))))
			 (node-set! name 'variable var)
			 var))
		     (cadr (node-form node)))))
      (let ((lnode (make-lambda-node 'p 'proc (cons cont-var vars))))
	(attach-body lnode (tail-cps (caddr form) cont-var))
	lnode))))

;----------------------------------------------------------------
; References and SET!

(define-value-cps-converter 'name
  (lambda (node)
    (cond ((node-ref node 'variable)
	   => make-reference-node)    ; eventually have to check for SET!'s
	  ((node-ref node 'binding)
	   => (lambda (binding)
		(let ((var (binding-place binding)))
		  (cond ((not (variable? var))
			 (bug "binding for ~S has funny place ~S" node var))
			((variable-set!? var)
			 (make-global-ref var))
			(else
			 (make-reference-node var))))))
	  (else
	   (bug "name node ~S has neither variable nor binding" node)))))
  
(define (make-global-ref var)
  ((structure-ref node let-nodes)
     ((call (global-ref 0 (* var))))
   call))

; Stolen from form.scm as an expedient.  This needs to be moved to somewhere
; that both FORMS and TO-CPS can see it.

(define (variable-set!? var)
  (memq 'set! (variable-flags var)))

(define-cps-converter 'set!
  (lambda (node)
    (receive (first-call last-lambda)
	(make-global-set! (node-form node))
      (values (make-unspecific) first-call last-lambda))))

(define-tail-cps-converter 'set!
  (lambda (node cont-var)
    (receive (first-call last-lambda)
	(make-global-set! (node-form node))
      (attach-body last-lambda (make-return cont-var (make-unspecific)))
      first-call)))

(define (make-global-set! form)
  (let ((name (cadr form))
	(value (caddr form)))
    (receive (value first-call last-lambda)
	(cps value)
      (maybe-add-name! value name)
      (let ((cont (make-lambda-node 'c 'cont '()))
	    (var (name-node->variable name)))
	((structure-ref node let-nodes)
	 ((call (global-set! 1 cont (* var) value)))
	 (values (splice!->first first-call last-lambda call)
		 cont))))))

(define (name-node->variable name-node)
  (let ((binding (node-ref name-node 'binding)))
    (if (and binding
             (variable? (binding-place binding)))
        (binding-place binding)
	(bug "name node ~S has no variable" name-node))))

;----------------------------------------------------------------
; CALL & GOTO

(define-cps-converter 'call
  (lambda (node)
    (let ((exp (node-form node)))
      (convert-call (car exp) (cdr exp) node))))

; Treat non-tail-recursive GOTO's as normal calls.

(define-cps-converter 'goto
  (lambda (node)
    (let ((exp (node-form node)))
      (user-warning "Ignoring non-tail-recursive GOTO: ~S" (schemify node))
      (convert-call (cadr exp) (cddr exp) node))))

; Dispatch on the procedure.  Do something special with lambdas, primitives,
; primops (in literal nodes).  Everything else is turned into an unknown call.
; Calls to primitives are expanded and then CPS'ed.

(define (convert-call proc args node)
  (cond ((lambda-node? proc)
	 (convert-let (node-form proc) args node))
	((primitive-node? proc)
	 (values-cps (expand-primitive-call proc args node)))
	((and (literal-node? proc)
	      (primop? (node-form proc)))
	 (convert-primop-call (node-form proc) args (node-type node)))
	(else
	 (convert-primop-call (get-primop (enum primop unknown-call))
			      (cons proc    ; add protocol argument
				    (cons (make-literal normal-protocol)
					  args))
			      (node-type node)))))

; Same again, except that for unknown tail-recursive calls we use different
; protocols for CALL and GOTO.

(define-tail-cps-converter 'call
  (lambda (node cont-var)
    (if (join? cont-var)
	(convert-and-add-jump node cont-var)
	(let ((exp (node-form node)))
	  (tail-convert-call (car exp) (cdr exp) node cont-var normal-protocol)))))

(define-tail-cps-converter 'goto
  (lambda (node cont-var)
    (if (join? cont-var)
	(convert-and-add-jump node cont-var)
	(let ((exp (node-form node)))
	  (tail-convert-call (cadr exp) (cddr exp) node cont-var goto-protocol)))))

(define (convert-and-add-jump node join-var)
  (receive (values first-call last-lambda)
      (values-cps node)
    (let ((jump (make-multiple-value-return join-var values)))
      (cond (first-call
	     (attach-body last-lambda jump)
	     first-call)
	    (else
	     jump)))))

(define (tail-convert-call proc args node cont-var protocol)
  (cond ((lambda-node? proc)
	 (convert-tail-let (node-form proc) args node cont-var))
	((primitive-node? proc)
	 (tail-cps (expand-primitive-call proc args node)
		   cont-var))
	((and (literal-node? proc)
	      (primop? (node-form proc)))
	 (convert-primop-tail-call (node-form proc) args cont-var))
	(else
	 (convert-unknown-tail-call (cons proc args) cont-var protocol))))

; Every primitive has its own expander.

(define (expand-primitive-call proc args node)
  ((primitive-expander (node-form proc)) args (node-type node)))

(define lambda-node? (node-predicate 'lambda))
(define primitive-node? (node-predicate 'primitive))
(define literal-node? (node-predicate 'literal))

(define literal-op (get-operator 'literal))

(define (make-literal value)
  (make-node literal-op value))

;----------------------------------------------------------------
; LET (= a call whose procedure is a LAMBDA)

; REALLY-CONVERT-LET does all the work.  These convert the body of the LET
; using either CPS or TAIL-CPS and connect everything up.

(define (convert-let proc args node)
  (receive (lnode first-call)
      (really-convert-let proc args node)
    (receive (vals body-first-call body-last-lambda)
	(values-cps (caddr proc))
      (values vals
	      first-call
	      (splice!->last lnode body-first-call body-last-lambda)))))
	
(define (convert-tail-let proc args node cont-var)
  (receive (lnode first-call)
      (really-convert-let proc args node)
    (attach-body lnode (tail-cps (caddr proc) cont-var))
    first-call))

;  Make the call to the LET primop and build the lambda node for the procedure.

(define (really-convert-let proc args node)
  (receive (call first-call last-lambda)
      (cps-call (get-primop (enum primop let)) 1 1 args cps)
    (let ((vars (map (lambda (name)
		       (let ((var (make-variable (name-node->symbol name)
						 (node-type name))))
			 (node-set! name 'variable var)
			 var))
		     (cadr proc))))
      (do ((names (cadr proc) (cdr names))
	   (index 1 (+ index 1)))
	  ((null? names))
	(maybe-add-argument-name! call index (node-form (car names))))
      (let ((lnode (make-lambda-node #f 'cont vars)))
	(attach call 0 lnode)
	(values lnode (splice!->first first-call last-lambda call))))))

; Primitive calls
; Use CPS-CALL to do the work and then make a continuation if the primop is
; not trivial.

(define (convert-primop-call primop args type)
  (let ((trivial? (primop-trivial? primop)))
    (receive (call first-call last-lambda)
	(cps-call primop (if trivial? 0 1) (if trivial? 0 1) args cps)
      (if (not trivial?)
	  (add-continuation call first-call last-lambda type)
	  (values call first-call last-lambda)))))

(define (add-continuation call first-call last-lambda type)
  (let* ((vars (map (lambda (type)
		      (make-variable 'v type))
		    (if (tuple-type? type)
			(tuple-type-types type)
			(list type))))
	 (cont (make-lambda-node 'c 'cont vars)))
    (attach call 0 cont)
    (values (if (tuple-type? type)
		(map make-reference-node vars)
		(make-reference-node (car vars)))
	    (splice!->first first-call last-lambda call)
	    cont)))

; Call CONVERT-PRIMOP-CALL and then make a return.

(define (convert-primop-tail-call primop args cont-var)
  (receive (value first-call last-lambda)
      (convert-primop-call primop args (variable-type cont-var))
    (splice!->first first-call
		    last-lambda
		    (if (list? value)
			(make-multiple-value-return cont-var value)
			(make-return cont-var value)))))

; Another front for CPS-CALL, passing it the UNKNOWN-TAIL-CALL primop and
; its arguments, which are the procedure being called, the protocol, and
; the actual arguments.

(define (convert-unknown-tail-call args cont-var protocol)
  (receive (call first-call last-lambda)
      (cps-call (get-primop (enum primop unknown-tail-call)) 0 1
		(cons (car args)
		      (cons (make-literal protocol) (cdr args)))
		cps)
    (attach call 0 (make-reference-node cont-var))
    (splice!->first first-call last-lambda call)))

;----------------------------------------------------------------
; BEGIN
; These are fronts for CPS-SEQUENCE.

(define-cps-converter 'begin
  (lambda (node)
    (receive (last-node real-first-call last-lambda)
	(cps-sequence (cdr (node-form node)) values-cps)
      (if (not real-first-call)
	  (cps last-node)
	  (receive (vals first-call real-last-lambda)
	      (values-cps last-node)
	    (values vals
		    real-first-call
		    (splice!->last last-lambda first-call real-last-lambda)))))))

(define-tail-cps-converter 'begin
  (lambda (node cont-var)
    (receive (last-node first-call last-lambda)
	(cps-sequence (cdr (node-form node)) values-cps)
      (splice!->first first-call last-lambda (tail-cps last-node cont-var)))))

;----------------------------------------------------------------
;
; (IF <a> <b> <c>)
;  =>
; (LET ((J (LAMBDA (V) [rest-goes-here])))
;   (TEST (LAMBDA () [tail-cps <b> J])
;         (LAMBDA () [tail-cps <c> J])
;         <a>))

(define-cps-converter 'if
  (lambda (node)
    (let ((exp (node-form node))
	  (join-var (make-variable 'j type/unknown))
	  (res-vars (make-variables (node-type node))))
      (receive (call first-call last-lambda)
	  (convert-if exp join-var)
	(let ((let-lambda (make-lambda-node 'c 'cont (list join-var)))
	      (let-call (make-call-node (get-primop (enum primop let)) 2 1))
	      (join-lambda (make-lambda-node 'j 'jump res-vars)))
	  (attach let-call 0 let-lambda)
	  (attach let-call 1 join-lambda)
	  (attach-body let-lambda call)
	  (values (map make-reference-node res-vars)
		  (splice!->first first-call last-lambda let-call )
		  join-lambda))))))

(define (make-variables type)
  (map (lambda (type)
	 (make-variable 'v type))
       (if (tuple-type? type)
	   (tuple-type-types type)
	   (list type))))

; Tail-recursive IFs do not require a join point.

(define-tail-cps-converter 'if
  (lambda (node cont-var)
    (let ((exp (node-form node)))
      (receive (call first-call last-lambda)
	  (convert-if exp cont-var)
	(splice!->first first-call last-lambda call)))))

; Actually build the two-continuation call to the TEST primop.

(define (convert-if exp cont-var)	
  (receive (call first-call last-lambda)
      (cps-call (get-prescheme-primop 'test) 2 2 (list (cadr exp)) cps)
    (let ((true-cont (make-lambda-node 'c 'cont '()))
	  (true-call (tail-cps (caddr exp) cont-var))
	  (false-cont (make-lambda-node 'c 'cont '()))
	  (false-call (tail-cps (cadddr exp) cont-var)))
      (attach-body true-cont true-call)
      (attach-body false-cont false-call)
      (attach call 0 true-cont)
      (attach call 1 false-cont)
      (values call first-call last-lambda))))

;----------------------------------------------------------------

(define-cps-converter 'values
  (lambda (node)
    (let ((args (cdr (node-form node))))
      (receive (call first-call last-lambda)
	  (cps-call (get-prescheme-primop 'unspecific) 0 0 args cps)
	(let ((vals (vector->list (call-args call))))
	  (map detach vals)
	  (values vals first-call last-lambda))))))
	  
(define-tail-cps-converter 'values
  (lambda (node cont-var)
    (let ((args (cdr (node-form node))))
      (receive (call first-call last-lambda)
	  (cps-call (get-primop (enum primop unknown-return)) 0 1 args cps)
	(attach call 0 (make-reference-node cont-var))
	(splice!->first first-call last-lambda call)))))
	  
(define-cps-converter 'call-with-values
  (lambda (node)
    (convert-call-with-values node #f)))

(define-tail-cps-converter 'call-with-values
  (lambda (node cont-var)
    (convert-call-with-values node cont-var)))

; Consumer is known to be a lambda node.

(define (convert-call-with-values node maybe-cont-var)
  (receive (vals first-call last-lambda)
      (values-cps (cadr (node-form node)))
    (let ((consumer (x->cps (caddr (node-form node)) #f))
	  (call (make-call-node (get-primop (if maybe-cont-var
						(enum primop tail-call)
						(enum primop call)))
				(+ 2 (length vals))
				(if maybe-cont-var 0 1))))
      (attach-call-args call `(#f ,consumer . ,vals))
      (cond (maybe-cont-var
	     (attach call 0 (make-reference-node maybe-cont-var))
	     (splice!->first first-call last-lambda call))
	    (else
	     (add-continuation call first-call last-lambda (node-type node)))))))

;----------------------------------------------------------------
; LETRECs have been analyzed and restructured by FLATTEN, so we know that
; the values are all lambdas.

(define-cps-converter 'letrec
  (lambda (node)
    (let ((form (node-form node)))
      (receive (first-call last-lambda)
	  (convert-letrec form)
	(receive (vals body-first-call body-last-lambda)
	    (values-cps (caddr form))
	  (values vals
		  first-call
		  (splice!->last last-lambda
				 body-first-call
				 body-last-lambda)))))))

(define-tail-cps-converter 'letrec
  (lambda (node cont-var)
    (let ((form (node-form node)))
      (receive (first-call last-lambda)
	  (convert-letrec form)
	(attach-body last-lambda (tail-cps (caddr form) cont-var))
	first-call))))

(define (convert-letrec form)
  (let ((vars (map (lambda (l)
		     (let ((var (make-variable (name-node->symbol (car l))
					       (node-type (car l)))))
		       (node-set! (car l) 'variable var)
		       var))
		   (cadr form)))
	(vals (map (lambda (l)
		     (receive (value first-call last-lambda)
			 (cps (cadr l))
		       value))
		   (cadr form)))
	(cont (make-lambda-node 'c 'cont '())))
    ((structure-ref node let-nodes)
       ((top (letrec1 1 l1))
        (l1 ((x #f) . vars) call2)
        (call2 (letrec2 1 cont (* x) . vals)))
      (do ((names (cadr form) (cdr names))
	   (index 2 (+ index 1)))
	  ((null? names))
	(maybe-add-argument-name! call2 index (node-form (caar names))))
      (values top cont))))

;----------------------------------------------------------------
; Utilities.

; Stuff is a list of alternating call and lambda nodes, with possible #Fs.
; This joins the nodes together by making the calls be the bodies of the
; lambdas (the call->lambda links are already done).  The last node is
; returned.

(define (splice! stuff)
  (let loop ((stuff stuff) (first #f) (last #f))
    (if (null? stuff)
	(values first last)
	(receive (first last)
	    (let ((next (car stuff)))
	      (cond ((not next)
		     (values first last))
		    ((not first)
		     (values next next))
		    (else
		     (if (and ((structure-ref node lambda-node?) last)
			      ((structure-ref node call-node?) next))
			 (attach-body last next))
		     (values first next))))
	  (loop (cdr stuff) first last)))))

(define (splice!->first . stuff)
  (receive (first last)
      (splice! stuff)
    first))

(define (splice!->last . stuff)
  (receive (first last)
      (splice! stuff)
    last))

; Stuff for making CPS nodes
(define make-reference-node (structure-ref node make-reference-node))
(define make-lambda-node    (structure-ref node make-lambda-node))
(define make-literal-node   (structure-ref node make-literal-node))
(define make-call-node      (structure-ref node make-call-node))
(define attach              (structure-ref node attach))
(define detach              (structure-ref node detach))
(define attach-body         (structure-ref node attach-body))
(define attach-call-args    (structure-ref node attach-call-args))
(define call-args           (structure-ref node call-args))

; Adding names to lambda nodes for debugging help.

(define (maybe-add-argument-name! call index name)
  (maybe-add-name! ((structure-ref node call-arg) call index) name))

(define (maybe-add-name! value name)
  (if ((structure-ref node lambda-node?) value)
      ((structure-ref node set-lambda-name!) value (schemify name))))

; Getting symbols for use as variable names.

(define (name-node->symbol node)
  (let ((name (node-form node)))
    (if (generated? name)
	(generated-symbol name)
	name)))
