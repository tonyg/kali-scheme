; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file wind.scm.  (Rhymes with "find," not "pinned.")

;;;; Dynamic-wind

; This is a version of dynamic-wind that tries to do "the right thing"
; in the presence of multiple threads of control.
; This definition of "the right thing" is due to Pavel Curtis, and is
; the one used in Scheme Xerox.  It is very different from what you will
; find in, say, MIT Scheme.
;
; When we want to go to a new target state (e.g. on invoking a
; continuation), we ascend to the nearest common ancestor of the
; current state and the target state, executing the "out" (or
; "unwind") thunk for each state on the way; then we climb back down
; to the target state executing the "in" thunk for each state.  Unlike
; the Hanson/Lamping algorithm, the tree of states is not altered in
; any way.
;
; Each thread starts out in the root state, but continuations capture
; the state where they're created.

; Dynamic-wind

(define (dynamic-wind in body out)
  (in)
  (let ((here (get-dynamic-point)))
    (set-dynamic-point! (make-point (if here
					(+ (point-depth here) 1)
					1)
				    in
				    out
				    (get-dynamic-env)
				    here))
    (let ((results (call-with-values body list)))
      (set-dynamic-point! here)
      (out)
      (apply values results))))

; call-with-current-continuation

(define (call-with-current-continuation proc)
  (primitive-cwcc
    (lambda (cont)
      (let ((env (get-dynamic-env))
	    (point (get-dynamic-point))
	    (proposal (current-proposal)))
	;; don't close over PROC
	(proc (continuation->procedure cont env point proposal))))))

(define (continuation->procedure cont env point proposal)
  (lambda results
    (travel-to-point! (get-dynamic-point) point)
    (set-dynamic-env! env)
    (set-dynamic-point! point)
    (set-current-proposal! proposal)
    (with-continuation cont
      (lambda ()
	(apply values results)))))

; Point in state space = <depth, in, out, dynamic-env, parent>
; dynamic-env = dynamic environment for execution of the in and out thunks

(define-record-type point :point
  (make-point depth in out dynamic-env parent)
  (depth point-depth)
  (in point-in)
  (out point-out)
  (dynamic-env point-dynamic-env)
  (parent point-parent))

; To make the modularity simpler, and to help Kali, the root point is #F.

; Go to a point in state space.  This involves running out-thunks from
; the current point out to its common ancestor with the target, and
; then running in-thunks from the ancestor to the target.

(define (travel-to-point! here target)
  (cond ((eq? here target) 'done)
	((or (not here)             ; HERE has reached the root.
	     (and target
		  (< (point-depth here)
		     (point-depth target))))
	 (travel-to-point! here (point-parent target))
	 (set-dynamic-env! (point-dynamic-env target))
	 (set-dynamic-point! target)
	 ((point-in target)))
	(else
	 (set-dynamic-env! (point-dynamic-env here))
	 (set-dynamic-point! here)
	 ((point-out here))
	 (travel-to-point! (point-parent here) target))))

; (put 'let-dynamic-point 'scheme-indent-hook 1)
