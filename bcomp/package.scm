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
  (name	     structure-name set-structure-name!))

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

(define (make-structure package int-thunk . name-option)
  (if (not (package? package))
      (call-error "invalid package" make-structure package int-thunk))
  (let ((struct (really-make-structure package
				       (if (procedure? int-thunk)
					   int-thunk
					   (lambda () int-thunk))
				       #f
				       (make-population)
				       #f)))
    (if (not (null? name-option))
	(note-structure-name! struct (car name-option)))
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

(define (note-structure-name! struct name)
  (if (and name (not (structure-name struct)))
      (begin (set-structure-name! struct name)
	     (note-package-name! (structure-package struct) name))))

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

(define (make-package opens-thunk accesses-thunk unstable? tower file clauses
		      uid name)
  (let ((p (really-make-package
	    (if uid
		(begin (if (>= uid *package-uid*)
			   (set! *package-uid* (+ uid 1)))
		       uid)
		(new-package-uid))
	    opens-thunk
	    #f				;opens
	    accesses-thunk		;thunk returning alist
	    (make-table name-hash)	;definitions
	    (fluid $get-location)	;procedure for making new locations
	    '()				;property list...
	    (make-table name-hash)	;bindings cached in templates
	    (make-population)		;structures
	    unstable?			;unstable (suitable for EVAL)?
	    file			;file containing DEFINE-STRUCTURE form
	    clauses			;misc. DEFINE-STRUCTURE clauses
	    #f)))			;loaded?
    (note-package-name! p name)
    (set-package->environment! p (really-package->environment p))
    (if unstable?			;+++
	(define-funny-names! p tower))
    p))

(define (really-package->environment p)
  (lambda (name)
    (package-lookup p name)))

; Unique id's

(define (new-package-uid)
  (let ((uid *package-uid*))		;unique identifier
    (set! *package-uid* (+ *package-uid* 1))
    uid))

(define *package-uid* 0)

; Package names

(define package-name-table (make-table))

(define (package-name package)
  (table-ref package-name-table (package-uid package)))

(define (note-package-name! package name)
  (if name
      (let ((uid (package-uid package)))
	(if (not (table-ref package-name-table uid))
	    (table-set! package-name-table uid name)))))

(define (package-opens p)
  (initialize-package-if-necessary! p)
  (package-opens-really p))

(define (initialize-package-if-necessary! p)
  (if (not (package-opens-really p))
      (initialize-package! p)))

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
			 ""		;file containing DEFINE-STRUCTURE form
			 '()		;clauses
			 #f		;uid
			 (if (null? name-option)
			     #f
			     (car name-option)))))
    (set-package-loaded?! p #t)
    p))

; --------------------
; The definitions table

; Each entry in the package-definitions table is a binding
; #(type place static).  "Place" will typically be a location,
; but it doesn't have to be.

(define (package-definition p name)
  (initialize-package-if-necessary! p)
  (let ((probe (table-ref (package-definitions p) name)))
    (if probe
	(maybe-fix-place probe)
	#f)))

; Disgusting.  Interface predates invention of "binding" records.

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
	       (binding-place (maybe-fix-place probe)))
	(let ((place (or place (get-new-location p name))))
	  (table-set! (package-definitions p)
		      name
		      (make-binding type place static))
	  place))))


; --------------------
; Lookup

; Look up a name in a package.  Returns a binding if bound, or a name if
; not.  In the unbound case, the name returned is either the original
; name or, if the name is generated, the name's underlying symbol.

(define (package-lookup p name)
  (really-package-lookup p name (package-integrate? p)))

(define (really-package-lookup p name integrate?)
  (let ((probe (package-definition p name)))
    (cond (probe
	   (if integrate?
	       probe
	       (forget-integration probe)))
	  ((generated? name)
	   (generic-lookup (generated-env name)
			   (generated-symbol name)))
	  (else
	   (let loop ((opens (package-opens-really p)))
	     (if (null? opens)
		 name			;Unbound
		 (or (structure-lookup (car opens) name integrate?)
		     (loop (cdr opens)))))))))

; Get a name's binding in a structure.  If the structure doesn't
; export the name, this returns #f.  If the structure exports the name
; but the name isn't bound, it returns the name.

(define (structure-lookup struct name integrate?)
  (let ((type (interface-ref (structure-interface struct) name)))
    (if type
	(impose-type type
		     (really-package-lookup (structure-package struct)
					    name
					    integrate?)
		     integrate?)
	#f)))

(define (generic-lookup env name)
  (cond ((package? env)
	 (package-lookup env name))
	((structure? env)
	 (or (structure-lookup env name
			       (package-integrate? (structure-package env)))
	     (call-error "not exported" generic-lookup env name)))
	((procedure? env)
	 (lookup env name))
	(else
	 (error "invalid environment" env name))))

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
  (table-walk (lambda (name binding)
		(proc name (maybe-fix-place binding)))
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
		(make-immutable!
		 (cons (name->symbol name) (package-uid p))))
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

; Special kludge for shadowing and package mutation.
; Ignore this on first reading.  See env/shadow.scm.

(define (maybe-fix-place b)
  (let ((place (binding-place b)))
    (if (and (location? place)
	     (vector? (location-id place)))
	(set-binding-place! b (follow-forwarding-pointers place))))
  b)

(define (follow-forwarding-pointers place)
  (let ((id (location-id place)))
    (if (vector? id)
	(follow-forwarding-pointers (vector-ref id 0))
	place)))

; (put 'package-define! 'scheme-indent-hook 2)
