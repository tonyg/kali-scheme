; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.


; Identifying values called by primops

; Is NODE the value being called by a primop?
                            
(define (procedure-node? node)
  (let ((primop (call-primop (node-parent node))))
    (and (primop-procedure? primop)
	 (eq? (primop-call-index (call-primop (node-parent node)))
	      (node-index node)))))

; Get the node called at CALL.

(define (called-procedure-node call)
  (cond ((and (primop-procedure? (call-primop call))
	      (primop-call-index (call-primop call)))
	 => (lambda (i)
	      (call-arg call i)))
	(else '#f)))






