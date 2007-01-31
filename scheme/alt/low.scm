; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Portable versions of low-level things that would really like to rely
; on the Scheme 48 VM or on special features provided by the byte code
; compiler.

(define (vector-unassigned? v i) #f)

(define maybe-open-input-file open-input-file)
(define maybe-open-output-file open-output-file)

