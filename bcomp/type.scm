; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; The types.

(define :syntax
  (loophole :type syntax-type))

(define :values
  (loophole :type any-values-type))

(define some-values
  (loophole (procedure :values :type)
	    some-values-type))

(define :value
  (loophole :type value-type))

(define variable
  (loophole (procedure (some-values :type) :type)
	    variable-type))

(define procedure
  (loophole (procedure (some-values :type :type) :type)
	    procedure-type))

; Use the definition from the meta-types module
;(define-syntax proc
;  (syntax-rules ()
;    ((proc (?type ...) ?cod)
;     (procedure (some-values ?type ...) ?cod))))


; Various base types

(define :boolean
  (loophole :type boolean-type))
(define :char
  (loophole :type char-type))
(define :number
  (loophole :type number-type))
(define :null
  (loophole :type null-type))
(define :unspecific
  (loophole :type unspecific-type))

(define :pair
  (loophole :type pair-type))
(define :string
  (loophole :type string-type))
(define :symbol
  (loophole :type symbol-type))
(define :vector
  (loophole :type vector-type))
(define :procedure
  (loophole :type (procedure-type any-values-type any-values-type)))

(define :zero
  (loophole :type zero-type))
(define :escape
  (loophole :type escape-type))

(define :structure
  (loophole :type ':structure))
(define :type
  (loophole :type ':type))
