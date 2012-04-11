; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This is file gc.scm.

; Collector

(define (in-oldspace? descriptor)
  (and (stob? descriptor)
       (let ((a (address-after-header descriptor)))
         (and (address>= a *oldspace-begin*)
              (address< a *oldspace-end*)))))

; The interface to the GC consists of
; (BEGIN-COLLECTION)                        ; call first
; (TRACE-VALUE value) => copied value
; (TRACE-LOCATIONS! start end)              ; trace all pointers
; (TRACE-STOB-CONTENTS! stob)
; (DO-GC finalize-list cont)                ; then do the GC,
;                                           ; calls CONT on traced and untraced
;                                           ;  finalize sublists
; (END-COLLECTION)                          ; and either finish
; (ABORT-COLLECTION)                        ; or abort

(define *gc-count* 0)
(define (gc-count) *gc-count*)

(define *saved-hp*)
(define *saved-limit*)

(define (begin-collection)
  (swap-spaces)
  (set! *saved-limit* *limit*)
  (set! *saved-hp* *hp*)
  (set! *limit* *newspace-end*)
  (set! *hp* *newspace-begin*)
  (set! *weak-pointer-hp* null-address))

(define (swap-spaces)
  (let ((b *newspace-begin*))
    (set! *newspace-begin* *oldspace-begin*)
    (set! *oldspace-begin* b))
  (let ((e *newspace-end*))
    (set! *newspace-end* *oldspace-end*)
    (set! *oldspace-end* e)))

(define (trace-value stob)
  (if (in-oldspace? stob)
      (copy-object stob)
      stob))
    
(define (end-collection)
  (set! *gc-count* (+ *gc-count* 1)))

; Undo the effects of the current collection (assuming that it did not
; modify any VM registers or the stack).

(define (abort-collection)
  (swap-spaces)
  (set! *limit* *saved-limit*)
  (set! *hp* *saved-hp*)
  (let loop ((addr *newspace-begin*))
    (if (address< addr *hp*)
	(let* ((d (fetch addr))
	       (h (if (header? d)
		      d
		      (let ((h (stob-header d)))
			(store! addr h)            ; mend heart
			h))))
	  (loop (address1+ (address+ addr (header-a-units h))))))))

; Complete a GC after all roots have been traced.
; Need to trace the procedures in the finalize list.

(define (do-gc finalizer-alist)
  (walk-impure-areas
   (lambda (start end)
     (trace-locations! start end)
     #t))
  (trace-finalizer-alist! finalizer-alist)
  (really-do-gc *newspace-begin*)
  (clean-weak-pointers)
  (partition-finalizer-alist finalizer-alist))

; Scan the heap, copying pointed to objects, starting from START.  Quit once
; the scanning pointer catches up with the heap pointer.

(define (really-do-gc start)
  (let loop ((start start))
    (let ((end *hp*))
      (trace-locations! start end)
      (cond ((address>= *hp* *limit*)
	     (error "GC error: ran out of space in new heap"))
	    ((address< end *hp*)
	     (loop end))))))

(define (trace-stob-contents! stob)
  (let ((start (address-after-header stob))
	(size (bytes->a-units (header-length-in-bytes (stob-header stob)))))
    (trace-locations! start (address+ start size))))

; Copy everything pointed to from somewhere between START (inclusive)
; and END (exclusive).

(define (trace-locations! start end)
  (let loop ((addr start) (frontier *hp*))
    (if (address< addr end)
	(let ((thing (fetch addr))
	      (next (address1+ addr)))
	  (cond ((b-vector-header? thing)
		 (loop (address+ next (header-a-units thing)) frontier))
		((in-oldspace? thing)
		 (receive (new-thing frontier)
		     (real-copy-object thing frontier)
		   (store! addr new-thing)
		   (loop next frontier)))
		(else
		 (loop next frontier))))
	(set! *hp* frontier)))
  0)  ; for the type-checker

; Copy THING if it has not already been copied.

(define (copy-object thing)
  (receive (new-thing new-hp)
      (real-copy-object thing *hp*)
    (set! *hp* new-hp)
    new-thing))

; NON-*hp* version for better code in TRACE-LOCATIONS

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
		     (address+ data-addr (header-a-units h))))))))

;----------------------------------------------------------------
; Dealing with the list of finalizers.

;  1. Trace the contents of every finalizer object, updating them in oldspace.
;     If any contains a pointer to itself, quit and trace it normally.
;     If any have already been copied ignore them.
;  2. Do the usual GC, including weaks.
;  3. Check each to see if each has been copied.  If not, copy it.  There is
;     no need to trace any additional pointers.

; Walk down the finalizer alist, tracing the procedures and the contents of
; the things.

(define (trace-finalizer-alist! alist)
  (let loop ((alist alist))
    (if (not (vm-eq? alist null))
	(let* ((pair (vm-car alist)))
	  (if (header? (stob-header (vm-car pair)))  ; if not already traced
	      (trace-stob-contents! (vm-car pair)))
	  (vm-set-cdr! pair (trace-value (vm-cdr pair)))
	  (loop (vm-cdr alist))))))

; Walk down the finalizer alist, separating out the pairs whose things
; have been copied.

(define (partition-finalizer-alist alist)
  (let loop ((alist alist) (okay null) (goners null))
    (if (vm-eq? alist null)
	(values okay goners)
	(let* ((alist (copy-object alist))
	       (pair (copy-object (vm-car alist)))
	       (thing (vm-car pair))
	       (next (vm-cdr alist)))
	  (call-with-values
	   (lambda ()
	     (cond ((not (in-oldspace? thing))    ; already copied
		    (values #t thing))
		   ((stob? (stob-header thing))   ; broken heart
		    (values #t (stob-header thing)))
		   (else                          ; no other pointers
		    (values #f (copy-object thing)))))
	   (lambda (traced? thing)
	     (vm-set-car! pair thing)
	     (vm-set-car! alist pair)
	     (cond (traced?
		    (vm-set-cdr! alist okay)
		    (loop next alist goners))
		   (else
		    (vm-set-cdr! alist goners)
		    (loop next okay alist)))))))))

; VM pair operations, defined here to avoid the GC depending on struct.scm.
;
; This is a macro because I am too lazy to add ASSQ, CDDDR, etc. to Pre-Scheme
; just for this.

(define-syntax pair-offset
  (lambda (e r c?)
    (let* ((name (cadr e))
	   (pair-data (assq 'pair stob-data)))
      (do ((i 0 (+ i 1))
	   (data (cdddr pair-data) (cdr data)))
	   ((c? (car (car data)) name)
	    i)))))

(define (vm-car pair)
  (fetch (address+ (address-after-header pair)
		   (cells->a-units (pair-offset car)))))

(define (vm-cdr pair)
  (fetch (address+ (address-after-header pair)
		   (cells->a-units (pair-offset cdr)))))

(define (vm-set-car! pair value)
  (store! (address+ (address-after-header pair)
		    (cells->a-units (pair-offset car)))
          value))

(define (vm-set-cdr! pair value)
  (store! (address+ (address-after-header pair)
		    (cells->a-units (pair-offset cdr)))
          value))

;----------------------------------------------------------------
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
  (make-header (enum stob code-vector)
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
	      (store! end (make-header (enum stob code-vector)
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

