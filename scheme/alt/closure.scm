; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.



; Closures

(define closure-rtd      (make-record-type 'closure '(template env)))
(define closure?         (record-predicate closure-rtd))
(define make-closure     (record-constructor closure-rtd '(template env)))
(define closure-template (record-accessor closure-rtd 'template))
(define closure-env	 (record-accessor closure-rtd 'env))
