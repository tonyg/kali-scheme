; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; Translating the node tree into C

(define (write-c-file init-name file header forms)
  (set! *c-variable-id* 0)
  (set! *type-uids* '())
  (set! *next-type-uid* 0)
  (let* ((real-out (open-output-file file))
	 (out (make-tracking-output-port real-out)))
    (merge-forms forms)
    (check-hoisting forms)
    (format #t "Translating~%")
    (write-c-header header out)
    (write-function-prototypes forms out)
    (write-global-arg-variable-declarations forms out)
    (write-global-variable-declarations forms out)
    (newline out)
    (for-each (lambda (f)
		(case (form-type f)
		  ((lambda)
		   (compile-proc-to-c f out))
		  ((alias constant integrate merged stob initialize unused)
		   (values))
		  (else
		   (bug "unknown type of form ~S" f))))
	      forms)
    (write-c-main init-name out forms)
    (newline out)
    (set! *type-uids* '())
    (close-output-port out)
    (close-output-port real-out)))
    

(define (write-c-main init-name out forms)
  (set! *doing-tail-called-procedure?* #f)
  (set! *current-merged-procedure* #f)
  (cond ((any? (lambda (f)
		 (or (eq? (form-type f) 'initialize)
		     (eq? (form-type f) 'stob)
		     (eq? (form-type f) 'alias)))
	       forms)
	 (write-c-main-header (if init-name init-name 'main) out)
	 (for-each (lambda (f)
		     (case (form-type f)
		       ((initialize alias)
			(write-initialize (form-var f) (form-value f) out))
		       ((stob)
			(write-stob (form-var f)
				    (form-value-type f)
				    (lambda-body (form-value f))
				    out))))
		   forms)
	 (write-c-main-end out))))

(define (write-c-header header out)
  (format out "#include <stdio.h>~%")
  (format out "#include \"prescheme.h\"~%")
  (for-each (lambda (s)
	      (display s out)
	      (newline out))
	    header)
  (for-each (lambda (rtype)
	      (declare-record-type rtype out))
	    (all-record-types))
  (newline out)
  (values))

(define (declare-record-type rtype out)
  (format out "~%struct ")
  (write-c-identifier (record-type-name rtype) out)
  (format out " {~%")
  (for-each (lambda (field)
	      (format out "  ")
	      (display-c-type (record-field-type field)
			      (lambda (port)
				(write-c-identifier (record-field-name field)
						    out))
			      out)
	      (format out ";~%"))
	    (record-type-fields rtype))
  (format out "};"))

; Even when finished we need to keep the lambda around for help with
; calls to it.

(define (compile-proc-to-c form out)
  (format #t " ~A~%" (form-c-name form))
  (let ((name (form-c-name form)))
    (proc->c name form (form-shadowed form) out #f)
    (for-each make-form-unused! (form-merged form))
    (erase (detach-body (lambda-body (form-value form))))
    (suspend-form-use! form)))

(define (form-c-name form)
  (let* ((var (form-var form))
	 (name (c-ify (variable-name var))))
    (if (generated-top-variable? var)
	(string-append "H" name (number->string (c-variable-id var)))
	name)))

(define (no-value-node? node)
  (or (undefined-value-node? node)
      (and (reference-node? node)
	   (let ((type (final-variable-type (reference-variable node))))
	     (or (eq? type type/unit)
		 (eq? type type/null))))))

;------------------------------------------------------------
; Initialization procedure at the end of the file (often called `main').

; Header for initialization code

(define (write-c-main-header initname out)
  (format out "void~%")
  (write-c-identifier initname out)
  (format out "(void)~%{"))

; Write the end of the initialization code

(define (write-c-main-end out)
  (format out "~&}"))

(define (write-initialize var value out)
  (let ((wants (maybe-follow-uvar (variable-type var))))
    (receive (value has)
	(cond ((variable? value)
	       (values value (final-variable-type value)))
	      ((literal-node? value)
	       (values (literal-value value) (literal-type value)))
	      ((reference-node? value)
	       (let ((var (reference-variable value)))
		 (values var (final-variable-type var))))
	      (else
	       (error "unknown kind of initial value ~S" value)))
      (cond ((not (unspecific? value))
	     (c-assign-to-variable var out 0)
	     (if (not (type-eq? wants has))
		 (write-c-coercion wants out))
	     (cond ((input-port? value)
		    (display "0" out))
		   ((output-port? value)
		    (display "1" out))
		   ((variable? value)
		    (c-variable value out))
		   (else
		    (c-literal-value value has out)))
	     (writec out '#\;))))))

(define (write-stob var type call out)
  (let ((value (literal-value (call-arg call 0)))
	(wants (final-variable-type var)))
    (c-assign-to-variable var out 0)
    (cond ((vector? value)
	   (if (not (type-eq? type wants))
	       (write-c-coercion wants out))
	   (format out "malloc(~D * sizeof(" (vector-length value))
	   (display-c-type (pointer-type-to type) #f out)
	   (format out "));")
	   (do ((i 0 (+ i 1)))
	       ((>= i (vector-length value)))
	     (let* ((elt (call-arg call (+ i 1)))
		    (has (finalize-type
			  (if (reference-node? elt)
			      (variable-type (reference-variable elt))
			      (literal-value-type (literal-value elt))))))
	       (newline out)
	       (c-variable var out)
	       (format out "[~D] = " i)
	       (if (not (type-eq? (pointer-type-to type) has))
		   (write-c-coercion (pointer-type-to type) out))
	       (c-value elt out)
	       (write-char #\; out))))
	  (else
	   (error "don't know how to generate stob value ~S" value)))))

;------------------------------------------------------------
; Writing out a procedure.

(define (proc->c name form rename-vars port maybe-merged-count)
  (let ((top       (form-value form))
	(merged    (form-merged form))
	(tail?     (form-tail-called? form))
	(exported? (form-exported? form))
	(lambda-kids lambda-block))        ; filled in by the hoist code
    (let ((lambdas (filter (lambda (l)
			     (not (proc-lambda? l)))
			   (lambda-kids top))))
      (if maybe-merged-count
	  (merged-proc->c name top lambdas merged maybe-merged-count port tail?)
	  (real-proc->c name (form-var form) top lambdas
			merged rename-vars port tail? exported?))
      (values))))

(define (write-merged-form form port)
  (format #t "  ~A~%" (form-c-name form))
;  (breakpoint "write-merged-form ~S" form)
  (proc->c (form-c-name form)
	   form
	   '()
	   port
	   (length (variable-refs (form-var form)))))

;------------------------------------------------------------

; 1. write the header
; 2. declare the local variables
; 3. write out the body
; 4. write out all of the label lambdas

(define (real-proc->c id var top lambdas merged rename-vars port tail? exported?)
  (let ((vars (cdr (lambda-variables top)))
	(return-type (final-variable-type (car (lambda-variables top))))
	(all-lambdas (append lambdas (gather-merged-lambdas merged)))
	(merged-procs (gather-merged-procs merged)))
    (set! *doing-tail-called-procedure?* tail?)
    (set! *current-merged-procedure* #f)
    (receive (first rest)
	(parse-return-type return-type)
      (set! *extra-tail-call-args*
	    (do ((i (length rest) (- i 1))
		 (args '() (cons (format #f "TT~D" (- i 1)) args)))
		((= i 0)
		 args))))
    (set! *jumps-to-do* '())
    (write-procedure-header id return-type vars port tail? exported?)
    (write-char '#\{ port)
    (newline port)
    (for-each (lambda (v)
		(set-variable-flags! v (cons 'shadowed (variable-flags v))))
	      rename-vars)
    (write-arg-variable-declarations all-lambdas merged port)      
    (write-rename-variable-declarations rename-vars port)
    (write-merged-declarations merged port)
    (fixup-nasty-c-primops! (lambda-body top))
    (for-each (lambda (form)
		(write-merged-decls form port))
	      merged)
    (clear-lambda-generated?-flags lambdas)
    (set! *local-vars* '())
    (let ((body (call-with-string-output-port
		 (lambda (temp-port)
		   (let ((temp-port (make-tracking-output-port temp-port)))
		     (write-c-block (lambda-body top) temp-port 2)
		     (write-jump-lambdas temp-port 0)
		     (for-each (lambda (f)
				 (write-merged-form f temp-port))
			       (reverse merged))  ; makes for more readable output
		     (newline temp-port)
		     (force-output temp-port))))))
      (declare-local-variables port)
      (if tail? 
	  (write-global-argument-initializers (cdr (lambda-variables top))
					      port 2))
      (format port "~% {")
      (display body port)
      (write-char '#\} port))
    (for-each (lambda (v)
		(set-variable-flags! v (delq! 'shadowed (variable-flags v))))
	      rename-vars)
    (values)))

; These global variables should be replaced with fluids.

(define *doing-tail-called-procedure?* #f)
(define *current-merged-procedure* #f)
(define *extra-tail-call-args* '())

(define (gather-merged-lambdas merged)
  (let loop ((merged merged) (lambdas '()))
    (if (null? merged)
	lambdas
	(loop (append (form-merged (car merged)) (cdr merged))
	      (append (form-lambdas (car merged)) lambdas)))))

(define (gather-merged-procs merged)
  (let loop ((merged merged) (procs '()))
    (if (null? merged)
	procs
	(loop (append (form-merged (car merged)) (cdr merged))
	      (cons (form-value (car merged)) procs)))))

(define (write-merged-decls form port)
  (let ((top (form-value form))
	(merged (form-merged form)))
    (let ((vars (filter (lambda (var)
			  (and (used? var)
			       (not (eq? type/unit (final-variable-type var)))))
			(cdr (lambda-variables top)))))
      (write-variable-declarations vars port 2))
    (write-merged-declarations merged port)))

(define (merged-proc->c name top lambdas merged return-count port tail?)
  (let ((vars (cdr (lambda-variables top)))
	(body (lambda-body top)))
    (set! *doing-tail-called-procedure?* tail?)
    (set! *current-merged-procedure* name)
    (write-merged-header name top port)
    (write-char '#\{ port)
    (clear-lambda-generated?-flags lambdas)
    (write-c-block body port 2)
    (write-jump-lambdas port 0)
    (if (not tail?)
	(write-merged-return name return-count port))
    (for-each (lambda (f)
		(write-merged-form f port))
	      (reverse merged))  ; makes for more readable output
    (write-char '#\} port)
    (newline port)
    (values)))

(define (write-merged-header name top port)
  (format port "~% ~A: {~%" name)
  (if (not (null? (cdr (lambda-variables top))))
      (write-merged-argument-initializers (cdr (lambda-variables top)) port 2)))

; We use `default:' for the last tag so that the C compiler will
; know that the code following the switch is unreachable (to avoid
; a spurious warning if this is the end of the procedure).

(define (write-merged-return name return-count port)
  (format port "~% ~A_return:~%  switch (~A_return_tag) {~%" name name)
  (do ((i 0 (+ i 1)))
      ((>= i (- return-count 1)))
    (format port "  case ~S: goto ~A_return_~S;~%" i name i))
  (format port "  default: goto ~A_return_~S;~%" name (- return-count 1))
  (format port "  }"))

(define (write-merged-declarations forms port)
  (for-each (lambda (f)
	      (if (not (form-tail-called? f))
		  (write-merged-declaration f port)))
	    forms))

(define (write-merged-declaration form port)
  (let ((name (form-c-name form))
	(types (lambda-return-types (form-value form))))
    (format port "~%  int ~A_return_tag;" name)
    (do ((i 0 (+ i 1))
	 (types types (cdr types)))
	((null? types))
      (let ((type (car types)))
	(cond ((not (or (eq? type type/unit)
			(eq? type type/null)))
	       (format port "~%  ")
	       (display-c-type type
			       (lambda (port)
				 (format port "~A~D_return_value" name i))
			       port)
	       (writec port #\;)))))))

(define (lambda-return-types node)
  (let ((type (final-variable-type (car (lambda-variables node)))))
    (if (tuple-type? type)
	(tuple-type-types type)
	(list type))))

(define (write-procedure-header id return-type vars port tail? exported?)
  (newline port)
  (if (not exported?)
      (display "static " port))
  (receive (first rest)
      (parse-return-type return-type)
    (display-c-type (if tail? type/integer first)
		    (lambda (port)
		      (if tail? (write-char #\T port))
		      (display id port))
		    port)
    (write-char '#\( port)
    (if (not tail?)
	(let ((args (append vars
			    (do ((i 0 (+ i 1))
				 (rest rest (cdr rest))
				 (res '() (cons (cons i (car rest)) res)))
				((null? rest)
				 (reverse res))))))
	  (if (null? args)
	      (display "void" port)
	      (write-variables args port))))
    (write-char '#\) port)
    (newline port)))

; Write the names of VARS out to the port.  VARS may contain pairs of the
; form (<integer> . <type>) as well as variables.

(define (write-variables vars port)
  (let ((do-one (lambda (var)
		  (display-c-type (if (pair? var)
				      (make-pointer-type (cdr var))
				      (final-variable-type var))
				  (lambda (port)
				    (if (pair? var)
					(format port "TT~D" (car var))
					(c-variable var port)))
				  port))))
    (cond ((null? vars)
	   (values))
	  ((null? (cdr vars))
	   (do-one (car vars)))
	  (else
	   (do-one (car vars))
	   (do ((vars (cdr vars) (cdr vars)))
	       ((null? vars)
		(values))
	     (write-char '#\, port)
	     (write-char '#\space port)
	     (do-one (car vars)))))))
  
(define (write-rename-variable-declarations vars port)
  (for-each (lambda (var)
	      (indent-to port 2)
	      (display-c-type (final-variable-type var)
			      (lambda (port)
				(writec port #\R)
				(write-c-identifier (variable-name var) port))
			      port)
	      (display " = " port)
	      (write-c-identifier (variable-name var) port)
	      (format port ";~%"))
	    vars))

(define (write-c-block body port indent)
  (write-c-block-with-args body '() port indent))

(define (write-c-block-with-args body arg-vars port indent)
  (if (not (null? arg-vars))
      (write-argument-initializers arg-vars port indent))
  (call->c body port indent)
  (write-char '#\} port))

; Jump lambdas.  These are generated more-or-less in the order they are
; referenced.

(define (clear-lambda-generated?-flags lambdas)
  (for-each (lambda (l)
	      (set-lambda-block! l #f))
	    lambdas))

(define *jumps-to-do* '())

(define (note-jump-generated! proc)
  (if (not (lambda-block proc))
      (begin
	(set! *jumps-to-do* (cons proc *jumps-to-do*))
	(set-lambda-block! proc #t))))

(define (write-jump-lambdas port indent)
  (let loop ()
    (let ((jumps (reverse *jumps-to-do*)))
      (set! *jumps-to-do* '())
      (for-each (lambda (jump)
		  (jump-lambda->c jump port indent))
		jumps)
      (if (not (null? *jumps-to-do*))
	  (loop)))))
      
(define (jump-lambda->c node port indent)
  (newline port)
  (indent-to port indent)
  (display " L" port)
  (display (lambda-id node) port)
  (display ": {" port)
  (newline port)
  (write-c-block-with-args (lambda-body node)
			   (lambda-variables node)
			   port
			   (+ '2 indent)))

