; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; The stack grows from higher addresses to lower ones.
; *STACK-BEGIN* and *STACK-END* delimit the stack portion of memory.
; *STACK* points to the next unused cell on top of the stack.
; *STACK-LIMIT* is value against which stack requests are checked.  There is a
;   buffer area between the limit and the actual end of the stack.  The buffer
;   is large enough to contain the default procedure stack requirement plus the
;   space needed to make a call to an exception plus an interrupts plus some
;   slack for safety's sake.

; These slots are filled with a funny value so that we can detect overruns.

(define stack-warning-limit 30)

; The supposedly unused space at the end of the stack is marked with this
; value so that we can detect incursions.

(define stack-marker #xf0f0f0f)

; We need at least this amount of space for the stack.

(define stack-slack
  (+ default-stack-space
     continuation-stack-size	    ; *bottom-of-stack*
     (+ continuation-stack-size 16) ; exceptions need at most 16 values
                                    ; (long pole is external exceptions with
                                    ; exception + procedure-name + 12 irritants
                                    ; + saved-exception + stack-block)
     (+ continuation-stack-size 7)  ; interrupts need at most 7 values
                                    ; (also from examining the code)
     (+ stack-warning-limit 2)))    ; safety

; *BOTTOM-OF-STACK* is a continuation that lies a the base of the stack.

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
      (set! *stack* (the-pointer-before *stack-end*))
      (set! *cont* false)
      (set! *env* quiescent)
      (do ((a start (address1+ a)))
	  ((address= a *stack-end*))
	(store! a stack-marker))
      (let ((key (ensure-space (op-template-size 2))))
	(push-continuation-on-stack
	  (make-template-containing-ops (enum op protocol)
					bottom-of-stack-protocol
					key)
	  (enter-fixnum 0)
	  0))
      (set! *bottom-of-stack* *cont*))))

(define (reset-stack-pointer base-continuation)
  (set! *stack* (the-pointer-before (address-at-header *bottom-of-stack*)))
  (set-continuation-cont! *bottom-of-stack* base-continuation)
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
  (store! *stack* x)
  (add-cells-to-stack! 1))

(define (pop)
  (add-cells-to-stack! -1)
  (fetch *stack*))

; Use the stack as a vector, with (STACK-REF 0) => current top of stack

(define (stack-ref index)
  (fetch (address+ *stack* (cells->a-units (+ 1 index)))))

(define (stack-set! index value)
  (store! (address+ *stack* (cells->a-units (+ 1 index))) value))

(define (pointer-to-stack-arguments)
  (address+ *stack* (cells->a-units 1)))

(define (remove-stack-arguments count)
  (add-cells-to-stack! (- 0 count)))

;----------------
; *STACK-LIMIT* is offset by DEFAULT-STACK-SPACE to make this test faster.

(define (ensure-default-procedure-space! ensure-space)
  (if (address< *stack* *stack-limit*)
      (begin
	(copy-stack-into-heap (ensure-space (current-stack-size)))
	(if (address< *stack* *stack-limit*)
	    (error "Couldn't get default procedure space (how can this happen?)"))))
  0)

; Compiler is not allowed to use more than one stack's worth of space for
; any given procedure.

(define (ensure-stack-space! space ensure-space)
  (if (not (available-on-stack? space))
      (begin
	(copy-stack-into-heap (ensure-space (current-stack-size)))
	(if (not (available-on-stack? space))
	    (error "VM's stack is too small (how can this happen?)")))))

(define (available-on-stack? space)
  (> (+ (a-units->cells			; space on the stack
	   (address-difference *stack* *stack-limit*))
	default-stack-space)            ; allow for *stack-limit*'s offset
     space))

;----------------
; Getting and setting the current continuation.

(define (current-continuation)
  *cont*)

; Called when replacing the current continuation with a new one.

(define (set-current-continuation! cont)
  (if (continuation? cont)
      (copy-continuation-from-heap! cont 0)
      (reset-stack-pointer cont)))

; For multiple-value returns (see call.scm).

(define (skip-current-continuation!)
  (let ((next (continuation-cont *cont*)))
    (if (= *cont* *bottom-of-stack*)
	(set-continuation-cont! *cont* (continuation-cont next))
	(set! *cont* next))))

; Called when returning off of the end of the stack.

(define (get-continuation-from-heap)
  (continuation-cont *bottom-of-stack*))

; Copy CONT from heap onto stack just above *BOTTOM-OF-STACK*, linking it
; to *BOTTOM-OF-STACK* and *BOTTOM-OF-STACK* to CONT's continuation.
; There are STACK-ARG-COUNT arguments on top of the stack that need to be
; preserved.

(define (copy-continuation-from-heap! cont stack-arg-count)
  (assert (continuation? cont))
  (let* ((top (address- (address-at-header *bottom-of-stack*)
			(cells->a-units (+ 1 (continuation-length cont)))))
         (new-cont (address->stob-descriptor (address1+ top))))
    (add-copy-cont-from-heap-stats cont)
    (if (= stack-arg-count 0)
	(set! *stack* (the-pointer-before top))
	(let ((new-stack (address- (the-pointer-before top)
				   (cells->a-units stack-arg-count))))
	  (if (address< new-stack *stack*)
	      (begin
		(copy-memory! (address1+ *stack*)
			      (address1+ new-stack)
			      (cells->bytes stack-arg-count))
		(set! *stack* new-stack)))))
    (set! *cont* new-cont)
    (copy-memory! (address-at-header cont)
		  top
		  (cells->bytes (+ 1 (continuation-length cont))))
    (set-continuation-cont! *bottom-of-stack* (continuation-cont new-cont))
    (set-continuation-cont! new-cont *bottom-of-stack*)
    new-cont))

(define (push-continuation-on-stack template pc arg-count)
  (add-continuation-stats arg-count)
  (add-cells-to-stack! (+ 1 continuation-cells))
  (store! (address1+ *stack*) (make-continuation-header arg-count))
  (let ((cont (address->stob-descriptor (address2+ *stack*))))
    (set-continuation-pc!       cont pc)
    (set-continuation-template! cont template)
    (set-continuation-env!      cont *env*)
    (set-continuation-cont!     cont *cont*)
    (set! *cont* cont)))

(define make-continuation-header
  (let ((type (enum stob continuation)))
    (lambda (arg-count)
      (make-header-immutable
       (make-header type (cells->bytes (+ arg-count continuation-cells)))))))

(define (pop-continuation-from-stack set-template!)
  (let ((cont *cont*))
    (set-template! (continuation-template cont)
		   (continuation-pc       cont))
    (set! *env*    (continuation-env      cont))
    (set! *cont*   (continuation-cont     cont))
    (set! *stack* (address+ (address-at-header cont)
			    (cells->a-units continuation-cells)))))

;----------------
; Copying the stack into the heap because there is no more room on the
; stack.  This preserves the continuation and then moves any arguments
; down on top of the current continuation.

(define (copy-stack-into-heap key)
  (let ((arg-count (arguments-on-stack))
	(top *stack*))
    (preserve-continuation key (enum copy overflow))
    (set! *stack* (the-pointer-before (address-at-header *cont*)))
    (move-stack-arguments! top arg-count)))

(define (arguments-on-stack)
  (do ((p (address1+ *stack*) (address1+ p))
       (i 0 (+ i 1)))
      ((header? (fetch p))
       (if (= (fetch p) argument-limit-marker)
	   (- i 1)  ; marker is one past the original header location
	   i))))

; This is used by the environment code to mark environments that have been
; migrated to the heap (their header gets clobbered during the of migration).
; Any header type that is not normally found on the heap will work.

(define argument-limit-marker (make-header (enum stob channel) 0))

; Move NARGS values from TOP-OF-ARGS to the current top of the stack.

(define (move-stack-arguments! top-of-args nargs)
  (let ((start-arg (address+ top-of-args (cells->a-units nargs))))
    (do ((loc *stack* (the-pointer-before loc))
	 (arg start-arg (the-pointer-before arg)))
	((address<= arg top-of-args)
	 (set! *stack* loc))
      (store! loc (fetch arg)))))

(define (really-move-args-above-cont! nargs top-of-args)
  (let ((start-loc (the-pointer-before (address-at-header *cont*)))
	(start-arg (address+ top-of-args (cells->a-units nargs))))
    (do ((loc start-loc (the-pointer-before loc))
	 (arg start-arg (the-pointer-before arg)))
	((address<= arg top-of-args)
	 (set! *stack* loc))
      (store! loc (fetch arg)))))

; Copy NARGS arguments from the top of the stack to just above CONT.

(define (move-args-above-cont! nargs)
  (let ((top-of-args *stack*)
	(top-of-cont (the-pointer-before (address-at-header *cont*))))
    (if (not (address= top-of-cont
		       (address+ top-of-args
				 (cells->a-units nargs))))
	(begin
	  (set! *stack* top-of-cont)
	  (move-stack-arguments! top-of-args nargs)))))

; Return the top-of-args value needed above.

(define (top-of-argument-stack)
  *stack*)

; Migrating the current continuation into the heap, saving the environment
; first.  The heap space needed is no more than the current stack size.

(define current-continuation-size current-stack-size)

(define (copy-current-continuation-to-heap key)
  (preserve-continuation key (enum copy preserve)))

(define (preserve-continuation key reason)
  (if (false? *cont*)
      false
      (really-preserve-continuation key reason)))

; 1. Preserve the current lexical environment
; 2. Loop down the continuations copying them into the heap, including their
;    lexical environments
; 3. Copy the current continuation back onto the stack

(define (really-preserve-continuation key reason)
  (preserve-current-env-with-reason key reason)
  (let ((end (continuation-cont *bottom-of-stack*)))
    (let loop ((cont *cont*) (previous *bottom-of-stack*))
      (cond ((vm-eq? cont *bottom-of-stack*)
	     (set-continuation-cont! previous end))
            (else
             (if (within-stack? (continuation-env cont))
                 (save-env-in-heap (continuation-env cont) cont key reason)
		 0) ; for type inferencer (which could use some improvement)
             (let ((new (header+contents->stob
			 (stob-header cont)
			 (address-after-header cont)
			 key)))
	       (add-preserve-cont-stats new reason)
               (set-continuation-cont! previous new)
               (loop (continuation-cont new) new))))))
  (set! *cont* *bottom-of-stack*)  
  (continuation-cont *bottom-of-stack*))

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
    (trace-locations! (address1+ *stack*)
		      (address+ *stack* (cells->a-units (+ arg-count 1)))))
  (if (within-stack? *env*)
      (trace-env *env* trace-stob-contents!)
      (set! *env* (trace-value *env*)))
  (let loop ((cont *cont*) (last-env 0))
    (let ((env (continuation-env cont)))
      (trace-stob-contents! cont)
      (if (not (vm-eq? env last-env))
	  (trace-env env trace-stob-contents!))
      (if (not (vm-eq? cont *bottom-of-stack*))
	  (loop (continuation-cont cont) env)))))

(define (trace-env env trace-stob-contents!)
  (let loop ((env env))
    (if (within-stack? env)
	(begin
	  (trace-stob-contents! env)
	  (loop (vm-vector-ref env 0))))))

;----------------
; Error reporting

(define (report-continuation-uids current-template out)
  (let ((not-first? (maybe-write-template current-template #f out)))
    (let loop ((cont *cont*) (not-first? not-first?))
      (if (continuation? cont)
	  (loop (continuation-cont cont)
		(maybe-write-template (continuation-template cont)
				      not-first?
				      out))))))

(define (maybe-write-template template not-first? out)
  (if (and (fixnum? (template-name template))
	   (not (vm-eq? template (continuation-template *bottom-of-stack*))))
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
