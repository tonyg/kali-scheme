; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Packages.

; Much too complicated.  Much too complicated.

(define-record-type package type/package
  (really-make-package uid
		       opens-thunk opens accesses-thunk
		       definitions locations seen
		       cached
		       structures
		       eval
		       env-for-syntax
		       file-name clauses loaded?)
  package?
  (opens           package-opens-really set-package-opens!)
  (opens-thunk     package-opens-thunk set-package-opens-thunk!)
  (accesses-thunk  package-accesses-thunk)
  (definitions     package-definitions)
  (eval		   package-evaluator)
  (env-for-syntax  package-for-syntax-promise)
  (integrate?      package-integrate? set-package-integrate?!)

  (locations       package-locations)
  (seen	           package-seen)
  (cached	   package-cached)
  (structures      package-structures)
  (file-name       package-file-name)
  (clauses         package-clauses)
  (loaded?         package-loaded? set-package-loaded?!)
  (uid	           package-uid))

(define-record-discloser type/package
  (lambda (p)
    (let ((name (package-name p)))
      (if name
	  (list 'package (package-uid p) name)
	  (list 'package (package-uid p))))))

(define make-package
  (lambda (opens-thunk accesses-thunk evaluator for-syntax-promise dir clauses)
    (really-make-package
       (new-package-uid)
       opens-thunk
       #f
       accesses-thunk			;list of thunks
       (make-table name-hash)		;definitions
       (make-table name-hash)		;locations
       (make-table name-hash)		;names seen
       (make-table name-hash)		;locations cached
       (make-population)
       evaluator for-syntax-promise
       dir clauses #f)))

(define (package-for-syntax p)
  (force (package-for-syntax-promise p)))

; Unique id's

(define (new-package-uid)
  (let ((uid *package-uid*))		;unique identifier
    (set! *package-uid* (+ *package-uid* 1))
    uid))

(define *package-uid* 200)

(define package-name-table (make-table))
(define (package-name package)
  (table-ref package-name-table (package-uid package)))
(define (set-package-name! package name)
  (table-set! package-name-table (package-uid package) name))


(define (make-simple-package opens evaluator efs-promise . name-option)
  (if (not (list? opens))
      (error "invalid package opens list" opens))
  (let ((p (make-package (lambda () opens)
			 (lambda () '()) ;accesses-thunk
			 evaluator
			 efs-promise
			 "" '()))) ;dir, clauses
    (set-package-loaded?! p #t)
    (if (not (null? name-option))
	(set-package-name! p (car name-option)))
    p))

(define package-unstable? package-evaluator)

(define (package-opens p)
  (or (package-opens-really p)
      (begin (initialize-package! p)
	     (package-opens-really p))))


(define (package-accesses p)		;=> alist
  ((package-accesses-thunk p)))

; --------------------
; Lookup

; Each entry in the package-definitions table is one of:
;   operator       - special operator or compiler primitive
;   transform	   - defined as a macro or inlinable procedure
;   type (= anything else) - ordinary variable definition with no integration
;   (assigned ...) - variable reference; there are SET!'s


; Look up a name occurring in operator position of a form.
; Returns #f if no appropriate operator or rewrite.

(define (package-lookup p name)
  (call-with-values (lambda ()
		      (package-lookup-1 p name (package-integrate? p)
					#f))
    proj2))

(define proj2 (lambda (q op) op))
      
; Want to record having seen something if either
;  (a) we wanted an operator or transform, and found either, or
;  (b) we didn't want one, and didn't find one, but found something.

(define (package-lookup-1 p name integrate? need-value?)
  (let ((probe (table-ref (package-definitions p) name)))
    (cond (probe
	   (let ((syn? (syntax? probe)))
	     (values p (if (and need-value? syn?)
			   'syntax
			   (assume-denotation p name
			     (if (or syn?
				     integrate?
				     (not (or (operator? probe)
					      (transform? probe))))
				 probe
				 'procedure)))))) ;don't integrate
	  ((generated? name)
	   (package-lookup-1 (generated-env name)
			     (generated-symbol name)
			     integrate? need-value?))
	  (else
	   (exporting-package p name
	     (lambda (q type)
	       (let ((syn? (eq? type 'syntax)))
		 (if (and need-value? syn?)
		     (values q 'syntax)
		     (if (or syn? integrate?)
			 (call-with-values
			     (lambda ()
			       (package-lookup-1 q name integrate?
						 need-value?))
			   (lambda (q info)
			     (values q
				     (assume-denotation p name info))))
			 (values q 'value)))))
	     (lambda () (values p #f)))))))

; Check to see whether we're doing anything funny with a variable.

(define (package-check-variable p name)
  (call-with-values (lambda ()
		      (package-lookup-1 p name (package-integrate? p) #t))
    (lambda (q probe)
      (cond ((not probe)
	     (note-undefined! q name))
	    ((eq? probe 'syntax)
	     (warn "found syntax where variable was required"
		   name
		   q)))
      (cons p name))))

(define (package-check-assigned p name)
  (call-with-values (lambda ()
		      (package-lookup-1 p name (package-integrate? p) #t))
    (lambda (q probe)
      (cond ((not probe)
	     (note-undefined! q name))
	    ((eq? probe 'syntax)
	     (warn "attempt to set! a keyword" name p))
	    ((or (operator? probe)
		 (transform? probe))
	     (warn "assigning an integrated variable" name p))
	    ((not (eq? q p))
	     (warn "assigning an imported variable" name p)))
      (cons p name))))



; Define a name.

(define (package-define! p name new)
  (maybe-note-redefinition p name (table-ref (package-seen p) name) new)
  (table-set! (package-definitions p) name new))

(define (maybe-note-redefinition p name seen new)
  (if (and seen (not (compatible? new seen)))
      (let ((old-description (binding-description-string seen))
	    (new-description (binding-description-string new))
	    (doing (if (table-ref (package-definitions p) name)
		       "redefining"
		       "shadowing")))
	(warn (if (equal? old-description new-description)
		  doing
		  (string-append doing
				 " as "
				 new-description
				 " (prior references assumed "
				 old-description
				 ")"))
	      name))))


(define (compatible? new seen)
  (or (same-denotation? new seen)
      (and (not (syntax? new))
	   (not (syntax? seen))
	   ;OK to redefine to a more specific type
	   ;(eq? seen 'value)
	   )))


; Return package for a structure that exports a binding for name to p.

(define (exporting-package p name succeed fail)
  (let loop ((opens (package-opens p)))
    (if (null? opens)
	(fail)
	(let ((type (structure-exports? (car opens) name)))
	  (if type
	      (succeed (structure-package (car opens)) type)
	      (loop (cdr opens)))))))



(define (not-variable name d)
  (warn (string-append "found "
		       (binding-description-string d)
		       " as target of SET!")
	name))

(define (assume-denotation p name info)
  ;; (if (package-unstable? p) ...)	;***** save space?  and time?
  (table-set! (package-seen p) name info)
  info)


; This is used for name matching in transforms.
; Returns (name . package) if bound, name otherwise.

(define (probe-package p name)
  (let ((probe (table-ref (package-definitions p) name)))
    (cond (probe
	   (if (syntax? probe)
	       probe
	       (cons p name)))
	  ((generated? name)
	   (probe-package (generated-env name)
			  (generated-symbol name)))
	  (else
	   (exporting-package p name
			      (lambda (q type)
				(probe-package q name))
			      (lambda () name))))))


; This is called to process a top-level DEFINE form (variable definition).

(define (package-ensure-defined! p name)
  (let* ((probe (table-ref (package-locations p) name))
	 (loc (if probe  ;Either already defined, or undefined and referenced
		  (begin (set-location-defined?! probe #t)
			 probe)
		  (let ((new (make-new-location p name))
			(cached (table-ref (package-cached p) name)))
		    (set-location-defined?! new #t)
		    (cond (cached
			   (copy-shadowed-contents! cached new)
			   (cope-with-mutation p name new cached))
			  (else
			   (let loop ((p p))
			     (exporting-package p name
			       (lambda (p type)
				 (let ((probe (table-ref (package-locations p)
							 name)))
				   (if probe
				       (copy-shadowed-contents! probe new)
				       (loop p))))
			       (lambda () #f)))))
		    (table-set! (package-locations p) name new)
		    new))))
    (package-define! p name 'value)
    loc))

(define (for-each-definition proc p)
  (table-walk proc
	      (package-definitions p)))

; --------------------
; Package initialization

(define (initialize-package! p)
  (let ((opens ((package-opens-thunk p))))
    (set-package-opens! p opens)
    (for-each (lambda (struct)
		(if (structure-unstable? struct)
		    (add-to-population!
		     p (structure-clients struct))))
	      opens))
  (for-each (lambda (name+struct)
	      (package-define-access! p
				      (car name+struct)
				      (cdr name+struct)))
	    (package-accesses p)))

(define (reinitialize-package! p)
  (set-package-opens! p #f))

; Set up bindings for names of accessed structures.

(define (package-define-access! p name struct)
  (let ((sig (structure-signature struct)))
    (package-define! p name
      ;; proc env aux-names source id
      (make-transform (transform-for-structure-ref sig)
		      (structure-package struct)
		      'structure
		      struct  ;special aux-names hack
		      `(transform-for-structure-ref some-signature)
		      name))))

(define (transform-for-structure-ref sig)
  (lambda (exp rename compare)
    (cond ((eq? (cadr exp) 'structure-ref)
	   ;; (compare (cadr exp) (rename '...)) ?
	   (let ((name (caddr exp)))
	     (if (and (name? name)
		      (signature-ref sig name))
		 (rename name)
		 (syntax-error "invalid structure reference"
			       `(,(cadr exp) ,(car exp) ,name)
			       exp))))
	  (else
	   (syntax-error "invalid structure operation" exp)))))

; Cf. link/reify.scm

(define (initialize-reified-package! p names statics locs)
  (let ((end (vector-length names))
	(loc-table (package-locations p))
	(den-table (package-definitions p)))
    (do ((i 0 (+ i 1)))
	((= i end))
      (let ((name (vector-ref names i))
	    (static (vector-ref statics i))
	    (loc (vector-ref locs i)))
	(if loc
	    (table-set! loc-table name
			(if (location? loc)
			    loc
			    (location-from-id loc))))
	(if static
	    (case static
	      ((syntax operator) #f)
	      ((transform)  ;Inline procedure - will be initialized soon
	       (table-set! den-table name 'procedure))
	      (else
	       (table-set! den-table name static))))))))

(define (location-from-id loc)
  (error "location-from-id NYI"))


; --------------------
; Foo


; Little utility for warning messages.

(define (binding-description-string d)
  (cond ((transform? d)
	 (if (syntax? d) "macro" "integrated procedure"))
	((operator? d)
	 (if (syntax? d) "special operator" "primitive procedure"))
	(else "variable")))


; Debugging stuff

(define $note-undefined (make-fluid #f))

(define (note-undefined! p name)
  (let ((note (fluid $note-undefined)))
    (if note (note p name))))

(define (noting-undefined-variables p thunk)
  (let ((losers '()))
    (let-fluid $note-undefined
	(or (fluid $note-undefined)	;Recursive LOAD
	    (lambda (p name)
	      (let ((probe (assq p losers)))
		(if probe
		    (if (not (member name (cdr probe)))
			(set-cdr! probe (cons name (cdr probe))))
		    (set! losers (cons (list p name) losers))))))
      (lambda ()
	(let ((result (thunk)))
	  (for-each
	    (lambda (p+names)
	      (let* ((q (car p+names))
		     (names
		      (filter (lambda (name)
				(unbound? (probe-package q name)))
			      (cdr p+names))))
		(if (not (null? names))
		    (begin (display "Undefined")
			   (if (not (eq? q p))
			       (begin (display " ")
				      (write `(package
					       ,(package-name (car p+names))))))
			   (display ": ")
			   (write (map (lambda (name)
					 (if (generated? name)
					     (generated-symbol name)
					     name))
				       (reverse names)))
			   (newline)))))
	    losers)
	  result)))))

; (put 'package-define! 'scheme-indent-hook 2)
