; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Alpha-conversion for Scheme48.

(define (alpha-stuff stuff p)
  (map (lambda (filename+forms)
	 (let ((filename (car filename+forms))
	       (forms (cdr filename+forms)))
	   (let-fluid $source-file-name filename
	     (lambda ()
	       (cons filename
		     (alpha-forms forms p))))))
       stuff))


(define (alpha exp env)
  (cond ((name? exp)
	 (alpha-variable exp env))
	((pair? exp)
	 (cond ((name? (car exp))
		(let ((den (lookup env (car exp))))
		  (cond ((operator? den)
			 ((get-alphatizer den) den exp env))
			((transform? den)
			 (alpha (transform den exp (lambda (name)
						     (probe-env env name)))
				env))
			((local? den)
			 (alpha-local-call den (cdr exp) env))
			(else
			 (alpha-call exp env)))))
	       ((operator? (car exp))
	        ((get-alphatizer (car exp)) (car exp) exp env))
	       (else
		(alpha-call exp env))))
        ((literal? exp)
	 (make-quotation exp))
        ((location? exp) exp)
        (else (alpha (syntax-error "invalid expression" exp) env))))

(define (alpha-variable exp env)
  (let ((den (lookup-variable env exp)))
    (if (local? den)
	(note-value-reference! den))
    den))

(define (alpha-call exp env)
  (alpha-call-1 (alpha (car exp) env) (cdr exp) env))

(define (alpha-call-1 proc args env)
  (make-call proc
	     (map (lambda (arg-exp)
		    (alpha arg-exp env))
		  args)))

(define (alpha-local-call local args env)
  (note-operator-reference! local)
  (alpha-call-1 local args env))


; Special operators

(define alphatizers
  (make-compilator-table (lambda (den form env)
			   (cons den
				 (map (lambda (arg-exp)
					(alpha arg-exp env))
				      (cdr form))))))

(define define-alphatizer (car alphatizers))
(define get-alphatizer (cdr alphatizers))

(define-alphatizer 'quote 'syntax
  (lambda (den exp env) (make-quotation (cadr exp))))

(define-alphatizer 'local 'syntax
  (lambda (den exp env) exp))

(define-alphatizer 'lambda 'syntax
  (lambda (den exp env)
    (let* ((formals (cadr exp))
	   (body-env (abind formals env)))
      (make-lambda (cadr exp)
		   (map (lambda (name)
			  (lookup body-env name))
			(normalize-formals formals))
		   (alpha-body (cddr exp) body-env)))))

(define (alpha-body body env)
  (scan-body
      body
      (lambda (name) (probe-env env name))
      (lambda (name) (lookup env name))
      (lambda (defs exps)
	(if (null? defs)
	    (alpha-begin exps env)
	    ;; *** not hygienic ***
	    (alpha `(letrec ,(map cdr defs) ,@exps)
		   env)))))

(define-alphatizer 'begin 'syntax
  (lambda (den exp env)
    (alpha-begin (cdr exp) env)))

(define (alpha-begin exp-list env)
  (if (null? exp-list)
      (syntax-error "null begin")
      (let loop ((exp-list exp-list))
	(if (null? (cdr exp-list))
	    (alpha (car exp-list) env)
	    (make-begin (alpha (car exp-list) env)
			(loop (cdr exp-list)))))))

(define-alphatizer 'set! 'syntax
  (lambda (den exp env)
    (make-set! (lookup-assigned env (cadr exp))
	       (alpha (caddr exp) env))))

(define-alphatizer 'if 'syntax
  (lambda (den exp env)
    (make-if (alpha (cadr exp) env)
	     (alpha (caddr exp) env)
	     (if (null? (cdddr exp))
		 *unspecific*
		 (alpha (cadddr exp) env)))))

(define-alphatizer 'primitive-procedure 'syntax
  (lambda (den expr env)
    (make-primitive-procedure (cadr expr))))


; Environments

(define (package->environment p) p) ;?

(define (probe-env env name)
  (really-lookup env name probe-package))

(define (lookup env name)
  (really-lookup env name package-lookup))

(define (lookup-variable env name)
  (really-lookup env name proj2))

(define (lookup-assigned env name)
  (really-lookup env name
		 (lambda (p name)
		   (note-top-level-assignment! p name)
		   name)))

(define proj2 (lambda (p name) name))

(define (really-lookup env name p-lookup)
  (if (procedure? env)
      (env name p-lookup)
      (p-lookup env name)))

(define (abind1 name env)
  (let ((local (make-local name)))
    (lambda (a-name p-lookup)
      (if (eq? a-name name)
	  local
	  (really-lookup env a-name p-lookup)))))

(define (abind names env)
  (cond ((null? names) env)
	((not (pair? names))
	 (abind1 names env))
	(else
	 (abind1 (car names)
		 (abind (cdr names) env)))))


(define (note-top-level-assignment! p name)
  (call-with-values (lambda () (follow-path p name))
    (lambda (p name)
      (table-set! (get-packette p) name #t))))

(define (variable-ever-assigned? p name)
  (call-with-values (lambda () (follow-path p name))
    (lambda (p name)
      (table-ref (get-packette p) name))))

(define (follow-path p name)
  (do ((name name (generated-symbol name))
       (p p (generated-env name)))
      ((not (generated? name)) (values p name))))

(define packette-table (make-table))

(define (get-packette p)
  (or (table-ref packette-table (package-uid p))
      (let ((w (make-table)))
	(table-set! packette-table (package-uid p) w)
	w)))



; Alpha-convert a structure (should be package).

(define (alpha-forms scanned-forms p)
  (map (lambda (scanned-form)
	 (alpha scanned-form p))
       scanned-forms))
