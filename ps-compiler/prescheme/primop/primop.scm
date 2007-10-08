; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


(define prescheme-primop-table (make-symbol-table))

(walk-vector (lambda (primop)
	       (if (primop? primop)
		   (table-set! prescheme-primop-table
			       (primop-id primop)
			       primop)))
	     all-primops)

(define (get-prescheme-primop id)
  (cond ((table-ref prescheme-primop-table id)
	 => identity)
	((name->enumerand id primop)
	 => get-primop)
	(else
	 (bug "Scheme primop ~A not found" id))))

(define (add-scheme-primop! id primop)
  (table-set! prescheme-primop-table id primop))

(define-syntax define-scheme-primop
  (syntax-rules ()
    ((define-scheme-primop id type)
     (define-scheme-primop id #f type))
    ((define-scheme-primop id side-effects type)
     (define-scheme-primop id side-effects type default-simplifier))
    ((define-scheme-primop id side-effects type simplifier)
     (define-polymorphic-scheme-primop
       id side-effects (lambda (call) type) simplifier))))

(define-syntax define-polymorphic-scheme-primop
  (syntax-rules ()
    ((define-polymorphic-scheme-primop id type)
     (define-polymorphic-scheme-primop id #f type))
    ((define-polymorphic-scheme-primop id side-effects type)
     (define-polymorphic-scheme-primop id side-effects type default-simplifier))
    ((define-scheme-primop id side-effects type simplifier)
     (add-scheme-primop! 'id
			 (make-primop 'id #t 'side-effects simplifier
				      (lambda (call) 1)
				      type)))))

(define-syntax define-nonsimple-scheme-primop
  (syntax-rules ()
    ((define-nonsimple-scheme-primop id)
     (define-nonsimple-scheme-primop id #f))
    ((define-nonsimple-scheme-primop id side-effects)
     (define-nonsimple-scheme-primop id side-effects default-simplifier))
    ((define-nonsimple-scheme-primop id side-effects simplifier)
     (add-scheme-primop! 'id
			 (make-primop 'id #f 'side-effects simplifier
				      (lambda (call) 1)
				      'nontrivial-primop)))))

(define-syntax define-scheme-cond-primop
  (syntax-rules ()
    ((define-scheme-cond-primop id simplifier expand simplify?)
     (add-scheme-primop! 'id
			 (make-conditional-primop 'id
						  #f
						  simplifier
						  (lambda (call) 1)
						  expand
						  simplify?)))))

;(define-prescheme! 'error  ; all four args must be present if used as value
;  (lambda (exp env)
;    (let ((string (expand (cadr exp) env #f))
;          (args (map (lambda (arg)
;                       (expand arg env #f))
;                     (cddr exp))))
;      (make-block-exp
;       (list
;        (make-call-exp (get-prescheme-primop 'error)
;                       0
;                       type/unknown
;                       `(,string
;                         ,(make-quote-exp (length args) type/int32)
;                         . ,(case (length args)
;                              ((0)
;                               (list (make-quote-exp 0 type/int32)
;                                     (make-quote-exp 0 type/int32)
;                                     (make-quote-exp 0 type/int32)))
;                              ((1)
;                               (list (car args)
;                                     (make-quote-exp 0 type/int32)
;                                     (make-quote-exp 0 type/int32)))
;                              ((2)
;                               (list (car args)
;                                     (cadr args)
;                                     (make-quote-exp 0 type/int32)))
;                              ((3)
;                               args)
;                              (else
;                               (error "too many arguments to ERROR in ~S" exp))))
;                       exp)
;        (make-quote-exp the-undefined-value type/unknown))))))

; For the moment VALUES is more or less a macro.

;(define-prescheme! 'values   ; dies if used as a value
;  (lambda (exp env)
;    (make-call-exp (get-prescheme-primop 'pack)
;                   0
;                   type/unknown
;                   (map (lambda (arg)
;                          (expand arg env #f))
;                        (cdr exp))
;                   exp)))

; Each arg spec is either #F = non-continuation argument or a list of
; variable (name . type)s for the continuation.

;(define (define-continuation-expander id primop-id arg-specs)
;  (define-primitive-expander id (length arg-specs)
;    (lambda (source args cenv)
;      (receive (conts other)
;          (expand-arguments args arg-specs cenv)
;        (make-call-exp (get-prescheme-primop primop-id)
;                       (length conts)
;                       type/unknown
;                       (append conts other)
;                       source)))))

;(define (expand-arguments args specs cenv)
;  (let loop ((args args) (specs specs) (conts '()) (other '()))
;    (if (null? args)
;        (values (reverse conts) (reverse other))
;        (let ((arg (expand (car args) cenv #f)))
;          (if (not (car specs))
;              (loop (cdr args) (cdr specs) conts (cons arg other))
;              (loop (cdr args) (cdr specs)
;                    (cons (expand-continuation-arg arg (car specs))
;                          conts)
;                    other))))))                                            
;
;(define (expand-continuation-arg arg var-specs)
;  (let* ((vars (map (lambda (p)
;                      (make-variable (car p) (cdr p)))
;                    var-specs)))
;    (make-continuation-exp
;     vars
;     (make-call-exp (get-primop (enum primop unknown-call))
;                    0
;                    type/unknown
;                    `(,arg
;                      ,(make-quote-exp (length vars) #f)
;                      . ,vars)
;                    #f)))) ; no source

; Randomness needed by both arith.scm and c-arith.scm.

; What we will get in C.
(define int-mask (- (arithmetic-shift 1 pre-scheme-integer-size) 1))

(define (lshr i n)
  (arithmetic-shift (bitwise-and i int-mask) (- 0 n)))

