; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Interrupts

; Create and install a vector of interrupt handlers.  We want this to happen
; as early as possible.  All but the post-gc and keyboard interrupts raise an
; exception by default.  We exit when a keyboard interrupt occurs. The default
; post-gc handler is defined below.

(define (initialize-interrupts! spawn-on-root thunk)
  (primitive-cwcc
    (lambda (exit)
      (let ((handlers (make-vector interrupt-count 0)))
	(do ((i 0 (+ i 1)))
	    ((= i interrupt-count))
	  (vector-set! handlers
		       i
		       (lambda stuff
			 (apply signal (cons 'interrupt (cons i stuff))))))
	(vector-set! handlers
		     (enum interrupt post-gc)
		     (post-gc-handler spawn-on-root))
	(vector-set! handlers
		     (enum interrupt keyboard)
		     (lambda args
		       (with-continuation exit (lambda () -1))))
	(set-interrupt-handlers! handlers)
	(session-data-set! interrupt-handlers handlers))
      (set-enabled-interrupts! all-interrupts)
      (thunk))))
  
(define interrupt-handlers (make-session-data-slot! 0))

; Set an interrupt handler.

(define (set-interrupt-handler! interrupt handler)
  (vector-set! (session-data-ref interrupt-handlers)
	       interrupt
	       handler))
      
(define no-interrupts 0)

(define all-interrupts
  (- (arithmetic-shift 1 interrupt-count) 1))

(define (with-interrupts-inhibited thunk)
  (with-interrupts no-interrupts thunk))

(define (with-interrupts-allowed thunk)
  (with-interrupts all-interrupts thunk))

(define (disable-interrupts!)
  (set-enabled-interrupts! no-interrupts))

(define (enable-interrupts!) 
  (set-enabled-interrupts! all-interrupts))

(define (with-interrupts interrupts thunk)
  ;; I might consider using dynamic-wind here, but (a) I'm worried
  ;; about the speed of thread switching (which uses this) and (b)
  ;; it's a pretty bad idea to throw in or out of one of these anyhow.
  (let ((ei (set-enabled-interrupts! interrupts)))
    (call-with-values thunk
      (lambda results
	(set-enabled-interrupts! ei)
	(apply values results)))))

(define (enabled-interrupts)		;For debugging
  (let ((e (set-enabled-interrupts! 0)))
    (set-enabled-interrupts! e)
    e))

;----------------
; Post-GC interrupts

(define *post-gc-procedures* '())

(define (call-after-gc! thunk)
  (if (not (memq thunk *post-gc-procedures*))
      (set! *post-gc-procedures* (cons thunk *post-gc-procedures*))))

(define (post-gc-handler spawn-on-root)
  (lambda (finalizer-list enabled-interrupts)
    (let ((space (memory-status (enum memory-status-option available) 0)))
      (if (> (session-data-ref required-post-gc-space)
	     space)
	  (spawn-on-root
	   (lambda ()
	     ((session-data-ref space-shortage-handler)
	      (session-data-ref required-post-gc-space)
	      space)))))
    (spawn-on-root
     (lambda ()
       (for-each (lambda (p)
		   ((cdr p) (car p)))
		 finalizer-list)
       (for-each (lambda (thunk)
		   (thunk))
		 *post-gc-procedures*))
     'post-gc-handler)
    (set-enabled-interrupts! enabled-interrupts)))

; Notifying someone if an insufficient amount of memory is reclaimed by
; a garbage collection.  The amount defaults to 10% of the heap.

(define required-post-gc-space (make-session-data-slot! 0))

(define space-shortage-handler
  (make-session-data-slot! (lambda (required space) #f)))

(define (call-before-heap-overflow! handler . maybe-required-space)
  (session-data-set! required-post-gc-space
		     (if (null? maybe-required-space)
			 (quotient (memory-status (enum memory-status-option heap-size) 0)
				   10)
			 (car maybe-required-space)))
  (session-data-set! space-shortage-handler handler))
