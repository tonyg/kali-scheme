; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.


; Data must be done last as it may contain references to the other stuff.

(define (display-forms-as-scheme forms out)
  (receive (data other)
      (partition-list (lambda (f)
			(and (node? (form-value f))
			     (literal-node? (form-value f))))
		      forms)
    (for-each (lambda (f)
		(display-form-as-scheme f (schemify (form-value f)) out))
	      other)
    (for-each (lambda (f)
		(display-data-form-as-scheme f out))
	      data)))

(define form-value (structure-ref forms form-value))
(define form-var   (structure-ref forms form-var))

(define literal-node? (node-predicate 'literal #f))

(define (display-form-as-scheme f value out)
  (cond ((unspecific? value)
	 (p `(define ,(get-form-name f)) out)
	 (newline out))
	((or (external-value? value)
	     (memq 'closed-compiled-primitive (variable-flags (form-var f))))
	 (values))
	(else
	 (p `(define ,(get-form-name f) ,value)
	    out)
	 (newline out))))

(define (display-data-form-as-scheme f out)
  (let* ((value (clean-literal (node-form (form-value f))))
	 (value (if (and (quoted? value)
			 (not (or (list? (cadr value))
				  (vector? (cadr value)))))
		    (cadr value)
		    value)))
    (display-form-as-scheme f value out)))

(define (get-form-name form)
  (name->symbol (get-variable-name (form-var form))))

(define (schemify node)
  (if (node? node)
      ((operator-table-ref schemifiers (node-operator-id node))
       node)
      (schemify-sexp node)))

(define unspecific?
  (let ((x (if #f #t)))
    (lambda (y)
      (eq? x y))))

(define schemifiers
  (make-operator-table (lambda (node)
			 (let ((form (node-form node)))
			   (if (list? form)
			       (map schemify form)
			       form)))))

(define (define-schemifier name type proc)
  (operator-define! schemifiers name type proc))

(define-schemifier 'name 'leaf
  (lambda (node)
    (cond ((node-ref node 'binding)
	   => (lambda (binding)
		(let ((var (binding-place binding)))
		  (if (variable? var)
		      (get-variable-name var)
		      (desyntaxify (node-form node))))))
	  (else
	   (name->symbol (node-form node))))))

; Rename things that have differ in Scheme and Pre-Scheme

(define aliases
  (map (lambda (s)
	 (cons s (string->symbol (string-append "ps-" (symbol->string s)))))
       '(read-char peek-char write-char newline
	 open-input-file open-output-file
	 close-input-port close-output-port)))

(define (get-variable-name var)
  (cond ((and (generated-top-variable? var)
	      (not (memq 'closed-compiled-primitive (variable-flags var))))
	 (string->symbol (string-append (symbol->string
					 (name->symbol (variable-name var)))
					"."
					(number->string (variable-id var)))))
	((assq (variable-name var) aliases)
	 => cdr)
	(else
	 (variable-name var))))

(define (name->symbol name)
  (if (symbol? name)
      name
      (string->symbol (string-append (symbol->string (generated-symbol name))
				     "."
				     (number->string (generated-uid name))))))

(define-schemifier 'quote #f
  (lambda (node)
    (list 'quote (cadr (node-form node)))))

(define-schemifier 'literal #f
  (lambda (node)
    (let ((form (node-form node)))
      (cond ((primop? form)
	     (primop-id form))
	    ((external-value? form)
	     (let ((string (external-value-string form)))
	       (if (string=? string "(long(*)())")
		   'integer->procedure
		   (string->symbol (external-value-string form)))))
	    ((external-constant? form)
	     `(enum ,(external-constant-enum-name form)
		    ,(external-constant-name form)))
	    (else
	     (schemify-sexp form))))))

(define-schemifier 'unspecific #f
  (lambda (node)
    ''unspecific))

; Used for primitives in non-call position.  The CDR of the form is a
; variable that will be bound to the primitive's closed-compiled value.

(define-schemifier 'primitive #f
  (lambda (node)
    (let ((form (node-form node)))
      (cond ((pair? form)
	     (get-variable-name (cdr form))) ; non-call position
	    ((assq (primitive-id form) aliases)
	     => cdr)
	    (else
	     (primitive-id form))))))

; lambda, let-syntax, letrec-syntax...

(define-schemifier 'letrec #f
  (lambda (node)
    (let ((form (node-form node)))
      `(letrec ,(map (lambda (spec)
                       `(,(schemify (car spec)) ,(schemify (cadr spec))))
                     (cadr form))
         ,@(map (lambda (f) (schemify f))
                (cddr form))))))

(define-schemifier 'lambda #f
  (lambda (node)
    (let ((form (node-form node)))
      `(lambda ,(let label ((vars (cadr form)))
		  (cond ((pair? vars)
			 (cons (schemify (car vars))
			       (label (cdr vars))))
			((null? vars)
			 '())
			(else
			 (schemify vars))))
	 ,@(map schemify (cddr form))))))

(define-schemifier 'goto #f
  (lambda (node)
    (map schemify (cdr (node-form node)))))

(define (schemify-sexp thing)
  (cond ((name? thing)
	 (desyntaxify thing))
	((primop? thing)
	 (primop-id thing))
	((primitive? thing)
	 (primitive-id thing))
	((variable? thing)
	 (get-variable-name thing))
	((pair? thing)
	 (let ((x (schemify-sexp (car thing)))
	       (y (schemify-sexp (cdr thing))))
	   (if (and (eq? x (car thing))
		    (eq? y (cdr thing)))
	       thing			;+++
	       (cons x y))))
	((vector? thing)
	 (let ((new (make-vector (vector-length thing) #f)))
	   (let loop ((i 0) (same? #t))
	     (if (>= i (vector-length thing))
		 (if same? thing new)	;+++
		 (let ((x (schemify-sexp (vector-ref thing i))))
		   (vector-set! new i x)
		   (loop (+ i 1)
			 (and same? (eq? x (vector-ref thing i)))))))))
	(else thing)))

(define (clean-literal thing)
  (cond ((name? thing)
	 (desyntaxify thing))
	((variable? thing)
	 (get-variable-name thing))
	((external-constant? thing)
	 `(enum ,(external-constant-enum-name thing)
		,(external-constant-name thing)))
	((pair? thing)
	 (let ((x (clean-literal (car thing)))
	       (y (clean-literal (cdr thing))))
	   (if (and (quoted? x) (quoted? y))
	       `(quote (,(cadr x) . ,(cadr y)))
	       `(cons ,x ,y))))
	((vector? thing)
	 (let ((elts (map clean-literal (vector->list thing))))
	   (if (every? quoted? elts)
	       `(quote ,(list->vector (map cadr elts)))
	       `(vector . ,elts))))
	(else
	 `(quote ,thing))))

(define (quoted? x)
  (and (pair? x)
       (eq? (car x) 'quote)))