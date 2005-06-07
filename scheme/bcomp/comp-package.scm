; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; package -> template

(define (compile-package package)
  (let ((template (compile-forms ((get-optimizer
				     (package-optimizer-names package))
				    (expand-package package)
				    package)
				 (package-name package)
				 (package-uid package))))
    (link! template package #t)		; #t means warn about undefined variables
    template))

; First we map down the FORMS+FILES, adding the filenames to ENV and
; scanning the forms.  Then we walk down the list of scanned forms and
; expand all the macros.
;
; All of the reversing in the second step makes it so that we process the
; forms in there original order, to keep any errors or warnings in as
; appropriate an order as possible, and then return them in their original
; order.

(define (expand-package package)
  (let ((env (package->environment package)))
    (call-with-values
     (lambda ()
       (package-source package))
     (lambda (forms+files transforms needs-primitives?)
       (for-each (lambda (name)
		   (define-usual-transform env name))
		 transforms)
       (let ((scanned+envs
	      (map (lambda (forms+file)
		     (let ((filename (car forms+file))
			   (forms (cdr forms+file)))
		       (let ((env (bind-source-file-name filename env)))
			 (cons env
			       (scan-forms forms env)))))
		   (if needs-primitives?
		       `((#f . ,(define-primitives env))
			 . ,forms+files)
		       forms+files))))
	 (reverse 
	  (fold (lambda (scanned+env expanded)
		  (let ((env (car scanned+env)))
		    (fold (lambda (form expanded)
			    (cons (delay (expand-scanned-form form env))
				  expanded))
			  (cdr scanned+env)
			  expanded)))
		scanned+envs
		'())))))))
		       
; NAME is the name of one of the usual Scheme macros (AND, OR, COND, and so
; forth).  This adds the appropriate transform to ENV.

(define (define-usual-transform env name)
  (environment-define! env
		       name
		       syntax-type
		       (make-transform (usual-transform name)
				       (extract-package-from-environment env)
				       syntax-type
				       `(usual-transform ',name)
				       name)))

; This adds definitions of all operators to ENV and returns a list of forms
; that define the closed-compiled versions of those operators that have such.
; It also adds a definition of ALL-OPERATORS to a vector of all the primitive
; operators, mostly for later use by the debugger to identify which primop
; caused an exception.

(define (define-primitives env)
  (table-walk (lambda (name op)
		(let ((type (operator-type op)))
		  (if (not (eq? (operator-type op) 'leaf))
		      (environment-define! env name (operator-type op) op))))
	      operators-table)

  (environment-define! env 'all-operators vector-type)
  
  (let ((all-operators-node (expand 'all-operators env))
	(vector-set!-node (make-node operator/literal (get-primop 'vector-set!)))
	(procs '())
	(primop-count
	 (let ((count 0))
	   (walk-primops (lambda (name type primop)
			   (set! count (+ 1 count))))
	   count))
	(index 0))

    (define (make-define-primitive-node name env)
      (make-node operator/define
		 `(define ,(expand name env)
		    ,(make-node operator/primitive-procedure
				`(primitive-procedure ,name)))))

    (define (make-register-primitive name index env)
      (make-node operator/call
		 (cons vector-set!-node
		       (list all-operators-node
			     (make-node operator/literal index)
			     (expand name env)))))

    (walk-primops (lambda (name type primop)
		    (environment-define! env name type primop)
		    (set! procs
			  (cons (make-define-primitive-node name env)
				(cons
				 (make-register-primitive name index env)
				 procs)))
		    (set! index (+ 1 index))))

    (set! procs
	  (cons
	   (make-node
	    operator/define
	    `(define ,all-operators-node
	       ,(make-node operator/call
			   (cons (make-node operator/literal 
					    (get-primop 'make-vector))
				 (list (make-node operator/literal
						  primop-count))))))
	   procs))

    procs))

;----------------
(define operator/define	             (get-operator 'define syntax-type))
(define operator/primitive-procedure (get-operator 'primitive-procedure syntax-type))
(define operator/call                (get-operator 'call   'internal))
(define operator/literal             (get-operator 'literal 'leaf))
