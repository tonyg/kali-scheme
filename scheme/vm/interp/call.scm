; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code to handle the calling and return protocols.

; *VAL* is the procedure, the arguments are on stack, and the next byte
; is the number of arguments (all of which are on the stack).  This checks
; that *VAL* is in fact a closure and for the common case of a non-n-ary
; procedure that has few arguments.  This case is handled directly and all
; others are passed off to PLAIN-PROTOCOL-MATCH.

(define-opcode call
  (let ((stack-arg-count (code-byte 2)))
    (make-continuation-on-stack (address+ *code-pointer* (code-offset 0))
				stack-arg-count)
    (goto do-call stack-arg-count)))

(define-opcode tail-call
  (let ((stack-arg-count (code-byte 0)))
    (move-args-above-cont! stack-arg-count)
    (goto do-call (code-byte 0))))

(define (do-call stack-arg-count)
  (if (closure? *val*)
      (let* ((template (closure-template *val*))
	     (code (template-code template))
	     (protocol (code-vector-ref code 1)))
	(cond ((= protocol stack-arg-count)
	       (goto run-body-with-default-space code 2 template))
	      ((= (native->byte-protocol protocol)
		  stack-arg-count)
	       (goto call-native-code *val* 2 stack-arg-count))
	      (else
	       (goto plain-protocol-match stack-arg-count))))
      (goto application-exception
	    (enum exception bad-procedure)
	    stack-arg-count null 0)))

;----------------------------------------------------------------
; Native protocols have the high bit set.

(define (native-protocol? protocol)
  (< 127 protocol))

(define (native->byte-protocol protocol)
  (bitwise-and protocol 127))

(define (byte->native-protocol protocol)
  (bitwise-ior protocol 128))

(define (call-native-code proc protocol-skip stack-nargs)
  (move-args-above-cont! stack-nargs)
  (goto post-native-dispatch (s48-call-native-procedure proc protocol-skip)))

(define s48-*native-protocol*)

; The external code just sets the C variable directly, but we export this to
; let the Pre-Scheme compiler know that someone, somewhere, does a set.

(define (s48-set-native-protocol! protocol)
  (set! s48-*native-protocol* protocol))

(define (post-native-dispatch tag)
  (let loop ((tag tag))
    (case tag
      ((0)
       (goto return-values s48-*native-protocol* null 0))
      ((1)
       (goto perform-application s48-*native-protocol*))
      ((2)
       (cond ((pending-interrupt?)
	      (push-native-interrupt-continuation)
	      (goto find-and-call-interrupt-handler))
	     (else
	      (loop (s48-invoke-native-continuation
		     (+ (address->integer (pop-continuation-from-stack))
			2))))))
      ((3)
       ;;; just a prototype
       (push-native-interrupt-continuation)
       (let ((nargs (pop)))
	 (write-string "exception with " (current-error-port))
	 (write-integer nargs (current-error-port))
	 (write-string " arguments" (current-error-port))
	 (goto raise nargs)))
      ((4)
       (goto interpret *code-pointer*))
      (else
       (error "unexpected native return value" tag)))))

;; Not used for now
(define (push-native-exception-continuation)
 (push-continuation! (code+pc->code-pointer *native-exception-return-code*
					     return-code-pc)))

;----------------------------------------------------------------
; As above but with a two-byte argument count.  The tail and not-tail calls
; are both done using the same opcode (which is not done above for speed in
; the tail case; this optimization needs to be tested for effectiveness).

(define-opcode big-call
  (let ((stack-arg-count (code-offset 2)))
    (maybe-make-continuation stack-arg-count)
    (goto perform-application stack-arg-count)))

; A number of opcodes that make calls use this.  We get a the offset of the
; return address and then either add it as a new continuation below the current
; arguments or, for tail calls, move those arguments above the current
; continuation.

(define (maybe-make-continuation stack-arg-count)
  (let ((return-pointer-offset (code-offset 0)))
    (if (= return-pointer-offset 0)
	(move-args-above-cont! stack-arg-count)
	(make-continuation-on-stack (address+ *code-pointer*
					      return-pointer-offset)
				    stack-arg-count))))

; Call a template instead of a procedure.  This is currently only used for
; stringing together the initialization code made by the static linker.
;
; call-template <return-offset> <template-index> <index-within-template> <nargs>

(define-opcode call-template
  (let* ((template (get-literal 2))
	 (code (template-code template))
	 (nargs (code-byte 6)))
    (maybe-make-continuation nargs)
    (cond ((= nargs (code-vector-ref code 1))
	   (goto run-body-with-default-space code 2 template))
	  ((and (= big-stack-protocol (code-vector-ref code 1))
		(= nargs
		   (code-vector-ref code (- (code-vector-length code) 3))))
	   (goto run-body
		 code
		 6
		 template
		 (code-vector-ref16 code (- (code-vector-length code) 2))))
	  (else
	   (raise-exception wrong-type-argument 7 template)))))

; The following is used only for experiments.  The compiler does not use it.

;(define-opcode goto-template
;  (set-code-pointer! (template-code template) 0)
;  (goto interpret *code-pointer*))

; APPLY: *VAL* is the procedure, the rest-arg list is on top of the stack,
; the next two bytes are the number of stack arguments below the rest-args list.
; We check that the rest-arg list is a proper list and let
; PERFORM-APPLICATION-WITH-REST-LIST do the work.
    
(define-opcode apply
  (let ((list-args (pop))
	(stack-nargs (code-offset 2)))
    (receive (okay? length)
	(okay-argument-list list-args)
      (if okay?
	  (begin
	    (maybe-make-continuation stack-nargs)
	    (goto perform-application-with-rest-list
		  stack-nargs
		  list-args
		  length))
	  (let ((args (pop-args->list*+gc list-args stack-nargs)))
	    (raise-exception wrong-type-argument -1 *val* args))))))

; This is only used for the closed-compiled version of APPLY.
;
; Stack = arg0 arg1 ... argN rest-list N+1 total-arg-count
; Arg0 is the procedure.
;
; Note that the rest-list on the stack is the rest-list passed to APPLY
; procedure and not the rest-list to be used in the call to the procedure.
; Consider (APPLY APPLY (LIST LIST '(1 2 3))), where the initial APPLY
; is not done in-line.  The stack for the inner call to APPLY will be
; [(<list-procedure> (1 2 3)), 1, 2], whereas for
; (APPLY APPLY LIST 1 '(2 (3))) the stack will be
; [<list-procedure>, 1, (2 (3)), 3, 4].
;
; We grab the counts and the procedure and copy the rest of the stack arguments
; down to make us properly tail-recursive.  Then we get the true stack-arg count
; and list args and again let PERFORM-APPLICATION-WITH-REST-LIST do the work.

(define-opcode closed-apply
  (let* ((nargs (extract-fixnum (pop)))
	 (stack-nargs (extract-fixnum (pop))))
    (set! *val* (stack-ref stack-nargs))	; proc in *VAL*
    (move-args-above-cont! nargs)
    (receive (okay? stack-arg-count list-args list-arg-count)
	(get-closed-apply-args nargs stack-nargs)
      (if okay?
	  (begin
	    (goto perform-application-with-rest-list
		  stack-arg-count
		  list-args
		  list-arg-count))
	  (let ((args (pop-args->list*+gc list-args stack-arg-count)))
	    (raise-exception wrong-type-argument -1 *val* args))))))

; Stack = arg0 arg1 ... argN rest-list
; This needs to get the last argument, which is either argN or the last
; element of the rest-list, and splice it into the rest of the arguments.
; If the rest-list is null, then argN is the last argument and becomes the
; new rest-list.  If the rest-list is non-null, then we go to the end, get
; the list there, and splice the two together to make a single list.
; This only happens if someone does (APPLY APPLY ...).

(define (get-closed-apply-args nargs stack-nargs)
  (let ((rest-list (pop)))
    (receive (list-args stack-nargs)
	(cond ((vm-eq? rest-list null)
	       (values (pop)
		       (- stack-nargs 2))) ; drop proc and final list
	      ((vm-eq? (vm-cdr rest-list) null)
	       (values (vm-car rest-list)
		       (- stack-nargs 1))) ; drop proc
	      (else
	       (let* ((penultimate-cdr (penultimate-cdr rest-list))
		      (list-args (vm-car (vm-cdr penultimate-cdr))))
		 (vm-set-cdr! penultimate-cdr list-args)
		 (values rest-list
			 (- stack-nargs 1))))) ; drop proc
      (receive (okay? list-arg-count)
	  (okay-argument-list list-args)
	(values okay?
		stack-nargs
		list-args
		list-arg-count)))))

; If LIST is a proper list (the final cdr is null) then we return #T and the
; length of the list, otherwise we return #F.

(define (okay-argument-list list)
  (let loop ((fast list) (len 0) (slow list) (move-slow? #f))
    (cond ((vm-eq? null fast)
	   (values #t len))
	  ((or (not (vm-pair? fast)))
	   (values #f 0))
	  ((not move-slow?)
	   (loop (vm-cdr fast) (+ len 1) slow #t))
	  ((vm-eq? fast slow)
	   (values #f 0))
	  (else
	   (loop (vm-cdr fast) (+ len 1) (vm-cdr slow) #f)))))

; Return the second-to-last cdr of LIST.

(define (penultimate-cdr list)
  (let loop ((list (vm-cdr (vm-cdr list))) (follower list))
    (if (eq? null list)
	follower
	(loop (vm-cdr list) (vm-cdr follower)))))

;----------------
; Call the procedure in *VAL*.  STACK-ARG-COUNT is the number of arguments
; on the stack, LIST-ARGS is a list of LIST-ARG-COUNT additional arguments.
;
; The CLOSURE? and protocol checks must come before the interrupt check because
; the interrupt code assumes that the correct template is in place.  This delays
; the handling of interrupts by a few instructions; it shouldn't matter.

(define (perform-application stack-arg-count)
  (if (closure? *val*)
      (goto plain-protocol-match stack-arg-count)
      (goto application-exception
	    (enum exception bad-procedure)
	    stack-arg-count null 0)))

(define (perform-application-with-rest-list stack-arg-count
					    list-args list-arg-count)
  (cond ((= 0 list-arg-count)
	 (goto perform-application stack-arg-count))
	((closure? *val*)
	 (goto list-protocol-match
	       stack-arg-count list-args list-arg-count))
	(else
	 (goto application-exception
	       (enum exception bad-procedure)
	       stack-arg-count list-args list-arg-count))))

(define (wrong-nargs stack-arg-count list-args list-arg-count)
  (goto application-exception
	(enum exception wrong-number-of-arguments)
	stack-arg-count list-args list-arg-count))

; The main protocol-matching function takes as an argument a token indicating
; if the called-value is a handler and if so, what kind.  A non-negative value
; is the opcode whose exception handler is begin called.  -1 means that the
; procedure is not a handler.  Any other negative value indicates that the
; procedure is an interrupt handler.  The interrupt is (- -2 token).

(define not-a-handler -1)

(define (call-exception-handler stack-arg-count opcode)
  (goto real-protocol-match
	stack-arg-count
	null
	0
	opcode))
  
(define (call-interrupt-handler stack-arg-count interrupt)
  (goto real-protocol-match
	stack-arg-count
	null
	0
	(- -2 interrupt)))

; Check that the arguments match those needed by *VAL*, which is a closure,
; moving arguments to or from the stack if necessary, and ensure that there
; is enough stack space for the procedure.  The environment needed by *VAL*
; is created.

(define (plain-protocol-match stack-arg-count)
  (goto real-protocol-match stack-arg-count null 0 not-a-handler))

(define (list-protocol-match stack-arg-count list-args list-arg-count)
  (goto real-protocol-match
	stack-arg-count
	list-args
	list-arg-count
	not-a-handler))

(define (real-protocol-match stack-arg-count
			     list-args
			     list-arg-count
			     handler-tag)
  (let ((code (template-code (closure-template *val*)))
	(total-arg-count (+ stack-arg-count list-arg-count))
  	(lose (lambda ()
		(cond ((= handler-tag not-a-handler)
		       (goto wrong-nargs
			     stack-arg-count list-args list-arg-count))
		      ((<= 0 handler-tag)
		       (error "wrong number of arguments to exception handler"
			      handler-tag))
		      (else
		       (error "wrong number of arguments to interrupt handler"
			      (- -2 handler-tag)))))))
    (assert (= (enum op protocol)
  	       (code-vector-ref code 0)))
    (let loop ((protocol (code-vector-ref code 1))
  	       (stack-space default-stack-space)
  	       (native? #f))
      (let ((win (lambda (skip stack-arg-count)
  		   (if native?
  		       (goto call-native-code *val* skip stack-arg-count)
  		       (goto run-body code
			              skip
				      (closure-template *val*)
			              stack-space)))))
	(let ((fixed-match (lambda (wants skip)
			     (if (= wants total-arg-count)
				 (begin
				   (if (not (= 0 list-arg-count))
				       (push-list list-args list-arg-count))
				   (win skip total-arg-count))
				 (lose))))
	      ;; N-ary procedure.
	      (n-ary-match (lambda (wants-stack-args skip)
			     (if (<= wants-stack-args total-arg-count)
				 (begin
				   (rest-list-setup+gc wants-stack-args
						       stack-arg-count
						       list-args
						       list-arg-count)
				   (win skip (+ wants-stack-args 1)))
				 (lose))))
	      ;; If there are > 2 args the top two are pushed on the stack.
	      ;; Then the remaining list, the total number of arguments, and
	      ;; the number on the stack are pushed on the stack.
	      (args+nargs-match (lambda (skip)
				  (let ((final-stack-arg-count
					 (if (< total-arg-count 3)
					     total-arg-count
					     (max 2 stack-arg-count))))
				    (rest-list-setup+gc (max stack-arg-count
							     final-stack-arg-count)
							stack-arg-count
							list-args
							list-arg-count)
				    (push (enter-fixnum final-stack-arg-count))
				    (push (enter-fixnum total-arg-count))
				    (win skip (+ final-stack-arg-count 3))))))
	  (cond ((= protocol nary-dispatch-protocol)
		 (cond ((< total-arg-count 3)
			(let ((skip (code-vector-ref code (+ 3 total-arg-count))))
			  (if (= 0 skip)
			      (lose)
			      (begin
				(push-list list-args list-arg-count)
				(goto run-nary-dispatch-body code skip)))))
		       ((= 0 (code-vector-ref code 2))
			(lose))
		       (else
			(args+nargs-match 6))))	; leave env/template
		((<= protocol maximum-stack-args)
		 (fixed-match protocol 2))
		((= protocol two-byte-nargs+list-protocol)
		 (n-ary-match (code-vector-ref16 code 2) 4))
		((= protocol args+nargs-protocol)
		 (if (>= total-arg-count
			 (code-vector-ref code 2))
		     (args+nargs-match 3)
		     (lose)))
		((native-protocol? protocol)
		 (loop (native->byte-protocol protocol) stack-space #t))
		((= protocol two-byte-nargs-protocol)
		 (fixed-match (code-vector-ref16 code 2) 4))
		((= protocol big-stack-protocol)
		 (let ((length (code-vector-length code)))
		   (loop (code-vector-ref code (- length 3))
			 (code-vector-ref16 code (- length 2))
			 native?)))
		(else
		 (error "unknown protocol" protocol)
		 (lose))))))))

; Adjusts the number of stack arguments to be WANTS-STACK-ARGS by moving
; arguments between the stack and LIST-ARGS as necessary.  Whatever is left
; of LIST-ARGS is then copied and the copy is pushed onto the stack.

(define (rest-list-setup+gc wants-stack-args stack-arg-count
			    list-args list-arg-count)
  (cond ((= stack-arg-count wants-stack-args)
	 (push (copy-list*+gc list-args list-arg-count)))
	((< stack-arg-count wants-stack-args)
	 (let ((count (- wants-stack-args stack-arg-count)))
	   (push (copy-list*+gc (push-list list-args count)
				(- list-arg-count count)))))
	(else ; (> stack-arg-count wants-stack-args)
	 (let ((count (- stack-arg-count wants-stack-args)))
	   (push (pop-args->list*+gc (copy-list*+gc list-args list-arg-count)
				     count))))))

; Raise an exception, passing to it a list of the arguments on the stack and
; in LIST-ARGS.

(define (application-exception exception
			       stack-arg-count list-args list-arg-count)
  (let ((args (pop-args->list*+gc (copy-list*+gc list-args list-arg-count)
				  stack-arg-count)))
    (raise-exception* exception -1 *val* args))) ; no next opcode

;----------------------------------------------------------------

(define (run-body-with-default-space code used template)
  (real-run-body-with-default-space code used (+ used 1) template))

(define (run-nary-dispatch-body code start-pc)
  (real-run-body-with-default-space code 6 start-pc (closure-template *val*)))

(define (real-run-body-with-default-space code env/temp-offset used template)
  (env-and-template-setup (code-vector-ref code env/temp-offset) template)
  (set-code-pointer! code used)
  (if (and (ensure-default-procedure-space!)
	   (pending-interrupt?))
      (goto handle-interrupt)
      (goto interpret *code-pointer*)))

(define (run-body code used template needed-stack-space)
  (env-and-template-setup (code-vector-ref code used) template)
  (set-code-pointer! code (+ used 1))
  (if (and (ensure-stack-space! needed-stack-space)
	   (pending-interrupt?))
      (goto handle-interrupt)
      (goto interpret *code-pointer*)))

(define (env-and-template-setup env/template template)
  (cond ((= #b11 env/template)
	 (push (closure-env *val*))
	 (push template))
	((= #b01 env/template)
	 (push (closure-env *val*)))
	((= #b10 env/template)
	 (push template))))

;----------------------------------------------------------------
; Get a two-byte number from CODE-VECTOR.

(define (code-vector-ref16 code-vector index)
  (let ((high (code-vector-ref code-vector index)))
    (adjoin-bits high
		 (code-vector-ref code-vector (+ index 1))
		 bits-used-per-byte)))

(define (code-pointer-ref code-pointer index)
  (fetch-byte (address+ code-pointer index)))

(define (code-pointer-ref16 code-pointer index)
  (let ((high (code-pointer-ref code-pointer index)))
    (adjoin-bits high
		 (code-pointer-ref code-pointer (+ index 1))
		 bits-used-per-byte)))

;----------------
; Returns - these use many of the same protocols.

; Invoke the contination, if it can handle a single value.  There are four
; protocols that are okay:
;
;  1, ignore-values
;    We just leave *VAL* as is and return.
;  bottom-of-stack
;    Real continuation is either in the heap or #F (if we are really at the
;    bottom of the stack).  We get the real continuation and either try again
;    or return from the VM.
;  two-byte-nargs+list
;    Continuation is n-ary.  If it want 0 or 1 value on the stack we are okay
;    and do the setup and return.

(define-opcode return
  (let loop ()
    (let ((code-pointer (current-continuation-code-pointer)))
      (assert (= (enum op protocol)
		 (code-pointer-ref code-pointer 0)))
      (let ((protocol (code-pointer-ref code-pointer 1)))
	(cond ((or (= protocol 1)
		   (= protocol ignore-values-protocol))
	       (pop-continuation!)
	       (goto continue 1))		; one protocol byte
	      ((or (= protocol (byte->native-protocol 1))
		   (= protocol (byte->native-protocol
				  ignore-values-protocol)))
	       (goto native-return 2))
	      ((= protocol bottom-of-stack-protocol)
	       (let ((cont (get-continuation-from-heap)))
		 (if (continuation? cont)
		     (begin
		       (copy-continuation-from-heap! cont 0)
		       (loop))
		     (goto return-from-vm cont))))
	      ((= protocol call-with-values-protocol)
	       (let ((proc (current-continuation-ref 0))
		     (offset (code-pointer-ref16 code-pointer 2)))
		 (if (= offset 0)
		     (skip-current-continuation! 0)	; we're done with it
		     (shrink-and-reset-continuation!
		       (address+ code-pointer offset)))
		 (push *val*)
		 (set! *val* proc)
		 (goto perform-application-with-rest-list 1 null 0)))
	      ((= protocol two-byte-nargs+list-protocol)
	       (let ((wants-stack-args (code-pointer-ref16 code-pointer 2)))
		 (cond ((= 0 wants-stack-args)
			(pop-continuation!)
			(push (vm-cons *val* null (ensure-space vm-pair-size)))
			(goto continue 3))
		       ((= 1 wants-stack-args)
			(pop-continuation!)
			(push *val*)
			(push null)
			(goto continue 3))
		       (else
			(push *val*)
			(goto return-exception 1 null)))))
	      (else
	       (push *val*)
	       (goto return-exception 1 null)))))))

; CONT is not a continuation.  If it is false and *VAL* is a fixnum we can
; return from the VM.  Otherwise we set the continuation to #F and raise an
; exception.

(define (return-from-vm cont)
  (cond ((and (false? cont)
	      (fixnum? *val*))
	 (set! s48-*callback-return-stack-block* false) ; not from a callback
	 (extract-fixnum *val*))	  		; VM returns here
	(else
	 (set-current-continuation! false)
	 (raise-exception wrong-type-argument -1 *val* cont))))

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

; STACK-NARGS return values are on the stack.  Find the actual continuation
; and check the protocol:
;
; 1
;   If we have just one value we put it in *VAL* and return.
; ignore-values
;   Drop everything and just return
; bottom-of-stack
;   The real continuation is either in the stack or is FALSE (if we are really
;   at the bottom of the stack).  If the former we install it and try again.
;   If the latter we can return a single value, but not multiple values.
; call-with-values
;   Current continuation has a single value, a closure.  We remove the closure
;   and invoke it on the current values.

(define (return-values stack-nargs list-args list-arg-count)
  (let* ((code-pointer (current-continuation-code-pointer))
	 (protocol (code-pointer-ref code-pointer 1)))
    (assert (= (enum op protocol)
	       (code-pointer-ref code-pointer 0)))
    (cond ((= protocol 1)
	   (if (= 1 (+ stack-nargs list-arg-count))
	       (begin
		 (return-value->*val* stack-nargs list-args)
		 (pop-continuation!)
		 (goto continue 1))
	       (goto return-exception stack-nargs list-args)))
	  ((= protocol ignore-values-protocol)
	   (pop-continuation!)
	   (goto continue 1))
          ((native-protocol? protocol)
           (goto native-return-values
                 protocol stack-nargs list-args list-arg-count))
	  ((= protocol bottom-of-stack-protocol)
	   (let ((cont (get-continuation-from-heap)))
	     (cond ((continuation? cont)
		    (copy-continuation-from-heap! cont stack-nargs)
		    (goto return-values stack-nargs list-args list-arg-count))
		   ((= 1 (+ stack-nargs list-arg-count))
		    (return-value->*val* stack-nargs list-args)
		    (goto return-from-vm cont))
		   (else
		    (goto return-exception stack-nargs list-args)))))
	  ((= protocol call-with-values-protocol)
	   (set! *val* (current-continuation-ref 0))
	   (let ((offset (code-pointer-ref16 code-pointer 2)))
	     (cond ((= offset 0)
		    (skip-current-continuation! stack-nargs))
		   (else
		    (shrink-and-reset-continuation!
		      (address+ code-pointer offset))
		    (move-args-above-cont! stack-nargs))))
	   (goto perform-application-with-rest-list
		 stack-nargs
		 list-args
		 list-arg-count))
	  ((<= protocol maximum-stack-args)
	   (goto fixed-arg-return protocol 1
		 stack-nargs list-args list-arg-count))
	  ((= protocol two-byte-nargs+list-protocol)
	   (goto nary-arg-return (code-pointer-ref16 code-pointer 2) 3
		 stack-nargs list-args list-arg-count))
	  ((= protocol two-byte-nargs-protocol)
	   (goto fixed-arg-return (code-pointer-ref16 code-pointer 2) 3
		 stack-nargs list-args list-arg-count))
	  (else
	   (error "unknown protocol" protocol)
	   (goto return-exception stack-nargs list-args)))))

(define (native-return-values protocol stack-nargs list-args list-arg-count)
  (cond ((= protocol (byte->native-protocol 1))
	 (if (= 1 (+ stack-nargs list-arg-count))
	     (begin
	       (return-value->*val* stack-nargs list-args)
	       (goto native-return 2))
	     (goto return-exception stack-nargs list-args)))
	((= protocol (byte->native-protocol ignore-values-protocol))
	 (goto native-return 2))
	(else
	 (error "unknown native return protocol" protocol)
	 (goto return-exception stack-nargs list-args))))

(define (native-return protocol-skip)
  (goto post-native-dispatch
	(s48-invoke-native-continuation
	  (+ (address->integer (pop-continuation-from-stack))
	     protocol-skip))))

; The continuation wants a fixed number of arguments.  We pop the current
; continuation, move the stack arguments down to the new stack top, push
; any list arguments and off we go.

(define (fixed-arg-return count bytes-used stack-nargs list-args list-arg-count)
  (if (= count (+ stack-nargs list-arg-count))
      (let ((arg-top (pointer-to-stack-arguments)))
	(pop-continuation!)
	(move-stack-arguments! arg-top stack-nargs)
	(if (not (= 0 list-arg-count))
	    (push-list list-args list-arg-count))
	(goto continue bytes-used))
      (goto return-exception stack-nargs list-args)))

; The continuation wants a COUNT arguments on the stack plus a list of any
; additional arguments.  We pop the current continuation, move the stack
; arguments down to the new stack top, adjust the number of stack arguments,
; push the remaining list arguments, and off we go.

(define (nary-arg-return count bytes-used stack-nargs list-args list-arg-count)
  (if (<= count (+ stack-nargs list-arg-count))
      (let ((arg-top (pointer-to-stack-arguments)))
	(pop-continuation!)
	(move-stack-arguments! arg-top stack-nargs)
	(push (if (<= stack-nargs count)
		  (do ((stack-nargs stack-nargs (+ stack-nargs 1))
		       (l list-args (vm-cdr l)))
		      ((= count stack-nargs)
		       l)
		    (push (vm-car l)))
		  (pop-args->list*+gc list-args (- stack-nargs count))))
	(goto continue bytes-used))
      (goto return-exception stack-nargs list-args)))

; Move the (single) return value to *VAL*.

(define (return-value->*val* stack-nargs list-args)	   
  (set! *val*
	(if (= 1 stack-nargs)
	    (pop)
	    (vm-car list-args))))

; The return protocol doesn't match up so we gather all the return values into
; a list and raise an exception.

(define (return-exception stack-nargs list-args)
  (let ((args (pop-args->list*+gc list-args stack-nargs)))
    (raise-exception wrong-number-of-arguments
		     -1		 ; no next opcode
		     false
		     args)))

;----------------
; Manipulating lists of arguments

; Push COUNT elements from LIST onto the stack, returning whatever is left.

(define (push-list list count)
  (push list)
  (ensure-stack-space! count)
  (let ((list (pop)))
    (do ((i count (- i 1))
	 (l list (vm-cdr l)))
	((<= i 0) l)
      (push (vm-car l)))))

; Copy LIST, which has LENGTH elements.

(define (copy-list*+gc list length)
  (if (= length 0)
      null
      (begin
	(save-temp0! list)
	(let* ((key (ensure-space (* vm-pair-size length)))
	       (list (recover-temp0!))
	       (res (vm-cons (vm-car list) null key)))
	  (do ((l (vm-cdr list) (vm-cdr l))
	       (last res (let ((next (vm-cons (vm-car l) null key)))
			   (vm-set-cdr! last next)
			   next)))
	      ((vm-eq? null l)
	       res))))))

; Pop COUNT arguments into a list with START as the cdr.

(define (pop-args->list*+gc start count)
  (save-temp0! start)
  (let* ((key (ensure-space (* vm-pair-size count)))
	 (start (recover-temp0!)))
    (do ((args start (vm-cons (pop) args key))
	 (count count (- count 1)))
	((= count 0)
	 args))))

;----------------
; Opcodes for the closed-compiled versions of arithmetic primitives.
; The opcode sequences used are:
;   binary-reduce1 binary-op binary-reduce2 return
; and
;   compare-reduce1 binary-comparison-op compare-reduce2 return
; The compare version quits if the comparison operator returns false.
;
; For ...-reduce1 the stack looks like:
;   arg0 arg1 ... argN rest-list N+1
; If there are two or more arguments then at least two arguments are on the
; stack.

; Turn
;   *stack* = arg0 (arg1 . more) <3
; into
;   *val* = arg1, *stack* = arg0 (arg1 .more) 1 arg0
; or turn
;   *stack* = arg0 arg1 ... argN rest-list N+1
; into
;   *val* = arg1, *stack* = false arg1 ... argN rest-list N arg0

(define-opcode binary-reduce1
  (let ((stack-nargs (extract-fixnum (stack-ref 0))))
    (if (= stack-nargs 0)
	(let ((rest-list (stack-ref 1))
	      (arg0 (stack-ref 2)))
	  (push arg0)
	  (set! *val* (vm-car rest-list)))
	(let ((arg0 (stack-ref (+ stack-nargs 1)))
	      (arg1 (stack-ref stack-nargs)))
	  (stack-set! (+ stack-nargs 1) false)
	  (stack-set! 0 (enter-fixnum (- stack-nargs 1)))
	  (push arg0)
	  (set! *val* arg1)))
    (goto continue 0)))

; Turn
;   *val* = result, *stack* = arg0 (arg1 . more) 2
; into
;   *stack* = result more 2
; or turn
;   *val* = result, *stack* = arg1 ... argN rest-list N
; into
;   *stack* = result ... argN rest-list N

(define-opcode binary-reduce2
  (let* ((stack-nargs (extract-fixnum (stack-ref 0)))
	 (delta (case stack-nargs
		  ((0)
		   (let ((rest-list (stack-ref 1)))
		     (if (vm-eq? (vm-cdr rest-list) null)
			 1
			 (begin
			   (stack-set! 1 (vm-cdr rest-list))
			   (stack-set! 2 *val*)
			   -2))))
		  ((1)
		   (let ((rest-list (stack-ref 1)))
		     (if (vm-eq? rest-list null)
			 1
			 (begin
			   (stack-set! 0 (enter-fixnum 0))
			   (stack-set! 2 *val*)
			   -2))))
		  (else
		   (stack-set! (+ stack-nargs 1) *val*)
		   -2))))
    (set! *code-pointer* (address+ *code-pointer* delta))
    (goto interpret *code-pointer*)))

(define-opcode binary-comparison-reduce2
  (if (false? *val*)
      (goto continue 0)
      (let* ((stack-nargs (extract-fixnum (stack-ref 0)))
	     (delta (case stack-nargs
		      ((0)
		       (let ((rest-list (stack-ref 1)))
			 (if (vm-eq? (vm-cdr rest-list) null)
			     1
			     (begin
			       (stack-set! 1 (vm-cdr rest-list))
			       (stack-set! 2 (vm-car rest-list))
			       -2))))
		      ((1)
		       (let ((rest-list (stack-ref 1)))
			 (if (vm-eq? rest-list null)
			     1
			     (begin
			       (stack-set! 0 (enter-fixnum 0))
			       -2))))
		      (else
		       -2))))
	(set! *code-pointer* (address+ *code-pointer* delta))
	(goto interpret *code-pointer*))))

;----------------
; Statistics stuff
;
;(define call-stats (make-vector 16 0))
;
;		(let ((i (min stack-arg-count 15)))
;		  (vector-set! call-stats i (+ 1 (vector-ref call-stats i))))
;
;(define plain-calls (make-vector 7 0))
;
;(define (add-plain-call i)
;  (vector-set! plain-calls i (+ (vector-ref plain-calls i) 1)))
;
;(define apply-calls (make-vector 7 0))
;
;(define (add-apply-call i)
;  (vector-set! apply-calls i (+ (vector-ref apply-calls i) 1)))
;
;(define (dump-call-stats)
;  (let ((out (current-output-port)))
;    (write-string "Calls:" out)
;    (do ((i 0 (+ i 1)))
;        ((= i 16))
;      (write-char #\space out)
;      (write-integer (vector-ref call-stats i) out))
;    (write-char #\newline out)
;    (write-string "Plain calls" out)
;    (write-char #\newline out)
;    (do ((i 0 (+ i 1)))
;        ((= i 7))
;      (write-char #\space out)
;      (write-string (vector-ref call-strings i) out)
;      (write-integer (vector-ref plain-calls i) out)
;      (write-char #\newline out))
;    (write-string "Apply calls" out)
;    (write-char #\newline out)
;    (do ((i 0 (+ i 1)))
;        ((= i 7))
;      (write-char #\space out)
;      (write-string (vector-ref call-strings i) out)
;      (write-integer (vector-ref apply-calls i) out)
;      (write-char #\newline out))))
;
;(define call-strings
;  '#("nary-dispatch: "
;     "args&nargs: "
;     "no-env: "
;     "two-bytes-nargs+list: "
;     "plain: "
;     "two-byte-nargs: "
;     "big-stack: "))
     

