; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The types.

(define :syntax
  (loophole :type syntax-type))

(define :values
  (loophole :type any-values-type))

(define :arguments
  (loophole :type any-arguments-type))

(define :value
  (loophole :type value-type))

(define procedure
  (loophole (proc (:type :type) :type)
	    (lambda (dom cod) (procedure-type dom cod #t))))

; Use the definitions of PROC and SOME-VALUES from the meta-types module


; Various base types

(define :boolean     (loophole :type boolean-type))
(define :char        (loophole :type char-type))
(define :null        (loophole :type null-type))
(define :unspecific  (loophole :type unspecific-type))

(define :number      (loophole :type number-type))
(define :complex     (loophole :type complex-type))
(define :real	     (loophole :type real-type))
(define :rational    (loophole :type rational-type))
(define :integer     (loophole :type integer-type))
(define :exact-integer (loophole :type exact-integer-type))

(define :pair        (loophole :type pair-type))
(define :string      (loophole :type string-type))
(define :symbol      (loophole :type symbol-type))
(define :vector	     (loophole :type vector-type))
(define :procedure   (loophole :type any-procedure-type))

; Temporary
(define :input-port :value)
(define :output-port :value)

(define :error       (loophole :type error-type))
(define :escape	     (loophole :type escape-type))

(define :structure   (loophole :type structure-type))
(define :type	     (loophole :type value-type))
