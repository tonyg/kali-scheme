; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


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
  (let ((results (let-dynamic-point (let ((here (get-dynamic-point)))
				      (make-point (+ (point-depth here) 1)
						  in
						  out
						  (get-dynamic-env)
						  here))
		   (lambda ()
		     (call-with-values body list)))))
    (out)
    (apply values results)))

; call-with-current-continuation

(define (call-with-current-continuation proc)
  (primitive-cwcc
    (lambda (cont)
      (let ((env (get-dynamic-env)))
	(proc (continuation->procedure cont env)))))) ;don't close over proc

(define (continuation->procedure cont env)
  (lambda results
    (travel-to-point! (get-dynamic-point) (env-dynamic-point env))
    (set-dynamic-env! env)
    (with-continuation cont
      (lambda () (apply values results)))))

; Point in state space = <depth, in, out, dynamic-env, parent>
; dynamic-env = dynamic environment for execution of the in and out thunks

(define-record-type point :point
  (make-point depth in out dynamic-env parent)
  (depth point-depth)
  (in point-in)
  (out point-out)
  (dynamic-env point-dynamic-env)
  (parent point-parent))

(define root-point			;Shared among all state spaces
  (make-point 0
	      (lambda () (error "winding in to root!"))
	      (lambda () (error "winding out of root!"))
	      '() ;(empty-dynamic-env)	;Should never be seen
	      #f))

(define $dynamic-point (make-fluid root-point))
(define (get-dynamic-point) (fluid $dynamic-point))
(define (env-dynamic-point env)
  (fluid-lookup env $dynamic-point))
(define (let-dynamic-point point thunk)
  (let-fluid $dynamic-point point thunk))

; Go to a point in state space.  This involves running out-thunks from
; the current point out to its common ancestor with the target, and
; then running in-thunks from the ancestor to the target.

(define (travel-to-point! here target)
  (cond ((eq? here target) 'done)
	((< (point-depth here)
	    (point-depth target))
	 (travel-to-point! here (point-parent target))
	 (set-dynamic-env! (point-dynamic-env target))
	 ((point-in target)))
	(else
	 (set-dynamic-env! (point-dynamic-env here))
	 ((point-out here))
	 (travel-to-point! (point-parent here) target))))


; (put 'let-dynamic-point 'scheme-indent-hook 1)
