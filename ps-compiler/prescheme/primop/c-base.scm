; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.

(define-c-generator let #f
  (lambda (call port indent)
    (let ((args (call-args call))
	  (vars (lambda-variables (call-arg call 0))))
      (do ((i 1 (+ i 1))
	   (vars vars (cdr vars)))
	  ((null? vars))
	(let ((val (vector-ref args i)))
	  (if (not (lambda-node? val))
	      (c-assignment (car vars) val port indent)))))))

(define-c-generator letrec1 #f
  (lambda (call port indent)
    (values)))

(define-c-generator letrec2 #f
  (lambda (call port indent)
    (values)))

(define-c-generator jump #f
  (lambda (call port indent)
    (let ((proc (called-lambda call)))
      (assign-argument-vars (lambda-variables proc) call 1 port indent)
      (indent-to port indent)
      (display "goto " port)
      (writec port #\L)
      (display (lambda-id proc) port)
      (write-char #\; port)
      (note-jump-generated! proc)
      (values))))
  
(define (assign-argument-vars vars call start port indent)
  (really-assign-argument-vars vars call start "arg" port indent))

(define (assign-merged-argument-vars vars call start port indent)
  (really-assign-argument-vars vars call start "merged_arg" port indent))

(define (assign-global-argument-vars vars call start port indent)
  (really-assign-argument-vars vars call start "goto_arg" port indent))

(define (really-assign-argument-vars vars call start name port indent)
  (let ((args (call-args call)))
    (do ((i start (+ i 1))
	 (vars vars (cdr vars)))
	((>= i (vector-length args)))
      (if (not (or (undefined-value-node? (vector-ref args i))
		   (eq? type/unit (get-variable-type (car vars)))))
	  (c-assignment (c-argument-var name
					(get-variable-type (car vars))
					(- i start)
					port)
			(vector-ref args i)
			port indent)))))

; Calls

; Unknown calls have a first argument of 'goto if they are supposed to be
; tail-recursive.  For known calls the protocol field of the lambda node
; is set to 'tail-called if any of the calls are supposed to be tail-recursive.
;
; Calls to non-tail-called procedures are just regular C calls.  For tail-
; called procedures there are two kinds of calls:
;  Tail-call from a tail-called procedure: proceed through the driver loop
;  All others: start a driver loop
;
; Known and unknown calls are handled identically, except that known calls
; may be to merged procedures.
;
; Merged procedures with GOTO calls:
;  This works if we merge the return points as well.  Possibly there should be
; one return switch per C procedure.  There do have to be separate return point
; variables (and one global one for the switch).

(define-c-generator call #f
  (lambda (call port indent)
    (cond ((merged-procedure-reference (call-arg call 1))
	   => (lambda (form)
		(generate-merged-call call 2 form port indent)))
	  (else
           (generate-c-call call 2 port indent)))))

(define-c-generator tail-call #f
  (lambda (call port indent)
    (cond ((merged-procedure-reference (call-arg call 1))
	   => (lambda (form)
		(generate-merged-goto-call call 2 form port indent)))
	  (else
           (generate-c-tail-call call 2 port indent)))))

(define-c-generator unknown-call #f
  (lambda (call port indent)
    (if (goto-protocol? (literal-value (call-arg call 2)))
	(user-warning "ignoring GOTO declaration for non-tail-recursive call to"
		      (variable-name (reference-variable
				      (call-arg call 1)))))
    (generate-c-call call 3 port indent)))

(define-c-generator unknown-tail-call #f
  (lambda (call port indent)
    (generate-c-tail-call call 3 port indent)))

(define (generate-merged-goto-call call start form port indent)
  (let ((proc (form-value form)))
    (assign-merged-argument-vars (cdr (lambda-variables proc))
				 call start
				 port indent)
    (indent-to port indent)
    (display "goto " port)
    (display (form-c-name form) port)
    (write-char #\; port)
    (values)))

(define (generate-goto-call call start port indent)
  (let ((proc (call-arg call 1)))
    (if (not (global-reference? proc))
	(bug "incorrect procedure in goto call ~S" call))
    (assign-global-argument-vars (cdr (lambda-variables
				       (global-lambda
					(reference-variable proc))))
				 call start
				 port indent)
    ; T is the marker for the tail-call version of the procedure
    (indent-to port indent)
    (display "return((long)T" port)
    (c-value proc port)
    (display ");" port)))

(define (global-lambda var)
  (let ((form (maybe-variable->form var)))
    (if (and form
	     (or (eq? 'lambda (form-type form))
		 (eq? 'merged (form-type form))))
	(form-value form)
	(bug "value of ~S, called using goto, is not a known procedure"
	       var))))

; C requires that we dereference all but calls to global functions.
; Calls to literals are macros that must take care of themselves.

(define (generate-c-call call start port indent)
  (let ((vars (lambda-variables (call-arg call 0)))
        (args (call-args call))
        (proc (call-arg call 1)))
    (if (and (global-reference? proc)
	     (memq? 'tail-called (variable-flags (reference-variable proc))))
	(call-with-driver-loop call start port indent (car vars))
	(let ((deref? (or (and (reference-node? proc)
			       (variable-binder (reference-variable proc)))
			  (call-node? proc))))
	  (if (used? (car vars))
	      (c-assign-to-variable (car vars) port indent))
	  (if deref?
	      (display "(*" port))
	  (c-value proc port)
	  (if deref?
	      (writec port #\)))
	  (write-value+result-var-list args start (cdr vars) port)))
    (writec port #\;)
    (values)))

(define (generate-c-tail-call call start port indent)
  (let ((proc (call-arg call 1))
	(args (call-args call))
	(cont (call-arg call 0)))
    (cond ((not (and (global-reference? proc)
		     (memq? 'tail-called
			    (variable-flags (reference-variable proc)))))
	   (let* ((type (get-variable-type (reference-variable cont)))
		  (void? (or (eq? type type/unit)
			     (eq? type type/null))))
	     (indent-to port indent)
	     (if (not void?)
		 (display "return " port))
	     (c-value proc port)
	     (write-value-list-with-extras args start *extra-tail-call-args* port)
	     (if void?
		 (begin
		   (display ";" port)
		   (indent-to port indent)
		   (display "return" port)))))
	  (*doing-tail-called-procedure?*
	   (generate-goto-call call start port indent))
	  (else
	   (call-with-driver-loop call start port indent #f)))
    (writec port #\;)
    (values)))

(define (global-reference? node)
  (and (reference-node? node)
       (global-variable? (reference-variable node))))

(define (call-with-driver-loop call start port indent result-var)	   
  (let* ((proc-var (reference-variable (call-arg call 1)))
	 (vars (lambda-variables (global-lambda proc-var))))
    (assign-global-argument-vars (cdr vars) call start port indent)
    (if result-var
	(c-assign-to-variable result-var port indent)
	(begin
	  (indent-to port indent)
	  (display "return " port)))
    (display "TTrun_machine((long)" port)
    (display "T" port)
    (write-c-identifier (variable-name proc-var) port)
    (display ")" port)))

(define (generate-merged-call call start form port indent)
  (let ((return-index (form-return-count form))
	(name (form-c-name form))
	(res (lambda-variables (call-arg call 0))))
    (set-form-return-count! form (+ 1 return-index))
    (assign-merged-argument-vars (cdr (lambda-variables (form-value form)))
				 call start port indent)
    (indent-to port indent)
    (format port "~A_return_tag = ~D;" name return-index)
    (indent-to port indent)
    (format port "goto ~A;" name)
    (indent-to port (- indent 1))
    (format port "~A_return_~S:" name return-index)
    (do ((i 0 (+ i 1))
	 (res res (cdr res)))
	((null? res))
      (let ((var (car res)))
	(cond ((and (used? var)
		    (let ((type (get-variable-type var)))
		      (and (not (eq? type type/unit))
			   (not (eq? type type/null)))))
	       (c-assign-to-variable var port indent)
	       (format port "~A~D_return_value;" name i)))))))

; Returns

(define-c-generator return #f
  (lambda (call port indent)
    (if *current-merged-procedure*
	(generate-return-from-merged-call call 1 port indent)
	(really-generate-c-return call 1 port indent))))

(define-c-generator unknown-return #f
  (lambda (call port indent)
    (cond (*doing-tail-called-procedure?*
	   (generate-return-from-tail-call call port indent))
	  (*current-merged-procedure*
	   (generate-return-from-merged-call call 1 port indent))
	  (else
	   (really-generate-c-return call 1 port indent)))))

(define (generate-return-from-tail-call call port indent)
  (if (not (no-value-node? (call-arg call 1)))
      (c-assignment "TTreturn_value" (call-arg call 1) port indent))
  (indent-to port indent)
  (display "return(0L);" port))

(define (generate-return-from-merged-call call start port indent)
  (let ((name *current-merged-procedure*))
    (do ((i start (+ i 1)))
	((= i (call-arg-count call)))
      (let ((arg (call-arg call i)))
	(if (not (no-value-node? arg))
	    (c-assignment (format #f "~A~D_return_value" name (- i start))
			  arg port indent))))
    (indent-to port indent)
    (format port "goto ~A_return;" name)))

(define (really-generate-c-return call start port indent)
  (do ((i (+ start 1) (+ i 1)))
      ((= i (call-arg-count call)))
    (let ((arg (call-arg call i)))
      (if (not (no-value-node? arg))
	  (begin
	    (indent-to port indent)
	    (format port "*TT~D = " (- (- i start) 1))
	    (c-value arg port)
	    (write-char #\; port)))))
  (let ((result (call-arg call start)))
    (cond
     ((and (not (no-value-node? result))
	   (let ((type (get-variable-type
			(reference-variable (call-arg call 0)))))
	     (and (not (eq? type type/unit))
		  (not (eq? type type/null)))))

      (indent-to port indent)
      (display "return" port)
      (write-char #\space port)
      (c-value result port)
      (display ";" port))
     (else
      (if (call-node? result)
	  ;; emit for the side effects
	  (begin
	    (indent-to port indent)
	    (primop-generate-c (call-primop result) result port 0)
	    (display ";" port)
	    (newline port)))
      (indent-to port indent)
      (display "return" port)
      (display ";" port))))
  (values))

; Allocate

;(define-c-generator allocate #f
;  (lambda (call port indent)
;    (let ((cont (call-arg call 0))
;          (size (call-arg call 1)))
;      (c-assign-to-variable (car (lambda-variables cont)) port indent)
;      (display "(long) malloc(" port)
;      (c-value size port)
;      (display "* sizeof(char));" port))))

(define-c-generator global-ref #t
  (lambda (call port indent)
    (c-value (call-arg call 0) port)))
  
(define-c-generator global-set! #f
  (lambda (call port indent)
    (let ((value (call-arg call 2)))
      (if (not (and (literal-node? value)
		    (unspecific? (literal-value value))))
	  (c-assignment (reference-variable (call-arg call 1))
			value
			port indent)))))

; if (ARG1 OP ARG2) {
;   cont1 }
; else {
;   cont2 }

(define-c-generator test #f
  (lambda (call port indent)
    (destructure ((#(cont1 cont2 value) (call-args call)))
      (generate-c-conditional-prelude port indent)
      (c-value value port)
      (generate-c-conditional-jumps cont1 cont2 port indent))))

(define (generate-c-conditional-prelude port indent)
  (indent-to port indent)
  (display "if " port)
  (writec port #\())

(define (generate-c-conditional-jumps cont1 cont2 port indent)
  (display ") {" port)
  (write-c-block (lambda-body cont1) port (+ indent 2))
  (newline port)
  (indent-to port indent)
  (display "else {" port)
  (write-c-block (lambda-body cont2) port (+ indent 2)))

(define-c-generator unspecific #t
  (lambda (call port indent)
    (bug "generating code for undefined value ~S" call)))

(define-c-generator uninitialized-value #t
  (lambda (call port indent)
    (bug "generating code for uninitialized value ~S" call)))

(define-c-generator null-pointer? #t
  (lambda (call port indent)
    (display "NULL == " port)
    (c-value (call-arg call 0) port)))

(define-c-generator null-pointer #t
  (lambda (call port indent)
    (display "NULL" port)))

(define-c-generator eq? #t
  (lambda (call port indent)
    (simple-c-primop "==" call port)))

