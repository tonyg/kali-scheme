; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; The output of alpha-conversion is an expression tree that can be
; passed to the ordinary Scheme48 compiler, which expects the usual
; S-expression representation.  Each subexpression is a list whose head
; is an operator.

(define operator/quote  (get-operator 'quote 'syntax))
(define operator/begin  (get-operator 'begin 'syntax))
(define operator/lambda (get-operator 'lambda 'syntax))
(define operator/local (get-operator 'local 'syntax))
(define operator/call	(get-operator 'call 'syntax))
(define operator/set!	(get-operator 'set! 'syntax))
(define operator/if	(get-operator 'if 'syntax))
(define operator/primitive-procedure (get-operator 'primitive-procedure 'syntax))

(define (make-quotation thing)
  `(,operator/quote ,thing))

(define (quotation-value q)
  (if (not (eq? (car q) operator/quote))
      (error "not a quotation" q))
  (cadr q))


(define (make-call proc args)
  `(,proc ,@args))

(define call-proc car)
(define call-args cdr)
  

; lambda

(define (make-lambda formals local-list body)
  `(,operator/lambda ,formals
		     ,(make-quotation local-list)
		     ,body))

(define lambda-formals cadr)
(define (lambda-locals exp) (quotation-value (caddr exp)))
(define (lambda-body form)
  (let ((body (cddr form)))
    (if (and (eq? (caar body) operator/quote)
	     (null? (cddr body)))
	(cadr body)
	(error "unexpected format for lambda" exp))))


; begin

(define (make-begin first second)
  `(,operator/begin ,first ,second))

(define begin-first cadr)
(define begin-second caddr)


; set!

(define (make-set! den rhs)
  `(,operator/set! ,(if (local? den)
			(begin (note-assignment! den)
			       (local-name den))
			den)
		   ,rhs))


; if

(define (make-if test con alt)
  `(,operator/if ,test ,con ,alt))

(define *unspecific* (list (get-operator 'unspecific 'procedure)))

(define if-test cadr)
(define if-con caddr)
(define if-alt cadddr)

; primitive-procedure

(define (make-primitive-procedure name)
  `(,operator/primitive-procedure ,name))


; Local variables

(define (make-local name)
  (list operator/local name 0 0 0))

(define (note-value-reference! local)
  (let ((z (cddr local))) (set-car! z (+ 1 (car z)))))

(define (note-operator-reference! local)
  (let ((z (cdddr local))) (set-car! z (+ 1 (car z)))))

(define (note-assignment! local)
  (let ((z (cddddr local))) (set-car! z (+ 1 (car z)))))

(define local-name cadr)
(define local-value-reference-count caddr)
(define local-operator-reference-count cadddr)
(define (local-assignment-count l) (cadddr (cdr l)))

(define (local-reference-count l)
  (let ((z (cddr l))) (+ (car z) (cadr z))))

(define (local? thing)
  (and (pair? thing)
       (eq? (car thing) operator/local)))


