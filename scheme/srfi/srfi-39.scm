; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Parameters are like fluids, but support mutation, and have a really
; awkward API.

; Note that the parameter cells are shared among threads, which gives
; us semantics different from, say, MzScheme, but probably the same as
; Gambit-C.

(define *return-fluid* (list 'return-fluid))

(define make-parameter
  (lambda (init . conv)
    (let* ((converter
	    (if (null? conv) (lambda (x) x) (car conv)))
	   (global-cell
	    (make-cell (converter init)))
	   ($fluid (make-fluid global-cell)))
      (letrec ((parameter
		(lambda new-val
		  (let ((cell (fluid $fluid)))
		    (cond ((null? new-val)
			   (cell-ref cell))
			  ((not (null? (cdr new-val)))
			   (apply assertion-violation
				  'make-parameter
				  "parameter object called with more than one argument"
				  parameter new-val))
			  ((eq? (car new-val) *return-fluid*)
			   $fluid)
			  (else
			   (cell-set! cell (converter (car new-val)))))))))
	parameter))))


(define-syntax parameterize
  (syntax-rules ()
    ((parameterize ((?expr1 ?expr2) ...) ?body ...)
     ;; massage the argument list for LET-FLUIDS
     (parameterize-helper (((?expr1 *return-fluid*) (make-cell ?expr2)) ...)
			  ?body ...))))

(define-syntax parameterize-helper
  (syntax-rules ()
    ((parameterize-helper ((?stuff ...) ...) ?body ...)
     (let-fluids ?stuff ... ... (lambda () ?body ...)))))
