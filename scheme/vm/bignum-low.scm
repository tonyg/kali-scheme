; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; fixnum-as-bignum-length - the maximum bignum digits required to hold a fixnum
; From struct.scm:
;   bignum-length - usual length operator, gives number of descriptors
;   bignum-size - space to hold a bignum of N descriptors
; Defined here:
;   bignum-digits - number of digits in a bignum
;   bignum-digits->size - space to hold a bignum of N digits
;   fixnum-as-bignum-digits - number of digits to hold a fixnum

; The first word in a bignum is used as a header by the C code.

(define (bignum-digits bignum)
  (- (bignum-length bignum) 1))

(define (bignum-digits->size n)
  (bignum-size (+ n 1)))

(define bignum-digit-bits 30)		; BIGNUM_DIGIT_LENGTH in bignumint.h

(define (bignum-bits-to-digits n)
  (quotient (+ n (- bignum-digit-bits 1))
	    bignum-digit-bits))

(define (bignum-bits-to-size x)
  (bignum-digits->size (bignum-bits-to-digits x)))

(define fixnum-as-bignum-digits (bignum-bits-to-digits bits-per-fixnum))

(define fixnum-as-bignum-size (bignum-digits->size fixnum-as-bignum-digits))

;----------------------------------------------------------------
; Manipulating bignums.

(define (extract-bignum desc)
  (assert (bignum? desc))
  (address-after-header desc))

(define (enter-bignum external-bignum)
  (let ((desc (address->stob-descriptor external-bignum)))
    (if (not (bignum? desc))
	(error "not a bignum" desc))
    (make-immutable! desc)
    desc))

(define (initialize-bignums)
  (ensure-bignum-space! (+ (bignum-digits->size 0)	; zero
			   (bignum-digits->size 1)	; 1
			   (bignum-digits->size 1)))    ; -1
  (external-bignum-make-cached-constants))
  
(define *bignum-preallocation-key*)

(define (set-bignum-preallocation-key! key)
  (if (checking-preallocation?)
      (set! *bignum-preallocation-key* key)))
  
(define (ensure-bignum-space! needed)
  (set-bignum-preallocation-key! (ensure-space needed)))

(define (s48-allocate-bignum size)
  (make-b-vector (enum stob bignum)
		 size
		 *bignum-preallocation-key*))

; This doesn't use ENTER-BIGNUM because we need to preserve mutability
; until the entire bignum operation has completed.
;
; If the new size is smaller we change the length in the header and install a
; new header at the beginning of the now-unused bytes at the end.

(define (s48-shorten-bignum external-bignum number-of-digits)
  (let ((bignum (address->stob-descriptor external-bignum)))
    (let ((new-size (cells->bytes
		      (bignum-size (+ number-of-digits 1))))
	  (old-size (header-length-in-bytes (stob-header bignum))))
      (if  (> new-size old-size)
	   (error "shorten bignum" new-size old-size))
      (if (< new-size old-size)
	  (begin
	    (stob-header-set! bignum
			      (make-header (enum stob bignum) new-size))
	    (stob-header-set! (address->stob-descriptor
			         (address+ (address-after-header bignum)
					   (+ stob-overhead
					      (bytes->a-units new-size))))
			      (make-header (enum stob bignum)
					   (- old-size new-size)))))
      external-bignum)))
