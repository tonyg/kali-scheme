
; Arithmetic inference rules

(define (arith-op-rule id args node depth)
  (let ((type (make-arith-op-uvar id depth)))
    (let loop ((args args))
      (if (not (null? args))
	  (begin
	    (unify! type (infer-type args depth) node)
	    (loop (cdr args)))))
    (maybe-follow-uvar type)))

(define (make-arith-op-rule id)
  (lambda (args node depth return?)
    (arith-op-rule id args node depth)))

(define (make-arith-comparison-rule id)
  (lambda (args node depth return?)
    (arith-op-rule id args node depth)
    type/boolean))

(define (integer-binop-rule args node depth return?)
  (check-arg-type args 0 type/int32 depth node)
  (check-arg-type args 1 type/int32 depth node)
  type/int32)

(define (integer-monop-rule args node depth return?)
  (check-arg-type args 0 type/int32 depth node)
  type/int32)

(define (integer-comparison-rule args node depth return?)
  (check-arg-type args 0 type/int32 depth node)
  type/boolean)

;----------------------------------------------------------------
; Arithmetic

(define-complex-primitive (+ . number?) +
  (lambda (args node depth return?)
    (if (null? args)
	type/int7u
	(arith-op-rule '+ args node depth)))
  (lambda (x y) (+ x y))
  (lambda (args type)
    (if (null? args)
	(make-arith-literal-node type 0 0.0)
	(n-ary->binary args
		       (make-literal-node (arith-primop type '+ 'fl+))
		       type))))

(define-complex-primitive (* . number?) *
  (lambda (args node depth return?)
    (if (null? args)
	type/int7u
	(arith-op-rule '* args node depth)))
  (lambda (x y) (* x y))
  (lambda (args type)
    (if (null? args)
	(make-arith-literal-node type 1 1.0)
	(n-ary->binary args
		       (make-literal-node (arith-primop type '* 'fl*))
		       type))))

(define-complex-primitive (- number? . number?)
  (lambda args
    (if (or (null? (cdr args))
	    (null? (cddr args)))
	(apply - args)
	(user-error "error while evaluating: type error ~A" (cons '- args))))
  (lambda (args node depth return?)
    (case (length args)
      ((1 2)
       (arith-op-rule '- args node depth))
      (else
       (user-error "wrong number of arguments to - in ~S" (schemify node)))))
  (lambda (x y) (- x y))
  (lambda (args type)
    (let ((primop (arith-primop type '- 'fl-)))
      (if (null? (cdr args))
	  (make-primop-call-node primop
				 (list (make-arith-literal-node type 0 0.0)
				       (car args))
				 type)
	  (make-primop-call-node primop args type)))))

(define (n-ary->binary args proc type)
  (let loop ((args args))
    (if (null? (cdr args))
	(car args)
	(loop (cons (make-call-node proc
				    (list (car args) (cadr args))
				    type)
		    (cddr args))))))

(define-syntax define-binary-primitive
  (lambda (exp r c)
    (let ((id (cadr exp)))
      (call-with-values
       (lambda ()
	 (if (null? (cdddr exp))
	     (values #f (caddr exp))
	     (values (caddr exp) (car (cdddr exp)))))
       (lambda (maybe-fl-id type-reconstruct)
	 `(define-complex-primitive (,id number? number?) ,id
	    ,type-reconstruct
	    (lambda (x y) (,id x y))
	    (lambda (args type)
	      (make-primop-call-node
	       ,(if maybe-fl-id
		    `(arith-primop type ',id ',maybe-fl-id)
		    `(get-prescheme-primop ',id))
	       args
	       type))))))))

(define-binary-primitive = fl= (make-arith-comparison-rule '=))
(define-binary-primitive < fl< (make-arith-comparison-rule '<))

(define-semi-primitive (>  number? number?) >
  (make-arith-comparison-rule '>)
  (lambda (x y) (< y x)))

(define-semi-primitive (<= number? number?) <=
  (make-arith-comparison-rule '<=)
  (lambda (x y) (not (< y x))))

(define-semi-primitive (>= number? number?) >=
  (make-arith-comparison-rule '>=)
  (lambda (x y) (not (< x y))))

(define-binary-primitive quotient  integer-binop-rule)
(define-binary-primitive remainder integer-binop-rule)
(define-binary-primitive modulo    integer-binop-rule)

(define-primitive bitwise-and ((integer? type/int32) (integer? type/int32)) type/int32)
(define-primitive bitwise-ior ((integer? type/int32) (integer? type/int32)) type/int32)
(define-primitive bitwise-xor ((integer? type/int32) (integer? type/int32)) type/int32)
(define-primitive bitwise-not ((integer? type/int32)) type/int32)

(define-primitive shift-left
  ((integer? type/int32) (integer? type/int32))
  type/int32
  ashl)

(define-primitive logical-shift-right
  ((integer? type/int32) (integer? type/int32))
  type/int32
  lshr)

(define-primitive arithmetic-shift-right
  ((integer? type/int32) (integer? type/int32))
  type/int32
  ashr)

(define-semi-primitive (abs number?) abs
  (make-arith-op-rule 'abs)
  (lambda (n) (if (< n 0) (- 0 n) n)))

(define-semi-primitive (zero? number?) zero?
  (make-arith-comparison-rule 'zero?)
  (lambda (n) (= n 0)))

(define-semi-primitive (positive? number?) positive?
  (make-arith-comparison-rule 'positive?)
  (lambda (n) (< 0 n)))

(define-semi-primitive (negative? number?) negative?
  (make-arith-comparison-rule 'negative?)
  (lambda (n) (< n 0)))

(define-semi-primitive (even? number?) even?
  integer-comparison-rule
  (lambda (n) (= 0 (remainder n 2))))

(define-semi-primitive (odd? number?) odd?
  integer-comparison-rule
  (lambda (n) (not (even? n))))
  
(define-semi-primitive (max number? . number?) max
  (make-arith-op-rule 'max)
  (lambda (x y)
    (if (< x y) y x)))
  
(define-semi-primitive (min number? . number?) min
  (make-arith-op-rule 'max)
  (lambda (x y)
    (if (< x y) x y)))

(define-semi-primitive (expt number? positive-integer?) expt
  (make-arith-op-rule 'expt)
  (lambda (x y)
    (do ((r x (* r x))
	 (y y (- y 1)))
	((<= y 0)
	 r))))

