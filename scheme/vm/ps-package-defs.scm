; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structures ((vm-utilities vm-utilities-interface))
  (open prescheme)
  (files vm-utilities)
  (begin
    (define-syntax assert
      (lambda (exp rename compare)
    	0))
    ))

(define-structures ((external external-interface))
  (open prescheme)
  (begin
    (define extended-vm
      (external "s48_extended_vm" (=> (integer integer) integer)))
    (define external-call
      (external "s48_external_call" (=> (integer integer integer address)
					integer)))
    (define schedule-interrupt 
      (external "s48_schedule_alarm_interrupt" (=> (integer) integer)))
    (define cheap-time
      (external "CHEAP_TIME" (=> () integer)))
    (define real-time 
      (external "s48_real_time" (=> () integer integer)))
    (define run-time 
      (external "s48_run_time" (=> () integer integer)))))
   
(define-structures ((channel-io channel-interface)
		    (events event-interface))
  (open prescheme)
  (files ps-channel))

; The number of usable bits in a small integer.

(define-structures ((system-spec (export useful-bits-per-word)))
  (open prescheme)
  (begin
    (define useful-bits-per-word 32)   ; when compiled
    ))

