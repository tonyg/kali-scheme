; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Substitution

; ST stands for substitution template (cf. MAKE-SUBSTITUTION-TEMPLATE)

(define (inline-transform st aux-names)
  (cons
   (if (and (pair? st)
	    (eq? (car st) 'lambda))
       (let ((formals (cadr st))
	     (body (caddr st)))
	 (lambda (e r c)
	   (let ((args (cdr e)))
	     (if (= (length formals) (length args))
		 (substitute body (make-substitution r formals args))
		 ;; No need to generate warning since the type checker will
		 ;; produce one.  Besides, we don't want to produce a warning
		 ;; for things like (> x y z).
		 e))))
       (lambda (e r c)
	 (cons (substitute st r)
	       (cdr e))))
   aux-names))

(define (make-substitution r formals args)
  (let ((subs (map cons formals args)))
    (lambda (name)
      (let ((probe (assq name subs)))
	(if probe
	    (cdr probe)
	    (if (generated? name)
		(begin (signal 'note
			       "this shouldn't happen - make-substitution"
			       name)
		       name)			;TEMPORARY KLUDGE.
		(r name)))))))


; Substitute into an expression.
; ST is an S-expression as returned by MAKE-SUBSTITUTION-TEMPLATE.
; Make nodes instead of s-expressions because otherwise TYPE-CHECK
; and SCHEMIFY will get confused.

(define (substitute st r)
  (cond ((symbol? st)
         (let ((foo (r st)))
           (if (name? foo)
               (make-node operator/name foo)
               foo)))
	((qualified? st)
	 (make-node operator/name
		    (qualified->name st r)))
	((pair? st)
	 (case (car st)
	   ((quote) (make-node (get-operator 'quote) st))
	   ((call)
	    (make-node (get-operator 'call)
		       (map (lambda (st) (substitute st r))
			    (cdr st))))
	   ((lambda) (error "lambda substitution NYI" st))
	   (else
	    (let ((keyword (car st)))
	      (make-node (get-operator keyword)
			 (cons keyword
			       (map (lambda (st) (substitute st r))
				    (cdr st))))))))
	(else
	 (make-node (get-operator 'literal) st))))

(define operator/name (get-operator 'name))

; --------------------
; Convert a qualified name #(>> parent-name symbol) to an alias.

(define (qualified->name q r)
  (let recur ((q q))
    (if (qualified? q)
	(let ((name (recur (qualified-parent-name q))))
	  (generate-name (qualified-symbol q)
			 (get-qualified-env (generated-env name)
					    (generated-symbol name))
			 name))
	(r q))))

(define (get-qualified-env env parent)
  (let ((binding (generic-lookup env parent)))
    (if (binding? binding)
	(let ((s (binding-static binding)))
	  (cond ((transform? s) (transform-env s))
		((structure? s) s)
		(else (error "invalid qualified reference"
			     env parent s))))
	(error "invalid qualified reference"
	       env parent binding))))
