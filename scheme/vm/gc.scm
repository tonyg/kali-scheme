; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Collector

; The interface to the GC consists of
; (S48-BEGIN-COLLECTION)		; call either this
; (BEGIN-WRITING-IMAGE)			; or this first
; (S48-TRACE-LOCATIONS! start end)	; trace all roots
; (S48-TRACE-VALUE value) => copied value
; (S48-TRACE-STOB-CONTENTS! stob)
; (S48-DO-GC)				; then do the GC
; (S48-END-COLLECTION)			; and either finish
; (ABORT-COLLECTION)			; or abort if writing an image

(define *gc-count* 0)
(define (s48-gc-count) *gc-count*)

; True if the GC is being done for the purpose of dumping an image, in which
; case we check for undumpable records.

(define *writing-image?*)
(define *undumpable-records*)
(define *undumpable-count*)

(define (s48-begin-collection)
  (really-begin-collection)
  (set! *writing-image?* #f)
  (trace-static-areas)
  (unspecific))

(define (begin-writing-image)
  (really-begin-collection)
  (set! *writing-image?* #t)
  (set! *undumpable-records* null)
  (set! *undumpable-count* 0))

(define (really-begin-collection)
  (set! *from-begin* (heap-start))
  (set! *from-end* (heap-limit))
  (swap-spaces)
  (set-heap-pointer! (heap-start))
  (set! *weak-pointer-hp* null-address))

(define *from-begin*)
(define *from-end*)

(define (in-oldspace? descriptor)
  (and (stob? descriptor)
       (let ((a (address-after-header descriptor)))
         (and (address>= a *from-begin*)
              (address< a *from-end*)))))

(define (s48-trace-value stob)
  (cond ((and *writing-image?*
	      (undumpable? stob))
	 (begin
	   (note-undumpable! stob)
	   (s48-trace-value (undumpable-alias stob))))
	((in-oldspace? stob)
	 (copy-object stob))
	(else
	 stob)))
    
(define (s48-end-collection)
  (set! *gc-count* (+ *gc-count* 1)))

(define (s48-undumpable-records)
  (values *undumpable-records*
	  *undumpable-count*))

; Undo the effects of the current collection (assuming that it did not
; modify any VM registers or the stack).

(define (abort-collection)
  (swap-spaces)
  (let loop ((addr (heap-start)))
    (if (address< addr (heap-pointer))
	(let* ((d (fetch addr))
	       (h (if (header? d)
		      d
		      (let ((h (stob-header d)))
			(store! addr h)            ; mend heart
			h))))
	  (loop (address+ addr
			  (+ (cells->a-units stob-overhead)
			     (header-length-in-a-units h))))))))

; Complete a GC after all roots have been traced.

(define (trace-static-areas)
  (walk-impure-areas
   (lambda (start end)
     (s48-trace-locations! start end)
     #t)))

; Scan the heap, copying pointed to objects, starting from START.  Quit once
; the scanning pointer catches up with the heap pointer.

(define (s48-do-gc)
  (let loop ((start (heap-start)))
    (let ((end (heap-pointer)))
      (s48-trace-locations! start end)
      (cond ((< (s48-available) 0)
	     (error "GC error: ran out of space in new heap"))
	    ((address< end (heap-pointer))
	     (loop end)))))
  (clean-weak-pointers))

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
		       (else
			(loop next frontier))))
		((and *writing-image?*
		      (undumpable? thing))
		 (note-undumpable! thing)
		 (store! addr (undumpable-alias thing))
		 (loop addr frontier))
		((in-oldspace? thing)
		 (receive (new-thing frontier)
		     (real-copy-object thing frontier)
		   (store! addr new-thing)
		   (loop next frontier)))
		(else
		 (loop next frontier))))
	(set-heap-pointer! frontier)))
  0)  ; for the type-checker

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

;----------------
; Undumpable records
;
; Record types may be marked as `undumpable', in which case they are replaced in
; images by the value of their first slot.

(define (undumpable? x)
  (and (gc-record? x)
       (let ((type (record-ref x 0)))
	 (and (gc-record? type)
	      (= false (record-ref type 1))))))

(define (gc-record? x)
  (and (stob? x)
       (let ((header (stob-header x)))
	 (if (stob? header)
	     (record? header)
	     (record? x)))))

(define (undumpable-alias record)
  (record-ref record 1))

; This is a bit weird.
;
; We want to cons a list of undumpable records that the user is trying to dump.
; The list is used by the write-image instruction after the image is written out,
; so it needs to be in what is currently oldspace.  We swap the spaces, cons
; onto the list, and then swap back.
;
; We only return the first one-thousand undumpable objects because:
;  A. It is unlikely anyone will want more.
;  B. We don't want to get hung up in MEMQ? in pathological cases.
;  C. Using an NlogN algorithm would be too much work for this.

(define (note-undumpable! thing)
  (if (and (<= *undumpable-count* 1000)
	   (not (vm-memq? thing *undumpable-records*)))
      (begin
	(set! *undumpable-count* (+ 1 *undumpable-count*))
	(swap-spaces)
	(if (s48-available? vm-pair-size)
	    (set! *undumpable-records*
		  (vm-cons thing
			   *undumpable-records*
			   (s48-preallocate-space vm-pair-size))))
	(swap-spaces))))

(define (vm-memq? thing list)
  (let loop ((list list))
    (cond ((vm-eq? null list)
	   #f)
	  ((vm-eq? (vm-car list) thing)
	   #t)
	  (else
	   (loop (vm-cdr list))))))
