; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Interrupts

(define interrupt-handlers
  (make-vector interrupt-count 0))

(do ((i 0 (+ i 1)))
    ((= i interrupt-count))
  (vector-set! interrupt-handlers i
	       (lambda (enabled-int)
		 (signal 'interrupt i enabled-int))))

(define (initialize-interrupts!)
  (set-interrupt-handlers! interrupt-handlers)
  (set! one-second (time time-option/ticks-per-second #f))
  (set-enabled-interrupts! all-interrupts))

(define time-option/ticks-per-second (enum time-option ticks-per-second))
(define one-second #f)

(define no-interrupts 0)

(define all-interrupts (- (arithmetic-shift 1 interrupt-count) 1))

(define (with-interrupts-inhibited thunk)
  (with-interrupts no-interrupts thunk))

(define (with-interrupts-allowed thunk)
  (with-interrupts all-interrupts thunk))

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


; Signal an interrupt if an insufficient amount of memory is reclaimed by
; a garbage collection.  The amount defaults to 10% of the heap.

(define (interrupt-before-heap-overflow! . maybe-required-space)
  (let ((space (if (null? maybe-required-space)
		   (quotient (memory-status memory-status-option/heap-size 0)
			     10)
		   (car maybe-required-space))))
    (vector-set! interrupt-handlers
		 interrupt/memory-shortage
		 (lambda (ei)
		   (memory-status
		        memory-status-option/set-minimum-recovered-space!
			space)
		   (signal 'interrupt interrupt/memory-shortage ei)))
    (memory-status memory-status-option/set-minimum-recovered-space!
		   space)))

(define interrupt/memory-shortage (enum interrupt memory-shortage))
(define memory-status-option/set-minimum-recovered-space!
  (enum memory-status-option set-minimum-recovered-space!))
(define memory-status-option/heap-size
  (enum memory-status-option heap-size))

