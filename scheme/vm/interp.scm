; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This is file interp.scm.

; Interpreter state

(define *template*           (unspecific))
(define *code-pointer*       (unspecific)) ; pointer to next instruction byte
(define *nargs*              (unspecific))
(define *val*                (unspecific))
(define *enabled-interrupts* (unspecific))
(define *exception-handlers* (unspecific))
(define *interrupt-handlers* (unspecific))
(define *current-thread*     (unspecific))
(define *session-data*       (unspecific))

(define *finalizer-alist*    (unspecific))
(define *finalize-these*     (unspecific))

; Miscellaneous registers

(define *pending-interrupts*   (unspecific))
(define *pending-interrupt?*   (unspecific))
(define *interrupt-template*   (unspecific))
(define *interrupted-template* (unspecific))

(define (clear-registers)
  (reset-stack-pointer false)
  (reserve-exception-space exception-frame-size universal-key)
  (set-current-env! unspecific-value)
  (set-template! *interrupt-template*          ; has to be some template
		 (enter-fixnum 0))
  (set! *nargs*              unspecific-value)        ; interpreter regs
  (set! *val*                unspecific-value)
  (set! *current-thread*     null)
  (set! *session-data*       null)
  (set! *exception-handlers* null)
  (set! *interrupt-handlers* null)
  (set! *enabled-interrupts* 0)
  (set! *finalizer-alist*    null)
  (set! *finalize-these*     null)

  (set! *pending-interrupts* 0)
  (set! *pending-interrupt?* #f)
  (set! *interrupted-template* false)
  unspecific-value)

(define (trace-registers)
  (set! *template*             (trace-value *template*))
  (set! *val*                  (trace-value *val*))
  (set! *current-thread*       (trace-value *current-thread*))
  (set! *session-data*         (trace-value *session-data*))
  (set! *exception-handlers*   (trace-value *exception-handlers*))
  (set! *interrupt-handlers*   (trace-value *interrupt-handlers*))
  (set! *interrupt-template*   (trace-value *interrupt-template*))
  (set! *interrupted-template* (trace-value *interrupted-template*))
  (set! *finalize-these*       (trace-value *finalize-these*))
  (set-current-env! (trace-value (current-env)))
  (trace-io trace-value)
  (trace-stack trace-locations! trace-stob-contents! trace-value))

(define (set-template! tem pc)
  (set! *template* tem)
  (set-code-pointer! (template-code tem) (extract-fixnum pc)))

(define (set-code-pointer! code pc)
  (set! *code-pointer* (addr+ (address-after-header code) pc)))

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

(define (push-exception-continuation! byte-args exception)
  (let ((key (allow-exception-consing exception-frame-size)))
    (push-continuation! *code-pointer* (arguments-on-stack) key)
    (push (enter-fixnum (current-opcode byte-args)))
    (push (enter-fixnum exception))))

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
  (template-ref *template* (next-offset)))

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

(define (collect)
  (collect-saving-temp (enter-fixnum 0)))

(define (collect-saving-temp value)
  (let ((pc (current-pc)))
    (begin-collection)
    (trace-registers)
    (let ((value (trace-value value)))
      (receive (okay sickly)
	  (do-gc *finalizer-alist*)
	(set! *finalizer-alist* okay)
	(set! *finalize-these* (vm-append! sickly *finalize-these*))
	(end-collection)
	(close-untraced-channels!)
	(set-template! *template* pc)
	(note-interrupt! (enum interrupt post-gc))
	value))))

(define (vm-append! l1 l2)
  (if (vm-eq? l1 null)
      l2
      (let ((last-pair (let loop ((l l1))
			 (if (vm-eq? (vm-cdr l) null)
			     l
			     (loop (vm-cdr l))))))
	(vm-set-cdr! last-pair l2)
	l1)))

; INTERPRET is the main instruction dispatch for the interpreter.

;(define trace-instructions? #f)

(define (interpret)
;  (if (and trace-instructions? (> *i* 749))
;      (write-instruction *template* (extract-fixnum (current-pc)) 1 #f))
;  (set! *i* (+ *i* 1))
  (let ((op-code (next-byte)))
    ((vector-ref opcode-dispatch op-code))))

;;; Exceptions

(define-syntax raise-exception
  (syntax-rules ()
    ((raise-exception why stuff ...)
     (raise-exception* (enum exception why) stuff ...))))

(define-syntax raise-exception*
  (syntax-rules ()
    ((raise-exception why bytes)
     (begin (push-exception-continuation! bytes why)
	    (goto raise 0 bytes)))
    ((raise-exception why bytes arg1)
     (begin (push-exception-continuation! bytes why)
	    (push arg1)
	    (goto raise 1 bytes)))
    ((raise-exception why bytes arg1 arg2)
     (begin (push-exception-continuation! bytes why)
	    (push arg1)
	    (push arg2)
	    (goto raise 2 bytes)))
    ((raise-exception why bytes arg1 arg2 arg3)
     (begin (push-exception-continuation! bytes why)
	    (push arg1)
	    (push arg2)
	    (push arg3)
	    (goto raise 3 bytes)))
    ((raise-exception why bytes arg1 arg2 arg3 arg4)
     (begin (push-exception-continuation! bytes why)
	    (push arg1)
	    (push arg2)
	    (push arg3)
	    (push arg4)
	    (goto raise 4 bytes)))
    ((raise-exception why bytes arg1 arg2 arg3 arg4 arg5)
     (begin (push-exception-continuation! bytes why)
	    (push arg1)
	    (push arg2)
	    (push arg3)
	    (push arg4)
	    (push arg5)
	    (goto raise 5 bytes)))))

;;; Opcodes

(define (uuo)
  (raise-exception unimplemented-instruction 0))

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
      (goto application-exception 1 (enum exception wrong-number-of-arguments))))

(define-opcode check-nargs>=
  (if (>= *nargs* (next-byte))
      (goto interpret)
      (goto application-exception 1 (enum exception wrong-number-of-arguments))))

; This requires too many changes to the run-time system for now.
;(define (nargs-check template nargs)
;  (let* ((code (template-code template))
;         (first-op (code-vector-ref code 0)))
;    (cond ((= first-op (enum op check-nargs=))
;           (if (= nargs (code-vector-ref code 1))
;               2                        ; we used two bytes
;               -1))                     ; wrong number of args
;          ((= first-op (enum op check-nargs>=))
;           (if (>= nargs (code-vector-ref code 1))
;               2                        ; we used two bytes
;               -1))                     ; wrong number of args
;	  (else 0))))			; they'll accept any number

(define-opcode nargs
  (set! *val* (enter-fixnum *nargs*))
  (goto interpret))

; Pop off all arguments into a list and raise an exception.  *val* is the
; procedure being called.  BYTE-ARGS is the number of bytes of the instruction
; stream that the instruction has consumed.

(define (application-exception byte-args exception)
  (let ((args (if (= *nargs* arg-stack-overflow-nargs)
		  (pop-args-list (pop) maximum-stack-args)
		  (pop-args-list null *nargs*))))
    (goto raise-exception-no-cont2 byte-args exception *val* args)))

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
; Loaded from *template* into *val*, using either a one-byte or two-byte index.

(define-opcode literal    ;Load a literal into *val*.
  (set! *val* (next-literal))
  (goto interpret))

(define-opcode small-literal
  (set! *val* (template-ref *template* (this-byte)))
  (set! *code-pointer* (addr+ *code-pointer* 2))  ; eat unused byte
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
  (if (not (vm-eq? *val* unassigned-marker))
      (goto interpret)
      (raise-exception unassigned-local arg-count)))

(define-opcode set-local!
  (let ((back (next-byte)))
    (env-set! (env-back (current-env) back)
              (next-byte)
              *val*)
    (set! *val* unspecific-value)
    (goto interpret)))

; Global variable access

(define-opcode global        ;Load a global variable.
  (let ((location (next-literal)))
    (set! *val* (contents location))
    (if (undefined? *val*)           ;unbound or unassigned
	(raise-exception undefined-global 2 location)
	(goto interpret))))

(define-opcode set-global!
  (let ((location (next-literal)))
    (cond ((vm-eq? (contents location) unbound-marker)
           (raise-exception undefined-global 2 location *val*))
          (else
           (set-contents! location *val*)
           (set! *val* unspecific-value)
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

(define-opcode flat-closure
  (let* ((count (next-byte))
	 (key (ensure-space (+ (vm-vector-size count) closure-size)))
	 (new-env (vm-make-vector count key)))
    (do ((i 0 (+ i 1)))
        ((= i count))
      (let ((env (env-back (current-env) (next-byte))))
        (vm-vector-set! new-env i (vm-vector-ref env (next-byte)))))
    (set! *val* (make-closure (next-literal) new-env key))
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
  (set-template! (next-literal) (enter-fixnum 0))
  (goto interpret))

(define-opcode call-template
  (set! *nargs* (next-byte))  ; needed in case of interrupts
  (set-template! (next-literal) (enter-fixnum 0))
  (if (pending-interrupt?)
      (goto handle-interrupt)
      (goto interpret)))

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
    (raise-exception wrong-number-of-arguments 0 args)))

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
      (raise-exception wrong-type-argument 0 *val*)
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
    (cond ((continuation? cont)
	   (set-current-continuation! cont)
	   (goto interpret))
	  ((and (false? cont)
		(fixnum? *val*))           ; VM returns here
	   (set! *val* (extract-fixnum *val*))
	   (enum return-option exit))
	  (else
	   (set-current-continuation! false)
	   (raise-exception wrong-type-argument 0 *val* cont)))))

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
	 (raise-exception wrong-type-argument 0 proc args *val*))))))
       
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
  (set! *val* unspecific-value)
  (goto interpret))

(define-opcode set-exception-handlers!
  ;; New exception handlers in *val*
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) op-count))
	 (raise-exception wrong-type-argument 0 *val*))
	(else
	 (let ((temp *exception-handlers*))
	   (set! *exception-handlers* *val*)
	   (set! *val* temp)
	   (goto interpret)))))

; Procedure call

; The CLOSURE? and number-of-arguments checks must come before the interrupt
; check because the interrupt code assumes that the correct template is in
; place.  This delays the handling of interrupts by a few instructions; it
; shouldn't matter.

(define (perform-application bytes-consumed)
  (cond ((closure? *val*)
         (set-current-env! (closure-env *val*))
         (set-template! (closure-template *val*) 0)
	 (if (pending-interrupt?)
	     (goto handle-interrupt)
	     (goto interpret)))
	(else
	 (goto application-exception bytes-consumed (enum exception bad-procedure)))))

; This requires too many changes to the run-time system for now.
;(define (perform-application bytes-consumed)
;  (if (closure? *val*)
;      (let* ((template (closure-template *val*))
;             (skip (nargs-check template *nargs*)))
;        (cond ((<= 0 skip)
;               (set! *template* template)
;               (set-current-env! (closure-env *val*))
;               (set-code-pointer! (template-code template) skip)
;               (if (interrupt-pending?)
;                   (goto handle-interrupt -1)
;                   (goto interpret)))
;              (else
;               (goto application-exception bytes-consumed
;                     (enum exception wrong-number-of-arguments)))))
;      (goto application-exception bytes-consumed
;            (enum exception bad-procedure))))

; Exceptions
; The system reserves enough stack space to allow for an exception at any time.
; If the reserved space is used a gc must be done before the exception handler
; is called.

(define exception-frame-size      ; a continuation plus up to five arguments
  (+ stack-continuation-size 5))  ; one of which is the exception

(define (raise-exception-no-cont2 args exception a1 a2)
  (push (enter-fixnum (current-opcode args)))
  (push (enter-fixnum exception))
  (push a1)
  (push a2)
  (goto raise 2 args))

;(define no-exceptions? #t)

(define (raise nargs args)
;  (let ((opcode (enumerand->name (extract-fixnum (stack-ref (+ nargs 1))) op))
;	(why (enumerand->name (extract-fixnum (stack-ref nargs)) exception)))
;    (if (and no-exceptions?
;             (not (and (eq? 'write-char opcode)
;                       (eq? 'buffer-full/empty why))))
;        (breakpoint "exception check ~A ~A ~A" opcode why nargs)))
  ;; try to be helpful when all collapses
  (let ((lose (lambda (message)
		(let ((opcode (extract-fixnum (stack-ref (+ nargs 1))))
		      (why (extract-fixnum (stack-ref nargs))))
		  (write-string "Template UIDs: " (current-error-port))
		  (report-continuation-uids *template* (current-error-port))
		  (newline (current-error-port))
		  (if (and (eq? why (enum exception undefined-global))
			   (fixnum? (location-id (stack-ref (- nargs 1)))))
		      (error message opcode why
			     (extract-fixnum (location-id (stack-ref (- nargs 1)))))
		      (error message opcode why))))))
    (set! *nargs* (+ nargs 2))   ; extras are opcode and exception
    (if (not (vm-vector? *exception-handlers*))
	(lose "exception-handlers is not a vector"))
    (set! *val* (vm-vector-ref *exception-handlers* (current-opcode args)))
    (if (not (closure? *val*))
	(lose "exception handler is not a closure"))
    (reserve-exception-space exception-frame-size
			     (ensure-stack-space exception-frame-size
						 ensure-space))
    (goto perform-application 0)))

