
; Allocating new space when needed.
;
; We keep a list of unused pages, and try to use them first.
; The free areas are in address order to reduce fragmentation.
; These are like normal areas except they are doubly linked.

; The problem with using these is that they need to be GC'ed.
; There must be a better way.

(define-record-type free-area :free-area
  (really-make-free-area start size next prev)
  (start address free-area-start set-free-area-start!)        ; first page
  (size  integer free-area-size set-free-area-size!)          ; number of pages
  (next  free-area free-area-next set-free-area-next!)	      ; next free-area
  (prev  free-area free-area-previous set-free-area-previous!))

(define (make-free-area start size)
  (let ((new (really-make-free-area start size (null-pointer) (null-pointer))))
    (if (null-pointer? new)
	(error "out of memory"))
    (set! *free-area-count* (+ 1 *free-area-count*))
    new))

(define (deallocate-free-area! area)
  (deallocate area)
  (set! *free-area-count* (- *free-area-count* 1)))

(define (connect! first second)
  (set-free-area-previous! second first)
  (set-free-area-next! first second))

(define (address-after-free-area area)
  (address+ (free-area-start area)
	    (pages->bytes (free-area-size area))))

(define (adjust-free-area! area pages)
  (set-free-area-size! area (+ pages (free-area-size area))))

; The list of free-areas.

(define *freelist*)

; Statistics

(define *free-area-count* 0)
(define *free-page-count* 0)

(define (initialize-allocation)
  (set! *freelist* (make-free-area 0 0))
  (connect! *freelist* *freelist*)
  (set! *free-area-count* 0)
  (set! *free-page-count* 0))

(define (check-freelist)
;  (format #t "[free")
  (let loop ((area (free-area-next *freelist*)) (areas 0) (pages 0))
    (let ((end (address-after-free-area area)))
      (cond ((eq? area *freelist*)
	     (if (not (and (= areas *free-area-count*)
			   (= pages *free-page-count*)))
		 (error "bad page freelist")))
	    ((or (address<= end (free-area-start area))
		 (and (not (eq? *freelist* (free-area-previous area)))
		      (address< (free-area-start area) 
				(address-after-free-area
				  (free-area-previous area)))))
	     (error "bad page freelist"))
	    (else
;             (format #t " ~D-~D"
;                     (bytes->pages (address->integer (free-area-start area)))
;                     (- (bytes->pages (address->integer
;                                       (address-after-free-area area))
;                                      1)))
	     (loop (free-area-next area)
		   (+ areas 1)
		   (+ pages (free-area-size area)))))))
;  (format #t "]~%")
  (unspecific))

; Add SIZE pages starting from START to the set of free pages.
; We walk down the list of free areas to find where START goes and
; then either merge with an existing area or create a new one.

(define (free-pages! start size)
  (let ((end (address+ start (pages->bytes size))))
    (set! *free-page-count* (+ size *free-page-count*))
    (let loop ((before *freelist*))
      (let ((after (free-area-next before)))
	(if (eq? after *freelist*)  ; we're last
	    (if (and (address= start (address-after-free-area before))
		     (not (eq? before *freelist*)))
		(adjust-free-area! before size)
		(let ((new (make-free-area start size)))
		  (connect! before new)
		  (connect! new after)))
	    (let ((end-of-previous (address-after-free-area before)))
	      (assert (address<= end-of-previous start))
	      (cond ((address< (free-area-start after) start)
		     ; We're after AFTER
		     (loop after))
		    ((begin (assert (address<= end (free-area-start after)))
			    #f)
		     (unspecific))
		    ((and (address= start end-of-previous)
			  (not (eq? before *freelist*)))
		     ; merge us with BEFORE
		     (adjust-free-area! before size)
		     (cond ((address= end (free-area-start after))
			    ; and with AFTER, deleting AFTER
			    (adjust-free-area! before (free-area-size after))
			    (connect! before (free-area-next after))
			    (deallocate-free-area! after))))
		    ((address= end (free-area-start after))
		     ; merge us with AFTER
		     (set-free-area-start! after start)
		     (adjust-free-area! after size))
		    (else
		     ; nothing doing, we're on our own
		     (let ((new (make-free-area start size)))
		       (connect! before new)
		       (connect! new after)))))))))
  (check-freelist))

(define (add-pages addr count)
  (+ addr (pages->bytes count)))

; Do a first-fit search of the free list to find a free section of
; between MINIMUM and MAXIMUM pages, inclusive.

(define (allocate-pages minimum maximum)
  (let loop ((area (free-area-next *freelist*)))
    (cond ((eq? area *freelist*)
	   (get-more-memory minimum)
	   (loop (free-area-next *freelist*)))
	  ((< (free-area-size area) minimum)
	   (loop (free-area-next area)))
	  ((< maximum (free-area-size area))
	   (let ((start (free-area-start area)))
	     (set-free-area-start! area (add-pages start minimum))
	     (adjust-free-area! area (- minimum))
	     (set! *free-page-count* (- *free-page-count* minimum))
	     (check-freelist)
	     (values start minimum)))
	  (else
	   (let ((start (free-area-start area))
		 (size (free-area-size area)))
	     (connect! (free-area-previous area) (free-area-next area))
	     (deallocate-free-area! area)
	     (set! *free-page-count* (- *free-page-count* size))
	     (check-freelist)
	     (values start size))))))

;----------------
; Getting more memory from the OS

; Grab at least a quarter-megabyte (2**18) at a time.

(define minimum-allocation-quantum (bytes->pages (expt 2 18)))
	   
; The next place to ask for memory

;(define *memory-frontier*)

; Get more memory from the OS and add it to the page tables.  Returns
; a pointer to COUNT pages worth of memory.

; We grab the memory and then cut it down to even page boundaries.

(define (get-more-memory minimum)
  (let* ((ask-for (+ (max minimum minimum-allocation-quantum)
                     1)) ; may lose up to one full page on the ends
	 (size (pages->bytes ask-for))
         (memory (allocate-memory size)))
;        (memory (allocate-here *memory-frontier* size))
    (if (null-address? memory)
        (error "out of memory"))  ; we may really be able to recover
;    (set! *memory-frontier* (max *memory-frontier* (+ memory size)))
    (let ((start (page-start-address memory)))
      (if (address= start memory)
	  (free-pages! start ask-for)
	  (free-pages! (add-pages start 1)
		       (- ask-for 1))))))
