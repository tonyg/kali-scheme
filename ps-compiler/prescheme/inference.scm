; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.


; Type Inference
;
; The entry points to the inferencer are:
;
; (unify! type1 type2 context)
;   Unify TYPE1 and TYPE2.  CONTEXT is used to provide user feedback when type
;   errors are detected.
;
; (make-uvar prefix depth . maybe-id)
;   Makes a new type variable.  PREFIX is a symbol, DEPTH is the current type
;   depth (used for polymorphism), and MAYBE-ID is an optional unique
;   integer.
;
; (schemify-type type depth)
;   Make TYPE polymorphic in any variables bound at DEPTH.
;
; (instantiate-type-scheme scheme depth)
;   Return an instantiation of SCHEME at DEPTH.
;
; (reset-inference!)
;   Clear various global variables (to be replaced with fluids at some point)


(define (unify! type1 type2 context)
  (cond ((really-unify! type1 type2)
	 => (lambda (error-thunk)
	      (unify-lost error-thunk type1 type2 context)))))

(define *currently-checking* #f)
(define *current-top-exp* #f)

(define (unify-lost error-thunk type1 type2 context)
  (cond ((eq? context 'simplifying)
	 (bug "unification error while instantiating an integrable procedure"))
	((eq? context 'make-monomorphic)
	 #f)
	(else
	 (user-type-error-message error-thunk type1 type2 context))))

(define (user-type-error-message error-thunk type1 type2 context)
  (format #t "Type error in ~S~% " (schemify context))
  (error-thunk)
  (if *currently-checking*
      (begin
	(format #t "~% while reconstructing the type of~%  ")
	(*currently-checking* (current-output-port))))
  (error "type problem"))

(define (really-unify! p1 p2)
  (let ((p1 (maybe-follow-uvar p1))    ; get the current value of P1
	(p2 (maybe-follow-uvar p2)))   ; get the current value of P2
    (cond ((or (eq? p1 p2)
	       (eq? p1 type/null)
	       (eq? p2 type/null))
	   #f)
	  ((uvar? p1)
	   (bind-uvar! p1 p2))
	  ((uvar? p2)
	   (bind-uvar! p2 p1))
	  ((or (eq? p1 type/unit)
	       (eq? p2 type/unit))
	   #f)
	  ((other-type? p1)
	   (if (and (other-type? p2)
		    (eq? (other-type-kind p1) (other-type-kind p2))
		    (= (length (other-type-subtypes p1))
		       (length (other-type-subtypes p2))))
	       (unify-lists! (other-type-subtypes p1)
			     (other-type-subtypes p2))
	       (mismatch-failure p1 p2)))
	  (else
	   (mismatch-failure p1 p2)))))

(define (mismatch-failure t1 t2)
  (lambda ()
    (format #t "type mismatch~% ")
    (display-type t1 (current-output-port))
    (format #t "~% ")
    (display-type t2 (current-output-port))))

(define (unify-lists! l1 l2)
  (let loop ((l1 l1) (l2 l2))
    (if (null? l1)
	#f
	(or (really-unify! (car l1) (car l2))
	    (loop (cdr l1) (cdr l2))))))

(define (type-conflict message . stuff)
  (apply breakpoint message stuff))

; For debugging
(define (uvar-name uvar)
  (concatenate-symbol (uvar-prefix uvar) "." (uvar-id uvar)))

