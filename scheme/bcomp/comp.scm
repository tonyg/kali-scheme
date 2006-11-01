; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is the main entry point to the compiler.  It returns a template
; that will execute the forms (each of which is a node).
;
; This is written in a somewhat odd fashion to make sure that the forms are
; not retained once they have been compiled.

(define (compile-forms forms name package-key)
  (with-package-key package-key
    (lambda ()
      (if (null? forms)
	  (segment->template (sequentially
			      (lambda-protocol 0 #t #f #f)
			      (deliver-value (instruction (enum op unspecific))
					     (return-cont #f)))
			     (make-frame #f name 0 #f #f #f))
	  (compile-forms-loop (reverse forms)
			      name
			      #f)))))			;next template

(define (compile-forms-loop forms name next)
  (if (null? forms)
      next
      (compile-forms-loop (cdr forms)
			  name
			  (compile-form (car forms) name next))))

; Compile a single top-level form, returning a template.  NEXT is either #F or
; a template; if it is a template we jump to it after FORM.
; Stack has zero args, no env, template.
  
(define (compile-form form name next)
  (let ((frame (make-frame #f name 0 #f #t #f)))
    (segment->template
      (sequentially
        (lambda-protocol 0 #t #f #f)	; template, no env, no closure
	(let ((node (flatten-form (force-node form))))
	  (cond ((define-node? node)
		 (sequentially
		   (compile-definition node frame an-ignore-values-cont)
		   (if next
		       (call-template-inst next #f 0 1 frame)
		       (instruction (enum op values) 0 0))))
		(next
		 (sequentially
		   (compile-expression node 1 frame an-ignore-values-cont)
		   (call-template-inst next #f 0 1 frame)))
		(else
		 (compile-expression node 1 frame (return-cont #f))))))
      frame)))

(define (call-template-inst template label nargs depth frame)
  (let ((offset (template-offset frame depth))
	(index (literal->index frame template)))
    (using-optional-label (enum op call-template)
			  label
			  (high-byte offset)
			  (low-byte offset)
			  (high-byte index)
			  (low-byte index)
			  nargs)))

(define (template-call template depth frame cont)
  (receive (before depth label after)
      (push-continuation depth frame cont #f)
    (sequentially before
		  (call-template-inst template label 0 depth frame)
		  after)))

(define define-node? (node-predicate 'define syntax-type))

; Definitions must be treated differently from assignments: we must
; use SET-CONTENTS! instead of SET-GLOBAL! because the SET-GLOBAL!
; instruction traps if an attempt is made to store into an undefined
; location.
;
; Called with a stack depth of one (the template).

(define (compile-definition node frame cont)
  (let* ((form (node-form node))
	 (name (cadr form)))
    (sequentially (stack-indirect-instruction
		    (template-offset frame 1)
		    (binding->index frame
				    (node-ref name 'binding)
				    (node-form name)
				    value-type))
		  (begin (depth-check! frame 2)
			 (instruction (enum op push)))
		  (compile-expression (caddr form)
				      2			; stack depth
				      frame
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
; returns a procedure that takes 8 arguments (the number the VM passes to
; the startup procedure).

; The length of the argument list needs to be in sync with
; MAKE-USUAL-RESUMER in rts/init.scm, and S48-CALL-STARTUP-PROCEDURE
; in vm/interp/resume.scm.

(define (make-startup-procedure inits resumer)
  (let* ((nargs 8)
	 (frame (make-frame #f		; no parent
			    #f		; no name
			    nargs	; args on stack
			    #f		; drop environment
			    #t		; keep template
			    #f)))       ; drop closure
    (append-templates inits
		      nargs
		      frame
		      (sequentially
		        (template-call resumer
				       (+ nargs 1)	; args + template
				       frame
				       (fall-through-cont #f #f))
			(instruction (enum op pop-n) 0 1) ; remove template
			(instruction (enum op tail-call) nargs)))))

; Return a template that accepts NARGS arguments, invokes TEMPLATES in turn,
; and then calls template FINAL on the arguments.

(define (append-templates templates nargs frame final)
  (segment->template
    (sequentially
      (lambda-protocol nargs #t #f #f)	; push template
      (reduce (lambda (template seg)
		(sequentially
		  (template-call template
				 (+ nargs 1)		; arguments + template
				 frame
				 an-ignore-values-cont)
		  seg))
	      final
	      templates))
    frame))

(define an-ignore-values-cont (ignore-values-cont #f #f))

