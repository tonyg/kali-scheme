; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


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
  (operator-define! closed-compilators name proc))

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
; on the stack except the last).

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
                         (instruction (enum op check-nargs=) (car arg-specs))
                         (instruction (enum op pop)))
                        (instruction (enum op check-nargs=) 0))
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
    (sequentially (instruction (enum op check-nargs=) nargs)
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
    (if (not (eq? maker 'make-symbol))  ; Symbols are made using op/intern.
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
        (if (cadr slot)
            (def-prim (cadr slot)
              (proc (type value-type) unspecific-type)
              (enum op stored-object-set!) type-byte i))))))

(for-each (lambda (stuff)
            (apply define-data-struct-primitives stuff))
          stob-data)


; Define primitives for vector-like stored objects.

(define (define-vector-primitives name element-type make length ref set!)
  (let* ((type-byte (name->enumerand name stob))
         (def-prim (lambda (name type op)
                     (define-simple-primitive name type
                       (instruction op type-byte))))
         (type (sexp->type (symbol-append ': name) #t)))
    (define-stob-predicate (symbol-append name '?) name)
    (def-prim (symbol-append 'make- name)
      (proc (exact-integer-type element-type) type)
      make)
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
                     (compile (car args) cenv 1
                              (fall-through-cont node 1))
                     (instruction (enum op call) 1))
       0
       cont)))
  (lambda ()
    (sequentially (instruction (enum op check-nargs=) 1)
                  (instruction (enum op make-env) 1)  ;Seems unavoidable.
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
      (sequentially (instruction (enum op check-nargs=) 2)
                    (instruction (enum op make-env) 2)
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


; APPLY wants to first spread the list, then load the procedure.
; The list argument has to be in *VAL* so that its length can be checked
; before the instruction is begun.

(define-n-ary-compiler-primitive 'apply #f 2
  (lambda (node cenv depth cont)
    (let ((exp (node-form node)))       ; (apply proc arg1 arg2 arg3 rest)
      (let* ((proc+args+rest (cdr exp))
             (rest+args                 ; (rest arg3 arg2 arg1)
              (reverse (cdr proc+args+rest)))
             (args (cdr rest+args))     ; (arg3 arg2 arg1)
             (args+proc+rest            ; (arg1 arg2 arg3 proc rest)
              (reverse (cons (car rest+args)
                             (cons (car proc+args+rest) args)))))
        (maybe-push-continuation
         (sequentially (push-all-but-last args+proc+rest cenv 0 #f)
                       ;; Operand is number of non-final arguments
                       (instruction (enum op apply) (length args)))
         depth
         cont))))
  (lambda ()
    (sequentially (instruction (enum op check-nargs=) 2)
                  (instruction (enum op pop))
                  (instruction (enum op apply) 0))))


; (values value1 value2 ...)

(define-n-ary-compiler-primitive 'values #f 0
  (lambda (node cenv depth cont)
    (let ((args (cdr (node-form node))))
      (maybe-push-continuation (sequentially (push-arguments node cenv 0)
                                             (instruction (enum op return-values)
                                                          (length args)))
                               depth
                               cont)))
  (lambda () (instruction (enum op values))))


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
      (sequentially (instruction (enum op make-rest-list) 0)
                    (instruction (enum op push))
                    (instruction-with-literal (enum op literal) 'error)
                    (instruction (enum op push))
                    (instruction (enum op stack-ref) 1)
                    cons-instruction
                    (instruction (enum op trap))
                    (instruction (enum op return))))))


; (external-call external-routine arg ...)

(define-n-ary-compiler-primitive 'external-call value-type 1
  #f                                         ;Must set *nargs*
  (lambda ()
    (sequentially (instruction (enum op check-nargs>=) 1)
                  (instruction (enum op external-call))
                  (instruction (enum op return)))))


; --------------------
; Utility

(define (push-all-but-last args cenv depth source-info)
  (let recur ((args args) (depth depth) (i 1))
    (let ((first-code
           (compile (car args) cenv depth (fall-through-cont source-info i))))
      (if (null? (cdr args))
          first-code
          (sequentially first-code
                        (instruction (enum op push))
                        (recur (cdr args) (+ depth 1) (+ i 1)))))))
