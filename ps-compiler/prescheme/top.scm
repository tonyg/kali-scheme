; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.

; Entry point

(define (prescheme-compiler package-id spec-files init-name c-file . commands)
  (reset-node-id)
  (initialize-lambdas)
  (reset-record-data!)
  (reset-type-vars!)
  (receive (copy no-copy shadow integrate header)
      (parse-prescheme-commands commands)
    (let ((forms (prescheme-front-end (if (list? package-id)
					  package-id
					  (list package-id))
				      spec-files copy no-copy shadow)))
      (for-each simplify-form forms)
      (let ((forms (remove-unreferenced-forms forms)))
	(for-each integrate-stob-form forms) 
	; prevent further automatic integration
	(for-each (lambda (form)
		    (remove-variable-known-value! (form-var form)))
		  forms)
	(integrate-by-command integrate forms)
	(for-each resimplify-form forms)
	(let* ((forms (remove-unreferenced-forms forms))
	       (forms (integrate-single-uses forms))
	       (forms (remove-unreferenced-forms forms)))
	  (for-each resimplify-form forms)
	  (for-each determine-form-protocol forms)
	  (let ((forms (form-tail-calls->jumps forms)))
	    (for-each maybe-add-self-label forms)
	    (let ((forms (hoist-nested-procedures forms)))
	      (for-each remove-polymorphism forms)
	      ; (if cps-file (write-cps-file cps-file forms))
	      (if c-file (write-c-file init-name c-file header forms)))))))))

;(define (expand-and-eval-program package-id spec-files output-file . commands)
;  (reset-node-id)
;  (reset-record-data!)
;  (receive (copy no-copy shadow integrate header)
;      (parse-prescheme-commands commands)
;    (let ((forms (prescheme-front-end package-id spec-files copy no-copy shadow)))
;      (call-with-output-file output-file
;        (lambda (out)
;          (display-forms-as-scheme forms out))))))

;(define (simplify-and-print-program package-id spec-files output-file c-file . commands)
;  (reset-node-id)
;  (reset-record-data!)
;  (receive (copy no-copy shadow integrate header)
;      (parse-prescheme-commands commands)
;    (let ((forms (prescheme-front-end package-id spec-files copy no-copy shadow)))
;      (for-each simplify-form forms)
;      (let ((forms (remove-unreferenced-forms forms)))
;        (call-with-output-file output-file
;          (lambda (out)
;            (display-cps-forms-as-scheme forms out)))))))

(define command-names '(copy no-copy shadow integrate header))

(define (parse-prescheme-commands commands)
  (let ((res (map list command-names)))
    (for-each (lambda (command)
		(cond ((assq (car command) res)
		       => (lambda (l)
			    (set-cdr! l (append (reverse (cdr command))
						(cdr l)))))
		      (else
		       (error "unknown directive ~S" command))))
	      commands)
    (apply values (map (lambda (l) (reverse (cdr l))) res))))

;--------------------------------------------------

(define (simplify-form form)
  (format #t " ~A " (form-name form))
  (let ((status (expand-and-simplify-form form)))
    (if status
	(format #t "(~A): " status)
	(format #t ": "))
    (display-type (variable-type (form-var form))
		  (current-output-port))
    (newline (current-output-port))))

;--------------------------------------------------

(define (integrate-single-uses forms)
  (format #t "In-lining single-use procedures~%")
  (let loop ((forms forms) (done '()) (hit? #f))
    (cond ((null? forms)
	   (if hit?
	       (loop (reverse done) '() #f)
	       (reverse done)))
	  ((single-called-use? (car forms))
	   (let ((form (car forms)))
;	     (format #t " ~S~%" (variable-name (form-var form)))
	     (integrate-single-use form
				   (car (variable-refs (form-var form)))
				   #f)
	     (set-form-value! form #f)
	     (make-form-unused! form)
	     (loop (cdr forms) done #t)))
	  (else
	   (loop (cdr forms) (cons (car forms) done) hit?)))))

(define (single-called-use? form)
  (let ((var (form-var form)))
    (and (not (form-exported? form))
	 (eq? (form-type form) 'lambda)
	 (not (null? (variable-refs var)))
	 (null? (cdr (variable-refs var)))
	 (called-node? (car (variable-refs var))))))

(define (integrate-single-use form ref copy?)
  (let* ((in-node (node-base ref))
	 (in-form (node-form in-node))
	 (type (variable-type (form-var form))))
    (use-this-form! in-form)
    (let ((node (cond (copy?
		       (copy-node-tree (form-node form)))
		      (else
		       (also-use-this-form! form)
		       (form-node form)))))
      (if (type-scheme? type)
	  (if (not (called-node? ref))
	      (error "integrating polymorphic value into non-call position")
	      (instantiate-type&value type node ref)))
      (determine-lambda-protocol node (list ref))
      (replace ref node)
      (simplify-all in-node (form-name form))
      (suspend-form-use! in-form))))

; Commands are (<proc> <caller>)

(define (integrate-by-command commands forms)
  (for-each (lambda (command)
	      (receive (proc refs)
		  (process-integrate-command command forms)
		(if proc
		    (for-each (lambda (r)
				(integrate-single-use proc r #t))
			      refs))))
	    commands))

; Horrendous error checking and notification.

(define (process-integrate-command command forms)
  (let* ((proc (any (lambda (f)
		      (eq? (form-name f) (car command)))
		    forms))
	 (var (if proc (form-var proc) #f))
	 (node (if proc (form-value proc) #f))
	 (caller (any (lambda (f)
			(eq? (form-name f) (cadr command)))
		      forms))
	 (refs (if (and var caller)
		   (filter (lambda (ref)
			     (eq? caller (node-form ref)))
			   (variable-refs var))
		   #f)))
    (cond ((or (not proc) (not var) (not caller))
	   (cond ((or (not proc) (not var))
		  (format #t "Bad command: no value for ~S~%"
			  (car command)))
		 ((or (not node)
		      (not (lambda-node? node)))
		  (format #t "Bad command: ~S is not a procedure~%"
			  (car command))))
	   (if (not caller)
	       (format #t "Bad command: no definition for ~S~%"
		       (cadr command)))
	   (values #f #f))
	  ((or (null? refs) (not node) (not (lambda-node? node)))
	   (if (null? refs)
	       (format #t "Bad command: ~S is not referenced by ~S~%"
		       (car command) (cadr command)))
	   (if (or (not node)
		   (not (lambda-node? node)))
	       (format #t "Bad command: ~S is not a procedure~%"
		       (car command)))
	   (values #f #f))
	  (else
	   (values proc refs)))))

;--------------------------------------------------

(define (determine-form-protocol form)
  (let ((var (form-var form)))
    (cond ((and (not (form-exported? form))
		(eq? 'lambda (form-type form))
		(every? called-node? (variable-refs var)))
	   (determine-lambda-protocol (form-node form) (variable-refs var))
	   (note-known-global-lambda! var (form-node form))))))

;--------------------------------------------------

(define (form-tail-calls->jumps forms)
  (receive (hits useless)
      (find-jump-procs (filter-map (lambda (form)
				     (if (eq? 'lambda (form-type form))
					 (form-node form)
					 #f))
				   forms)
		       find-form-proc-calls)
    (for-each (lambda (p)
		(let* ((procs (cdr p))
		       (proc-forms (map node-form procs))
		       (owner (node-flag (node-base (car p))))
		       (vars (map form-var proc-forms)))
		  (use-this-form! owner)
		  (for-each also-use-this-form! proc-forms)
		  (procs->jumps (cdr p) vars (car p))
		  (simplify-node (form-value owner)) ; worth it?
		  (suspend-form-use! owner)
		  (for-each (lambda (f)
			      (set-form-value! f #f)
			      (make-form-unused! f))
			    proc-forms)))
	      hits)
    (for-each (lambda (p)
		(make-form-unused! (node-form p)))
	      useless)
    (filter (lambda (f)
	      (not (eq? (form-type f) 'unused)))
	    forms)))

(define (find-form-proc-calls l)
  (let ((refs (variable-refs (form-var (node-form l)))))
    (cond ((and refs (every? called-node? refs))
	   refs)
	  ((calls-known? l)
	   (bug "cannot find calls for known lambda ~S" l))
	  (else #f))))

;--------------------------------------------------
; Determine an actual type for a polymorphic procedure.

(define (remove-polymorphism form)
  (if (and (null? (variable-refs (form-var form)))
	   (eq? 'lambda (form-type form)))
      (for-each (lambda (var)
		  (if (and (null? (variable-refs var))
			   (uvar? (maybe-follow-uvar (variable-type var))))
		      (unused-variable-warning var form)))
		(cdr (lambda-variables (form-node form)))))
  (if (type-scheme? (variable-type (form-var form)))
      (make-monomorphic! (form-var form))))

(define (unused-variable-warning var form)		
  (format #t "Warning: argument `~S' of `~S' is not used, and `~S' is not called;~%"
	  (variable-name var) (form-name form) (form-name form))
  (format #t "  assuming the type of argument `~S' of procedure `~S' is `long'.~%"
	  (variable-name var) (form-name form))
  (set-variable-type! var type/integer))

;--------------------------------------------------

; Various methods for getting values from thunks.  These are no longer used
; here.

(define (thunk-value thunk)
  (let ((refs (variable-refs (car (lambda-variables thunk)))))
    (if (= 1 (length refs))
        (call-arg (node-parent (car refs)) 2)
        #f)))

(define (simple-thunk? thunk value)
  (eq? (node-parent (node-parent value)) thunk))

;----------------------------------------------------------------
; Turning internal tail-recursive calls to jumps.

; f = (proc (c . vars)
;       ... ([unknown-]tail-call c f . args) ...)
;  =>
; f = (proc (c . vars)
;       (letrec ((f' (jump . vars) ... (jump f' . args) ...))
;         (jump f' . vars)))

(define (maybe-add-self-label form)
  (if (eq? 'lambda (form-type form))
      (let* ((node (form-node form))
	     (self-calls (filter (lambda (ref)
				   (and (eq? (node-index ref) 1)
					(calls-this-primop? (node-parent ref)
							    (if (calls-known? node)
								'tail-call
								'unknown-tail-call))
					(eq? node (node-base ref))))
				 (variable-refs (form-var form)))))
	(if (not (null? self-calls))
	    (begin
	      (use-this-form! form)
	      (replace-self-calls-with-jumps node self-calls)
	      (suspend-form-use! form))))))

(define (replace-self-calls-with-jumps proc refs)
  (let* ((outside-var (reference-variable (car refs)))
	 (var (make-variable (variable-name outside-var)
			     (variable-type outside-var)))
	 (vars (map copy-variable (cdr (lambda-variables proc))))
	 (args (map make-reference-node vars))
	 (body (lambda-body proc))
	 (jump-proc (make-lambda-node (lambda-name proc) 'jump vars)))
    (for-each (lambda (ref)
		(let ((call (node-parent ref)))
		  (if (not (calls-known? proc))
		      (remove-call-arg call 2))  ; remove TAIL? argument
		  (remove-call-arg call 0)       ; remove continuation argument
		  (replace (call-arg call 0) (make-reference-node var))
		  (set-call-primop! call (get-primop (enum primop jump)))))
	      refs)
    (let-nodes ((call (jump 0 (* var) . args)))
      (move-body body (lambda (body)
			(attach-body jump-proc body)
			call))
      (put-in-letrec (list var) (list jump-proc) call))))
    
