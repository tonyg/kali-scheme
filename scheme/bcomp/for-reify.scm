; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Things used by the expression returned by REIFY-STRUCTURES.
; Cf. link/reify.scm.

(define (operator name type-exp)
  (get-operator name (sexp->type type-exp #t)))

(define (primop name)
  (get-primop name))

(define (simple-interface names types)
  (make-simple-interface #f
			 (map (lambda (name type)
				(list name (sexp->type type #t)))
			      (vector->list names)
			      (vector->list types))))

(define (package names locs get-location uid)
  (let ((end (vector-length names))
	(p (make-package list list ;(lambda () '())
			 #f #f "" '()
			 uid #f)))
    (set-package-loaded?! p #t)
    (do ((i 0 (+ i 1)))
	((= i end))
      (let* ((name (vector-ref names i))
	     (probe (package-lookup p name)))
	(if (not (binding? probe))
	    (package-define! p
			     name
			     usual-variable-type
			     (get-location (vector-ref locs i))
			     #f))))
    (make-table-immutable! (package-definitions p))
    p))

(define (transform names+proc env type-exp source name)
  (make-transform names+proc env (sexp->type type-exp #t) source name))

(define (package-define-static! package name static)
  (package-define! package
		   name
		   (cond ((transform? static)
			  (transform-type static))
			 ((primop? static)
			  (primop-type static))
			 ((operator? static)
			  (operator-type static))
			 ((structure? static)
			  structure-type)
			 (else
			  (error "unknown kind of static value" static)))
		   #f
		   static))
