; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code shared by both GCs for the GC package.

; We can't put it in a separate package because of circular
; dependencies.

; Tracing continuations

(define (trace-continuation contents-pointer size)
  (let* ((code (continuation-code contents-pointer))
	 (pc   (continuation-pc       contents-pointer))
	 (code-pointer (address+ (address-after-header code)
				 (extract-fixnum pc)))
	 (mask-size (fetch-byte (address+ code-pointer gc-mask-size-offset))))
    (if (= mask-size 0)
	(s48-trace-locations! contents-pointer
			      (address+ contents-pointer size))
	(let ((data-pointer (address+ contents-pointer
				      continuation-registers-size)))
	  (s48-trace-locations! contents-pointer data-pointer)
	  (s48-trace-continuation-contents! data-pointer
					    code-pointer
					    mask-size))))
  (unspecific))

; The extra values added when a continuation is moved to the heap are not
; included in the continuation's mask.

(define continuation-registers-size
  (cells->a-units continuation-cells))

; Exported for use by the stack code.

(define (s48-trace-continuation-contents! contents-pointer
					  code-pointer
					  mask-size)
  (let ((mask-pointer (address+ code-pointer (+ gc-mask-offset 1))))
    (let byte-loop ((mask-ptr (address- mask-pointer mask-size))
		    (trace-ptr contents-pointer))
      (if (not (address= mask-ptr mask-pointer))
	  (let bit-loop ((mask (fetch-byte mask-ptr)) (ptr trace-ptr))
	    (if (= mask 0)
		(byte-loop (address+ mask-ptr 1)
			   (address+ trace-ptr (cells->a-units 8)))
		(begin
		  (if (odd? mask)
		      (store! ptr (s48-trace-value (fetch ptr))))
		  (bit-loop (arithmetic-shift-right mask 1)
			    (address1+ ptr)))))
	  (unspecific)))))
  
(define (odd? x)
  (= (bitwise-and x 1)
     1))

(define (continuation-code contents-pointer)
  (fetch (address+ contents-pointer
		   (cells->a-units continuation-code-index))))

(define (continuation-pc contents-pointer)
  (fetch (address+ contents-pointer
		   (cells->a-units continuation-pc-index))))

(define (continuation-header? x)
  (= (header-type x)
     (enum stob continuation)))
