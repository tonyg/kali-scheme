; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; This is file fluid.scm.

; Fluid (dynamic) variables.

; Fluid variables are implemented using deep binding.  This allows
; each thread in a multiprocessor system to have its own fluid
; environment, and allows for fast thread switching in a multitasking
; one.

; current-thread and set-current-thread! access a special virtual
; machine register.  On a multiprocessor, each processor has its own
; current-thread register.  The run-time system stores the current
; thread in this register.

; Here we define a particular thread record, but a different one is
; defined by the (uniprocessor) threads package.  The current thread
; may actually be any kind of record as long as its first component
; can be used by the fluid variable implementation to maintain the
; deep-binding dynamic environment.  This is kind of gross but it is
; motivated by efficiency concerns.

(define-record-type thread :thread
  (make-thread dynamic-env)
  (dynamic-env thread-dynamic-env))

(define (get-dynamic-env)
  (record-ref (current-thread) 1))
(define (set-dynamic-env! env)
  (record-set! (current-thread) 1 env))

(define (initialize-dynamic-state!)
  (set-current-thread! (make-thread (empty-dynamic-env))))


; Dynamic environment

(define (with-dynamic-env env thunk)
  (let ((saved-env (get-dynamic-env)))
    (set-dynamic-env! env)
    (set! env #f)			;For GC and debugger
    (call-with-values
	;; thunk
	(let ((x thunk)) (set! thunk #f) x) ;For GC
      (lambda results
	(set-dynamic-env! saved-env)
	(apply values results)))))

(define (empty-dynamic-env) '())


; Fluids

(define-record-type fluid :fluid
  (make-fluid top)
  (top fluid-top-level-value set-fluid-top-level-value!))

(define (fluid f)
  (let ((probe (assq f (get-dynamic-env))))
    (if probe (cdr probe) (fluid-top-level-value f))))

(define (set-fluid! f val)
  (let ((probe (assq f (get-dynamic-env))))
    (if probe (set-cdr! probe val) (set-fluid-top-level-value! f val))))

(define (let-fluid f val thunk)
  (with-dynamic-env (cons (cons f val) (get-dynamic-env)) thunk))

(define (let-fluids . args)
  (let loop ((args args)
	     (env (get-dynamic-env)))
    (if (null? (cdr args))
	(with-dynamic-env env (car args))
	(loop (cddr args)
	      (cons (cons (car args) (cadr args)) env)))))

(define (fluid-lookup env f)
  (let ((probe (assq f env)))
    (if probe (cdr probe) (fluid-top-level-value f))))


; Initialize

(initialize-dynamic-state!)

