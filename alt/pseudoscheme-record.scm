; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define make-record-type   #'scheme-translator::make-record-type)
(define record-constructor #'scheme-translator::record-constructor)
(define record-accessor    #'scheme-translator::record-accessor)
(define record-modifier    #'scheme-translator::record-modifier)
(define record-predicate   #'scheme-translator::record-predicate)
(define define-record-discloser #'scheme-translator::define-record-discloser)

; Internal record things, for inspector or whatever
(define disclose-record #'scheme-translator::disclose-record)
(define record-type #'scheme-translator::record-type)
(define (record? x) (if (record-type x) #t #f))

