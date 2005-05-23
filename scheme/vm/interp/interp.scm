; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

;Need to fix the byte-code compiler to make jump etc. offsets from the
;beginning of the instruction.  (Later: why?)

; Interpreter state

(define *code-pointer*)		; pointer to next instruction byte
(define *val*)			; last value produced
(define *exception-handlers*)	; vector of procedures, one per opcode
(define *interrupt-handlers*)	; vector of procedures, one per interrupt type

; These are set when calls or returns occur.  We use them to figure out what
; the current program counter is.
(define *last-code-called*)
(define *last-code-pointer-resumed*)

; Two registers used only by the RTS (except for one hack; see GET-CURRENT-PORT
; in prim-io.scm).

(define *current-thread*)	; dynamic state
(define *session-data*)		; session state

; Finalizers

(define *finalizer-alist*)	; list of (<thing> . <procedure>) pairs
(define *finalize-these*)	; list of such pairs that should be executed

(define *gc-in-trouble?* #f)	; says whether the GC is running out of space

; Interrupts

(define *enabled-interrupts*)	; bitmask of enabled interrupts
(define s48-*pending-interrupt?*) ; true if an interrupt is pending

(define *interrupted-template*) ; template in place when the most recent
				; interrupt occured - for profiling

(define *interrupted-byte-call-return-code*)	; used to return from interrupts
(define *interrupted-native-call-return-code*)	; used to return from interrupts
(define *poll-interrupt-return-code*)           ; used to return from interrupts
(define *exception-return-code*)	; used to mark exception continuations
(define *native-exception-return-code*)	; used to mark native exception continuations
(define *call-with-values-return-code*)	; for call-with-values opcode

; These are referred to from other modules.

(define (val) *val*)
(define (set-val! val) (set! *val* val))
(define (code-pointer) *code-pointer*)
(define (current-thread) *current-thread*)

; The current proposal is stored in the current thread.

(define (current-proposal)
  (record-ref (current-thread) 3))

(define (set-current-proposal! proposal)
  (record-set! (current-thread) 3 proposal))

; For saving native code registers accross a GC

(define s48-*nc-template*)
(define s48-*nc-environment*)

;----------------

(define (clear-registers)
  (reset-stack-pointer false)
  (set-code-pointer! *interrupted-byte-call-return-code* 0)
  (set-code-pointer! *interrupted-native-call-return-code* 0)
  (set-code-pointer! *poll-interrupt-return-code* 0)
  (set! *last-code-pointer-resumed* *code-pointer*)
  (set! *val*                unspecific-value)
  (set! *current-thread*     null)
  (shared-set! *session-data*       null)
  (shared-set! *exception-handlers* null)
  (shared-set! *interrupt-handlers* null)
  (set! *enabled-interrupts* 0)
  (shared-set! *finalizer-alist*    null)
  (set! *finalize-these*	    null)

  (pending-interrupts-clear!)
  (set! s48-*pending-interrupt?* #f)
  (set! *os-signal-ring-start* 0)
  (set! *os-signal-ring-ready* 0)
  (set! *os-signal-ring-end* 0)
  (set! *interrupted-template* false)
  (set! s48-*nc-template* false)
  (set! s48-*nc-environment* false)
  unspecific-value)

(define (s48-initialize-shared-registers! s-d e-h i-h f-a)
  (set! *session-data*       s-d)
  (set! *exception-handlers* e-h)
  (set! *interrupt-handlers* i-h)
  (set! *finalizer-alist*    f-a))

(define *saved-pc*)	; for saving the pc across GC's

(add-gc-root!
  (lambda ()
    ; headers may be busted here...
    (receive (code pc)
	(current-code+pc)
      (set! *saved-pc* pc)
      (set! *last-code-called* (s48-trace-value code)))

    (set! *val*                   (s48-trace-value *val*))
    (set! *current-thread*        (s48-trace-value *current-thread*))
    (set! *interrupted-byte-call-return-code*
          (s48-trace-value *interrupted-byte-call-return-code*))
    (set! *interrupted-native-call-return-code*
          (s48-trace-value *interrupted-native-call-return-code*))
    (set! *poll-interrupt-return-code*
          (s48-trace-value *poll-interrupt-return-code*))
    (set! *exception-return-code* (s48-trace-value *exception-return-code*))
    (set! *native-exception-return-code* (s48-trace-value *native-exception-return-code*))
    (set! *call-with-values-return-code*
	  (s48-trace-value *call-with-values-return-code*))
    (set! *interrupted-template*  (s48-trace-value *interrupted-template*))
    (set! s48-*nc-template*       (s48-trace-value s48-*nc-template*))
    (set! s48-*nc-environment*    (s48-trace-value s48-*nc-environment*))

    (shared-set! *session-data*
		 (s48-trace-value (shared-ref *session-data*)))
    (shared-set! *exception-handlers*
		 (s48-trace-value (shared-ref *exception-handlers*)))
    (shared-set! *interrupt-handlers*
		 (s48-trace-value (shared-ref *interrupt-handlers*)))
    (shared-set! *finalize-these*
		 (s48-trace-value (shared-ref *finalize-these*)))

    (trace-finalizer-alist!)
    
    ; These could be moved to the appropriate modules.
    (trace-io s48-trace-value)
    (trace-channel-names s48-trace-value)))

(add-post-gc-cleanup!
  (lambda (major? in-trouble?)
    (set-code-pointer! *last-code-called* *saved-pc*)
    (set! *last-code-pointer-resumed* *code-pointer*)
    (partition-finalizer-alist!)
    (close-untraced-channels! s48-extant? s48-trace-value)
    (set! *gc-in-trouble?* in-trouble?)
    (note-interrupt! (if major?
			 (enum interrupt post-major-gc)
			 (enum interrupt post-minor-gc)))))

;----------------
; Dealing with the list of finalizers.
;
; Pre-gc:
;  Trace the contents of every finalizer object, updating them in oldspace.
;  If any contains a pointer to itself, quit and trace it normally.
;  If any have already been copied, ignore it.
; Post-gc:
;  Check each to see if each has been copied.  If not, copy it.  There is
;  no need to trace any additional pointers.

; Walk down the finalizer alist, tracing the procedures and the contents of
; the things.

(define (trace-finalizer-alist!)
  (let loop ((alist (shared-ref *finalizer-alist*)))
    (if (not (vm-eq? alist null))
	(let* ((pair (vm-car alist)))
	  (if (not (s48-extant? (vm-car pair)))  ; if not already traced
	      (s48-trace-stob-contents! (vm-car pair)))
	  (vm-set-cdr! pair (s48-trace-value (vm-cdr pair)))
	  (loop (vm-cdr alist))))))

; Walk down the finalizer alist, separating out the pairs whose things
; have been copied.

(define (partition-finalizer-alist!)
  (let loop ((alist (shared-ref *finalizer-alist*)) (okay null) (goners null))
    (if (vm-eq? alist null)
	(begin
	  (shared-set! *finalizer-alist* okay)
	  (set! *finalize-these* (vm-append! goners *finalize-these*)))
	(let* ((alist (s48-trace-value alist))
	       (pair (s48-trace-value (vm-car alist)))
	       (thing (vm-car pair))
	       (next (vm-cdr alist))
	       (traced? (s48-extant? thing)))
	  (vm-set-car! pair (s48-trace-value thing))
	  (vm-set-car! alist pair)
	  (cond (traced?
		 (vm-set-cdr! alist okay)
		 (loop next alist goners))
		(else
		 (vm-set-cdr! alist goners)
		 (loop next okay alist)))))))

(define (vm-append! l1 l2)
  (if (vm-eq? l1 null)
      l2
      (let ((last-pair (let loop ((l l1))
			 (if (vm-eq? (vm-cdr l) null)
			     l
			     (loop (vm-cdr l))))))
	(vm-set-cdr! last-pair l2)
	l1)))

;----------------

(define (set-code-pointer! code pc)
;  (if (= code 100384375)
;      (breakpoint "set-code-pointer!"))
  (set! *last-code-called* code)
  (set! *code-pointer* (code+pc->code-pointer code pc)))

(define (code+pc->code-pointer code pc)
  (address+ (address-after-header code) pc))

(define (code-pointer->pc code pointer)
  (address-difference pointer
		      (address-after-header code)))

(define (within-code? code-pointer code)
  (let ((start (address-after-header code)))
    (and (address<= start code-pointer)
	 (address< code-pointer
		   (address+ start 
			     (safe-byte-vector-length code))))))

; This has to operate with broken hearts.

(define (safe-byte-vector-length code)
  (code-vector-length (let ((header (stob-header code)))
                        (if (stob? header)
                            header
                            code))))

(define (current-code-vector)
  (if (within-code? *code-pointer* *last-code-called*)
      *last-code-called*
      (let ((code (code-pointer->code *last-code-pointer-resumed*)))
	(if (within-code? *code-pointer* code)
	    code
	    (error "VM error: unable to locate current code vector")))))

(define (code-pointer->code code-pointer)
  (let ((pc (fetch-two-bytes (address- code-pointer 5))))
    (address->stob-descriptor (address- code-pointer pc))))

; Silly utility that should be elsewhere.

(define (fetch-two-bytes pointer)
  (+ (shift-left (fetch-byte pointer) 8)
     (fetch-byte (address+ pointer 1))))

(define (current-code+pc)
  (let ((code (current-code-vector)))
    (values code
	    (code-pointer->pc code *code-pointer*))))

; These two templates are used when pushing continuations for calls to
; interrupt and exception handlers.  The continuation data gives zero
; as the size, which tells the stack-tracing code that the real size
; is in the continuation just below the saved registers.

(define (initialize-interpreter+gc)          ;Used only at startup
  (let ((key (ensure-space (* 6 return-code-size))))
    (set! *interrupted-byte-call-return-code*
	  (make-return-code ignore-values-protocol
                            #xffff              ; dummy template offset
			    (enum op resume-interrupted-call-to-byte-code)
			    #xFFFF		; escape value
			    key))
    (set! *interrupted-native-call-return-code*
	  (make-return-code ignore-values-protocol
                            #xffff              ; dummy template offset
			    (enum op resume-interrupted-call-to-native-code)
			    #xFFFF		; escape value
			    key))
    (set! *poll-interrupt-return-code*
	  (make-return-code ignore-values-protocol
                            #xffff              ; dummy template offset
			    (enum op return-from-poll-interrupt)
			    #xFFFF		; escape value
			    key))
    (set! *exception-return-code*
	  (make-return-code 1	; want exactly one return value
                            #xffff              ; dummy template offset
			    (enum op return-from-exception)
			    #xFFFF		; escape value
			    key))
    (set! *native-exception-return-code*
	  (make-return-code 1	; want exactly one return value
                            #xffff              ; dummy template offset
			    (enum op return-from-native-exception)
			    #xFFFF		; escape value
			    key))
    (set! *call-with-values-return-code*
	  (make-return-code call-with-values-protocol
                            #xffff              ; dummy template offset
			    0			; opcode is not used
			    1
			    key))))

; Users of the above templates have to skip over the continuation data.
(define continuation-data-size 3)

;----------------
; Continuations

(define (pop-continuation!)
  (set! *code-pointer* (pop-continuation-from-stack))
  (set! *last-code-pointer-resumed* *code-pointer*))
  
;----------------
; Instruction stream access

(define (code-byte index)
  (fetch-byte (address+ *code-pointer* (+ index 1))))

(define (code-offset index)
  (adjoin-bits (code-byte index)
	       (code-byte (+ index 1))
	       bits-used-per-byte))

(define (get-literal code-index)
  (template-ref (stack-ref (code-offset code-index))
		(code-offset (+ code-index 2))))

; Return the current op-code.  CODE-ARGS is the number of argument bytes that
; have been used.

(define (current-opcode)
  (code-byte -1))

; INTERPRET is the main instruction dispatch for the interpreter.

;(define trace-instructions? #f)
;(define *bad-count* 0)
;(define *i* 0)

(define (interpret code-pointer)
;  (if (and trace-instructions? (> *i* *bad-count*))
;      (receive (code pc)
;          (current-code+pc)
;        (write-instruction code pc 1 #f)
;        (check-stack)))
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
       (push-exception-setup! why (+ byte-args 1)) ; add 1 for the opcode
       (push arg1) ...
       (goto raise (count-exception-args arg1 ...))))))

;----------------
; Opcodes

(define (uuo)
  (raise-exception unimplemented-instruction
		   0
		   (enter-fixnum (fetch-byte *code-pointer*))))

(define opcode-dispatch (make-vector op-count uuo))

(define-syntax define-opcode
  (syntax-rules ()
    ((define-opcode op-name body ...)
     (vector-set! opcode-dispatch (enum op op-name) (lambda () body ...)))))

;----------------
; Exceptions

; The system reserves enough stack space to allow for an exception at any time.
; If the reserved space is used a gc must be done before the exception handler
; is called.

; New exception handlers in *val*.

(define-opcode set-exception-handlers!
  (cond ((or (not (vm-vector? *val*))
	     (< (vm-vector-length *val*) op-count))
	 (raise-exception wrong-type-argument 0 *val*))
	(else
	 (let ((temp (shared-ref *exception-handlers*)))
	   (shared-set! *exception-handlers* *val*)
	   (goto continue-with-value
		 temp
		 0)))))

; The current opcode and the exception are pushed as arguments to the handler.
; INSTRUCTION-SIZE is the size of the current instruction and is used to jump
; to the next instruction when returning.  The exception is saved in the
; continuation for use in debugging.

(define *native-exception-cont* 0)

(define (reset-native-exception-cont!)
  (set! *native-exception-cont* 0))

;;; Eventually, the native code wants the VM to detect and handle and
;;; excpetion In this case, the native code sets
;;; *native-exception-cont* to the continuation of the exception. If
;;; this is detected here, push-native-exception-setup! fills the
;;; exception data diffently and installs return-from-native-exception
;;; as opcode

(define (push-exception-setup! exception instruction-size)
;  (breakpoint "exception continuation")
  (if (= 0 *native-exception-cont*)
      (receive (code pc)
	 (current-code+pc)
	 (push-exception-continuation! (code+pc->code-pointer *exception-return-code*
							      return-code-pc)
				       (enter-fixnum pc)
				       code
				       (enter-fixnum exception)
				       (enter-fixnum instruction-size)))
      (receive (bc-code bc-pc) (current-code+pc)
	(push *native-exception-cont*)
	(set! *cont* *stack*)
	(write-string "handling exception for nc " (current-error-port))
	(write-integer *native-exception-cont* (current-error-port))
	(write-string " return code pc is " (current-error-port))
	(write-integer return-code-pc (current-error-port))
        (write-string " opcode is " (current-error-port))
        (write-integer (current-opcode) (current-error-port))
        (write-string " exception is " (current-error-port))
        (write-integer exception (current-error-port))
        (write-string " *val* is " (current-error-port))
        (write-integer *val* (current-error-port))
	(push-native-exception-continuation! (code+pc->code-pointer *native-exception-return-code*
								    return-code-pc)
					     (enter-fixnum exception)
                                             (enter-fixnum bc-pc)
                                             bc-code)
	(reset-native-exception-cont!)))
  (push (enter-fixnum (current-opcode)))
  (push (enter-fixnum exception)))

(define-opcode return-from-exception
  (receive (pc code exception size)
      (pop-exception-data)
    (let* ((pc (extract-fixnum pc))
	   (opcode (code-vector-ref code pc)))
      (cond ((okay-to-proceed? opcode)
	     (set-code-pointer! code (+ pc (extract-fixnum size)))
	     (goto interpret *code-pointer*))
	    (else
	     (set-code-pointer! code pc)
	     (raise-exception illegal-exception-return 0 exception))))))

(define-opcode return-from-native-exception
  (receive (exception bc-pc bc-code)
      (pop-native-exception-data)
    (let* ((bc-pc (extract-fixnum bc-pc))
           (opcode (code-vector-ref bc-code bc-pc)))
      (cond ((okay-to-proceed? opcode)
	     (write-string "returning to nc " (current-error-port))
	     (write-integer (fetch *stack*)  (current-error-port))
	     (return-values 0 null 0))
	    (else 
	     (set-code-pointer! bc-code bc-pc)
	     (raise-exception illegal-exception-return 0 exception))))))

; It's okay to proceed if the opcode is a data operation, which are all those
; from EQ? on up, or references to globals (where the use can supply a value).

(define (okay-to-proceed? opcode)
  (or (<= (enum op eq?) opcode)
      (= opcode (enum op global))))
	      
(define no-exceptions? #f)

(define (raise nargs)
;  (let ((opcode (enumerand->name (extract-fixnum (stack-ref (+ nargs 1))) op))
;        (why (enumerand->name (extract-fixnum (stack-ref nargs)) exception)))
;    (if (and no-exceptions?
;             (not (and (eq? 'write-char opcode)
;                       (eq? 'buffer-full/empty why))))
;        (breakpoint "exception check ~A ~A ~A" opcode why nargs)))
  ;; try to be helpful when all collapses
  (let* ((opcode (extract-fixnum (stack-ref (+ nargs 1))))
	 (lose (lambda (message)
		 (write-string "Template UIDs: " (current-error-port))
		 (report-continuation-uids (current-code-vector)
					   (current-error-port))
		 (newline (current-error-port))
		 (let ((why (extract-fixnum (stack-ref nargs))))
		   (if (and (eq? why (enum exception undefined-global))
			    (fixnum? (location-id (stack-ref (- nargs 1)))))
		       (error message opcode why
			      (extract-fixnum
			       (location-id (stack-ref (- nargs 1)))))
		       (error message opcode why)))))
	 (handlers (shared-ref *exception-handlers*)))
    (if (not (vm-vector? handlers))
	(lose "exception-handlers is not a vector"))
    (set! *val* (vm-vector-ref handlers opcode))
    (if (not (closure? *val*))
	(lose "exception handler is not a closure"))
    ;; We add 2, one for the opcode, one for the exception itself
    (goto call-exception-handler (+ nargs 2) opcode)))

;----------------
; Literals and local variables.
; Loaded from the stack, template, or environment into *val*, with the
; template or environment obtained from the stack.

(define-opcode stack-indirect
  (goto continue-with-value
	(d-vector-ref (stack-ref (code-byte 0))
		      (code-byte 1))
	2))

(define-opcode push+stack-indirect
  (push *val*)
  (goto continue-with-value
	(d-vector-ref (stack-ref (code-byte 0))
		      (code-byte 1))
	2))

(define-opcode stack-indirect+push
  (push (d-vector-ref (stack-ref (code-byte 0))
		      (code-byte 1)))
  (goto continue 2))

(define-opcode big-stack-indirect
  (goto continue-with-value
	(d-vector-ref (stack-ref (code-offset 0))
		      (code-offset 2))
	4))

(define-opcode unassigned-check
  (if (vm-eq? *val* unassigned-marker)
      (raise-exception unassigned-local 0)
      (goto continue 0)))

(define-opcode integer-literal
  (goto continue-with-value
	(integer-literal 0)
	1))

(define-opcode push+integer-literal
  (push *val*)
  (goto continue-with-value
	(integer-literal 0)
	1))

(define-opcode integer-literal+push
  (push (integer-literal 0))
  (goto continue 1))

; PreScheme does not have a SIGNED-BYTE-REF so we bias the value by 128.

(define (integer-literal offset)
  (enter-fixnum (- (code-byte offset)
		   128)))

;----------------
; Global variable access

(define-opcode global        ;Load a global variable.
  (let* ((template (stack-ref (code-offset 0)))
	 (index (code-offset 2))
	 (location (template-ref template index)))
    (set! *val* (contents location))
    (if (undefined? *val*)           ;unbound or unassigned
	(raise-exception undefined-global 4
			 location template (enter-fixnum index))
	(goto continue 4))))

(define-opcode set-global!
  (let* ((template (stack-ref (code-offset 0)))
	 (index (code-offset 2))
	 (location (template-ref template index)))
    (cond ((vm-eq? (contents location) unbound-marker)
	   (raise-exception undefined-global 4
			    location template (enter-fixnum index) *val*))
          (else
           (set-contents! location *val*)
	   (goto continue-with-value
		 unspecific-value
		 4)))))

;----------------
; Stack operation

(define-opcode push	;Push *val* onto the stack.
  (push *val*)
  (goto continue 0))

(define-opcode push-false
  (push false)
  (goto continue 0))

(define-opcode pop	;Pop *val* from the stack.
  (goto continue-with-value
	(pop)
	0))

(define-opcode pop-n
  (add-cells-to-stack! (- (code-offset 0)))
  (goto continue 2))

(define-opcode push-n
  (do ((n (code-offset 0) (- n 1)))
      ((= 0 n))
    (push false))
  (goto continue 2))

(define-opcode stack-ref
  (goto continue-with-value
	(stack-ref (code-byte 0))
	1))

(define-opcode big-stack-ref
  (goto continue-with-value
	(stack-ref (code-offset 0))
	2))

(define-opcode push+stack-ref
  (push *val*)
  (goto continue-with-value
	(stack-ref (code-byte 0))
	1))

(define-opcode stack-ref+push
  (push (stack-ref (code-byte 0)))
  (goto continue 1))

(define-opcode stack-set!
  (stack-set! (code-byte 0) *val*)
  (goto continue 1))

(define-opcode big-stack-set!
  (stack-set! (code-offset 0) *val*)
  (goto continue 2))

(define-opcode stack-shuffle!
  (push *val*)
  (let ((n-moves (code-byte 0)))
    (do ((move 0 (+ 1 move)))
	((= move n-moves))
      (let ((index (+ 1 (* 2 move))))
	(stack-set! (code-byte (+ 1 index))
		    (stack-ref (code-byte index)))))
    (goto continue-with-value
	  (pop)
	  (+ 1 (* 2 n-moves)))))

(define-opcode big-stack-shuffle!
  (push *val*)
  (let ((n-moves (code-offset 0)))
    (do ((move 0 (+ 1 move)))
	((= move n-moves))
      (let ((index (+ 2 (* 4 move))))
	(stack-set! (code-offset (+ 2 index))
		    (stack-ref (code-offset index)))))
    (goto continue-with-value
	  (pop)
	  (+ 2 (* 4 n-moves)))))


;----------------
; LAMBDA

; No longer used.
;(define-opcode closure
;  (receive (env key)
;      (if (= 0 (code-byte 2))
;          (let ((key (ensure-space (+ closure-size (current-env-size)))))
;            (values (preserve-current-env key) key))
;          (let ((key (ensure-space closure-size)))
;            (values *val* key)))
;    (goto continue-with-value
;          (make-closure (get-literal 0) env key)
;	  3)))

; (enum op make-[big-]flat-env)
; number of values
; number of closures
; [offset of template in frame
;  offsets of templates in template]
; number of variables in frame (size)
; offsets of vars in frame
; [offset of env in frame
;  number of vars in env
;  offsets of vars in level]*
;
; All numbers and offsets are one byte for make-flat-env and two bytes for
; make-big-flat-env.

(define (flat-env-maker size fetch)
  (define (make-env)
    (let* ((total-count (fetch 0))
	   (closures (fetch size))
	   (key (ensure-space (+ (vm-vector-size total-count)
				 (* closures closure-size))))
	   (new-env (vm-make-vector total-count key)))
;      (format #t "[flat env with ~D values]~%" total-count)
      (goto continue-with-value
	    new-env
	    (if (= closures 0)
		(fill-env new-env 0 (+ size size) total-count)
		(make-closures new-env closures key total-count)))))

  (define (make-closures new-env count key total-count)
;    (format #t "[making ~D closures]~%" count)
    (let* ((offset (+ size size))
	   (template (stack-ref (fetch offset))))
      (do ((count count (- count 1))
	   (i 0 (+ i 1))
	   (offset (+ offset size) (+ offset size)))
	  ((= count 0)
	   (fill-env new-env i offset total-count))
	(vm-vector-set! new-env
			i
			(make-closure (template-ref template (fetch offset))
				      new-env
				      key)))))

  (define (fill-env new-env i offset total-count)
    (do ((count (fetch offset) (- count 1))
	 (i i (+ i 1))
	 (offset (+ offset size) (+ offset size)))
	((= count 0)
	 (if (< i total-count)
	     (finish-env new-env i offset total-count)
	     offset))
      (vm-vector-set! new-env i (stack-ref (fetch offset)))))

  (define (finish-env new-env i offset total-count)
    (let loop ((i i)
	       (offset offset))
      (if (= i total-count)
	  offset
	  (let ((env (stack-ref (fetch offset)))
		(count (fetch (+ offset size))))
;	    (format #t "[getting ~D more values from ~D]~%"
;		    count
;		    (fetch offset))
	    (do ((count count (- count 1))
		 (i i (+ i 1))
		 (offset (+ offset size size) (+ offset size)))
		((= count 0)
		 (loop i offset))
	      (vm-vector-set! new-env
			      i
			      (vm-vector-ref env (fetch offset))))))))
  make-env)

(let ((one-byte-maker (flat-env-maker 1 code-byte)))
  (define-opcode make-flat-env (one-byte-maker)))

(let ((two-byte-maker (flat-env-maker 2 code-offset)))
  (define-opcode make-big-flat-env (two-byte-maker)))

;----------------
; Preserve the current continuation and put it in *val*.

(define-opcode current-cont
  (let ((key (ensure-space (current-continuation-size))))
    (goto continue-with-value
	  (copy-current-continuation-to-heap key)
	  0)))

(define-opcode with-continuation
  (set-current-continuation! (pop))
  (goto perform-application 0))

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

; Unconditional jumps

(define-opcode jump
  (set! *code-pointer*
	(address+ *code-pointer*
		  (code-offset 0)))
  (goto interpret *code-pointer*))

; Same thing except the other way.

(define-opcode jump-back
  (set! *code-pointer*
        (address- *code-pointer*
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

