; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Structures 'n' packages.

; --------------------
; Structures

(define-record-type structure :structure
  (really-make-structure package interface-thunk interface clients name)
  structure?
  (interface-thunk structure-interface-thunk)
  (interface structure-interface-really set-structure-interface!)
  (package   structure-package)    ; allow #f
  (clients   structure-clients)
  (name	     structure-name))

(define-record-discloser :structure
  (lambda (s) (list 'structure
		    (package-uid (structure-package s))
		    (structure-name s))))

(define (structure-interface s)
  (or (structure-interface-really s)
      (begin (initialize-structure! s)
	     (structure-interface-really s))))

(define (initialize-structure! s)
  (let ((int ((structure-interface-thunk s))))
    (if (interface? int)
	(begin (set-structure-interface! s int)
	       (note-reference-to-interface! int s))
	(call-error "invalid interface" initialize-structure! s))))

(define (make-structure package int-thunk name)
  (if (not (package? package))
      (call-error "invalid package" make-structure package int-thunk name))
  (let ((struct (really-make-structure package
				       (if (procedure? int-thunk)
					   int-thunk
					   (lambda () int-thunk))
				       #f
				       (make-population)
				       name)))
    (add-to-population! struct (package-clients package))
    struct))

(define (structure-unstable? struct)
  (package-unstable? (structure-package struct)))

(define (for-each-export proc struct)
  (let ((int (structure-interface struct)))
    (for-each-declaration
        (lambda (name want-type)
	  (let ((binding (structure-lookup struct name #t)))
	    (proc name
		  (if (and (binding? binding)
			   (eq? want-type undeclared-type))
		      (let ((type (binding-type binding)))
			(if (variable-type? type)
			    (variable-value-type type)
			    type))
		      want-type)
		  binding)))
	int)))


; --------------------
; Packages

(define-record-type package :package
  (really-make-package uid
		       opens-thunk opens accesses-thunk
		       definitions
		       get-location
		       plist
		       cached
		       clients
		       unstable?
		       file-name clauses loaded?)
  package?
  (uid	           package-uid)
  (opens           package-opens-really set-package-opens!)
  (definitions     package-definitions)
  (unstable?       package-unstable?)
  (integrate?      package-integrate? set-package-integrate?!)

  ;; For EVAL and LOAD (which can only be done in unstable packages)
  (get-location    package-get-location set-package-get-location!)
  (file-name       package-file-name)
  (clauses         package-clauses)
  (loaded?         package-loaded? set-package-loaded?!)
  (env             package->environment set-package->environment!)

  ;; For package mutation
  (opens-thunk     package-opens-thunk set-package-opens-thunk!)
  (accesses-thunk  package-accesses-thunk)
  (plist           package-plist set-package-plist!)
  (clients         package-clients)
  (cached	   package-cached))

(define-record-discloser :package
  (lambda (p)
    (let ((name (package-name p)))
      (if name
	  (list 'package (package-uid p) name)
	  (list 'package (package-uid p))))))

(define make-package
  (lambda (opens-thunk accesses-thunk unstable? tower dir clauses
		       uid name)
    (let ((p (really-make-package
	      (if uid
		  (begin (if (>= uid *package-uid*)
			     (set! *package-uid* (+ uid 1)))
			 uid)
		  (new-package-uid))
	      opens-thunk
	      #f
	      accesses-thunk		;list of thunks
	      (make-table name-hash)	;definitions
	      (fluid $get-location)	;procedure for making new locations
	      '()			;property list...
	      (make-table name-hash)	;bindings cached in templates
	      (make-population)		;structures
	      unstable?			;unstable
	      dir
	      clauses
	      #f)))			;loaded?
      (if name (set-package-name! p name))
      (set-package->environment! p (really-package->environment p))
      (define-funny-names! p tower)
      p)))

(define (really-package->environment p)
  (lambda (name)
    (package-lookup p name)))

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

(define (package-opens p)
  (or (package-opens-really p)
      (begin (initialize-package! p)
	     (package-opens-really p))))

(define (package-accesses p)		;=> alist
  ((package-accesses-thunk p)))

; --------------------
; A simple package has no ACCESSes or other far-out clauses.

(define (make-simple-package opens unstable? tower . name-option)
  (if (not (list? opens))
      (error "invalid package opens list" opens))
  (let ((p (make-package (lambda () opens)
			 (lambda () '()) ;accesses-thunk
			 unstable?
			 tower
			 ""		;directory
			 '()		;clauses
			 #f		;uid
			 (if (null? name-option)
			     #f
			     (car name-option)))))
    (set-package-loaded?! p #t)
    p))

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
	;; Special kludge for shadowing and package mutation.
	;; Ignore this on first reading.  See env/shadow.scm.
	(let ((place (binding-place probe)))
	  (if (and (location? place)
		   (vector? (location-id place)))
	      ;; should iterate, really
	      (set-binding-place! probe (vector-ref (location-id place) 0)))))
    probe))

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
    (if probe
	(begin (clobber-binding! probe type place static)
	       (binding-place probe))
	(let ((place (or place (get-new-location p name))))
	  (table-set! (package-definitions p)
		      name
		      (make-binding type place static))
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
  (let ((type (interface-ref (structure-interface struct) name)))
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
				      structure-type
				      #f
				      (cdr name+struct)))
	    (package-accesses p)))


(define (define-funny-names! p tower)
  (package-define-funny! p funny-name/the-package p)
  (if tower
      (package-define-funny! p funny-name/reflective-tower
			     tower)))

(define (package-define-funny! p name static)
  (table-set! (package-definitions p)
	      name
	      (make-binding syntax-type (cons 'dummy-place name) static)))


; The following funny name is bound in every package to the package
; itself.  This is a special hack used by the byte-code compiler
; (procedures LOCATION-FOR-UNDEFINED and NOTE-CACHING) so that it can
; extract the underlying package from any environment.

(define funny-name/the-package (string->symbol ".the-package."))

(define (extract-package-from-environment env)
  (get-funny env funny-name/the-package))

; (define (package->environment? env)
;   (eq? env (package->environment
;	        (extract-package-from-environment env))))


; --------------------
; For implementation of INTEGRATE-ALL-PRIMITIVES! in scanner, etc.

(define (for-each-definition proc p)
  (table-walk proc (package-definitions p)))

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

(define *location-uid* 5000)  ; 1510 in initial system as of 1/22/94

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
		(if (interface-ref (structure-interface (car opens))
				   name)
		    (begin (table-set! (package-cached p) name place)
			   (package-note-caching
			       (structure-package (car opens))
			       name place))
		    (loop (cdr opens)))))))
  place)

(define (with-fresh-packages-state uid thunk)
  (let ((swap (lambda ()
		(let ((temp *package-uid*))
		  (set! *package-uid* uid)
		  (set! uid temp)))))
    (dynamic-wind swap thunk swap)))

; (put 'package-define! 'scheme-indent-hook 2)
