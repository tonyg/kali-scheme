; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Accessing packages

(define (environment-ref package name)
  (let ((binding (package-lookup package name)))
    (if (unbound? binding)
	(error "unbound variable" name package)
	(contents-carefully binding package name))))

(define (environment-set! package name value)
  (let ((binding (package-lookup package name)))
    (if (unbound? binding)
	(error "unbound variable" name package)
	(if (not (variable-type? (binding-type binding)))
	    (error "invalid assignment" name package value)
	    (set-contents! (binding-place binding) value)))))

(define (environment-define! package name value)
  (set-contents! (package-define! package name usual-variable-type) value))

(define (*structure-ref struct name)
  (let ((binding (structure-lookup struct name #f)))
    (cond ((unbound? binding)
	   (error "structure-ref: name not exported" struct name))
	  (else (contents-carefully binding struct name)))))

(define (contents-carefully binding env name)
  (if (and (binding? binding)
	   (location? (binding-place binding)))
      (if (eq? (binding-type binding) syntax-type)
	  (error "attempt to reference syntax as variable" name env)
	  (let ((loc (binding-place binding)))
	    (if (location-defined? loc)
		(if (location-assigned? loc)
		    (contents loc)
		    (error "unassigned variable" name env))
		(error "unbound variable" name env))))
      (error "peculiar binding" name env)))



; Interaction environment

(define $interaction-environment (make-fluid #f))

(define (interaction-environment)
  (fluid $interaction-environment))

(define (set-interaction-environment! p)
  (if (package? p)
      (set-fluid! $interaction-environment p)
      (call-error "invalid package" set-interaction-environment! p)))

(define (with-interaction-environment p thunk)
  (if (package? p)
      (let-fluid $interaction-environment p thunk)
      (call-error "invalid package" with-interaction-environment p)))


; Scheme report environment.  Should be read-only; fix later.

(define (scheme-report-environment n)
  (if (= n *scheme-report-number*)
      *scheme-report-environment*
      (error "no such Scheme report environment")))

(define *scheme-report-environment* #f)
(define *scheme-report-number* 0)

(define (set-scheme-report-environment! repnum env)
  (set! *scheme-report-number* repnum)
  (set! *scheme-report-environment* env))



; Infinite tower of packages for syntax

(define (make-package-for-syntax eval structs)
  (let recur ()
    (make-simple-package structs
			 eval
			 (delay (recur)) ;package for syntax
			 'for-syntax)))
