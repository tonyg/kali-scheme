; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; The types.

(define syntax
  (loophole type syntax-type))

(define any-values
  (loophole type any-values-type))

(define some-values
  (loophole (procedure any-values type)
	    some-values-type))

(define value
  (loophole type value-type))

(define variable
  (loophole (procedure (some-values type) type)
	    variable-type))

(define procedure
  (loophole (procedure (some-values type type) type)
	    procedure-type))

; Use the definition from the meta-types module
;(define-syntax proc
;  (syntax-rules ()
;    ((proc (?type ...) ?cod)
;     (procedure (some-values ?type ...) ?cod))))


; Various base types

(define boolean
  (loophole type boolean-type))
(define pair
  (loophole type pair-type))
(define number
  (loophole type number-type))
(define unspecific
  (loophole type unspecific-type))

(define char
  (loophole type char-type))
(define structure
  (loophole type 'structure))
; etc. etc.


(define type
  (loophole type 'type))
