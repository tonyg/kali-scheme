; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; These are hacked to ensure that all calls to INPUT-TYPE-PREDICATE and
; INPUT-TYPE-COERCION are evaluated at load time (because they don't
; have readily reconstructed types).

(define-syntax define-primitive
  (syntax-rules ()
    ((define-primitive opcode input-types action)
     (define-consing-primitive opcode input-types #f action))
    ((define-primitive opcode input-types action returner)
     (define-consing-primitive opcode input-types #f action returner))))

(define-syntax define-consing-primitive
  (syntax-rules ()
    ((define-consing-primitive opcode input-types space-proc action)
     (let ((proc (primitive-procedure-action input-types space-proc action)))
       (define-opcode opcode (proc))))
    ((define-consing-primitive opcode input-types space-proc action returner)
     (let ((proc (primitive-procedure-action input-types space-proc action returner)))
       (define-opcode opcode (proc))))))
  
(define-syntax primitive-procedure-action
  (lambda (exp rename compare)
    (destructure (((p-p-b input-types space-proc action . returner-option) exp))
      (let* ((nargs (length input-types))
	     (%action       (rename 'action))
	     (%key          (rename 'key))
	     (%ensure-space (rename 'ensure-space))
	     (%*val*        (rename '*val*))
	     (%arg2         (rename 'arg2))
	     (%arg3         (rename 'arg3))
	     (%arg4         (rename 'arg4))
	     (%arg5         (rename 'arg5))
	     (%pop          (rename 'pop))
	     (%let          (rename 'let))
	     (%let*         (rename 'let*))
	     (%lambda       (rename 'lambda))
	     (%if           (rename 'if))
	     (%and          (rename 'and))
	     (%goto         (rename 'goto))
	     (%input-type-predicate (rename 'input-type-predicate))
	     (%input-type-coercion  (rename 'input-type-coercion))
	     (%raise-exception      (rename 'raise-exception))
	     (%wrong-type-argument  (rename 'wrong-type-argument))
	     (shorten (lambda (l1 l2)
			(map (lambda (x1 x2) x2 x1) l1 l2)))
	     (places (reverse (shorten (list %*val* %arg2 %arg3 %arg4 %arg5)
				       input-types)))
	     (preds (reverse (shorten (map rename
					   '(pred1 pred2 pred3 pred4 pred5))
				      input-types)))
	     (x->ys (reverse (shorten (map rename
					   '(x->y1 x->y2 x->y3 x->y4 x->y5))
				      input-types))))
	(if (> nargs 5)
	    (error "time to add more arguments to DEFINE-PRIMITIVE"))
	`(,%let (,@(map (lambda (type pred)
			  `(,pred (,%input-type-predicate ,type)))
			input-types
			preds)
		 ,@(map (lambda (type x->y)
			  `(,x->y (,%input-type-coercion ,type)))
			input-types
			x->ys)
		 (,%action ,action))
	   (,%lambda ()
	     (,%let* (,@(if space-proc
			  `((,%key (,%ensure-space (,space-proc ,%*val*))))
			  '())
		    ,@(if (>= nargs 2) `((,%arg2 (,%pop))) `())
		    ,@(if (>= nargs 3) `((,%arg3 (,%pop))) `())
		    ,@(if (>= nargs 4) `((,%arg4 (,%pop))) `())
		    ,@(if (>= nargs 5) `((,%arg5 (,%pop))) `())
		    )
	       (,%if (,%and ,@(map (lambda (pred place)
				     `(,pred ,place))
				   preds
				   places))
		     ,(let ((yow `(,%action
				   ,@(map (lambda (x->y place)
					    `(,x->y ,place))
					  x->ys
					  places)
				   ,@(if space-proc `(,%key) '()))))
			(if (null? returner-option)
			    yow
			    `(,%goto ,(car returner-option) ,yow)))
		     (,%raise-exception ,%wrong-type-argument
					0
					. ,places)))))))))

;----------------
; Checking inputs and coercing results

(define (input-type pred coercer)  ;Alonzo wins
  (lambda (f) (f pred coercer)))

(define (input-type-predicate type) (type (lambda (x y) y x)))
(define (input-type-coercion type)  (type (lambda (x y) x y)))

(define (no-coercion x) x)

(define any->         (input-type (lambda (x) x #t) no-coercion))
(define fixnum->      (input-type fixnum?      extract-fixnum))
(define char->        (input-type vm-char?     extract-char))
(define vm-char->     (input-type vm-char?     no-coercion))
(define boolean->     (input-type vm-boolean?  extract-boolean))
(define location->    (input-type location?    no-coercion))
(define string->      (input-type vm-string?   no-coercion))
(define vector->      (input-type vm-vector?   no-coercion))
(define code-vector-> (input-type code-vector? no-coercion))

; Output coercion

(define (return val)
  (set! *val* val)
  (goto continue 0))

(define return-any return)

(define (return-boolean x)
  (goto return (enter-boolean x)))

(define (return-fixnum x)
  (goto return (enter-fixnum x)))

(define (return-char x)
  (goto return (enter-char x)))

(define (return-unspecific x)
  x ;ignored
  (goto return unspecific-value))

(define (no-result)
  (goto return unspecific-value))

