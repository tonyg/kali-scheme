; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.

; Move nested procedures out to top level.  We move them all out, then merge
; as many as possible back together (see merge.scm), and finally check to
; see if there are any out-of-scope references.

(define (hoist-nested-procedures forms)
  (set! *hoist-index* 0)
  (let loop ((forms forms) (done '()))
    (if (null? forms)
	(reverse done)
	(loop (cdr forms)
	      (let ((form (car forms)))
		(if (eq? 'lambda (form-type form))
		    (append (really-hoist-nested-procedures form)
			    (cons form done))
		    (cons form done)))))))

(define (really-hoist-nested-procedures form)
  (let ((top     (form-value form))
	(lambdas (form-lambdas form))
	(lambda-parent lambda-env)	; Rename a couple of handy fields
	(lambda-kids lambda-block)
	(new-forms '()))
;    (format #t " ~S: ~S~%" (form-name form) lambdas)
;    (if (eq? 'read-list (form-name form))
;	(breakpoint "read-list"))
    (receive (procs others)
	(find-scoping lambdas
		      lambda-env set-lambda-env!
		      lambda-block set-lambda-block!)
      (set-form-lambdas! form (cons top (non-proc-lambdas (lambda-kids top))))
      (map (lambda (proc)
	     (let ((var (replace-with-variable proc)))
	       (make-hoist-form proc
				var
				(form-name form)
				(non-proc-lambdas (lambda-kids proc)))))
	   (filter (lambda (p)
		     (not (eq? p top)))
		   procs)))))

(define (non-proc-lambdas lambdas)
  (filter (lambda (l)
	    (not (or (eq? 'proc (lambda-type l))
		     (eq? 'known-proc (lambda-type l)))))
	  lambdas))

(define (make-hoist-form value var hoisted-from lambdas)
  (let ((form (make-form var #f #f)))
    (set-form-node! form value (cons value lambdas))
    (set-form-type! form 'lambda)
    (set-variable-flags! var
			 (cons (cons 'hoisted hoisted-from)
			       (variable-flags var)))
    form))

(define (replace-with-variable node)
  (let ((var (make-hoist-variable node)))
    (case (primop-id (call-primop (node-parent node)))
      ((let)
       (substitute-var-for-proc (node-parent node) node var))
      ((letrec2)
       (substitute-var-for-proc-in-letrec (node-parent node) node var))
      (else
       (move node
	     (lambda (node)
	       (make-reference-node var)))))
    var))

(define (make-hoist-variable node)
  (cond ((bound-to-variable node)
	 => (lambda (var)
	      (make-global-variable (generate-hoist-name (variable-name var))
				    (variable-type var))))
	(else
	 (let* ((vars (lambda-variables node))
		(type (make-arrow-type (map variable-type (cdr vars))
				       (variable-type (car vars))))
		(id (generate-hoist-name (or (lambda-name node) 'hoist))))
	   (make-global-variable id type)))))
    
(define (substitute-var-for-proc call node value-var)
  (let ((proc (call-arg call 0)))
    (really-substitute-var-for-proc proc call node value-var)
    (if (null? (lambda-variables proc))
        (replace-body call (detach-body (lambda-body proc))))))

(define (substitute-var-for-proc-in-letrec call node value-var)
  (let ((proc (node-parent call)))
    (really-substitute-var-for-proc proc call node value-var)
    (if (null? (cdr (lambda-variables proc)))
        (replace-body (node-parent proc)
		      (detach-body (lambda-body (call-arg call 0)))))))

(define (really-substitute-var-for-proc binder call node value-var)
  (let* ((index (node-index node))
         (var (list-ref (lambda-variables binder)
			(- (node-index node) 1))))
    (walk-refs-safely
     (lambda (ref)
       (replace ref (make-reference-node value-var)))
     var)
    (remove-variable binder var)
    (detach node)
    (remove-call-arg call index)))

(define *hoist-index* 0)

(define (generate-hoist-name sym)
  (let ((i *hoist-index*))
    (set! *hoist-index* (+ i 1))
    (concatenate-symbol sym "." i)))

;----------------------------------------------------------------
; Part 2: checking for variables moved out of scope.

(define (check-hoisting forms)
  (let ((forms (filter (lambda (form)
			 (or (eq? 'merged (form-type form))
			     (eq? 'lambda (form-type form))))
		       forms)))
    (for-each (lambda (form)
		(cond ((flag-assq 'hoisted (variable-flags (form-var form)))
		       => (lambda (p)
			    (check-hoisted-form form (cdr p))))))
	      forms)))

(define (check-hoisted-form form hoisted-from)
  (let ((vars (find-unbound-variables (form-value form) (form-head form))))
    (if (not (null? vars))
	(user-error "Procedure ~S in ~S is closed over: ~S~%"
		    (form-name form)
		    hoisted-from
		    (map variable-name vars)))))

(define (find-unbound-variables node form)
  (let ((unbound '())
	(mark (cons 0 0)))
    (let label ((n node))
      (cond ((lambda-node? n)
	     (let ((flag (node-flag n)))
	       (set-node-flag! n mark)
	       (label (lambda-body n))
	       (set-node-flag! n flag)))
	    ((call-node? n)
	     (let ((vec (call-args n)))
	       (do ((i 0 (+ i 1)))
		   ((= i (vector-length vec)))
		 (label (vector-ref vec i)))))
	    ((reference-node? n)
	     (let* ((v (reference-variable n))
		    (b (variable-binder v)))
	       (cond ((and b
			   (not (eq? mark (node-flag b)))
			   (not (variable-flag v)))
		      (set-variable-flag! v #t)
		      (set! unbound (cons v unbound))))))))
    (filter (lambda (v)
	      (set-variable-flag! v #f)
	      (not (eq? form (form-head (node-form (variable-binder v))))))
	    unbound)))
