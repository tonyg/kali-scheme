; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; This file is obsolete and no longer used.

;----------------------------------------------------------------------------
;                           SPECIAL FORMS
;
;              QUOTE CALL RETURN BLOCK LAMBDA LETREC
;               + LET for reasons of type-checking
;
;----------------------------------------------------------------------------

(define-record-type quote-exp :quote-exp
  (make-quote-exp value type)
  quote-exp?
  (value quote-exp-value)
  (type quote-exp-type set-quote-exp-type!))

(define-record-type call-exp :call-exp
  (make-call-exp! proc exits type args source)
  call-exp?
  (proc call-exp-proc)
  (exits call-exp-exits)
  (type call-exp-type set-call-exp-type!)
  (args call-exp-args)
  (source call-exp-source))

(define-record-type let-exp :let-exp
  (make-let-exp vars vals body source)
  let-exp?
  (vars let-exp-vars)
  (vals let-exp-vals)
  (body let-exp-body set-let-exp-body!)
  (source let-exp-source))

(define-record-type return-exp :return-exp
  (make-return-exp protocol type args)
  return-exp?
  (protocol return-exp-protocol)
  (type return-exp-type)
  (args return-exp-args))

(define-record-type block-exp :block-exp
  (make-block-exp exps)
  block-exp?
  (exps block-exp-exps))

(define-record-type lambda-exp :lambda-exp
  (make-lambda-exp id return-type protocol vars body source)
  lambda-exp?
  (id lambda-exp-id)
  (return-type lambda-exp-return-type set-lambda-exp-return-type!)
  (protocol lambda-exp-protocol)
  (vars lambda-exp-vars)
  (body lambda-exp-body set-lambda-exp-body!)
  (source lambda-exp-source))

(define (make-continuation-exp vars body)
  (make-lambda-exp #f #f #f vars body #f))

(define-record-type letrec-exp :letrec-exp
  (make-letrec-exp vars vals body source)
  letrec-exp?
  (vars letrec-exp-vars)
  (vals letrec-exp-vals)
  (body letrec-exp-body set-letrec-exp-body!)
  (source letrec-exp-source))

(define-record-type external-value :external-value
  (make-external-value type)
  external-value?
  (type external-value-type set-external-value-type!))

; Creating nodes and CPS converting calls and blocks.
;-------------------------------------------------------------------------------
; (CPS expression) => value + first-call + last-lambda
;  = the value of the expression
;    + the first of any calls that must be executed to get the value
;    + the continuation lambda of the last of the necessary calls
; The first call and the last lambda will be #F if the value is trivial.
;
; (TAIL-CPS expression continuation-variable) => call
;  = the first call to execute to return the value of the expression to
;    the continuation variable

(define (cps exp)
  (let ((value (cps-value exp)))
    (if value
	(values value #f #f)
	(generic-cps exp #f))))

(define (tail-cps exp cont-var)
  (receive (value type)
      (cps-value+type exp)
    (if value
	(make-value-return cont-var value type)
	(generic-cps exp cont-var))))

(define (cps-value exp)
  (receive (value type)
      (cps-value+type exp)
    value))

(define (cps-value+type exp)
  (cond ((variable? exp)
	 (values (make-reference-node exp) (variable-type exp)))
	((quote-exp? exp)
	 (values (make-literal-node (quote-exp-value exp)
				    (quote-exp-type exp))
		 (quote-exp-type exp)))
	((lambda-exp? exp)
	 (let ((node (lambda-exp->node exp)))
	   (values node (lambda-node-type node))))
	(else
	 (values #f #f))))

(define (generic-cps exp cont-var)
  (cond ((block-exp? exp)
	 (make-block (block-exp-exps exp) cont-var))
	((return-exp? exp)
	 (make-return-call exp cont-var))
	((call-exp? exp)
	 (make-primop-call exp cont-var))
	((let-exp? exp)
	 (make-lambda-call exp cont-var))
	((letrec-exp? exp)
	 (letrec-exp->node exp cont-var))
        (else
         (bug "unknown syntax~% ~S" exp))))

(define (lambda-exp->node exp)
  (let* ((cvar (make-variable 'c (lambda-exp-return-type exp)))
         (node (make-lambda-node (lambda-exp-id exp)
				 'proc
				 (cons cvar (copy-list (lambda-exp-vars exp))))))
    (set-lambda-protocol! node (lambda-exp-protocol exp))
    (set-lambda-source! node (lambda-exp-source exp))
    (attach-body node (tail-cps (lambda-exp-body exp) cvar))
    node))

(define (letrec-exp->node exp cont-var)
  (let ((vals (map cps-value (letrec-exp-vals exp)))
	(vars (letrec-exp-vars exp))
	(cont (make-lambda-node 'c 'cont '())))
    (let-nodes ((top (letrec1 1 l1))
		(l1 ((x #f) . vars) call2)
		(call2 (letrec2 1 cont (* x) . vals)))
      (set-call-source! top (letrec-exp-source exp))
      (happens-after top cont (letrec-exp-body exp) cont-var))))

; (CATCH id . body)
; (THROW primop rep id . args)

(define (make-undefined-value)
  (make-quote-exp the-undefined-value #f))

(define (exp->s-exp exp)
  (cond ((variable? exp)
	 (format #f "~S_~S" (variable-name exp) (variable-id exp)))
	((quote-exp? exp)
	 (list 'quote (quote-exp-value exp)))
	((block-exp? exp)
	 (cons 'begin (map exp->s-exp (block-exp-exps exp))))
	((return-exp? exp)
	 (cons 'return (map exp->s-exp (return-exp-args exp))))
	((call-exp? exp)
	 `(,(primop-id (call-exp-proc exp))
	   ,(call-exp-exits exp)
	   . ,(map exp->s-exp (call-exp-args exp))))
	((let-exp? exp)
	 `(let ,(map list
		     (map exp->s-exp (let-exp-vars exp))
		     (map exp->s-exp (let-exp-vals exp)))
	    ,(exp->s-exp (let-exp-body exp))))
	((lambda-exp? exp)
	 `(lambda ,(map exp->s-exp (lambda-exp-vars exp))
	    ,(exp->s-exp (lambda-exp-body exp))))
	((letrec-exp? exp)
	 `(letrec ,(map list
			(map exp->s-exp (letrec-exp-vars exp))
			(map exp->s-exp (letrec-exp-vals exp)))
	    ,(exp->s-exp (letrec-exp-body exp))))
        (else
         (error '"unknown syntax~% ~S" exp))))

(define (exp-source exp)
  (cond ((call-exp? exp)
	 (call-exp-source exp))
	((let-exp? exp)
	 (let-exp-source exp))
	((letrec-exp? exp)
	 (letrec-exp-source exp))
	((lambda-exp? exp)
	 (lambda-exp-source exp))
	(else
	 #f)))

(define (find-some-source top-exp exp)
  (or (exp-source exp)
      (call-with-current-continuation
	(lambda (exit)
	  (let recur ((at top-exp))
	    (let ((hit? (cond ((eq? at exp)
			       #t)
			      ((call-exp? at)
			       (or (recur (call-exp-proc at))
				   (any recur (call-exp-args at))))
			      ((let-exp? at)
			       (or (recur (let-exp-body at))
				   (any recur (let-exp-vals at))))
			      ((letrec-exp? at)
			       (or (recur (letrec-exp-body at))
				   (any recur (letrec-exp-vals at))))
			      ((return-exp? at)
			       (any recur (return-exp-args at)))
			      ((lambda-exp? at)
			       (recur (lambda-exp-body at)))
			      ((block-exp? at)
			       (any recur (block-exp-exps at)))
			      (else #f))))
	      (if (and hit? (exp-source at))
		  (exit (exp-source at)))
	      hit?))))))
  