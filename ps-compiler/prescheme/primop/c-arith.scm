; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

(define-local-syntax (define-c-arith-binop-generator id c-op)
  `(define-c-generator ,id #t
     (lambda (call port indent)
       (simple-c-primop ,c-op call port))))

(define-c-arith-binop-generator + "+")
(define-c-arith-binop-generator - "-")
(define-c-arith-binop-generator * "*")
(define-c-arith-binop-generator quotient  "/")

(define-c-arith-binop-generator fl+ "+")
(define-c-arith-binop-generator fl- "-")
(define-c-arith-binop-generator fl* "*")
(define-c-arith-binop-generator fl/ "/")

(define-c-generator small* #t
  (lambda (call port indent)
    (format port "PS_SMALL_MULTIPLY(")
    (c-value (call-arg call 0) port)
    (format port ", ")
    (c-value (call-arg call 1) port)
    (format port ")")))

(define-c-arith-binop-generator remainder "%")
(define-c-arith-binop-generator bitwise-and "&")
(define-c-arith-binop-generator bitwise-ior "|")
(define-c-arith-binop-generator bitwise-xor "^")

(define-c-generator ashl #t
  (lambda (call port indent)
    (generate-shift call port indent "LEFT" "<<" #f)))

(define-c-generator ashr #t
  (lambda (call port indent)
    (generate-shift call port indent "RIGHT" ">>" #f)))

(define-c-generator lshr #t
  (lambda (call port indent)
    (generate-shift call port indent "RIGHT_LOGICAL" ">>" #t)))

(define (generate-shift call port indent macro c-op logical?)
  (cond ((= 1 (call-exits call))
	 ; PS_SHIFT_??? is a C macro that handles overshifting even if C doesn't
	 (indent-to port indent)
	 (format port "PS_SHIFT_~A(" macro)
	 (if logical? (format port "(unsigned long)"))
	 (c-value (call-arg call 1) port)
	 (format port ", ")
	 (c-value (call-arg call 2) port)
	 (format port ", ")
	 (c-variable (car (lambda-variables (call-arg call 0))) port)
	 (format port ")"))
	((>= (literal-value (call-arg call 1)) prescheme-integer-size)
	 (format port "0L"))
	(else
	 (if logical?
	     (format port "(long)(((unsigned long)")
	     (format port "(("))
	 (c-value (call-arg call 0) port)
	 (format port ")~A" c-op)
	 (c-value (call-arg call 1) port)
	 (format port ")"))))
    
(define-c-generator bitwise-not #t
  (lambda (call port indent)
    (simple-c-primop "~" call port)))

(define-local-syntax (define-c-comp-binop-generator id c-op)
  `(define-c-generator ,id #t
     (lambda (call port indent)
       (simple-c-primop ,c-op call port))))

(define-c-comp-binop-generator =      "==")
(define-c-comp-binop-generator <      "<" )
(define-c-comp-binop-generator fl=    "==")
(define-c-comp-binop-generator fl<    "<" )
(define-c-comp-binop-generator char=? "==")
(define-c-comp-binop-generator char<? "<" )

(define-c-generator ascii->char #t
  (lambda (call port indent)
    (c-value (call-arg call 0) port)))
    
(define-c-generator char->ascii #t
  (lambda (call port indent)
    (c-value (call-arg call 0) port)))
    
;(define-c-generator sign-extend #t
;  (lambda (call port indent)
;    (display "((long) " port)
;    (c-value (call-arg call 0) port)
;    (display ")" port)))
;
;(define-c-generator zero-extend #t
;  (lambda (call port indent)
;    (display "((unsigned long) " port)
;    (c-value (call-arg call 0) port)
;    (display ")" port)))
