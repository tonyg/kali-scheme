; C variable declarations.
; 
; (write-function-prototypes forms port)
;
; (write-variable-declarations vars port indent)

; Writing declarations.

(define (write-function-prototypes forms port)
  (for-each (lambda (f)
	      (if (eq? (form-type f) 'lambda)
		  (if (form-tail-called? f)
		      (write-function-tail-prototype (form-var f) port)
		      (write-function-prototype (form-var f) port))))
	    forms))

(define (write-function-tail-prototype var port)
  (display "long T" port)
  (write-c-identifier (variable-name var) port)
  (display "(void)" port)
  (newline port))

(define (write-function-prototype var port)
  (receive (result args)
      (parse-arrow-type (final-variable-type var))
    (display-c-type result
		    (lambda (port)
		      (write-c-identifier (variable-name var) port))
		    port)
    (write-char #\( port)
    (if (null? args)
	(display "void" port)
	(begin
	  (display-c-type (car args) #f port)
	  (let loop ((args (cdr args)))
	    (if (not (null? args))
		(begin
		  (display ", " port)
		  (display-c-type (car args) #f port)
		  (loop (cdr args)))))))
    (display ");" port)
    (newline port)))

(define (write-variable-declarations vars port indent)
  (for-each (lambda (var)
	      (let ((type (final-variable-type var)))
		(if (not (or (eq? type type/unit)
			     (eq? type type/null)))
		    (really-write-variable-declaration var type port indent))))
	    vars))

(define (really-write-variable-declaration var type port indent)
  (indent-to port indent)
  (display-c-type type
		  (lambda (port)
		    (c-variable-no-shadowing var port))
		  port)
  (writec port #\;))

;----------------------------------------------------------------
; Writing C types

(define (display-c-type type name port)
  (display-c-base-type (type->c-base-type type) port)
  (if name (display " " port))
  (display-c-type-modifiers type name port))

(define (write-c-coercion type out)
  (write-char #\( out)
  (display-c-type type #f out)
  (write-char #\) out))
  
; Searches through the type modifiers until the base type is found.

(define (type->c-base-type type)
  (let ((type (maybe-follow-uvar type)))
    (cond ((or (base-type? type)
	       (record-type? type))
	   type)
	  ((pointer-type? type)
	   (type->c-base-type (pointer-type-to type)))
	  ((arrow-type? type)
	   (let ((res (arrow-type-result type)))
	     (cond ((not (tuple-type? res))
		    (type->c-base-type res))
		   ((null? (tuple-type-types res))
		    type/unit)
		   (else
		    (type->c-base-type (car (tuple-type-types res)))))))
	  (else
	   (bug "don't know how to write ~S as a C type" type)))))

; Table of C names for base types.

(define c-decl-table (make-integer-table))

(define (add-c-type-declaration! type decl)
  (table-set! c-decl-table (base-type-uid type) decl))

(for-each (lambda (p)
	    (let ((type (lookup-type (car p))))
	      (add-c-type-declaration! type (cadr p))))
	  '((boolean "unsigned char")
	    (int7u   "unsigned char")
	    (int8    "signed char")
	    (int8u   "unsigned char")
	    (int32   "long")
	    (input-port "FILE *")
	    (output-port "FILE *")
	    (unit    "void")
	    (null    "void")))

(define (display-c-base-type type port)
  (cond ((record-type? type)
	 (display "struct " port)
	 (write-c-identifier (record-type-name type) port))
	(else
	 (display (or (table-ref c-decl-table (base-type-uid type))
		      (bug "no C declaration for ~S" type))
		  port))))

; Writes out the modifiers of TYPE with NAME used when the base type is reached.

(define (display-c-type-modifiers type name port)
  (let label ((type type) (name name))
    (let ((type (maybe-follow-uvar type)))
      (cond ((or (base-type? type)
		 (record-type? type))
	     (if name (name port)))
	    ((pointer-type? type)
	     (label (pointer-type-to type)
		    (lambda (port)
		      (format port "*")
		      (if name (name port)))))
	    ((arrow-type? type)
	     (format port "(*")
	     (receive (return-type args)
		 (parse-arrow-type type)
	       (label return-type name)
	       (format port ")(")
	       (cond ((null? args)
		      (display "void" port))
		     (else
		      (display-c-type (car args) #f port)
		      (do ((args (cdr args) (cdr args)))
			  ((null? args))
			(display ", " port)
			(display-c-type (car args) #f port))))
	       (format port ")")))
	    (else
	     (bug "don't know how to write ~S as a C type" type))))))

(define (parse-arrow-type type)
  (receive (first rest)
      (parse-return-type (arrow-type-result type))
    (values first
	    (append (arrow-type-args type)
		    (map make-pointer-type rest)))))

(define (parse-return-type type)
  (cond ((not (tuple-type? type))
	 (values type '()))
	((null? (tuple-type-types type))
	 (values type/unit '()))
	(else
	 (values (car (tuple-type-types type))
		 (cdr (tuple-type-types type))))))

;------------------------------------------------------------

(define (declare-local-vars node port)
  (if (not (proc-lambda? node))
      (write-variable-declarations (filter used? (lambda-variables node))
				   port
				   2))
  (write-variable-declarations (gather-local-vars (lambda-body node)) port 2))

; Walk NODE finding all of the local variables.  Some primops are given
; explicit continuations because they need to generate C statements instead
; of expressions.

(define (gather-local-vars node)
  (define (do-cont cont vars)
    (do-call (lambda-body cont)
	     (append (filter used? (lambda-variables cont)) vars)))
    
  (define (do-call call vars)
    (let* ((call (expand-nasty-c-primops! call))
	   (args   (call-args   call))
	   (exits  (call-exits  call)))
      (cond ((or (= exits 0)
		 (goto-call? call))
	     (reverse! vars))
	    ((> exits 1)
	     (do ((i (- exits 1) (- i 1))
		  (vars vars (do-cont (vector-ref args i) vars)))
		 ((= i 0)
		  (do-cont (vector-ref args 0) vars))))
	    (else
	     (let* ((cont (vector-ref args 0))
		    (next (lambda-body cont)))
	       (case (primop-id (call-primop call))
		 ((let)
		  (do-call next
			   (append (gather-local-vars-in-let cont args) vars)))
		 ((letrec1 letrec2)  ; no reasonable variables
		  (do-call next vars))
		 ((call unknown-call)
		  ; extra result variables have to be declared even if not used
		  (let ((cont-vars (lambda-variables cont)))
		    (do-call next
			     (append (if (used? (car cont-vars))
					 cont-vars
					 (cdr cont-vars))
				     vars))))
		 (else
		  (do-cont cont vars))))))))

  (do-call node '()))

(define (gather-local-vars-in-let proc args)
  (let loop ((i 1) (vs (lambda-variables proc)) (vars '()))
    (cond ((>= i (vector-length args))
	   vars)
	  ((lambda-node? (vector-ref args i))
	   (loop (+ i 1) (cdr vs) vars))
	  (else
	   (loop (+ i 1) (cdr vs)
		 (if (used? (car vs))
		     (cons (car vs) vars)
		     vars))))))

(define (goto-call? call)
  (and (calls-this-primop? call 'unknown-tail-call)
       (goto-protocol? (literal-value (call-arg call 2)))))

(define (jumped-to? var)
  (any? (lambda (n)
	  (and (calls-this-primop? (node-parent n) 'jump)
	       (= 0 (node-index n))))
	(variable-refs var)))

; Some primops must be given continuations so that calls to them will
; be translated into separate C statements and so expand into arbitrarily
; complex chunks of C if necessary.

(define (expand-nasty-c-primops! call)
  (let ((top call))
    (let label ((call call))
      (cond ((call-node? call)
	     (if (and (= 0 (call-exits call))
		      (nasty-c-primop-call? call))
		 (set! top (expand-nasty-c-primop! call top)))
	     (walk-vector label (call-args call)))))
    top))

(define (nasty-c-primop-call? call)
  (case (primop-id (call-primop call))
    ((lshl ashl ashr)     ; C does poorly when shifting by large amounts
     (not (literal-node? (call-arg call 1))))
    (else #f)))

; Give CALL a continuation and move it above TOP, replacing CALL
; with the continuation's variable.
;
; top = (p1 ... (p2 a1 ...) ...)
;  =>
; (p2 (lambda (v) (p1 ... v ...)) a1 ...)

(define (expand-nasty-c-primop! call top)
  (let* ((var (make-variable 'x (node-type call)))
	 (cont (make-lambda-node 'c 'cont (list var))))
    (move call
	  (lambda (call)
	    (make-reference-node var)))
    (insert-body call
		 cont
		 (node-parent top))
    (set-call-exits! call 1)
    (insert-call-arg call 0 cont)
    call))

;------------------------------------------------------------
; Declare the variables used to pass arguments to procedures.
; This is done in each procedure so that the C compiler doesn't have to contend
; with the possibility of globally visible side-effects.

(define (write-arg-variable-declarations lambdas merged port)
  (let ((lambdas (filter (lambda (l)
			   (eq? 'jump (lambda-type l)))
			 lambdas))
	(merged (map form-value merged)))
    (really-write-arg-variable-declarations lambdas "arg" port 2)
    (really-write-arg-variable-declarations merged "merged_arg" port 2)))

(define (write-global-arg-variable-declarations forms port)
  (let ((lambdas (filter-map (lambda (f)
			       (if (and (form-var f)
					(memq? 'tail-called
					       (variable-flags (form-var f))))
				   (form-value f)
				   #f))
			     forms)))
    (really-write-arg-variable-declarations lambdas "goto_arg" port 0)))
  
(define (really-write-arg-variable-declarations lambdas name port indent)
  (for-each (lambda (data)
	      (destructure (((uid type . indicies) data))
		(if (not (eq? type type/unit))
		    (for-each (lambda (i)
				(indent-to port indent)
				(declare-arg-variable type uid i name port))
			      indicies))))
	    (get-variable-decl-data lambdas)))

(define (get-variable-decl-data lambdas)
  (let ((data '()))
    (for-each (lambda (l)
		(do ((vars (if (eq? 'jump (lambda-type l))
			       (lambda-variables l)
			       (cdr (lambda-variables l)))
			   (cdr vars))
		     (i 0 (+ i 1)))
		    ((null? vars))
		  (let* ((type (final-variable-type (car vars)))
			 (uid (type->uid type))
			 (datum (assq uid data)))
		    (cond ((not datum)
			   (set! data (cons (list uid type i) data)))
			  ((not (memq i (cddr datum)))
			   (set-cdr! (cdr datum) (cons i (cddr datum))))))))
	      lambdas)
    data))

(define (declare-arg-variable type uid i name port)
  (display-c-type type
		  (lambda (port)
		    (format port "~A~DK~D" name uid i))
		  port)
  (format port ";~%"))

;------------------------------------------------------------

(define (write-argument-initializers arg-vars port indent)
  (really-write-argument-initializers arg-vars "arg" #f port indent))

(define (write-merged-argument-initializers arg-vars port indent)
  (really-write-argument-initializers arg-vars "merged_arg" #f port indent))

(define (write-global-argument-initializers arg-vars port indent)
  (really-write-argument-initializers arg-vars "goto_arg" #t port indent))

(define (really-write-argument-initializers arg-vars name type? port indent)
  (do ((i 0 (+ i 1))
       (vars arg-vars (cdr vars)))
      ((null? vars) (values))
    (if (used? (car vars))
	(let* ((var (car vars))
	       (type (final-variable-type var)))
	  (cond ((not (eq? type/unit type))
		 (indent-to port indent)
		 (if type?
		     (display-c-type type
				     (lambda (port) (c-variable var port))
				     port)
		     (c-variable var port))
		 (display " = " port)
		 (display (c-argument-var name type i port) port)
		 (write-char '#\; port)))))))

(define (c-argument-var name type i port)
  (format #f "~A~DK~D" name (type->uid type) i))

(define *type-uids* '())
(define *next-type-uid* 0)

(define (type->uid type)
  (cond ((any (lambda (p)
		(type-eq? type (car p)))
	      *type-uids*)
	 => cdr)
	(else
	 (let ((id *next-type-uid*))
	   (set! *next-type-uid* (+ id 1))
	   (set! *type-uids* (cons (cons type id) *type-uids*))
	   id))))

;----------------------------------------------------------------
; random type stuff

(define (reference-type node)
  (finalize-variable-type (reference-variable node)))

(define (finalize-variable-type var)
  (let* ((type (finalize-type (variable-type var)))
	 (type (if (uvar? type)
		   type/null
		   type)))
    (set-variable-type! var type)
    type))

(define final-variable-type finalize-variable-type)

