; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file interp.scm.

; Interpreter state

(define *template*           (unassigned))
(define *code-pointer*       (unassigned)) ; pointer to next instruction byte
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
  (reset-stack-pointer)
  (reserve-exception-space exception-frame-size universal-key)
  (set-current-env! unspecific)
  (set-template! *interrupt-template*          ; has to be some template
		 (enter-fixnum 0))
  (set! *nargs*              unspecific)        ; interpreter regs
  (set! *val*                unspecific)
  (set! *dynamic-state*      null)
  (set! *exception-handler*  null)
  (set! *interrupt-handlers* null)
  (set! *enabled-interrupts* 0)

  (set! *pending-interrupts* 0)
  unspecific)

(define (trace-registers)
  (let ((pc (code-pointer->pc *code-pointer* *template*)))
    (set-template! (trace-value *template*) pc))
  (set! *val*                (trace-value *val*))
  (set! *dynamic-state*      (trace-value *dynamic-state*))
  (set! *exception-handler*  (trace-value *exception-handler*))
  (set! *interrupt-handlers* (trace-value *interrupt-handlers*))
  (set! *interrupt-template* (trace-value *interrupt-template*))
  (set-current-env! (trace-value (current-env)))
  (trace-stack trace-locations))

(define (set-template! tem pc)
  (set! *template* tem)
  (set! *code-pointer* (addr+ (address-after-header (template-code tem))
			      (extract-fixnum pc))))

(define (code-pointer->pc pointer template)
  (enter-fixnum (addr- pointer
                       (address-after-header (template-code template)))))

(define (current-pc)
  (code-pointer->pc *code-pointer* *template*))

(define (initialize-interpreter)          ;Used only at startup
  (set! *interrupt-template*
	(make-template-containing-ops (enum op ignore-values)
				      (enum op return-from-interrupt))))

(define initial-interpreter-heap-space (op-template-size 2))

; Continuations

(define (push-continuation! code-pointer size key)
  (let ((pc (code-pointer->pc code-pointer *template*)))
    (push-continuation-on-stack *template* pc size key)))

(define (push-exception-continuation!)
  (let ((key (allow-exception-consing exception-frame-size)))
    (push-continuation! *code-pointer* (arguments-on-stack) key)))

(define (pop-continuation!)
  (pop-continuation-from-stack set-template!))

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

; Different ways to call the GC.

(define (ensure-space space)
  (maybe-ensure-space-saving-temp space (enter-fixnum 0)
    (lambda (okay? key temp)
      (if (not okay?)
	  (error "Scheme48 heap overflow"))
      key)))

(define (maybe-ensure-space-saving-temp space temp cont)
  (if (available? space)
      (cont #t (preallocate-space space) temp)
      (let ((temp (collect-saving-temp temp)))
	(cont (available? space) 0 temp))))

; Actual call to GC

(define *minimum-recovered-space* 0)

(define (collect)
  (collect-saving-temp (enter-fixnum 0)))

(define (collect-saving-temp value)
  (begin-collection)
  (trace-registers)
  (trace-impure-areas)
  (let ((value (trace-value value)))
    (do-gc)
    (end-collection)
    (close-untraced-ports!)
    (if (not (available? *minimum-recovered-space*))
	(note-interrupt! (enum interrupt memory-shortage)))
    value))

; INTERPRET is the main instruction dispatch for the interpreter.

;(define trace-instructions? #f)

(define (interpret)
;  (if trace-instructions?
;      (write-instruction *template* (extract-fixnum (current-pc))))
  (let ((op-code (next-byte)))
    ((vector-ref opcode-dispatch op-code))))

;;; Opcodes

(define (uuo) (goto raise-exception 0))

(define opcode-dispatch (make-vector op-count))

(vector+length-fill! opcode-dispatch op-count uuo)

;(define (define-opcode opcode tag)
;  (vector-set! opcode-dispatch opcode tag))

(define-syntax define-opcode
  (syntax-rules ()
    ((define-opcode op-name body ...)
     (vector-set! opcode-dispatch (enum op op-name) (lambda () body ...)))))

; Check number of arguments

(define-opcode check-nargs=
  (if (= *nargs* (next-byte))
      (goto interpret)
      (goto application-exception 1)))

(define-opcode check-nargs>=
  (if (>= *nargs* (next-byte))
      (goto interpret)
      (goto application-exception 1)))

(define-opcode nargs
  (set! *val* (enter-fixnum *nargs*))
  (goto interpret))

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

(define-opcode make-rest-list
  (let* ((min-args (next-byte))
	 (args (if (= *nargs* arg-stack-overflow-nargs)
		   (pop-args-list (pop) (- (- *nargs* 1) min-args))
		   (pop-args-list null  (- *nargs* min-args)))))
    (set! *val* args)
    (set! *nargs* (+ min-args 1))
    (goto interpret)))

; Environment creation
;  The MAKE-ENV instruction adds a env to the local environment.
; It pops values off the stack and stores them into the new env.

(define-opcode make-env
  (let ((key (ensure-stack-space (stack-env-space (this-byte)) ensure-space)))
    (pop-args-into-env (next-byte) key)
    (goto interpret)))

; The above with the environment in the heap and not on the stack.

(define-opcode make-heap-env
  (let ((key (ensure-space (heap-env-space (this-byte)))))
    (pop-args-into-heap-env (next-byte) key)
    (goto interpret)))

(define-opcode pop-env
  (set-current-env! (env-parent (current-env)))
  (goto interpret))

; Literals

(define-opcode literal    ;Load a literal into *val*.
  (set! *val* (next-literal))
  (goto interpret))

; Local variable access and assignment

(define-opcode local      ;Load value of a local.
  (goto finish-local (env-back (current-env) (next-byte)) 2))

(define-opcode local0
  (goto finish-local (current-env) 1))

(define-opcode local1
  (goto finish-local (env-parent (current-env)) 1))

(define-opcode local2
  (goto finish-local (env-parent (env-parent (current-env))) 1))

(define (finish-local env arg-count)
  (set! *val* (env-ref env (next-byte)))
  (cond ((not (vm-eq? *val* unassigned-marker))
	 (goto interpret))
	(else
	 (goto raise-exception arg-count))))

(define-opcode set-local!
  (let ((back (next-byte)))
    (env-set! (env-back (current-env) back)
              (next-byte)
              *val*)
    (set! *val* unspecific)
    (goto interpret)))

; Global variable access

(define-opcode global        ;Load a global variable.
  (let ((location (next-literal)))
    (set! *val* (contents location))
    (cond ((undefined? *val*)           ;unbound or unassigned
	   (goto raise-exception1 1 location))
          (else
           (goto interpret)))))

(define-opcode set-global!
  (let ((location (next-literal)))
    (cond ((vm-eq? (contents location) unbound-marker)
           (goto raise-exception2 1 location *val*))
          (else
           (set-contents! location *val*)
           (set! *val* unspecific)
           (goto interpret)))))

; Stack operation

(define-opcode push       ;Push *val* onto the stack.
  (push *val*)
  (goto interpret))

(define-opcode pop       ;Pop *val* from the stack.
  (set! *val* (pop))
  (goto interpret))

(define-opcode stack-ref
  (set! *val* (stack-ref (next-byte)))
  (goto interpret))

(define-opcode stack-set!
  (stack-set! (next-byte) *val*)
  (goto interpret))

; LAMBDA

(define-opcode closure
  (let ((env (preserve-current-env (ensure-space (current-env-size))))
	(key (ensure-space closure-size)))
    (set! *val* (make-closure (next-literal) env key))
    (goto interpret)))

; Procedure call

(define-opcode call
  (set! *nargs* (this-byte))
  (goto perform-application 0))

; Same as op/call except that the arguments are moved to just above the
; current continuation before the call is made.  For non-tail calls and some
; tail-calls the arguments will already be there.

(define-opcode move-args-and-call
  (set! *nargs* (this-byte))
  (move-args-above-cont! *nargs*)
  (goto perform-application 0))

(define-opcode goto-template
  (set-template! (next-literal) 0)
  (goto interpret))

(define-opcode call-template
  (set! *nargs* (next-byte))  ; needed in case of interrupts
  (set-template! (next-literal) 0)
  (goto poll-for-interrupts))

; Continuation creation and invocation

(define-opcode make-cont   ;Start a non-tail call.
  (let* ((offset (next-offset))
	 (size (next-byte))
	 (key (ensure-stack-space stack-continuation-size ensure-space)))
    (push-continuation! (addr+ *code-pointer* offset) size key)
    (goto interpret)))

(define-opcode return            ;Invoke the continuation.
  (pop-continuation!)
  (goto interpret))

; The arguments are sitting on the stack, with the count in *nargs*.
; This is only used in the closed-compiled version of VALUES.

(define-opcode values
  (goto return-values *nargs*))

; Same as the above, except that the value count is in the instruction stream.
; This is used for in-lining calls to VALUES.

(define-opcode return-values
  (goto return-values (this-byte)))

; NARGS return values are on the stack.  If there is only one value, pop
; it off and do a normal return.  Otherwise, find the actual continuation
; and see if it ignores the values, wants the values, or doesn't know
; anything about them.

(define (return-values nargs)
  (cond ((= nargs 1)
	 (set! *val* (pop))
	 (pop-continuation!)
	 (goto interpret))
	(else
	 (let ((cont (peek-at-current-continuation)))
	   (if (continuation? cont)
	       (goto really-return-values cont nargs)
	       (goto return-exception nargs))))))
	 
; If the next op-code is:
; op/ignore-values - just return, ignoring the return values
; op/call-with-values - remove the continuation containing the consumer
;  (the consumer is the only useful information it contains), and then call
;  the consumer.
; anything else - only one argument was expected so raise an exception.

(define (really-return-values cont nargs)
  (let ((next-op (code-vector-ref (template-code (continuation-template cont))
				  (extract-fixnum (continuation-pc cont)))))
    (cond ((= next-op (enum op ignore-values))
	   (pop-continuation!)
	   (goto interpret))
	  ((= next-op (enum op call-with-values))
	   (skip-current-continuation!)
	   (set! *nargs* nargs)
	   (set! *val* (continuation-ref cont continuation-cells))
	   (goto perform-application 0))
	  (else
	   (goto return-exception nargs)))))

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
;  (set! *code-pointer* (addr+ *code-pointer* 1)) ; move past (enum op call-with-values
;)  (goto interpret))

(define (return-exception nargs)
  (let ((args (if (= nargs arg-stack-overflow-nargs)
		  (pop-args-list (pop) maximum-stack-args)
		  (pop-args-list null nargs))))
    (goto raise-exception1 0 args)))

; This is executed only if the producer returned exactly one value.

(define-opcode call-with-values
  (let ((consumer (pop)))
    (push *val*)
    (set! *val* consumer)
    (set! *nargs* 1)
    (goto perform-application 0)))

; This is just a marker for the code that handles returns.

(define-opcode ignore-values
  (goto interpret))

; IF

(define-opcode jump-if-false
  (let ((offset (next-offset)))
    (cond ((false? *val*)
           (set! *code-pointer* (addr+ *code-pointer* offset))
           (goto interpret))
          (else
           (goto interpret)))))

; Unconditional jump

(define-opcode jump
  (let ((offset (next-offset)))
    (set! *code-pointer* (addr+ *code-pointer* offset))
    (goto interpret)))

; Computed goto
; Goto index is in *val*, the next byte is the number of offsets specified
; The default is to jump to the instruction following the offsets
; The instruction stream looks like
; op/computed-goto max offset0 offset1 ... offsetmax-1 code-for-default...

(define-opcode computed-goto
  (if (not (fixnum? *val*))
      (goto raise-exception1 0 *val*)
      (let ((max (next-byte))
	    (val (extract-fixnum *val*)))
	(let ((offset (if (and (>= val 0)
			       (< val max))
			  (nth-offset val)
			  (* max 2))))
	  (set! *code-pointer* (addr+ *code-pointer* offset))
	  (goto interpret)))))

; Preserve the current continuation and put it in *val*.

(define-opcode current-cont
  (let ((key (ensure-space (current-continuation-size))))
    (set! *val* (current-continuation key))
    (goto interpret)))

(define-opcode with-continuation
  (set-current-continuation! (pop))
  (set! *nargs* 0)
  (goto perform-application 0))

; only used in the stack underflow template

(define-opcode get-cont-from-heap
  (let ((cont (get-continuation-from-heap)))
    (cond ((not (false? cont))
	   (set-current-continuation! cont)
	   (goto interpret))
	  ((fixnum? *val*)           ; VM returns here
	   (set! *val* (extract-fixnum *val*))
	   (enum return-option exit))
	  (else
	   (reset-stack-pointer)     ; get a real continuation on the stack
	   (goto raise-exception1 0 *val*)))))

; APPLY - pop the procedure off of the stack, push each argument from the list,
; and then call the procedure.  Tne next byte is the number of arguments that
; have already been pushed on the stack.

(define-opcode apply
  (let ((proc (pop)))
    (okay-argument-list
     *val*
     (lambda (length)
       (push-argument-list *val* length (this-byte))
       (set! *val* proc)
       (goto perform-application 0))
     (lambda ()
       (let ((args (pop-args-list null (this-byte))))
	 (goto raise-exception3 0 proc args *val*))))))
       
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

(define-opcode unassigned
  (set! *val* unassigned-marker)
  (goto interpret))

(define-opcode unspecific
  (set! *val* unspecific)
  (goto interpret))

(define-opcode set-exception-handler!
  ;; New exception handler in *val*
  (cond ((not (closure? *val*))
	 (goto raise-exception1 0 *val*))
	(else
	 (set! *exception-handler* *val*)
	 (goto interpret))))

(define-opcode set-interrupt-handlers!
  ;; New interrupt handler vector in *val*
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) interrupt-count))
	 (goto raise-exception1 0 *val*))
	(else
	 (set! *interrupt-handlers* *val*)
	 (goto interpret))))

(define-opcode set-enabled-interrupts!
  ;; New interrupt mask as fixnum in *val*
  (let ((temp *enabled-interrupts*))
    (set! *enabled-interrupts* (extract-fixnum *val*))
    (set! *val* (enter-fixnum temp))
    (goto interpret)))

;;; Procedure call
; The CLOSURE? check must come before the interrupt check, as the interrupt
; code assumes that the correct template is in place.  This delays the
; interrupt handling by a few instructions.

(define (perform-application bytes-consumed)
  (cond ((closure? *val*)
         (set-current-env! (closure-env *val*))
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
  (+ stack-continuation-size 5))  ; one of which is the opcode

(define (start-exception args)
  (push-exception-continuation!)
  (push (enter-fixnum (current-opcode args))))

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
;  (if no-exceptions?
;      (breakpoint "exception check" nargs))
  (set! *nargs* (+ nargs 1))   ; extra arg is the op-code
  (set! *val* *exception-handler*)
  (if (not (closure? *val*))
      (error "exception handler is not a closure"))
  (reserve-exception-space exception-frame-size
			   (ensure-stack-space exception-frame-size
					       ensure-space))
  (goto perform-application 0))

; Interrupts

; This is only called when a template is about to be jumped to, so the only
; values that must be saved are *env*, *template*, *nargs*, and
; *enabled-interrupts*.

(define (handle-interrupt)
  (let ((key (ensure-stack-space stack-continuation-size ensure-space))
	(interrupt (get-highest-priority-interrupt!)))
    (push *val*)       ; may be needed for nargs exception
    (push *template*)
    (push (current-env))
    (push (enter-fixnum *nargs*))
    (push (enter-fixnum *enabled-interrupts*))
    (set-template! *interrupt-template* (enter-fixnum 0))
    (push-continuation! *code-pointer* (+ *nargs* 5) key)
    (push (enter-fixnum *enabled-interrupts*))
    (set! *nargs* 1)
    (if (not (vm-vector? *interrupt-handlers*))
	(error "interrupt handler is not a vector"))
    (set! *val* (vm-vector-ref *interrupt-handlers* interrupt))
    (set! *enabled-interrupts* 0)   ;Disable all interrupts
    (if (not (closure? *val*))
        (error "interrupt handler is not a closure" interrupt))
    (goto perform-application 0)))

(define-opcode return-from-interrupt
  (set! *enabled-interrupts* (extract-fixnum (pop)))
  (set! *nargs*              (extract-fixnum (pop)))
  (set-current-env! (pop))
  (set-template!    (pop) 0)
  (set! *val*       (pop))
  (goto interpret))

(define (get-highest-priority-interrupt!)
  (let ((n (bitwise-and *pending-interrupts* *enabled-interrupts*)))
    (let loop ((i 0) (m 1))
      (cond ((= 0 (bitwise-and n m))
	     (loop (+ i 1) (* m 2)))
	    (else
	     (set! *pending-interrupts* (bitwise-and n (bitwise-not m)))
	     i)))))
	 
(define (note-interrupt! interrupt)
  (set! *pending-interrupts*
	(bitwise-ior *pending-interrupts*
		     (ashl 1 interrupt))))

(define (clear-interrupt! interrupt)
  (set! *pending-interrupts*
	(bitwise-and *pending-interrupts*
		     (bitwise-not (ashl 1 interrupt)))))


