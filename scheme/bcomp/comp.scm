; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is the main entry point to the compiler.  It returns a template
; that will execute the forms (each of which is a node).
;
; This is written in a somewhat odd fashion to make sure that the forms are
; not retained once they have been compiled.

;(define (compile-forms forms name)
;  (if (null? forms)
;      (segment->template (sequentially
;                           (instruction (enum op protocol) 0)
;                           (deliver-value (instruction (enum op unspecific))
;                                          (return-cont #f)))
;                         name
;                         #f             ;pc-in-segment
;                         #f)            ;debug data
;      (really-compile-forms forms
;                            (instruction (enum op protocol) 0)
;                            name)))
;      
;(define (really-compile-forms forms segment name)
;  (if (null? (cdr forms))
;      (segment->template (sequentially segment
;                                       (compile-form (car forms)
;                                                     (return-cont #f)))
;                         name
;                         #f             ;pc-in-segment
;                         #f)            ;debug data
;      (really-compile-forms (cdr forms)
;                            (sequentially segment
;                                          (compile-form (car forms)
;                                                        an-ignore-values-cont))
;                            name)))

(define (compile-forms forms name env-key)
  (if (null? forms)
      (segment->template (sequentially
                           (instruction (enum op protocol) 0)
                           (deliver-value (instruction (enum op unspecific))
                                          (return-cont #f)))
                         name
                         #f             ;pc-in-segment
                         #f)            ;debug data
      (compile-forms-loop (reverse forms)
			  name
			  env-key
			  #f)))

(define (compile-forms-loop forms name env-key next)
  (if (null? forms)
      next
      (compile-forms-loop (cdr forms)
			  name
			  env-key
			  (compile-form (car forms) name env-key next))))

; Compile a single top-level form, returning a template.  NEXT is either #F or
; a template; if it is a template we jump to it after FORM.
  
(define (compile-form form name env-key next)
  (segment->template
    (let-fluid $env-key env-key
      (lambda ()
	(sequentially
 	 (instruction (enum op protocol) 0)
	 (let ((node (flatten-form (force-node form))))  ; could flatten here
	   (cond ((define-node? node)
		  (sequentially
		   (compile-definition node an-ignore-values-cont)
		   (if next
		       (instruction-with-literal (enum op call-template)
						 next
						 0)
		       (instruction (enum op values) 0 0))))
		 (next
		  (sequentially
		   (compile-expression node 0 an-ignore-values-cont)
		   (instruction-with-literal (enum op call-template)
					     next
					     0)))
		 (else
		  (compile-expression node 0 (return-cont #f))))))))
    name
    #f		;pc-in-segment
    #f))	;debug data

(define define-node? (node-predicate 'define syntax-type))

(define $env-key (make-fluid #f))

; Definitions must be treated differently from assignments: we must
; use SET-CONTENTS! instead of SET-GLOBAL! because the SET-GLOBAL!
; instruction traps if an attempt is made to store into an undefined
; location.

(define (compile-definition node cont)
  (let* ((form (node-form node))
	 (name (cadr form)))
    (sequentially (instruction-with-location (enum op literal)
					     (node-ref name 'binding)
					     (node-form name)
					     value-type)
		  (instruction (enum op push))
		  (compile-expression (caddr form)
				      1
				      (named-cont (node-form name)))
		  (deliver-value
		   (instruction (enum op stored-object-set!)
				(enum stob location)
				location-contents-offset
				0)	; do not log in current proposal
		   cont))))

(define location-contents-offset
  (cond ((assq 'location stob-data)
	 => (lambda (stuff)
	      (let loop ((slots (cdddr stuff)) (i 0))
		(if (eq? (caar slots) 'contents)
		    i
		    (loop (cdr slots) (+ i 1))))))
	(else
	 (error "can't find location data in STOB-DATA"))))

;----------------
; Make a startup procedure from a list of initialization templates.  This
; is only used by the static linker.  RESUMER should be a template that
; returns a procedure that takes 5 arguments (the number the VM passes to
; the startup procedure).

(define (make-startup-procedure inits resumer)
  (let ((nargs 5))
    (append-templates inits
		      nargs
		      (sequentially
		        (maybe-push-continuation
			  (instruction-with-literal (enum op call-template)
						    resumer
						    0)
			  nargs
			  (fall-through-cont #f #f))
			(instruction (enum op call) nargs)))))

; Return a template that accepts NARGS arguments, invokes TEMPLATES in turn,
; and then calls template FINAL on the arguments.

(define (append-templates templates nargs final)
  (segment->template
    (sequentially
      (instruction (enum op protocol) nargs)
      (reduce (lambda (template seg)
		(sequentially
		  (maybe-push-continuation
		    (instruction-with-literal (enum op call-template)
					      template
					      0)
		    nargs
		    an-ignore-values-cont)
		  seg))
	      final
	      templates))
    #f		; no name
    #f		; pc-in-segment = #f
    #f))	; no debug data

(define an-ignore-values-cont (ignore-values-cont #f #f))

