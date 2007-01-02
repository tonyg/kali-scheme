; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; The reference implementation is written in some weird Scheme variant.
; This is an attempt to produce the same result using SYNTAX-RULES.

; I found the both the specification and the implementation unhelpful.
; For example, one would think that (AND-LET* ()) -> #T by analogy with
; (AND) -> #T.  The specification doesn't say.
;
; The following behaves correctly on the test cases at the end of the
; reference implementation,  except that it doesn't catch the three syntax
; errors.  There is no way for SYNTAX-RULES to distinguish between a
; constant and a variable, and no easy way to check if a variable is
; being used twice in the same AND-LET* (and why is that an error? LET*
; allows it).

(define-syntax and-let*
  (syntax-rules ()

    ; No body - behave like AND.
    ((and-let* ())
     #t)
    ((and-let* ((var exp)))
     exp)
    ((and-let* ((exp)))
     exp)
    ((and-let* (var))
     var)

    ; Have body - behave like LET* but check for #F values.

    ; No clauses so just use the body.
    ((and-let* () . body)
     (begin . body))

    ; (VAR VAL) clause - bind the variable and check for #F.
    ((and-let* ((var val) more ...) . body)
     (let ((var val))
       (if var
	   (and-let* (more ...) . body)
	   #f)))

    ; Error check to catch illegal (A B ...) clauses.
    ((and-let* ((exp junk . more-junk) more ...) . body)
     (error "syntax error"
	    '(and-let* ((exp junk . more-junk) more ...) . body)))

    ; (EXP) and VAR - just check the value for #F.
    ; There is no way for us to check that VAR is an identifier and not a
    ; constant
    ((and-let* ((exp) more ...) . body)
     (if exp
	 (and-let* (more ...) . body)
	 #f))
    ((and-let* (var more ...) . body)
     (if var
	 (and-let* (more ...) . body)
	 #f))))

;(define-syntax expect
;  (syntax-rules ()
;    ((expect a b)
;     (if (not (equal? a b))
;         (error "test failed" 'a b)))))
;
;(expect  (and-let* () 1) 1)
;(expect  (and-let* () 1 2) 2)
;(expect  (and-let* () ) #t)
;
;(expect (let ((x #f)) (and-let* (x))) #f)
;(expect (let ((x 1)) (and-let* (x))) 1)
;(expect (and-let* ((x #f)) ) #f)
;(expect (and-let* ((x 1)) ) 1)
;;(must-be-a-syntax-error (and-let* ( #f (x 1))) )
;(expect (and-let* ( (#f) (x 1)) ) #f)
;;(must-be-a-syntax-error (and-let* (2 (x 1))) )
;(expect (and-let* ( (2) (x 1)) ) 1)
;(expect (and-let* ( (x 1) (2)) ) 2)
;(expect (let ((x #f)) (and-let* (x) x)) #f)
;(expect (let ((x "")) (and-let* (x) x)) "")
;(expect (let ((x "")) (and-let* (x)  )) "")
;(expect (let ((x 1)) (and-let* (x) (+ x 1))) 2)
;(expect (let ((x #f)) (and-let* (x) (+ x 1))) #f)
;(expect (let ((x 1)) (and-let* (((positive? x))) (+ x 1))) 2)
;(expect (let ((x 1)) (and-let* (((positive? x))) )) #t)
;(expect (let ((x 0)) (and-let* (((positive? x))) (+ x 1))) #f)
;(expect (let ((x 1)) (and-let* (((positive? x)) (x (+ x 1))) (+ x 1)))  3)
;;(must-be-a-syntax-error
;;  (let ((x 1)) (and-let* (((positive? x)) (x (+ x 1)) (x (+ x 1))) (+ x 1)))
;;)
;
;(expect (let ((x 1)) (and-let* (x ((positive? x))) (+ x 1))) 2)
;(expect (let ((x 1)) (and-let* ( ((begin x)) ((positive? x))) (+ x 1))) 2)
;(expect (let ((x 0)) (and-let* (x ((positive? x))) (+ x 1))) #f)
;(expect (let ((x #f)) (and-let* (x ((positive? x))) (+ x 1))) #f)
;(expect (let ((x #f)) (and-let* ( ((begin x)) ((positive? x))) (+ x 1))) #f)
;
;(expect  (let ((x 1)) (and-let* (x (y (- x 1)) ((positive? y))) (/ x y))) #f)
;(expect  (let ((x 0)) (and-let* (x (y (- x 1)) ((positive? y))) (/ x y))) #f)
;(expect  (let ((x #f)) (and-let* (x (y (- x 1)) ((positive? y))) (/ x y))) #f)
;(expect  (let ((x 3)) (and-let* (x (y (- x 1)) ((positive? y))) (/ x y))) 3/2)

