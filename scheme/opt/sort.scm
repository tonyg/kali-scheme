; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Topological sort on forms.

; Puts top-level forms in the following order:
;
; (DEFINE X <literal>)
; (DEFINE Z (LAMBDA ...))
; ...everything else...
;
; Every (DEFINE W ...) for which W is never SET! is followed by all forms
; (DEFINE V W).
;
; The procedure definitions are topologically sorted; whenever possible no
; use of a variable occurs before its definition.
;
; COMPLETE? is true if STUFF contains the entire body of a module.

(define (sort-stuff stuff complete?)
  (let ((table (make-name-table))
	(procs '())
	(literals '())
	(aliases '())
	(rest '()))
    (for-each
     (lambda (filename+nodes)
       (let ((filename (car filename+nodes))
	     (nodes (cdr filename+nodes)))
	 (for-each (lambda (node)
		     (let ((form (make-form filename node)))
		       (if (define-node? node)
			   (let ((name (cadr (node-form node)))
				 (value (caddr (node-form node))))
			     (table-set! table name form)
			     (cond ((lambda-node? value)
				    (set! procs (cons form procs)))
				   ((name-node? value)
				    (set! aliases (cons form aliases))
				    (set! rest (cons form rest)))
				   ((or (quote-node? value)
					(literal-node? value))
				    (set! literals (cons form literals)))
				   (else
				    (set! rest (cons form rest)))))
			   (set! rest (cons form rest)))))
		   nodes)))
     stuff)
    (if complete?
	(for-each (lambda (form)
		    (maybe-make-aliased form table))
		  aliases))
    (rebuild-stuff
     (append literals
	     (topologically-sort procs table)
	     (reverse (filter form-unaliased? rest))))))

(define (stuff-count s)
  (apply + (map (lambda (s) (length (cdr s))) s)))

; For (DEFINE A B) add the form to the list of B's aliases if B is defined
; in the current package and never SET!.

(define (maybe-make-aliased form table)
  (let* ((value (caddr (node-form (form-node form))))
	 (maker (table-ref table (node-form value))))
    (if (and (node-ref value 'binding)
	     maker
	     (= 0 (usage-assignment-count (node-ref (form-node maker) 'usage))))
	(begin
	  (set-form-aliases! maker (cons form (form-aliases maker)))
	  (set-form-unaliased?! form #f)))))

(define (form-name form)
  (cadr (node-form (form-node form))))

(define (topologically-sort forms table)
  (for-each (lambda (form)
	      (set-form-free! form
			      (free-top-level-variables (form-node form))))
	    forms)
  (apply append
	 (reverse
	  (strongly-connected-components
	   forms
	   (lambda (form)
	     (filter (lambda (f)
		       (and f (form-free f)))
		     (map (lambda (name)
			    (table-ref table name))
			  (form-free form))))
	   form-temp
	   set-form-temp!))))

(define-record-type form :form
  (really-make-form file node aliases free unaliased?)
  form?
  (file form-file)
  (node form-node)
  (aliases form-aliases set-form-aliases!)
  (unaliased? form-unaliased? set-form-unaliased?!)
  (free form-free set-form-free!)
  (temp form-temp set-form-temp!))

(define (make-form file node)
  (really-make-form file node '() #f #t))

(define make-name-table
  (make-table-maker eq?
		    (lambda (name)
		      (cond ((symbol? name)
			     (string-hash (symbol->string name)))
			    ((generated? name)
			     (generated-uid name))
			    (else
			     (error "funny type of name" name))))))

; Merge adjacent forms from the same file and take care of aliases.
; (DEFINE A ...) is followed by all forms (DEFINE X A).

(define (rebuild-stuff forms)
  (if (null? forms)
      '()
      (let* ((form (car forms))
	     (forms (append (form-aliases form) (cdr forms))))
	(let loop ((forms forms)
		   (file (form-file form))
		   (same (list (form-node form)))
		   (res '()))
	  (if (null? forms)
	      (reverse (cons (cons file (reverse same)) res))
	      (let* ((form (car forms))
		     (forms (append (form-aliases form) (cdr forms))))
		(if (eq? file (form-file form))
		    (loop forms file (cons (form-node form) same) res)
		    (loop forms
			  (form-file form)
			  (list (form-node form))
			  (cons (cons file (reverse same)) res)))))))))

; Also exported for use by others.
		     
(define (free-top-level-variables node)
  (if (define-node? node)
      (find-free (caddr (node-form node)) '())
      (find-free node '())))

(define (find-free node vars)
  (if (name-node? node)    ; singled out because we need the node
      (let ((var (node-form node)))
	(if (memq var vars) vars (cons var vars)))
      ((operator-table-ref find-free-vars (node-operator-id node))
       (node-form node)
       vars)))

; Particular operators

(define find-free-vars
  (make-operator-table (lambda (exp vars)
			 (reduce find-free vars (cdr exp)))))

(define (define-find-free name proc)
  (operator-define! find-free-vars name #f proc))

(define-find-free 'literal
  (lambda (exp vars) vars))

(define-find-free 'quote
  (lambda (exp vars) vars))

(define-find-free 'lambda
  (lambda (exp vars)
    (find-free (caddr exp) vars)))

(define-find-free 'letrec
  (lambda (exp vars)
    (reduce (lambda (spec vars)
	      (find-free (cadr spec) vars))
	    (find-free (caddr exp) vars)
	    (cadr exp))))

(define-find-free 'loophole
  (lambda (exp vars)
    (find-free (caddr exp) vars)))

(define-find-free 'call
  (lambda (exp vars)
    (reduce find-free vars exp)))
