
(define-structures ((pre-scheme pre-scheme-interface))
  (open scheme code-vectors bitwise ascii primitives signals)
  (files prescheme ps-memory))

(define-structures ((vm-utilities vm-utilities-interface))
  (open pre-scheme)
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

; The number of usable bits in a small integer.

(define-structures ((system-spec (export useful-bits-per-word)))
  (open pre-scheme)
  (begin
    (define useful-bits-per-word 30)   ; in Scheme 48
    ))

