; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file init.scm.

; Allocating memory to the heap and stack

; These are all in address units
(define *newspace-begin* (unassigned))
(define *newspace-end*   (unassigned))
(define *oldspace-begin* (unassigned))
(define *oldspace-end*   (unassigned))

(define *stack-begin* (unassigned))
(define *stack-end*   (unassigned))

; size is in cells
(define *stack-size* 10000)

; size is in cells
(define (initialize-heap size)
  (create-memory size quiescent)      ;create memory
  ;; Divide all of memory into three parts.
  (let* ((memory-cells (a-units->cells (addr-difference *memory-end*
							*memory-begin*)))
	 (semisize (cells->a-units (quotient (- memory-cells *stack-size*)
					     2))))
    (set! *newspace-begin* *memory-begin*)
    (set! *newspace-end* (addr+ *newspace-begin* semisize))
    (set! *oldspace-begin* *newspace-end*)
    (set! *oldspace-end* (addr+ *oldspace-begin* semisize))
    (set! *stack-begin* *oldspace-end*)
    (set! *stack-end* (addr+ *stack-begin* (cells->a-units *stack-size*)))
    (reset-heap-pointer)))

