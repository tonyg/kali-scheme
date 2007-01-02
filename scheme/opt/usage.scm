; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Getting usage counts and doing a topological sort (so that definitions
; will be seen before uses, where possible).
;
; We change the types of all unassigned top-level variables from
; (VARIABLE <type>) to <type>.
;
; Steps:
;  1. Make usage records for the variables bound by this package.
;  2. Analyze each form to update the usage records and to find the referenced
;     variables defined in this package.
;  3. Update the types of the variables based on their usages.
;  4. Do a topological sort of the forms using the referenced-variable sets
;     from step 2.

(define (find-usages forms package)
  (let ((usages (make-name-table)))
    (for-each (lambda (form)
		(if (define-node? form)
		    (let* ((lhs (cadr (node-form form)))
			   (usage (make-package-usage lhs)))
		      (table-set! usages (node-form lhs) usage)
		      (node-set! lhs 'usage usage))))
	      forms)
    (for-each (lambda (form)
		(node-set! form
			   'free-variables
			   (analyze form
				    '()
				    (lambda (node)
				      (table-ref usages (node-form node))))))
	      forms)
    (for-each (lambda (form)
		(if (define-node? form)
		    (maybe-update-known-type form package)))
	      forms)
    (sort-forms forms #t)))

(define (maybe-update-known-type node package)
  (let* ((lhs (cadr (node-form node)))
	 (usage (node-ref lhs 'usage)))
    (if (= 0 (usage-assignment-count usage))
	(let ((new-type (reconstruct-type (caddr (node-form node))
					  (package->environment package))))
	  (if (subtype? new-type any-values-type)
	      (package-refine-type! package
				    (node-form lhs)
				    (if (subtype? new-type value-type)
					new-type
					value-type))
	      (warn "ill-typed right-hand side"
		    (schemify node)
		    (type->sexp new-type #t)))))))

;----------------
; Another entry point.
; Here we want to return all package variables found, not just the ones from
; this package.  We also don't update the actual usage records for package
; variables, as they refer to the entire package, not just one form.

(define (find-node-usages node)
  (let* ((usages (make-name-table))
	 (referenced (analyze node
			      '()
			      (lambda (node)
				(let ((usage (node-ref node 'usage)))
				  (if (and usage
					   (not (package-usage? usage)))
				      #f
				      (let ((name (node-form node)))
					(or (table-ref usages name)
					    (let ((usage (make-package-usage node)))
					      (table-set! usages name usage)
					      usage)))))))))
    (map (lambda (usage)
	   (node-form (usage-name-node usage)))
	 referenced)))
    
;----------------
; The usual node walk.  FREE is a list of usage records for package variables
; that have been seen so far.  USAGES is a function that maps names to usages.

(define (analyze node free usages)
  ((operator-table-ref usage-analyzers (node-operator-id node))
     node
     free
     usages))

(define (analyze-nodes nodes free usages)
  (reduce (lambda (node free)
	    (analyze node free usages))
	  free
	  nodes))

(define usage-analyzers
  (make-operator-table (lambda (node free usages)
			 (analyze-nodes (node-form node) free usages))))

(define (define-usage-analyzer name type proc)
  (operator-define! usage-analyzers name type proc))

(define (nothing node free usages) free)

(define-usage-analyzer 'literal    #f nothing)
(define-usage-analyzer 'unspecific #f nothing)
(define-usage-analyzer 'unassigned #f nothing)
(define-usage-analyzer 'quote               syntax-type nothing)
(define-usage-analyzer 'primitive-procedure syntax-type nothing)

(define-usage-analyzer 'name #f
  (lambda (node free usages)
    (note-reference! node usages)
    (add-if-free node free usages)))

; If NODE has a usage record, then add it to FREE if it (the usage record) isn't
; already there.

(define (add-if-free node free usages)
  (let ((usage (usages node)))
    (if (and usage
	     (not (memq usage free)))
	(cons usage free)
	free)))

(define-usage-analyzer 'call #f
  (lambda (node free usages)
    (let* ((exp (node-form node))
	   (proc (car exp)))
      (if (name-node? proc)
	  (note-operator! proc usages))
      (analyze-nodes exp free usages))))

(define-usage-analyzer 'lambda syntax-type
  (lambda (node free usages)
    (let* ((exp (node-form node))
	   (formals (cadr exp)))
      (for-each (lambda (node)
		  (node-set! node 'usage (make-usage)))
		(normalize-formals formals))
      (analyze (caddr exp) free usages))))

(define-usage-analyzer 'letrec syntax-type
  (lambda (node free usages)
    (let ((exp (node-form node)))
      (analyze-letrec (cadr exp) (caddr exp) free usages))))

(define-usage-analyzer 'pure-letrec syntax-type
  (lambda (node free usages)
    (let ((exp (node-form node)))
      (analyze-letrec (cadr exp) (cadddr exp) free usages))))

(define (analyze-letrec specs body free usages)
  (for-each (lambda (spec)
	      (node-set! (car spec) 'usage (make-usage)))
	    specs)
  (analyze body
	   (analyze-nodes (map cadr specs)
			  free
			  usages)
	   usages))

(define-usage-analyzer 'begin syntax-type
  (lambda (node free usages)
    (analyze-nodes (cdr (node-form node)) free usages)))

(define-usage-analyzer 'set! syntax-type
  (lambda (node free usages)
    (let ((exp (node-form node)))
      (let ((lhs (cadr exp))
	    (rhs (caddr exp)))
	(note-assignment! lhs usages)
	(analyze rhs (add-if-free lhs free usages) usages)))))

(define-usage-analyzer 'define syntax-type
  (lambda (node free usages)
    (analyze (caddr (node-form node))
	     free
	     usages)))

(define-usage-analyzer 'if syntax-type
  (lambda (node free usages)
    (analyze-nodes (cdr (node-form node)) free usages)))

(define-usage-analyzer 'lap syntax-type
  (lambda (node free usages)
    (analyze-nodes (caddr (node-form node))
		   free
		   usages)))

(define-usage-analyzer 'loophole syntax-type
  (lambda (node free usages)
    (analyze (caddr (node-form node))
	     free
	     usages)))

;--------------------
; Usage records record the number of times that a variable is referenced, set!,
; and called.

(define-record-type usage :usage
  (really-make-usage name-node reference operator assignment)
  usage?
  (name-node usage-name-node)  ; only for package variables
  (reference usage-reference-count set-reference!)
  (operator usage-operator-count set-operator!)
  (assignment usage-assignment-count set-assignment!))

(define (make-usage)
  (really-make-usage #f 0 0 0))

(define (make-package-usage name-node)
  (really-make-usage name-node 0 0 0))

(define (package-usage? usage)
  (usage-name-node usage))

(define (usage-incrementator ref set)
  (lambda (node usages)
    (let ((v (or (node-ref node 'usage)
		 (usages node))))
      (if v
	  (set v (+ (ref v) 1))))))

(define note-reference! (usage-incrementator usage-reference-count set-reference!))
(define note-operator! (usage-incrementator usage-operator-count set-operator!))
(define note-assignment! (usage-incrementator usage-assignment-count set-assignment!))

;----------------

(define lambda-node? (node-predicate 'lambda))
(define quote-node? (node-predicate 'quote))
(define literal-node? (node-predicate 'literal))
(define call-node? (node-predicate 'call))
(define name-node? (node-predicate 'name 'leaf))
(define define-node? (node-predicate 'define syntax-type))
