; Copyright (c) 1994 Richard Kelsey.  See file COPYING.

; This file has the Pre-Scheme compiler's code for dealing with the
; Scheme 48's module system.

; FILES is a list of files that contain structure definitions, including
; a definition for NAME.  The files are loaded into a config package
; containing:
;   - the procedures and macros for defining structures and interfaces
;   - a Pre-Scheme structure (called PRESCHEME)
;   - a ps-memory structure
;   - a ps-receive structure
;   - the STRUCTURE-REFS structure
; We then return:
;   1. a function that will scan the definitions of the package to which
;      the named structure belongs and any packages that it uses (and
;      the packages they use, and so on)
;   2. a list of the names exported by the named structure
;   3. a procedure that for looking up names defined in packages in the
;      config package (this is used to map user directives to their targets)

(define (package-specs->scanner+exports struct-names files)
  (let ((config (make-very-simple-package 'config (list defpackage)))
	(old-config ((structure-ref package-commands-internal config-package))))
    (environment-define! config 'prescheme prescheme)
    (environment-define! config 'ps-memory ps-memory)
    (environment-define! config 'ps-receive ps-receive)
    (environment-define! config 'prescheme-defrecord prescheme-defrecord)
    (environment-define! config 'structure-refs structure-refs)
    (environment-define! config ':syntax (structure-ref meta-types syntax-type))
    (set-reflective-tower-maker! config (get-reflective-tower-maker old-config))
    (let-fluids (structure-ref packages-internal $get-location) get-variable
		$note-file-package (lambda (filename package) (values))
      (lambda () 
	(for-each (lambda (file)
		    (load file config))
		  files)))
    (values (make-scanner-maker (environment-ref config (car struct-names)))
	    (let ((names '()))
	      (for-each (lambda (struct-name)
			  (let ((my-names '()))
			    (for-each-declaration
			     (lambda (name type)
			       (set! my-names (cons name my-names)))
			     (structure-interface (environment-ref config struct-name)))
			    (set! names (cons (cons struct-name my-names) names))))
			struct-names)
	      names)
	    (make-lookup config))))

; This creates new variables as needed for packages.

(define (get-variable package name)
  ;(format #t "Making variable ~S for ~S~%" name package)
  ((structure-ref variable make-global-variable)
   name
   (structure-ref ps-types type/unknown)))

; Return something that will find the binding of ID in the package belonging
; to the structure PACKAGE-ID in the CONFIG package.

(define (make-lookup config)
  (lambda (package-id id)
    (let ((binding (package-lookup config package-id)))
      (if (and (binding? binding)
	       (location? (binding-place binding))
	       (structure? (contents (binding-place binding))))
	  (let* ((package (structure-package
			   (contents (binding-place binding))))
		 (binding (package-lookup package id)))
	    (if (binding? binding)
		(binding-place binding)
		#f))
	  #f))))
	       
; Return a procedure that will apply its argument to all packages reachable
; from the structure STRUCT.

(define (make-scanner-maker struct)
  (lambda (package-action)
    (let ((loaded '()))
      (let-fluid $note-file-package (lambda (filename package) (values))
	(lambda ()
	  (scan-structures
	   (list struct)
	   (lambda (package)
	     (not (memq package loaded)))
	   (lambda (stuff package)
	     ;; stuff = pair (file . (node1 node2 ...))
	     (for-each (lambda (filename+scanned-forms)
			 (package-action package
					 (cdr filename+scanned-forms)))
		       stuff)
	     (set! loaded (cons package loaded)))))))))

;----------------------------------------------------------------
; Handy packages and package making stuff.

(define defpackage     (structure-ref built-in-structures defpackage))
(define structure-refs (structure-ref built-in-structures structure-refs))
(define scheme         (structure-ref built-in-structures scheme))

(define (make-env-for-syntax-promise . structures)
  (make-reflective-tower eval structures 'prescheme-linking))

(define (make-very-simple-package name opens)
  (make-simple-package opens
		       eval
		       (make-env-for-syntax-promise scheme)
		       name))

(define (get-reflective-tower-maker p)
  (environment-ref p (string->symbol ".make-reflective-tower.")))

;----------------------------------------------------------------
; BASE-PACKAGE contains all of the primitives, syntax, etc. for Pre-Scheme

(define (prescheme-unbound package name)
  (bug "~S has no binding in package ~S" name package))

(define base-package
;  (let-fluid (structure-ref packages-internal $get-location) prescheme-unbound
;    (lambda () ))
  (make-very-simple-package 'base-package '()))

; Add the operators.

(let ((syntax-type (structure-ref meta-types syntax-type)))
  (for-each (lambda (id)
	      (package-define! base-package
			       id
			       (get-operator id syntax-type)
			       #t))
	    '(if begin lambda letrec quote set!
		 define define-syntax let-syntax
		 goto real-external)))  ; special for Prescheme

; Add the usual macros.

(for-each (lambda (name)
	    (package-define! base-package
			     name
			     (make-transform
			      (usual-transform name)
			      base-package
			      (structure-ref meta-types syntax-type)
			      `(usual-transform ',name)
			      name)
			     #t))
	  '(and cond do let let* or quasiquote syntax-rules)) ; delay

; Plus whatever primitives are wanted.

(define (define-prescheme! name value)
  (package-define! base-package
		   name
		   usual-variable-type
		   value))

; Copy over the enumeration macros and the ERRORS enumeration.

(define (import-syntax! package-id name)
  (let ((config ((structure-ref package-commands-internal config-package))))
    (let ((binding (structure-lookup (environment-ref config package-id)
				     name
				     #t)))
      (package-define! base-package
		       name
		       (binding-static binding)
		       (binding-place binding)))))

(import-syntax! 'enumerated 'define-enumeration)
(import-syntax! 'enumerated 'enum)
(import-syntax! 'enumerated 'name->enumerand)
(import-syntax! 'enumerated 'enumerand->name)
(import-syntax! 'prescheme  'errors)
(import-syntax! 'prescheme  'define-external-enumeration)

; define still more syntax

(load "prescheme/ps-syntax.scm" base-package)

;----------------------------------------------------------------
; Make the Pre-Scheme structure and related structures

(define (get-interface name)
  (environment-ref ((structure-ref package-commands-internal config-package))
		   name))

(define prescheme
  (make-structure base-package
		  (lambda () (get-interface 'prescheme-interface))
		  'prescheme))

(define ps-memory
  (make-structure base-package
		  (lambda () (get-interface 'ps-memory-interface)) 
		  'ps-memory))

(define ps-receive
  (make-structure base-package
		  (lambda () (get-interface 'ps-receive-interface)) 
		  'ps-receive))

; and a handy environment

(define prescheme-compiler-env
  (package->environment base-package))

;----------------------------------------------------------------
; The following stuff is used to define the define-record-type macro.

(define defrecord-for-syntax-package
  (make-very-simple-package 'defrecord-for-syntax-package '()))

(define defrecord-for-syntax-structure
  (make-structure defrecord-for-syntax-package
		  (lambda () (export expand-define-record-type))
		  'defrecord-for-syntax-structure))

(define (define-for-syntax-value id value)
  (let ((loc (make-new-location defrecord-for-syntax-package id)))
    (set-contents! loc value)
    (package-define! defrecord-for-syntax-package id usual-variable-type loc)))

(define-for-syntax-value 'expand-define-record-type expand-define-record-type)

; A package in which to define DEFINE-RECORD-TYPE.  This is separate
; from BASE-PACKAGE because this package needs to open SCHEME.

(define prescheme-defrecord-package
  (make-simple-package (list prescheme)
		       eval
		       (make-env-for-syntax-promise
			scheme
			defrecord-for-syntax-structure)
		       'prescheme-defrecord-package))

(define prescheme-defrecord
  (make-structure prescheme-defrecord-package
		  (lambda () (export (define-record-type :syntax)))
		  'prescheme-defrecord))

(eval '(define-syntax define-record-type expand-define-record-type)
      prescheme-defrecord-package)

