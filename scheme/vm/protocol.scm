; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This is file interp.scm.

; Interpreter state

(define *template*           (unspecific))
(define *code-pointer*       (unspecific)) ; pointer to next instruction byte
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
  (set! *val*                unspecific-value) ; interpreter registers
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
  (trace-stack trace-locations! trace-stob-contents!))

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

(define (pc-at-start-of-current-opcode args)
  (code-pointer->pc (- *code-pointer* (+ args 1)) *template*))

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

(define (collect)
  (collect-saving-temp (enter-fixnum 0)))

(define do-gc
  (make-gc vm-cons vm-car vm-cdr vm-set-car! vm-set-cdr!))

(define (collect-saving-temp value)
  (let ((pc (current-pc)))
    (begin-collection)
    (trace-registers)
    (trace-impure-areas)
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
	(raise-exception undefined-global 1 location)
	(goto interpret))))

(define-opcode set-global!
  (let ((location (next-literal)))
    (cond ((vm-eq? (contents location) unbound-marker)
           (raise-exception undefined-global 1 location *val*))
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
    (if (not (vm-vector? *exception-handlers*))
	(lose "exception-handlers is not a vector"))
    (set! *val* (vm-vector-ref *exception-handlers* (current-opcode args)))
    (if (not (closure? *val*))
	(lose "exception handler is not a closure"))
    (reserve-exception-space exception-frame-size
			     (ensure-stack-space exception-frame-size
						 ensure-space))
    (goto perform-application
	  (+ nargs 2)         ; extras are opcode and exception
	  0)))

