; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; The stack grows from higher addresses to lower ones.
; *STACK-BEGIN* and *STACK-END* delimit the stack portion of memory.
; *STACK* points to the value at the top of the stack.
; *STACK-LIMIT* is value against which stack requests are checked.  There is a
;   buffer area between the limit and the actual end of the stack.  The buffer
;   is large enough to contain the default procedure stack requirement plus the
;   space needed to make a call to an exception plus an interrupts plus some
;   slack for safety's sake.
;
; Continuations on the stack look like:
;
;					 gc mask
;                                        size of continuation (two bytes)
;  *cont* --> code-pointer ------------> return protocol
;             operand stack              opcodes ...
;             ...
;	      frame values (including template and lexical environment)
;	      ...
;
; Continuations on the heap look like:
;
;                     header
;  tagged pointer --> pc as a fixnum
;		      code vector
;                     operand stack
;                     ...
;                     frame values
;                     ...
; 
; The code that copies continuations back and forth converts between the
; two representations.  Most continuations never get copied off of the
; stack so we usually save a bit of time by not putting headers on them.

; This number of slots at the top of the stack are filled with a funny value
; so that we can detect overruns.

(define stack-warning-limit 30)

; The supposedly unused space at the end of the stack is marked with this
; value so that we can detect incursions.

(define stack-marker #xf0f0f0f)

; We need at least this amount of space for the stack.

(define stack-slack
  (+ default-stack-space
     continuation-stack-size        ; *bottom-of-stack*
     (+ continuation-stack-size 13) ; exceptions need at most 14 values
                                    ; (long pole is external exceptions with
                                    ; exception + procedure-name + 10 irritants +
                                    ; saved-exception + stack-block)
     (+ continuation-stack-size 7)  ; interrupts need at most 7 values
                                    ; (also from examining the code)
     (+ stack-warning-limit 2)))    ; safety

; *BOTTOM-OF-STACK* is a stack continuation that lies a the base of the stack.

(define *stack-begin*)
(define *stack-end*)
(define *stack*)
(define s48-*stack-limit*)
(define *real-stack-limit*)

; Current continuation.  This points not to the top frame on the stack,
; which has no code pointer (because its code pointer is being used by
; the interpreter), but to the top complete frame on the stack.  This
; is updated for non-tail calls and returns.

(define *cont*)

; At the bottom of the stack is a special continuation that is never removed.
; When it is invoked it copies the next continuation out of the heap (if there
; is any such) and invokes that instead.

(define *bottom-of-stack*)
(define *heap-continuation*)

; Initialize the stack and related registers.  All sizes are in descriptors.

(define (initialize-stack+gc start have-size)
  (let ((required-size (+ available-stack-space stack-slack)))
    (receive (start size)
	(if (>= have-size required-size)
	    (values start have-size)
	    (let ((stack (allocate-memory (cells->a-units required-size))))
	      (if (null-address? stack)
		  (error "out of memory, unable to continue"))
	      (values stack required-size)))
      (set! *stack-begin* start)
      (set! *stack-end* (address+ start (cells->a-units size)))
      (set! *real-stack-limit*
	    (address+ *stack-begin* (cells->a-units stack-slack)))
      (set! s48-*stack-limit* *real-stack-limit*)
      (set! *stack* *stack-end*)
      (set! *cont* (integer->address false))
      (do ((a start (address1+ a)))
	  ((address= a *stack-end*))
	(store! a stack-marker))
      (let* ((key (ensure-space return-code-size))
	     (code (make-return-code bottom-of-stack-protocol
				     0		; opcode (never executed)
				     0		; size
				     key)))
	(push false)
	(make-continuation-on-stack (address+ (address-after-header code)
					      return-code-pc)
				    0))	; offset
      (set! *bottom-of-stack* *cont*)
      (set! *heap-continuation* false))))

(define (reset-stack-pointer base-continuation)
  (set! *stack* *bottom-of-stack*)
  (set! *heap-continuation* base-continuation)
  (set! *cont* *bottom-of-stack*))

;----------------
; Utilities

(define (within-stack? p)
  (and (stob? p)
       (>= p (address->integer *stack-begin*))
       (<= p (address->integer *stack-end*))))

(define (stack-size)
  (address-difference *stack-end* *stack-begin*))

; We need an additional 3 descriptors per continuation (code-pointer expands
; to header, code, pc, cont).  A continuation may be as small as one descriptor.
; Our choices are: multiply by four, count the continuations, or check the heap
; as we go.  For now I'll just multiply.

(define (current-stack-size)
  (* 4 (a-units->cells (address-difference *stack-end* *stack*))))

; Add CELLS cells onto the stack.
; The stack grows towards negative memory.

(define (add-cells-to-stack! cells)
  (set! *stack* (address- *stack* (cells->a-units cells))))

(define (the-pointer-before x)
  (address- x (cells->a-units 1)))

(define (push x)     ; check for overflow is done when continuations are pushed
  (add-cells-to-stack! 1)
  (store! *stack* x))

(define (pop)
  (let ((v (fetch *stack*)))
    (add-cells-to-stack! -1)
    v))

; Use the stack as a vector, with (STACK-REF 0) => current top of stack

(define (stack-ref index)
  (fetch (address+ *stack* (cells->a-units index))))

(define (stack-set! index value)
  (store! (address+ *stack* (cells->a-units index)) value))

(define (pointer-to-stack-arguments)
  *stack*)

(define (remove-stack-arguments count)
  (add-cells-to-stack! (- count)))

; Look at the return protocol to see if a continuation is byte code or
; native code.

(define (stack-continuation-is-byte-code? cont)
  (byte-code-return-pointer? (integer->address (stack-cont-code-pointer cont))))

(define (byte-code-return-pointer? pointer)
  (= 0 (bitwise-and (fetch-byte (address+ pointer 1))	; skip protocol marker
		    native-protocol-mask)))

;----------------------------------------------------------------
; Dealing with stack continuations.

; A heuristic - stack continuations must point into the stack and be four-byte
; aligned.

(define (stack-continuation? x)
  (and (address>= x *stack-begin*)
       (address<= x *stack-end*)
       (= 0 (bitwise-and (address->integer x)
			 #b11))))

(define (stack-cont-code-pointer cont)
  (assert (stack-continuation? cont))
  (integer->address (fetch cont)))

(define (set-stack-cont-code-pointer! cont code-pointer)
  (assert (stack-continuation? cont))
  (store! cont (address->integer code-pointer)))

; The +1 is because the size does not include return pointer.

(define (stack-cont-continuation cont)
  (address+ cont
	    (cells->a-units (+ (stack-continuation-size cont)
			       1))))

;----------------------------------------------------------------
; Pending interrupts are signalled by setting the stack limit to -1.
; This allows us to use a single test to check for stack space and
; pending interrupts.

(define (interrupt-flag-set?)
  (address= s48-*stack-limit* (integer->address -1)))

(define (clear-interrupt-flag!)
  (set! s48-*stack-limit* *real-stack-limit*))

(define (set-interrupt-flag!)
  (set! s48-*stack-limit* (integer->address -1)))

(define (stack-space-check space-test)
  (lambda (need)
    (if (space-test need s48-*stack-limit*)
	#f
	(let ((interrupt? (interrupt-flag-set?)))
	  (clear-interrupt-flag!)
	  (if (not (space-test need *real-stack-limit*))
	      (begin
		(s48-copy-stack-into-heap)
		(if (not (space-test need *real-stack-limit*))
		    (error "VM's stack is too small (how can this happen?)"))))
	  interrupt?))))

; S48-*STACK-LIMIT* is offset by DEFAULT-STACK-SPACE to make this test faster.

(define ensure-default-procedure-space!
  (let ((check (stack-space-check (lambda (ignore limit)
				    (address<= limit *stack*)))))
    (lambda ()
      (check 0))))

(define (available-on-stack? space limit)
  (> (+ (a-units->cells			; space on the stack
	   (address-difference *stack* limit))
	default-stack-space)            ; allow for s48-*stack-limit*'s offset
     space))

(define ensure-stack-space! (stack-space-check available-on-stack?))

;----------------------------------------------------------------
; Getting and setting the current continuation.

; Called when replacing the current continuation with a new one.

(define (set-current-continuation! cont)
  (if (continuation? cont)
      (copy-continuation-from-heap! cont 0)
      (reset-stack-pointer cont)))

; For returns (see call.scm).

(define (current-continuation-code-pointer)
;  (format #t "~%[cont code pointer ~S (from ~S)]"
;          (integer->address (fetch *cont*))
;          *cont*)
  (integer->address (fetch *cont*)))

;----------------------------------------------------------------
; Various random operations used by CALL-WITH-VALUES.
;
; Remove the current continuation from the chain.

(define (skip-current-continuation! nargs)
  (set! *cont* (stack-cont-continuation *cont*))
  (move-args-above-cont! nargs))

(define (current-continuation-ref index)
  (fetch (address+ *cont* (cells->a-units (+ index 1)))))

(define (shrink-and-reset-continuation! code-pointer)
  (set! *cont* (address1+ *cont*))
  (set-stack-cont-code-pointer! *cont* code-pointer))

(define (remove-current-frame)
  (set! *stack* *cont*))

;----------------------------------------------------------------

(define (make-continuation-on-stack code-pointer offset)
; (set! *old-cont* *cont*)
  (set! *cont* (address+ *stack* (cells->a-units offset)))
;  (format #t "~%[new cont at ~S with code pointer ~S(~S)]"
;          *cont*
;          code-pointer
;          (fetch-two-bytes (address- code-pointer 2)))
  (store! *cont* (address->integer code-pointer)))

;  (if (and (address<= *stack-begin* *old-cont*)
;           (address<= *old-cont* *stack-end*)
;           (not (address= *old-cont*
;                          (stack-cont-continuation *cont*))))
;      (breakpoint "make-continuation-on-stack"))

(define (push-continuation! code-pointer)
  (push (address->integer code-pointer))
  (set! *cont* *stack*))

; Interrupt and exception continuations come in all sizes.  We push the size
; on the stack just below the return pointer.  The pointer itself has a key
; size that tells to look on the stack.

(define (push-adlib-continuation! code-pointer)
  (push (enter-fixnum (+ (operands-on-stack) 1)))	; count includes itself
  (push-continuation! code-pointer)
  (assert (= (raw-stack-continuation-size *cont*)
	     #xFFFF)))

; Set the stack to point to the current continuation, make the next one down
; be the new current contnuation, and return the code pointer.

(define (pop-continuation-from-stack)
  (set! *stack* *cont*)
;  (format #t "~%[popping cont at ~S with code pointer ~S(~S)]"
;          *stack*
;          (integer->address (fetch *stack*))
;          (stack-continuation-size *cont*))
  (set! *cont* (stack-cont-continuation *cont*))
  (integer->address (pop)))

; Pushing exception data.  We do this in a chunk to ensure that we agree
; with the VM as to which value is where.

(define (push-exception-continuation! code-pointer pc code exception inst-size)
  (add-cells-to-stack! exception-continuation-cells)
  (let ((data (address->stob-descriptor *stack*))
	(size (enter-fixnum (operands-on-stack))))
    (data-init! data exception-cont-size-index             size)
    (data-init! data exception-cont-pc-index               pc)
    (data-init! data exception-cont-code-index             code)
    (data-init! data exception-cont-exception-index        exception)
    (data-init! data exception-cont-instruction-size-index inst-size)
    (push-continuation! code-pointer)))

(define (pop-exception-data)
  (let ((data (address->stob-descriptor *stack*)))
    (add-cells-to-stack! (- exception-continuation-cells))
    (values (data-ref data exception-cont-pc-index)
	    (data-ref data exception-cont-code-index)
	    (data-ref data exception-cont-exception-index)
	    (data-ref data exception-cont-instruction-size-index))))

; The indexes into the data are the indexes into the continuation minus
; the normal continuation cells.  We use raw STORE! and FETCH to avoid
; having to put a temporary header on the data (D-VECTOR-INIT! and
; D-VECTOR-REF have assertions that check for the presence of a header).
;
; We subtract off CONTINUATION-CELLS because the indexes are for heap
; continuations which have that many extra values pushed on top.

(define (data-init! data index value)
  (store! (address+ (address-after-header data)
		    (cells->a-units (- index continuation-cells)))
          value))

(define (data-ref data index)
  (fetch (address+ (address-after-header data)
		   (cells->a-units (- index continuation-cells)))))

;----------------
; Error reporting

(define (report-continuation-uids current-code out)
  (let* ((template (find-template *stack*
				  (operands-on-stack)
				  current-code))
	 (not-first? (maybe-write-template template #f out)))
    (really-show-stack-continuation-uids not-first? out)))

(define (show-stack-continuation-uids)
  (really-show-stack-continuation-uids #f (current-error-port)))

(define (really-show-stack-continuation-uids not-first? out)
  (let loop ((cont *cont*) (not-first? not-first?))
    (if (address= cont *bottom-of-stack*)
	(really-show-heap-continuation-uids *heap-continuation* not-first? out)
	(loop (stack-cont-continuation cont)
	      (maybe-write-template (stack-cont-template cont)
				    not-first?
				    out))))
  0)

(define (show-heap-continuation-uids cont)
  (really-show-heap-continuation-uids cont #f (current-error-port)))

(define (really-show-heap-continuation-uids cont not-first? out)
  (let loop ((cont cont) (not-first? not-first?))
    (if (continuation? cont)
	(loop (continuation-cont cont)
	      (maybe-write-template (heap-cont-template cont)
				    not-first?
				    out)))))

(define (stack-cont-template cont)
  (find-template (address1+ cont)
		 (stack-continuation-size cont)
		 (code-pointer->code (stack-cont-code-pointer cont))))

(define (heap-cont-template cont)
  (find-template (address-after-header cont)
		 (continuation-length cont)
		 (continuation-code cont)))

(define (code-pointer->code code-pointer)
  (let ((pc (fetch-two-bytes (address- code-pointer 5))))
    (address->stob-descriptor (address- code-pointer pc))))

; Look for a template whose code is CODE-VECTOR among the COUNT descriptors
; starting from START.

(define (find-template start count code-vector)
  (let loop ((i 0))
    (if (= i count)
	false
	(let ((next (fetch (address+ start (cells->a-units i)))))
	  (if (and (template? next)
		   (vm-eq? (template-code next)
			   code-vector))
	      next
	      (loop (+ i 1)))))))

; Native code continuations have code vectors instead of templates.
; We could look for a template in the continuation and see if it has
; the same code vector.

(define (maybe-write-template template not-first? out)
  (if not-first?
      (write-string " <- " out))
  (if (template? template)
      (let ((name (template-name template)))
	(cond ((fixnum? name)
	       (write-integer (extract-fixnum name) out))
	      ((and (record? name)
		    (vm-string? (record-ref name 2)))
	       (write-string (extract-string (record-ref name 2)) out))
	      ((and (record? name)
		    (vm-symbol? (record-ref name 2)))
	       (write-string (extract-string
			      (vm-symbol->string (record-ref name 2)))
			     out))
	      (else
	       (write-string "?" out))))
      (write-string " ?? " out))
  #t)

(define (stack-warning-message)
  (do ((a *stack-begin* (address1+ a)))
      ((not (= stack-marker (fetch a)))
       (let ((unused (a-units->cells (address-difference a *stack-begin*))))
	 (if (< unused stack-warning-limit)
	     (begin
	       (newline (current-error-port))
	       (write-string "[Alert: stack overconsumption ("
			     (current-error-port))
	       (write-integer unused (current-error-port))
	       (write-string "); please inform the Scheme 48 implementors]"
			     (current-error-port))
	       (newline (current-error-port))
	       (set! *stack-warning?* #f)))))))

