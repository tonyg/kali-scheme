; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Calling C procedures.

; The arguments have been pushed on the stack after the procedure.
; *stack* = procedure name arg1 ... argN rest-list N+1 total-nargs
;
; The procedure must be an external binding whose value is a pointer-sized
; code-vector.  If it is, we actually do the call.
;
; The REMOVE-CURRENT-FRAME call pops all of our values off of the stack.
; In fact we still use them there for a moment (see s48_external_call() in
; external.c) but all of the values are fetched from the stack before
; anything new is pushed on.

(define-primitive call-external-value ()
  (lambda ()
    (let* ((nargs (extract-fixnum (pop)))
	   (stack-nargs (extract-fixnum (pop)))
	   (rest-list (pop)))
      (if (< maximum-external-call-args
	     (- nargs 2))			; procedure & name
	  (raise-exception too-many-arguments-to-external-procedure
			   0
			   (stack-ref (- stack-nargs 1))
			   nargs)
	  (begin
	    (do ((rest-list rest-list (vm-cdr rest-list)))
		((vm-eq? rest-list null))
	      (push (vm-car rest-list)))
	    (let ((proc (stack-ref (- nargs 1)))
		  (name (stack-ref (- nargs 2)))
		  (args (pointer-to-stack-arguments)))
	      (if (and (vm-string? name)
		       (code-vector? proc)
		       (= (code-vector-length proc)
			  (cells->bytes 1)))
		  (begin
		    (remove-current-frame)
		    (let ((result (external-call proc name (- nargs 2) args)))
		      (cond (*external-exception?*
			     (set! *external-exception?* #f)
			     (goto raise *external-exception-nargs*))
			    (else
			     (goto continue-with-value result 0)))))
		  (raise-exception wrong-type-argument 0 proc name))))))))

;----------------
; Raising exceptions from C.

; True if the C procedure is raising an exception instead of doing a normal
; return.

(define *external-exception?* #f)

; The number of arguments being passed to the exception handler.

(define *external-exception-nargs*)

; These are for exceptions raised by external code.  They work pretty
; much in the same way as other VM instructions raise exceptions.
; Note that this doesn't actually perform the raise; it just sets
; *EXTERNAL-EXCEPTION?* to be true, so that the EXTERNAL-CALL procedure
; will do the raise.
;
; If you extend the maximum number of arguments, be sure to adjust the
; definition of STACK-SLACK in stack.scm.  It needs to know the maximum
; number of values pushed by an exception handler, which is the number
; of arguments pushed here, plus the procedure.

(define (s48-setup-external-exception why nargs)
  (push-exception-setup! why 1)		; 1 = one-byte instruction
  (if (< 10 nargs)
      (error "too many arguments from external exception"))
  (set! *external-exception-nargs* nargs)
  (set! *external-exception?* #t))

; The external code needs to piggyback an exception on top of one already
; being raised.  We increase the argument count and return the old exception.

(define (s48-resetup-external-exception new-why additional-nargs)
  (let* ((old-nargs *external-exception-nargs*)
	 (old-why (stack-ref old-nargs)))
    (stack-set! old-nargs (enter-fixnum new-why))
    (set! *external-exception-nargs* (+ old-nargs additional-nargs))
    old-why))

;----------------
; Two tables of shared bindings: those we import from the outside and those
; that we provide to the outside.

(define *imported-bindings* false)
(define *exported-bindings* false)

; When resuming a statically-linked image these tables are FALSE.

(define (install-shared-bindings!+gc imported-bindings exported-bindings)
  (if (vm-eq? imported-bindings false)
      (begin
	(set! *imported-bindings* (make-hash-table+gc))
	(set! *exported-bindings* (make-hash-table+gc)))
      (begin
	(set! *imported-bindings* imported-bindings)
	(set! *exported-bindings* exported-bindings))))

(define shared-binding-table-size
  (* hash-table-size 2))

(let ((tracer (table-tracer shared-binding-next
			    set-shared-binding-next!
			    s48-trace-value)))
  (add-gc-root!
    (lambda ()
      (set! *imported-bindings* (tracer *imported-bindings*))
      (set! *exported-bindings* (tracer *exported-bindings*)))))

; These next two procedure are used to write the bindings tables out in images.

(define (s48-exported-bindings)
  *exported-bindings*)

(define (s48-imported-bindings)
  *imported-bindings*)

; Imported bindings.

(define lookup-imported-binding
  (let* ((maker (lambda (string next key)
		  (make-shared-binding string true unspecific-value next key)))
	 (lookup (table-searcher shared-binding-name
				 shared-binding-next
				 maker)))
    (lambda (name key)
      (lookup *imported-bindings* name key))))
    
; Exporting bindings.

(define lookup-exported-binding
  (let* ((maker (lambda (string next key)
		  (make-shared-binding string false unspecific-value next key)))
	 (lookup (table-searcher shared-binding-name
				 shared-binding-next
				 maker)))
    (lambda (name key)
      (lookup *exported-bindings* name key))))

; Print warnings about all imported bindings which the external code
; has not yet defined.

(define s48-warn-about-undefined-imported-bindings
  (let ((walker (table-walker shared-binding-next)))
    (lambda ()
      (let ((out (current-error-port)))
	(walker (lambda (binding)
		  (if (undefined? (shared-binding-ref binding))
		      (let ((name (shared-binding-name binding)))
			(write-string "undefined imported name " out)
			(if (vm-string? name)
			    (write-string (extract-string name) out)
			    (write-string "<invalid name>" out))
			(write-char #\newline out))))
		*imported-bindings*)))))

;----------------
; Four primitives for adding and removing imports and exports.

(define-consing-primitive lookup-shared-binding (string-> boolean->)
  (lambda (ignore)
    shared-binding-size)
  (lambda (name is-import? key)
    (goto return
	  (if is-import?
	      (lookup-imported-binding name key)
	      (lookup-exported-binding name key)))))

(define undefine-shared-binding!
  (table-remover shared-binding-name
		 shared-binding-next
		 set-shared-binding-next!))

(define-primitive undefine-shared-binding (string-> boolean->)
  (lambda (name is-import?)
    (undefine-shared-binding! (if is-import?
				  *imported-bindings*
				  *exported-bindings*)
			      name)
    (goto continue 0)))

;----------------
; The following two functions are exported to C, hence the reversal of the
; export/import terminology.

(define (s48-define-exported-binding name value)
  (save-temp0! value)
  (let ((name (enter-string+gc name)))
    (save-temp1! name)
    (let ((key (ensure-space shared-binding-size)))
      (let ((name (recover-temp1!))
	    (value (recover-temp0!)))
	(shared-binding-set! (lookup-imported-binding name key)
			     value)))))

(define (s48-get-imported-binding name)
  (save-temp0! (enter-string+gc name))
  (let* ((key (ensure-space shared-binding-size))
	 (name (recover-temp0!)))
    (lookup-exported-binding name key)))
