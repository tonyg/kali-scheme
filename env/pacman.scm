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


(define-command-syntax 'in "<struct> [<command>]"
  "go to package, or run single command in package"
  '(name &opt command))

(define (in name . maybe-command)
  (if (and (not (null? maybe-command))
	   (command-just-evaluates-symbol? (car maybe-command)))
      (set-focus-object! (environment-ref (really-get-package name)
					  (cadr (car maybe-command))))
      (in-package (get-package name) maybe-command)))

(define (command-just-evaluates-symbol? command)
  (and (pair? command)
       (not (car command))
       (symbol? (cadr command))))


(define-command-syntax 'new-package "" "make and enter a new package"
  '())

(define (new-package)
  (let ((p (make-simple-package (list (get-structure 'scheme))
				eval
				(environment-for-syntax-promise)
				'*unnamed*)))
    (set-package-integrate?! p
			     (package-integrate? (environment-for-commands)))
    (set-environment-for-commands! p)))


(define (environment-for-syntax-promise)
  ;; should be (make-package-for-syntax eval ???)
  (let ((user (user-environment)))
    (delay (package-for-syntax user))))

; load-package

(define-command-syntax 'load-package "<struct>" "load package's files"
  '(name))

(define (load-package name)
  (ensure-loaded (get-structure name)))

(define-command-syntax 'reload-package "<struct>" "load package's files again"
  '(name))

(define (reload-package name)
  (let ((s (get-structure name)))
    (set-package-loaded?! (structure-package s) #f)
    (ensure-loaded s)))

(define-command-syntax 'load-config "<filename> ..."
  "load configuration file(s)"
  '(&rest filename))

(define (load-config filenames)
  (let ((p (config-package)))
    (noting-undefined-variables p
      (lambda ()
	(for-each (lambda (filename)
		    (load filename p))
		  filenames)))))


(define-command-syntax 'structure "<name> <sig>" "create new structure"
  '(name expression))

(define (structure name sig-expression)
  (let* ((c (config-package))
	 (p (environment-for-commands)))
    (environment-define! c name
			 (make-structure p
					 (lambda ()
					   (evaluate sig-expression c))
					 name))
    (set-package-name! p name)))


(define-command-syntax 'open "<struct> ..." "open a structure"
  '(&rest name))

(define (open . names)
  (for-each (lambda (name)
	      (let* ((c (config-package))
		     (thunk (lambda () (environment-ref c name)))
		     (probe (thunk)))
		(if (structure? probe)
		    (if (ensure-loaded-query probe)
			(package-open! (environment-for-commands) thunk)
			(error "structure not loaded" name))
		    (error "not a structure" name))))
	    names))

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


(define-command-syntax 'for-syntax "[<command>]"
  "go to current package's package for syntax"
  '(&opt command))

(define (for-syntax . maybe-command)
  (in-package (package-for-syntax (environment-for-commands)) maybe-command))


; ,user  goes to the user initial environment.

(define-command-syntax 'user "[<command>]" "user package"
  '(&opt command))

(define (user . maybe-command)
  (in-package (user-environment) maybe-command))

(define-command-syntax 'user-package-is "<struct>" "designate package for ,user"
  '(name))

(define (user-package-is name)
  (set-user-environment! (get-package name)))

(define set-user-environment!
  (user-context-modifier 'user-environment))


; Configuration package  (should there be ,load-config as well?)

(define-command-syntax 'config "[<command>]" "go to configuration package"
  '(&opt command))

(define (config . maybe-command)
  (in-package (config-package) maybe-command))

(define-command-syntax 'config-package-is "<struct>"
  "designate configuration package"
  '(name))

(define (config-package-is name)
  (set-config-package! (get-package name)))


; ,exec  goes to the exec initial environment.

(define-command-syntax 'exec "[<command>]" "exec package"
  '(&opt command))

(define (exec . maybe-command)
  (in-package (user-command-environment) maybe-command))

; Auxiliaries for package commands

(define (in-package p maybe-command)
  (if (null? maybe-command)
      (set-environment-for-commands! p)
      (with-interaction-environment p
	(lambda ()
	  (let ((command (car maybe-command)))
	    (if (procedure? command)
		(command)
		(execute-command (car maybe-command))))))))

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
					      more-structures))
	 (exec-package (make-exec-package 'exec user built-in-structures)))
    (values (make-user-context
	     (lambda ()
	       (set-user-environment! user)
	       (set-config-package! config-package)
	       (set-user-command-environment! exec-package)))
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

; Exec package

(define (make-exec-package name user built-in-structures)
  (let* ((scheme-structure (*structure-ref built-in-structures 'scheme))
	 (for-syntax (package-for-syntax user))
	 (exec-package
	  (make-simple-package (list (command-structure) scheme-structure)
			       eval
			       (trivial-delay for-syntax)
			       name)))
    exec-package))

;(define (make-trivial-package name opens user built-in-structures)
;  (make-simple-package (map (lambda (spec)
;                              (if (symbol? spec)
;                                  (*structure-ref built-in-structures spec)
;                                  spec)))
;                       eval
;                       (trivial-delay (package-for-syntax user))
;                       name))

(define (trivial-delay thing)  ;extracted for gc purposes
  (delay thing))
