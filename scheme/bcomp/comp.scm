; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; This is the main entry point to the compiler.  It returns a template
; that will execute the forms (each of which is a node).

(define (compile-forms forms name debug-data)
  (segment->template
    (sequentially
      (instruction (enum op protocol) 0)	; no arguments passed
      (if (null? forms)
	  (deliver-value (instruction (enum op unspecific))
			 (return-cont #f))
	  (let recur ((forms forms))
	    (if (null? (cdr forms))
		(compile-form (car forms) (return-cont #f))
		(sequentially 
		  (compile-form (car forms) an-ignore-values-cont)
		  ;; Cf. compile-begin
		  (recur (cdr forms)))))))
    name
    #f					;pc-in-segment = #f
    debug-data))

; Compile a single top-level form, returning a segment.

(define (compile-form node cont)
  (let ((node (force-node node)))
    (if (define-node? node)
	(compile-definition node cont)
	(compile-expression node 0 cont))))

(define define-node? (node-predicate 'define syntax-type))

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
				location-contents-offset)
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
; is only used by the static linker.

(define (make-startup-procedure inits resumer)
  (let ((nargs 4))
    (segment->template
     (sequentially
      (instruction (enum op protocol) nargs)
      (reduce (lambda (init seg)
		(sequentially
		 (maybe-push-continuation
		   (instruction-with-literal (enum op call-template) init 0)
		   nargs
		   an-ignore-values-cont)
		 seg))
	      (sequentially
	       (maybe-push-continuation
		 (instruction-with-literal (enum op call-template) resumer 0)
		 nargs
		 (fall-through-cont #f #f))
	       ;; was (compile resumer p nargs (fall-through-cont))
	       (instruction (enum op call) nargs))
	      inits))
      #f #f #f)))

(define an-ignore-values-cont (ignore-values-cont #f #f))

