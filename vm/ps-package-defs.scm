
(define-structures ((vm-utilities vm-utilities-interface))
  (open pre-scheme)
  (files vm-utilities)
  (begin
    (define-syntax assert
      (lambda (exp rename compare)
    	0))
    ))

(define-structures ((external external-interface))
  (open pre-scheme)
  (begin
    (define extended-vm (external (=> (int32 int32) int32) #f))
    (define lookup-external-name (external (=> (int32 int32) boolean) #f))
    (define call-external-value (external (=> (int32 int32 int32) int32) #f))
    (define ps-schedule-interrupt (external (=> (int32) int32) #f))
    (define ps-real-time (external (=> () int32) #f))
    (define ps-run-time (external (=> () int32) #f))
    (define ps-ticks-per-second (external (=> () int32) #f))))
   
; The number of usable bits in a small integer.

(define-structures ((system-spec (export useful-bits-per-word)))
  (open pre-scheme)
  (begin
    (define useful-bits-per-word 32)   ; when compiled
    ))

