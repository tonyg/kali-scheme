; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file heap-stack.scm.

; Implementing the interpreter stack as a stack of arguments with all
; continuations and environments in the heap.

(define *stack*               (unassigned))
(define *bottom-of-stack*     (unassigned))
(define *stack-limit*         (unassigned))

(define maximum-stack-args       63)
(define arg-stack-overflow-nargs (%+ maximum-stack-args 1))
 ; APPLY needs a little extra room
(define required-stack-arg-count (%+ maximum-stack-args 1))

;;; Measurements

(define copy/closure 0) ; leave these as there are still references lying around
(define copy/overflow 1)
(define copy/preserve 2)

;(define *conts* 0)
;(define *conts-slots* 0)
;(define *conts-overflow* 0)
;(define *conts-overflow-slots* 0)
;(define *conts-preserved* 0)
;(define *conts-preserved-slots* 0)
;(define *conts-from-heap* 0)
;(define *conts-from-heap-slots* 0)

;(define *envs* 0)
;(define *envs-slots* 0)
;(define *envs-closed* 0)
;(define *envs-closed-slots* 0)
;(define *envs-overflow* 0)
;(define *envs-overflow-slots* 0)
;(define *envs-preserved* 0)
;(define *envs-preserved-slots* 0)

;(define (reset-stack-stats)
;  (set! *conts* 0)
;  (set! *conts-slots* 0)
;  (set! *conts-overflow* 0)
;  (set! *conts-overflow-slots* 0)
;  (set! *conts-preserved* 0)
;  (set! *conts-preserved-slots* 0)
;  (set! *conts-from-heap* 0)
;  (set! *conts-from-heap-slots* 0)

;  (set! *envs* 0)
;  (set! *envs-slots* 0)
;  (set! *envs-closed* 0)
;  (set! *envs-closed-slots* 0)
;  (set! *envs-overflow* 0)
;  (set! *envs-overflow-slots* 0)
;  (set! *envs-preserved* 0)
;  (set! *envs-preserved-slots* 0)
;  )

(define (reset-stack-stats)
  0)

(define (print-stack-stats port)
  0)

;(define (print-stack-stats port)
;  (let ((one-record (lambda (name count slots port)
;		      (%newline port)
;		      (%write-string "(" port)
;		      (%write-string name port)
;		      (%write-string " " port)
;		      (%write-number count port)
;		      (%write-number slots port)
;		      (%write-string ")" port))))
;    (%newline port)
;    (%write-string "(continuations" port)
;    (one-record "made" *conts* *conts-slots* port)
;    (one-record "overflow" *conts-overflow* *conts-overflow-slots* port)
;    (one-record "preserved" *conts-preserved* *conts-preserved-slots* port)
;    (one-record "from-heap" *conts-from-heap* *conts-from-heap-slots* port)
;    (%write-string ")" port)

;    (%newline port)
;    (%write-string "(environments" port)
;    (one-record "made" *envs* *envs-slots* port)
;    (one-record "closed" *envs-closed* *envs-closed-slots* port)
;    (one-record "overflow" *envs-overflow* *envs-overflow-slots* port)
;    (one-record "preserved" *envs-preserved* *envs-preserved-slots* port)
;    (%write-string ")" port)
;    (%newline port)
;    ))

;;; Initializing

(define (initialize-stack)
  (set! *stack* (addr-1+ *stack-end*)))

(define initial-stack-heap-space 0)

(define (reset-stack-pointer)
  (set! *stack* (addr-1+ *stack-end*))
  #f)   ; initial value of *cont*

(define (reserve-stack-space size)
  0)

(define (enable-stack-reserve)
  0)

(define (within-stack? p)
  #f)

; Space usage

(define push-size 1)

(define (stack-continuation-size cells)
  (%+ (%+ cells 1)                 ; header
      required-stack-arg-count))   ; pre-checking for pushed arguments

(define (available-on-stack? cells)
  (available? cells))

(define preallocate-stack-space preallocate-heap-space)

(define check-stack-cons check-heap-cons)

(define ensure-stack-space ensure-space)

(define (ensure-env-space count)
  (ensure-space (%+ count 2)))  ; header + superior environment

(define (ensure-heap-env-space count)
  0)

(define (pop-args-into-heap-env count key)
  0)

; What to do with this?  Ignore?

(define *exception-space-used?* #f)

; This is unsafe, but it shouldn't come up in running benchmarks
(define (allow-exception-consing)
  0)

(define (reserve-exception-space)
  0)

;;; Add CELLS cells onto the stack.

(define (stack-add cells)
  (set! *stack* (addr- *stack* (cells->a-units cells))))

;;; Pushing and popping

(define (push x)         ; Check for space is done when continuation is pushed
  (store! *stack* x)
  (stack-add 1))

(define (pop)
  (stack-add -1)
  (fetch *stack*))

; Make a continuation in the heap and put the current argument stack in it.
; Resets the stack pointer.

(define (push-continuation-on-stack arg-count key)
  (pop-args-into-heap stob/continuation
		      arg-count
		      continuation-cells
		      key))

; Copy the arguments out of CONT and back onto the stack.

(define (pop-continuation-from-stack cont)
  (let ((end (addr+ (address-after-header cont)
		    (cells->a-units continuation-cells)))
	(start (addr+ (address-at-header cont)
		      (cells->a-units (continuation-length cont)))))
    (do ((from start (addr-1+ from))
	 (to *stack-end* (addr-1+ to)))
	((addr< from end)
	 (set! *stack* to))
      (store! to (fetch from)))))

(define (pop-args-into-env count key)
  (pop-args-into-heap stob/vector count 1 key))

(define (pop-args-into-heap type count extra-slots key)
  (let ((stob (make-stob type
			 (cells->bytes (%+ count extra-slots))
			 key)))
    (copy-cells! (addr1+ *stack*)
		 (addr+ (cells->a-units extra-slots)
			(address-after-header stob))
		 count)
    (set! *stack* (addr-1+ *stack-end*))
    stob))

; Tracing the stack

(define (trace-stack)
  (trace-locations (addr1+ *stack*) *stack-end*))

(define (arguments-on-stack)
  (%+ -1 (a-units->cells (addr- *stack-end* *stack*))))

; Dummy definitions.

(define (perserve-*env*)
  0)

(define (move-args-above-cont cont nargs)
  0)

(define (stack-size)
  0)

(define (preserve-continuation cont key reason)
  cont)

(define (preserve-*env*)
  0)

(define (restore-continuation cont)
  cont)

(define (get-continuation-from-heap)  ; should never be called
  0)

(define (addr-1+ x)
  (addr- x addressing-units-per-cell))


