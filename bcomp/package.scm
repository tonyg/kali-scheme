; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Structures 'n' packages.

; --------------------
; Structures

(define-record-type structure type/structure
  (really-make-structure package signature-thunk signature clients name)
  structure?
  (signature-thunk structure-signature-thunk)
  (signature structure-signature-really set-structure-signature!)
  (package   structure-package)    ; allow #f
  (clients   structure-clients)
  (name	     structure-name))

(define-record-discloser type/structure
  (lambda (s) (list 'structure
		    (package-uid (structure-package s))
		    (structure-name s))))

(define (structure-signature s)
  (or (structure-signature-really s)
      (begin (initialize-structure! s)
	     (structure-signature-really s))))

(define (initialize-structure! s)
  (let ((sig ((structure-signature-thunk s))))
    (if (signature? sig)
	(begin (set-structure-signature! s sig)
	       (note-reference-to-signature! sig s))
	(call-error "invalid signature" initialize-structure! s))))

(define (make-structure package sig-thunk name)
  (if (not (package? package))
      (call-error "invalid package" make-structure package sig-thunk name))
  (let ((struct (really-make-structure package
				       (if (procedure? sig-thunk)
					   sig-thunk
					   (lambda () sig-thunk))
				       #f
				       (make-population)
				       name)))
    (add-to-population! struct (package-clients package))
    struct))

(define (structure-unstable? struct)
  (package-unstable? (structure-package struct)))

(define (for-each-export proc struct)
  (let ((sig (structure-signature struct)))
    (for-each-declaration
        (lambda (name want-type)
	  (let ((binding (structure-lookup struct name #t)))
	    (proc name
		  (if (and (binding? binding)
			   (eq? want-type 'undeclared))
		      (let ((type (binding-type binding)))
			(if (variable-type? type)
			    (variable-value-type type)
			    type))
		      want-type)
		  binding)))
	sig)))


; --------------------
; Packages

(define-record-type package type/package
  (really-make-package uid
		       opens-thunk opens accesses-thunk
		       definitions
		       get-location
		       plist
		       seen cached
		       clients
		       eval
		       env-for-syntax
		       file-name clauses loaded?)
  package?
  (uid	           package-uid)
  (opens           package-opens-really set-package-opens!)
  (opens-thunk     package-opens-thunk set-package-opens-thunk!)
  (accesses-thunk  package-accesses-thunk)
  (definitions     package-definitions)
  (get-location    package-get-location set-package-get-location!)
  (eval		   package-evaluator)
  (env-for-syntax  package-for-syntax-promise)
  (integrate?      package-integrate? set-package-integrate?!)
  (file-name       package-file-name)
  (clauses         package-clauses)

  ;; extra
  (plist           package-plist set-package-plist!)
  (clients         package-clients)
  (seen	           package-seen)
  (cached	   package-cached)
  (loaded?         package-loaded? set-package-loaded?!))

(define-record-discloser type/package
  (lambda (p)
    (let ((name (package-name p)))
      (if name
	  (list 'package (package-uid p) name)
	  (list 'package (package-uid p))))))

(define make-package
  (lambda (opens-thunk accesses-thunk evaluator for-syntax-promise dir clauses
		       name)
    (let ((p (really-make-package
	      (new-package-uid)
	      opens-thunk
	      #f
	      accesses-thunk		;list of thunks
	      (make-table name-hash)	;definitions
	      (fluid $get-location)	;procedure for making new locations
	      '()
	      (make-table name-hash)	;names seen
	      (make-table name-hash)	;bindings cached in templates
	      (make-population)
	      evaluator for-syntax-promise
	      dir clauses #f)))
      (if name (set-package-name! p name))
      p)))

(define (package-for-syntax p)
  (force (package-for-syntax-promise p)))

; Unique id's

(define (new-package-uid)
  (let ((uid *package-uid*))		;unique identifier
    (set! *package-uid* (+ *package-uid* 1))
    uid))

(define *package-uid* 200)

; Package names

(define package-name-table (make-table))

(define (package-name package)
  (table-ref package-name-table (package-uid package)))

(define (set-package-name! package name)
  (table-set! package-name-table (package-uid package) name))

; A simple package has no ACCESSes or other far-out clauses.

(define (make-simple-package opens evaluator efs-promise . name-option)
  (if (not (list? opens))
      (error "invalid package opens list" opens))
  (let ((p (make-package (lambda () opens)
			 (lambda () '()) ;accesses-thunk
			 evaluator
			 efs-promise
			 ""		;directory
			 '()		;clauses
			 (if (null? name-option)
			     #f
			     (car name-option)))))
    (set-package-loaded?! p #t)
    p))

(define (package-opens p)
  (or (package-opens-really p)
      (begin (initialize-package! p)
	     (package-opens-really p))))

(define (package-accesses p)		;=> alist
  ((package-accesses-thunk p)))

(define package-unstable? package-evaluator)

; --------------------
; The definitions table

; Each entry in the package-definitions table is either a binding
; #(type place static) or a place.  A non-pair abbreviates
; #(usual-variable-type place #f), which is expected to be a space
; saver for the common case.  "Place" will typically be a location,
; but it doesn't have to be.

(define (package-definition p name)
  (let ((probe (table-ref (package-definitions p) name)))
    (if probe
	(if (binding? probe)
	    probe
	    (make-binding usual-variable-type probe))
	#f)))

; disgusting

(define (package-define! p name type-or-static . place-option)
  (let ((place (if (null? place-option)
		   #f
		   (car place-option))))
    (cond ((transform? type-or-static)
	   (really-package-define! p name
				   (transform-type type-or-static)
				   place
				   type-or-static))
	  ((operator? type-or-static)
	   (really-package-define! p name
				   (operator-type type-or-static)
				   place
				   type-or-static))
	  (else
	   (really-package-define! p name
				   type-or-static
				   place
				   #f)))))
    

(define (really-package-define! p name type place static)
  (let ((probe (table-ref (package-definitions p) name)))
    (if (binding? probe)			;ugh, microhackery
	(begin (clobber-binding! probe type place static)
	       (binding-place probe))
	(let ((place (or place probe (get-new-location p name))))
	  (table-set! (package-definitions p)
		      name
		      (if (and (not static)
			       (equal? type usual-variable-type))
			  place
			  (make-binding type place static)))
	  place))))


; --------------------
; Lookup

; Look up a name occurring in operator position of a form.  Returns
; a binding if bound, name if not.

(define (package-lookup p name . integrate?-option)
  (really-package-lookup p name (if (null? integrate?-option)
				    (package-integrate? p)
				    (car integrate?-option))))

(define (really-package-lookup p name integrate?)
  (let ((probe (package-definition p name)))
    (cond (probe
	   (if integrate?
	       probe
	       (forget-integration probe)))
	  ((generated? name)
	   (lookup-generated-name p name))
	  (else
	   (let loop ((opens (package-opens p)))
	     (if (null? opens)
		 name			;Unbound
		 (let ((probe (structure-lookup (car opens) name integrate?)))
		   (if (binding? probe)
		       probe
		       (loop (cdr opens))))))))))

; Exported names

(define (structure-lookup struct name integrate?)
  (let ((type (signature-ref (structure-signature struct) name)))
    (if type
	(let ((probe (really-package-lookup (structure-package struct)
					    name
					    integrate?)))
	  (if (binding? probe)
	      (impose-type type probe integrate?)
	      probe))
	name)))

(define (lookup-generated-name ignore-env name)
  (generic-lookup (generated-env name) (generated-symbol name)))

(define (generic-lookup env name)
  (cond ((package? env)
	 (package-lookup env name))
	((structure? env)
	 (structure-lookup env name #t))
	((procedure? env)
	 (lookup env name))
	(else
	 (error "invalid environment" env name))))

; Environments for CLASSIFY are procedures.

(define (package->environment p)
  (bind-evaluator-for-syntax
     (lambda (form env)
       ;; ENV is the environment in which the define-syntax or let-syntax
       ;; form appears.  For now, it is the same as the environment that
       ;; package->environment returns, but in the future it may differ,
       ;; and contain good stuff that needs to be seen by the code in the
       ;; macro transformer.
       (let ((f (package-for-syntax p)))
	 ((package-evaluator f) form f)))
     (lambda (name)
       (package-lookup p name))))


(define (package-lookup-type p name)
  (let ((probe (package-lookup p name)))
    (if (binding? probe)
	(binding-type probe)
	#f)))

; --------------------
; Package initialization

(define (initialize-package! p)
  (let ((opens ((package-opens-thunk p))))
    (set-package-opens! p opens)
    (for-each (lambda (struct)
		(if (structure-unstable? struct)
		    (add-to-population! p (structure-clients struct))))
	      opens))
  (for-each (lambda (name+struct)
	      ;; Cf. CLASSIFY method for STRUCTURE-REF
	      (really-package-define! p
				      (car name+struct)
				      'structure
				      #f
				      (cdr name+struct)))
	    (package-accesses p)))


; Cf. link/reify.scm

(define (initialize-reified-package! p names locs get-location)
  (let ((end (vector-length names)))
    (do ((i 0 (+ i 1)))
	((= i end))
      (let* ((name (vector-ref names i))
	     (probe (package-lookup p name)))
	(if (not (binding? probe))
	    (package-define! p
			     name
			     usual-variable-type
			     (get-location (vector-ref locs i))))))))


; For implementation of INTEGRATE-ALL-PRIMITIVES! in scanner, etc.

(define (for-each-definition proc p)
  (table-walk (lambda (name stuff)
		(if (binding? stuff)
		    (proc name stuff)
		    (proc name (make-binding usual-variable-type stuff))))
	      (package-definitions p)))

; --------------------
; Locations

(define (get-new-location p name)
  ((package-get-location p) p name))

; Default new-location method for new packages

(define (make-new-location p name)
  (let ((uid *location-uid*))
    (set! *location-uid* (+ *location-uid* 1))
    (table-set! location-info-table uid
		(cons (name->symbol name) (package-uid p)))
    (make-undefined-location uid)))

(define $get-location (make-fluid make-new-location))

(define *location-uid* 2000)

(define location-info-table (make-table))


(define (flush-location-names)
  (set! location-info-table (make-table))
  ;; (set! package-name-table (make-table)) ;hmm, not much of a space saver
  )

; --------------------
; Extra

(define (package-get p ind)
  (cond ((assq ind (package-plist p)) => cdr)
	(else #f)))

(define (package-put! p ind val)
  (cond ((assq ind (package-plist p)) => (lambda (z) (set-cdr! z val)))
	(else (set-package-plist! p (cons (cons ind val)
					  (package-plist p))))))

; compiler calls this

(define (package-note-caching p name place)
  (if (package-unstable? p)		;?????
      (if (not (table-ref (package-definitions p) name))
	  (let loop ((opens (package-opens p)))
	    (if (not (null? opens))
		(if (signature-ref (structure-signature (car opens))
				   name)
		    (begin (table-set! (package-cached p) name place)
			   (package-note-caching
			       (structure-package (car opens))
			       name place))
		    (loop (cdr opens)))))))
  place)

; (put 'package-define! 'scheme-indent-hook 2)
