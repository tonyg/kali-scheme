; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code to handle the calling conventions.

; *VAL* is the procedure, the arguments are on stack, and the next byte
; is the protocol.  This checks that *VAL* is in fact a closure and checks
; for the common case of a non-n-ary procedure that has few arguments.
; The common case is handled directly and all others are passed off to
; PLAIN-PROTOCOL-MATCH.

(define-opcode call
  (let ((stack-arg-count (code-byte 0)))
    (if (closure? *val*)
	(let* ((template (closure-template *val*))
	       (code (template-code template)))
	  (if (= stack-arg-count (code-vector-ref code 1))
	      (begin
		(set! *template* template)
		(set-current-env! (closure-env *val*))
		(set-code-pointer! code 2)
		(ensure-default-procedure-space! ensure-space)
		(if (pending-interrupt?)
		    (goto handle-interrupt stack-arg-count)
		    (goto interpret *code-pointer*)))
	      (goto plain-protocol-match stack-arg-count)))
	(goto application-exception
	      (enum exception bad-procedure)
	      stack-arg-count null 0))))

; Two bytes of arguments.

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
		   (goto handle-interrupt (code-byte 2))  ; pass nargs count
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

; The following two instructions are used only for experiments.  The compiler
; does not normally use them.
;
; Same as op/call except that the arguments are moved to just above the
; current continuation before the call is made.  For non-tail calls and some
; tail-calls the arguments will already be there.

(define-opcode move-args-and-call
  (let ((nargs (code-byte 0)))
    (move-args-above-cont! nargs)
    (goto perform-application nargs)))

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
		(code-offset 0) list-args length)
	  (begin
	    (push list-args)
	    (let ((args (pop-args->list* null (+ (code-offset 0) 1))))
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
	    (let ((args (pop-args->list* null (+ stack-arg-count 1))))
	      (pop)	; remove procedure
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
  (let ((code (template-code (closure-template *val*)))
	(lose (lambda ()
		(goto wrong-nargs stack-arg-count null 0))))
    (assert (= (enum op protocol)
	       (code-vector-ref code 0)))
    (let loop ((protocol (code-vector-ref code 1))
	       (stack-space default-stack-space))
      (let ((win (lambda (skip stack-arg-count)
		   (install-*val*-closure skip)
		   (goto check-interrupts-and-go stack-space stack-arg-count))))
	(let (;; Fixed number of arguments.
	      (fixed-match (lambda (wants skip)
			     (if (= wants stack-arg-count)
				 (win skip stack-arg-count)
				 (lose))))
	      ;; N-ary procedure.
	      (n-ary-match (lambda (wants-stack-args skip)
			     (if (<= wants-stack-args stack-arg-count)
				 (begin
				   (rest-list-setup wants-stack-args
						    stack-arg-count
						    null
						    0)
				   (win skip (+ wants-stack-args 1)))
				 (lose))))
	      ;; Push the rest list, the total number of arguments, and the
	      ;; number on the stack arguments onto the stack.
	      (args+nargs-match (lambda (skip)
				  (push null)
				  (push (enter-fixnum stack-arg-count))
				  (push (enter-fixnum stack-arg-count))
				  (win skip (+ stack-arg-count 3)))))
	  (cond ((= protocol nary-dispatch-protocol)
		 (if (< stack-arg-count 3)
		     (let ((skip (code-vector-ref code (+ 2 stack-arg-count))))
		       (if (= 0 skip)
			   (lose)
			   (win skip stack-arg-count)))
		     (let ((skip (code-vector-ref code 5)))
		       (if (= 0 skip)
			   (lose)
			   (args+nargs-match skip)))))
		((= protocol args+nargs-protocol)
		 (if (>= stack-arg-count
			 (code-vector-ref code 2))
		     (args+nargs-match 3)
		     (lose)))
		((= protocol two-byte-nargs+list-protocol)
		 (n-ary-match (code-vector-ref16 code 2) 4))
		((<= protocol maximum-stack-args)
		 (fixed-match protocol 2))
		((= protocol two-byte-nargs-protocol)
		 (fixed-match (code-vector-ref16 code 2) 4))
		((= protocol big-stack-protocol)
		 (let ((length (code-vector-length code)))
		   (loop (code-vector-ref code (- length 3))
			 (code-vector-ref16 code (- length 2)))))
		(else
		 (error "unknown protocol" protocol)
		 (lose))))))))

; Same thing, except that there is an additional list of arguments.

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
		   (goto check-interrupts-and-go stack-space stack-arg-count))))
	(let (;; Fixed number of arguments.
	      (fixed-match (lambda (wants skip)
			     (if (= wants total-arg-count)
				 (begin
				   (push-list list-args list-arg-count)
				   (win skip total-arg-count))
				 (lose))))
	      ;; N-ary procedure.
	      (n-ary-match (lambda (wants-stack-args skip)
			     (if (<= wants-stack-args total-arg-count)
				 (begin
				   (rest-list-setup wants-stack-args
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
				    (rest-list-setup (max stack-arg-count
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
		((= protocol args+nargs-protocol)
		 (if (>= total-arg-count
			 (code-vector-ref code 2))
		     (args+nargs-match 3)
		     (lose)))
		((<= protocol maximum-stack-args)
		 (fixed-match protocol 2))
		((= protocol two-byte-nargs+list-protocol)
		 (n-ary-match (code-vector-ref16 code 2) 4))
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

(define (rest-list-setup wants-stack-args stack-arg-count list-args list-arg-count)
  (cond ((= stack-arg-count wants-stack-args)
	 (push (copy-list* list-args list-arg-count)))
	((< stack-arg-count wants-stack-args)
	 (let ((count (- wants-stack-args stack-arg-count)))
	   (push (copy-list* (push-list list-args count)
			     (- list-arg-count count)))))
	(else ; (> stack-arg-count wants-stack-args)
	 (let ((count (- stack-arg-count wants-stack-args)))
	   (push (pop-args->list* (copy-list* list-args list-arg-count)
				  count))))))

; Raise an exception, passing to it a list of the arguments on the stack and
; in LIST-ARGS.

(define (application-exception exception
			       stack-arg-count list-args list-arg-count)
  (cond ((not (vm-eq? *template* *val*))
	 (let ((args (pop-args->list* (copy-list* list-args list-arg-count)
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

(define (copy-list* list length)
  (if (= length 0)
      null
      (receive (key list)
	  (ensure-space-saving-temp (* vm-pair-size length) list)
	(let ((res (vm-cons (vm-car list) null key)))
	  (do ((l (vm-cdr list) (vm-cdr l))
	       (last res (let ((next (vm-cons (vm-car l) null key)))
			   (vm-set-cdr! last next)
			   next)))
	      ((vm-eq? null l)
	       res))))))

; Pop COUNT arguments into a list with START as the cdr.

(define (pop-args->list* start count)
  (receive (key start)
      (ensure-space-saving-temp (* vm-pair-size count) start)
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
     

