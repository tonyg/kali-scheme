; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


(define (prescheme-front-end package-ids spec-files copy no-copy shadow)
  (receive (packages exports lookup)
      (package-specs->packages+exports package-ids spec-files)
    (let ((forms (flatten-definitions (scan-packages packages))))
      (annotate-forms! (car package-ids) lookup exports copy no-copy shadow)
      (receive (forms producer)
	  (sort-forms forms)
	(format #t "Checking types~%")
	(let ((sorted (let loop ((forms '()))
			(cond ((producer)
			       => (lambda (f)
				    (type-check-form f)
				    (loop (cons f forms))))
			      (else
			       (reverse forms))))))
;	  (format #t "Adding coercions~%")
;	  (add-type-coercions (form-reducer forms))
	  sorted)))))

(define (form-reducer forms)
  (lambda (proc init)
    (let loop ((forms forms) (value init))
      (if (null? forms)
	  value
	  (loop (cdr forms)
		(proc (form-name (car forms))
		      (form-value (car forms))
		      value))))))

(define (test id files)
  ((structure-ref node reset-node-id))
  ((structure-ref record-types reset-record-data!))
  (prescheme-front-end id files '() '() '()))

(define (annotate-forms! package-id lookup exports copy no-copy shadow)
  (mark-forms! exports
	       lookup
	       (lambda (f) (set-form-exported?! f #t))
	       "exported")
  (mark-forms! copy
	       lookup
	       (lambda (f) (set-form-integrate! f 'yes))
	       "to be copied")
  (mark-forms! no-copy
	       lookup
	       (lambda (f) (set-form-integrate! f 'no))
	       "not to be copied")
  (for-each (lambda (data)
	      (let ((owner (package-lookup lookup (caar data) (cadar data))))
		(if owner
		    (mark-forms! (cdr data)
				 lookup
				 (lambda (f)
				   (set-form-shadowed! owner
						       (cons (form-var f)
							     (form-shadowed owner))))
				 (format #f "shadowed in ~S" (car data)))
		    (format #t "Warning: no definition for ~S, cannot shadow ~S~%"
			    (car data) (cdr data)))))
	    shadow))

(define (mark-forms! specs lookup marker mark)
  (let ((lose (lambda (p n)
		(format #t "Warning: no definition for ~S, cannot mark as ~A~%"
			(list p n) mark))))
    (for-each (lambda (spec)
		(let ((package-id (car spec))
		      (ids (cdr spec)))
		  (for-each (lambda (id)
			      (cond ((package-lookup lookup package-id id)
				     => marker)
				    (else
				     (lose package-id id))))
			    ids)))
	      specs)))

(define (package-lookup lookup package-id id)
  (let ((var (lookup package-id id)))
    (and (variable? var)
	 (maybe-variable->form var))))

; Two possibilities:
; 1. The variable is settable but the thunk gives it no particular value.
; 2. A real value is or needs to be present, so we relate the type of
; the variable with the type of the value.

; thunk's value may be a STOB and not a lambda.

(define (type-check-form form)
  ;; (format #t "  ~S: " (variable-name (form-var form)))
  (let* ((value (form-value form))
	 (var (form-var form))
	 (name (form-name form))
	 (value-type (cond (((structure-ref nodes node?) value)
			    (infer-definition-type value (source-proc form)))
			   ((variable? value)
			    (get-package-variable-type value))
			   (else
			    (bug "unknown kind of form value ~S" value)))))
    (set-form-value-type! form value-type)
    (cond ((not (variable-set!? var))
	   (let ((type (cond ((eq? type/unknown (variable-type var))
			      (let ((type (schemify-type value-type 0)))
				(set-variable-type! var type)
				type))
			     (else
			      (unify! value-type (get-package-variable-type var) form)
			      value-type))))
	     (if (not (type-scheme? type))
		 (make-nonpolymorphic! type)) ; lock down any related uvars
	     ;;(format #t "~S~%" (instantiate type))
	     ))
	  ((not (or (eq? type/unit value-type)
		    (eq? type/null value-type)))
	   (make-nonpolymorphic! value-type) ; no polymorphism allowed (so it
	   ;; is not checked for, so there may be depth 0 uvars in the type)
	   ;; (format #t " ~S~%" (instantiate value-type))
	   (unify! value-type (get-package-variable-type var) form))
	  ((eq? type/unknown (variable-type var))
	   (get-package-variable-type var)))))

(define (source-proc form)
  (lambda (port)
    (write-one-line port
		    70
		    (lambda (port)
		      (format port "~S = ~S"
			      (form-name form)
			      ((structure-ref nodes schemify)
			         (form-value form)))))))
