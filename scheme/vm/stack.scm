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
;                                        size of continuation (two bytes)
;  *cont* --> code-pointer ------------> return protocol
;             other saved registers      opcodes ...
;             ...
;             saved argument stack
;             ...
;
; Continuations on the heap look like:
;
;                     header
;  tagged pointer --> pc as a fixnum
;                     other saved registers
;                     ...
;                     saved argument stack
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
(define *stack-limit*)

; Current continuation
(define *cont*)

; For tracking the reason for copying closures and environments (used for
; debugging and gathering statistics).

(define-enumeration copy
  (closure
   overflow
   preserve))

; At the bottom of the stack is a special continuation that is never removed.
; When it is invoked it copies the next continuation out of the heap (if there
; is any such) and invokes that instead.

(define *bottom-of-stack*)

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
      (set! *stack-limit* (address+ *stack-begin* (cells->a-units stack-slack)))
      (set! *stack* *stack-end*)
      (set! *cont* (integer->address false))
      (set! *env* quiescent)
      (do ((a start (address1+ a)))
	  ((address= a *stack-end*))
	(store! a stack-marker))
      (let* ((key (ensure-space (op-template-size 6)))
	     (template (make-template-containing-six-ops
			 (enum op cont-data)
			 0
			 continuation-cells
			 (enum op protocol)
			 bottom-of-stack-protocol
			 0
			 key)))
	(push-continuation-on-stack template
				    (template+pc->code-pointer template 3)))
      (set! *bottom-of-stack* *cont*))))

(define (reset-stack-pointer base-continuation)
  (set! *stack* *bottom-of-stack*)
  (set-stack-cont-cont! *bottom-of-stack* base-continuation)
  (set! *cont* *bottom-of-stack*))

;----------------
; Utilities

(define (within-stack? p)
  (and (stob? p)
       (>= p (address->integer *stack-begin*))
       (<= p (address->integer *stack-end*))))

(define (stack-size)
  (address-difference *stack-end* *stack-begin*))

; The stob-overhead is to get room for one vector that is made.
(define (current-stack-size)
  (+ stob-overhead (a-units->cells (address-difference *stack-end* *stack*))))

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

;----------------
; Dealing with stack continuations.

(define (stack-continuation? x)
  (and (address>= x *stack-begin*)
       (address<= x *stack-end*)
       (fixnum? (address->integer x))))		; check for low bits = 0

(define-syntax define-cont-field
  (syntax-rules ()
    ((define-cont-field ref set offset)
     (begin
       (define (ref c)
	 (assert (stack-continuation? c))
	 (fetch (address+ c (cells->a-units offset))))
       (define (set c val)
	 (assert (stack-continuation? c))
	 (store! (address+ c (cells->a-units offset)) val))))))

(define-cont-field stack-cont-code-pointer set-stack-cont-code-pointer!
  continuation-pc-index)
(define-cont-field stack-cont-cont set-stack-cont-cont!
  continuation-cont-index)
(define-cont-field stack-cont-template set-stack-cont-template!
  continuation-template-index)
(define-cont-field stack-cont-env set-stack-cont-env!
  continuation-env-index)

(define (stack-cont-ref c index)
  (assert (stack-continuation? c))
  (fetch (address+ c (cells->a-units index))))

(define (stack-cont-pc c)
  (enter-fixnum 
    (address-difference (integer->address (stack-cont-code-pointer c))
			(address-after-header
			  (template-code (stack-cont-template c))))))

(define (continuation-code-pointer cont)
  (template+pc->code-pointer (continuation-template cont)
			     (extract-fixnum (continuation-pc cont))))

(define (template+pc->code-pointer template pc)
  (address+ (address-after-header (template-code template))
	    pc))

(define (current-continuation-code-pointer)
  (integer->address (stack-cont-code-pointer *cont*)))
   
(define (current-continuation-ref index)
  (stack-cont-ref *cont* index))

;----------------
; *STACK-LIMIT* is offset by DEFAULT-STACK-SPACE to make this test faster.

(define (ensure-default-procedure-space!)
  (if (address< *stack* *stack-limit*)
      (begin
	(s48-copy-stack-into-heap)
	(if (address< *stack* *stack-limit*)
	    (error "Couldn't get default procedure space (how can this happen?)"))))
  0)

; Compiler is not allowed to use more than one stack's worth of space for
; any given procedure.

(define (ensure-stack-space! space)
  (if (not (available-on-stack? space))
      (begin
	(s48-copy-stack-into-heap)
	(if (not (available-on-stack? space))
	    (error "VM's stack is too small (how can this happen?)")))))

(define (available-on-stack? space)
  (> (+ (a-units->cells			; space on the stack
	   (address-difference *stack* *stack-limit*))
	default-stack-space)            ; allow for *stack-limit*'s offset
     space))

;----------------
; Getting and setting the current continuation.

; Called when replacing the current continuation with a new one.

(define (set-current-continuation! cont)
  (if (continuation? cont)
      (copy-continuation-from-heap! cont 0)
      (reset-stack-pointer cont)))

; For multiple-value returns (see call.scm).

(define (skip-current-continuation!)
  (let ((next (stack-cont-cont *cont*)))
    (if (address= *cont* *bottom-of-stack*)
	(set-stack-cont-cont! *cont* (continuation-cont next))
	(set! *cont* (integer->address next)))))

; Called when returning off of the end of the stack.

(define (get-continuation-from-heap)
  (stack-cont-cont *bottom-of-stack*))

; Copy CONT from heap onto stack just above *BOTTOM-OF-STACK*, linking it
; to *BOTTOM-OF-STACK* and *BOTTOM-OF-STACK* to CONT's continuation.
; There are STACK-ARG-COUNT arguments on top of the stack that need to be
; preserved.

(define (copy-continuation-from-heap! cont stack-arg-count)
  (assert (continuation? cont))
  (let ((new-cont (address- *bottom-of-stack*
			    (cells->a-units (continuation-length cont)))))
    (if (= stack-arg-count 0)
	(set! *stack* new-cont)
	(let ((new-stack (address- new-cont (cells->a-units stack-arg-count))))
	  (if (address< new-stack *stack*)
	      (begin
		(copy-memory! *stack*
			      new-stack
			      (cells->bytes stack-arg-count))
		(set! *stack* new-stack)))))
    (set! *cont* new-cont)
    (copy-memory! (address1+ (address-after-header cont))
		  (address1+ new-cont)
		  (cells->bytes (- (continuation-length cont) 1)))
    (set-stack-cont-code-pointer! new-cont
				  (address->integer
				    (continuation-code-pointer cont)))
    (set-stack-cont-cont! *bottom-of-stack* (stack-cont-cont new-cont))
    (set-stack-cont-cont! new-cont (address->integer *bottom-of-stack*))
    new-cont))

(define (push-continuation-on-stack template code-pointer)
  (add-cells-to-stack! continuation-cells)
  (let ((cont *stack*))
    (set-stack-cont-code-pointer! cont (address->integer code-pointer))
    (set-stack-cont-template!     cont template)
    (set-stack-cont-env!          cont *env*)
    (set-stack-cont-cont!         cont (address->integer *cont*))
    (set! *cont* cont)))

(define (pop-continuation-from-stack set-template&code-pointer!)
  (let ((cont *cont*))
    (set-template&code-pointer!    (stack-cont-template     cont)
		 (integer->address (stack-cont-code-pointer cont)))
    (set! *env*                    (stack-cont-env          cont))
    (set! *cont* (integer->address (stack-cont-cont         cont)))
    (set! *stack* (address+ cont
			    (cells->a-units continuation-cells)))))

; A native continuation has the code pointer on top; we remove everything
; above the continuation and then return the code pointer.

(define (pop-native-continuation-from-stack)
  (set! *stack* *cont*)
  (pop))

; Pushing exception data.  We do this in a chunk to ensure that we agree
; with the VM as to which value is where.

(define (push-exception-data template pc exception inst-size)
  (let ((size (enter-fixnum (+ (arguments-on-stack)
			       continuation-cells
			       exception-continuation-cells))))
    (add-cells-to-stack! exception-continuation-cells)
    (let ((data (address->stob-descriptor *stack*)))
      (data-init! data exception-cont-size-index             size)
      (data-init! data exception-cont-pc-index               pc)
      (data-init! data exception-cont-template-index         template)
      (data-init! data exception-cont-exception-index        exception)
      (data-init! data exception-cont-instruction-size-index inst-size))))

(define (pop-exception-data)
  (let ((data (address->stob-descriptor *stack*)))
    (add-cells-to-stack! (- exception-continuation-cells))
    (values (data-ref data exception-cont-pc-index)
	    (data-ref data exception-cont-template-index)
	    (data-ref data exception-cont-exception-index)
	    (data-ref data exception-cont-instruction-size-index))))

; The indexes into the data are the indexes into the continuation minus
; the normal continuation cells.  We use raw STORE! and FETCH to avoid
; having to put a temporary header on the data (D-VECTOR-INIT! and
; D-VECTOR-REF have assertions that check for the presence of a header).

(define (data-init! data index value)
  (store! (address+ (address-after-header data)
		    (cells->a-units (- index continuation-cells)))
          value))

(define (data-ref data index)
  (fetch (address+ (address-after-header data)
		   (cells->a-units (- index continuation-cells)))))

;----------------
; Copying the stack into the heap because there is no more room on the
; stack.  This preserves the continuation and then moves any arguments
; down on top of the current continuation.

(define (s48-copy-stack-into-heap)
  (let ((key (ensure-space (current-stack-size)))
	(arg-count (arguments-on-stack))
	(top *stack*))
    (preserve-continuation key (enum copy overflow))
    (set! *stack* *cont*)
    (move-stack-arguments! top arg-count)))

; Returns the number of arguments pushed above the topmost object on the stack.
; We have to walk down looking to see if the base of the current argument stack
; is the mortal remains of an environment that has been copied to the heap.
;
; Reasons for quitting:
;  - we have reached *CONT*
;  - we have reached the grave marker of a copied-to-the-heap environment
;  - we have reached an environment that is no longer live but was not copied

(define (arguments-on-stack)
  (let loop ((p (pointer-to-stack-arguments))
	     (i 0))
    (cond ((address= p *cont*)
	   i)
	  ((= (fetch p) argument-limit-marker)
	   (- i 1))  ; marker is one past the original header location
	  ((header? (fetch p))
	   i)
	  (else
	   (loop (address1+ p)
		 (+ i 1))))))

; This is used by the environment code to mark environments that have been
; migrated to the heap (their header gets clobbered during the of migration).
; Any header type will work, because headers are never pushed as arguments.

(define argument-limit-marker (make-header (enum stob channel) 0))

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
  (preserve-continuation key (enum copy preserve)))

(define (preserve-continuation key reason)
  (if (false? (address->integer *cont*))
      false
      (really-preserve-continuation key reason)))

; 1. Preserve the current lexical environment
; 2. Loop down the continuations copying them into the heap, including their
;    lexical environments
; 3. Copy the current continuation back onto the stack

(define (really-preserve-continuation key reason)
  (preserve-current-env-with-reason key reason)
  (let ((end (stack-cont-cont *bottom-of-stack*)))
    (let loop ((cont *cont*) (previous false))
      (if (address= cont *bottom-of-stack*)
	  (if (continuation? previous)
	      (set-continuation-cont! previous end))
	  (begin
	    (if (within-stack? (stack-cont-env cont))
		(save-env-in-heap (stack-cont-env cont) cont key reason)
		0) ; for type inferencer (which could use some improvement)
	    (let ((new (copy-continuation-to-heap! cont key)))
	      (add-preserve-cont-stats new reason)
	      (if (continuation? previous)
		  (set-continuation-cont! previous new)
		  (set-stack-cont-cont! *bottom-of-stack* new))
	      (loop (integer->address (continuation-cont new)) new))))))
  (set! *cont* *bottom-of-stack*)  
  (stack-cont-cont *bottom-of-stack*))

(define (copy-continuation-to-heap! cont key)
  (let* ((size (stack-continuation-size cont))
	 (new (make-continuation size key)))
    (copy-memory! (address1+ cont)
		  (address1+ (address-after-header new))
		  (cells->bytes (- size 1)))
    (set-continuation-pc! new (stack-cont-pc cont))
    new))

(define (stack-continuation-size cont)
  (let* ((code-pointer (integer->address (stack-cont-code-pointer cont)))
	 (size (+ (shift-left (fetch-byte (address- code-pointer 2)) 8)
		  (fetch-byte (address- code-pointer 1)))))
    (if (= size 0)
	(extract-fixnum (stack-cont-ref cont continuation-cells))
	size)))

(define make-continuation-header
  (let ((type (enum stob continuation)))
    (lambda (size)
      (make-header-immutable
       (make-header type (cells->bytes size))))))

;----------------
; Tracing the stack for garbage collection - first trace any arguments pushed
; above the current continuation, then loop down the continuations, tracing
; each one along with its environment (if the environment has not yet been
; done).

(define *stack-warning?* #t)

(define (trace-stack trace-locations! trace-stob-contents! trace-value)
  (if *stack-warning?*
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
  (let ((arg-count (arguments-on-stack)))
    (trace-locations! *stack*
		      (address+ *stack* (cells->a-units arg-count))))
  (if (within-stack? *env*)
      (trace-env *env* trace-stob-contents!)
      (set! *env* (trace-value *env*)))
  (let loop ((cont *cont*) (last-env 0))
    (let ((env (stack-cont-env cont)))
      (trace-stack-continuation cont trace-locations! trace-value)
      (if (not (vm-eq? env last-env))
	  (trace-env env trace-stob-contents!))
      (if (not (address= cont *bottom-of-stack*))
	  (loop (integer->address (stack-cont-cont cont))
		env))))
  (set-stack-cont-cont! *bottom-of-stack*
			(trace-value (stack-cont-cont *bottom-of-stack*))))

(define (trace-env env trace-stob-contents!)
  (let loop ((env env))
    (if (within-stack? env)
	(begin
	  (trace-stob-contents! env)
	  (loop (vm-vector-ref env 0))))))

; We cannot count on any headers being in place, so we can't use
; the standard accessors because they have assertions that look at
; the headers.

(define (trace-stack-continuation cont trace-locations! trace-value)
  (let ((code (raw-template-code (stack-cont-template cont))))
    (let ((pc (- (stack-cont-code-pointer cont)
		 (address->integer (address-after-header code))))
	  (new-code (trace-value code)))
      (set-stack-cont-code-pointer! cont
				    (address->integer
				      (address+ (address-after-header new-code)
						pc))))
    (set-stack-cont-template! cont (trace-value (stack-cont-template cont)))
    (set-stack-cont-env!      cont (trace-value (stack-cont-env      cont)))
    (trace-locations! (address+ cont (cells->a-units continuation-cells))
		      (address+ cont (cells->a-units
				      (stack-continuation-size cont))))))

; This should be in arch.scm
(define template-code-index 0)

(define (raw-template-code template)
  (fetch (address+ (address-after-header template)
		   template-code-index)))

;----------------
; Error reporting

(define (report-continuation-uids current-template out)
  (let ((not-first? (maybe-write-template current-template #f out)))
    (let loop ((cont *cont*) (not-first? not-first?))
      (if (address= cont *bottom-of-stack*)
	  (let loop ((cont (stack-cont-cont cont)) (not-first? not-first?))
	    (if (continuation? cont)
		(loop (continuation-cont cont)
		      (maybe-write-template (continuation-template cont)
					    not-first?
					    out))))
	  (loop (integer->address (stack-cont-cont cont))
		(maybe-write-template (stack-cont-template cont)
				      not-first?
				      out))))))

(define (maybe-write-template template not-first? out)
  (if (fixnum? (template-name template))
      (begin
	(if not-first?
	    (write-string " <- " out))
	(write-integer (extract-fixnum (template-name template)) out)
	#t)
      not-first?))

;----------------
; Collecting and printing statistics on stack usage

(define collect-statistics? #f)

(define *conts* 0)
(define *conts-slots* 0)
(define *conts-overflow* 0)
(define *conts-overflow-slots* 0)
(define *conts-preserved* 0)
(define *conts-preserved-slots* 0)
(define *conts-from-heap* 0)
(define *conts-from-heap-slots* 0)

(define *envs* 0)
(define *envs-slots* 0)
(define *envs-closed* 0)
(define *envs-closed-slots* 0)
(define *envs-overflow* 0)
(define *envs-overflow-slots* 0)
(define *envs-preserved* 0)
(define *envs-preserved-slots* 0)

(define (reset-stack-stats)
  (cond (collect-statistics?
	 (set! *conts* 0)
	 (set! *conts-slots* 0)
	 (set! *conts-overflow* 0)
	 (set! *conts-overflow-slots* 0)
	 (set! *conts-preserved* 0)
	 (set! *conts-preserved-slots* 0)
	 (set! *conts-from-heap* 0)
	 (set! *conts-from-heap-slots* 0)

	 (set! *envs* 0)
	 (set! *envs-slots* 0)
	 (set! *envs-closed* 0)
	 (set! *envs-closed-slots* 0)
	 (set! *envs-overflow* 0)
	 (set! *envs-overflow-slots* 0)
	 (set! *envs-preserved* 0)
	 (set! *envs-preserved-slots* 0)
	 )
	(else 0)))

(define (print-stack-stats port)
  (if collect-statistics?
      (really-print-stack-stats port)))

(define (really-print-stack-stats port)
  (let ((one-record (lambda (name count slots port)
		      (newline port)
		      (write-string "(" port)
		      (write-string name port)
		      (write-string " " port)
		      ;(write-number count port)   ; don't have write-number!
		      ;(write-number slots port)
		      (write-string ")" port))))
    (newline port)
    (write-string "(continuations" port)
    (one-record "made" *conts* *conts-slots* port)
    (one-record "overflow" *conts-overflow* *conts-overflow-slots* port)
    (one-record "preserved" *conts-preserved* *conts-preserved-slots* port)
    (one-record "from-heap" *conts-from-heap* *conts-from-heap-slots* port)
    (write-string ")" port)

    (newline port)
    (write-string "(environments" port)
    (one-record "made" *envs* *envs-slots* port)
    (one-record "closed" *envs-closed* *envs-closed-slots* port)
    (one-record "overflow" *envs-overflow* *envs-overflow-slots* port)
    (one-record "preserved" *envs-preserved* *envs-preserved-slots* port)
    (write-string ")" port)
    (newline port)
    ))

(define (add-continuation-stats arg-count)
  (cond (collect-statistics?
	 (set! *conts* (+ *conts* 1))
	 (set! *conts-slots*
	       (+ *conts-slots* (+ arg-count continuation-cells))))))


(define (add-env-stats count)
  (cond (collect-statistics?
	 (set! *envs* (+ *envs* 1))
	 (set! *envs-slots* (+ *envs-slots* (+ count 1))))))

(define (add-copy-env-stats env reason)
    (cond ((not collect-statistics?)
	   (unspecific))
	  ((= reason (enum copy closure))
	   (set! *envs-closed* (+ *envs-closed* 1))
	   (set! *envs-closed-slots*
		 (+ *envs-closed-slots* (vm-vector-length env))))
	  ((= reason (enum copy overflow))
	   (set! *envs-overflow* (+ *envs-overflow* 1))
	   (set! *envs-overflow-slots*
		 (+ *envs-overflow-slots* (vm-vector-length env))))
	  ((= reason (enum copy preserve))
	   (set! *envs-preserved* (+ *envs-preserved* 1))
	   (set! *envs-preserved-slots*
		 (+ *envs-preserved-slots* (vm-vector-length env))))))

(define (add-preserve-cont-stats new reason)
  (cond ((not collect-statistics?)
	 (unspecific))
	((= reason (enum copy overflow))
	 (set! *conts-overflow* (+ *conts-overflow* 1))
	 (set! *conts-overflow-slots*
	       (+ *conts-overflow-slots*
		  (continuation-length new))))
	((= reason (enum copy preserve))
	 (set! *conts-preserved* (+ *conts-preserved* 1))
	 (set! *conts-preserved-slots*
	       (+ *conts-preserved-slots*
		  (continuation-length new))))))

(define (add-copy-cont-from-heap-stats cont)
  (cond (collect-statistics?
	 (set! *conts-from-heap* (+ *conts-from-heap* 1))
	 (set! *conts-from-heap-slots* (+ *conts-from-heap-slots*
					  (continuation-length cont))))))
