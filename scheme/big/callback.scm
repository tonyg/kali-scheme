; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This code, along with C code in c/external.c, handles the interaction between
; callbacks from external code to Scheme functions and uses of continuations in
; Scheme.  The problem is that Scheme 48 uses multiple continuations while
; operating with only one process stack.
;
; Suppose we have Scheme procedures s1 and s2 and C procedure c1 such that
; s1 calls c1 and c1 calls s2.  There are two trampoline functions that are
; used to do this.  The VM uses s48_external_call to call c1 and c1 uses
; s48_call_scheme to start the VM running s2.  While in s2 the process stack will
; look like this:
;
;  <C frame for VM running s2>
;  <C frame for s48_call_scheme>
;  <C frame for c1>
;  <C frame for s48_external_call>
;  <C frame for VM running s1>
;  <base>
;
; The C code in c/external.scm keeps a record of the portions of the process
; stack that are running external code.  Each of these stack portions has an
; s48_external_call frame at the base and an s48_call_scheme frame at the top.
; The stack is represented as linked list of records, called `stack-block's,
; each of which contains the following values:
;    free?       ; true if this frame is no longer needed
;    unwind      ; the longjmp target used to skip over this frame
;    proc-name   ; the name of the procedure this block is executing
;    placeholder ; either #f or a placeholder, see the section on threads below
;    next        ; the next stack-block below this one
; These are Scheme records and are traced by the GC.

(define-record-type stack-block :stack-block
  (stack-blocks-are-made-from-c)
  stack-block?
  (free? stack-block-free? set-stack-block-free?!)
  (unwind stack-block-unwind)
  (proc-name stack-block-proc-name)
  (placeholder stack-block-placeholder set-stack-block-placeholder!)
  (next stack-block-next))

; Stack-blocks are made from C, so we need to export the type.

(define-exported-binding "s48-stack-block-type" :stack-block)

; There is no need to keep track of the VM frames.  These are all interchangable
; because 1) the VM's state is kept in top-level variables and 2) we have
; arranged it so that the relevent VM opcodes, call-external-value and
; return-from-callback, are all the same length and are always immediately
; followed by a return instruction.  s48_call_scheme can safely overwrite the
; template and code-pointer registers in the VM as they always point to a
; one-byte instruction followed by a return instruction.  When the VM returns
; from the callback, via a return-from-callback instruction, that too is a
; one-byte instruction followed by a return instruction.  The VM can proceed,
; happily ignorant of all this fooling around.
;
; On entry, s48_external_call saves a longjump target.  This is used when
; raising exceptions from with the external code and for unwinding the process
; stack.  Each invocation of s48_call_scheme creates a new stack-block, saving
; within it the longjump target of the corresponding s48_external_call.  `Free?'
; and `placeholder' are initially false and `next' points to existing list of
; stack-blocks.
;
; When a callback returns to s48_call_scheme, the corresponding block is popped
; off the list of stack-blocks.
;
; So far so good, and if that were all that happened there would be no need for
; all this mechanism.  There are two problems: call/cc and threads.  Call/cc is
; simpler to deal with.  We have downward continuations in C, as implemented
; by longjmp(), so we simply limit continuations that cross callbacks to being
; downwards only.  We also need to arrange for any jumped-over stack portions
; to be popped off of the stack.
;
; The popping off is handled by s48_external_call.  Just before returning to the
; VM it checks to see if the top stack-block is free.  If so, it loops through
; the list of stack-blocks to find the first non-free stack portion.  A longjump
; is performed to the target in the last free block, removing any unneeded frames
; from the stack.
;
; s48_call_scheme starts the VM running the following CALLBACK procedure.  The
; arguments are BLOCK, the stack-block just created for this callback, and
; the procedure and arguments for the actual callback.  It prevents jumps back
; into the callback and frees BLOCK if a throw out occurs.
;
; We disable interrupts to ensure that nothing intervenes between setting DONE?
; and returning from the callback.  BLOCK is then either freed or returned to,
; but not both or neither.  RETURN-FROM-CALLBACK reenables interrupts.

(define (callback block proc . args)
  (let ((done? #f))
    (return-from-callback block
			  (dynamic-wind
			   (lambda ()
			     (if done?
				 (error "attempt to throw into a callback"
					(cons proc args))))
			   (lambda ()
			     (let ((result (apply proc args)))
			       (disable-interrupts!)
			       (set! done? #t)
			       result))
			   (lambda ()
			     (if (not done?)
				 (begin
				   (set! done? #t)
				   (set-stack-block-free?! block #t)
				   (clear-stack-top!))))))))

(define-exported-binding "s48-callback" callback)

; CLEAR-STACK-TOP! is an empty C procedure.  When it returns, s48_external_call
; will automatically clear any free frames off of the stack.

(import-lambda-definition clear-stack-top! () "s48_clear_stack_top")

; Dealing with threads.
;
; The difficulty here is that each stack-block belongs to some thread.  Thread A
; can call a C procedures which calls back into Scheme.  At that point a context
; switch occurs and we start running thread B, which promptly does the same
; calls.  THe process stack then looks like this:
; 
;  <C frame for VM running B1>
;  <C frame for s48_call_scheme>
;  <C frame for B's c code>
;  <C frame for s48_external_call>
;  <C frame for VM running A1 and then B0>
;  <C frame for s48_call_scheme>
;  <C frame for A's c code>
;  <C frame for s48_external_call>
;  <C frame for VM running A0>
;  <base>
;
; At this point A cannot return from its callback before B does, because B's
; portion of the process stack is above A's.  If A does try to return it must
; block until it again is at the top of the stack.
;
; This is handled by s48_call_scheme, which checks to see if the stack-block
; being returned to is at the top of the stack.  If not, it does a second
; callback to DELAY-CALLBACK-RETURN, defined below, with the same stack-block.
; DELAY-CALLBACK-RETURN creates a placeholder, puts it in the stack-block, and
; then blocks on it.  When the placeholder gets a value the procedure attempts
; another return-from-callback.
;
; This is called with interrupts disabled, as we need to avoid having BLOCK
; reach the top of the stack before the placeholder is installed.

(define (delay-callback-return block value)
  (let ((placeholder (make-placeholder)))
    (set-stack-block-placeholder! block placeholder)
    (enable-interrupts!)
    (placeholder-value placeholder)
    value))

(define-exported-binding "s48-delay-callback-return" delay-callback-return)

; Finally, s48_external_call looks to see if the top stack-block has a
; placeholder.  If it does, it raises an exception instead of doing a normal
; return.  The exception handler sets the placeholder's value, allowing the
; blocked thread to continue.  The handler then returns the external call's
; value to its own thread, or, if the callback-return-uncovered is piggybacked
; on another exception, we raise that exception.
;
; Because of the all of the games played above, the callback-return-uncovered
; exception may appear to have come from either the call-external-value, or
; return-from-callback opcodes.

(define uncovered-return-handler
  (lambda (opcode reason . args)
    (if (= reason (enum exception callback-return-uncovered))
	(call-with-values
	 (lambda ()
	   (if (= 2 (length args))
	       (values (car args)
		       (cadr args)
		       #f)
	       (let ((args (reverse args)))
		 (values (car args)
			 (cadr args)
			 (reverse (cddr args))))))
	 (lambda (block return-value exception-args)
	   (let ((placeholder (stack-block-placeholder block)))
	     (set-stack-block-placeholder! block #f)
	     (placeholder-set! placeholder #t)
	     (if exception-args
		 (apply signal-vm-exception opcode return-value exception-args)
		 return-value))))
	(apply signal-vm-exception opcode reason args))))

(define (block-depth block)
  (if block
      (+ 1 (block-depth (stack-block-next block)))
      0))

(for-each (lambda (opcode)
	    (define-vm-exception-handler opcode uncovered-return-handler))
	  (list (enum op call-external-value)
		(enum op return-from-callback)))

;----------------
; Utility for the common case of calling an imported binding.

(define (call-imported-binding proc . args)
  (if (and (shared-binding? proc)
	   (shared-binding-is-import? proc))
      (let ((value (shared-binding-ref proc)))
	(if (byte-vector? value)
	    (apply call-external-value
		   value
		   (shared-binding-name proc)
		   args)
	    (apply call-error "bad procedure" call-imported-binding proc args)))
      (apply call-error "bad procedure" call-imported-binding proc args)))

;----------------
; We export the record-type type so that external code can check to see if
; supposed record types really are such.

(define-exported-binding "s48-the-record-type" :record-type)

;----------------
; Testing
;
; `s48_trampoline' is a C routine that calls its Scheme argument with between
; zero and three arguments.  The arguments are 100, 200, and 300.
;
;(import-lambda-definition trampoline (proc nargs)
;                          "s48_trampoline")
;
;(define (foo . args)
;  (for-each display (list "[foo " args "]"))
;  (newline)
;  (cons 'foo-return args))
;
;; This should return 1100.
;
;(define (test0)
;  (trampoline (lambda ()
;                (call-with-current-continuation
;                  (lambda (c)
;                    (trampoline (lambda (x)
;                                  (c (+ x 1000)))
;                                1))))
;              0))
;
;; ,open threads locks debug-messages
;
;(define (test1 error?)
;  (let ((lock (make-lock))
;        (repl-lock (make-lock)))
;    (obtain-lock repl-lock)
;    (spawn (lambda ()
;             (obtain-lock lock)
;             (debug-message "A returned "
;                           (trampoline (lambda ()
;                                         (obtain-lock lock) ; we block
;                                         'a)
;                                       0))
;             (release-lock repl-lock))
;           'thread-a)
;    (spawn (lambda ()
;             (debug-message "B returned "
;                           (trampoline (lambda ()
;                                         (release-lock lock)    ; A can run
;                                         (relinquish-timeslice) ; let A run
;                                         (if error? #f 'b))
;                                       0)))
;           'thread-b)
;    (obtain-lock repl-lock)))
