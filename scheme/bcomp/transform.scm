; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Transforms

; A transform represents a source-to-source rewrite rule: either a
; macro or an in-line procedure.

(define-record-type transform :transform
  (really-make-transform xformer env type aux-names source id)
  transform?
  (xformer   transform-procedure)
  (env	     transform-env)
  (type	     transform-type)
  (aux-names transform-aux-names) ;for reification
  (source    transform-source)    ;for reification
  (id	     transform-id))

(define (make-transform thing env type source id)
  (let ((type (if (or (pair? type)
		      (symbol? type))
		  (sexp->type type #t)
		  type)))
    (make-immutable!
     (if (pair? thing)
	 (really-make-transform (car thing) env type (cdr thing) source id)
	 (really-make-transform thing       env type #f          source id)))))

(define-record-discloser :transform
  (lambda (m) (list 'transform (transform-id m))))

(define (maybe-apply-macro-transform transform exp parent-name env-of-use)
  (let* ((token (cons #f #f))
	 (new-env (bind-aliases token transform env-of-use))
	 (rename (make-name-generator (transform-env transform)
				      token
				      parent-name))
	 (compare (make-keyword-comparator new-env)))
    (values ((transform-procedure transform) exp rename compare)
	    new-env)))

(define (apply-inline-transform transform exp parent-name)
  (let* ((env (transform-env transform))
	 (rename (make-name-generator env (cons #f #f) parent-name)))
    ((transform-procedure transform) exp env rename)))

; Two keywords are the same if:
;  - they really are the same
;  - neither one is bound and they have the same symbol in the source
;  - they are bound to the same denotation (macro or location or ...)

(define (make-keyword-comparator environment)
  (lambda (name1 name2)
    (or (eqv? name1 name2)
	(and (name? name1)	; why might they not be names?
	     (name? name2)
	     (let ((v1 (lookup environment name1))
		   (v2 (lookup environment name2)))
	       (if v1
		   (and v2 (same-denotation? v1 v2))
		   (and (not v2)
			(equal? (name->source-name name1)
				(name->source-name name2)))))))))

; Get the name that appeared in the source.

(define (name->source-name name)
  (if (generated? name)
      (generated-symbol name)
      name))
				       
; The env-of-definition for macros defined at top-level is a package,
; and the package system will take care of looking up the generated
; names.

(define (bind-aliases token transform env-of-use)
  (let ((env-of-definition (transform-env transform)))
    (if (procedure? env-of-definition)
	(lambda (name)
	  (if (and (generated? name)
		   (eq? (generated-token name)
			token))
	      (lookup env-of-definition (generated-symbol name))
	      (lookup env-of-use name)))
	env-of-use)))

; Generate names for bindings reached in ENV reached via PARENT-NAME.
; The names are cached to preserve identity if they are bound.  TOKEN
; is used to identify names made by this generator.

(define (make-name-generator env token parent-name)
  (let ((alist '()))			;list of (symbol . generated)
    (lambda (symbol)
      (if (symbol? symbol)
	  (let ((probe (assq symbol alist)))
	    (if probe
		(cdr probe)
		(let ((new-name (make-generated symbol token env parent-name)))
		  (set! alist (cons (cons symbol new-name)
				    alist))
		  new-name)))
	  (error "non-symbol argument to rename procedure"
		 symbol parent-name)))))

;----------------
; We break an abstraction here to avoid a circular module dependency.

(define (lookup cenv name)
  (cenv name))