; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


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
