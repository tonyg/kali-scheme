; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file stack.scm.

; *STACK-BEGIN* and *STACK-END* delimit the stack portion of memory.
; *STACK* points to the next unused cell on top of the stack.
; *STACK-LIMIT* is then end of the currently usable stack space.
; *BOTTOM-OF-STACK* is a continuation that lies a the base of the stack.

(define *stack-begin*     (unassigned))
(define *stack-end*       (unassigned))
(define *stack*           (unassigned))
(define *stack-limit*     (unassigned))
(define *bottom-of-stack* (unassigned))

(define *cont*            (unassigned))

; For tracking the reason for copying closures and environments (used for
; debugging only).

(define-enumeration copy
  (closure
   overflow
   preserve))

; At the bottom of the stack is a special continuation that is never removed.
; When it is invoked it copies the next continuation out of the heap (if there
; is any such) and invokes that instead.

(define (initialize-stack start size)
  (set! *stack-begin* start)
  (set! *stack-end* (+ start (cells->a-units size)))
  (set! *stack-limit* *stack-begin*)
  (set! *stack* (the-pointer-before *stack-end*))
  (set! *cont* false)
  (set! *env* quiescent)
  (push-continuation-on-stack (make-template-containing-ops
			       (enum op get-cont-from-heap)
			       (enum op return))
			      (enter-fixnum 0)
			      0
			      universal-key)
  (set! *bottom-of-stack* *cont*))

; The amount of heap space required to initialize the stack.
(define initial-stack-heap-space (op-template-size 2))

(define (reset-stack-pointer)
  (set! *stack* (the-pointer-before
		 (the-pointer-before (address-after-header *bottom-of-stack*))))
  (set-continuation-cont! *bottom-of-stack* (enter-boolean #f)))

(define (within-stack? p)
  (and (addr>= p *stack-begin*)
       (addr<= p *stack-end*)))

(define (stack-size)
  (- *stack-end* *stack-begin*))

(define (available-on-stack? cells)
  (addr> (addr- *stack* (cells->a-units cells)) *stack-limit*))

; The + 1 is to get room for one header that is made.
(define (current-stack-size)
  (+ 1 (a-units->cells (addr- *stack-end* *stack*))))

; Value of *NARGS* indicating that the arguments overflowed the stack limit
(define arg-stack-overflow-nargs (+ maximum-stack-args 1))

 ; Room for MAXIMUM-STACK-ARGS plus one for the procedure argument to op/apply
(define maximum-stack-arg-count (+ maximum-stack-args 1))

; Add CELLS cells onto the stack.
; The stack grows towards negative memory.

(define (stack-add cells)
  (set! *stack* (addr- *stack* (cells->a-units cells))))

(define (the-pointer-before x)
  (addr- x (cells->a-units 1)))

(define (push x)     ; check for overflow is done when continuations are pushed
  (store! *stack* x)
  (stack-add 1))

(define (pop)
  (stack-add -1)
  (fetch *stack*))

(define (stack-ref index)
  (fetch (addr+ *stack* (cells->a-units (+ 1 index)))))

(define (stack-set! index value)
  (store! (addr+ *stack* (cells->a-units (+ 1 index))) value))

(define (pointer-to-stack-arguments)
  (addr+ *stack* (cells->a-units 1)))

(define (remove-stack-arguments count)
  (stack-add (- 0 count)))


; Making sure that no one uses stack space without checking for overflow.

; Returns a key.  Only the most recent key is valid for allocating storage.
; UNIVERSAL-KEY is always valid.

(define (ensure-stack-space space ensure-space)
  (if (not (available-on-stack? space))
      (copy-stack-into-heap (ensure-space (current-stack-size))))
  (preallocate-stack-space space))

(define (preallocate-stack-space space)
  (cond (check-stack-preallocation?
	 (set! *stack-key* (+ *stack-key* -1)) ;go down to distinguish from heap keys
	 (set! *okayed-stack-space* space)
	 *stack-key*)
	(else 0)))

(define check-stack-preallocation? #f)
(define *stack-key* 0)
(define *okayed-stack-space* 0)

; Checks that KEY is the most recent key, and that the overflow check was
; made for at least CELLS space.

(define (check-stack-cons cells key)
  (cond ((and check-stack-preallocation?
	      (not (= key universal-key)))
	 (if (not (and (= key *stack-key*)
		       (>= *okayed-stack-space* cells)))
	     (error "invalid stack key" key cells))
	 (set! *okayed-stack-space* (- *okayed-stack-space* cells)))))

; Space for an exception continuation is reserved on the stack to
; ensure that pushing an exception continuation will not trigger a
; garbage collection.  Exceptions occur at points where there are
; live values that will not be found by the GC.

(define (reserve-stack-space size)
  (set! *stack-limit* (+ *stack-begin* (cells->a-units size))))

(define (enable-stack-reserve)
  (set! *stack-limit* *stack-begin*))

(define *exception-space-used?* #t)

(define (exception-frame-space exception-frame-size)
  (if (and *exception-space-used?*
	   (not (available-on-stack? exception-frame-size)))
      (current-stack-size)
      0))

(define (reserve-exception-space exception-frame-size key)
  (cond (*exception-space-used?*
	 (if (not (available-on-stack? exception-frame-size))
	     (error "no space on stack to reserve exception space"))
	 (reserve-stack-space exception-frame-size)
	 (set! *exception-space-used?* #f))))

(define (allow-exception-consing exception-frame-size)
  (cond ((not (available-on-stack? exception-frame-size))
	 (enable-stack-reserve)
	 (set! *exception-space-used?* #t)
	 (if (not (available-on-stack? exception-frame-size))
	     (error "insufficient space on stack for exception frame"))))
  (preallocate-stack-space exception-frame-size))



(define (peek-at-current-continuation)
  (if (addr= *cont* *bottom-of-stack*)
      (continuation-cont *bottom-of-stack*)
      *cont*))

; Skip to the continuation preceding the current one (used for multiple
; value returns).

(define (skip-current-continuation!)
  (let ((next (continuation-cont *cont*)))
    (if (addr= *cont* *bottom-of-stack*)
	(set-continuation-cont! *cont* (continuation-cont next))
	(set! *cont* next))))

; Called when replacing the current continuation with a new one.

(define (set-current-continuation! cont)
  (set! *cont* (cond ((false? cont)
		      (reset-stack-pointer)
		      *bottom-of-stack*)
		     (else
		      (copy-continuation-from-heap cont)))))

; Called when returning off of the end of the stack.

(define (get-continuation-from-heap)
  (continuation-cont *bottom-of-stack*))

; Copy CONT from heap onto stack just above *BOTTOM-OF-STACK*, linking it
; to *BOTTOM-OF-STACK* and *BOTTOM-OF-STACK* to CONT's continuation.

(define (copy-continuation-from-heap cont)
  (assert (continuation? cont))
  (let* ((top (addr- (address-at-header *bottom-of-stack*)
                     (addr1+ (cells->a-units (continuation-length cont)))))
         (new-cont (address->stob-descriptor (addr1+ top))))
    (add-copy-cont-from-heap-stats cont)
    (set! *stack* (the-pointer-before top))
    (copy-cells! (address-at-header cont) top (+ 1 (continuation-length cont)))
    (set-continuation-cont! *bottom-of-stack* (continuation-cont new-cont))
    (set-continuation-cont! new-cont *bottom-of-stack*)
    new-cont))


; Pushing and popping continuations.

(define stack-continuation-size
  (+ (+ continuation-cells 1)     ; header
     maximum-stack-arg-count))    ; pre-checking for pushed arguments

(define (push-continuation-on-stack template pc arg-count key)
  (check-stack-cons stack-continuation-size key)
  (add-continuation-stats arg-count)
  (stack-add (+ 1 continuation-cells))
  (store! (addr1+ *stack*) (make-continuation-header arg-count))
  (let ((cont (address->stob-descriptor (addr1+ (addr1+ *stack*)))))
    (set-continuation-pc!       cont pc)
    (set-continuation-template! cont template)
    (set-continuation-env!      cont *env*)
    (set-continuation-cont!     cont *cont*)
    (set! *cont* cont)))

(define make-continuation-header
  (let ((type (enum stob continuation)))
    (lambda (arg-count)
      (make-header type (cells->bytes (+ arg-count continuation-cells))))))

(define (pop-continuation-from-stack set-template!)
  (let ((cont *cont*))
    (set-template! (continuation-template cont)
		   (continuation-pc       cont))
    (set! *env*    (continuation-env      cont))
    (set! *cont*   (continuation-cont     cont))
    (set! *stack* (addr+ (address-at-header cont)
			 (cells->a-units continuation-cells)))))

; Making environments on the stack - the values are already there so this
; only needs to push space for a pointer and push a header.


; Copying the stack into the heap because there is no more room on the
; stack.  This copies any arguments that are on the top of the stack into
; a vector, migrates and recovers the current, and then moves the arguments
; from the vector back to the stack.
;
; Why can't this move the arguments directly?  The restored continuation
; cannot be larger than the original.

(define (copy-stack-into-heap key)
  (let* ((arg-count (arguments-on-stack))
         (vec (vm-make-vector arg-count key)))
    (do ((i (+ -1 arg-count) (- i 1)))
        ((<= i -1))
      (vm-vector-set! vec i (pop)))
    (preserve-continuation key (enum copy overflow))
    (do ((i 0 (+ i 1)))
        ((>= i arg-count))
      (push (vm-vector-ref vec i)))
    (unassigned)))

(define (arguments-on-stack)
  (do ((p (addr1+ *stack*) (addr1+ p))
       (i 0 (+ i 1)))
      ((header? (fetch p)) i)))

; Migrating the current continuation into the heap, saving the environment
; first.

(define current-continuation-size current-stack-size)

(define (current-continuation key)
  (preserve-continuation key (enum copy preserve)))

(define (preserve-continuation key reason)
  (preserve-current-env-with-reason key reason)
  (let ((end (continuation-cont *bottom-of-stack*)))
    (let loop ((cont *cont*) (previous *bottom-of-stack*))
      (cond ((vm-eq? cont *bottom-of-stack*)
	     (set-continuation-cont! previous end))
            (else
             (if (within-stack? (continuation-env cont))
                 (save-env-in-heap (continuation-env cont) cont key reason)
		 0) ; for type inferencer (which could use some improvement)
             (let ((new (copy-stob cont key)))
	       (add-preserve-cont-stats new reason)
               (set-continuation-cont! previous new)
               (loop (continuation-cont new) new)))))
    (let ((cont (continuation-cont *bottom-of-stack*)))
      (set! *cont* (if (false? cont)
		       *bottom-of-stack*
		       (copy-continuation-from-heap cont)))
      cont)))


; Copy NARGS arguments from the top of the stack to just above CONT.
; Used by OP/MOVE-ARGS-AND-CALL to implement tail-recursive calls.
; 
; The IF saves work in a rare case at the cost of a test in the common
; case; is it worth it?

(define (move-args-above-cont! nargs)
  (let ((start-loc (the-pointer-before (address-at-header *cont*)))
	(start-arg (addr+ *stack* (cells->a-units nargs))))
    (if (not (addr<= start-loc start-arg))
	(do ((loc start-loc (the-pointer-before loc))
	     (arg start-arg (the-pointer-before arg)))
	    ((addr<= arg *stack*)
	     (set! *stack* loc))
	  (store! loc (fetch arg))))))

; Tracing the stack for garbage collection - first trace any arguments pushed
; above the current continuation, then loop down the continuations, tracing
; each one along with its environment (if the environment has not yet been
; done).

(define (trace-stack trace-locations)
  (trace-locations (addr1+ *stack*) (address-at-header *cont*))
  (let loop ((cont *cont*) (last-env 0))
    (let ((env (continuation-env cont)))
      (trace-locations (address-after-header cont) (address-after-stob cont))
      (if (not (vm-eq? env last-env))
	  (trace-env env trace-locations))
      (if (not (vm-eq? cont *bottom-of-stack*))
	  (loop (continuation-cont cont) env)))))

; I do not think that the recursive call is necessary as the superior
; env will be traced by TRACE-STACK as it goes down the list of
; continuations.  For every superior env that is on the stack, there should
; be a continuation on the stack that points to it.

(define (trace-env env trace-locations)
  (let loop ((env env))
    (cond ((within-stack? env)
	   (trace-locations (address-after-header env) (address-after-stob env))
	   (loop (vm-vector-ref env 0))))))


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
		      (write-number count port)
		      (write-number slots port)
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
	   (unassigned))
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
	 (unassigned))
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
