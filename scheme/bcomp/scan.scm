; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Macro-expand and process top-level forms.
;
; Main entry points are:
;    scan-forms (suitable for use by eval)
;    scan-file (suitable for use by load or compile-file)
; These both return lists of nodes.
; DEFINE and DEFINE-SYNTAX forms cause side effects to the package.
; Forms are macro-expanded as necessary in order to locate all definitions.
;
; Entry points supporting the package system are:
;    scan-structures
;    scan-package
; These both return lists of (filename . node-list).
;
; Also defined here is SCAN-BODY, which scans a lambda-body for
; internal definitions.  This is an independent mechanism.


(define $note-file-package (make-fluid list)) ;Hook


; Scan a set of forms for definitions.

(define (scan-forms forms p filename . env-option)
  (*standard-optimizer* (apply really-scan-forms `(,forms ,p #f . ,env-option))
			#f
			p))

(define (really-scan-forms forms p filename . env-option)
  (let ((env (if (null? env-option)
		 (package->environment p)
		 (car env-option))))
    (if filename ((fluid $note-file-package) filename p))
    (scan-form-list forms p (bind-source-file-name filename env))))

; Read a file, scanning it for definitions.

(define (scan-file pathname p . env-option)
  (*standard-optimizer* (apply really-scan-file `(,pathname ,p . ,env-option))
			(namestring pathname #f *scheme-file-type*)
			p))

(define (really-scan-file pathname p . env-option)
  (let* ((env (if (null? env-option)
		  (package->environment p)
		  (car env-option)))
	 (filename (namestring pathname #f *scheme-file-type*))
	 (truename (translate filename)))
    (call-with-input-file truename
      (lambda (port)
	(if filename ((fluid $note-file-package) filename p))
	(let ((env (bind-source-file-name filename env))
	      (reader (environment-reader env))
	      (o-port (current-output-port)))
	  (display truename o-port)
	  (force-output o-port)
	  (let ((result
		 (let recur ()
		   (let ((form (reader port)))
		     (if (eof-object? form)
			 '()
			 (append (scan-form form p env)
				 (recur)))))))
	    (display #\space o-port)
	    (force-output o-port)
	    result))))))

;----------------
; List of optimization passes that are always used.

(define *standard-optimizer-names* '())

; The default optimizer for the REPL, EVAL, etc.

(define (default-standard-optimizer forms filename p)
  forms)

; The optimizer for the REPL, EVAL etc.

(define *standard-optimizer* default-standard-optimizer)

; Set the optimizers that are used for all compilations.
;
; Don't use the normal expand optimizer for the REPL because it saves type
; information which may become inaccurate (because we don't have the whole
; source yet).
;
; Optimizers take a list ((filename . forms) ...) and a package and return
; a similar list.

(define (set-standard-optimizers! . optimizer-names)
  (cond ((null? optimizer-names)
	 (set! *standard-optimizer-names* '())
	 (set! *standard-optimizer* default-standard-optimizer))
	(else
	 (set! *standard-optimizer-names*
	       (if (memq 'expand optimizer-names)
		   optimizer-names
		   (cons 'expand optimizer-names)))
	 (set! *standard-optimizer* (make-standard-optimizer optimizer-names)))))

; The standard optimizer has to convert a list of forms into the format the
; optimizers expect and then convert the result back into a list of forms.

(define (make-standard-optimizer optimizer-names)
  (let ((optimizers (map (lambda (name)
			   (or (get-optimizer (if (eq? name 'expand)
						  'simple-expand
						  name))
			       (error "standard optimizer not found" name)))
			 optimizer-names)))
    (lambda (forms filename package)
      (let ((stuff (apply-optimizers optimizers
				     (list (cons filename forms))
				     package)))
	(apply append (map cdr stuff))))))

(define (apply-optimizers optimizers stuff package)
  (reduce (lambda (pass stuff)
	    (pass stuff package))
	  stuff
	  (reverse optimizers)))

; --------------------
; Process a list of forms.

(define (scan-form-list forms p env)
  (let loop ((forms forms))
    (if (null? forms)
	'()
	;; Force order of evaluation
	(let ((scanned-forms (scan-form (car forms) p env)))
	  (append scanned-forms
		  (loop (cdr forms)))))))

; Process a single top-level form, returning a list of nodes.

(define scan-form
  (let ((begin-node? (node-predicate 'begin syntax-type)))
    (lambda (form p env)
      (let ((node (classify form env)))
	(cond ((begin-node? node)
	       (scan-form-list (cdr (node-form node)) p env))
	      ((define-node? node)
	       (let ((form (node-form node)))
		 (package-define! p (cadr form) usual-variable-type)
		 (list node)))
	      ((define-syntax-node? node)
	       (process-define-syntax (node-form node) p env)
	       (list node))
	      (else
	       (list node)))))))


; Process a single (define-syntax ...) form

(define (process-define-syntax form p env)
  (let ((name (cadr form))
	(source (caddr form)))
    (package-define! p name
		     (process-syntax (if (null? (cdddr form))
					 source
					 `(cons ,source ',(cadddr form))) ;foo
				     env name p))))

; Package system stuff


; Utility for compile-structures and ensure-loaded.

(define (scan-structures structs process-package? package-action)
  (let ((out (current-noise-port))
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
			      (display #\[ out)
			      (write (structure-name s) out)
			      (newline out)
			      (set! p-seen (cons p p-seen))
			      (for-each recur (package-opens p))
			      (for-each (lambda (name+struct)
					  (recur (cdr name+struct)))
					(package-accesses p))
			      (let ((stuff (scan-package p)))
				(noting-undefined-variables p
				  (lambda ()
				    (package-action stuff p))))
			      (check-structure s)
			      (display #\] out)
			      (newline out))
			    (check-structure s))))))))
      (for-each recur structs))))


; Returns a list of pairs (file . (node1 node2 ...)).

(define (scan-package p . env-option)
  (let* ((env (if (null? env-option)
		  (package->environment p)
		  (car env-option)))
	 (stuff '())
	 (config-file (package-file-name p))
	 (dir (if config-file
		  (file-name-directory config-file)
		  #f)))

    (for-each (lambda (clause)
		(case (car clause)
		  ((files)
		   (for-each (lambda (file)
			       (let ((file (namestring file
						       dir
						       *scheme-file-type*)))
				 (set! stuff
				       (cons (cons file
						   (really-scan-file file p env))
					     stuff))))
			     (cdr clause)))
		  ((begin)
		   (set! stuff
			 (cons (cons config-file
				     ;; We could pass config-file here, but
				     ;; that screws up the emacs interface
				     (really-scan-forms (cdr clause) p #f env))
			       stuff)))
		  ((integrate)
		   (set-package-integrate?! p (or (null? (cdr clause))
						  (cadr clause))))
		  ((optimize))
		  ((define-all-operators)
		   (set! stuff
			 (cons (define-all-operators p) stuff)))
		  ((usual-transforms)
		   (initialize-usual-transforms! p (cdr clause)))
                  ((reader)
		   (let ((r (force (reflective-tower
				    (package->environment p)))))
		     (package-define-funny! p
					    funny-name/reader
					    ((car r) (cadr clause) (cdr r)))))
		  (else
		   (error "unrecognized define-structure keyword"
			  clause))))
	      (package-clauses p))

    (optimize (reverse stuff) p)))

(define (optimize stuff p)
  (let* ((names (if (package-integrate? p)
		    (let* ((opts (apply append
					(map cdr (filter (lambda (clause)
							   (eq? (car clause) 'optimize))
							 (package-clauses p)))))
			   (opts (reduce (lambda (name opts)
					   (if (memq name opts)
					       opts
					       (cons name opts)))
					 opts
					 *standard-optimizer-names*)))
		      (if (or (null? opts)
			      (memq 'expand opts))
			  opts
			  (cons 'expand opts)))
		    *standard-optimizer-names*))
	 (passes (map (lambda (name)
			(or (get-optimizer name)
			    (begin (signal 'note
					   "optional optimization pass not invoked"
					   name)
				   (lambda (stuff p) stuff))))
		      names)))
    (apply-optimizers passes stuff p)))

(define (check-structure s)
  (let ((undefined '()))
    (for-each-export
         (lambda (name want-type binding)
	   (if (binding? binding)
	       (let ((have-type (binding-type binding)))
		 (if (not (compatible-types? have-type want-type))
		     (warn "Type in interface doesn't match binding"
			   name
			   `(binding: ,(type->sexp have-type #t))
			   `(interface: ,(type->sexp want-type #t))
			   s)))
	       (set! undefined (cons name undefined))))
	 s)
    (if (not (null? undefined))
	(warn "Structure has undefined exports"
	      s
	      undefined))))


; The usual transforms

(define (initialize-usual-transforms! p names)
  (for-each (lambda (name)
	      (package-define! p name
			       (make-transform (usual-transform name)
					       p
					       syntax-type
					       `(usual-transform ',name)
					       name)))
	    names))

; Initialization for built-in integrations.

(define (define-all-operators p)
  (let ((procs '()))
    (table-walk (lambda (name op)
		  (let ((type (operator-type op)))
		    (if (not (or (eq? type syntax-type)
				 (memq type '(leaf internal))))
			(set! procs (cons name procs)))))
		operators-table)
    (let ((nodes (scan-forms (map make-define-primitive-node procs)
			     p #f)))
      (table-walk (lambda (name op)
		    (if (not (eq? (operator-type op) 'leaf))
			(package-define! p name op)))
		  operators-table)
      (cons #f nodes))))


(define make-define-primitive-node
  (let ((operator/define (get-operator 'define syntax-type))
	(operator/primitive-procedure
	 (get-operator 'primitive-procedure syntax-type)))
    (lambda (name)
      (make-node operator/define
		 `(define ,name
		    ,(make-node operator/primitive-procedure
				`(primitive-procedure ,name)))))))

; Optimizers

(define optimizers-table (make-table))
(define (get-optimizer name)
  (table-ref optimizers-table name))
(define (set-optimizer! name opt) (table-set! optimizers-table name opt))
