; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Stuff moved from segment.scm  6/5/93
; Some of that stuff moved to state.scm 4/28/95

; Debug-data records are for communicating information from the
; compiler to various debugging tools.

; An environment map has the form
;   #(pc-before pc-after #(name+ ...) offset (env-map ...))
; where the two pc's delimit the region of code that executes in this
; environment.  The names indicate variables bound at from that stack
; offset up.  A name+ is either a name or a vector of names indicating
; that the stack holds a vector of values at that point.  The list of
; env-maps is for inferior (deeper) environments.

; Source is in the form of an a-list mapping pc's used in continuations
; to pairs of the form (i . expression), indicating that the continuation
; is returning the value of i'th subexpression in the source expression.

(define-record-type debug-data :debug-data
  (make-debug-data uid name parent env-maps source)
  debug-data?
  (uid      debug-data-uid)
  (name	    debug-data-name)
  (parent   debug-data-parent)
  (env-maps debug-data-env-maps set-debug-data-env-maps!)
  (source   debug-data-source set-debug-data-source!))

(define-record-discloser :debug-data
  (lambda (dd)
    (list 'debug-data (debug-data-uid dd) (debug-data-name dd))))

; Returns a list of proper lists describing the environment in effect
; at the given pc with the given template's code vector.
;
; Entries in the environment-maps table (one per template) have the form
;   #(#(pc-before pc-after #(var ...) offset (env-map ...)) ...)
;
; A PC of #F indicates that the caller wants the environment map for
; the closure itself, which will be the last thing in the outermost
; environment map (because that matches where the environment is pushed
; onto the stack).
;
; Cf. procedure (note-environment vars segment) in segment.scm.

(define (debug-data-env-shape dd pc)
  (cond ((not (debug-data? dd))
	 '())
	(pc
	 (let loop ((emaps (debug-data-env-maps dd))
		    (shape '()))
	   (if (null? emaps)
	       shape
	       (let ((pc-before (vector-ref (car emaps) 0))
		     (pc-after  (vector-ref (car emaps) 1))
		     (offset    (vector-ref (car emaps) 2))
		     (vars      (vector-ref (car emaps) 3))
		     (more-maps (vector-ref (car emaps) 4)))
		 (if (and (>= pc pc-before)
			  (< pc pc-after))
		     (loop more-maps
			   (cons (cons offset
				       (vector->list vars))
				 shape))
		     (loop (cdr emaps) shape))))))
	((not (null? (debug-data-env-maps dd)))
	 (let ((names (vector-ref (car (debug-data-env-maps dd))
				  3)))
	   (if (and names
		    (< 0 (vector-length names))
		    (pair? (vector-ref names (- (vector-length names) 1))))
	       (list (vector-ref names (- (vector-length names) 1)))
	       '())))
	(else
	 '())))

