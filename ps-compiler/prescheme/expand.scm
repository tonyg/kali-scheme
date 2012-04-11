; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.

; Expanding using the Scheme 48 expander.

(define (scan-packages packages)
  (let ((definitions
	  (fold (lambda (package definitions)
		  (let ((cenv (package->environment package)))
		    (fold (lambda (form definitions)
			    (let ((node (expand-form form cenv)))
			      (cond ((define-node? node)
				     (cons (eval-define (expand node cenv)
							cenv)
					   definitions))
				    (else
				     (eval-node (expand node cenv)
						global-ref
						global-set!
						eval-primitive)
				     definitions))))
			  (call-with-values
			   (lambda ()
			     (package-source package))
			   (lambda (files.forms usual-transforms primitives?)
			     (scan-forms (apply append (map cdr files.forms))
					 cenv)))
			  definitions)))
		packages
		'())))
    (reverse (map (lambda (var)
		    (let ((value (variable-flag var)))
		      (set-variable-flag! var #f)
		      (cons var value)))
		  definitions))))

(define package->environment (structure-ref packages package->environment))

(define define-node? (node-predicate 'define))

(define (eval-define node cenv)
  (let* ((form (node-form node))
	 (value (eval-node (caddr form)
			   global-ref
			   global-set!
			   eval-primitive))
	 (lhs (cadr form)))
    (global-set! lhs value)
    (name->variable-or-value lhs)))

(define (global-ref name)
  (let ((thing (name->variable-or-value name)))
    (if (variable? thing)
	(variable-flag thing)
	thing)))

(define (global-set! name value)
  (let ((thing (name->variable-or-value name)))
    (if (primitive? thing)
	(bug "trying to set the value of primitive ~S" thing)
	(set-variable-flag! thing value))))

(define (name->variable-or-value name)
  (let ((binding (node-ref name 'binding)))
    (if (binding? binding)
	(let ((value (binding-place binding))
	      (static (binding-static binding)))
	  (cond ((primitive? static)
		 static)
		((variable? value)
		 value)
		((and (location? value)
		      (constant? (contents value)))
		 (contents value))
		(else
		 (bug "global binding is not a variable, primitive or constant ~S" name))))
	(user-error "unbound variable ~S" (node-form name)))))

