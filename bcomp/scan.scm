; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Macro-expand and process top-level forms.
;
; Main entry points are:
;    scan-forms (suitable for use by eval), and
;    scan-file (suitable for use by load or compile-file).
;
; These all return lists of pairs (loc . form).
; Each loc is either
;   - a location, if the corresponding form is a definition, or
;   - #f, if the corresponding form is an expression.
; Forms are macro-expanded as necessary in order to locate all definitions.
;
; Defines and define-syntaxes cause side effects to the package.

; Also in this file, internal definition scanning; but this is an
; independent mechanism.


(define $note-file-package (make-fluid list)) ;Hook


; Scan a form for definitions.

(define (scan-forms forms p filename)
  (if filename ((fluid $note-file-package) filename p))
  (let-fluid $source-file-name filename
    (lambda ()
      (scan-form-list forms p))))

; Read a file, scanning it for definitions.

(define (scan-file pathname p)
  (let* ((filename (namestring pathname #f *scheme-file-type*))
	 (truename (translate filename)))
    (call-with-input-file truename
      (lambda (port)
	(if filename ((fluid $note-file-package) filename p))
	(let-fluid $source-file-name filename ;or pathname?
	  (lambda ()
	    (let ((o-port (current-output-port)))
	      (display truename o-port)
	      (force-output o-port)
	      (let ((result
		     (let recur ()
		       (let ((form (read port)))
			 (if (eof-object? form)
			     '()
			     (append (scan-form form p)
				     (recur)))))))
		(display #\space o-port)
		(force-output o-port)
		result))))))))


; Process a list of forms.

(define (scan-form-list forms p)
  (if (null? forms)
      '()
      ;; Force order of evaluation
      (let ((scanned-forms (scan-form (car forms) p)))
	(append scanned-forms
		(scan-form-list (cdr forms) p)))))

; Process a single top-level form, returning a list of (loc . form)'s.

(define scan-form
  (let ((operator/begin (get-operator 'begin 'syntax))
	(operator/define (get-operator 'define 'syntax))
	(operator/define-syntax (get-operator 'define-syntax 'syntax)))
    (lambda (form p)
      (cond ((and (pair? form) (name? (car form)))
	     (let ((probe (package-lookup p (car form))))
	       (cond ((transform? probe)
		      (scan-form (transform probe form
					       (lambda (name)
						 (probe-package p name)))
				    p))
		     ((eq? probe operator/begin)
		      (scan-form-list (cdr form) p))
		     ((eq? probe operator/define)
		      (let ((form (normalize-definition form)))
			(package-ensure-defined! p (define-lhs form))
			(list form)))
		     ((eq? probe operator/define-syntax)
		      (process-define-syntax form p)
		      '())
		     (else
		      (list form)))))
	    (else
	     (list form))))))

; Process a single (define-syntax ...) form

(define (process-define-syntax form p)
  (let* ((name (cadr form))
	 (source (desyntaxify (caddr form))) ; hello ???
	 (thing (eval-for-syntax p source)))
    (package-define! p name
      (cond ((procedure? thing)
	     (make-transform thing p 'syntax
			     (if (null? (cdddr form))
				 #f
				 (cadddr form))
			     source name))
	    ((and (pair? thing)
		  (procedure? (car thing))
		  (list? (cdr thing)))
	     (make-transform (car thing) p 'syntax (cdr thing) source name))
	    (else (error "invalid macro transformer" name thing))))))

(define (eval-for-syntax p form)
  (let ((f (package-for-syntax p)))
    ((package-evaluator f) form f)))


; Scan for internal defines.

(define (scan-body forms probe lookup cont)
  (if (null? forms)
      (cont '() '())
      (scan-body-form (car forms)
		      probe lookup
		      (lambda (defs)
			(scan-body (cdr forms)
				   probe lookup
				   (lambda (more-defs exps)
				     (cont (append defs more-defs)
					   exps))))
		      (lambda (exps)
			(cont '() (append exps (cdr forms)))))))

(define (scan-body-form form probe- lookup when-defs when-exps)
  (cond ((and (pair? form) (name? (car form)))
	 (let ((probe (lookup (car form))))
	   (cond ((transform? probe)
		  (scan-body-form (transform probe form probe-)
				  probe- lookup when-defs when-exps))
		 ((eq? probe operator/begin)
		  (scan-body (cdr form) probe- lookup
			     (lambda (defs exps)
			       (cond ((null? exps)
				      (when-defs defs))
				     ((null? defs)
				      (when-exps exps))
				     (else
				      (syntax-error "definitions and expressions intermixed in BEGIN" form))))))
		 ((eq? probe operator/define)
		  (when-defs (list (normalize-definition form))))
		 (else
		  (when-exps (list form))))))
	(else
	 (when-exps (list form)))))

(define operator/begin (get-operator 'begin 'syntax))
(define operator/define (get-operator 'define 'syntax))



; Package system stuff


; Utility for compile-structures and ensure-loaded.

(define (scan-structures structs process-package? package-action)
  (let ((out (current-output-port))	;should be (error-output-port) ?
	(p-seen '())
	(s-seen '()))
    (letrec ((recur
	      (lambda (s)
		(if (not (memq s s-seen))
		    (begin
		      (set! s-seen (cons s s-seen))
		      (let ((p (structure-package s)))
			(if (and (not (memq p p-seen))
				 (process-package? p))
			    (begin
			      (display "[" out)
			      (write (structure-name s) out)
			      (newline out)
			      (set! p-seen (cons p p-seen))
			      (for-each recur (package-opens p))
			      (for-each (lambda (name+struct)
					  (recur (cdr name+struct)))
					(package-accesses p))
			      (package-action (scan-package p)
					      p)
			      (check-structure s)
			      (display "]" out)
			      (newline out))))
		      (check-structure s))))))
      (for-each recur structs))))


; Returns a list of pairs (file . ((loc . form) ...))

(define (scan-package p)
  (let ((stuff '())
	(optimizers '()))
    (noting-undefined-variables p
      (lambda ()
	(let ((config-file (package-file-name p)))
	  (for-each
	   (lambda (clause)
	     (case (car clause)
	       ((files)
		(for-each (lambda (file)
			    (let ((file (namestring
					 file
					 (if config-file
					     (file-name-directory config-file)
					     #f)
					 *scheme-file-type*)))
			      (set! stuff
				    (cons (cons file (scan-file file p))
					  stuff))))
			  (cdr clause)))
	       ((begin)
		(set! stuff
		      (cons (cons config-file
				  (scan-forms (cdr clause) p config-file))
			    stuff)))
	       ((integrate-primitives)
		(integrate-all-primitives! p))
	       ((usual-transforms)
		(initialize-usual-transforms! p))
	       ((specials)
		(for-each (lambda (name)
			    (package-define! p name
					     (get-operator name 'syntax)))
			  (cdr clause)))
	       ((optimize)
		(set-package-integrate?! p #t)
		(set! optimizers (append optimizers
					 (map get-optimizer (cdr clause)))))
	       (else
		(error "unrecognized define-package keyword"
		       clause))))
	   (package-clauses p)))))

    (reduce (lambda (opt stuff) (opt stuff p))
	    (reverse stuff)
	    optimizers)))


; The usual transforms

(define (initialize-usual-transforms! p)
  (for-each-usual-macro (lambda (name proc aux-names)
			  (package-define! p name
			    (make-transform proc
					    p
					    'syntax
					    aux-names
					    `(usual-transform-procedure ',name)
					    name)))))

; Initialization for built-in integrations.

(define (integrate-all-primitives! p)
  (let ((winners '()))
    (for-each-definition
      (lambda (name info)
	(if (not (syntax? info))
	    (set! winners (cons name winners))))
      p)
    (for-each (lambda (name)
		(package-define! p name
				 (get-operator name 'procedure)))
	      winners)))

(define optimizers-table (make-table))
(define (get-optimizer name)
  (or (table-ref optimizers-table name)
      (begin (signal 'note "optional optimization pass not invoked" name)
	     (lambda (stuff p) stuff))))
(define (set-optimizer! name opt) (table-set! optimizers-table name opt))
