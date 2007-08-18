; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file fluid.scm.

; Fluid (dynamic) variables.

; Fluid variables are implemented using deep binding.  This allows
; each thread in a multiprocessor system to have its own fluid
; environment, and allows for fast thread switching in a multitasking
; one.

; CURRENT-THREAD and SET-CURRENT-THREAD! access a special virtual
; machine register.  On a multiprocessor, each processor would have
; its own current-thread register.  The run-time system stores the
; current thread in this register.

; Here we define a particular thread record, but a different one is
; defined by the (uniprocessor) threads package.  The current thread
; may actually be any kind of record as long as its first component
; can be used by the fluid variable implementation to maintain the
; deep-binding dynamic environment and its second component can be
; used by DYNAMIC-WIND.  This is kind of gross but it is motivated by
; efficiency concerns.

(define-record-type thread :thread
  (make-thread dynamic-env dynamic-point proposal)
  (dynamic-env   thread-dynamic-env)
  (dynamic-point thread-dynamic-point)
  (proposal      thread-proposal))	; only accessed by the VM

(define (get-dynamic-env)
  (record-ref (current-thread) 1))

(define (set-dynamic-env! env)
  (record-set! (current-thread) 1 env))

; The dynamic-wind point used to be just an ordinary fluid variable, but that
; doesn't work well with threads.

(define (get-dynamic-point)
  (record-ref (current-thread) 2))

(define (set-dynamic-point! point)
  (record-set! (current-thread) 2 point))

(define (initialize-dynamic-state!)
  (set-current-thread! (make-thread (empty-dynamic-env) #f #f)))

;----------------
; Dynamic environment
; A dynamic environment is an alist where the cars are fluid records.

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


;; kali - begin

; Each fluid has a top-level value that is used when the fluid is unbound
; in the current dynamic environment.

; Kali needs prevent the base of fluid environments, which is shared between
; all threads, from being sent between address spaces.  To do so we put that
; base in the session-data record and make (FLUID ...) look there after
; reaching the top of the fluid state.

(define-record-type fluid :fluid
  (really-make-fluid top)
  fluid?
  (top really-fluid-top-level-value really-set-fluid-top-level-value!))

(define (make-fluid top)
  (make-proxy (really-make-fluid top)))

(define (fluid-top-level-value fluid)
  (really-fluid-top-level-value (proxy-local-ref fluid)))

(define (set-fluid-top-level-value! fluid-proxy value)
  (let ((fluid (any-proxy-value fluid-proxy)))
    (really-set-fluid-top-level-value! fluid value)
    (proxy-local-set! fluid-proxy fluid)))

; Extending fluid environments to allow intervening procedures.

(define (real-fluid-lookup fluid-env fluid)
  (let loop ((env fluid-env))
    (cond ((null? env)
	   #f)
	  ((procedure? env)
	   (env fluid))
	  ((eq? (caar env) fluid)
	   (car env))
	  (else
	   (loop (cdr env))))))

(define (fluid f)
  (let ((probe (real-fluid-lookup (get-dynamic-env) f)))
    (if probe (cdr probe) (fluid-top-level-value f))))

; Deprecated.

(define (set-fluid! f val)
  (let ((probe (real-fluid-lookup (get-dynamic-env) f)))
    (if probe (set-cdr! probe val) (set-fluid-top-level-value! f val))))

;; kali - end

;; non-kali was - begin

;; Each fluid has a top-level value that is used when the fluid is unbound
;; in the current dynamic environment.

;(define-record-type fluid :fluid
;  (make-fluid top)
;  (top fluid-top-level-value set-fluid-top-level-value!))

;(define (fluid f)
;  (let ((probe (assq f (get-dynamic-env))))
;    (if probe (cdr probe) (fluid-top-level-value f))))

;; Deprecated.

;(define (set-fluid! f val)
;  (let ((probe (assq f (get-dynamic-env))))
;    (if probe (set-cdr! probe val) (set-fluid-top-level-value! f val))))

;; non-kali was - end

(define (let-fluid f val thunk)
  (with-dynamic-env (cons (cons f val) (get-dynamic-env)) thunk))

(define (let-fluids . args)
  (let loop ((args args)
	     (env (get-dynamic-env)))
    (if (null? (cdr args))
	(with-dynamic-env env (car args))
	(loop (cddr args)
	      (cons (cons (car args) (cadr args)) env)))))


;; kali - begin - probably unnecessary

(define (fluid-lookup env fluid)
  (let ((probe (real-fluid-lookup (get-dynamic-env) fluid)))
    (if probe (cdr probe) (fluid-top-level-value fluid))))

;; kali - end - probably unnecessary

; Handy utilities.

(define (fluid-cell-ref f)
  (cell-ref (fluid f)))

(define (fluid-cell-set! f value)
  (cell-set! (fluid f) value))

; Initialize

(initialize-dynamic-state!)

