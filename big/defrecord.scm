; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.



; Syntax for defining record types


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

    (lambda (form rename compare)
      (let* ((name (cadr form))
	     (arg-fields (caddr form))
	     (other-fields (cadddr form))
	     (init-fields (filter pair? other-fields))

	     (field-name (lambda (field-name)
			   (s-conc (s->s name) "-" (s->s field-name))))
	     (set-name (lambda (field-name)
			 (s-conc "set-" (s->s name)
				 "-" (s->s field-name) "!")))
	     (pred-name (s-conc (s->s name) "?"))
	     (maker-name (s-conc (s->s name) "-maker"))
	     (type-name (s-conc "type/" (s->s name)))

	     (make (rename 'make))
	     (%make-record-type   (rename 'make-record-type))
	     (%record-constructor (rename 'record-constructor))
	     (%record-predicate	  (rename 'record-predicate))
	     (%record-accessor	  (rename 'record-accessor))
	     (%record-modifier	  (rename 'record-modifier))
	     (%unspecific	  (rename 'unspecific))
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
	     (,%let ((,make (,%record-constructor
			     ,type-name
			     ',(map spec-name
				    (append arg-fields init-fields)))))
	       (,%lambda ,(map spec-name arg-fields)
		 (,make ,@(map spec-name arg-fields)
			,@(map cadr init-fields)))))
	   (,%define ,pred-name (,%record-predicate ,type-name))
	   ,@(map (lambda (spec)
		    `(,%define ,(field-name (spec-name spec))
		       (,%record-accessor ,type-name ',(spec-name spec))))
		  (append arg-fields other-fields))
	   ,@(map (lambda (spec)
		    `(,%define ,(set-name (spec-name spec))
		       (,%record-modifier ,type-name ',(spec-name spec))))
		  (filter pair? arg-fields))
	   ,@(map (lambda (spec)
		    `(,%define ,(set-name (spec-name spec))
		       (,%record-modifier ,type-name ',(spec-name spec))))
		  other-fields))))))
