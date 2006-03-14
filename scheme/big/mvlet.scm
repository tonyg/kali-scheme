; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; A version of LET and LET* which allows clauses that return multiple values.
;
; MV = multiple-value
;
; (mvlet (<clause> ...) <body>)
; (mvlet* (<clause> ...) <body>)
;
; <clause> ::= (<ids> <expression>)
; <ids> ::= <id> | (<id> ...) | (<id> ... . <id>)
;
; A clause of the form (<id> <exp>) is like a normal LET clause.  There is no
; clause equivalent to
;   (call-with-values (lambda () <expression>)
;                     (lambda <id> <body>))

(define-syntax mvlet
  (syntax-rules ()
    ((mvlet () body ...)
     (let () body ...))
    ((mvlet (clause ...) body ...)
     (mvlet-helper (clause ...) () (body ...)))))

(define-syntax mvlet-helper
  (syntax-rules ()
    ((mvlet-helper () clauses (body ...))
     (let clauses body ...))
    ((mvlet-helper (((var . more-vars) val) more ...) clauses body)
     (copy-vars (var . more-vars) () val (more ...) clauses body))
    ((mvlet-helper ((var val) more ...) clauses body)
     (mvlet-helper (more ...) ((var val) . clauses) body))))

(define-syntax copy-vars
  (syntax-rules ()
    ((copy-vars (var . more-vars) (copies ...)
		val more clauses body)
     (copy-vars more-vars (copies ... x)
		val more ((var x) . clauses) body))
    ((copy-vars () copies val more clauses body)
     (call-with-values
       (lambda () val)
       (lambda copies
	 (mvlet-helper more clauses body))))
    ((copy-vars last (copies ...) val more clauses body)
     (call-with-values
       (lambda () val)
       (lambda (copies ... . lastx)
	 (mvlet-helper more ((last lastx) . clauses) body))))))

(define-syntax mvlet*
  (syntax-rules ()
    ((mvlet* () body ...)
     (let () body ...))
    ((mvlet* (((vars ...) val) clause ...) body ...)
     (call-with-values
       (lambda () val)
       (lambda (vars ...)
	 (mvlet* (clause ...) body ...))))
    ((mvlet* ((var val) clause ...) body ...)
     (let ((var val)) (mvlet* (clause ...) body ...)))))

