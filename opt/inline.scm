; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Substitution

(define (inline-transform form aux-names)
  (cons
   (if (and (pair? form)
	    (eq? (car form) 'lambda))
       (let ((formals (cadr form))
	     (body (caddr form)))
	 (lambda (e r c)
	   (let ((args (cdr e)))
	     (if (= (length formals) (length args))
		 (substitute body (make-substitution r formals args))
		 (begin (warn "wrong number of arguments to in-line procedure"
			      e)
			e)))))
       (lambda (e r c)
	 (cons (substitute form r)
	       (cdr e))))
   aux-names))

(define (make-substitution r formals args)
  (let ((subs (map cons formals args)))
    (lambda (name)
      (let ((probe (assq name subs)))
	(if probe
	    (cdr probe)
	    (if (generated? name)
		name			;TEMPORARY KLUDGE.
		(r name)))))))


; Substitute into an expression.

(define (substitute form r)
  (cond ((symbol? form)
	 (r form)
;         (let ((foo (r form)))
;           (if (name? foo)
;               (make-node (get-operator 'name) foo)
;               foo))
	 )
	((qualified? form)
	 ;; (make-node (get-operator 'name) ...)
	 (qualified->name form r))
	((pair? form)
	 (case (car form)
	   ((quote) (make-node (get-operator 'quote) form))
	   ((lambda) (error "lambda substitution NYI" form))
	   ((call)
	    ;; ? (make-node (get-operator 'call) ...)
	    (map (lambda (form) (substitute form r))
		 (cdr form)))
	   (else
	    (let ((keyword (car form)))
	      (make-node (get-operator keyword)
			 (cons keyword
			       (map (lambda (form) (substitute form r))
				    (cdr form))))))))
	(else
	 (make-node (get-operator 'literal) form))))


; --------------------
; Qualified names provide a read/print representation for generated names.
; A qualified name has the form
;
;   (qualified <symbol-or-qualified> <symbol>)
;

(define (qualified? thing)
  (and (pair? thing)
       (eq? (car thing) 'qualified)))

(define (qualified->name q r)
  (let recur ((q q))
    (if (qualified? q)
	(let ((name (recur (cadr q))))
	  (generate-name (caddr q)
			 (get-qualified-env (generated-env name)
					    (generated-symbol name))
			 name))
	(r q))))

(define (get-qualified-env env parent)
  (let ((binding (generic-lookup env parent)))
    (if (binding? binding)
	(let ((s (binding-static (generic-lookup env parent))))
	  (cond ((transform? s) (transform-env s))
		((structure? s) s)
		(else (error "invalid qualified reference"
			     env parent s))))
	(error "invalid qualified reference"
	       env parent binding))))

; As an optimization, we elide intermediate steps in the lookup path
; if possible.  E.g. instead of
;     (qualified (qualified (qualified define-record-type define-accessors)
;			    define-accessor)
;		 record-ref)
; we can do
;     (qualified define-record-type record-ref)

(define (name->qualified name env)
  (if (generated? name)
      `(qualified
	,(let ((symbol (generated-symbol name)))
	   (name->qualified
	    (let loop ((name name))
	      (let ((parent (generated-parent-name name)))
		(if (generated? parent)
		    (let ((gparent (generated-parent-name parent)))
		      (if (same-denotation? (qlookup env parent symbol)
					    (qlookup env gparent symbol))
			  (loop parent)	;Win!
			  (begin (signal 'note
					 "elision lost"
					 parent symbol)
				 parent)))
		    parent)))
	    env))
	,(generated-symbol name))
      name))

(define (qlookup env parent symbol)
  (let ((binding (generic-lookup env parent)))
    (generic-lookup (transform-env (binding-static binding)) symbol)))


; cf. packages.scm

(define (generic-lookup env name)
  (cond ((package? env)
	 (package-lookup env name))
	((structure? env)
	 (structure-lookup env name #t))
	((procedure? env)
	 (lookup env name))
	(else
	 (error "invalid environment" env name))))
