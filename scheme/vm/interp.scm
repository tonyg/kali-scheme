; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

;Need to fix the byte-code compiler to make jump etc. offsets from the
;beginning of the instruction.

; This is file interp.scm.

; Interpreter state

(define *template*)		; current template
(define *code-pointer*)		; pointer to next instruction byte
(define *val*)			; last value produced
(define *exception-handlers*)	; vector of procedures, one per opcode
(define *interrupt-handlers*)	; vector of procedures, one per interrupt type

; Two registers used only by the RTS (except for one hack; see GET-CURRENT-PORT
; in prim-io.scm).

(define *current-thread*)	; dynamic state
(define *session-data*)		; session state

; Finalizers

(define *finalizer-alist*)	; list of (<thing> . <procedure>) pairs
(define *finalize-these*)	; list of such pairs that should be executed

; Interrupts

(define *enabled-interrupts*)	; bitmask of enabled interrupts
(define *pending-interrupts*)	; bitmask of pending interrupts
(define *pending-interrupt?*)	; true if an interrupt is pending

(define *interrupted-template*) ; template in place when the most recent
				; interrupt occured - for profiling

(define *interrupt-template*)	; used to return from interrupts
(define *exception-template*)	; used to mark exception continuations

;----------------

(define (clear-registers)
  (reset-stack-pointer false)
  (set-current-env! unspecific-value)
  (set-template! *interrupt-template*          ; has to be some template
		 (enter-fixnum 0))
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
  (set! *exception-template*   (trace-value *exception-template*))
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
  (set! *code-pointer* (address+ (address-after-header code) pc)))

(define (code-pointer->pc pointer template)
  (enter-fixnum (address-difference
		  pointer
		  (address-after-header (template-code template)))))

(define (current-pc)
  (code-pointer->pc *code-pointer* *template*))

(define (initialize-interpreter)          ;Used only at startup
  (set! *interrupt-template*
	(make-template-containing-ops (enum op ignore-values)
				      (enum op return-from-interrupt)))
  (set! *exception-template*
	(make-template-containing-ops (enum op return-from-exception)
				      (enum op false)))) ; ignored

(define initial-interpreter-heap-space
  (+ (op-template-size 2)
     (op-template-size 2)))

;----------------
; Continuations

(define (push-continuation! code-pointer size)
  (let ((pc (code-pointer->pc code-pointer *template*)))
    (push-continuation-on-stack *template* pc size)))

(define (pop-continuation!)
  (pop-continuation-from-stack set-template!))
  
;----------------
; Instruction stream access

(define (code-byte index)
  (fetch-byte (address+ *code-pointer* (+ index 1))))

(define (code-offset index)
  (adjoin-bits (code-byte index)
	       (code-byte (+ index 1))
	       bits-used-per-byte))

(define (get-literal index)
  (template-ref *template* (code-offset index)))

; Return the current op-code.  CODE-ARGS is the number of argument bytes that
; have been used.

(define (current-opcode)
  (code-byte -1))

;----------------
; Different ways to call the GC.

(define (ensure-space space)
  (receive (key temp)
      (ensure-space-saving-temp space (enter-fixnum 0))
    key))

(define (ensure-space-saving-temp space temp)
  (receive (okay? key temp)
      (maybe-ensure-space-saving-temp space temp)
    (if (not okay?)
	(error "Scheme48 heap overflow"))
    (values key temp)))

(define (maybe-ensure-space-saving-temp space temp)
  (if (available? space)
      (values #t (preallocate-space space) temp)
      (let ((temp (collect-saving-temp temp)))
	(if (available? space)
	    (values #t (preallocate-space space) temp)
	    (values #f 0 temp)))))

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
;(define *bad-count* 0)
;(define *i* 0)

(define (interpret code-pointer)
;  (if (and trace-instructions? (> *i* *bad-count*))
;      (write-instruction *template* (extract-fixnum (current-pc)) 1 #f))
;  (set! *i* (+ *i* 1))
  ((vector-ref opcode-dispatch (fetch-byte code-pointer))))

(define (continue bytes-used)
  (set! *code-pointer* (address+ *code-pointer* (+ bytes-used 1)))
  (goto interpret *code-pointer*))

(define (continue-with-value value bytes-used)
  (set! *val* value)
  (goto continue bytes-used))

;----------------
; Exception syntax

; For restartable exceptions the saved code-pointer points to the instruction
; following the offending one.  For all other exceptions it points to the
; offending instruction.
;
; The ...* versions evaluate the exception enum argument, the plain ones
; invoke the enumeration.

(define-syntax raise-exception
  (syntax-rules ()
    ((raise-exception why byte-args stuff ...)
     (raise-exception* (enum exception why) byte-args stuff ...))))

(define-syntax count-exception-args
  (syntax-rules ()
    ((count-exception-args) 0)
    ((count-exception-args arg1 rest ...)
     (+ 1 (count-exception-args rest ...)))))

(define-syntax raise-exception*
  (syntax-rules ()
    ((raise-exception* why byte-args arg1 ...)
     (begin
       (push-exception-continuation! why (+ byte-args 1)) ; add 1 for the opcode
       (push arg1) ...
       (goto raise (count-exception-args arg1 ...))))))

;----------------
; Opcodes

(define (uuo)
  (raise-exception unimplemented-instruction 0))

(define opcode-dispatch (make-vector op-count))

(vector+length-fill! opcode-dispatch op-count uuo)

(define-syntax define-opcode
  (syntax-rules ()
    ((define-opcode op-name body ...)
     (vector-set! opcode-dispatch (enum op op-name) (lambda () body ...)))))

;----------------
; Literals
; Loaded from *template* into *val*, using either a one-byte or two-byte index.

(define-opcode literal    ;Load a literal into *val*.
  (goto continue-with-value
	(get-literal 0)
	2)) ; offset

(define-opcode small-literal
  (goto continue-with-value
	(template-ref *template* (code-byte 0))
	2)) ; byte + wasted byte

;----------------
; Environment creation
;  The MAKE-ENV instruction adds a env to the local environment.
; It pops values off the stack and stores them into the new env.

(define-opcode make-env
  (pop-args-into-env (code-offset 0))
  (goto continue 2)) ; offset

; Local variable access and assignment

(define-opcode local      ;Load value of a local.
  (goto finish-local (env-back (current-env) (code-byte 0)) 1))

(define-opcode local0
  (goto finish-local (current-env) 0))

(define-opcode push-local0
  (push *val*)
  (goto finish-local (current-env) 1))

(define-opcode local0-push
  (set! *val* (env-ref (current-env) (code-byte 0)))
  (if (not (vm-eq? *val* unassigned-marker))
      (begin
	(push *val*)
	(goto continue 2))
      (raise-exception unassigned-local 2)))

(define-opcode local1
  (goto finish-local (env-parent (current-env)) 0))

(define-opcode local2
  (goto finish-local (env-parent (env-parent (current-env))) 0))

(define (finish-local env arg-count)
  (set! *val* (env-ref env (code-byte arg-count)))
  (if (not (vm-eq? *val* unassigned-marker))
      (goto continue (+ arg-count 1))
      (raise-exception unassigned-local (+ arg-count 1))))

(define-opcode big-local
  (let ((back (code-byte 0)))
    (set! *val* (env-ref (env-back (current-env) back)
			 (code-offset 1)))
    (if (not (vm-eq? *val* unassigned-marker))
	(goto continue 3) ; byte + offset
	(raise-exception unassigned-local 3))))

(define-opcode set-local!
  (let ((back (code-byte 0)))
    (env-set! (env-back (current-env) back)
              (code-offset 1)
              *val*)
    (set! *val* unspecific-value)
    (goto continue 3))) ; byte + offset

;----------------
; Global variable access

(define-opcode global        ;Load a global variable.
  (let ((location (get-literal 0)))
    (set! *val* (contents location))
    (if (undefined? *val*)           ;unbound or unassigned
	(raise-exception undefined-global 2 location)
	(goto continue 2)))) ; offset

(define-opcode set-global!
  (let ((location (get-literal 0)))
    (cond ((vm-eq? (contents location) unbound-marker)
           (raise-exception undefined-global 2 location *val*))
          (else
           (set-contents! location *val*)
	   (goto continue-with-value
		 unspecific-value
		 2))))) ; offset

;----------------
; Stack operation

(define-opcode push	;Push *val* onto the stack.
  (push *val*)
  (goto continue 0))

(define-opcode pop	;Pop *val* from the stack.
  (goto continue-with-value
	(pop)
	0))

(define-opcode stack-ref
  (goto continue-with-value
	(stack-ref (code-byte 0))
	1))

(define-opcode stack-set!
  (stack-set! (code-byte 0) *val*)
  (goto continue 1))

;----------------
; LAMBDA

(define-opcode closure
  (let ((env (preserve-current-env (ensure-space (current-env-size)))))
    (receive (key env)
	(ensure-space-saving-temp closure-size env)
      (goto continue-with-value
	    (make-closure (get-literal 0) env key)
	    2))))

(define-opcode flat-closure
  (let* ((count (code-byte 0))
	 (key (ensure-space (+ (vm-vector-size count) closure-size)))
	 (new-env (vm-make-vector count key)))
    (do ((i 0 (+ i 1)))
        ((= i count))
      (let ((env (env-back (current-env) (code-byte (+ (* i 2) 1)))))
        (vm-vector-set! new-env i (vm-vector-ref env (code-byte (+ (* i 2) 2))))))
    (goto continue-with-value
	  (make-closure (get-literal (+ (* count 2) 1)) new-env key)
	  (+ (* count 2) 3))))

;----------------
; Continuation creation and invocation

(define-opcode make-cont   ;Start a non-tail call.
  (push-continuation! (address+ *code-pointer*
				(code-offset 0))
		      (code-byte 2))
  (goto continue 3))

(define-opcode make-big-cont   ;Start a non-tail call.
  (push-continuation! (address+ *code-pointer*
				(code-offset 0))
		      (code-offset 2))
  (goto continue 4))

(define-opcode return            ;Invoke the continuation.
  (pop-continuation!)
  (goto interpret *code-pointer*))

; This is only used in the closed-compiled version of VALUES.
; Stack is: arg0 arg1 ... argN rest-list N+1 total-arg-count.
; If REST-LIST is non-empty then there are at least two arguments on the stack.

(define-opcode closed-values
  (let* ((nargs (extract-fixnum (pop)))
	 (stack-nargs (extract-fixnum (pop)))
	 (rest-list (pop)))
    (goto return-values stack-nargs rest-list (- nargs stack-nargs))))

; Same as the above, except that the value count is in the instruction stream
; and all of the arguments are on the stack.
; This is used for in-lining calls to VALUES.

(define-opcode values
  (goto return-values (code-offset 0) null 0))

; STACK-NARGS return values are on the stack.  If there is only one value, pop
; it off and do a normal return.  Otherwise, find the actual continuation
; and see if it ignores the values, wants the values, or doesn't know
; anything about them.

(define (return-values stack-nargs list-args list-arg-count)
  (cond ((= stack-nargs 1)  ; if list-arg-count > 0 then stack-nargs > 1
	 (set! *val* (pop))
	 (pop-continuation!)
	 (goto interpret *code-pointer*))
	(else
	 (let ((cont (peek-at-current-continuation)))
	   (if (continuation? cont)
	       (goto really-return-values
		     cont stack-nargs list-args list-arg-count)
	       (goto return-exception stack-nargs list-args))))))
	 
; If the next op-code is:
; op/ignore-values - just return, ignoring the return values
; op/call-with-values - remove the continuation containing the consumer
;  (the consumer is the only useful information it contains), and then call
;  the consumer.
; anything else - only one argument was expected so raise an exception.

(define (really-return-values cont stack-nargs list-args list-arg-count)
  (let ((next-op (code-vector-ref (template-code (continuation-template cont))
				  (extract-fixnum (continuation-pc cont)))))
    (cond ((= next-op (enum op ignore-values))
	   (pop-continuation!)
	   (goto interpret *code-pointer*))
	  ((= next-op (enum op call-with-values))
	   (skip-current-continuation!)
	   (set! *val* (continuation-ref cont continuation-cells))
	   (goto perform-application-with-rest-list stack-nargs
		                                    list-args
						    list-arg-count))
	  (else
	   (goto return-exception stack-nargs list-args)))))

; This would avoid the need for the consumer to be a closure.  It doesn't
; work because the NARGS check (which would be the next instruction to be
; executed) assumes that a closure has just been called.

;(define (do-call-with-values nargs)
;  (cond ((address= *cont* *bottom-of-stack*)
;	 (restore-from-continuation (continuation-cont *cont*))
;	 (set-continuation-cont! *bottom-of-stack* *cont*)
;	 (set! *cont* *bottom-of-stack*))
;	(else
;	 (restore-from-continuation cont)))
;  (set! *code-pointer* (address+ *code-pointer* 1)) ; move past (enum op call-with-values
;)  (goto interpret *code-pointer*))

(define (return-exception stack-nargs list-args)
  (let ((args (pop-args->list* list-args stack-nargs)))
    (raise-exception wrong-number-of-arguments -1 false args))) ; no next opcode

; This is executed only if the producer returned exactly one value.

(define-opcode call-with-values
  (let ((consumer (pop)))
    (push *val*)
    (set! *val* consumer)
    (goto perform-application 1)))

; This is just a marker for the code that handles returns.

(define-opcode ignore-values
  (goto continue 0))

;----------------
; Preserve the current continuation and put it in *val*.

(define-opcode current-cont
  (let ((key (ensure-space (current-continuation-size))))
    (goto continue-with-value
	  (current-continuation key)
	  0)))

(define-opcode with-continuation
  (set-current-continuation! (pop))
  (goto perform-application 0))

; only used in the stack underflow template

(define-opcode get-cont-from-heap
  (let ((cont (get-continuation-from-heap)))
    (cond ((continuation? cont)
	   (set-current-continuation! cont)
	   (goto continue 0))
	  ((and (false? cont)
		(fixnum? *val*))           ; VM returns here
	   (set! *val* (extract-fixnum *val*))
	   (enum return-option exit))
	  (else
	   (set-current-continuation! false)
	   (raise-exception wrong-type-argument 0 *val* cont)))))

;----------------
; Control flow

; IF

(define-opcode jump-if-false
  (cond ((false? *val*)
	 (set! *code-pointer*
	       (address+ *code-pointer*
			 (code-offset 0)))
	 (goto interpret *code-pointer*))
	(else
	 (goto continue 2))))

; Unconditional jump

(define-opcode jump
  (set! *code-pointer*
	(address+ *code-pointer*
		  (code-offset 0)))
  (goto interpret *code-pointer*))

; Computed goto
; Goto index is in *val*, the next byte is the number of offsets specified
; The default is to jump to the instruction following the offsets
; The instruction stream looks like
; op/computed-goto max offset0 offset1 ... offsetmax-1 code-for-default...

(define-opcode computed-goto
  (if (not (fixnum? *val*))
      (raise-exception wrong-type-argument -1 *val*)  ; back up over opcode
      (let ((max (code-byte 0))
	    (val (extract-fixnum *val*)))
	(let ((offset (if (and (>= val 0)
			       (< val max))
			  (code-offset (+ (* val 2) 1))
			  (+ (* max 2) 2))))
	  (set! *code-pointer* (address+ *code-pointer* offset))
	  (goto interpret *code-pointer*)))))

;----------------
; Miscellaneous primitive procedures

(define-opcode unassigned
  (goto continue-with-value
	unassigned-marker
	0))

(define-opcode unspecific
  (goto continue-with-value
	unspecific-value
	0))

;----------------
; Exceptions

(define-opcode set-exception-handlers!
  ;; New exception handlers in *val*
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) op-count))
	 (raise-exception wrong-type-argument 0 *val*))
	(else
	 (let ((temp *exception-handlers*))
	   (set! *exception-handlers* *val*)
	   (goto continue-with-value
		 temp
		 0)))))

; The system reserves enough stack space to allow for an exception at any time.
; If the reserved space is used a gc must be done before the exception handler
; is called.

; New exception handlers in *val*.

(define-opcode set-exception-handlers!
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) op-count))
	 (raise-exception wrong-type-argument 0 *val*))
	(else
	 (let ((temp *exception-handlers*))
	   (set! *exception-handlers* *val*)
	   (goto continue-with-value
		 temp
		 0)))))

; The current opcode and the exception are pushed as arguments to the handler.
; INSTRUCTION-SIZE is the size of the current instruction and is used to jump
; to the next instruction when returning.  The exception is saved in the
; continuation for use in debugging.

(define (push-exception-continuation! exception instruction-size)
  (let ((opcode (current-opcode)))
    (push (enter-fixnum instruction-size))
    (push (enter-fixnum exception))
    (push *template*)
    (push (current-pc))
    (set-template! *exception-template* (enter-fixnum 0))
    (push-continuation! *code-pointer* (arguments-on-stack))
    (push (enter-fixnum opcode))
    (push (enter-fixnum exception))))

(define-opcode return-from-exception
  (let* ((pc (extract-fixnum (pop)))
	 (template (pop))
	 (exception (pop))	; ignored
	 (size (extract-fixnum (pop))))
    (set-template! template (enter-fixnum (+ pc size)))
    (goto interpret *code-pointer*)))

;(define no-exceptions? #t)

(define (raise nargs)
;  (let ((opcode (enumerand->name (extract-fixnum (stack-ref (+ nargs 1))) op))
;	(why (enumerand->name (extract-fixnum (stack-ref nargs)) exception)))
;    (if (and no-exceptions?
;             (not (and (eq? 'write-char opcode)
;                       (eq? 'buffer-full/empty why))))
;        (breakpoint "exception check ~A ~A ~A" opcode why nargs)))
  ;; try to be helpful when all collapses
  (let* ((opcode (extract-fixnum (stack-ref (+ nargs 1))))
	 (lose (lambda (message)
		(let ((why (extract-fixnum (stack-ref nargs))))
		  (write-string "Template UIDs: " (current-error-port))
		  (report-continuation-uids *template* (current-error-port))
		  (newline (current-error-port))
		  (if (and (eq? why (enum exception undefined-global))
			   (fixnum? (location-id (stack-ref (- nargs 1)))))
		      (error message opcode why
			     (extract-fixnum (location-id (stack-ref (- nargs 1)))))
		      (error message opcode why))))))
    (if (not (vm-vector? *exception-handlers*))
	(lose "exception-handlers is not a vector"))
    (set! *val* (vm-vector-ref *exception-handlers* opcode))
    (if (not (closure? *val*))
	(lose "exception handler is not a closure"))
    (goto call-exception-handler (+ nargs 2) opcode)))


