; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file gc.scm.

;;;; Garbage collector

(define *initial-heap-size* 100000)

(define (reset-heap-pointer)
  (set! *hp* *newspace-begin*)
  (set! *limit* *newspace-end*))

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
; (BEGIN-COLLECTION)
; (TRACE-VALUE value) => copied value
; (TRACE-LOCATIONS start end)
; (CLEAN-WEAK-POINTERS)
; (END-COLLECTION)
; (ABORT-COLLECTION)

(define *gc-count* 0)
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
      (let* ((start *hp*)
	     (new (copy-object value)))
	(scan-heap start)
	new)
      value))
    
(define (trace-locations start end)
  (let ((starting-hp *hp*))
    (scan start end)
    (scan-heap starting-hp)))

(define (end-collection)
  (clean-weak-pointers)
  (map-over-open-ports!
   (lambda (port)
     (cond ((stob? (stob-header port))
	    (stob-header port))
	   (else
	    ;; the PORT-ID may have been copied
	    (let ((id (port-id port)))
	      (if (stob? (stob-header id))
		  (set-port-id! port (stob-header id))))
	    (close-port-noisily port)
	    false))))
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

(define (scan-heap start)
  (let loop ((start start))
    (let ((end *hp*))
      (scan start end)
      (cond ((addr>= *hp* *limit*)
	     (error "out of memory (sorry)"))
	    ((addr< end *hp*)
	     (loop end))))))

; Copy everything pointed to from somewhere between START and END.

(define (scan start end)
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
	   (let ((new (make-stob-descriptor *hp*)))
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

; The number of weak pointers in each block.
(define weak-pointer-alloc-count 128)

; The size of a block of weak pointers.
(define weak-pointer-alloc-quantum
  (cells->a-units (* weak-pointer-alloc-count weak-pointer-size)))

; Used both to detect weak pointers and for setting the headers when the
; weak-pointer blocks are scanned.
(define weak-pointer-header
  (make-header stob/weak-pointer (cells->bytes (- weak-pointer-size 1))))

; A header used to stop the GC from scanning weak-pointer blocks.
(define weak-alloc-area-header
  (make-header stob/code-vector
	       (cells->bytes (- (* weak-pointer-alloc-count weak-pointer-size)
				1))))  ; don't count the header

(define (copy-weak-pointer weak)
  (if (or (= -1 *weak-pointer-hp*)
	  (>= *weak-pointer-hp* *weak-pointer-limit*))
      (allocate-more-weak-pointer-space))
  (let ((new (make-stob-descriptor (addr1+ *weak-pointer-hp*))))
    (store! (addr1+ *weak-pointer-hp*) (weak-pointer-ref weak))
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
				       (a-units->bytes unused-portion))))))))

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

;;;; Write-image and read-image

(define level 14)

(define (write-image port restart-proc)
  (map-over-open-ports!
   (lambda (port)
     ;; Don't let the restored image get confused by open ports.
     (cond ((stob? (stob-header port))
	    (close-port-noisily (stob-header port))
	    false)
	   (else
	    port))))
  (write-string "This is a Scheme48 heap image file." port)
  (newline port)
  (write-page port)
  (newline port)
  (write-number level            port)
  (write-number bytes-per-cell   port)
  (write-number (a-units->cells *newspace-begin*) port)
  (write-number (a-units->cells *hp*)             port)
  (write-number restart-proc    port)
  (write-page port)
  (store! *hp* 1)  ; used to detect endianess of image
  (write-bytes *hp* (- (addr1+ *hp*) *hp*) port)
  (write-bytes *newspace-begin* (- *hp* *newspace-begin*) port)
  (- *hp* *newspace-begin*))

(define (read-image filename command-line-space)
  (call-with-input-file filename
    (lambda (port)
      (if (null-pointer? port)
	  (error "Can't open heap image file"))
      (read-page port) ; read past any user cruft at the beginning of the file
      (let* ((old-level          (read-number port))
             (old-bytes-per-cell (read-number port))
             (old-begin (cells->a-units (read-number port)))
             (old-hp    (cells->a-units (read-number port)))
             (startup-proc       (read-number port)))
        (read-page port)
        (if (not (= old-level level))
            (error "format of image is incompatible with this version of system"
                   old-level level))
        (if (not (= old-bytes-per-cell bytes-per-cell))
            (error "incompatible bytes-per-cell"
                   old-bytes-per-cell bytes-per-cell))

        (initialize-heap *initial-heap-size*)

        (let* ((delta (- *newspace-begin* old-begin))
               (new-hp (+ old-hp delta))
               (new-limit *newspace-end*)
	       (initial-space (+ command-line-space
				 (+ initial-i/o-heap-space
				    initial-machine-heap-space))))
          (if (addr>= (+ initial-space new-hp) new-limit)
	      (error "heap not big enough to restore this image"
		     new-hp new-limit)
	      (let ((reverse? (check-image-byte-order port)))
		(read-bytes *hp* (- old-hp old-begin) port)
		(if reverse?
		    (reverse-byte-order new-hp))
		(if (= delta 0)
		    (set! *hp* new-hp)
		    (relocate-image delta new-hp))
		(initialize-i/o-system)  ; clear out port vectors
		(initialize-machine)     ; initialize interpreter
		(adjust startup-proc delta))))))))

(define (check-image-byte-order port)
  (read-bytes *hp* (cells->a-units 1) port)
  (cond ((= (fetch *hp*) 1)
	 #f)
	(else
	 (reverse-descriptor-byte-order! *hp*)
	 (if (= (fetch *hp*) 1)
	     #t
	     (begin (error "unable to correct byte order" (fetch *hp*))
		    #f))))) ; to keep from confusing the type checker

; ABCD => DCBA

; memory intensive, but independent of Scheme's integer size

(define (reverse-descriptor-byte-order! addr)
  (let ((x (fetch-byte addr)))
    (store-byte! addr (fetch-byte (addr+ addr 3)))
    (store-byte! (addr+ addr 3) x))
  (let ((x (fetch-byte (addr+ addr 1))))
    (store-byte! (addr+ addr 1) (fetch-byte (addr+ addr 2)))
    (store-byte! (addr+ addr 2) x)))

(define (reverse-byte-order end)
  (write-string "Correcting byte order of resumed image."
		 (current-output-port))
  (newline (current-output-port))
  (let loop ((ptr *hp*))
    (reverse-descriptor-byte-order! ptr)
    (let ((value (fetch ptr)))
      (if (addr< ptr end)
	  (loop (if (b-vector-header? value)
		    (addr+ (addr1+ ptr) (header-a-units value))
		    (addr1+ ptr)))))))

(define (adjust descriptor delta)
  (if (stob? descriptor)
      (make-stob-descriptor (addr+ (address-after-header descriptor) delta))
      descriptor))

(define (relocate-image delta new-hp)
  (let loop ()
    (cond ((addr< *hp* new-hp)
	   (let ((d (adjust (fetch *hp*) delta)))
	     (store-next! d)
	     (cond ;;((eq? d the-primitive-header)
	       ;; Read symbolic label name.
	       ;;(store-next!
	       ;;  (label->fixnum (name->label (read port)))))
	       ((b-vector-header? d)
		(set! *hp* (addr+ *hp*
				  (cells->bytes
				   (bytes->cells
				    (header-length-in-bytes d)))))))
	     (loop))))))

; The page character is used to mark the ends of the user and prelude sections
; of image files.

(define page-character (ascii->char 12))

(define (write-page port)
  (write-char page-character port))

(define (read-page port)
  (let loop ()
    (let ((ch (read-char port)))
      (cond ((eof-object? ch)
	     (error "end of file while looking for page break"))
	    ((not (char=? ch page-character))
	     (loop))))))


; Collect all the symbols into a symbol table.

;(define (add-symbols-to-table table)
;  (let ((start-hp *hp*))
;    (let loop ((addr *newspace-begin*))
;      (cond ((addr< addr start-hp)
;	     (let ((d (fetch addr))
;		   (addr (addr1+ addr)))
;	       (cond ((b-vector-header? d)
;		      (loop (addr+ addr (header-a-units d))))
;		     ((not (vm-symbol? d))
;		      (loop addr))
;		     ((available? vm-pair-size)
;		      (let ((key (preallocate-space vm-pair-size)))
;			(add-to-symbol-table d table key)
;			(loop addr)))
;		     (else
;		      (vm-vector-fill! table null)
;		      #f))))
;	    (else
;	     #t)))))

(define (walk-heap type proc limit)
  (let loop ((addr *newspace-begin*))
    (cond ((addr< addr limit)
	   (let ((d (fetch addr)))
	     (cond ((not (header? d))
		    (write-string "Heap is in an inconsistent state."
		                  (current-output-port))
		    #f)
		   ((or (not (= type (header-type d)))
			(proc (make-stob-descriptor (addr1+ addr))))
		    (loop (addr1+ (addr+ addr (header-a-units d)))))
		   (else
		    #f))))
	  (else
	   #t))))

(define (add-symbols-to-table table)
  (let ((start-hp *hp*))
    (cond ((walk-heap stob/symbol
		      (lambda (symbol)
			(if (available? vm-pair-size)
			    (let ((key (preallocate-space vm-pair-size)))
			      (add-to-symbol-table symbol table key)
			      #t)
			    #f))
		      start-hp)
	   #t)
	  (else
	   (set! *hp* start-hp) ; out of space, so undo and give up
	   #f))))

(define (find-all-xs type)
  (let ((start-hp *hp*))
    (store-next! 0)                              ; reserve space for header
    (cond ((walk-heap type
		      (lambda (stob)
			(cond ((available? (cells->a-units 1))
			       (store-next! stob)
			       #t)
			      (else #f)))
		      start-hp)
	   (let ((size (addr- *hp* (addr1+ start-hp))))
	     (store! start-hp (make-header stob/vector size) )
	     (make-stob-descriptor (addr1+ start-hp))))
	  (else
	   (set! *hp* start-hp) ; out of space, so undo and give up
	   false))))


