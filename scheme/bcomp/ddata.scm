; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Stuff moved from segment.scm  6/5/93
; Some of that stuff moved to state.scm 4/28/95

; Debug-data records are for communicating information from the
; compiler to various debugging tools.

; An environment map has the form
;   #(pc-before pc-after #(name ...) (env-map ...))
; where the two pc's delimit the region of code that executes in this
; environment.  The names indicate which variables become bound at
; pc-before, and the list of env-maps is for inferior (deeper)
; environments.

; pc-in-parent gives the pc of a make-closure instruction in the code
; for this template's superior (outer) lambda-expression.

; Source is in the form of an a-list mapping pc's used in continuations
; to pairs of the form (i . expression), indicating that the continuation
; is returning the value of i'th subexpression in the source expression.

(define-record-type debug-data :debug-data
  (make-debug-data uid name parent pc-in-parent env-maps source)
  debug-data?
  (uid      debug-data-uid)
  (name	    debug-data-name)
  (parent   debug-data-parent)
  (pc-in-parent debug-data-pc-in-parent)
  (env-maps debug-data-env-maps set-debug-data-env-maps!)
  (source   debug-data-source set-debug-data-source!))

(define-record-discloser :debug-data
  (lambda (dd)
    (list 'debug-data (debug-data-uid dd) (debug-data-name dd))))

; Returns a list of proper lists describing the environment in effect
; at the given pc with the given template's code vector.
;
; Entries in the environment-maps table (one per template) have the form
;   #(parent-uid pc-in-parent (env-map ...))
;
; Each environment map (one per let or lambda-expression) has the form
;   #(pc-before pc-after (var ...) (env-map ...))
;
; Cf. procedure (note-environment vars segment) in comp.scm.

(define (debug-data-env-shape dd pc)
  (if (debug-data? dd)
      (let loop ((emaps (debug-data-env-maps dd))
                 (shape (debug-data-env-shape (get-debug-data
					        (debug-data-parent dd))
					      (debug-data-pc-in-parent dd))))
        (if (null? emaps)
            shape
            (let ((pc-before (vector-ref (car emaps) 0))
                  (pc-after  (vector-ref (car emaps) 1))
                  (vars      (vector-ref (car emaps) 2))
                  (more-maps (vector-ref (car emaps) 3)))
              (if (and (>= pc pc-before)
                       (< pc pc-after))
                  (loop more-maps
                        (cons (vector->list vars) shape))
                  (loop (cdr emaps) shape)))))
      '()))
