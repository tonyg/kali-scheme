; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structures ((vm-utilities vm-utilities-interface))
  (open prescheme)
  (files (util vm-utilities))
  (begin
;    (define-syntax assert
;      (lambda (exp rename compare)
;    	0))
    (define (assert x)
      (if (not x)
    	  (error "assertion failed")))
    ))

(define-structures ((external external-interface))
  (open scheme bitwise ps-memory)
  (for-syntax (open scheme signals)) ; for error
  (files (util external)))

(define-structures ((channel-io channel-interface)
		    (events event-interface))
  (open scheme big-scheme ps-memory ports
	(subset i/o		(current-error-port))
	(modify prescheme	(prefix prescheme:)
		                (expose open-input-file open-output-file
					close-input-port close-output-port
					errors)))
  (files (util s48-channel)))

; The number of usable bits in a small integer.

(define-structures ((system-spec (export useful-bits-per-word)))
  (open prescheme)
  (begin
    (define useful-bits-per-word 30)   ; in Scheme 48
    ))

