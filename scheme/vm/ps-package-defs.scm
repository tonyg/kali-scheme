; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


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
      (external "extended_vm" (=> (integer integer) integer)))
    (define lookup-external-name
      (external "lookup_external_name" (=> (address address) boolean)))
    (define schedule-interrupt 
      (external "schedule_alarm_interrupt" (=> (integer) integer)))
    (define cheap-time
      (external "CHEAP_TIME" (=> () integer)))
    (define real-time 
      (external "real_time" (=> () integer integer)))
    (define run-time 
      (external "run_time" (=> () integer integer)))))
   
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

