; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Collector

; (S48-TRACE-LOCATIONS! start end)	; trace all roots
; (S48-TRACE-VALUE value) => copied value
; (S48-TRACE-STOB-CONTENTS! stob)

(define *gc-count* 0)
(define (s48-gc-count) *gc-count*)

(define (s48-collect)
  (set! *from-begin* (s48-heap-begin))
  (set! *from-end* (heap-limit))
  (swap-spaces)
  (set-heap-pointer! (s48-heap-begin))
  (set! *weak-pointer-hp* null-address)
  (s48-gc-root)			; trace the interpreter's roots
  (do-gc)
  (clean-weak-pointers)
  (s48-post-gc-cleanup)		; interpreter's post-gc clean up
  (set! *gc-count* (+ *gc-count* 1)))

(define *from-begin*)
(define *from-end*)

(define (in-oldspace? descriptor)
  (and (stob? descriptor)
       (let ((a (address-after-header descriptor)))
         (and (address>= a *from-begin*)
              (address< a *from-end*)))))

(define (s48-trace-value stob)
  (if (in-oldspace? stob)
      (copy-object stob)
      stob))

; Scan the heap, copying pointed to objects, starting from START.  Quit once
; the scanning pointer catches up with the heap pointer.

(define (do-gc)
  (let loop ((start (s48-heap-begin)))
    (let ((end (heap-pointer)))
      (s48-trace-locations! start end)
      (cond ((< (s48-available) 0)
	     (error "GC error: ran out of space in new heap"))
	    ((address< end (heap-pointer))
	     (loop end))))))

(define (s48-trace-stob-contents! stob)
  (let ((start (address-after-header stob))
	(size (bytes->a-units (header-length-in-bytes (stob-header stob)))))
    (s48-trace-locations! start (address+ start size))))

; Copy everything pointed to from somewhere between START (inclusive)
; and END (exclusive).

(define (s48-trace-locations! start end)
  (let loop ((addr start) (frontier (heap-pointer)))
    (if (address< addr end)
	(let ((thing (fetch addr))
	      (next (address1+ addr)))
	  (cond ((header? thing)
		 (cond ((b-vector-header? thing)
			(loop (address+ next (header-length-in-a-units thing))
			      frontier))
		       ((continuation-header? thing)
			(let ((size (header-length-in-a-units thing)))
			  (loop (address+ next size)
				(trace-continuation next size frontier))))
		       (else
			(loop next frontier))))
		((in-oldspace? thing)
		 (receive (new-thing frontier)
		     (real-copy-object thing frontier)
		   (store! addr new-thing)
		   (loop next frontier)))
		(else
		 (loop next frontier))))
	(set-heap-pointer! frontier))))
;  0)  ; for the type-checker

; Copy THING if it has not already been copied.

(define (copy-object thing)
  (receive (new-thing new-hp)
      (real-copy-object thing (heap-pointer))
    (set-heap-pointer! new-hp)
    new-thing))

; Non-heap-pointer version for better code in TRACE-LOCATIONS

(define (real-copy-object thing frontier)
  (let ((h (stob-header thing)))
    (cond ((stob? h)            ;***Broken heart
	   ;; (assert (in-newspace? h))
	   (values h frontier))
	  ((and (vm-eq? weak-pointer-header h)
		(in-oldspace? (fetch (address-after-header thing))))
	   (copy-weak-pointer thing frontier))
	  (else
	   (store! frontier h)
	   (let* ((data-addr (address+ frontier (cells->a-units stob-overhead)))
		  (new (address->stob-descriptor data-addr)))
	     (stob-header-set! thing new) ;***Break heart
	     (copy-memory! (address-after-header thing)
			   data-addr
			   (header-length-in-bytes h))
	     (values new
		     (address+ data-addr (header-length-in-a-units h))))))))

(define (s48-extant? thing)
  (or (not (stob? thing))
      (not (in-oldspace? thing))
      (stob? (stob-header thing))))

;----------------
; Continuations

(define (trace-continuation contents-pointer size frontier)
  (set-heap-pointer! frontier)
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
					    mask-size)))
    (heap-pointer)))

; The extra values added when a continuation is moved to the heap are not
; included in the continuation's mask.

(define continuation-registers-size
  (cells->a-units continuation-cells))

					; -1 -2        frame size
(define gc-mask-size-offset -3)		; -3           gc mask size
					; -4 -5        offset
(define gc-mask-offset      -6)         ; -6 ...       mask

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
			    (address1+ ptr)))))))))
  
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

(define (stob-is-template? x)
  (let ((header (stob-header x)))
    (if (header? header)
	(= (header-type header)
	   (enum stob template))
	(stob-is-template? header))))

;----------------
; Weak pointers
;
; Weak pointers are copied into contiguous blocks so that they can be
; scanned after the main GC has finished.  They have their own heap pointer
; and heap limit.

(define *weak-pointer-hp*)
(define *weak-pointer-limit*)

; header + one slot
(define weak-pointer-size 2)

; The number of weak pointers in each block.
(define weak-pointer-alloc-count 128)

; The size of a block of weak pointers.
(define weak-pointer-alloc-quantum
  (cells->a-units (* weak-pointer-alloc-count weak-pointer-size)))

; Used both to detect weak pointers and for setting the headers when the
; weak-pointer blocks are scanned.
(define weak-pointer-header
  (make-header (enum stob weak-pointer) (cells->bytes (- weak-pointer-size 1))))

; A header used to stop the GC from scanning weak-pointer blocks.
(define weak-alloc-area-header
  (make-header (enum stob byte-vector)
	       (cells->bytes (- (* weak-pointer-alloc-count weak-pointer-size)
				1))))  ; don't count the header

(define (copy-weak-pointer weak frontier)
  (let ((frontier (if (or (null-address? *weak-pointer-hp*)
			  (address>= *weak-pointer-hp* *weak-pointer-limit*))
		      (allocate-more-weak-pointer-space frontier)
		      frontier)))
    (let ((new (address->stob-descriptor
		(address+ *weak-pointer-hp* (cells->a-units stob-overhead)))))
      (store! (address1+ *weak-pointer-hp*) (fetch (address-after-header weak)))
      (set! *weak-pointer-hp* (address2+ *weak-pointer-hp*))
      (stob-header-set! weak new) ;***Break heart
      (values new frontier))))

; The weak pointer blocks are linked in their third slot (= the header space
; of the second weak pointer).  The header for the first weak pointer contains
; a header for the block, and the value slots contain the (untraced) values.

(define (allocate-more-weak-pointer-space frontier)
  (let ((old *weak-pointer-hp*)
	(new-frontier (address+ frontier weak-pointer-alloc-quantum)))
    (set! *weak-pointer-hp* frontier)
    (set! *weak-pointer-limit* new-frontier)
    (store! *weak-pointer-hp* weak-alloc-area-header)
    (store! (address2+ *weak-pointer-hp*) (address->integer old))
    new-frontier))

; If any weak pointers were found, then get the limits of the most recently
; allocated block and scan it and the rest of the blocks.  Put a string header
; on the unused portion of the block the most recent block.

(define (clean-weak-pointers)
  (if (not (null-address? *weak-pointer-hp*))
      (let ((start (address- *weak-pointer-limit* weak-pointer-alloc-quantum))
	    (end *weak-pointer-hp*))
	(scan-weak-pointer-blocks start end)
	(if (not (address>= end *weak-pointer-limit*))
	    (let ((unused-portion (address-difference *weak-pointer-limit*
						      (address1+ end))))
	      (store! end (make-header (enum stob byte-vector)
				       (cells->bytes
					(a-units->cells unused-portion)))))))))

(define (scan-weak-pointer-blocks start end)
  (let loop ((start start) (end end))
    (let ((next (integer->address (fetch (address2+ start)))))
      (scan-weak-pointer-block start end)
      (if (not (null-address? next))
	  (loop (address- next weak-pointer-alloc-quantum) next)))))

; Go from START to END putting headers on the weak pointers and seeing if
; their contents were traced.

(define (scan-weak-pointer-block start end)
  (do ((scan start (address2+ scan)))
      ((address>= scan end))
    (store! scan weak-pointer-header)
    (let ((value (fetch (address1+ scan))))
      (if (and (in-oldspace? value)
	       (stob? value))
	  (store! (address1+ scan)
		  (let ((h (stob-header value)))
		    (if (stob? h) h false)))))))

