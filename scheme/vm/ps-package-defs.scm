; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


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
      (external "s48_run_time" (=> () integer integer)))

    (define s48-call-native-code
      (external "s48_call_native_code" (=> (integer) null)))

    (define get-proposal-lock!
      (external "GET_PROPOSAL_LOCK" (=> () null)))
    (define release-proposal-lock!
      (external "RELEASE_PROPOSAL_LOCK" (=> () null)))

    (define shared-ref
      (external "SHARED_REF" (=> (integer) integer)))
    (define real-shared-set!
      (external "SHARED_SETB" (=> (integer integer) null)))
    (define-syntax shared-set!
      (syntax-rules ()
	((shared-set! x v)
         (real-shared-set! x v))))

    ; Lots of bignum stuff.  This should be moved to its own interface.
    (define export-key
      (external "s48_export_key" (=> (integer) integer)))
    (define external-bignum-make-cached-constants
      (external "s48_bignum_make_cached_constants" (=> () null)))
    (define external-bignum-add
      (external "(char *)s48_bignum_add" (=> (address address) address)))
    (define  external-bignum-subtract
      (external "(char *)s48_bignum_subtract" (=> (address address) address)))
    (define external-bignum-multiply
      (external "(char *)s48_bignum_multiply" (=> (address address) address)))
    (define external-bignum-quotient
      (external "(char *)s48_bignum_quotient" (=> (address address) address)))
    (define external-bignum-remainder
      (external "(char *)s48_bignum_remainder" (=> (address address) address)))
    (define external-bignum-divide
      (external "s48_bignum_divide" (=> (address address) 
 					boolean address address)))
    (define external-bignum-equal?
      (external "s48_bignum_equal_p" (=> (address address) boolean)))
    (define external-bignum-compare
      (external "s48_bignum_compare" (=> (address address) integer)))
    (define external-bignum-test
      (external "s48_bignum_test" (=> (address) integer)))
    (define external-bignum-negate
      (external "(char *) s48_bignum_negate" (=> (address) address)))
    (define external-bignum-arithmetic-shift
      (external "(char *) s48_bignum_arithmetic_shift" 
 		(=> (address integer) address)))
    (define external-bignum-bitwise-not
      (external "(char *) s48_bignum_bitwise_not"
 		(=> (address) address)))
    (define external-bignum-bit-count
      (external "s48_bignum_bit_count"
 		(=> (address) integer)))
    (define external-bignum-bitwise-and
      (external "(char *) s48_bignum_bitwise_and"
 		(=> (address address) address)))
    (define external-bignum-bitwise-ior
      (external "(char *) s48_bignum_bitwise_ior"
 		(=> (address address) address)))
    (define external-bignum-bitwise-xor
      (external "(char *) s48_bignum_bitwise_xor"
 		(=> (address address) address)))
    (define external-bignum-from-long
      (external "(char *) s48_long_to_bignum" (=> (integer) address)))
    (define external-bignum->long
      (external "s48_bignum_to_long" (=> (address) integer)))
    (define external-bignum-fits-in-word?
      (external "s48_bignum_fits_in_word_p" (=> (address integer boolean) 
 						boolean)))
    ))

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

