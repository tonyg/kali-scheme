; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Accessing packages

(define (environment-ref package name)
  (carefully (package-lookup package name)
	     (lambda (loc)
	       (if (location-assigned? loc)
		   (contents loc)
		   (error "uninitialized variable" name package)))
	     package
	     name))

(define (environment-set! package name value)
  (let ((binding (package-lookup package name)))
    (if (and (binding? binding)
	     (not (variable-type? (binding-type binding))))
	(error "invalid assignment" name package value)
	(carefully binding
		   (lambda (loc)
		     (set-contents! loc value))
		   package name))))

(define (environment-define! package name value)
  (set-contents! (package-define! package name usual-variable-type #f #f)
		 value))

(define (*structure-ref struct name)
  (let ((binding (structure-lookup struct name #f)))
    (if binding
	(carefully binding contents struct name)
	(error "structure-ref: name not exported" struct name))))

(define (carefully binding action env name)
  (cond ((not binding)
	 (error "unbound variable" name env))
	((not (binding? binding))
	 (error "peculiar binding" binding name env))
	((eq? (binding-type binding) syntax-type)
	 (error "attempt to reference syntax as variable" name env))
	(else
	 (let ((loc (binding-place binding)))
	   (if (location? loc)
	       (if (location-defined? loc)
		   (action loc)
		   (error "unbound variable" name env))
	       (error "variable has no location" name env))))))

; Interaction environment

(define $interaction-environment (make-fluid (make-cell #f)))

(define (interaction-environment)
  (fluid-cell-ref $interaction-environment))

(define (set-interaction-environment! p)
  (if (package? p)
      (fluid-cell-set! $interaction-environment p)
      (call-error "invalid package" set-interaction-environment! p)))

(define (with-interaction-environment p thunk)
  (if (package? p)
      (let-fluid $interaction-environment (make-cell p) thunk)
      (call-error "invalid package" with-interaction-environment p)))

; Scheme report environment.  Should be read-only; fix later.

(define (scheme-report-environment n)
  (if (= n *scheme-report-number*)
      *scheme-report-environment*
      (error "no such Scheme report environment")))

(define *scheme-report-environment* #f)
(define *null-environment* #f)
(define *scheme-report-number* 0)

(define (set-scheme-report-environment! repnum env)
  (set! *scheme-report-number* repnum)
  (set! *scheme-report-environment* env)
  (set! *null-environment* env))		; A cheat.

(define (null-environment n)
  (if (= n *scheme-report-number*)
      *null-environment*
      (error "no such Scheme report environment")))

; Make an infinite tower of packages for syntax.
; structs should be a non-null list of structures that should be
; opened at EVERY level of the tower.

(define (make-reflective-tower eval structs id)
  (let recur ((level 1))
    (delay (cons eval
		 (make-simple-package structs
				      eval
				      (recur (+ level 1))
				      `(for-syntax ,level ,id))))))

; (set-reflective-tower-maker! p (lambda (clauses id) ...))
; where clauses is a list of DEFINE-STRUCTURE clauses

(define set-reflective-tower-maker!
  (let ((name (string->symbol ".make-reflective-tower.")))
    (lambda (p proc)
      (environment-define! p name proc))))
