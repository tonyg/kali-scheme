; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; See comments at top of packages-for-debugging.scm for information on
; using this.

(define underlying-error (structure-ref signals error))

(define (unimplemented name)
  (lambda args (underlying-error "unimplemented primitive" name args)))


; --------------------
; Things that are in the compiler base (= all-primitives structure)
; but not anywhere else.

; Don't let the VM trap

(define (unassigned) '(*unassigned*))


; Don't let our locations confuse the native debugger.

(define (make-location value id)
  ((structure-ref locations make-location) value (list id)))

(define (location-id loc)
  (let ((id ((structure-ref locations location-id) loc)))
    (if (and (pair? id)
	     (null? (cdr id)))
	(car id)
	id)))


; Hacks for base.scm

(define (find-all-symbols table) table)

(define (intern string table)
  (string->symbol string))


; Shadow some "privileged" operations to avoid undebuggable crashes.

(define (trap n)
  (underlying-error "trap instruction" n))

(define (error message . rest)
  (underlying-error "error called" (cons message rest)))

(define write-image (unimplemented 'write-image))


(define (open-port name how)
  ((case how
     ((1) open-input-file)
     ((2) open-output-file)
     (else (underlying-error "invalid argument to open-port" name how)))
   name))

(define (reverse-list->string l n)
  (list->string (reverse l)))

(define (false) #f)


; Funny internal syntax.

(define-syntax primitive-procedure
  (lambda (e r c)
    `(lambda x (,(r 'underlying-error)
		"primitive-procedure is unimplemented"
		',e))))

(define-syntax local
  (lambda (e r c) (cadr e)))
      
(define-syntax call
  (lambda (e r c)
    `((,(r 'begin) ,(cadr e)) ,@(cddr e))))

