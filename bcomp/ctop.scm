; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; File processing, etc. for compiler


; Compile-form and compile-file return templates for thunks, suitable for
;  execution via ((make-closure <template> <arbitrary>)).

(define (compile-form form p)
  (compile-scanned-forms (scan-forms (list form) p #f)
			 p #f #f))


; Compile a file.  The entire file is first scanned to process all
; definitions.  Then it's compiled.  The result is a template.

(define (compile-file filename p)	; => thunk template
  (let* ((scanned-forms (scan-file filename p))
	 (o-port (current-output-port))
	 (template (compile-scanned-forms scanned-forms p filename o-port)))
    (newline o-port)
    (force-output o-port)
    template))


; compile-and-run-{form,file} are the same as compile-{form, file},
; but instead of returning templates, they produce a template for each
; form as the form is compiled and passes the template to a specified action
; procedure.

(define (compile-and-run-forms forms p filename run noisy?)
  (compile-and-run-scanned-forms (scan-forms forms p filename)
				 p filename run noisy?))

(define (compile-and-run-file filename p run noisy?)
  (compile-and-run-scanned-forms (scan-file filename p) p filename run
				 noisy?))

; If non-noisy, tail-recurs to last form.

(define (compile-and-run-scanned-forms scanned-forms p filename run noisy?)
  (let* ((do-it (lambda (scanned-form)
		  (compile-scanned-forms (list scanned-form)
					   p filename noisy?)))
	 (do-them (lambda ()
		    (if (null? scanned-forms)
			(compile-scanned-forms '() p #f #f)
			(do-it (do ((scanned-forms scanned-forms
						(cdr scanned-forms)))
				   ((null? (cdr scanned-forms))
				    (car scanned-forms))
				 (run (do-it (car scanned-forms)))))))))
    (if noisy?
	(call-with-values (lambda ()
			    (run (noting-undefined-variables p
				   (lambda ()
				     (do-them)))))
	  (lambda results
	    (newline noisy?)
	    (force-output noisy?)
	    (apply values results)))
	(run (let-fluid $note-undefined #f
	       (lambda ()
		 (do-them)))))))


; compile-scanned-forms

(define (compile-scanned-forms scanned-forms p filename noisy?)
  (let-fluid $source-file-name filename
    (lambda ()
      (segment->template
       (if (null? scanned-forms)
	   (deliver-value (instruction op/unspecific)
			  (return-cont #f))
	   (let recur ((scanned-forms scanned-forms))
	     (if (null? (cdr scanned-forms))
		 (compile-processed-form (car scanned-forms)
					 p (return-cont #f) noisy?)
		 (careful-sequentially 
		  (compile-processed-form (car scanned-forms)
					  p an-ignore-values-cont noisy?)
		  ;; Cf. compile-begin
		  (recur (cdr scanned-forms))
		  0
		  (return-cont #f)))))
       filename
       #f))))     ;pc-in-segment = #f

; scanned-form is a pair (loc-or-#f . form) -- see comments for scan-form

(define (compile-processed-form form p cont noisy?)
  (if (define? form)
      ;; Form is a definition:
      (let* ((name (define-lhs form))
	     (segment (sequentially
		       (compile (define-rhs form)
				p
				0
				(named-cont name))
		       (deliver-value
			(instruction-with-variable op/set-global!
						   name
						   (cons p name)
						   #f)
			cont))))
	(if noisy?
	    (begin (write-char #\. noisy?)
		   (force-output noisy?)))
	segment)
      ;; Not a definition:
      (compile form p 0 cont)))

; Make a startup procedure from a list of initialization thunks.  This
; is only used by the static linker.

(define (make-startup-procedure inits resumer)
  (let ((nargs 3))
    (make-closure
     (segment->template
      (reduce (lambda (init seg)
		(sequentially
		 (maybe-push-continuation
		  (sequentially
		   (instruction-with-literal op/closure init)
		   (instruction op/call 0))
		  nargs
		  an-ignore-values-cont)
		 seg))
	      (sequentially
	       (maybe-push-continuation
		  (sequentially
		   (instruction-with-literal op/closure resumer)
		   (instruction op/call 0))
		  nargs
		  (fall-through-cont #f #f))
	       ;; was (compile resumer p nargs (fall-through-cont))
	       (instruction op/call nargs))
	      inits)
      #f #f)
     0)))

(define an-ignore-values-cont (ignore-values-cont #f #f))
