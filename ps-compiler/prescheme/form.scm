; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; temporary hack
;(define enqueue! enqueue)
;(define dequeue! dequeue)

(define-record-type form
  (var              ; variable being defined (if any)
   (value)          ; current value
   ;source           ; one line of source code
   (free)           ; variables free in this form
   )

  (used?            ; is the value used in the program
   (exported? #f)   ; true if the definition in this form is exported
   (integrate 'okay) ; one of OKAY, YES, NO, PARTIAL
   (aliases   '())  ; variables that are aliases for this one
   (shadowed  '())  ; package variables that should be shadowed here

   value-type       ; value's type

   (dependency-index #f) ; index of this form in the data dependent order

   lambdas          ; list of all non-cont lambdas in this form

   (clients '())    ; forms that use this one's variable
   (providers '())  ; forms that define a variable used by this one
   
   (type #f)        ; one of LAMBDA, INTEGRATE, INITIALIZE or
                    ;  #F for unfinished forms
   merge            ; slot used by form-merging code
   temp             ; handy slot
   ))

(define-record-discloser type/form
  (lambda (form)
    `(form ,(variable-name (form-var form)))))

(define (make-form var value free)
  (let ((form (form-maker var value free)))
    (if (maybe-variable->form var)
	(error "more than one definition of ~S" (variable-name var)))
    (set-variable-flags! var `((form . ,form) . ,(variable-flags var)))
    form))

(define (pp-one-line x)
  (call-with-string-output-port
   (lambda (p)
     (write-one-line p 70 (lambda (p) (write x p))))))

(define (form-node form)
  (let ((value (form-value form)))
    (if (node? value)
	value
	(bug "form's value is not a node ~S ~S" form value))))

(define (set-form-node! form node lambdas)
  (set-node-flag! node form)
  (set-form-value! form node)
  (set-form-lambdas! form lambdas))

(define (node-form node)
  (let ((form (node-flag (node-base node))))
    (if (form? form)
	form
	(bug "node ~S (~S) not in any form" node (node-base node)))))

(define (suspend-form-use! form)
  (set-form-lambdas! form (make-lambda-list))
  (set-node-flag! (form-node form) form))

(define (use-this-form! form)
  (initialize-lambdas)
  (also-use-this-form! form))

(define (also-use-this-form! form)
  (add-lambdas (form-lambdas form))
  (set-node-flag! (form-node form) #f))

(define (form-name form)
  (variable-name (form-var form)))

(define (make-form-unused! form)
  (set-form-type! form 'unused)
  (cond ((node? (form-value form))
	 (erase (form-value form))
	 (set-form-value! form #f)
	 (set-form-lambdas! form #f))))

; notes on writing and reading forms
; What we really need here are forms.
; What to do?  Can read until there are no missing lambdas = end of form
; Need the variables as well.

; (form index type var source? clients providers integrate?)
; clients and providers are lists of indicies
; can get lambdas automatically

;(define (write-cps-file file forms)
;  (let ((port (make-tracking-output-port (open-output-file file))))
;    (reset-pp-cps)
;    (walk (lambda (f)
;	    (write-form f port))
;	  (sort-list forms
;		     (lambda (f1 f2)
;		       (< (form-index f1) (form-index f2)))))
;    (close-output-port port)))

;(define (write-form form port)
;  (format port "(FORM ~D ~S ~S "
;	  (form-index form)
;	  (form-type form)
;	  (form-integrate form))
;  (if (form-var form)
;      (print-variable-name (form-var form) port)
;      (format port "#f"))
;  (format port "~%  ~S" (map form-index (form-clients form)))
;  (rereadable-pp-cps (form-value form) port)
;  (format port ")~%~%"))

;------------------------------------------------------------------------------
; Put the forms that do not reference any other forms' variables in a queue.
; Every form gets a list of forms that use its variable and a list of forms
; whose variables it uses.

(define (sort-forms forms)
  (let ((queue (make-queue)))
    (for-each (lambda (f)
		(set-variable-flag! (form-var f) f))
	      forms)
    (let ((forms (really-remove-unreferenced-forms
		  forms
		  set-providers-using-free)))
      (for-each (lambda (f)
		  (if (null? (form-providers f))
		      (enqueue! queue f)))
		(reverse forms))
      (for-each (lambda (f)
		  (set-variable-flag! (form-var f) #f))
		forms)
      (values forms (make-form-queue queue forms)))))

(define (set-providers-using-free form)
  (let loop ((vars (form-free form)) (provs '()))
    (cond ((null? vars)
	   (set-form-providers! form provs))
	  ((variable-flag (car vars))
	   => (lambda (prov)
		(set-form-clients! prov (cons form (form-clients prov)))
		(loop (cdr vars) (cons prov provs))))
	  (else
	   (loop (cdr vars) provs)))))

(define (make-form-queue ready forms)
  (let ((index 0))
    (lambda ()
      (let loop ()
	(cond ((not (queue-empty? ready))
	       (let ((form (dequeue! ready)))
		 (set-form-dependency-index! form index)
		 (for-each (lambda (f)
			     (set-form-providers! f (delq! form (form-providers f)))
			     (if (and (null? (form-providers f))
				      (not (form-dependency-index f))
				      (form-used? f))
				 (enqueue! ready f)))
			   (form-clients form))
		 (set! index (+ index 1))
		 form))
	      ((find-dependency-loop ready forms)
	       => (lambda (rest)
		    (set! forms rest)
		    (loop)))
	      (else #f))))))

; Find a circular dependence between the remaining forms.

(define (find-dependency-loop queue forms)
  (let ((forms (do ((forms forms (cdr forms)))
                   ((or (null? forms)
			(not (form-dependency-index (car forms))))
                    forms))))
    (cond ((null? forms)
           #f)
          (else
           ;;(format #t "Dependency loop!~%")
           (let ((form (really-find-dependency-loop forms)))
	     (if (not (every? (lambda (f) (eq? 'no (form-integrate f)))
			      (form-providers form)))
		 (set-form-integrate! form 'no))
             (set-form-providers! form '())
             (enqueue! queue form)
             forms)))))

(define (really-find-dependency-loop forms)
  (for-each (lambda (f) (set-form-temp! f #f))
	    forms)
  (let label ((form (car forms)))
    (cond ((form-temp form)
           (break-dependency-loop (filter (lambda (f)
					    (and (form-temp f) (form-var f)))
					  forms)))
          (else
           (set-form-temp! form #t)
           (cond ((any-map label (form-providers form))
                  => (lambda (res)
                       (set-form-temp! form #f)
                       res))
                 (else
                  (set-form-temp! form #f)
                  #f))))))

(define (any-map proc list)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
	  ((proc (car list))
	   => identity)
	  (else
	   (loop (cdr list))))))

(define *loop-forms* #f)

(define (break-dependency-loop forms)
  (or (first (lambda (f)
               (or (every? (lambda (f)
                             (eq? 'no (form-integrate f)))
                           (form-providers f))
                   (memq? f (form-providers f))
		   (and (scheme-node? (form-value f))
			(scheme-literal-node? (form-value f)))))
             forms)
      (begin (set! *loop-forms* forms)
	     (let ((f (breakpoint "Break dependency loop: *loop-forms* = ~S" forms)))
	       (set! *loop-forms* #f)
	       f))))

(define scheme-literal-node? 
  ((structure-ref nodes node-predicate) 'literal))

(define scheme-node? 
  (structure-ref nodes node?))

;----------------------------------------------------------------

(define (variable-set!? var)
  (memq 'set! (variable-flags var)))

(define (note-variable-set!! var)
  (if (not (variable-set!? var))
      (set-variable-flags! var (cons 'set! (variable-flags var)))))

;------------------------------------------------------------------------------
; Turn expression into nodes and simplify it.

; Still to do:
;  Get representations of data values
;  Need to constant fold vector slots, including detection of modifications
;    and single uses.

(define (expand-and-simplify-form form)
  (initialize-lambdas)
  (let* ((value (form-value form))
	 (node (if (variable? value)
		   (make-reference-node value)
		   (x->cps (form-value form) (form-name form)))))
    (cond ((variable-set!? (form-var form))
	   (set-form-type! form 'initialize)
	   (set-form-node! form node '())
	   "settable")
         ((reference-node? node)
	  (let ((var (reference-variable node)))
	    (add-known-form-value! form node)
	    (cond ((maybe-variable->form var)
		   => (lambda (f)
			(set-form-aliases! f
					   `(,(form-var form)
					     ,@(form-aliases form)
					     . ,(form-aliases f))))))
	    (set-form-type! form 'alias)
	    (erase node)
	    (set-form-value! form var)
	    "alias"))
	 ((literal-node? node)
	  (expand-and-simplify-literal node form))
	 ((lambda-node? node)
	  (expand-and-simplify-lambda node form))
	 (else
	  (bug "funny form value ~S" node)))))

; This could pay attention to immutability.

(define (atomic? value)
  (not (or (vector? value)
	   (pair? value))))

(define (expand-and-simplify-literal node form)
  (let ((value (literal-value node)))
    (cond ((unspecific? value)
	   (format #t "~%Warning: variable `~S' has no value and is not SET!~%"
		   (form-name form))
	   (set-form-value! form node)
	   (set-form-lambdas! form '())
	   (set-form-integrate! form 'no)
	   (set-form-type! form 'unused)
           "constant")
	  ((atomic? value)
	   (add-known-form-value! form node)
	   (set-form-value! form node)
	   (set-form-lambdas! form '())
	   "constant")
	  (else
	   (set-form-node! form (stob->node value) '())
	   (set-form-type! form 'stob)
	   "consed"))))

; Make a call node containing the contents of the stob so that any
; variables will be seen as referenced and any integrable values will
; be integrated.

; Only works for vectors at this point.
; MAKE-VECTOR is a randomly chosen primop, almost anything could be used.

(define (stob->node value)
  (let* ((contents '())
	 (add! (lambda (x) (set! contents (cons x contents)))))
    (cond ((vector? value)
	   (do ((i 0 (+ i 1)))
	       ((>= i (vector-length value)))
	     (add! (vector-ref value i))))
	  (else
	   (error "unknown kind of stob value ~S" value)))
    (let ((call (make-call-node (get-prescheme-primop 'make-vector)
				(+ 1 (length contents))
				0))
	  (node (make-lambda-node 'stob 'init '())))
      (attach call 0 (make-literal-node value #f)) ; save for future use
      (do ((i 1 (+ i 1))
	   (cs (reverse contents) (cdr cs)))
	  ((null? cs))
	(let ((x (car cs)))
	  (attach call i (if (variable? x)
			     (make-reference-node x)
			     (make-literal-node x type/unknown)))))
      (attach-body node call)
      (simplify-args call 1)
      node)))

(define (add-known-form-value! form value)
  (let ((node (if (variable? value)
		  (make-reference-node value)
		  value))
	(var (form-var form)))
    (set-form-type! form 'integrate)
    (cond ((or (literal-node? node)
	       (reference-node? node)
	       (and (call-node? node)
		    (trivial? node)))
	   (add-variable-known-value! var (node->vector node))
	   (if (variable? value)
	       (erase node)))
	  ((lambda-node? node)
	   (add-variable-simplifier! var (make-inliner (node->vector node))))
	  (else
	   (bug "form's value ~S is not a value" value)))))

(define (make-inliner vector)
  (lambda (call)
    (let ((proc (call-arg call 1)))
      (replace proc (reconstruct-value vector proc call)))))

(define (reconstruct-value value proc call)
  (let ((has-type (maybe-follow-uvar (variable-type (reference-variable proc))))
	(node (vector->node value)))
    (if (type-scheme? has-type)
	(instantiate-type&value has-type node proc))
    node))

(define (expand-and-simplify-lambda node form)
  (simplify-all node (form-name form))
  (let ((lambdas (make-lambda-list))
	(status (duplicate-form? form node)))
    (if status
	(add-known-form-value! form node))
    (set-form-node! form node lambdas)
    (set-form-type! form 'lambda)
    (set-form-free! form #f)   ; old value no longer valid
    status))

(define *duplicate-lambda-size* 10)

(define (set-duplicate-lambda-size! n)
  (set! *duplicate-lambda-size* n))

(define (duplicate-form? form node)
  (cond ((or (variable-set!? (form-var form))
	     (eq? 'no (form-integrate form)))
         #f)
        ((small-node? node *duplicate-lambda-size*)
	 "small")
	((eq? 'yes (form-integrate form))
	 "by request")
;        ((called-arguments? node)
;         "called arguments")
        (else
         #f)))

(define (called-arguments? node)
  (any? (lambda (v)
	  (any? (lambda (n)
		  (eq? n (called-node (node-parent n))))
		(variable-refs v)))
	(cdr (lambda-variables node))))

;------------------------------------------------------------------------------

(define (integrate-stob-form form)
  (if (and (eq? 'stob (form-type form))
	   (elide-aliases! form)
	   (not (form-exported? form))
	   (every? cell-use (variable-refs (form-var form))))
      (let* ((var (form-var form))
	     (ref (car (variable-refs var)))
	     (call (lambda-body (form-value form))))
	; could fold any fixed references - do it later
	(cond ((and (null? (cdr (variable-refs var)))
		    (called-node? (cell-use ref)))
	       (format #t "computed-goto: ~S~%" (variable-name var)) 
	       (make-computed-goto form))))))

(define (cell-use node)
  (let ((parent (node-parent node)))
    (if (and (call-node? parent)
	     (eq? 'vector-ref (primop-id (call-primop parent))))
	parent
	#f)))

(define (elide-aliases! form)
  (not (or-map (lambda (f)
		 (switch-references! (form-var f) (form-var form))
		 (form-exported? f))
	       (form-aliases form))))

(define (switch-references! from to)
  (for-each (lambda (r)
	      (set-reference-variable! r to))
	    (variable-refs from))
  (set-variable-refs! to (append (variable-refs from) (variable-refs to))))

;------------------------------------------------------------------------------

(define (resimplify-form form)
  (let ((node (form-value form)))
    (cond ((and (node? node)
		(not (eq? 'stob (form-type form)))
		(not (node-simplified? node)))
	   (use-this-form! form)
	   (simplify-node node)
	   (suspend-form-use! form)))))

;------------------------------------------------------------------------------
; This is removes all forms that are not ultimately referenced from some
; exported form.

(define (add-form-provider! form provider)
  (if (not (memq? provider (form-providers form)))
      (set-form-providers!
       form
       (cons provider (form-providers form)))))

(define (variable->form var)
  (or (maybe-variable->form var)
      (bug "variable ~S has no form" var)))

(define (maybe-variable->form var)
  (cond ((flag-assq 'form (variable-flags var))
	 => cdr)
	(else
	 #f)))

(define (remove-unreferenced-forms forms)
  (really-remove-unreferenced-forms forms set-form-providers))

(define (really-remove-unreferenced-forms forms set-form-providers)
  (receive (exported others)
      (partition-list form-exported? forms)
    (for-each (lambda (f)
		(set-form-providers! f '())
		(set-form-clients!   f '())
		(set-form-used?!     f (form-exported? f)))
	      forms)
    (for-each set-form-providers forms)
    (propogate-used?! exported)
    (append (remove-unused-forms others) exported)))

(define (set-form-providers form)
  (for-each (lambda (n)
	      (add-form-provider! (node-form n) form))
	    (variable-refs (form-var form)))
  (if (eq? (form-type form) 'alias)
      (add-form-provider! form (variable->form (form-value form)))))

(define (propogate-used?! forms)
  (let loop ((to-do forms))
    (if (not (null? to-do))
	(let loop2 ((providers (form-providers (car to-do)))
		    (to-do (cdr to-do)))
	  (if (null? providers)
	      (loop to-do)
	      (loop2 (cdr providers)
		     (let ((p (car providers)))
		       (cond ((form-used? p)
			      to-do)
			     (else
			      (set-form-used?! p #t)
			      (cons p to-do))))))))))

; Actually remove forms that are not referenced.

(define (remove-unused-forms forms)
;  (format #t "Removing unused forms~%")
  (filter (lambda (f)
	    (cond ((or (not (form-used? f))
		       )
		       ;(let ((value (form-value f)))
			; (and (quote-exp? value)
			 ;     (external-value? (quote-exp-value value))))
;		   (format #t " ~S~%" (variable-name (form-var f)))
		   (erase-variable (form-var f))
		   (cond ((node? (form-value f))
			  (erase (form-value f))
			  (set-form-value! f #f)
			  (set-form-lambdas! f '())))
		   #f)
		  (else #t)))
	  forms))

;------------------------------------------------------------
; Total yucko.

; (unknown-call (lambda e-vars e-body)
;               protocol
;               (vector-ref x offset)
;               . args)
; =>
; (let (lambda ,vars
;        (computed-goto
;          ...
;          (lambda ()
;            (unknown-call (lambda ,copied-evars
;                            (jump ,(car vars) ,copied-evars))
;                          ,(vector-ref proc-vector i)
;                          . ,(cdr vars)))
;          ...
;          '((offsets ...) ...)    ; offsets for each continuation
;          ,offset))
;      ,exit
;      . ,args)

(define (make-computed-goto form)
  (let* ((ref (car (variable-refs (form-var form))))
	 (in-form (node-form ref))
	 (entries (vector->offset-map (call-args (lambda-body (form-node form))))))
    (use-this-form! in-form)
    (also-use-this-form! form)
    (really-make-computed-goto (node-parent ref) entries)
    (erase (form-node form))
    (set-form-value! form #f)
    (set-form-lambdas! form #f)
    (simplify-node (form-node in-form))
    (suspend-form-use! in-form)))

; Returns a list ((<node> . <offsets>) ...) where <offsets> are where <node>
; was found in VECTOR.  The first element of VECTOR is a marker which we
; pretend isn't there.
;
; This would be more effective if done by a simplifier after the continuations
; had been simplified.

(define (vector->offset-map vector)
  (let loop ((i 0) (res '()))
    (if (= (+ i 1) (vector-length vector))
	(reverse (map (lambda (p)
			(cons (car p) (reverse (cdr p))))
		      res))
	(let ((n (vector-ref vector (+ i 1))))
	  (loop (+ i 1)
		(cond ((first (lambda (p)
				(node-equal? n (car p)))
			      res)
		       => (lambda (p)
			    (set-cdr! p (cons i (cdr p)))
			    res))
		      (else
		       (cons (list n i) res))))))))


(define (really-make-computed-goto vec-ref entries)
  (let* ((exits (length entries))
	 (offset (call-arg vec-ref 1))
	 (vector-call (node-parent vec-ref))
	 (args (sub-vector->list (call-args vector-call) 3))
	 (call (make-call-node (get-prescheme-primop 'computed-goto)
			       (+ 2 exits)
			       exits))
	 (arg-vars (map (lambda (arg) (make-variable 't (node-type arg)))
			args))
	 (protocol (literal-value (call-arg vector-call 2)))
	 (cont (call-arg vector-call 0)))
    (for-each detach args)
    (attach call exits (make-literal-node (map cdr entries) #f))
    (attach call (+ exits 1) (detach offset))
    (receive (top continuations)
	(if (reference-node? cont)
	    (make-computed-goto-tail-conts call args arg-vars entries cont protocol)
	    (make-computed-goto-conts call args arg-vars entries cont protocol))
      (do ((i 0 (+ i 1))
	   (l continuations (cdr l)))
	  ((= i exits))
	(attach call i (car l)))
      (replace-body vector-call top))))
    
(define (make-computed-goto-tail-conts call args arg-vars entries cont protocol)
  (let-nodes ((top (let 1 l1 . args))
	      (l1 arg-vars call))
    (values top (map (lambda (p)
		       (computed-goto-tail-exit
			(detach (car p))
			protocol
			(reference-variable cont)
			arg-vars))
		     entries))))

(define (computed-goto-tail-exit node protocol cont-var arg-vars)
  (let ((args (map make-reference-node arg-vars)))
    (let-nodes ((l1 () (unknown-tail-call 0 (* cont-var)
					  node
					  '(protocol #f) . args)))
      l1)))

(define (make-computed-goto-conts call args arg-vars entries cont protocol)
  (let ((cont-vars (lambda-variables cont))
	(cont-type (make-arrow-type (map variable-type
					 (lambda-variables cont))
				    type/null)))
    (detach cont)
    (change-lambda-type cont 'jump)
    (let-nodes ((top (let 1 l1 cont . args))
		(l1 ((j cont-type) . arg-vars) call))
      (values top
	      (map (lambda (p)
		     (computed-goto-exit (detach (car p))
					 protocol
					 arg-vars
					 j
					 cont-vars))
		   entries)))))

(define (computed-goto-exit node protocol arg-vars cont-var cont-vars)
  (let* ((cont-vars (map copy-variable cont-vars))
	 (cont-args (map make-reference-node cont-vars))
	 (args (map make-reference-node arg-vars)))
    (let-nodes ((l1 () (unknown-call 1 l2 node '(protocol #f) . args))
		(l2 cont-vars (jump 0 (* cont-var) . cont-args)))
      l1)))

