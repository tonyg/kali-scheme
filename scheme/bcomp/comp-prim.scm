; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Compiling primitive procedures and calls to them.

; (primitive-procedure name)  =>  a procedure

(define-compilator 'primitive-procedure syntax-type
  (lambda (node depth frame cont)
    (let ((name (cadr (node-form node))))
      (deliver-value
        (sequentially
	  (stack-indirect-instruction
	    (template-offset frame depth)
	    (literal->index frame (primop-closed-template name)))
	  (instruction (enum op push))
	  (instruction (enum op false))		; no environment
	  (instruction (enum op make-stored-object) 2 (enum stob closure))
	  (instruction (enum op make-immutable!)))
	cont))))

(define (primop-closed-template name)
  (let ((data (primop-closed (get-primop name))))
    (receive (maybe-nargs proc)
	(if (pair? data)
	    (values (car data) (cdr data))
	    (values #f data))
      (let ((frame (make-frame #f	; no parent frame
			       name	; name of primop
			       (or maybe-nargs 0)
					; nargs (needed if template used)
			       #f	; no env
			       maybe-nargs))) ; need template if nargs
	(segment->template (proc frame) frame)))))

; --------------------
; Direct primitives.

; The simplest kind of primitive has fixed arity, corresponds to some
; single VM instruction, and takes its arguments in the usual way (all
; on the stack).

(define (direct-compilator type opcode)
  (lambda (node depth frame cont)
    (let ((args (cdr (node-form node))))
      (sequentially (if (null? args)
                        empty-segment
                        (push-all-but-last args depth frame node))
                    (deliver-value (instruction opcode) cont)))))

(define (direct-closed-compilator opcode)
  (lambda (frame)
    (let ((arg-specs (vector-ref opcode-arg-specs opcode)))
      (sequentially (if (pair? arg-specs)
                        (sequentially
                         (instruction (enum op protocol)
                                      (car arg-specs)
                                      #b00)     ; ignore env and template
                         (instruction (enum op pop)))
                        (instruction (enum op protocol)
                                     0
                                     #b00))     ; ignore env and template
                    (instruction opcode)
                    (instruction (enum op return))))))

(define (nargs->domain nargs)
  (do ((nargs nargs (- nargs 1))
       (l '() (cons value-type l)))
      ((= nargs 0) (make-some-values-type l))))

(define (get-primop-type id arg-count)
  (or (any (lambda (foo)
	     (if (if (pair? (car foo))
		     (memq id (car foo))
		     (eq? id (car foo)))
		 (cadr foo)
		 #f))
	   primop-types)
      (procedure-type (nargs->domain arg-count)
		      value-type
		      #t)))

; Types for various primops.

(define primop-types
  `((with-continuation
     ,(proc (escape-type (proc () any-values-type #f))
	    any-arguments-type))
    (eq?
     ,(proc (value-type value-type) boolean-type))
    ((number? integer? rational? real? complex? char? eof-object? port?)
     ,(proc (value-type) boolean-type))
    (exact?
     ,(proc (number-type) boolean-type))
    (exact->inexact
     ,(proc (exact-type) inexact-type))
    (inexact->exact
     ,(proc (inexact-type) exact-type))
    ((exp log sin cos tan asin acos sqrt)
     ,(proc (number-type) number-type))
    ((floor)
     ,(proc (real-type) integer-type))
    ((real-part imag-part angle magnitude)
     ,(proc (complex-type) real-type))
    ((numerator denominator)
     ,(proc (rational-type) integer-type))
    ((make-polar make-rectangular)
     ,(proc (real-type real-type) complex-type))
    ((quotient remainder)
     ,(proc (integer-type integer-type) integer-type))
    ((bitwise-not)
     ,(proc (exact-integer-type) exact-integer-type))
    ((arithmetic-shift)
     ,(proc (exact-integer-type exact-integer-type)
	    exact-integer-type))
    ((char=? char<?)
     ,(proc (char-type char-type) boolean-type))
    (char->ascii
     ,(proc (char-type) exact-integer-type))
    (ascii->char
     ,(proc (exact-integer-type) char-type))
    (string=?
     ,(proc (string-type string-type) boolean-type))
    (open-channel
     ;; Can return #f
     ,(proc (string-type exact-integer-type boolean-type) value-type))
    (cons
     ,(proc (value-type value-type) pair-type))
    (intern
     ,(proc (string-type) symbol-type))))

; Can't do I/O until the meta-types interface exports input-port-type and
; output-port-type.

; Define all the primitives that correspond to opcodes in the obvious way.

(do ((opcode 0 (+ opcode 1)))
    ((= opcode op-count))
  (let ((arg-specs (vector-ref opcode-arg-specs opcode))
        (name (enumerand->name opcode op)))
    (cond ((memq name '(call-external-value return-from-interrupt return)))
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
           (let ((type (get-primop-type name (car arg-specs))))
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
  (lambda (node depth frame cont)
    (let ((args (cdr (node-form node))))
      (sequentially (if (null? args)
                        empty-segment
                        (push-all-but-last args depth frame node))
                    (deliver-value segment cont)))))

(define (simple-closed-compilator nargs segment)
  (lambda (frame)
    (sequentially (instruction (enum op protocol) nargs #b00)
                  (if (< 0 nargs)
                      (instruction (enum op pop))
                      empty-segment)
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

(define-stob-predicate 'byte-vector? 'byte-vector)
(define-stob-predicate 'double? 'double)
(define-stob-predicate 'string? 'string)

; Making doubles

(let ((:double (sexp->type ':double #t)))
  (define-compiler-primitive 'make-double (proc () :double)
    (lambda (node depth frame cont)
      (deliver-value
        (sequentially
	  (stack-indirect-instruction (template-offset frame depth)
				      (literal->index frame 0))
	  (instruction (enum op push))		; leaves value in *val*
	  (instruction (enum op make-stored-object) 2 (enum stob double)))
	cont))
    (cons 0
	  (lambda (frame)
	    (sequentially (instruction (enum op protocol) 0 #b10)
			  (instruction (enum op stack-indirect)
				       (template-offset frame 1) ; template
				       (literal->index frame 0))
			  (instruction (enum op push))
			  (instruction (enum op return)))))))

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
	    (begin
	      (if (not (eq? (cadr slot)
			    'cell-set!))
		  (def-prim (cadr slot)
		    (proc (type value-type) unspecific-type)
		    (enum op stored-object-set!) type-byte i 0))
	      (if (car slot)
		  (def-prim (symbol-append 'provisional- (car slot))
		    (proc (type) value-type)
		    (enum op stored-object-logging-ref) type-byte i))
	      (def-prim (symbol-append 'provisional- (cadr slot))
		(proc (type value-type) unspecific-type)
		(enum op stored-object-set!) type-byte i 1)))))))

(for-each (lambda (stuff)
            (apply define-data-struct-primitives stuff))
          stob-data)

; CELL-SET! is special because we want to capture names for the debugging data.
; Other than using NAMED-CONT when it can for compiling the value this is the
; same as all the other accessors.

(let ((inst (instruction (enum op stored-object-set!)
			 (enum stob cell)
			 0		; index
			 0)))		; not provisional
  (define-compiler-primitive 'cell-set!
    (proc ((sexp->type ':cell #t) value-type) unspecific-type)
    (lambda (node depth frame cont)
      (let ((args (cdr (node-form node))))
	(cond ((name-node? (car args))
	       (sequentially 
		 (push-argument node 0 depth frame)
		 (compile (cadr args)
			  (+ depth 1)
			  frame
			  (named-cont (node-form (car args))))
		 (deliver-value inst cont)))
	      (else
	       (sequentially (push-all-but-last args depth frame node)
			     (deliver-value inst cont))))))
    (simple-closed-compilator 2 inst)))

; Define primitives for vector-like stored objects.

(define (define-vector-primitives name element-type)
  (let* ((type-byte (name->enumerand name stob))
         (def-prim (lambda (name type op . more)
                     (define-simple-primitive name type
                       (apply instruction op type-byte more))))
         (type (sexp->type (symbol-append ': name) #t)))
    (define-stob-predicate (symbol-append name '?) name)
    (if (not (eq? name 'vector))  ; 2nd arg to make-vector is optional
	(def-prim (symbol-append 'make- name)
	  (proc (exact-integer-type element-type) type)
	  (enum op make-vector-object)))
    (def-prim (symbol-append name '- 'length)
      (proc (type) exact-integer-type)
      (enum op stored-object-length))
    (def-prim (symbol-append name '- 'ref)
      (proc (type exact-integer-type) element-type)
      (enum op stored-object-indexed-ref)
      0)		; do not log in the proposal
    (def-prim (symbol-append name '- 'set!)
      (proc (type exact-integer-type element-type) unspecific-type)
      (enum op stored-object-indexed-set!)
      0)))		; do not log in the proposal

(for-each (lambda (name)
            (define-vector-primitives name value-type))
          '(vector record continuation extended-number template))

(define-syntax define-more-vector-primitives
  (syntax-rules ()
    ((define-vector-primitives
       (ref ref-op)
       (set set-op)
       vector-type elt-type (more ...))
     (begin
       (define-simple-primitive 'ref
	 (proc (vector-type exact-integer-type) elt-type)
	 (instruction (enum op ref-op) more ...))
       (define-simple-primitive 'set
	 (proc (vector-type exact-integer-type elt-type) unspecific-type)
	 (instruction (enum op set-op) more ...))))))

; Vector ref and set! that use the current proposal's logs.

(define-more-vector-primitives
  (provisional-vector-ref  stored-object-indexed-ref)
  (provisional-vector-set! stored-object-indexed-set!)
  vector-type
  value-type
  ((enum stob vector) 1))
       
(define-more-vector-primitives
  (provisional-byte-vector-ref  byte-vector-logging-ref)
  (provisional-byte-vector-set! byte-vector-logging-set!)
  value-type
  exact-integer-type
  ())
       
; Checked-record-ref and friends.

(let ((record-type (sexp->type ':record #t)))
  (define-simple-primitive 'checked-record-ref
    (proc (record-type value-type exact-integer-type) value-type)
    (instruction (enum op checked-record-ref) 0))

  (define-simple-primitive 'provisional-checked-record-ref
    (proc (record-type value-type exact-integer-type) value-type)
    (instruction (enum op checked-record-ref) 1))

  (define-simple-primitive 'checked-record-set!
    (proc (record-type value-type exact-integer-type value-type)
	  unspecific-type)
    (instruction (enum op checked-record-set!) 0))

  (define-simple-primitive 'provisional-checked-record-set!
    (proc (record-type value-type exact-integer-type value-type)
	  unspecific-type)
    (instruction (enum op checked-record-set!) 1)))

(let ((copy-type (proc (value-type exact-integer-type
		        value-type exact-integer-type
			exact-integer-type)
		       unspecific-type)))
  (define-simple-primitive 'copy-bytes! copy-type
    (instruction (enum op copy-bytes!) 0))
  (define-simple-primitive 'attempt-copy-bytes! copy-type
    (instruction (enum op copy-bytes!) 1)))


; SIGNAL-CONDITION is the same as TRAP.

(define-simple-primitive 'signal-condition (proc (pair-type) unspecific-type)
  (instruction (enum op trap)))

; (primitive-catch (lambda (cont) ...))

(define-compiler-primitive 'primitive-catch
  (proc ((proc (escape-type) any-values-type #f)) any-values-type)
  ;; (primitive-catch (lambda (cont) ...))
  (lambda (node depth frame cont)
    (let* ((exp (node-form node))
           (args (cdr exp)))
      (receive (before depth label after)
	  (maybe-push-continuation depth frame cont (car args))
	(depth-check! frame (+ depth 1))
	(sequentially before
	              (instruction (enum op current-cont))
		      (instruction (enum op push))
		      (compile (car args)
			       (+ depth 1)
			       frame
			       (fall-through-cont node 1))
		      (call-instruction 1 label)	; one argument
		      after))))
  (lambda (frame)
    (sequentially (instruction (enum op protocol) 1 #b00)
                  (instruction (enum op current-cont))
		  (instruction (enum op push))
		  (instruction (enum op stack-ref 1))
                  (call-instruction 1 #f))))	; one argument, no return label

; (call-with-values producer consumer)

; Optimization 1 (not done):
;  If consumer is a lambda then generate it in-line with its own protocol as
; the return protocol for the producer.  Once you have the values on the stack
; it can be treated as a redex.
; 
; Optimization 2 (not done):
;  If both the consumer and producer are lambdas then do as above except
; that the producer gets a special multiple-values continuation.  There
; could be a VALUES opcode that moved its arguments down to the appropriate
; point and then jumped, or the jump could be a separate instruction.
;  The jump target would have a protocol to allow tail calls within the
; producer as well.
;
; The closed-compiled version gets the arguments on the stack in the wrong
; order for what it wants to do.  It does:
;                             *val*     stack
;   Start                     ?         consumer producer ...
;   (stack-ref+push 1)        producer  producer consumer producer ...
;   (false)                   #f        producer consumer producer ...
;   (stack-set! 2)            #f        producer consumer #f ...
;   (pop)                     producer  consumer #f ...
; to reverse the order.

(define-compiler-primitive 'call-with-values
  (proc ((proc () any-values-type #f)
         any-procedure-type)
        any-values-type)
  (lambda (node depth frame cont)
    (let ((args (cdr (node-form node))))
      (receive (ignore-before ignore-depth c-label c-after)
	  (maybe-push-continuation depth frame cont (cadr args))
	(receive (p-before p-depth p-label p-after)
	    (push-continuation-no-protocol (+ depth 1)
					   frame
					   (car args)
					   (fall-through-cont node 1))
	  (sequentially (push-argument node 1 depth frame)
			p-before
			(compile (car args)
				 p-depth
				 frame
				 (fall-through-cont node 1))
			(instruction-using-label (enum op call)
						 p-label
						 0)
			p-after
			(optional-label-reference
			 (instruction (enum op protocol)
				      call-with-values-protocol)
			 c-label
			 empty-segment)
			c-after)))))
    (lambda (frame)
      (receive (before depth label after)
	  (push-continuation-no-protocol 2 frame #f (plain-fall-through-cont))
	(sequentially (instruction (enum op protocol) 2 #b00)
		      (instruction (enum op stack-ref+push) 1)
		      (instruction (enum op false))
		      (instruction (enum op stack-set!) 2)
		      (instruction (enum op pop))
		      before
		      (using-optional-label (enum op call) label 0)
		      after
		      (instruction (enum op protocol)
				   call-with-values-protocol
				   0
				   0)))))

; Is NODE a lambda with a null variable list.

(define (thunk-node? node)
  (and (or (lambda-node? node)
	   (flat-lambda-node? node))
       (null? (cadr (node-form node)))))

; Works for both normal and flat lambdas.

(define (thunk-body node)
  (last (node-form node)))

; Return a non-flat version of the possibly-flat lambda NODE's form.

(define (unflatten-form node)
  (let ((form (node-form node)))
    (if (flat-lambda-node? node)
	`(lambda ,(cadr form) ,(cadddr form))
	form)))

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
  (lambda (node depth frame cont)
    (let ((exp (node-form node)))
      (if (>= (length (cdr exp)) min-nargs)
          (compilator node depth frame cont)
          (begin (warn "too few arguments to primitive"
                       (schemify node))
                 (compile-unknown-call node depth frame cont))))))

; APPLY wants the arguments on the stack, with the final list on top, and the
; procedure in *VAL*.

(define-compiler-primitive 'apply
  (proc (any-procedure-type &rest value-type) any-values-type)
  (n-ary-primitive-compilator 'apply 2
    (lambda (node depth frame cont)
      (let* ((exp (node-form node))     ; (apply proc arg1 arg2 arg3 rest)
	     (proc+args+rest (cdr exp))
	     (rest+args               ; (rest arg3 arg2 arg1)
	      (reverse (cdr proc+args+rest)))
	     (args+rest+proc          ; (arg1 arg2 arg3 rest proc)
	      (reverse (cons (car proc+args+rest) rest+args)))
	     (stack-nargs (length (cdr rest+args))))
	(receive (before depth label after)
	    (maybe-push-continuation depth frame cont node)
	  (sequentially before
			(push-all-but-last args+rest+proc depth frame #f)
			;; Operand is number of non-final arguments
			(using-optional-label (enum op apply)
					      label
					      (high-byte stack-nargs)
					      (low-byte stack-nargs))
			after)))))
  (lambda (frame)
    (sequentially (instruction (enum op protocol) args+nargs-protocol 2 #b00)
                  (instruction (enum op closed-apply)))))

; (values value1 value2 ...)
;
; Okay, this is the second half of the deal.
;  - if tail-recursive, then just push the arguments followed by the opcode
;  - if ignore-values continuation, then evaluate the arguments without
;    doing any pushes in between
;  - if fall-through, then there had better be only one value
;  - that's it for now, given that there is no special CALL-WITH-VALUES
;    continuation

(define-n-ary-compiler-primitive 'values #f 0
  (lambda (node depth frame cont)
    (let* ((args (cdr (node-form node)))
           (nargs (length args)))
      (cond ((= 1 nargs)		; +++ (we miss some errors this way)
	     (compile (car args)
		      depth
		      frame
		      cont))
	    ((return-cont? cont)
	     (depth-check! frame (+ depth nargs))
	     (sequentially (push-arguments node depth frame)
			   (instruction (enum op values)
					(high-byte nargs)
					(low-byte nargs))))
	    ((ignore-values-cont? cont)
	     (evaluate-arguments-for-effect args node depth frame))
	    ((fall-through-cont? cont)
	     (generate-trap depth
			    frame
			    cont
			    (if (= nargs 0)
				"returning no arguments where one is expected"
				(string-append "returning "
					       (number->string nargs)
					       " arguments where one is expected"))
			    (schemify node)))
	    (else
	     (error "unknown compiler continuation for VALUES" cont)))))
  (lambda (frame)
    (sequentially (instruction (enum op protocol) args+nargs-protocol 0 #b00)
		  (instruction (enum op closed-values)))))

(define (evaluate-arguments-for-effect args node depth frame)
  (do ((args args (cdr args))
       (i 1 (+ i 1))
       (code empty-segment
	     (sequentially code
			   (compile (car args)
				    depth
				    frame
				    (fall-through-cont node i)))))
      ((null? args)
       code)))

; (error message irritant1 irritant2)
;  => (trap (cons 'error (cons message (cons irritant1 (cons irritant2 '())))))

(let ((cons-instruction
       (instruction (enum op make-stored-object) 2 (enum stob pair))))

  (define-n-ary-compiler-primitive 'error error-type 1
    (lambda (node depth frame cont)
      (let* ((exp (node-form node))
	     (args (cdr exp)))
	(sequentially
	  (stack-indirect-instruction (template-offset frame depth)
				      (literal->index frame 'error))
	  (instruction (enum op push))
	  (push-arguments node (+ depth 1) frame)
	  (stack-indirect-instruction
	    (template-offset frame
			     (+ depth 1 (length args)))
	    (literal->index frame '()))
	  (apply sequentially
		 (map (lambda (arg) cons-instruction) args))
	  cons-instruction
	  (deliver-value (instruction (enum op trap)) cont))))
    (cons 2
	  (lambda (frame)
	    ; stack at start is: template irritants message
	    (sequentially (instruction (enum op protocol)
				       two-byte-nargs+list-protocol
				       0              ; (high-byte 1) 
				       1              ; (low-byte 1)
				       #b10)
			  (instruction (enum op stack-ref) 1)       ; irritants
			  (instruction (enum op push))
			  (instruction (enum op stack-ref) 3)       ; message
			  cons-instruction
			  (instruction (enum op push))
			  ; (message . irritants) template irritants message
			  (instruction (enum op stack-indirect)
				       1
				       (literal->index frame 'error))
			  (instruction (enum op push))
			  (instruction (enum op stack-ref) 1)
			  cons-instruction
			  (instruction (enum op trap))
			  (instruction (enum op return)))))))
    
; (call-external-value external-routine arg ...)

(define-n-ary-compiler-primitive 'call-external-value value-type 1
  #f                                         ;Could be done
  (lambda (frame)
    (sequentially (instruction (enum op protocol) args+nargs-protocol 1 #b00)
                  (instruction (enum op call-external-value))
                  (instruction (enum op return)))))

(let ((n-ary-constructor
        (lambda (name type type-byte)
	  (define-n-ary-compiler-primitive name type 0
	    (lambda (node depth frame cont)
	      (let ((args (cdr (node-form node))))
		(sequentially (if (null? args)
				  empty-segment
				  (push-all-but-last args depth frame node))
			      (deliver-value
			       (instruction (enum op make-stored-object)
					    (length args)
					    type-byte)
			       cont))))
	    (lambda (frame)
              (sequentially
                (instruction (enum op protocol) args+nargs-protocol 0 #b00)
		(instruction (enum op closed-make-stored-object) type-byte)
		(instruction (enum op return))))))))
  (n-ary-constructor 'vector vector-type (enum stob vector))
  (n-ary-constructor 'record #f (enum stob record)))

; READ-CHAR, PEEK-CHAR and WRITE-CHAR

(let ((define-char-io
	(lambda (id opcode type)
	  (define-compiler-primitive id
	    type
	    (lambda (node depth frame cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node depth frame cont)
		  (let ((args (cdr (node-form node))))
		    (if (null? args)
			(deliver-value (instruction opcode 1) cont)
			(sequentially
			 (push-all-but-last args depth frame node)
			 (deliver-value (instruction opcode 0) cont))))))
            (lambda (frame)
              (make-dispatch-protocol
	        ; Zero arguments
	        (sequentially (instruction opcode 1)
			      (instruction (enum op return)))
		; One argument
		(sequentially (instruction (enum op pop))
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
	    (lambda (node depth frame cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node depth frame cont)
		  (let ((args (cdr (node-form node))))
		    (sequentially
		     (push-all-but-last args depth frame node)
		     (if (null? (cdr args))
			 (deliver-value (instruction opcode 1) cont)
			 (sequentially
			  (deliver-value (instruction opcode 0) cont)))))))
            (lambda (frame)
              (make-dispatch-protocol
                empty-segment
                ; One argument
                (sequentially (instruction (enum op pop))
			      (instruction opcode 1)
			      (instruction (enum op return)))
                ; Two arguments
                (sequentially (instruction (enum op pop))
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
	    (lambda (node depth frame cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node depth frame cont)
		  (let ((args (cdr (node-form node))))
		    (cond ((null? args)
			   (deliver-value
			     (stack-indirect-instruction
			       (template-offset frame depth)
			       (literal->index frame identity))
			     cont))
			  ((null? (cdr args))
			   (call-on-arg-and-id opcode identity (car args)
					       node depth frame cont))
			  (else
			   (call-on-args opcode args node depth frame cont))))))
	    (lambda (frame)
              (make-dispatch-protocol
                ; No arguments
                (sequentially (integer-literal-instruction identity)
			      (instruction (enum op return)))
                ; One argument
                (sequentially (integer-literal-instruction identity)
			      (instruction opcode)
			      (instruction (enum op return)))
                ; Two arguments
                (sequentially
                  (instruction (enum op pop))
                  (instruction opcode)
                  (instruction (enum op return)))
                ; More than two arguments
                (sequentially
                  (instruction (enum op pop))   ; pop off nargs
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
	    (lambda (node depth frame cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node depth frame cont)
		  (let ((args (cdr (node-form node))))
		    (if (= (length args) 2)
			(call-on-args opcode args node depth frame cont)
			(compile-unknown-call node depth frame cont)))))
	    (lambda (frame)
              (make-dispatch-protocol
	        empty-segment
                empty-segment
                ; Two arguments
                (sequentially (instruction (enum op pop))  ; get first argument
			      (instruction opcode)
			      (instruction (enum op return)))
                ; More than two arguments
                (sequentially (instruction (enum op pop))
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

(define (call-on-arg-and-id opcode identity argument node depth frame cont)
  (sequentially (stack-indirect-instruction (template-offset frame depth)
					    (literal->index frame identity))
		(instruction (enum op push))
		(compile argument (+ depth 1) frame (fall-through-cont node 1))
		(deliver-value (instruction opcode) cont)))
  
; Returns code to reduce ARGS using OPCODE.

(define (call-on-args opcode args node depth frame cont)
  (let ((start (sequentially
		 (push-all-but-last (list (car args)
					  (cadr args))
				    depth
				    frame
				    node)
		  (instruction opcode))))
    (let loop ((args (cddr args)) (i 3) (code start))
      (if (null? args)
	  (deliver-value code cont)
	  (loop (cdr args)
		(+ i 1)
		(sequentially code
			      (push-and-compile (car args)
						(+ depth 1)
						frame
						(fall-through-cont node i))
			      (instruction opcode)))))))

(define (push-and-compile node depth frame cont)
  (or (maybe-compile-with-push node depth frame #t)	; +++
      (sequentially push-instruction
		    (compile node depth frame cont))))

(define op/unspecific (get-operator 'unspecific))
(define op/literal (get-operator 'literal))

; -, and / take one or two arguments.

(let ((define-one-or-two
	(lambda (id opcode default-arg)
	  (define-compiler-primitive id
            (proc (number-type &opt number-type) number-type)
	    (lambda (node depth frame cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node depth frame cont)
		  (let* ((args (cdr (node-form node)))
			 (args (if (null? (cdr args))
				   (list (make-node op/literal default-arg)
					 (car args))
				   args)))
		    (sequentially
		     (push-all-but-last args depth frame node)
		     (deliver-value (instruction opcode) cont)))))
	    (lambda (frame)
              (make-dispatch-protocol
	        empty-segment
                ; One argument
                (sequentially (integer-literal-instruction default-arg)
			      (instruction (enum op push))
			      (instruction (enum op stack-ref) 1)
			      (instruction opcode)
			      (instruction (enum op return)))
                ; Two arguments
                (sequentially (instruction (enum op pop))
			      (instruction opcode)
			      (instruction (enum op return)))
                empty-segment))))))
  (define-one-or-two '- (enum op -) 0)
  (define-one-or-two '/ (enum op /) 1))

; ATAN also takes one or two arguments, but the meanings are disjoint.

(define-compiler-primitive 'atan
  (proc (number-type &opt number-type) number-type)
  (lambda (node depth frame cont)
    (if (node-ref node 'type-error)
	(compile-unknown-call node depth frame cont)
	(let* ((args (cdr (node-form node)))
	       (opcode (if (null? (cdr args))
			   (enum op atan1)
			   (enum op atan2))))
	  (sequentially
	   (push-all-but-last args depth frame node)
	   (deliver-value (instruction opcode) cont)))))
  (lambda (frame)
    (make-dispatch-protocol
      empty-segment
      ; One argument
      (sequentially (instruction (enum op pop))
		    (instruction (enum op atan1))
		    (instruction (enum op return)))
      ; Two arguments
      (sequentially (instruction (enum op pop))
		    (instruction (enum op atan2))
		    (instruction (enum op return)))
      empty-segment)))

; make-vector and make-string take one or two arguments.

(let ((define-one-or-two
	(lambda (id op-segment default-arg default-arg-segment type)
	  (define-compiler-primitive id
	    type
	    (lambda (node depth frame cont)
	      (if (node-ref node 'type-error)
		  (compile-unknown-call node depth frame cont)
		  (let* ((args (cdr (node-form node)))
			 (args (if (null? (cdr args))
				   (list (car args) default-arg)
				   args)))
		    (sequentially
		     (push-all-but-last args depth frame node)
		     (deliver-value op-segment cont)))))
	    (lambda (frame)
              (make-dispatch-protocol
	        empty-segment
		; One argument
                (sequentially default-arg-segment
			      op-segment
			      (instruction (enum op return)))
                ; Two arguments
                (sequentially (instruction (enum op pop))
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
    (sequentially (integer-literal-instruction (char->ascii #\?))
		  (instruction (enum op ascii->char)))
    (proc (number-type &opt char-type) string-type)))

; --------------------
; Utilities

; Building primitives that use the computed-goto provided by the
; protocol dispatcher.

(define dispatch-protocol-size
  (segment-size (instruction (enum op protocol) nary-dispatch-protocol
			     0     ; 3+
			     0     ; 0
			     0     ; 1
			     0     ; 2
			     0)))  ; env/template

; For a silly reason involving the way the call-setup code in the VM is
; organized we have to the THREE-PLUS-ARGS offset and code come before
; the others.

(define (make-dispatch-protocol zero-args one-arg two-args three-plus-args)
  (let ((segments (list three-plus-args zero-args one-arg two-args)))
    (let loop ((to-do segments)
	       (offset dispatch-protocol-size)
	       (offsets '()))
      (if (null? to-do)
	  (apply sequentially
		 (apply instruction
			(enum op protocol)
			nary-dispatch-protocol
			(reverse (cons #b00 offsets))) ; no env, no template
		 segments)
	  (loop (cdr to-do)
		(+ offset (segment-size (car to-do)))
		(cons (if (empty-segment? (car to-do))
			  0
			  offset)
		      offsets))))))

(define (empty-segment? segment)
  (= (segment-size segment)
     0))