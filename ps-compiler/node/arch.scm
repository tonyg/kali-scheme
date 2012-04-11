; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.


; These are all of the primitives that are known to the compiler.

; The enumeration is needed by the expander for LET-NODES so it ends up
; being loaded into two separate packages.

(define-enumeration primop
  (
   ; Nontrivial Primops
   call              ; see below
   tail-call
   return
   jump
   throw

   unknown-call
   unknown-tail-call
   unknown-return

   dispatch          ; (dispatch <cont1> ... <contN> <exp>)
   let               ; (let <lambda-node> . <args>)
   letrec1           ; (letrec1 (lambda (x v1 v2 ...)
   letrec2           ;            (letrec2 <cont> x <lambda1> <lambda2> ...)))

   cell-set!
   global-set!

   undefined-effect  ; (undefined-effect . <maybe-args>)

   ; Trivial Primops
   make-cell
   cell-ref
   global-ref

   ; Environment stuff, these are both trivial
   closure
   env-ref
   ))
