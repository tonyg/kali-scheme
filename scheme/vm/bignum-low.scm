; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Manipulating bignums.

(define (extract-bignum desc)
  (assert (bignum? desc))
  (address-after-header desc))

(define (enter-bignum external-bignum)
  (let ((desc (address->stob-descriptor external-bignum)))
    (assert (bignum? desc))
    (make-immutable! desc)
    desc))

; The extra digit is where the MIT bignum code stores the length.

(define (bignum-alloc number-of-digits key)
  (make-bignum (cells->bytes (+ number-of-digits 1)) key))

; This doesn't use ENTER-BIGNUM because we need to preserve mutability
; until the entire bignum operation has completed.
;
; If the new size is smaller we change the length in the header and install a
; new header at the beginning of the now-unused bytes at the end.

(define (shorten-bignum external-bignum number-of-digits)
  (let ((bignum (address->stob-descriptor external-bignum)))
    (let ((new-size (cells->bytes
		      (bignum-size (cells->bytes (+ number-of-digits 1)))))
	  (old-size (header-lengh-in-bytes (stob-header bignum))))
      (assert (<= new-size old-size))
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
