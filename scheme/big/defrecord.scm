; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Syntax for defining record types

; This knows about the implementation of records and creates the various
; accessors, mutators, etc. directly instead of calling the procedures
; from the record structure.  This is done to allow the optional auto-inlining
; optimizer to inline the accessors, mutators, etc.

; LOOPHOLE is used to get a little compile-time type checking (in addition to
; the usual complete run-time checking).

; (define-record-type name constructor-fields other-fields)

; Constructor-arguments fields are either <name> or (<name>), the second
; indicating a field whose value can be modified.
; Other-fields are one of:
;  (<name> <expression>) = modifiable field with the given value.
;  <name>                = modifiable field with no initial value.

;(define-record-type job
;  ((thunk)
;   (dynamic-env)
;   number
;   inferior-lock
;   )
;  ((on-queue  #f)
;   (superior  #f)
;   (inferiors '())
;   (condition #f)
;   ))

(define-syntax define-record-type

  (let ()

    (define s->s symbol->string)
    (define s-conc (lambda args (string->symbol (apply string-append args))))
    (define spec-name (lambda (s) (if (pair? s) (car s) s)))
    (define (filter pred lst)
      (if (null? lst)
	  '()
	  (if (pred (car lst))
	      (cons (car lst) (filter pred (cdr lst)))
	      (filter pred (cdr lst)))))
    (define (ass compare name list)
      (cond ((null? list) #f)
	    ((and (pair? (car list))
		  (compare name (caar list)))
	     (car list))
	    (else
	     (ass compare name (cdr list)))))

    (lambda (form rename compare)
      (let* ((name (cadr form))
	     (arg-fields (caddr form))
	     (other-fields (cadddr form))
	     (init-fields (filter pair? other-fields))
	     (args (map spec-name arg-fields))

	     (field-name (lambda (field-name)
			   (s-conc (s->s name) "-" (s->s field-name))))
	     (set-name (lambda (field-name)
			 (s-conc "set-" (s->s name)
				 "-" (s->s field-name) "!")))
	     (pred-name (s-conc (s->s name) "?"))
	     (maker-name (s-conc (s->s name) "-maker"))
	     (type-name (s-conc "type/" (s->s name)))

	     (thing (rename 'name))
	     (%make-record-type   (rename 'make-record-type))
	     (%record             (rename 'record))
	     (%define-accessors   (rename 'define-accessors))
	     (%unspecific	  (rename 'unspecific))
	     (%loophole		  (rename 'loophole))
	     (%:value		  (rename ':value))
	     (%proc		  (rename 'proc))
	     (%define		  (rename 'define))
	     (%let		  (rename 'let))
	     (%lambda		  (rename 'lambda))
	     (%begin		  (rename 'begin)))
	`(,%begin
	   (,%define ,type-name
	     (,%make-record-type ',name
				 ',(map spec-name
					(append arg-fields other-fields))))
	   (,%define ,maker-name
	       (,%loophole (,%proc ,(map (lambda (x) %:value) args) ,type-name)
		 (,%lambda ,args
			   (,%record ,type-name
				     ,@args
				     . ,(map (lambda (field)
					       (cond ((ass compare
							   (spec-name field)
							   other-fields)
						      => cadr)
						     (else
						      `(,%unspecific))))
					     other-fields)))))
	   (,%define ,pred-name
	       (,%lambda (,(rename 'x))
		   (,(rename 'and)
		    (,(rename 'record?) ,(rename 'x))
		    (,(rename 'eq?) ,type-name
				    (,(rename 'record-ref) ,(rename 'x) 0)))))
	   (,%define-accessors ,type-name
			       ,@(map (lambda (spec)
					(if (pair? spec)
					    `(,(field-name (car spec))
					      ,(set-name (car spec)))
					    `(,(field-name spec))))
				      arg-fields)
			       ,@(map (lambda (spec)
					`(,(field-name (spec-name spec))
					  ,(set-name (spec-name spec))))
				      other-fields)))))))

(define-syntax define-accessors
  (lambda (e r c)
    (let ((%define-accessor (r 'define-accessor))
	  (%begin (r 'begin))
	  (type (cadr e))
	  (field-specs (cddr e)))
      (do ((i 1 (+ i 1))
	   (field-specs field-specs (cdr field-specs))
	   (ds '()
	       (cons `(,%define-accessor ,type ,i ,@(car field-specs))
		     ds)))
	  ((null? field-specs)
	   `(,%begin ,@ds)))))
  (define-accessor begin))

(define-syntax define-accessor
  (syntax-rules ()
    ((define-accessor ?type ?index ?accessor)
     (define ?accessor
       (loophole (proc (?type) :value)
		 (lambda (r)
		   (checked-record-ref (loophole :record r) ?type ?index)))))
    ((define-accessor ?type ?index ?accessor ?modifier)
     (begin (define-accessor ?type ?index ?accessor)
	    (define ?modifier
	      (loophole (proc (?type :value) :unspecific)
			(lambda (r new)
			  (checked-record-set! (loophole :record r) ?type ?index new))))))
    ((define-accessor ?type ?index)
     (begin))))
