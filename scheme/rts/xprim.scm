; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Hairier exceptions & interrupts.
; Enable generic arithmetic, informative error messages, etc.

; Deal with optional arguments, etc. to primitives.
; This is not necessarily the cleanest way to do this, and certainly not
; the most efficient, but for the time being it's the most expedient.

; We don't want to depend on tables.  But if we did, we might do this:
;(define (closure-hash closure)
;  (let ((cv (vector-ref (closure-template closure) 0)))  ;template-ref
;    (do ((h 0 (+ h (code-vector-ref cv i)))
;         (i (- (code-vector-length cv) 1) (- i 1)))
;        ((< i 0) h))))
;(define wna-handlers (make-table closure-hash))

(define-exception-handler (enum op check-nargs=)
  (lambda (opcode reason proc args)
    (let ((probe (assq proc *wna-handlers*)))
      (if probe
	  ((cdr probe) args)
	  (signal-exception opcode reason proc args)))))

(define *wna-handlers* '())

(define (define-wna-handler proc handler)
  (set! *wna-handlers* (cons (cons proc handler) *wna-handlers*)))

(define op/check-nargs= (enum op check-nargs=))

(define (wna-lose proc args)
  (signal-exception op/check-nargs= #f proc args))  ; lost our reason

(define-wna-handler + (lambda (args) (reduce + 0 args)))

(define-wna-handler * (lambda (args) (reduce * 1 args)))

(define-wna-handler bitwise-ior (lambda (args) (reduce bitwise-ior 0 args)))
(define-wna-handler bitwise-xor (lambda (args) (reduce bitwise-xor 0 args)))
(define-wna-handler bitwise-and (lambda (args) (reduce bitwise-and -1 args)))

(define-wna-handler -
  (lambda (args)
    (if (and (not (null? args))
		  (null? (cdr args)))
	(- 0 (car args))
	(wna-lose - args))))

(define-wna-handler /
  (lambda (args)
    (if (and (not (null? args))
	     (null? (cdr args)))
	(/ 1 (car args))
	(wna-lose / args))))

(define-wna-handler make-vector
  (lambda (args)
    (if (and (not (null? args))
	     (null? (cdr args)))
	(make-vector (car args) (unspecific))
	(wna-lose make-vector args))))

(define-wna-handler make-string
  (lambda (args)
    (if (and (not (null? args))
	     (null? (cdr args)))
	(make-string (car args) #\?)
	(wna-lose make-string args))))

(define-wna-handler apply
  (lambda (args)
    (if (null? args)
	(wna-lose apply args))
	(apply (car args)
	       (let recur ((l (cdr args)))
		 (if (null? (cdr l))
		     (car l)
		     (cons (car l) (recur (cdr l))))))))

(define-wna-handler error
  (lambda (args)
    (really-signal-condition (cons 'error args))))

(define (comparison-wna compare)	;Not really R4RS compliant.
  (lambda (args)
    (if (and (not (null? args))
	     (not (null? (cdr args))))
	(let loop ((x (car args))
		   (args (cdr args)))
	  (let ((y (car args))
		(args (cdr args)))
	    (if (compare x y)
		(if (null? args)
		    #t
		    (loop y args))
		#f)))
	(wna-lose compare args))))

(define-wna-handler = (comparison-wna =))
(define-wna-handler < (comparison-wna <))
(define-wna-handler > (comparison-wna >))
(define-wna-handler <= (comparison-wna <=))
(define-wna-handler >= (comparison-wna >=))
