; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file gc.scm.

; Collector

(define (store-next! descriptor)
  (store! *hp* descriptor)
  (set! *hp* (addr1+ *hp*)))

(define (in-oldspace? descriptor)
  (and (stob? descriptor)
       (let ((a (address-after-header descriptor)))
         (and (addr>= a *oldspace-begin*)
              (addr< a *oldspace-end*)))))

; The interface to the GC consists of
; (BEGIN-COLLECTION)                    ; call first
; (TRACE-VALUE value) => copied value
; (TRACE-LOCATIONS start end)           ; trace all pointers,
; (DO-GC)                               ; then do the GC,
; (END-COLLECTION)                      ; and either finish
; (ABORT-COLLECTION)                    ; or abort

(define *gc-count* 0)
(define (gc-count) *gc-count*)

(define *saved-hp* 0)
(define *saved-limit* 0)

(define (begin-collection)
  (swap-spaces)
  (set! *saved-limit* *limit*)
  (set! *saved-hp* *hp*)
  (set! *limit* *newspace-end*)
  (set! *hp* *newspace-begin*)
  (set! *weak-pointer-hp* -1))

(define (swap-spaces)
  (let ((b *newspace-begin*))
    (set! *newspace-begin* *oldspace-begin*)
    (set! *oldspace-begin* b))
  (let ((e *newspace-end*))
    (set! *newspace-end* *oldspace-end*)
    (set! *oldspace-end* e)))

(define (trace-value value)
  (if (in-oldspace? value)
      (copy-object value)
      value))
    
(define (end-collection)
  (set! *gc-count* (+ *gc-count* 1)))

; Undo the effects of the current collection (assuming that it did not
; modify any VM registers or the stack).

(define (abort-collection)
  (swap-spaces)
  (set! *limit* *saved-limit*)
  (set! *hp* *saved-hp*)
  (let loop ((addr *newspace-begin*))
    (if (addr< addr *hp*)
	(let* ((d (fetch addr))
	       (h (if (header? d)
		      d
		      (let ((h (stob-header d)))
			(store! addr h)            ; mend heart
			h))))
	  (loop (addr1+ (addr+ addr (header-a-units h))))))))

; Scan the heap, copying pointed to objects, starting from START.  Quit once
; the scanning pointer catches up with the heap pointer.

(define (do-gc)
  (let loop ((start *newspace-begin*))
    (let ((end *hp*))
      (trace-locations start end)
      (cond ((addr>= *hp* *limit*)
	     (error "GC error: ran out of space in new heap"))
	    ((addr< end *hp*)
	     (loop end)))))
  (clean-weak-pointers))

; Copy everything pointed to from somewhere between START and END.

(define (trace-locations start end)
  (let loop ((addr start))
    (if (addr< addr end)
	(loop (copy-next addr)))))

; Copy the thing pointed to from ADDR, returning the next address to copy.

(define (copy-next addr)
  (let ((thing (fetch addr))
	(next (addr1+ addr)))
    (cond ((b-vector-header? thing)
	   (addr+ next (header-a-units thing)))
          ((in-oldspace? thing)
	   (store! addr (copy-object thing))
	   next)
          (else
	   next))))

; Copy THING if it has not already been copied.
; It is important that this be in-lined into COPY-NEXT.

(define (copy-object thing)
  (let ((h (stob-header thing)))
    (cond ((stob? h)            ;***Broken heart
	   ;; (assert (in-newspace? h))
	   h)
	  ((vm-eq? weak-pointer-header h)
	   (copy-weak-pointer thing))
	  (else
	   (store-next! h)
	   (let ((new (address->stob-descriptor *hp*)))
	     (stob-header-set! thing new) ;***Break heart
	     (let ((new-hp (addr+ *hp* (header-a-units h))))
	       (do ((o (address-after-header thing) (addr1+ o)))
		   ((addr>= *hp* new-hp))
		 (store-next! (fetch o))))
	     new)))))

; Weak pointers are copied into contiguous blocks so that they can be
; scanned after the main GC has finished.  They have their own heap pointer
; and heap limit.

(define *weak-pointer-hp* -1)
(define *weak-pointer-limit* 0)

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

(define (copy-weak-pointer weak)
  (if (or (= -1 *weak-pointer-hp*)
	  (>= *weak-pointer-hp* *weak-pointer-limit*))
      (allocate-more-weak-pointer-space))
  (let ((new (address->stob-descriptor (addr1+ *weak-pointer-hp*))))
    (store! (addr1+ *weak-pointer-hp*) (d-vector-ref weak 0))
    (set! *weak-pointer-hp* (addr1+ (addr1+ *weak-pointer-hp*)))
    (stob-header-set! weak new) ;***Break heart
    new))

; The weak pointer blocks are linked in their third slot (= the header space
; of the second weak pointer).  The header for the first weak pointer contains
; a header for the block, and the value slots contain the (untraced) values.

(define (allocate-more-weak-pointer-space)
  (let ((old *weak-pointer-hp*))
    (set! *weak-pointer-hp* *hp*)
    (set! *hp* (+ *hp* weak-pointer-alloc-quantum))
    (set! *weak-pointer-limit* *hp*)
    (store! *weak-pointer-hp* weak-alloc-area-header)
    (store! (addr1+ (addr1+ *weak-pointer-hp*)) old)))

; If any weak pointers were found, then get the limits of the most recently
; allocated block and scan it and the rest of the blocks.  Put a string header
; on the unused portion of the block the most recent block.

(define (clean-weak-pointers)
  (if (not (= *weak-pointer-hp* -1))
      (let ((start (addr- *weak-pointer-limit* weak-pointer-alloc-quantum))
	    (end *weak-pointer-hp*))
	(scan-weak-pointer-blocks start end)
	(if (not (>= end *weak-pointer-limit*))
	    (let ((unused-portion (addr- *weak-pointer-limit* (addr1+ end))))
	      (store! end (make-header stob/code-vector
				       (cells->bytes
					(a-units->cells unused-portion)))))))))

(define stob/code-vector (enum stob code-vector))

(define (scan-weak-pointer-blocks start end)
  (let loop ((start start) (end end))
    (let ((next (fetch (addr1+ (addr1+ start)))))
      (scan-weak-pointer-block start end)
      (if (not (= next -1))
	  (loop (addr- next weak-pointer-alloc-quantum) next)))))

; Go from START to END putting headers on the weak pointers and seeing if
; their contents were traced.

(define (scan-weak-pointer-block start end)
  (do ((scan start (addr1+ (addr1+ scan))))
      ((>= scan end))
    (store! scan weak-pointer-header)
    (let ((value (fetch (addr1+ scan))))
      (if (and (in-oldspace? value)
	       (stob? value))
	  (store! (addr1+ scan)
		  (let ((h (stob-header value)))
		    (if (stob? h) h false)))))))

