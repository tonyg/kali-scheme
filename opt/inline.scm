; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Substitution

(define (make-procedure-for-inline-transform tem p)
  (really-make-procedure-for-inline-transform
   (invert-substitution-template tem p)))

(define (really-make-procedure-for-inline-transform exp)
  (if (pair? exp)  ;lambda-expression
      (let ((formals (cadr exp))
	    (body (cddr exp)))
	(lambda (e r c)
	  (let ((args (cdr e)))
	    (if (= (length formals) (length args))
		(cons operator/begin
		      (substitute-list body
				       (extend-substitution r formals args)))
		(syntax-error "wrong number of arguments to in-line procedure"
			      e)))))
      (lambda (e r c)
	(cons (substitute exp r)
	      (cdr e)))))

(define (extend-substitution r formals args)
  (let ((subs (map cons formals args)))
    (lambda (name)
      (let ((probe (assq name subs)))
	(if probe
	    (cdr probe)
	    (r name))))))


; Substitute into an expression.

(define (substitute exp r)
  (cond ((pair? exp)
	 (let ((op (car exp)))
	   (cond ((eq? op operator/quote)
		  exp)
		 ((eq? op operator/local)
		  (r (cadr exp)))
		 ((eq? op operator/lambda)
		  (let ((formals (cadr exp)))
		    `(,op ,formals
			  ,@(substitute-list (cddr exp)
					     ;; Should gensym!
					     (extend-substitution r
								  formals
								  formals)))))
		 (else
		  (substitute-list exp r)))))
	((symbol? exp)
	 (r exp))
	;; ((generated? exp) ...)
	(else exp)))

(define (substitute-list exps r)
  (map (lambda (exp) (substitute exp r)) exps))



; An inverse to the optimizer's MAKE-SUBSTITUTION-TEMPLATE.  This
; exists only for the reifier.  Damned close to what alpha-conversion
; does, however.

(define (invert-substitution-template tem p)
  (cond ((pair? tem)
	 (let ((op (invert-substitution-template (car tem) p)))
	   (cond ((eq? op operator/quote)
		  `(,op ,(cadr tem)))
		 ((eq? op operator/lambda)
		  (let ((formals (cadr tem))
			(body (caddr tem)))
		    `(,op ,formals
			,(invert-substitution-template body p))))
		 (else
		  (cons op
			(map (lambda (tem)
			       (invert-substitution-template tem p))
			     (cdr tem)))))))
	((oplet? tem)			;ugh
	 (oplet->operator tem))
	((extrinsic? tem)
	 (extrinsic->name tem p))
	(else tem)))

(define operator/quote  (get-operator 'quote 'syntax))
(define operator/lambda (get-operator 'lambda 'syntax))
(define operator/local (get-operator 'local 'syntax))
(define operator/begin (get-operator 'begin 'syntax))


(define (oplet? exp)
  (and (vector? exp)
       (= (vector-length exp) 3)
       (eq? (vector-ref exp 0) 'op)))

(define (oplet->operator exp)
  (get-operator (vector-ref exp 1) (vector-ref exp 2)))

(define (operator->oplet exp)
  (vector 'op (operator-name exp) (operator-type exp)))


; Extrinsics provide a read/print representation for generated names.

(define (extrinsic? exp)
  (and (vector? exp)
       (= (vector-length exp) 2)
       (eq? (vector-ref exp 0) 'extrinsic)))

(define (extrinsic->name extrinsic p)
  (let ((path (vector-ref extrinsic 1)))
    (do ((path (cdr path) (cdr path))
	 (p p (let ((t (package-lookup p (car path))))
		(if (transform? t)
		    (transform-env t)
		    (error "invalid extrinsic specifier" extrinsic t))))
	 (name (car path)
	       (generate-name (car path)
			      p
			      name)))
	((null? path)
	 name))))

(define (name->extrinsic name p)
  (do ((path '()
	     (cons (generated-symbol name)
		   (if (eq? (generated-env name) p)
		       (cdr path)		;+++
		       path)))
       (name name (generated-parent-name name))
       (p p (generated-env name)))
      ((symbol? name)
       (if (null? path)
	   name
	   (let ((x (vector 'extrinsic (cons name path))))
	     (if *extrinsic-debug?*
		 (begin (display "Extrinsic: ")
			(write-extrinsic x (current-output-port))
			(newline)))
	     x)))))

(define *extrinsic-debug?* #t)

(define (write-extrinsic x port)
  (do ((path (vector-ref x 1) (cdr path)))
      ((null? (cdr path)) (write (car path) port))
    (write (car path) port)
    (display #\#)))

;(define (write-with-extrinsics obj port)
;  (if (extrinsic? obj)
;      (write-extrinsic obj port)
;      (recurring-write obj port write-with-extrinsics)))
