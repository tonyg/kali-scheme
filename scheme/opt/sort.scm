; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


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
;
; This uses the FREE-VARIABLES field set by analyze.scm.

(define (sort-forms nodes complete?)
  (let ((table (make-name-table))
	(procs '())
	(literals '())
	(aliases '())
	(rest '()))
    (for-each (lambda (node)
		(let ((form (make-form node)))
		  (if (define-node? node)
		      (let ((name (node-form (cadr (node-form node))))
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
	      (reverse nodes))
    (for-each (lambda (form)
		(maybe-make-aliased form table))
	      aliases)
    (insert-aliases
     (append literals
	     (topologically-sort procs table)
	     (filter form-unaliased? rest)))))

(define (stuff-count s)
  (apply + (map (lambda (s) (length (cdr s))) s)))

; For (DEFINE A B) add the form to the list of B's aliases if B is defined
; in the current package and never SET!.

(define (maybe-make-aliased form table)
  (let* ((value (caddr (node-form (form-node form))))
	 (maker (table-ref table (node-form value))))
    (if (and (node-ref value 'binding)
	     maker
	     (= 0 (usage-assignment-count
		    (node-ref (cadr (node-form (form-node maker))) 'usage))))
	(begin
	  (set-form-aliases! maker (cons form (form-aliases maker)))
	  (set-form-unaliased?! form #f)))))

(define (topologically-sort forms table)
  (apply append
	 (strongly-connected-components
	   forms
	   (lambda (form)
	     (filter (lambda (f)
		       (and f
			    (lambda-node? (caddr (node-form (form-node f))))))
		     (map (lambda (name)
			    (table-ref table (node-form name)))
			  (form-free form))))
	   form-temp
	   set-form-temp!)))

(define-record-type form :form
  (really-make-form node free aliases unaliased?)
  form?
  (node form-node)
  (aliases form-aliases set-form-aliases!)
  (unaliased? form-unaliased? set-form-unaliased?!)
  (free form-free set-form-free!)
  (temp form-temp set-form-temp!))

(define-record-discloser :form
  (lambda (form)
    (list 'form
	  (let ((node (form-node form)))
	    (if (define-node? node)
		(node-form (cadr (node-form node)))
		node)))))

(define (make-form node)
  (really-make-form node
		    (map usage-name-node
			 (node-ref node 'free-variables))
		    '()		; aliases
		    #t))	; unaliased?

; (DEFINE A ...) is followed by all forms (DEFINE X A).

(define (insert-aliases forms)
  (let loop ((forms forms) (done '()))
    (if (null? forms)
	(reverse done)
	(let ((form (car forms)))
	  (loop (append (form-aliases form) (cdr forms))
		(cons (form-node form) done))))))
