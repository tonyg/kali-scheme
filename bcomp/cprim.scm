; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file cprim.scm.

;;;; Compiling primitive procedures and calls to them.

;(define op/closure (enum op closure))  -- defined in comp.scm
(define op/apply (enum op apply))
(define op/call-with-values (enum op call-with-values))
(define op/current-cont (enum op current-cont))
(define op/external-call (enum op external-call))
(define op/make-byte-vector-object (enum op make-byte-vector-object))
(define op/make-char-vector-object (enum op make-char-vector-object))
(define op/make-stored-object (enum op make-stored-object))
(define op/make-vector-object (enum op make-vector-object))
(define op/pop (enum op pop))
(define op/return-values (enum op return-values))
(define op/stack-ref (enum op stack-ref))
(define op/stored-object-byte-length (enum op stored-object-byte-length))
(define op/stored-object-has-type? (enum op stored-object-has-type?))
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
(define op/stored-object-ref (enum op stored-object-ref))
(define op/stored-object-set! (enum op stored-object-set!))
(define op/values (enum op values))

(define (define-compiler-primitive name nargs n-ary? compilator closed)
  (define-compilator name 'procedure
    (lambda (exp cenv depth cont)
      (let ((args (cdr exp)))
	(if (and ((if n-ary? >= =)
		  (length args) nargs)
		 compilator)
	    (compilator exp cenv depth cont)
	    (begin (if (not (memq name exceptional-primitives))
		       (warn "wrong number of arguments to primitive" exp))
		   (compile-unknown-call exp cenv depth cont))))))
  (define-closed-compilator name closed))

(define exceptional-primitives
  '(+ * - / = < make-vector make-string apply error
      read-char peek-char char-ready? write-char external-call))


; Closed-compiled versions of primitives are handled separately.

; (primitive-procedure name)  =>  a procedure

(define closed-compilators 
  (make-compilator-table (lambda (exp)
			   (generate-trap (return-cont #f)
					  "unknown primitive procedure"
					  exp))))

(define define-closed-compilator
  (let ((d (car closed-compilators)))
    (lambda (name proc)
      (d name 'procedure proc))))

(define get-closed-compilator (cdr closed-compilators))

(define-compilator 'primitive-procedure 'syntax
  (lambda (exp cenv depth cont)
    (deliver-value
     (instruction-with-template
      op/closure
      ((get-closed-compilator (get-operator (cadr exp) 'procedure)) exp)
      (cont-name cont))
     cont)))



; A simple primitive is one that takes all its arguments in order on
; the stack, except for the last, which goes in the value register.

(define (define-simple-primitive name nargs segment)
  (define-compiler-primitive name nargs #f
    (lambda (exp cenv depth cont)
      (let ((args (cdr exp)))
	(sequentially (if (null? args)
			  empty-segment
			  (push-all-but-last exp cenv depth))
		      (deliver-value segment cont))))
    (lambda (exp)
      (sequentially (instruction op/check-nargs= nargs)
		    (instruction op/pop)
		    segment
		    (instruction op/return)))))

(define (push-all-but-last exp cenv depth)
  (let recur ((args (cdr exp)) (depth depth) (i 1))
    (let ((first-code
	   (compile (car args) cenv depth (fall-through-cont exp i))))
      (if (null? (cdr args))
	  first-code
	  (sequentially first-code
			(instruction op/push)
			(recur (cdr args) (+ depth 1) (+ i 1)))))))

; Define all the primitives that correspond to opcodes in the obvious way.

(do ((opcode (enum op eq?) (+ opcode 1)))
    ((= opcode op-count))
  (let* ((arg-specs (vector-ref opcode-arg-specs opcode))
	 (name (enumerand->name opcode op))
	 (nargs (if (null? arg-specs) 0 (car arg-specs))))
    (if (and (number? nargs)
	     (not (memq name '(external-call return-from-interrupt))))
	(define-simple-primitive name nargs (instruction opcode)))))

(define-simple-primitive 'with-continuation 2
  (instruction (enum op with-continuation)))


; Define primitives for record-like stored objects (e.g. pairs).

(define (define-data-struct-primitives name predicate maker . slots)
  (let* ((type (name->enumerand name stob))
	 (def-prim (lambda (name nargs op . stuff)
		     (define-simple-primitive name nargs
		       (apply instruction (cons op stuff))))))
    (def-prim predicate 1 op/stored-object-has-type? type)
    (if (not (eq? maker 'make-symbol))  ; Symbols are made using op/intern.
	(def-prim maker (length slots)
	  op/make-stored-object (length slots) type))
    (do ((i 0 (+ i 1))
	 (slots slots (cdr slots)))
	((null? slots))
      (let ((slot (car slots)))
	(if (car slot)
	    (def-prim (car slot) 1 op/stored-object-ref type i))
	(if (cadr slot)
	    (def-prim (cadr slot) 2 op/stored-object-set! type i))))))

(for-each (lambda (stuff)
	    (apply define-data-struct-primitives stuff))
          stob-data)


; Define primitives for vector-like stored objects.

(define (define-vector-primitives name make length ref set!)
  (let* ((type (name->enumerand name stob))
	 (s-a (lambda syms
		(string->symbol (apply string-append
				       (map symbol->string syms)))))
	 (def-prim (lambda (name nargs op)
		     (define-simple-primitive name
		       nargs
		       (instruction op type)))))
    (def-prim (s-a name '?)         1 op/stored-object-has-type?)
    (def-prim (s-a 'make- name)     2 make)
    (def-prim (s-a name '- 'length) 1 length)
    (def-prim (s-a name '- 'ref)    2 ref)
    (def-prim (s-a name '- 'set!)   3 set!)))

(for-each (lambda (name)
	    (define-vector-primitives name
	      op/make-vector-object
	      op/stored-object-length
	      op/stored-object-indexed-ref
	      op/stored-object-indexed-set!))
	  '(vector record continuation extended-number template))

; There is only one of each of these so far, but there may be more later.

(for-each (lambda (name)
	    (define-vector-primitives name
	      op/make-byte-vector-object
	      op/stored-object-byte-length
	      op/stored-object-indexed-byte-ref
	      op/stored-object-indexed-byte-set!))
	  '(code-vector))

(for-each (lambda (name)
	    (define-vector-primitives name
	      op/make-char-vector-object
	      op/stored-object-byte-length
	      op/stored-object-indexed-char-ref
	      op/stored-object-indexed-char-set!))
	  '(string))


; SIGNAL-CONDITION is the same as TRAP.

(define-simple-primitive 'signal-condition 1 (instruction op/trap))


; (primitive-catch (lambda (cont) ...))

(define-compiler-primitive 'primitive-catch 1 #f
  ;; (primitive-catch (lambda (cont) ...))
  (lambda (exp cenv depth cont)
    (let ((args (cdr exp)))
      (maybe-push-continuation
       (sequentially (instruction op/current-cont)
		     (instruction op/push)
		     ;; If lambda exp, should do compile-lambda-code to
		     ;; avoid consing closure...
		     (compile (car args) cenv 1 (fall-through-cont exp 1))
		     (instruction op/call 1))
       0
       cont)))
  (lambda (exp)
    (sequentially (instruction op/check-nargs= 1)
		  (instruction op/make-env 1)  ;Seems unavoidable.
		  (instruction op/current-cont)
		  (instruction op/push)
		  (instruction op/local0 1)
		  (instruction op/call 1))))  


; APPLY wants to first spread the list, then load the procedure.
; The list argument has to be in *VAL* so that its length can be checked
; before the instruction is begun.

(define-compiler-primitive 'apply 2 #t
  (lambda (exp cenv depth cont)   ; (apply proc arg1 arg2 arg3 rest)
    (let* ((proc+args+rest (cdr exp))
	   (rest+args			     ; (rest arg3 arg2 arg1)
	    (reverse (cdr proc+args+rest)))
	   (args (cdr rest+args))	     ; (arg3 arg2 arg1)
           (args+proc+rest		     ; (arg1 arg2 arg3 proc rest)
	    (reverse (cons (car rest+args) (cons (car proc+args+rest) args)))))
      (maybe-push-continuation
         (sequentially (push-all-but-last (cons '?apply? args+proc+rest)
					  cenv 0)
		       ;; Operand is number of non-final arguments
		       (instruction op/apply (length args)))
	 depth
	 cont)))
  (lambda (exp)
    (sequentially (instruction op/check-nargs= 2)
		  (instruction op/pop)
		  (instruction op/apply 0))))


; (values value1 value2 ...)

(define-compiler-primitive 'values 0 #t
  (lambda (exp cenv depth cont)
    (let ((args (cdr exp)))
      (maybe-push-continuation (sequentially (push-arguments exp cenv 0)
					     (instruction op/return-values
							  (length args)))
			       depth
			       cont)))
  (lambda (exp) (instruction op/values)))

; (call-with-values (lambda () ...producer...)
;		    (lambda args ...consumer...))

(define-compiler-primitive 'call-with-values 2 #f
  (lambda (exp cenv depth cont)
    (let ((args (cdr exp)))
      (let ((producer (car args))
	    (consumer (cadr args)))
	(maybe-push-continuation
	 (sequentially (compile consumer cenv 0 (fall-through-cont exp 2))
		       (instruction op/push)
		       (compile-call `(,producer) cenv 1
				     (fall-through-cont exp 1))
		       (instruction op/call-with-values))
	 depth
	 cont))))
  (lambda (exp)
    ;; producer and consumer on stack
    (let ((code (sequentially (instruction op/local0 2) ;producer
			      (instruction op/call 0))))
      (sequentially (instruction op/check-nargs= 2)
		    (instruction op/make-env 2)
		    (instruction op/local0 1) ;consumer
		    (instruction op/push)
		    (instruction-with-offset&byte op/make-cont
						  (segment-size code)
						  1)
		    code
		    (instruction op/call-with-values)))))


; (error message irritant1 irritant2)
;  => (trap (cons 'error (cons message (cons irritant1 (cons irritant2 '())))))

(let ((cons-instruction
       (instruction op/make-stored-object 2 (enum stob pair))))
  (define-compiler-primitive 'error 1 #t
    (lambda (exp cenv depth cont)
      (let ((args (cdr exp)))
	(sequentially (instruction-with-literal op/literal 'error)
		      (instruction op/push)
		      (push-arguments exp cenv (+ depth 1))
		      (instruction-with-literal op/literal '())
		      (apply sequentially
			     (map (lambda (arg) cons-instruction) args))
		      cons-instruction
		      (deliver-value (instruction op/trap) cont))))
    (lambda (exp)
      (sequentially (instruction op/make-rest-list 0)
		    (instruction op/push)
		    (instruction-with-literal op/literal 'error)
		    (instruction op/push)
		    (instruction op/stack-ref 1)
		    cons-instruction
		    (instruction op/trap)
		    (instruction op/return)))))


; (external-call external-routine arg ...)

(define-compiler-primitive 'external-call 1 #t
  #f					     ;Must set *nargs*
  (lambda (exp)
    (sequentially (instruction op/check-nargs>= 1)
		  (instruction op/external-call)
		  (instruction op/return))))
