; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code to handle the calling and return protocols.

; *VAL* is the procedure, the arguments are on stack, and the next byte
; is the number of arguments (all of which are on the stack).  This checks
; that *VAL* is in fact a closure and for the common case of a non-n-ary
; procedure that has few arguments.  This case is handled directly and all
; others are passed off to PLAIN-PROTOCOL-MATCH.

(define-opcode call
  (let ((stack-arg-count (code-byte 0)))
    (if (closure? *val*)
	(let* ((template (closure-template *val*))
	       (code (template-code template))
	       (protocol (code-vector-ref code 1)))
	  (cond ((= protocol stack-arg-count)
		 (set! *template* template)
		 (set-current-env! (closure-env *val*))
		 (set-code-pointer! code 2)
		 (ensure-default-procedure-space! ensure-space)
		 (if (pending-interrupt?)
		     (goto handle-interrupt stack-arg-count)
		     (goto interpret *code-pointer*)))
		((= (native->byte-protocol protocol)
		    stack-arg-count)
		 (call-native-code *val* 2 stack-arg-count)
		 (goto interpret *code-pointer*))
		(else
		 (goto plain-protocol-match stack-arg-count))))
	(goto application-exception
	      (enum exception bad-procedure)
	      stack-arg-count null 0))))

; Native protocols have the high bit set.

(define (native->byte-protocol protocol)
  (bitwise-and protocol 127))

(define (call-native-code proc protocol-skip stack-nargs)
  (set! *template* (closure-template proc))
  (set-code-pointer! (template-code *template*) protocol-skip)
  (move-args-above-cont! stack-nargs)
  (s48-call-native-code proc))

; As above but with a two-byte argument count.

(define-opcode big-call
  (goto perform-application (code-offset 0)))

; Call a template instead of a procedure.  This is currently only used for
; stringing together the initialization code made by the static linker.
;
; **limitation**: this code only works for templates that take no arguments.

(define-opcode call-template
  (let ((win (lambda ()
	       (set-template! (get-literal 0)
			      (enter-fixnum 2)) ; skip the protocol
	       (if (pending-interrupt?)
		   (goto handle-interrupt 0)  ; pass nargs count (always 0)
		   (goto interpret *code-pointer*))))
	(code (template-code (get-literal 0))))
    (cond ((= 0 (code-vector-ref code 1))
	   (ensure-default-procedure-space! ensure-space)
	   (win))
	  ((and (= big-stack-protocol (code-vector-ref code 1))
		(= 0 (code-vector-ref code (- (code-vector-length code) 3))))
	   (ensure-stack-space! (code-vector-ref16
				   code
				   (- (code-vector-length code) 2))
				ensure-space)
	   (win))
	  (else
	   (raise-exception wrong-type-argument 3 (get-literal 0))))))

; The following is used only for experiments.  The compiler does not use it.

(define-opcode goto-template
  (set-template! (get-literal 0) (enter-fixnum 0))
  (goto interpret *code-pointer*))

; APPLY: *VAL* is the procedure, the rest-arg list is on top of the stack,
; the next two bytes are the number of stack arguments below the rest-args list.
; We check that the rest-arg list is a proper list and let
; PERFORM-APPLICATION-WITH-REST-LIST do the work.
    
(define-opcode apply
  (let ((list-args (pop)))
    (receive (okay? length)
	(okay-argument-list list-args)
      (if okay?
	  (goto perform-application-with-rest-list
		(code-offset 0)
		list-args
		length)
	  (begin
	    (push list-args)
	    (let ((args (pop-args->list*+gc null (+ (code-offset 0) 1))))
	      (raise-exception wrong-type-argument -1 *val* args))))))) ; no next

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
; We grab the counts and the procedure, and clobber the procedure's stack
; slot for GC safety.  Then we get the true stack-arg count and list args
; and again let PERFORM-APPLICATION-WITH-REST-LIST do the work.

(define-opcode closed-apply
  (let* ((nargs (extract-fixnum (pop)))
	 (stack-nargs (extract-fixnum (pop)))
	 (proc (stack-ref stack-nargs)))
    (stack-set! stack-nargs false)
    (receive (okay? stack-arg-count list-args list-arg-count)
	(get-closed-apply-args nargs stack-nargs)
      (if okay?
	  (begin
	    (set! *val* proc)
	    (goto perform-application-with-rest-list
		  stack-arg-count
		  list-args
		  list-arg-count))
	  (begin
	    (push list-args)
	    (let* ((args (pop-args->list*+gc null (+ stack-arg-count 1)))
		   (proc (pop)))
	      (raise-exception wrong-type-argument -1 proc args)))))))

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

(define (install-*val*-closure skip)
  (let ((template (closure-template *val*)))
    (set! *template* template)
    (set-code-pointer! (template-code template) skip)
    (set-current-env! (closure-env *val*))))

(define (check-interrupts-and-go stack-slots stack-arg-count)
  (ensure-stack-space! stack-slots ensure-space)
  (if (pending-interrupt?)
      (goto handle-interrupt stack-arg-count)
      (goto interpret *code-pointer*)))

(define (wrong-nargs stack-arg-count list-args list-arg-count)
  (goto application-exception
	(enum exception wrong-number-of-arguments)
	stack-arg-count list-args list-arg-count))

; Used by RAISE which can't raise an exception when an error occurs.

(define *losing-opcode* 0)

(define (call-exception-handler stack-arg-count opcode)
  (set! *template* *val*)  ; Use *VAL* (a closure) as a marker.
  (set! *losing-opcode* opcode)
  (goto plain-protocol-match stack-arg-count))

(define (call-interrupt-handler stack-arg-count interrupt)
  (set! *template* *val*)  ; Use *VAL* (a closure) as a marker.
  (set! *losing-opcode* (- interrupt))
  (goto plain-protocol-match stack-arg-count))

; Check that the arguments match those needed by *VAL*, which is a closure,
; moving arguments to or from the stack if necessary, and ensure that there
; is enough stack space for the procedure.  The environment needed by *VAL*
; is created.

(define (plain-protocol-match stack-arg-count)
  (goto list-protocol-match stack-arg-count null 0))

(define (list-protocol-match stack-arg-count list-args list-arg-count)
  (let ((code (template-code (closure-template *val*)))
	(total-arg-count (+ stack-arg-count list-arg-count))
	(lose (lambda ()
		(goto wrong-nargs
		      stack-arg-count list-args list-arg-count))))
    (assert (= (enum op protocol)
	       (code-vector-ref code 0)))
    (let loop ((protocol (code-vector-ref code 1))
	       (stack-space default-stack-space))
      (let ((win (lambda (skip stack-arg-count)
		   (install-*val*-closure skip)
		   (goto check-interrupts-and-go
			 stack-space
			 stack-arg-count))))
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
		 (if (< total-arg-count 3)
		     (let ((skip (code-vector-ref code (+ 2 total-arg-count))))
		       (if (= 0 skip)
			   (lose)
			   (begin
			     (push-list list-args list-arg-count)
			     (win skip total-arg-count))))
		     (let ((skip (code-vector-ref code 5)))
		       (if (= 0 skip)
			   (lose)
			   (args+nargs-match skip)))))
		((<= protocol maximum-stack-args)
		 (fixed-match protocol 2))
		((= protocol two-byte-nargs+list-protocol)
		 (n-ary-match (code-vector-ref16 code 2) 4))
		((= protocol args+nargs-protocol)
		 (if (>= total-arg-count
			 (code-vector-ref code 2))
		     (args+nargs-match 3)
		     (lose)))
		((= protocol two-byte-nargs-protocol)
		 (fixed-match (code-vector-ref16 code 2) 4))
		((= protocol big-stack-protocol)
		 (let ((length (code-vector-length code)))
		   (loop (code-vector-ref code (- length 3))
			 (code-vector-ref16 code (- length 2)))))
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
  (cond ((not (vm-eq? *template* *val*))
	 (let ((args (pop-args->list*+gc (copy-list*+gc list-args list-arg-count)
					 stack-arg-count)))
	   (raise-exception* exception -1 *val* args))) ; no next opcode
	((< 0 *losing-opcode*)
	 (error "wrong number of arguments to exception handler"
		*losing-opcode*))
	(else
	 (error "wrong number of arguments to interrupt handler"
		(- *losing-opcode*)))))

;(define (application-exception exception stack-arg-count list-args list-arg-count)
;  (if (vm-eq? *template* *val*)
;      (error "wrong number of arguments to exception handler"
;             *losing-opcode*)
;      (let ((args (pop-args->list* (copy-list* list-args list-arg-count)
;                                   stack-arg-count)))
;        (push (enter-fixnum (current-opcode)))
;        (push (enter-fixnum exception))
;        (push *val*)
;        (push args)
;        (goto raise 2))))

; Get a two-byte number from CODE-VECTOR.

(define (code-vector-ref16 code-vector index)
  (let ((high (code-vector-ref code-vector index)))
    (adjoin-bits high
		 (code-vector-ref code-vector (+ index 1))
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
    (let* ((cont (current-continuation))
	   (code (template-code (continuation-template cont)))
	   (pc (extract-fixnum (continuation-pc cont))))
      (assert (= (enum op protocol)
		 (code-vector-ref code pc)))
      (let ((protocol (code-vector-ref code (+ pc 1))))
	(cond ((or (= protocol 1)
		   (= protocol ignore-values-protocol))
	       (pop-continuation!)
	       (goto continue 1))		; one protocol byte
	      ((= protocol bottom-of-stack-protocol)
	       (let ((cont (get-continuation-from-heap)))
		 (if (continuation? cont)
		     (begin
		       (copy-continuation-from-heap! cont 0)
		       (loop))
		     (goto return-from-vm cont))))
	      ((= protocol call-with-values-protocol)
	       (skip-current-continuation!)
	       (push *val*)
	       (set! *val* (continuation-ref cont continuation-cells))
	       (goto perform-application-with-rest-list 1 null 0))
	      ((= protocol two-byte-nargs+list-protocol)
	       (let ((wants-stack-args (code-vector-ref16 code (+ pc 2))))
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
  (let* ((cont (current-continuation))
	 (code (template-code (continuation-template cont)))
	 (pc (extract-fixnum (continuation-pc cont)))
	 (protocol (code-vector-ref code (+ pc 1))))
    (assert (= (enum op protocol)
	       (code-vector-ref code pc)))
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
	   (skip-current-continuation!)
	   (set! *val* (continuation-ref cont continuation-cells))
	   (goto perform-application-with-rest-list
		 stack-nargs
		 list-args
		 list-arg-count))
	  ((<= protocol maximum-stack-args)
	   (goto fixed-arg-return protocol 1
		 stack-nargs list-args list-arg-count))
	  ((= protocol two-byte-nargs+list-protocol)
	   (goto nary-arg-return (code-vector-ref16 code (+ pc 2)) 3
		 stack-nargs list-args list-arg-count))
	  ((= protocol two-byte-nargs-protocol)
	   (goto fixed-arg-return (code-vector-ref16 code (+ pc 2)) 3
		 stack-nargs list-args list-arg-count))
	  (else
	   (error "unknown protocol" protocol)
	   (goto return-exception stack-nargs list-args)))))

; The continuation wants a fixed number of arguments.  We pop the current
; continuation, move the stack arguments down to the new stack top, push
; any list arguments and off we go.

(define (fixed-arg-return count bytes-used stack-nargs list-args list-arg-count)
  (if (= count (+ stack-nargs list-arg-count))
      (let ((arg-top (top-of-argument-stack)))
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
      (let ((arg-top (top-of-argument-stack)))
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
  (ensure-stack-space! count ensure-space)
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
     

