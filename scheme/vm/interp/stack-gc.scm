
; Called when returning off of the end of the stack.

(define (get-continuation-from-heap)
  *heap-continuation*)

; Copy CONT from heap onto stack just above *BOTTOM-OF-STACK*, linking it
; to *BOTTOM-OF-STACK* and *BOTTOM-OF-STACK* to CONT's continuation.
; There are STACK-ARG-COUNT arguments on top of the stack that need to be
; preserved.

(define (copy-continuation-from-heap! cont stack-arg-count)
  (assert (continuation? cont))
  (let* ((stack-size (- (continuation-length cont) 2))
	 (new-cont (address- *bottom-of-stack* (cells->a-units stack-size))))
    (copy-args-above-incoming-cont! new-cont stack-arg-count)
    (set! *cont* new-cont)
    (copy-memory! (address+ (address-after-header cont)
			    (cells->a-units 3))	; skip PC, code, cont
		  (address1+ new-cont)
		  (cells->bytes (- stack-size 1)))	; skip code pointer
    (set-stack-cont-code-pointer!
      new-cont
      (address+ (address-after-header (continuation-code cont))
		(extract-fixnum (continuation-pc cont))))
    (set! *heap-continuation* (continuation-cont cont))
    new-cont))

; Move STACK-ARG-COUNT values pointed to by *STACK* on top of NEW-CONT.

(define (copy-args-above-incoming-cont! new-cont stack-arg-count)
  (if (= stack-arg-count 0)
      (set! *stack* new-cont)
      (let ((new-stack (address- new-cont (cells->a-units stack-arg-count))))
	(if (address< new-stack *stack*)
	    (begin
	      (copy-memory! *stack*
			    new-stack
			    (cells->bytes stack-arg-count))
	      (set! *stack* new-stack))))))

;----------------
; Copying the stack into the heap because there is no more room on the
; stack.  This preserves the continuation and then moves any arguments
; down on top of the current continuation.

(define (s48-copy-stack-into-heap)
  (let ((key (ensure-space (current-stack-size)))
	(arg-count (operands-on-stack))
	(top *stack*))
    (preserve-continuation key)
    (set! *stack* *cont*)
    (move-stack-arguments! top arg-count)))

; Returns the number of arguments pushed above the topmost object on the stack.

(define (operands-on-stack)
  (a-units->cells (address-difference *cont* *stack*)))

; Move NARGS values from TOP-OF-ARGS to the current top of the stack.

(define (move-stack-arguments! top-of-args nargs)
  (let ((start-arg (address+ top-of-args (cells->a-units (- nargs 1)))))
    (do ((loc (the-pointer-before *stack*) (the-pointer-before loc))
	 (arg start-arg (the-pointer-before arg)))
	((address< arg top-of-args)
	 (add-cells-to-stack! nargs))
      (store! loc (fetch arg)))))

; Copy NARGS arguments from the top of the stack to just above CONT, if
; they are not already there.

(define (move-args-above-cont! nargs)
  (let ((top-of-args (pointer-to-stack-arguments)))
    (if (not (address= *cont*
		       (address+ top-of-args
				 (cells->a-units nargs))))
	(begin
	  (set! *stack* *cont*)
	  (move-stack-arguments! top-of-args nargs)))))

; Migrating the current continuation into the heap, saving the environment
; first.  The heap space needed is no more than the current stack size.

(define current-continuation-size current-stack-size)

(define (copy-current-continuation-to-heap key)
  (preserve-continuation key))

(define (preserve-continuation key)
  (if (false? (address->integer *cont*))
      false
      (really-preserve-continuation key)))

(define (really-preserve-continuation key)
  (if (not (address= *cont* *bottom-of-stack*))
      (let ((temp *heap-continuation*))
	(let loop ((cont *cont*) (previous false))
	  (if (address= cont *bottom-of-stack*)
	      (set-continuation-cont! previous temp)
	      (receive (new next)
		  (copy-continuation-to-heap! cont key)
		(if (continuation? previous)
		    (set-continuation-cont! previous new)
		    (set! *heap-continuation* new))
		(loop next new))))
	(set! *cont* *bottom-of-stack*)))
  *heap-continuation*)

; The continuation has three values added: PC, code vector, and the next
; continuation (filled in by REALLY-PRESERVE-CONTINUATION).

(define (copy-continuation-to-heap! cont key)
  (let* ((size (stack-continuation-size cont))
	 (new (make-continuation (+ size 3) key)))
    (receive (pc byte-vector)
	(decode-return-pointer cont)
      (copy-memory! (address1+ cont)
		    (address+ (address-after-header new)
			      (cells->a-units 3))
		    (cells->bytes size))
      (set-continuation-pc! new (enter-fixnum pc))
      (set-continuation-code! new byte-vector))
    (values new
	    (address+ cont (cells->bytes (+ size 1))))))

; Use the offset found three bytes before POINTER to convert POINTER into
; a program counter and a code vector.

(define (decode-return-pointer cont)
  (let* ((pointer (stack-cont-code-pointer cont))
	 (pc (fetch-two-bytes (address- pointer 5))))
    (values pc
	    (address->stob-descriptor (address- pointer pc)))))

; Returns the two-byte value found just ahead of where CONT's code pointer
; points.  The RAW part refers to the fact that we don't check for a zero
; size, which is an escape value.

(define (raw-stack-continuation-size cont)
  (fetch-two-bytes (address- (stack-cont-code-pointer cont)
			     2)))

; Get the raw size and check for zero, which means that the real size is found
; just after the VM's saved registers.  This is used for interrupt and exception
; continuations.

(define (stack-continuation-size cont)
  (let ((size (raw-stack-continuation-size cont)))
    (if (= size #xffff)
	(extract-fixnum (fetch (address1+ cont)))
	size)))

; Silly utility that should be elsewhere.

(define (fetch-two-bytes pointer)
  (+ (shift-left (fetch-byte pointer) 8)
     (fetch-byte (address+ pointer 1))))

;----------------
; Tracing the stack for garbage collection - first trace any arguments pushed
; above the current continuation, then loop down the continuations, tracing
; each one along with its environment (if the environment has not yet been
; done).

(define *stack-warning?* #t)

(define (trace-stack)
  (if *stack-warning?*
      (stack-warning-message))
  (s48-trace-locations! *stack*
			(address+ *stack*
				  (cells->a-units (operands-on-stack))))
  (let loop ((cont *cont*))
    (receive (pc code-vector)
	(decode-return-pointer cont)
      (trace-stack-continuation-contents cont code-vector pc)
      (if (not (address= cont *bottom-of-stack*))
	  (loop (stack-cont-continuation cont)))))
  (set! *heap-continuation* (s48-trace-value *heap-continuation*)))

; We cannot count on any headers being in place, so we can't use
; the standard accessors because they have assertions that look at
; the headers.

(define (trace-stack-continuation-contents cont code-vector pc)
  (let ((code-pointer (stack-cont-code-pointer cont)))
    (let ((new-code (s48-trace-value code-vector))
	  (mask-size (fetch-byte (address- code-pointer 3)))
	  (contents-pointer (address1+ cont)))
      (set-stack-cont-code-pointer! cont
				    (address+ (address-after-header new-code)
					      pc))
      (if (= mask-size 0)
	  (s48-trace-locations! contents-pointer
				(address+ contents-pointer
					  (cells->a-units
					    (stack-continuation-size cont))))
	  (s48-trace-continuation-contents! contents-pointer
					    code-pointer
					    mask-size))
      0)))		; Argh! PreScheme compiler problem.

; This should be in arch.scm
(define template-code-index 0)

(define (raw-template-code template)
  (fetch (address+ (address-after-header template)
		   template-code-index)))

; Reverse eta to allow redefinition during debugging.
(add-gc-root! (lambda () (trace-stack)))

;----------------------------------------------------------------
; Temporary code to check that the stack is okay.

(define *oldspace-begin*)
(define *oldspace-end*)

(define (check-stack)
  (set! *oldspace-begin* (address->integer (s48-oldspace-begin)))
  (set! *oldspace-end*   (address->integer (s48-oldspace-end)))
  (let loop ((cont *cont*))
    (if (not (address= cont *bottom-of-stack*))
	(loop (check-continuation cont)))))

(define (check-continuation cont)
  (receive (pc code-vector)
      (decode-return-pointer cont)
    (check-stack-continuation-contents cont code-vector pc))
  (stack-cont-continuation cont))

(define (check-stack-continuation-contents cont code-vector pc)
  (let ((code-pointer (stack-cont-code-pointer cont)))
    (let ((mask-size (fetch-byte (address+ code-pointer gc-mask-size-offset)))
	  (contents-pointer (address1+ cont)))
      (if (= mask-size 0)
	  (check-locations contents-pointer
			   (stack-cont-continuation cont))
	  (check-continuation-contents contents-pointer
				       code-pointer
				       mask-size))
      0)))		; Argh! PreScheme compiler problem.

(define (check-locations start end)
  (let loop ((addr start))
    (if (address< addr end)
	(begin
	  (check-descriptor (fetch addr))
	  (loop (address1+ addr)))))
  0)

					; -1 -2        frame size
(define gc-mask-size-offset -3)		; -3           gc mask size
					; -4 -5        offset
(define gc-mask-offset      -6)         ; -6 ...       mask

(define (check-continuation-contents contents-pointer code-pointer mask-size)
  (let ((mask-pointer (address+ code-pointer (+ gc-mask-offset 1))))
    (let byte-loop ((mask-ptr (address- mask-pointer mask-size))
		    (trace-ptr contents-pointer))
      (if (not (address= mask-ptr mask-pointer))
	  (let bit-loop ((mask (fetch-byte mask-ptr)) (ptr trace-ptr))
	    (if (= mask 0)
		(byte-loop (address+ mask-ptr 1)
			   (address+ trace-ptr (cells->a-units 8)))
		(begin
		  (if (odd? mask)
		      (check-descriptor (fetch ptr)))
		  (bit-loop (arithmetic-shift-right mask 1)
			    (address1+ ptr)))))))))
  
(define (odd? x)
  (= (bitwise-and x 1)
     1))

(define (check-descriptor x)
  (if (cond ((header? x) #t)
	    ((stob? x)
	     (let ((address (remove-stob-tag x)))
	       (and (<= *oldspace-begin* address)
		    (<= address *oldspace-end*))))
	    (else #f))
      (error "bad descriptor in stack" x)))

