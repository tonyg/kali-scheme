; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This is file cprim.scm.

;;;; Compiling primitive procedures and calls to them.

(define (define-compiler-primitive name type compilator closed)
  (define-compilator name type
    (or compilator compile-unknown-call))
  (define-closed-compilator name closed))


; Closed-compiled versions of primitives are handled separately.

(define closed-compilators 
  (make-operator-table (lambda ()
                         (error "unknown primitive procedure"))))

(define (define-closed-compilator name proc)
  (operator-define! closed-compilators name #f proc))

; (primitive-procedure name)  =>  a procedure

(define-compilator 'primitive-procedure syntax-type
  (lambda (node cenv depth cont)
    (let ((name (cadr (node-form node))))
      (deliver-value (instruction-with-template
                          (enum op closure)
                          ((get-closed-compilator (get-operator name)))
                          (cont-name cont))
                     cont))))

(define (get-closed-compilator op)
  (operator-lookup closed-compilators op))


; --------------------
; Direct primitives.

; The simplest kind of primitive has fixed arity, corresponds to some
; single VM instruction, and takes its arguments in the usual way (all
; on the stack).

(define (direct-compilator type opcode)
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (sequentially (if (null? args)
                        empty-segment
                        (push-all-but-last args cenv depth node))
                    (deliver-value (instruction opcode) cont)))))

(define (direct-closed-compilator opcode)
  (lambda ()
    (let ((arg-specs (vector-ref opcode-arg-specs opcode)))
      (sequentially (if (pair? arg-specs)
                        (sequentially
                         (instruction (enum op protocol) (car arg-specs))
                         (instruction (enum op pop)))
                        (instruction (enum op protocol) 0))
                    (instruction opcode)
                    (instruction (enum op return))))))

(define (nargs->domain nargs)
  (do ((nargs nargs (- nargs 1))
       (l '() (cons value-type l)))
      ((= nargs 0) (make-some-values-type l))))

; Define all the primitives that correspond to opcodes in the obvious way.

(do ((opcode 0 (+ opcode 1)))
    ((= opcode op-count))
  (let ((arg-specs (vector-ref opcode-arg-specs opcode))
        (name (enumerand->name opcode op)))
    (cond ((memq name '(external-call return-from-interrupt return)))
          ((null? arg-specs)
           (let ((type (proc () value-type)))
             (define-compiler-primitive name type
               (direct-compilator type opcode)
               (direct-closed-compilator opcode))))
          ((not (number? (car arg-specs))))
	  ((memq name '(+ * - / = < > <= >=
			bitwise-ior bitwise-xor bitwise-and
			make-string closed-apply)))
          (else
           (let ((type (procedure-type (nargs->domain (car arg-specs))
                                       (if (eq? name 'with-continuation)
                                           any-values-type
                                           ;; Return a single value.
                                           value-type)
                                       ;; nonrestrictive - domain might be
                                       ;; specialized later
                                       #t)))
             (define-compiler-primitive name type
               (direct-compilator type opcode)
               (direct-closed-compilator opcode)))))))

; --------------------
; Simple primitives are executed using a fixed instruction or
; instruction sequence.

(define (define-simple-primitive name type segment)
  (let ((winner? (fixed-arity-procedure-type? type)))
    (let ((nargs (if winner?
                     (procedure-type-arity type)
                     (error "n-ary simple primitive?!" name type))))
      (define-compiler-primitive name type
        (simple-compilator segment)
        (simple-closed-compilator nargs segment)))))

(define (simple-compilator segment)
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (sequentially (if (null? args)
                        empty-segment
                        (push-all-but-last args cenv depth node))
                    (deliver-value segment cont)))))

(define (simple-closed-compilator nargs segment)
  (lambda ()
    (sequentially (instruction (enum op protocol) nargs)
                  (instruction (enum op pop))
                  segment
                  (instruction (enum op return)))))

(define (symbol-append . syms)
  (string->symbol (apply string-append
                         (map symbol->string syms))))

(define (define-stob-predicate name stob-name)
  (define-simple-primitive name
    (proc (value-type) boolean-type)
    (instruction (enum op stored-object-has-type?)
                 (name->enumerand stob-name stob))))

(define-stob-predicate 'code-vector? 'code-vector)
(define-stob-predicate 'string? 'string)

; Define primitives for record-like stored objects (e.g. pairs).

(define (define-data-struct-primitives name predicate maker . slots)
  (let* ((def-prim (lambda (name type op . stuff)
                     (define-simple-primitive name type
                       (apply instruction (cons op stuff)))))
         (type-byte (name->enumerand name stob))
         (type (sexp->type (symbol-append ': name) #t)))
    (define-stob-predicate predicate name)
    (if maker
        (def-prim maker
          (procedure-type (nargs->domain (length slots)) type #t)
          (enum op make-stored-object)
          (length slots)
          type-byte))
    (do ((i 0 (+ i 1))
         (slots slots (cdr slots)))
        ((null? slots))
      (let ((slot (car slots)))
        (if (car slot)
            (def-prim (car slot)
              (proc (type) value-type)
              (enum op stored-object-ref) type-byte i))
        (if (not (null? (cdr slot)))
            (def-prim (cadr slot)
              (proc (type value-type) unspecific-type)
              (enum op stored-object-set!) type-byte i))))))

(for-each (lambda (stuff)
            (apply define-data-struct-primitives stuff))
          stob-data)

; For flat environments

(let ((:value (sexp->type ':value #t))
      (:vector (sexp->type ':vector #t)))
  (define-simple-primitive 'make-cell
    (proc (:value) :vector)
    (instruction (enum op make-stored-object) 1 (enum stob vector)))
  (define-simple-primitive 'cell-ref
    (proc (:vector) :value)
    (instruction (enum op stored-object-ref) (enum stob vector) 0))
  (define-simple-primitive 'cell-set!
    (proc (:vector :value) unspecific-type)
    (instruction (enum op stored-object-set!) (enum stob vector) 0)))

; Define primitives for vector-like stored objects.

(define (define-vector-primitives name element-type make length ref set!)
  (let* ((type-byte (name->enumerand name stob))
         (def-prim (lambda (name type op)
                     (define-simple-primitive name type
                       (instruction op type-byte))))
         (type (sexp->type (symbol-append ': name) #t)))
    (define-stob-predicate (symbol-append name '?) name)
    (if (not (eq? name 'vector))  ; 2nd arg to make-vector is optional
	(def-prim (symbol-append 'make- name)
	  (proc (exact-integer-type element-type) type)
	  make))
    (def-prim (symbol-append name '- 'length)
      (proc (type) exact-integer-type)
      length)
    (def-prim (symbol-append name '- 'ref)
      (proc (type exact-integer-type) element-type)
      ref)
    (def-prim (symbol-append name '- 'set!)
      (proc (type exact-integer-type element-type) unspecific-type)
      set!)))

(for-each (lambda (name)
            (define-vector-primitives name value-type
              (enum op make-vector-object)
              (enum op stored-object-length)
              (enum op stored-object-indexed-ref)
              (enum op stored-object-indexed-set!)))
          '(vector record continuation extended-number template))

; SIGNAL-CONDITION is the same as TRAP.

(define-simple-primitive 'signal-condition (proc (pair-type) unspecific-type)
  (instruction (enum op trap)))


; (primitive-catch (lambda (cont) ...))

(define-compiler-primitive 'primitive-catch #f
  ;; (primitive-catch (lambda (cont) ...))
  (lambda (node cenv depth cont)
    (let* ((exp (node-form node))
           (args (cdr exp)))
      (maybe-push-continuation
       (sequentially (instruction (enum op current-cont))
                     (instruction (enum op push))
                     ;; If lambda exp, should do compile-lambda-code to
                     ;; avoid consing closure...
                     (compile (car args) cenv 1 (fall-through-cont node 1))
                     (instruction (enum op call) 1))
       0
       cont)))
  (lambda ()
    (sequentially (instruction (enum op protocol) 1)
                  (instruction (enum op make-env)   ;Seems unavoidable.
			       (high-byte 1)
			       (low-byte 1))
                  (instruction (enum op current-cont))
                  (instruction (enum op push))
                  (instruction (enum op local0) 1)
                  (instruction (enum op call) 1))))  

; (call-with-values (lambda () ...producer...)
;                   (lambda args ...consumer...))

(define-compiler-primitive 'call-with-values #f
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (let ((producer (car args))
            (consumer (cadr args)))
        (maybe-push-continuation
         (sequentially (compile consumer cenv 0 (fall-through-cont node 2))
                       (instruction (enum op push))
                       (maybe-push-continuation     ; nothing maybe about it
                        (compile-call (classify `(,producer) cenv)
                                      cenv 0
                                      (return-cont #f))
                        1
                        (fall-through-cont #f 0))
                       ;; Was:
                       ;; (compile-call (classify `(,producer) cenv)
                       ;;            cenv 1
                       ;;            (fall-through-cont node 1))
                       (instruction (enum op call-with-values)))
         depth
         cont))))
  (lambda ()
    ;; producer and consumer on stack
    (let ((label (make-label)))
      (sequentially (instruction (enum op protocol) 2)
                    (instruction (enum op make-env)
				 (high-byte 2)
				 (low-byte 2))
                    (instruction (enum op local0) 1) ;consumer
                    (instruction (enum op push))
                    (instruction-using-label (enum op make-cont) label 1)
                    (instruction (enum op local0) 2) ;producer
                    (instruction (enum op call) 0)
                    (attach-label label
                                  (instruction (enum op call-with-values)))))))


; --------------------
; Variable-arity primitives

(define (define-n-ary-compiler-primitive name result-type min-nargs
                                         compilator closed)
  (define-compiler-primitive name
        (if result-type
            (procedure-type any-arguments-type result-type #f)
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
                       (schemify node cenv))
                 (compile-unknown-call node cenv depth cont))))))


; APPLY wants the arguments on the stack, with the final list on top, and the
; procedure in *VAL*.

(define-n-ary-compiler-primitive 'apply #f 2
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))       ; (apply proc arg1 arg2 arg3 rest)
      (let* ((proc+args+rest (cdr exp))
             (rest+args                 ; (rest arg3 arg2 arg1)
              (reverse (cdr proc+args+rest)))
             (args+rest+proc            ; (arg1 arg2 arg3 rest proc)
              (reverse (cons (car proc+args+rest) rest+args)))
	     (stack-nargs (length (cdr rest+args))))
        (maybe-push-continuation
         (sequentially (push-all-but-last args+rest+proc cenv 0 #f)
                       ;; Operand is number of non-final arguments
                       (instruction (enum op apply)
				    (high-byte stack-nargs)
				    (low-byte stack-nargs)))
         depth
         cont))))
  (lambda ()
    (sequentially (instruction (enum op protocol) args+nargs-protocol 2)
                  (instruction (enum op closed-apply)))))

; (values value1 value2 ...)

(define-n-ary-compiler-primitive 'values #f 0
  (lambda (node cenv depth cont)
    (let* ((args (cdr (node-form node)))
	   (nargs (length args)))
      (if (= 1 nargs)
	  (compile (car args) cenv depth cont)		;+++
	  (maybe-push-continuation (sequentially (push-arguments node cenv 0)
						 (instruction (enum op values)
							      (high-byte nargs)
							      (low-byte nargs)))
				   depth
				   cont))))
  (lambda ()
    (sequentially (instruction (enum op protocol) args+nargs-protocol 0)
		  (instruction (enum op closed-values)))))


; (error message irritant1 irritant2)
;  => (trap (cons 'error (cons message (cons irritant1 (cons irritant2 '())))))

(let ((cons-instruction
       (instruction (enum op make-stored-object) 2 (enum stob pair))))

  (define-n-ary-compiler-primitive 'error error-type 1
    (lambda (node cenv depth cont)
      (let ((exp (node-form node)))
        (let ((args (cdr exp)))
          (sequentially (instruction-with-literal (enum op literal) 'error)
                        (instruction (enum op push))
                        (push-arguments node cenv (+ depth 1))
                        (instruction-with-literal (enum op literal) '())
                        (apply sequentially
                               (map (lambda (arg) cons-instruction) args))
                        cons-instruction
                        (deliver-value (instruction (enum op trap)) cont)))))
    (lambda ()
      ; stack at start is: irritants message
      (sequentially (instruction (enum op protocol)
				 two-byte-nargs+list-protocol
				 0		; (high-byte 1) 
				 1)		; (low-byte 1)
		    (instruction (enum op pop))   ; list into *val*
		    cons-instruction
                    (instruction (enum op push))
                    (instruction-with-literal (enum op literal) 'error)
                    (instruction (enum op push))
                    (instruction (enum op stack-ref) 1)
                    cons-instruction
                    (instruction (enum op trap))
                    (instruction (enum op return))))))


; (external-call external-routine arg ...)

(define-n-ary-compiler-primitive 'external-call value-type 1
  #f                                         ;Could be done
  (lambda ()
    (sequentially (instruction (enum op protocol) args+nargs-protocol 1)
                  (instruction (enum op external-call))
                  (instruction (enum op return)))))

(let ((n-ary-constructor
        (lambda (name type type-byte)
	  (define-n-ary-compiler-primitive name type 0
	    (lambda (node cenv depth cont)
	      (let ((args (cdr (node-form node))))
		(sequentially (if (null? args)
				  empty-segment
				  (push-all-but-last args cenv depth node))
			      (deliver-value
			       (instruction (enum op make-stored-object)
					    (length args)
					    type-byte)
			       cont))))
	    (lambda ()
	      (sequentially
	       (instruction (enum op protocol) args+nargs-protocol 0)
	       (instruction (enum op closed-make-stored-object) type-byte)
	       (instruction (enum op return))))))))
  (n-ary-constructor 'vector vector-type (enum stob vector))
  (n-ary-constructor 'record #f (enum stob record)))

; READ-CHAR, PEEK-CHAR and WRITE-CHAR

(let ((define-char-io
	(lambda (id opcode type)
	  (define-compiler-primitive id
	    type
	    (lambda (node cenv depth cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node cenv depth cont)
		  (let ((args (cdr (node-form node))))
		    (if (null? args)
			(deliver-value (instruction opcode 1) cont)
			(sequentially
			 (push-all-but-last args cenv depth node)
			 (deliver-value (instruction opcode 0) cont))))))
	    (lambda ()
	      (make-dispatch-protocol
	        ; Zero arguments
 	        (sequentially
		  (instruction opcode 1)
		  (instruction (enum op return)))
		; One argument
		(sequentially
		  (instruction (enum op pop))
		  (instruction opcode 0)
		  (instruction (enum op return)))
		empty-segment
		empty-segment))))))
  (define-char-io 'read-char
    (enum op read-char)
    (proc (&opt value-type) value-type))
  (define-char-io 'peek-char
    (enum op peek-char)
    (proc (&opt value-type) value-type)))

(let ((define-char-io
	(lambda (id opcode type)
	  (define-compiler-primitive id
	    type
	    (lambda (node cenv depth cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node cenv depth cont)
		  (let ((args (cdr (node-form node))))
		    (sequentially
		     (push-all-but-last args cenv depth node)
		     (if (null? (cdr args))
			 (deliver-value (instruction opcode 1) cont)
			 (sequentially
			  (deliver-value (instruction opcode 0) cont)))))))
	    (lambda ()
	      (make-dispatch-protocol
	        empty-segment
	        ; One argument
	        (sequentially
		  (instruction (enum op pop))
		  (instruction opcode 1)
		  (instruction (enum op return)))
		; Two arguments
	        (sequentially
		  (instruction (enum op pop))
		  (instruction opcode 0)
		  (instruction (enum op return)))
		empty-segment))))))
  (define-char-io 'write-char
    (enum op write-char)
    (proc (char-type &opt value-type) unspecific-type)))

; Timings in 0.47 to figure out how to handle the optional ports.
; 
; reading 10**6 characters (no buffer underflow)
; empty loop    time:  3.44 seconds
; read-char     time:  3.68 seconds    ; special primitive, exceptions
; xread-char    time:  9.04 seconds    ; special primitive, no exceptions
; xxread-char   time: 14.05 seconds    ; no special primitive
; Currently, looping through a 10**6 character file takes 1.51 seconds or
; 2.50 seconds if you count the number of characters.

;----------------
; Variable-arity arithmetic primitives.

; +, *, bitwise-... take any number of arguments.

(let ((define+*
	(lambda (id opcode identity type)
	  (define-compiler-primitive id
	    (proc (&rest type) type)
	    (lambda (node cenv depth cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node cenv depth cont)
		  (let ((args (cdr (node-form node))))
		    (cond ((null? args)
			   (deliver-value
			    (instruction-with-literal (enum op literal)
						      identity)
			    cont))
			  ((null? (cdr args))
			   (call-on-arg-and-id opcode identity (car args)
					       node cenv depth cont))
			  (else
			   (call-on-args opcode args node cenv depth cont))))))
	    (lambda ()
	      (make-dispatch-protocol
	        ; No arguments
	        (sequentially
		  (instruction-with-literal (enum op literal) identity)
		  (instruction (enum op return)))
		; One argument
		(sequentially
		  (instruction-with-literal (enum op literal) identity)
		  (instruction opcode)
		  (instruction (enum op return)))
		; Two arguments
		(sequentially
		  (instruction (enum op pop))
		  (instruction opcode)
		  (instruction (enum op return)))
		; More than two arguments
		(sequentially
		  (instruction (enum op pop))	; pop off nargs
		  (instruction (enum op binary-reduce1))
		  (instruction opcode)
		  (instruction (enum op binary-reduce2))
		  (instruction (enum op return)))))))))
  (define+* '+ (enum op +) 0 number-type)
  (define+* '* (enum op *) 1 number-type)
  (define+* 'bitwise-ior (enum op bitwise-ior) 0 exact-integer-type)
  (define+* 'bitwise-xor (enum op bitwise-xor) 0 exact-integer-type)
  (define+* 'bitwise-and (enum op bitwise-and) -1 exact-integer-type))

; = and < and so forth take two or more arguments.

(let ((define=<
	(lambda (id opcode)
	  (define-compiler-primitive id
	    (proc (real-type real-type &rest real-type) boolean-type)
	    (lambda (node cenv depth cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node cenv depth cont)
		  (let ((args (cdr (node-form node))))
		    (if (= (length args) 2)
			(call-on-args opcode args node cenv depth cont)
			(compile-unknown-call node cenv depth cont)))))
	    (lambda ()
	      (make-dispatch-protocol
	        empty-segment
		empty-segment
		; Two arguments
		(sequentially 
		  (instruction (enum op pop))           ; get first argument
		  (instruction opcode)
		  (instruction (enum op return)))
		; More than two arguments
		(sequentially
		  (instruction (enum op pop))
		  (instruction (enum op binary-reduce1))
		  (instruction opcode)
		  (instruction (enum op binary-comparison-reduce2))
		  (instruction (enum op return)))))))))
  (define=< '= (enum op =))
  (define=< '< (enum op <))
  (define=< '> (enum op >))
  (define=< '<= (enum op <=))
  (define=< '>= (enum op >=)))

; Returns code to apply OPCODE to IDENTITY and ARGUMENT.

(define (call-on-arg-and-id opcode identity argument node cenv depth cont)
  (sequentially (instruction-with-literal (enum op literal) identity)
		(instruction (enum op push))
		(compile argument cenv (+ depth 1) (fall-through-cont node 1))
		(deliver-value (instruction opcode) cont)))
  
; Returns code to redue ARGS using OPCODE.

(define (call-on-args opcode args node cenv depth cont)
  (let ((do-arg (lambda (arg index)
		  (compile arg
			   cenv
			   (if (= index 1) depth (+ depth 1))
			   (fall-through-cont node index)))))
    (let loop ((args (cdr args)) (i 2) (code (do-arg (car args) 1)))
      (if (null? args)
	  (deliver-value code cont)
	  (loop (cdr args)
		(+ i 1)
		(sequentially code
			      (instruction (enum op push))
			      (do-arg (car args) i)
			      (instruction opcode)))))))

(define op/unspecific (get-operator 'unspecific))
(define op/literal (get-operator 'literal))

; -, and / take one or two arguments.

(let ((define-one-or-two
	(lambda (id opcode default-arg)
	  (define-compiler-primitive id
            (proc (number-type &opt number-type) number-type)
	    (lambda (node cenv depth cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node cenv depth cont)
		  (let* ((args (cdr (node-form node)))
			 (args (if (null? (cdr args))
				   (list (make-node op/literal default-arg)
					 (car args))
				   args)))
		    (sequentially
		     (push-all-but-last args cenv depth node)
		     (deliver-value (instruction opcode) cont)))))
	    (lambda ()
	      (make-dispatch-protocol
	        empty-segment
		; One argument
 	        (sequentially
		  (instruction-with-literal (enum op literal) default-arg)
		  (instruction (enum op push))
		  (instruction (enum op stack-ref) 1)
		  (instruction opcode)
		  (instruction (enum op return)))
		; Two arguments
		(sequentially
		  (instruction (enum op pop))
		  (instruction opcode)
		  (instruction (enum op return)))
		empty-segment))))))
  (define-one-or-two '- (enum op -) 0)
  (define-one-or-two '/ (enum op /) 1))

; make-vector and make-string take one or two arguments.

(let ((define-one-or-two
	(lambda (id op-segment default-arg default-arg-segment type)
	  (define-compiler-primitive id
	    type
	    (lambda (node cenv depth cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node cenv depth cont)
		  (let* ((args (cdr (node-form node)))
			 (args (if (null? (cdr args))
				   (list (car args) default-arg)
				   args)))
		    (sequentially
		     (push-all-but-last args cenv depth node)
		     (deliver-value op-segment cont)))))
	    (lambda ()
	      (make-dispatch-protocol
	        empty-segment
		; One argument
 	        (sequentially
		  default-arg-segment
		  op-segment
		  (instruction (enum op return)))
		; Two arguments
 	        (sequentially
		  (instruction (enum op pop))
		  op-segment
		  (instruction (enum op return)))
		empty-segment))))))
  (define-one-or-two 'make-vector
    (instruction (enum op make-vector-object) (enum stob vector))
    (make-node op/unspecific '(unspecific))
    (instruction (enum op unspecific))
    (proc (number-type &opt value-type) vector-type))
  (define-one-or-two 'make-string
    (instruction (enum op make-string))
    (make-node op/literal #\?)
    (instruction-with-literal (enum op literal) #\?)
    (proc (number-type &opt char-type) string-type)))

; --------------------
; Utilities

(define (push-all-but-last args cenv depth source-info)
  (let recur ((args args) (depth depth) (i 1))
    (let ((first-code
           (compile (car args) cenv depth (fall-through-cont source-info i))))
      (if (null? (cdr args))
          first-code
          (sequentially first-code
                        (instruction (enum op push))
                        (recur (cdr args) (+ depth 1) (+ i 1)))))))

; Building primitives that use the computed-goto provided by the
; protocol dispatcher.

(define dispatch-protocol-size
  (segment-size (instruction (enum op protocol) nary-dispatch-protocol
			     0 0 0 0)))

(define (make-dispatch-protocol zero-args one-arg two-args three-plus-args)
  (sequentially
    (instruction (enum op protocol) nary-dispatch-protocol
		 (if (= 0 (segment-size zero-args))
		     0
		     dispatch-protocol-size)
		 (if (= 0 (segment-size one-arg))
		     0
		     (+ dispatch-protocol-size
			(segment-size zero-args)))
		 (if (= 0 (segment-size two-args))
		     0
		     (+ dispatch-protocol-size
			(segment-size zero-args)
			(segment-size one-arg)))
		 (if (= 0 (segment-size three-plus-args))
		     0
		     (+ dispatch-protocol-size
			(segment-size zero-args)
			(segment-size one-arg)
			(segment-size two-args))))
    zero-args
    one-arg
    two-args
    three-plus-args))

