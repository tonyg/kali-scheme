; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file interp-st.scm.

; Machine state

(define (initialize-machine)          ;Used only at startup
  (set! *interrupt-template*
	(make-special-two-op-template op/ignore-values
				      op/return-from-interrupt))
  (initialize-stack))

(define initial-machine-heap-space
  (+ special-two-op-template-size
     initial-stack-heap-space))

; Continuations

(define *template* (unassigned))
(define *env*      (unassigned))
(define *cont*     (unassigned))

(define continuation-stack-size (stack-continuation-size continuation-cells))

(define (push-continuation code-pointer size key)
  (let ((cont (push-continuation-on-stack size key)))
    (set-continuation-pc!       cont (code-pointer->pc code-pointer *template*))
    (set-continuation-template! cont *template*)
    (set-continuation-env!      cont *env*)
    (set-continuation-cont!     cont *cont*)
    (set! *cont* cont)))

(define (push-exception-continuation key)
  (push-continuation *code-pointer* (arguments-on-stack) key))

(define (pop-continuation)
  (let ((cont *cont*))
    (restore-from-continuation cont)
    (pop-continuation-from-stack cont)))

(define (restore-from-continuation cont)
  (set-template! (continuation-template cont)
		 (continuation-pc       cont))
  (set! *env*    (continuation-env      cont))
  (set! *cont*   (continuation-cont     cont)))

(define *code-pointer* (unassigned)) ; pointer to next instruction byte

(define (set-template! tem pc)
  (set! *template* tem)
  (set! *code-pointer* (addr+ (address-after-header (template-code tem))
			      (extract-fixnum pc))))

(define (code-pointer->pc pointer template)
  (enter-fixnum (addr- pointer
                       (address-after-header (template-code template)))))

(define (current-pc)
  (code-pointer->pc *code-pointer* *template*))

; Interpreter state

(define *nargs*              (unassigned))
(define *val*                (unassigned))
(define *enabled-interrupts* (unassigned))
(define *exception-handler*  (unassigned))
(define *interrupt-handlers* (unassigned))
(define *dynamic-state*      (unassigned))

; Miscellaneous registers

(define *pending-interrupts*       (unassigned))
(define *interrupt-template*       (unassigned))

(define (clear-registers)
  (set-template! *interrupt-template*          ; has to be some template
		 (enter-fixnum 0))
  (set! *env*                unspecific)
  (set! *cont*               (reset-stack-pointer))

  (set! *nargs*              unspecific)        ; interpreter regs
  (set! *val*                unspecific)
  (set! *dynamic-state*      null)
  (set! *exception-handler*  null)
  (set! *interrupt-handlers* null)
  (set! *enabled-interrupts* 0)

  (set! *pending-interrupts* 0)
  unspecific)

; Instruction stream access

(define (this-byte)
  (fetch-byte *code-pointer*))

(define (next-byte)
  (let ((b (this-byte)))
    (set! *code-pointer* (addr+ *code-pointer* 1))
    b))

(define (previous-byte)  ;probably not necessary
  (set! *code-pointer* (addr+ *code-pointer* -1)))

(define (next-offset)
  (let ((high (next-byte)))
    (adjoin-bits high (next-byte) bits-used-per-byte)))

(define (nth-offset n)
  (set! *code-pointer* (addr+ *code-pointer* (* n 2)))
  (next-offset))

(define (next-literal)
  (template-ref *template* (next-byte)))

; Return the current op-code.  CODE-ARGS is the number of argument bytes that
; have been used.

(define (current-opcode code-args)
  (fetch-byte (addr- *code-pointer* (+ code-args 1))))

; Environment access

(define make-rib vm-make-vector)
(define rib-ref  vm-vector-ref)
(define rib-set! vm-vector-set!)
(define (rib-parent rib) (rib-ref rib 0))

(define (env-back env back)  ;Resembles NTHCDR
  (do ((env env (rib-parent env))
       (i back (- i 1)))
      ((= i 0) env)))

; Different ways to call the GC.

(define (maybe-ensure-space space)
  (if (not (available? space))
      (interpreter-collect)))

(define (ensure-space space)
  (if (not (available? space))
      (interpreter-collect))
  (if (available? space)
      (preallocate-space space)
      (error "Scheme48 heap overflow")))

; Actual call to GC

(define *minimum-recovered-space* 0)

(define (interpreter-collect)
  (collect-saving-temp (enter-fixnum 0)))

(define (collect-saving-temp value)
  (begin-collection)
  (trace-registers)
  (let ((value (trace-value value)))
    (end-collection)
    (if (not (available? *minimum-recovered-space*))
	(set-interrupt! interrupt/memory-shortage))
    value))

(define (trace-registers)
  (let ((pc (code-pointer->pc *code-pointer* *template*)))
    (set-template! (trace-value *template*) pc))
  (set! *env*                (trace-value *env*))
  (set! *val*                (trace-value *val*))
  (set! *dynamic-state*      (trace-value *dynamic-state*))
  (set! *exception-handler*  (trace-value *exception-handler*))
  (set! *interrupt-handlers* (trace-value *interrupt-handlers*))
  (set! *interrupt-template* (trace-value *interrupt-template*))
  (trace-stack *cont*))

; INTERPRET is the main instruction dispatch for the interpreter.

(define trace-instructions? #f)

(define (interpret)
  (if trace-instructions?
      (write-instruction *template* (extract-fixnum (current-pc)) 1 #f))
  (let ((op-code (next-byte)))
    ((vector-ref opcode-dispatch op-code))))

;;; Opcodes

(define (uuo) (goto raise-exception 0))

(define opcode-dispatch (make-vector op-count))

(vector+length-fill! opcode-dispatch op-count uuo)

(define (define-opcode opcode tag)
  (vector-set! opcode-dispatch opcode tag))

; Check number of arguments

(define-opcode op/check-nargs= (lambda ()
  (if (= *nargs* (next-byte))
      (goto interpret)
      (goto application-exception 1))))

(define-opcode op/check-nargs>= (lambda ()
  (if (>= *nargs* (next-byte))
      (goto interpret)
      (goto application-exception 1))))

(define-opcode op/nargs (lambda ()
  (set! *val* (enter-fixnum *nargs*))
  (goto interpret)))

; Pop off all arguments into a list and raise and exception.  *val* is the
; procedure being called.  BYTE-ARGS is the number of bytes of the instruction
; stream that the instruction has consumed.

(define (application-exception byte-args)
  (let ((args (if (= *nargs* arg-stack-overflow-nargs)
		  (pop-args-list (pop) maximum-stack-args)
		  (pop-args-list null *nargs*))))
    (goto raise-exception-no-cont2 byte-args *val* args)))

(define (pop-args-list start count)
  (let ((key (ensure-space (* vm-pair-size *nargs*))))
    (do ((args start (vm-cons (pop) args key))
	 (count count (- count 1)))
	((<= count 0)
	 args))))

;  Create a list to hold all but MIN-ARGS of the available arguments and put
; it in *val*.
;  If *nargs* is args-stack-overflow-nargs the top of the stack is a list of
; arguments and not just the last argument.

(define-opcode op/make-rest-list (lambda ()
  (let* ((min-args (next-byte))
	 (args (if (= *nargs* arg-stack-overflow-nargs)
		   (pop-args-list (pop) (- (- *nargs* 1) min-args))
		   (pop-args-list null  (- *nargs* min-args)))))
    (set! *val* args)
    (set! *nargs* (+ min-args 1))
    (goto interpret))))

; Environment creation
;  The MAKE-ENV instruction adds a rib to the local environment.
; It pops values off the stack and stores them into the new rib.

(define-opcode op/make-env (lambda ()
  (let* ((key (ensure-env-space (this-byte)))
	 (env (pop-args-into-env (this-byte) key)))
      (next-byte)
      (rib-set! env 0 *env*)
      (set! *env* env)
      (goto interpret))))

; The above with the environment in the heap and not on the stack.

(define-opcode op/make-heap-env (lambda ()
  (let* ((key (ensure-heap-env-space *nargs*))
	 (env (pop-args-into-heap-env (this-byte) key)))
      (next-byte)
      (rib-set! env 0 *env*)
      (set! *env* env)
      (goto interpret))))

(define-opcode op/pop-env (lambda ()
  (set! *env* (rib-parent *env*))
  (goto interpret)))

; Literals

(define-opcode op/literal (lambda ()    ;Load a literal into *val*.
  (set! *val* (next-literal))
  (goto interpret)))

; Local variable access and assignment

(define-opcode op/local (lambda ()      ;Load value of a local.
  (finish-local (env-back *env* (next-byte)) 2)))

(define-opcode op/local0 (lambda ()
  (finish-local *env* 1)))

(define-opcode op/local1 (lambda ()
  (finish-local (rib-parent *env*) 1)))

(define-opcode op/local2 (lambda ()
  (finish-local (rib-parent (rib-parent *env*)) 1)))

(define (finish-local env arg-count)
  (set! *val* (rib-ref env (next-byte)))
  (cond ((not (vm-eq? *val* unassigned-marker))
	 (goto interpret))
	(else
	 (goto raise-exception arg-count))))

(define-opcode op/set-local! (lambda ()
  (let ((back (next-byte)))
    (rib-set! (env-back *env* back)
              (next-byte)
              *val*)
    (set! *val* unspecific)
    (goto interpret))))

; Global variable access

(define-opcode op/global (lambda ()        ;Load a global variable.
  (let ((location (next-literal)))
    (set! *val* (contents location))
    (cond ((undefined? *val*)           ;unbound or unassigned
	   (goto raise-exception1 1 location))
          (else
           (goto interpret))))))

(define-opcode op/set-global! (lambda ()
  (let ((location (next-literal)))
    (cond ((vm-eq? (contents location) unbound-marker)
           (goto raise-exception2 1 location *val*))
          (else
           (set-contents! location *val*)
           (set! *val* unspecific)
           (goto interpret))))))

; Stack operation

(define-opcode op/push (lambda ()       ;Push *val* onto the stack.
  (push *val*)
  (goto interpret)))

(define-opcode op/pop (lambda ()       ;Pop *val* from the stack.
  (set! *val* (pop))
  (goto interpret)))

(define-opcode op/stack-ref (lambda ()
  (set! *val* (stack-ref (next-byte)))
  (goto interpret)))

(define-opcode op/stack-set! (lambda ()
  (stack-set! (next-byte) *val*)
  (goto interpret)))

; LAMBDA

(define-opcode op/closure (lambda ()
  (preserve-*env*)     ; may cause a GC
  (let ((key (ensure-space closure-size)))
    (set! *val* (make-closure (next-literal) *env* key))
    (goto interpret))))

; Procedure call

(define-opcode op/call (lambda ()
  (set! *nargs* (this-byte))
  (goto perform-application 0)))

; Same as OP/CALL except that the arguments are moved to just above *CONT*
; before the call is made.  For non-tail calls and some tail-calls the arguments
; will already be there.

(define-opcode op/move-args-and-call (lambda ()
  (set! *nargs* (this-byte))
  (move-args-above-cont *cont* *nargs*)
  (goto perform-application 0)))

(define-opcode op/goto-template (lambda ()
  (set-template! (next-literal) 0)
  (goto interpret)))

(define-opcode op/call-template (lambda ()
  (set! *nargs* (next-byte))  ; needed in case of interrupts
  (set-template! (next-literal) 0)
  (goto poll-for-interrupts)))

; Continuation creation and invocation

(define-opcode op/make-cont (lambda ()   ;Start a non-tail call.
  (let* ((key (ensure-stack-space continuation-stack-size))
	 (offset (next-offset))
	 (size (next-byte)))
     (push-continuation (addr+ *code-pointer* offset) size key)
     (goto interpret))))

(define-opcode op/return (lambda ()            ;Invoke the continuation.
  (pop-continuation)
  (goto interpret)))

; The arguments are sitting on the stack, with the count in *nargs*.
; This is only used in the closed-compiled version of VALUES.

(define-opcode op/values (lambda ()
  (goto return-values *nargs*)))

; Same as the above, except that the value count is in the instruction stream.
; This is used for in-lining calls to VALUES.

(define-opcode op/return-values (lambda ()
  (goto return-values (this-byte))))

; NARGS return values are on the stack.  If there is only one value, pop
; it off and do a normal return.  Otherwise, find the actual continuation
; and see if it ignores the values, wants the values, or doesn't know
; anything about them.

(define (return-values nargs)
  (cond ((= nargs 1)
	 (set! *val* (pop))
	 (pop-continuation)
	 (goto interpret))
	((not (addr= *cont* *bottom-of-stack*))
	 (goto really-return-values *cont* nargs))
	((continuation? (continuation-cont *cont*))
         (goto really-return-values (continuation-cont *cont*) nargs))
	(else
	 (goto return-exception nargs))))
	 
(define (really-return-values cont nargs)
  (let ((op (code-vector-ref (template-code (continuation-template cont))
			     (extract-fixnum (continuation-pc cont)))))
    (cond ((= op op/ignore-values)
	   (pop-continuation)
	   (goto interpret))
	  ((= op op/call-with-values)
	   (goto do-call-with-values nargs))
	  (else
	   (goto return-exception nargs)))))

; The continuation containing the consumer is removed (the consumer
; is the only useful information it contains), and then the consumer
; is called.

(define (do-call-with-values nargs)
  (let ((cont (if (addr= *cont* *bottom-of-stack*)
		  (let ((cont (continuation-cont *cont*)))
		    (set-continuation-cont! *cont* (continuation-cont cont))
		    cont)
		  (let ((cont *cont*))
		    (set! *cont* (continuation-cont cont))
		    cont))))
    (set! *nargs* nargs)
    (set! *val* (continuation-ref cont continuation-cells))
    (goto perform-application 0)))

; This would avoid the need for the consumer to be a closure.  It doesn't
; work because the NARGS check (which would be the next instruction to be
; executed) assumes that a closure has just been called.

;(define (do-call-with-values nargs)
;  (cond ((addr= *cont* *bottom-of-stack*)
;	 (restore-from-continuation (continuation-cont *cont*))
;	 (set-continuation-cont! *bottom-of-stack* *cont*)
;	 (set! *cont* *bottom-of-stack*))
;	(else
;	 (restore-from-continuation cont)))
;  (set! *nargs* nargs)
;  (set! *code-pointer* (addr+ *code-pointer* 1)) ; move past op/call-with-values
;  (goto interpret))

(define (return-exception nargs)
  (let ((args (if (= nargs arg-stack-overflow-nargs)
		  (pop-args-list (pop) maximum-stack-args)
		  (pop-args-list null nargs))))
    (goto raise-exception1 0 args)))

; This is executed only if the producer returned exactly one value.

(define-opcode op/call-with-values (lambda ()
  (let ((consumer (pop)))
    (push *val*)
    (set! *val* consumer)
    (set! *nargs* 1)
    (goto perform-application 0))))

; This is just a marker for the code that handles returns.

(define-opcode op/ignore-values (lambda ()
  (goto interpret)))

; IF

(define-opcode op/jump-if-false (lambda ()
  (let ((offset (next-offset)))
    (cond ((false? *val*)
           (set! *code-pointer* (addr+ *code-pointer* offset))
           (goto interpret))
          (else
           (goto interpret))))))

; Unconditional jump

(define-opcode op/jump (lambda ()
  (let ((offset (next-offset)))
    (set! *code-pointer* (addr+ *code-pointer* offset))
    (goto interpret))))

; Computed goto
; Goto index is in *val*, the next byte is the number of offsets specified
; The default is to jump to the instruction following the offsets
; The instruction stream looks like
; op/computed-goto max offset0 offset1 ... offsetmax-1 code-for-default...

(define-opcode op/computed-goto (lambda ()
  (if (not (fixnum? *val*))
      (goto raise-exception1 0 *val*)
      (let ((max (next-byte))
	    (val (extract-fixnum *val*)))
	(let ((offset (if (and (>= val 0)
			       (< val max))
			  (nth-offset val)
			  (* max 2))))
	  (set! *code-pointer* (addr+ *code-pointer* offset))
	  (goto interpret))))))

; Put *cont* in *val* (used by primitive-catch)

(define-opcode op/current-cont (lambda ()
  (let* ((key (ensure-space (stack-size)))
	 (cont (preserve-continuation *cont* key copy/preserve)))
    (set! *cont* (restore-continuation cont))
    (set! *val* cont)
    (goto interpret))))

(define-opcode op/with-continuation (lambda ()
  (set! *cont* (restore-continuation (pop)))
  (set! *nargs* 0)
  (goto perform-application 0)))

; only used in the stack underflow template

(define-opcode op/get-cont-from-heap (lambda ()
  (let ((cont (get-continuation-from-heap)))
    (cond ((not (false? cont))
	   (set! *cont* cont)
	   (goto interpret))
	  ((fixnum? *val*)           ; VM returns here
	   (set! *val* (extract-fixnum *val*))
	   return-option/exit)
	  (else
	   (reset-stack-pointer)     ; get a real continuation on the stack
	   (goto raise-exception1 0 *val*))))))

; APPLY - pop the procedure off of the stack, push each argument from the list,
; and then call the procedure.  Tne next byte is the number of arguments that
; have already been pushed on the stack.

(define-opcode op/apply (lambda ()
  (let ((proc (pop)))
    (okay-argument-list
     *val*
     (lambda (length)
       (push-argument-list *val* length (this-byte))
       (set! *val* proc)
       (goto perform-application 0))
     (lambda ()
       (let ((args (pop-args-list null (this-byte))))
	 (goto raise-exception3 0 proc args *val*)))))))
       
; If LIST is a proper list (final cdr is null) then OKAY-CONT is called on the
; length of LIST, otherwise LOSE-CONT is called.

(define (okay-argument-list list okay-cont lose-cont)
    (let loop ((fast list) (len 0) (slow list) (move-slow? #f))
      (cond ((vm-eq? null fast)
	     (okay-cont len))
	    ((not (vm-pair? fast))
             (lose-cont))
	    ((not move-slow?)
	     (loop (vm-cdr fast) (+ len 1) slow #t))
	    ((vm-eq? fast slow)
	     (lose-cont))
	    (else
	     (loop (vm-cdr fast) (+ len 1) (vm-cdr slow) #f)))))

; Push ARGS onto the stack.  LENGTH is the length of the arguments,
; STACK-ARGS is the number of arguments already on the stack.
; If the total number of arguments is greater than the allowed amount,
; the list of extras is copies and pushed.

(define (push-argument-list args length stack-args)
  (let* ((nargs (+ length stack-args))
	 (overflow (if (<= nargs maximum-stack-args)
		       0
		       (- nargs maximum-stack-args)))
	 (rest (push-list args (- length overflow))))
    (cond ((= overflow 0)
	   (set! *nargs* nargs))
	  (else
	   (push (copy-arg-list rest overflow))
	   (set! *nargs* arg-stack-overflow-nargs)))))

; push COUNT elements of LIST onto stack, returning the remainder of LIST

(define (push-list list count)
  (do ((i count (- i 1))
       (l list (vm-cdr l)))
      ((<= i 0) l)
    (push (vm-car l))))

; Assumes list is non-null

(define (copy-arg-list list length)
  (push list)
  (let* ((key (ensure-space (* vm-pair-size length)))
	 (list (pop))
	 (res (vm-cons (vm-car list) null key)))
    (do ((l (vm-cdr list) (vm-cdr l))
	 (last res (let ((next (vm-cons (vm-car l) null key)))
		     (vm-set-cdr! last next)
		     next)))
	((vm-eq? null l)
	 res))))

; Miscellaneous primitive procedures

(define-opcode op/unassigned (lambda ()
  (set! *val* unassigned-marker)
  (goto interpret)))

(define-opcode op/unspecific (lambda ()
  (set! *val* unspecific)
  (goto interpret)))

(define-opcode op/set-exception-handler! (lambda ()
  ;; New exception handler in *val*
  (cond ((not (closure? *val*))
	 (goto raise-exception1 0 *val*))
	(else
	 (set! *exception-handler* *val*)
	 (goto interpret)))))

(define-opcode op/set-interrupt-handlers! (lambda ()
  ;; New interrupt handler vector in *val*
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) interrupt-count))
	 (goto raise-exception1 0 *val*))
	(else
	 (set! *interrupt-handlers* *val*)
	 (goto interpret)))))

(define-opcode op/set-enabled-interrupts! (lambda ()
  ;; New interrupt mask as fixnum in *val*
  (let ((temp *enabled-interrupts*))
    (set! *enabled-interrupts* (extract-fixnum *val*))
    (set! *val* (enter-fixnum temp))
    (goto interpret))))

;;; Procedure call
; The CLOSURE? check must come before the interrupt check, as the interrupt
; code assumes that the correct template is in place.  This delays the
; interrupt handling by a few instructions.

(define (perform-application bytes-consumed)
  (cond ((closure? *val*)
         (set! *env* (closure-env *val*))
         (set-template! (closure-template *val*) 0)
	 (goto poll-for-interrupts))
	(else
	 (goto application-exception bytes-consumed))))

(define (poll-for-interrupts)
  (if (= 0 (bitwise-and *pending-interrupts* *enabled-interrupts*))
      (goto interpret)
      (goto handle-interrupt)))

; Exceptions
; The system reserves enough stack space to allow for an exception at any time.
; If the reserved space is used a gc must be done before the exception handler
; is called.

(define exception-frame-size      ; a continuation plus up to five arguments
  (+ continuation-stack-size 5))  ; one of which is the opcode

(define (start-exception args)
  (let ((key (allow-exception-consing)))
    (push-exception-continuation key)
    (push (enter-fixnum (current-opcode args)))))

(define (raise-exception args)
  (start-exception args)
  (goto raise 0))

(define (raise-exception1 args a1)
  (start-exception args)
  (push a1)
  (goto raise 1))

(define (raise-exception2 args a1 a2)
  (start-exception args)
  (push a1)
  (push a2)
  (goto raise 2))

(define (raise-exception-no-cont2 args a1 a2)
  (push (enter-fixnum (current-opcode args)))
  (push a1)
  (push a2)
  (goto raise 2))

(define (raise-exception3 args a1 a2 a3)
  (start-exception args)
  (push a1)
  (push a2)
  (push a3)
  (goto raise 3))

(define (raise-exception4 args a1 a2 a3 a4)
  (start-exception args)
  (push a1)
  (push a2)
  (push a3)
  (push a4)
  (goto raise 4))

(define no-exceptions? #f)

(define (raise nargs)
  (if no-exceptions?
      (breakpoint "exception check" nargs))
  (set! *nargs* (+ nargs 1))   ; extra arg is the op-code
  (set! *val* *exception-handler*)
  (if (not (closure? *val*))
      (error "exception handler is not a closure"))
  (reserve-exception-space)
  (goto perform-application 0))

; Interrupts

; This is only called when a template is about to be jumped to, so the only
; values that must be saved are *env*, *template*, *nargs*, and
; *enabled-interrupts*.

(define (handle-interrupt)
  (let ((key (ensure-stack-space continuation-stack-size))
	(interrupt (get-highest-priority-interrupt!)))
    (push *template*)
    (push *env*)
    (push (enter-fixnum *nargs*))
    (push (enter-fixnum *enabled-interrupts*))
    (set-template! *interrupt-template* (enter-fixnum 0))
    (push-continuation *code-pointer* (+ *nargs* 4) key)
    (push (enter-fixnum *enabled-interrupts*))
    (set! *nargs* 1)
    (if (not (vm-vector? *interrupt-handlers*))
	(error "interrupt handler is not a vector"))
    (set! *val* (vm-vector-ref *interrupt-handlers* interrupt))
    (set! *enabled-interrupts* 0)   ;Disable all interrupts
    (if (not (closure? *val*))
        (error "interrupt handler is not a closure" interrupt))
    (goto perform-application 0)))

(define-opcode op/return-from-interrupt (lambda ()
  (set! *enabled-interrupts* (extract-fixnum (pop)))
  (set! *nargs*              (extract-fixnum (pop)))
  (set! *env*                (pop))
  (set-template! (pop) 0)
  (goto interpret)))

(define (get-highest-priority-interrupt!)
  (let ((n (bitwise-and *pending-interrupts* *enabled-interrupts*)))
    (let loop ((i 0) (m 1))
      (cond ((= 0 (bitwise-and n m))
	     (loop (+ i 1) (* m 2)))
	    (else
	     (set! *pending-interrupts* (bitwise-and n (bitwise-not m)))
	     i)))))
	 
(define (set-interrupt! interrupt)
  (set! *pending-interrupts*
	(bitwise-ior *pending-interrupts*
		     (ashl 1 interrupt))))

(define (clear-interrupt! interrupt)
  (set! *pending-interrupts*
	(bitwise-and *pending-interrupts*
		     (bitwise-not (ashl 1 interrupt)))))

