; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file cprim.scm.

;;;; Compiling primitive procedures and calls to them.

(define (define-compiler-primitive name type compilator closed)
  (define-compilator (list name type)
    (or compilator compile-unknown-call))
  (define-closed-compilator name closed))


; Closed-compiled versions of primitives are handled separately.

(define closed-compilators 
  (make-operator-table (lambda ()
			 (error "unknown primitive procedure"))))

(define (define-closed-compilator name proc)
  (operator-define! closed-compilators name proc))

; (primitive-procedure name)  =>  a procedure

(define-compilator '(primitive-procedure syntax)
  (lambda (node cenv depth cont)
    (let ((name (cadr (node-form node))))
      (deliver-value (instruction-with-template
			  op/closure
			  ((get-closed-compilator (get-operator name)))
			  (cont-name cont))
		     cont))))

(define (get-closed-compilator op)
  (operator-lookup closed-compilators op))


; --------------------
; Direct primitives.

; The simplest kind of primitive has fixed arity, corresponds to some
; single VM instruction, and takes its arguments in the usual way (all
; on the stack except the last).

(define (direct-compilator opcode)
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (sequentially (if (null? args)
			empty-segment
			(push-all-but-last args cenv depth (schemify node)))
		    (deliver-value (instruction opcode) cont)))))

(define (direct-closed-compilator opcode)
  (lambda ()
    (let ((arg-specs (vector-ref opcode-arg-specs opcode)))
      (sequentially (if (pair? arg-specs)
			(sequentially
			 (instruction op/check-nargs= (car arg-specs))
			 (instruction op/pop))
			(instruction op/check-nargs= 0))
		    (instruction opcode)
		    (instruction op/return)))))

(define (nargs->domain nargs)
  (do ((nargs nargs (- nargs 1))
       (l '() (cons value-type l)))
      ((= nargs 0) (make-some-values-type l))))


; Some primitive procedures have variable arity, but arities other
; than a single fixed one are dealt with through the exception
; handler.

(define (checked-compilator nargs compilator)
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))
      (if (= (length (cdr exp)) nargs)
	  (compilator node cenv depth cont)
	  (compile-unknown-call node cenv depth cont)))))


; Define all the primitives that correspond to opcodes in the obvious way.

(do ((opcode 0 (+ opcode 1)))
    ((= opcode op-count))
  (let ((arg-specs (vector-ref opcode-arg-specs opcode))
	(name (enumerand->name opcode op)))
    (cond ((memq name '(external-call return-from-interrupt return)))
	  ((null? arg-specs)
	   (define-compiler-primitive name (proc () value-type)
	     (direct-compilator opcode)
	     (direct-closed-compilator opcode)))
	  ((not (number? (car arg-specs))))
	  ((null? (cdr arg-specs))
	   (define-compiler-primitive name
	         (procedure-type (nargs->domain (car arg-specs))
				 value-type)    ;Return a single value.
	     (direct-compilator opcode)
	     (direct-closed-compilator opcode)))
	  (else
	   (define-compiler-primitive name
	         (procedure-type any-values-type value-type)
	     (checked-compilator (car arg-specs)
				 (direct-compilator opcode))
	     (direct-closed-compilator opcode))))))


; --------------------
; Simple primitives are executed using a fixed instruction or
; instruction sequence.

(define (define-simple-primitive name type segment)
  (let ((c (simple-compilator segment))
	(winner? (fixed-arity-procedure-type? type)))
    (let ((nargs (if winner?
		     (length (procedure-type-argument-types type))
		     (case name
		       ((make-vector make-string) 2)
		       (else (error "n-ary simple primitive?!" name type))))))
      (define-compiler-primitive name type
	(checked-compilator nargs c)
	(simple-closed-compilator nargs segment)))))

(define (simple-compilator segment)
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (sequentially (if (null? args)
			empty-segment
			(push-all-but-last args cenv depth (schemify node)))
		    (deliver-value segment cont)))))

(define (simple-closed-compilator nargs segment)
  (lambda ()
    (sequentially (instruction op/check-nargs= nargs)
		  (instruction op/pop)
		  segment
		  (instruction op/return))))


; Define primitives for record-like stored objects (e.g. pairs).

(define (define-data-struct-primitives name predicate maker . slots)
  (let* ((type-byte (name->enumerand name stob))
	 (def-prim (lambda (name type op . stuff)
		     (define-simple-primitive name type
		       (apply instruction (cons op stuff))))))
    (def-prim predicate (proc (value-type) boolean-type)
			op/stored-object-has-type? type-byte)
    (if (not (eq? maker 'make-symbol))  ; Symbols are made using op/intern.
	(def-prim maker
	  (procedure-type (nargs->domain (length slots)) name)
	  op/make-stored-object
	  (length slots)
	  type-byte))
    (do ((i 0 (+ i 1))
	 (slots slots (cdr slots)))
	((null? slots))
      (let ((slot (car slots)))
	(if (car slot)
	    (def-prim (car slot)
	      (proc (name) value-type)
	      op/stored-object-ref type-byte i))
	(if (cadr slot)
	    (def-prim (cadr slot)
	      (proc (name value-type) unspecific-type)
	      op/stored-object-set! type-byte i))))))

(define op/stored-object-has-type? (enum op stored-object-has-type?))
(define op/make-stored-object (enum op make-stored-object))
(define op/stored-object-ref (enum op stored-object-ref))
(define op/stored-object-set! (enum op stored-object-set!))

(define op/make-byte-vector-object (enum op make-byte-vector-object))
(define op/make-char-vector-object (enum op make-char-vector-object))
(define op/make-vector-object (enum op make-vector-object))

(for-each (lambda (stuff)
	    (apply define-data-struct-primitives stuff))
          stob-data)


; Define primitives for vector-like stored objects.

(define (define-vector-primitives name element-type make length ref set!)
  (let* ((type-byte (name->enumerand name stob))
	 (s-a (lambda syms
		(string->symbol (apply string-append
				       (map symbol->string syms)))))
	 (def-prim (lambda (name type op)
		     (define-simple-primitive name type
		       (instruction op type-byte)))))
    (def-prim (s-a name '?)
      (proc (value-type) boolean-type)
      op/stored-object-has-type?)
    (def-prim (s-a 'make- name)
      (if (memq name '(vector string))
	  (procedure-type any-values-type name)
	  (proc (number-type value-type) name))
      make)
    (def-prim (s-a name '- 'length)
      (proc (name) number-type)
      length)
    (def-prim (s-a name '- 'ref)
      (proc (name number-type) element-type)
      ref)
    (def-prim (s-a name '- 'set!)
      (proc (name number-type element-type) 'unspecific)
      set!)))

(define op/stored-object-byte-length (enum op stored-object-byte-length))
(define op/stored-object-indexed-byte-ref
  (enum op stored-object-indexed-byte-ref))
(define op/stored-object-indexed-byte-set!
  (enum op stored-object-indexed-byte-set!))
(define op/stored-object-indexed-char-ref
  (enum op stored-object-indexed-char-ref))
(define op/stored-object-indexed-char-set!
  (enum op stored-object-indexed-char-set!))
(define op/stored-object-indexed-ref (enum op stored-object-indexed-ref))
(define op/stored-object-indexed-set! (enum op stored-object-indexed-set!))
(define op/stored-object-length (enum op stored-object-length))

(for-each (lambda (name)
	    (define-vector-primitives name value-type
	      op/make-vector-object
	      op/stored-object-length
	      op/stored-object-indexed-ref
	      op/stored-object-indexed-set!))
	  '(vector record continuation extended-number template))

; There is only one of each of these so far, but there may be more later.

(for-each (lambda (name)
	    (define-vector-primitives name number-type
	      op/make-byte-vector-object
	      op/stored-object-byte-length
	      op/stored-object-indexed-byte-ref
	      op/stored-object-indexed-byte-set!))
	  '(code-vector))

(for-each (lambda (name)
	    (define-vector-primitives name char-type
	      op/make-char-vector-object
	      op/stored-object-byte-length
	      op/stored-object-indexed-char-ref
	      op/stored-object-indexed-char-set!))
	  '(string))


; SIGNAL-CONDITION is the same as TRAP.

(define-simple-primitive 'signal-condition (proc (pair-type) value-type)
  (instruction op/trap))


; (primitive-catch (lambda (cont) ...))

(define-compiler-primitive 'primitive-catch #f
  ;; (primitive-catch (lambda (cont) ...))
  (lambda (node cenv depth cont)
    (let* ((exp (node-form node))
	   (args (cdr exp)))
      (maybe-push-continuation
       (sequentially (instruction op/current-cont)
		     (instruction op/push)
		     ;; If lambda exp, should do compile-lambda-code to
		     ;; avoid consing closure...
		     (compile (car args) cenv 1
			      (fall-through-cont (schemify node) 1))
		     (instruction op/call 1))
       0
       cont)))
  (lambda ()
    (sequentially (instruction op/check-nargs= 1)
		  (instruction op/make-env 1)  ;Seems unavoidable.
		  (instruction op/current-cont)
		  (instruction op/push)
		  (instruction op/local0 1)
		  (instruction op/call 1))))  

; (call-with-values (lambda () ...producer...)
;		    (lambda args ...consumer...))

(define-compiler-primitive 'call-with-values #f
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node)))
	  (exp (schemify node)))
      (let ((producer (car args))
	    (consumer (cadr args)))
	(maybe-push-continuation
	 (sequentially (compile consumer cenv 0 (fall-through-cont exp 2))
		       (instruction op/push)
		       (maybe-push-continuation     ; nothing maybe about it
			(compile-call (classify `(,producer) cenv)
				      cenv 0
				      (return-cont #f))
			1
			(fall-through-cont #f 0))
		       ;; Was:
		       ;; (compile-call (classify `(,producer) cenv)
		       ;; 	     cenv 1
		       ;; 	     (fall-through-cont exp 1))
		       (instruction op/call-with-values))
	 depth
	 cont))))
  (lambda ()
    ;; producer and consumer on stack
    (let ((label (make-label)))
      (sequentially (instruction op/check-nargs= 2)
		    (instruction op/make-env 2)
		    (instruction op/local0 1) ;consumer
		    (instruction op/push)
		    (instruction-using-label op/make-cont label 1)
		    (instruction op/local0 2) ;producer
		    (instruction op/call 0)
		    (attach-label label
				  (instruction op/call-with-values))))))


; --------------------
; Variable-arity primitives

(define (define-n-ary-compiler-primitive name result-type min-nargs
					 compilator closed)
  (define-compiler-primitive name
        (if result-type
	    (procedure-type any-values-type result-type)
	    #f)
    (if compilator
	(n-ary-primitive-compilator name min-nargs compilator)
	compile-unknown-call)
    closed))

(define (n-ary-primitive-compilator name min-nargs compilator)
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))
      (if (>= (length (cdr exp)) min-nargs)
	  (compilator node cenv depth cont)
	  (begin (warn "too few arguments to primitive"
		       (schemify node))
		 (compile-unknown-call node cenv depth cont))))))


; APPLY wants to first spread the list, then load the procedure.
; The list argument has to be in *VAL* so that its length can be checked
; before the instruction is begun.

(define-n-ary-compiler-primitive 'apply #f 2
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))	; (apply proc arg1 arg2 arg3 rest)
      (let* ((proc+args+rest (cdr exp))
	     (rest+args			; (rest arg3 arg2 arg1)
	      (reverse (cdr proc+args+rest)))
	     (args (cdr rest+args))	; (arg3 arg2 arg1)
	     (args+proc+rest		; (arg1 arg2 arg3 proc rest)
	      (reverse (cons (car rest+args)
			     (cons (car proc+args+rest) args)))))
	(maybe-push-continuation
         (sequentially (push-all-but-last args+proc+rest cenv 0 #f)
		       ;; Operand is number of non-final arguments
		       (instruction op/apply (length args)))
	 depth
	 cont))))
  (lambda ()
    (sequentially (instruction op/check-nargs= 2)
		  (instruction op/pop)
		  (instruction op/apply 0))))


; (values value1 value2 ...)

(define-n-ary-compiler-primitive 'values #f 0
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (maybe-push-continuation (sequentially (push-arguments node cenv 0)
					     (instruction op/return-values
							  (length args)))
			       depth
			       cont)))
  (lambda () (instruction op/values)))


; (error message irritant1 irritant2)
;  => (trap (cons 'error (cons message (cons irritant1 (cons irritant2 '())))))

(let ((cons-instruction
       (instruction op/make-stored-object 2 (enum stob pair))))

  (define-n-ary-compiler-primitive 'error any-values-type 1
    (lambda (node cenv depth cont)
      (let ((exp (node-form node)))
	(let ((args (cdr exp)))
	  (sequentially (instruction-with-literal op/literal 'error)
			(instruction op/push)
			(push-arguments node cenv (+ depth 1))
			(instruction-with-literal op/literal '())
			(apply sequentially
			       (map (lambda (arg) cons-instruction) args))
			cons-instruction
			(deliver-value (instruction op/trap) cont)))))
    (lambda ()
      (sequentially (instruction op/make-rest-list 0)
		    (instruction op/push)
		    (instruction-with-literal op/literal 'error)
		    (instruction op/push)
		    (instruction op/stack-ref 1)
		    cons-instruction
		    (instruction op/trap)
		    (instruction op/return)))))


; (external-call external-routine arg ...)

(define-n-ary-compiler-primitive 'external-call value-type 1
  #f					     ;Must set *nargs*
  (lambda ()
    (sequentially (instruction op/check-nargs>= 1)
		  (instruction op/external-call)
		  (instruction op/return))))


; --------------------
; Utility

(define (push-all-but-last args cenv depth exp)
  (let recur ((args args) (depth depth) (i 1))
    (let ((first-code
	   (compile (car args) cenv depth (fall-through-cont exp i))))
      (if (null? (cdr args))
	  first-code
	  (sequentially first-code
			(instruction op/push)
			(recur (cdr args) (+ depth 1) (+ i 1)))))))

;(define op/closure (enum op closure))  -- defined in comp.scm
(define op/apply (enum op apply))
(define op/call-with-values (enum op call-with-values))
(define op/current-cont (enum op current-cont))
(define op/external-call (enum op external-call))
(define op/pop (enum op pop))
(define op/return-values (enum op return-values))
(define op/stack-ref (enum op stack-ref))
(define op/values (enum op values))
