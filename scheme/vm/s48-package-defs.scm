; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structures ((vm-utilities vm-utilities-interface))
  (open prescheme)
  (files vm-utilities)
  (begin
;    (define-syntax assert
;      (lambda (exp rename compare)
;    	0))
    (define (assert x)
      (if (not x)
    	  (error "assertion failed")))
    ))

(define-structures ((external external-interface))
  (open scheme)
  (for-syntax (open scheme signals)) ; for error
  (files external))

(define-structures ((channel-io channel-interface)
		    (events event-interface))
  (open scheme big-scheme ps-memory structure-refs ports)
  (access prescheme
	  i/o)         ; current-error-port
  (files s48-channel))

; The number of usable bits in a small integer.

(define-structures ((system-spec (export useful-bits-per-word)))
  (open prescheme)
  (begin
    (define useful-bits-per-word 30)   ; in Scheme 48
    ))

