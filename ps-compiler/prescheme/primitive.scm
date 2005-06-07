; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.

; Eval'ing and type-checking code for primitives.

(define-record-type primitive
  (id                ; for debugging & making tables
   arg-predicates    ; predicates for checking argument types
   eval              ; evaluation function
   source            ; close-compiled source (if any)
   expander          ; convert call to one using primops
   expands-in-place? ; does the expander expand the definition in-line?
   inference-rule    ; type inference rule
   )
  ())

(define make-primitive primitive-maker)

(define-record-discloser type/primitive
  (lambda (primitive)
    (list 'primitive (primitive-id primitive))))

(define (eval-primitive primitive args)
  (cond ((not (primitive? primitive))
	 (user-error "error while evaluating: ~A is not a procedure" primitive))
	((args-okay? args (primitive-arg-predicates primitive))
	 (apply (primitive-eval primitive) args))
	(else
	 (user-error "error while evaluating: type error ~A"
		     (cons (primitive-id primitive) args)))))

; PREDICATES is a (possibly improper) list of predicates that should match
; ARGS.

(define (args-okay? args predicates)
  (cond ((atom? predicates)
	 (if predicates
	     (every? predicates args)
	     #t))
	((null? args)
	 #f)
	((car predicates)
	 (and ((car predicates) (car args))
	      (args-okay? (cdr args) (cdr predicates))))
	(else
	 (args-okay? (cdr args) (cdr predicates)))))

