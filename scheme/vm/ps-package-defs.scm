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
      (external "extended_vm" (=> (int32 int32) int32)))
    (define lookup-external-name
      (external "lookup_external_name" (=> (int32 int32) boolean)))
    (define schedule-interrupt 
      (external "schedule_alarm_interrupt" (=> (int32) int32)))
    (define cheap-time
      (external "CHEAP_TIME" (=> () int32)))
    (define real-time 
      (external "real_time" (=> () int32 int32)))
    (define run-time 
      (external "run_time" (=> () int32 int32)))))
   
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

