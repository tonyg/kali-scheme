; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


; Primitives that directly correspond to primops.
;
; (define-primitive (id (arg-pred arg-type) ...) result-type . maybe-primop-id)
;
; Primitives that are n-ary or have other weirdness.
;
; (define-complex-primitive (id . argument-predicates)
;   eval-fn inference-rule source . maybe-expander)
;
; Primitives that have only source but not a primop.
;
; (define-semi-primitive (id . argument-predicates)
;   eval-fn inference-rule source maybe-expander)
;
; Primitives available only at load time.
;
; (define-load-time-primitive (id . argument-predicates) eval-fn)

; (really-define-primitive (id . argument-predicates)
;   eval-fn inference-rule source expander expands-in-place?)

(define-syntax really-define-primitive
  (lambda (exp r c)
    (let* ((spec (cadr exp))
	   (id (car spec))
	   (arg-predicates (cdr spec))
	   (eval (caddr exp))
	   (rest (cdddr exp))
	   (inference-rule (car rest))
	   (source (cadr rest))
	   (expander (caddr rest))
	   (expands-in-place? (cadddr rest)))
      `(let ((,(r 'predicates) ,(let recur ((preds arg-predicates))
				  (cond ((pair? preds)
					 `(cons ,(car preds)
						,(recur (cdr preds))))
					((null? preds)
					 '(quote ()))
					(else
					 preds)))))
	 (define-prescheme! ',id
	   #f				; location
	   (make-primitive ',id
			   ,(r 'predicates)
			   ,eval
			   ',source
			   ,expander
			   ,expands-in-place?
			   ,inference-rule))))))

(define-syntax define-complex-primitive
  (lambda (exp r c)
    `(really-define-primitive ,@(cdr exp) #t)))

(define-syntax define-primitive
  (lambda (exp r c)
    (let* ((id (cadr exp))
	   (args (caddr exp))
	   (result (cadddr exp))
	   (primop (if (null? (cddddr exp)) (cadr exp) (car (cddddr exp))))
	   (names (map (lambda (a b) b)
		       args
		       '(x1 x2 x3 x4 x5 x6 x7 x8 x9))))
      `(define-complex-primitive (,id . ,(map car args)) ,id
	 (lambda (args node depth return?)
	   (if (not (= (length args)
		       ,(length args)))
	       (user-error "wrong number of arguments in ~S" (schemify node)))
	   ,@(do ((i 0 (+ i 1))
		  (args args (cdr args))
		  (res '() (cons `(check-arg-type args ,i ,(cadar args) depth node)
				 res)))
		 ((null? args)
		  (reverse res)))
	   ,result)
	 (lambda ,names (,id . ,names))
	 (lambda (args type)
	   (make-primop-call-node (get-prescheme-primop ',primop) args type))))))

(define-syntax define-semi-primitive
  (lambda (exp r c)
    `(really-define-primitive ,@(cdr exp) #f #f)))

(define-syntax define-load-time-primitive
  (lambda (exp r c)
    `(define-semi-primitive ,(cadr exp)
       ,(caddr exp)
       (make-load-time-only-rule ',(caadr exp))
       #f)))

(define (make-load-time-only-rule id)
  (lambda (args node depth return?)
    (user-error "~S is only available at load time ~S" id (schemify node))))
   
;----------------------------------------------------------------
; Boolean stuff

(define-semi-primitive (not #f) not
  (lambda (args node depth return?)
    (check-arg-type args 0 type/boolean depth node)
    type/boolean)
  (lambda (x) (if x #f #t)))

(define-load-time-primitive (boolean? #f) boolean?)

(define-complex-primitive (eq? #f #f) eq?
  (lambda (args node depth return?)
    (unify! (infer-type (car args) depth)
	    (infer-type (cadr args) depth)
	    node)
    type/boolean)
  (lambda (x y) (eq? x y))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'eq?) args type)))

(define-load-time-primitive (eqv? #f) eqv?)
(define-load-time-primitive (equal? #f) equal?)

;----------------------------------------------------------------
; Characters

(define (ascii-value? n)
  (and (integer? n)
       (>= n 0)
       (< n ascii-limit)))

(define-primitive ascii->char ((ascii-value? type/integer)) type/char)
(define-primitive char->ascii ((char? type/char)) type/integer)
    
(define (char-comparison-rule args node depth return?)
  (check-arg-type args 0 type/char depth node)
  (check-arg-type args 1 type/char depth node)
  type/boolean)

(define-syntax define-char-comparison
  (lambda (exp r c)
    (let ((id (cadr exp))
	  (op (caddr exp)))
      `(define-complex-primitive (,id char? char?) ,id
	 char-comparison-rule
	 (lambda (x y) (,op x y))
	 (lambda (args type)
	   (make-primop-call-node (get-prescheme-primop ',op) args type))))))

(define-char-comparison char=? =)
(define-char-comparison char<? <)
(define-char-comparison char>? >)
(define-char-comparison char<=? <=)
(define-char-comparison char>=? >=)

; Plus lots more...

;----------------------------------------------------------------
; Data manipulation

(define (any? x) #t)

(define (positive-integer? x)
  (and (integer? x)
       (<= 0 x)))

(define (unsigned-byte? x)
  (and (positive-integer? x)
       (<= x 256)))

(define-complex-primitive (make-vector positive-integer? any?) make-vector
  (lambda (args node depth return?)
    (let ((uvar (make-uvar 'v depth)))
      (make-nonpolymorphic! uvar)
      (check-arg-type args 0 type/integer depth node)
      (check-arg-type args 1 uvar         depth node)
      (make-pointer-type uvar)))
  (lambda (size init)
    (make-vector size init))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'make-vector) args type)))

(define-load-time-primitive (vector-length vector?) vector-length)

(define-complex-primitive (vector-ref vector? positive-integer?) vector-ref
  (lambda (args node depth return?)
    (let ((elt-type (make-uvar 'v depth)))
      (check-arg-type args 0 (make-pointer-type elt-type) depth node)
      (check-arg-type args 1 type/integer depth node)
      elt-type))
  (lambda (vector index)
    (vector-ref vector index))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'vector-ref) args type)))

(define-complex-primitive (vector-set! vector? positive-integer? any?)
  vector-set!
  (lambda (args node depth return?)
    (let ((elt-type (make-uvar 'v depth)))
      (check-arg-type args 0 (make-pointer-type elt-type) depth node)
      (check-arg-type args 1 type/integer depth node)
      (check-arg-type args 2 elt-type depth node)
      type/unit))
  (lambda (vector index value)
    (vector-set! vector index value))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'vector-set!) args type)))

(define-primitive make-string ((integer? type/integer)) type/string)
(define-primitive string-length ((string? type/string)) type/integer)

(define-primitive string-ref
  ((string? type/string) (integer? type/integer))
  type/char)

(define-primitive string-set!
  ((string? type/string) (integer? type/integer) (char? type/char))
  type/unit)

(define-complex-primitive (deallocate any?) (lambda (x) (values))
  (lambda (args node depth return?)
    (let ((type (make-pointer-type (make-uvar 'p depth))))
      (check-arg-type args 0 type depth node)
      type/unit))
  (lambda (thing)
    (deallocate thing))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'deallocate) args type)))

(define-complex-primitive (null-pointer? any?) (lambda (x) #f)
  (lambda (args node depth return?)
    (let ((type (make-pointer-type (make-uvar 'p depth))))
      (check-arg-type args 0 type depth node)
      type/boolean))
  (lambda (thing)
    (null-pointer? thing))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'null-pointer?) args type)))

(define-complex-primitive (null-pointer) (lambda () #f)
  (lambda (args node depth return?)
    (make-pointer-type (make-uvar 'null depth)))
  (lambda (type)
    (null-pointer type))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'null-pointer)
			   (list (make-literal-node type))
			   type)))

;----------------------------------------------------------------
; I/O

(define-primitive current-input-port  () type/input-port  stdin)
(define-primitive current-output-port () type/output-port stdout)
(define-primitive current-error-port  () type/output-port stderr)

(define type/status type/integer)

(let ((return (make-tuple-type (list type/input-port type/status))))
  (define-primitive open-input-file ((string? type/string)) return))

(let ((return (make-tuple-type (list type/output-port type/status))))
  (define-primitive open-output-file ((string? type/string)) return))

(define-primitive close-input-port ((input-port? type/input-port)) type/status)
(define-primitive close-output-port ((output-port? type/output-port)) type/status)

(define char-return-type
  (make-tuple-type (list type/char type/boolean type/status)))

(define-primitive read-char ((input-port? type/input-port)) char-return-type)
(define-primitive peek-char ((input-port? type/input-port)) char-return-type)

(define integer-return-type
  (make-tuple-type (list type/integer type/boolean type/status)))

(define-primitive read-integer ((input-port? type/input-port)) integer-return-type)

(define-primitive write-char
  ((char? type/char) (output-port? type/output-port))
  type/status)

(define-primitive write-string
  ((string? type/string) (output-port? type/output-port))
  type/status)

(define-primitive write-integer
  ((integer? type/integer) (output-port? type/output-port))
  type/status)

(define-complex-primitive (newline output-port?) newline
  (lambda (args node depth return?)
    (check-arg-type args 0 type/output-port depth node)
    type/status)
  (lambda (out)
    (write-char #\newline out))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'write-char)
			   (cons (make-literal-node #\newline) args)
			   type)))

(define-primitive force-output ((output-port? type/output-port)) type/status)

(define-primitive error-string
  ((positive-integer? type/status))
  type/string)

;----------------------------------------------------------------

(define-complex-primitive (values . any?) values
  (lambda (args node depth return?)
    (make-tuple-type (infer-types args depth)))
  #f
  (lambda (args type)
    (let ((node (make-node values-operator (cons 'values args))))
      (node-set! node 'type type)
      node)))

(define values-operator (get-operator 'values))

; CALL-WITH-VALUES that uses closures instead of procedures.

(define (ps-call-with-values producer consumer)
  (call-with-values
   (lambda ()
     (apply-closure producer '()))
   (lambda args
     (apply-closure consumer args))))

(define-complex-primitive (call-with-values closure? closure?)
  ps-call-with-values
  (lambda (args node depth return?)
    (if (not (lambda-node? (cadr args)))
	(user-error
	 "second argument to CALL-WITH-VALUES must be a lambda node~%  ~S"
	 (schemify node)))
    (let* ((consumer-type (infer-type (cadr args) depth))
	   (arg-types (arrow-type-args consumer-type))
	   (result-type (arrow-type-result consumer-type)))
      (unify! (infer-type (car args) depth)
	      (make-arrow-type '() (make-tuple-type arg-types))
	      node)
      (if (not return?)  ; so we cause a check for illegal tuples
	  (unify! result-type (make-uvar 'temp depth) node))
      result-type))
  #f
  (lambda (args type)
    (let* ((tuple-type (arrow-type-result
 			 (maybe-follow-uvar (node-ref (car args) 'type))))
	   (node (make-node call-with-values-operator
			    (list 'call-with-values
				  (make-call-node (car args) '() tuple-type)
				  (cadr args)))))
      (node-set! node 'type type)
      node)))

(define lambda-node? (node-predicate 'lambda))

(define call-with-values-operator (get-operator 'call-with-values))

(define-primitive unspecific () type/unit)

(define-complex-primitive (error string? . integer?) error
  (lambda (args node depth return?)
    (check-arg-type args 0 type/string depth node)
    (do ((args (cdr args) (cdr args)))
	((null? args))
      (check-arg-type args 0 type/integer depth node))
    type/null)
  (lambda (error string)
    (error string))
  (lambda (args type)
    (make-primop-call-node (get-prescheme-primop 'error) args type)))

; For enumerated types that are shared with C

(define-load-time-primitive (make-external-constant symbol? symbol? string?)
  make-external-constant)

;----------------------------------------------------------------
; Utilities for making nodes

(define call-operator (get-operator 'call))
(define literal-operator (get-operator 'literal))
(define name-operator (get-operator 'name))
(define primitive-operator (get-operator 'primitive))

(define (make-call-node proc args type)
  (let ((node (make-node call-operator (cons proc args))))
    (node-set! node 'type type)
    node))

(define (make-literal-node value)
  (make-node literal-operator value))

(define (make-primop-call-node primop args type)
  (make-call-node (make-literal-node primop) args type))

(define (make-reference-node id binding)
  (let ((node (make-node name-operator id)))
    (node-set! node 'binding binding)
    node))

(define (var->name-node var)
  (make-reference-node ((structure-ref variable variable-name) var)
		       (make-binding #f var #f)))

