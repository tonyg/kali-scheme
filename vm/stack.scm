; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file stack.scm.

; *STACK* points to the next unused cell on top of the stack.
; *STACK-LIMIT* is then end of the currently usable stack space.
; *BOTTOM-OF-STACK* is a continuation that lies a the base of the stack.

(define *stack*           (unassigned))
(define *stack-limit*     (unassigned))
(define *bottom-of-stack* (unassigned))

(define (within-stack? p)
  (and (addr>= p *stack-begin*)
       (addr<= p *stack-end*)))

(define (available-on-stack? cells)
  (addr> (addr- *stack* (cells->a-units cells)) *stack-limit*))

; The + 1 is to get room for one header that is made.
(define (stack-size)
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
  (addr- x addressing-units-per-cell))

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

(define (pointer-to-top-of-stack)
  (addr+ *stack* (cells->a-units 1)))

; Space for an exception continuation is reserved on the stack to
; ensure that pushing an exception continuation will not trigger a
; garbage collection.  Exceptions occur at points where there are
; live values that will not be found by the GC.

(define (reserve-stack-space size)
  (set! *stack-limit* (+ *stack-begin* (cells->a-units size))))

(define (enable-stack-reserve)
  (set! *stack-limit* *stack-begin*))

(define *exception-space-used?* #f)

(define (reserve-exception-space)
  (cond (*exception-space-used?*
	 (ensure-stack-space exception-frame-size)  ; may trigger a GC
	 (reserve-stack-space exception-frame-size)
	 (set! *exception-space-used?* #f))))

(define (allow-exception-consing)
  (cond ((not (available-on-stack? exception-frame-size))
	 (enable-stack-reserve)
	 (set! *exception-space-used?* #t)
	 (if (not (available-on-stack? exception-frame-size))
	     (error "insufficient space on stack for exception frame"))))
  (preallocate-stack-space exception-frame-size))

(define (stack-continuation-size cells)
  (+ (+ cells 1)                 ; header
      maximum-stack-arg-count))   ; pre-checking for pushed arguments

; This ensures that sufficient stack storage is available.  If it is not
; available the stack is copied into the heap.
;
; Stack overflow checks to happen at points when a GC is possible, even if the
; allocations themselves are at unsafe points.  As a safety check, when storage
; is actually allocated the preallocation code checks that the correct overflow
; test was made.  This safety check can be turned off.

(define (ensure-stack-space cells)
  (if (not (available-on-stack? cells))
      (get-stack-space cells))
  (preallocate-stack-space cells))

(define (get-stack-space cells)
  (cond ((not (available? (stack-size)))
	 (interpreter-collect)
	 (if (not (available? (stack-size))) 
	     (error "not enough room in heap for stack" (stack-size)))))
  (copy-stack-into-heap universal-key)
  (if (not (available-on-stack? cells))
      (error "stack overflow" cells)))

(define check-stack-preallocation? #f)

(define *stack-key* 0)
(define *okayed-stack-space* 0)

; Returns a key.  Only the most recent key is valid for allocating storage.
; UNIVERSAL-KEY is always valid.

(define (preallocate-stack-space cells)
  (cond (check-stack-preallocation?
	 (assert (available-on-stack? cells))
	 (set! *stack-key* (+ *stack-key* -1)) ;go down to distinguish from heap keys
	 (set! *okayed-stack-space* cells)
	 *stack-key*)
	(else 0)))

; Checks that KEY is the most recent key, and that the overflow check was
; made for at least CELLS space.

(define (check-stack-cons cells key)
  (cond ((or (not check-stack-preallocation?)
	     (= key universal-key))
	 0)
	((not (and (= key *stack-key*)
		   (>= *okayed-stack-space* cells)))
	 (error "invalid stack key" key cells))
	(else
	 (set! *okayed-stack-space* (- *okayed-stack-space* cells)))))

; At the bottom of the stack is a special continuation that is never removed.
; When it is invoked it copies the next continuation out of the heap (if there
; is any such) and invokes that instead.

(define (initialize-stack)
  (set! *stack* (the-pointer-before *stack-end*))
  (reserve-stack-space exception-frame-size)
  (let ((bottom (push-continuation-on-stack 0 universal-key)))
    (set-continuation-pc!       bottom (enter-fixnum 0))
    (set-continuation-cont!     bottom (enter-boolean #f))
    (set-continuation-template! bottom
     (make-special-op-template op/get-cont-from-heap))
    (set-continuation-env!      bottom quiescent)
    (set! *bottom-of-stack* bottom)))

; The amount of heap space required to initialize the stack.
(define initial-stack-heap-space special-op-template-size)

(define (reset-stack-pointer)
  (set! *stack* (the-pointer-before
		 (the-pointer-before (address-after-header *bottom-of-stack*))))
  (reserve-stack-space exception-frame-size)
  (set-continuation-cont! *bottom-of-stack* (enter-boolean #f))
  *bottom-of-stack*)  ; initial value of *cont*

; Called when replacing the current continuation with a new one.

(define (restore-continuation cont)
  (if (false? cont)
      (reset-stack-pointer)
      (copy-continuation-from-heap cont)))

; Called when returning off of the end of the stack.

(define (get-continuation-from-heap)
  (let ((cont (continuation-cont *bottom-of-stack*)))
    (if (false? cont)
	cont
	(copy-continuation-from-heap cont))))

; Copy CONT from heap onto stack just above *BOTTOM-OF-STACK*, linking it
; to *BOTTOM-OF-STACK* and *BOTTOM-OF-STACK* to CONT's continuation.

(define (copy-continuation-from-heap cont)
  (assert (continuation? cont))
  (let* ((top (addr- (address-at-header *bottom-of-stack*)
                     (addr1+ (cells->a-units (continuation-length cont)))))
         (new-cont (make-stob-descriptor (addr1+ top))))
    (add-copy-cont-from-heap-stats cont)
    (set! *stack* (the-pointer-before top))
    (copy-cells! (address-at-header cont) top (+ 1 (continuation-length cont)))
    (set-continuation-cont! *bottom-of-stack* (continuation-cont new-cont))
    (set-continuation-cont! new-cont *bottom-of-stack*)
    new-cont))


; Pushing and popping continuations.  KEY is used to check that an overflow
; check was made.  This returns a pointer to the stack.

(define (push-continuation-on-stack arg-count key)
  (add-continuation-stats arg-count)
  (check-stack-cons (+ 1 continuation-cells) key)
  (stack-add (+ 1 continuation-cells))
  (store! (addr1+ *stack*) (make-continuation-header arg-count))
  (make-stob-descriptor (addr1+ (addr1+ *stack*))))

(define (make-continuation-header arg-count)
  (make-header stob/continuation
	       (cells->bytes (+ arg-count continuation-cells))))

(define (pop-continuation-from-stack cont)
  (set! *stack* (addr+ (address-at-header cont)
                       (cells->a-units continuation-cells))))

; Making environments on the stack - the values are already there so this
; only needs to push space for a pointer and push a header.

(define (ensure-env-space count)
  (ensure-stack-space 2))  ; header + superior environment

(define (pop-args-into-env count key)
  (check-stack-cons 2 key)
  (push 0)                            ; space for superior env
  (push (make-header stob/vector (cells->bytes (+ count 1))))
  (add-env-stats count)
  (make-stob-descriptor (addr1+ (addr1+ *stack*))))

(define (ensure-heap-env-space count)
  (ensure-space (+ count 2)))  ; header + superior environment

; Alternative method for making environments - put the values into the heap.

(define (pop-args-into-heap-env count key)
  (let ((stob (make-stob stob/vector (cells->bytes (+ count 1)) key)))
    (copy-cells! (addr1+ *stack*)
		 (addr+ (cells->a-units 1)
			(address-after-header stob))
		 count)
    (stack-add (- 0 count))
    stob))

; Migrate the current environment to the heap.  Used when creating a closure.

(define (preserve-*env*)
  (cond ((within-stack? *env*)
         (let ((key (ensure-space (stack-env-size *env*))))
           (set! *env* (save-env-in-heap *env* *cont* key copy/closure))))))

; Returns the size of ENV plus that of any of its ancestors that reside in
; the stack.

(define (stack-env-size env)
  (let loop ((env env) (size 0))
    (let* ((size (+ size (vm-vector-size (vm-vector-length env))))
           (env (vm-vector-ref env 0)))
      (if (within-stack? env)
          (loop env size)
          size))))

; 1) Copy ENV and its ancestors into heap, adding forwarding pointers
; 2) Go down the continuation chain updating the env pointers
; 3) Remove the forwarding pointers as they may confuse things (this shouldn't
;    be necessary, but everything works so I do not want to mess with it now).
;
; This code depends on continuation-cont pointers not crossing environment
; parent pointers on the stack.

(define (save-env-in-heap env cont key reason)
  (let ((top (copy-env env key reason)))
    (let loop ((env top))
      (cond ((within-stack? (vm-vector-ref env 0))
             (let ((new (copy-env (vm-vector-ref env 0) key reason)))
               (vm-vector-set! env 0 new)
               (loop new)))))
    (let loop ((cont cont))
      (let ((env (continuation-env cont)))
        (cond ((and (stob? env)
                    (stob? (stob-header env)))
               (set-continuation-env! cont (stob-header env))
               (loop (continuation-cont cont))))))
    (let loop ((env env))
      (let ((h (stob-header env)))
        (cond ((stob? h)
               (stob-header-set! env (stob-header h))
               (if (stob? (vm-vector-ref h 0))
                   (loop (vm-vector-ref h 0)))))))
    top))

(define (copy-env env key reason)
  (let ((new (copy-stob env key)))
    (add-copy-env-stats env reason)
    (stob-header-set! env new)
    new))

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
    (set! *cont* (restore-continuation
		  (preserve-continuation *cont* key copy/overflow)))
    (do ((i 0 (+ i 1)))
        ((>= i arg-count))
      (push (vm-vector-ref vec i)))))

(define (arguments-on-stack)
  (do ((p (addr1+ *stack*) (addr1+ p))
       (i 0 (+ i 1)))
      ((header? (fetch p)) i)))

; Migrating the current continuation into the heap.
; *ENV* is moved first if necessary.

(define (preserve-continuation cont key reason)
  (if (within-stack? *env*)
      (set! *env* (save-env-in-heap *env* cont key reason)))
  (let ((end (continuation-cont *bottom-of-stack*)))
    (let loop ((cont cont) (previous *bottom-of-stack*))
      (cond ((vm-eq? cont *bottom-of-stack*)
	     (set-continuation-cont! previous end))
            (else
             (if (within-stack? (continuation-env cont))
                 (save-env-in-heap (continuation-env cont) cont key reason))
             (let ((new (copy-stob cont key)))
	       (add-preserve-cont-stats new reason)
               (set-continuation-cont! previous new)
               (loop (continuation-cont new) new)))))
    (continuation-cont *bottom-of-stack*)))


; Copy NARGS arguments from the top of the stack to just above CONT.
; Used by OP/MOVE-ARGS-AND-CALL to implement tail-recursive calls.
; 
; The IF saves work in a rare case at the cost of a test in the common
; case; is it worth it?

(define (move-args-above-cont cont nargs)
  (let ((start-loc (the-pointer-before (address-at-header cont)))
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

(define (trace-stack cont)
  (trace-locations (addr1+ *stack*) (address-at-header cont))
  (let loop ((cont cont) (last-env 0))
    (let ((env (continuation-env cont)))
      (trace-locations (address-after-header cont) (address-after-stob cont))
      (if (not (vm-eq? env last-env))
	  (trace-env env))
      (if (not (vm-eq? cont *bottom-of-stack*))
	  (loop (continuation-cont cont) env)))))

; I do not think that the recursive call is necessary as the superior
; env will be traced by TRACE-STACK as it goes down the list of
; continuations.  For every superior env that is on the stack, there should
; be a continuation on the stack that points to it.

(define (trace-env env)
  (cond ((within-stack? env)
         (trace-locations (address-after-header env) (address-after-stob env))
         (trace-env (vm-vector-ref env 0)))))


; Collecting and printing statistics on stack usage

(define collect-statistics? #f)

(define copy/closure 0) ; leave these as there are still references lying around
(define copy/overflow 1)
(define copy/preserve 2)

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
	   0)
	  ((= reason copy/closure)
	   (set! *envs-closed* (+ *envs-closed* 1))
	   (set! *envs-closed-slots*
		 (+ *envs-closed-slots* (vm-vector-length env))))
	  ((= reason copy/overflow)
	   (set! *envs-overflow* (+ *envs-overflow* 1))
	   (set! *envs-overflow-slots*
		 (+ *envs-overflow-slots* (vm-vector-length env))))
	  ((= reason copy/preserve)
	   (set! *envs-preserved* (+ *envs-preserved* 1))
	   (set! *envs-preserved-slots*
		 (+ *envs-preserved-slots* (vm-vector-length env))))))

(define (add-preserve-cont-stats new reason)
  (cond ((not collect-statistics?)
	 0)
	((= reason copy/overflow)
	 (set! *conts-overflow* (+ *conts-overflow* 1))
	 (set! *conts-overflow-slots*
	       (+ *conts-overflow-slots*
		  (continuation-length new))))
	((= reason copy/preserve)
	 (set! *conts-preserved* (+ *conts-preserved* 1))
	 (set! *conts-preserved-slots*
	       (+ *conts-preserved-slots*
		  (continuation-length new))))))

(define (add-copy-cont-from-heap-stats cont)
  (cond (collect-statistics?
	 (set! *conts-from-heap* (+ *conts-from-heap* 1))
	 (set! *conts-from-heap-slots* (+ *conts-from-heap-slots*
					  (continuation-length cont))))))
