; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; PACkage-manipulation comMANds

(add-sentinel! package-system-sentinel)


(define (set-environment-for-commands! p)
  (set-interaction-environment! p)
  (set-command-level-env! (command-level) p))

(define user-environment
  (user-context-accessor 'user-environment interaction-environment))
(define set-user-environment!
  (user-context-modifier 'user-environment))


(define-command 'in "<struct> [<form>]"
  "go to package, or run single form in package"
  '(name &opt form)
  (lambda (name form)
    (in-package (if (symbol? form)
		    (really-get-package name)
		    (get-package name))
      form)))

(define-command 'new-package "" "make and enter a new package"
  '()
  (lambda ()
    (let ((p (make-simple-package (list (get-structure 'scheme))
				  eval
				  (environment-for-syntax-promise)
				  '*unnamed*)))
      (set-package-integrate?! p
			       (package-integrate? (environment-for-commands)))
      (set-environment-for-commands! p))))


(define (environment-for-syntax-promise)
  ;; should be (make-package-for-syntax eval ???)
  (let ((user (user-environment)))
    (delay (package-for-syntax user))))

; load-package

(define-command 'load-package "<struct>" "load package's files"
  '(name)
  (lambda (name)
    (ensure-loaded (get-structure name))))

(define-command 'reload-package "<struct>" "load package's files again"
  '(name)
  (lambda (name)
    (let ((s (get-structure name)))
      (set-package-loaded?! (structure-package s) #f)
      (ensure-loaded s))))

(define-command 'load-into "<struct> <filename> ..."
  "load source file(s) into given package"
  '(name &rest filename)
  (lambda (name . filenames)
    (let ((p (get-package name)))
      (noting-undefined-variables p
        (lambda ()
	  (for-each (lambda (filename) (load filename p))
		    filenames))))))

(define-command 'load-config "<filename> ..."
  "load configuration file(s)"
  '(&rest filename)
  (lambda filenames
    (let ((p (config-package)))
      (noting-undefined-variables p
        (lambda ()
	  (for-each (lambda (filename)
		      (load filename p))
		    filenames))))))


(define-command 'structure "<name> <sig>" "create new structure"
  '(name expression)
  (lambda (name sig-expression)
    (let* ((c (config-package))
	   (p (environment-for-commands)))
      (environment-define! c name
			   (make-structure p
					   (lambda ()
					     (evaluate sig-expression c))
					   name))
      (set-package-name! p name))))


(define-command 'open "<struct> ..." "open a structure"
  '(&rest name)
  (lambda names
    (for-each (lambda (name)
		(let* ((c (config-package))
		       (thunk (lambda () (environment-ref c name)))
		       (probe (thunk)))
		  (if (structure? probe)
		      (if (ensure-loaded-query probe)
			  (package-open! (environment-for-commands) thunk)
			  (error "structure not loaded" name))
		      (error "not a structure" name))))
	      names)))

(define (ensure-loaded-query struct)
  (cond ((or (package-loaded? (structure-package struct))
	     (null? (package-clauses (structure-package struct))))
	 #t)
	((y-or-n? (string-append "Load structure "
				 (symbol->string
				  (structure-name struct)))
		  #f)
	 (ensure-loaded struct)
	 #t)
	(else #f)))


(define-command 'for-syntax "[<form>]"
  "go to current package's package for syntax"
  '(&opt form)
  (lambda (form)
    (in-package (package-for-syntax (environment-for-commands))
      form)))


; ,user  goes to the user initial environment.

(define-command 'user "[<form>]" "user package"
  '(&opt form)
  (lambda (form)
    (in-package (user-environment) form)))

(define-command 'user-package-is "<struct>" "designate package for ,user"
  '(name)
  (lambda (name)
    (set-user-environment! (get-package name))))

(define set-user-environment!
  (user-context-modifier 'user-environment))


; Configuration package  (should there be ,load-config as well?)

(define-command 'config "[<form>]" "go to configuration package"
  '(&opt form)
  (lambda (form)
    (in-package (config-package) form)))

(define-command 'config-package-is "<struct>"
  "designate configuration package"
  '(name)
  (lambda (name)
    (set-config-package! (get-package name))))


; Go to current package's package for syntax

;(define-comand 'syntax "" "package for syntax"
;  '(&opt form)
;  (lambda (form)
;    (in-package (package-for-syntax (environment-for-commands)) form)))


; Auxiliaries for package commands

(define (in-package p form)
  (if form
      (if (symbol? form)
	  (set-focus-object! (environment-ref p form))
	  (with-interaction-environment p
	    (lambda ()
	      (evaluate-and-select form p))))
      (set-environment-for-commands! p)))

(define config-package
  (user-context-accessor 'config-package user-environment))

(define set-config-package!
  (user-context-modifier 'config-package))


(define (get-package name)
  (let ((p (really-get-package name)))
    (if (package-unstable? p)
	p
	(error "read-only structure" p))))

(define (really-get-package name)
  (let ((s (get-structure name)))
    (ensure-loaded-query s)
    (structure-package s)))

(define (get-structure name)
  (let ((thing (environment-ref (config-package) name)))
    (cond ((structure? thing) thing)
	  (else (error "not a structure" name thing)))))


; Main entry point, with package setup.

(define (new-command-processor info built-in-structures . more)
  ;; Argument to ,build command
  (lambda (arg)
    (call-with-values (lambda ()
			(new-user-context built-in-structures more))
      (lambda (context env)
	(start-command-processor arg
				 context
				 env
				 (lambda ()
				   (greet-user info)))))))

(define (new-user-context built-in-structures more-structures)
  (let* ((for-syntax (make-package-for-syntax
		      eval
		      (list (*structure-ref built-in-structures 'scheme))))
	 (user (make-user-package built-in-structures for-syntax))
	 (config-package (make-config-package 'config
					      user
					      built-in-structures
					      more-structures)))
    (values (make-user-context
	     (lambda ()
	       (set-user-environment! user)
	       (set-config-package! config-package)))
	    user)))

; User package

(define (make-user-package built-in-structures for-syntax)
  (let* ((scheme-structure (*structure-ref built-in-structures 'scheme))
	 (user
	  (make-simple-package (list scheme-structure)
			       eval
			       (trivial-delay for-syntax)
			       'user)))
    (set-package-integrate?! user #f)
;    (environment-define! user
;                         'access-scheme48
;                         (make-access-scheme48 built-in-structures))
    user))

; Configuration package

(define (make-config-package name user built-in more-structures)
  (let* ((defpackage (*structure-ref built-in 'defpackage))
	 (types (*structure-ref built-in 'types))
	 (for-syntax (package-for-syntax user))
	 (config-package
	  (make-simple-package (cons defpackage
				     (cons types
					   (append more-structures
						   (list built-in))))
			       eval
			       (trivial-delay for-syntax)
			       name)))
    config-package))

(define (trivial-delay thing)  ;extracted for gc purposes
  (delay thing))
