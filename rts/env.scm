; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Accessing packages

(define (environment-ref package name)
  (let ((probe (package-lookup package name)))
    (if probe
	(if (syntax? probe)
	    (error "attempt to reference syntax as variable" name package)
	    (let ((loc (package-find-location package name #f)))
	      (if loc
		  (if (location-assigned? loc)
		      (contents loc)
		      (error "unassigned variable" name package))
		  (error (if (or (operator? probe) (transform? probe))
			     "in-line procedure not reified as a value"
			     "denotation/location inconsistency")
			 name package probe loc))))
	(error "unbound variable" name package))))

(define (environment-set! package name value)
  (let ((probe (package-lookup package name)))
    (if probe
	(if (syntax? probe)
	    (error "(set! <syntax> ...)" name package value)
	    (set-contents! (package-find-location package name #t) value))
	(error "attempt to assign an unbound variable" name package value))))

(define (environment-define! package name value)
  (set-contents! (package-ensure-defined! package name) value))



(define (*structure-ref struct name)
  (if (not (signature-ref (structure-signature struct) name))
      (error "structure-ref: name not exported" struct name))
  (environment-ref (structure-package struct) name))


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
