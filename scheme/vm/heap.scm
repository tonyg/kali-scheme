; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Allocation
;  s48-*hp* is the heap pointer and s48-*limit* is the limit beyond which no
;  storage should be allocated.  Both of these are addresses (not
;  descriptors).

(define s48-*hp*)
(define s48-*limit*)

(define *oldspace-hp*)
(define *oldspace-limit*)

; These are all in address units
(define *newspace-begin*)
(define *newspace-end*)
(define *oldspace-begin*)
(define *oldspace-end*)

; For the image code

(define (s48-heap-begin)
  *newspace-begin*)

(define (s48-heap-pointer)
  s48-*hp*)

(define (s48-oldspace-begin)
  *oldspace-begin*)

(define (s48-oldspace-end)
  *oldspace-end*)

; For the GC

(define (heap-pointer)
  s48-*hp*)

(define (set-heap-pointer! new)
  (set! s48-*hp* new))

(define (heap-limit)
  *newspace-end*)

;----------------

(define (s48-initialize-heap heap-size image-size image-start)
  (let* ((minimum-size (* 4 image-size))  ; two semi-spaces, won't totally thrash
	 (heap-size (if (< heap-size minimum-size)
			(begin
			  (write-error-string "heap size ")
			  (write-error-integer heap-size)
			  (write-error-string " is too small, using ")
			  (write-error-integer minimum-size)
			  (write-error-newline)
			  minimum-size)
			heap-size))
	 (heap (allocate-memory (cells->a-units heap-size))))
    (if (null-address? heap)
	(error "unable to allocate heap space")
	(let ((semisize (cells->a-units (quotient heap-size 2))))
	  (set! *newspace-begin* heap)
	  (set! *newspace-end* (address+ *newspace-begin* semisize))
	  (set! *oldspace-begin* *newspace-end*)
	  (set! *oldspace-end* (address+ *oldspace-begin* semisize))
	  (if (address= *oldspace-begin* image-start)
	      (swap-spaces))
	  (set! *oldspace-hp* *oldspace-begin*)
	  (set! *oldspace-limit* *oldspace-end*)
	  (set! s48-*hp*
		(address+ *newspace-begin* (cells->a-units image-size)))
	  (set! s48-*limit* *newspace-end*)
	  *newspace-begin*))))

; To write images we need to be able to undo the swapping.

(define-syntax swap!
  (syntax-rules ()
    ((swap! a b)
     (let ((temp a))
       (set! a b)
       (set! b temp)))))

(define (swap-spaces)
  (swap! s48-*limit*      *oldspace-limit*)
  (swap! s48-*hp*         *oldspace-hp*)
  (swap! *newspace-begin* *oldspace-begin*)
  (swap! *newspace-end*   *oldspace-end*))

;----------------

(define (s48-available? cells)
  (address< (address+ s48-*hp* (cells->a-units cells)) s48-*limit*))

(define (bytes-available? bytes)
  (address< (address+ s48-*hp* (bytes->a-units bytes)) s48-*limit*))

(define (s48-available)
  (a-units->cells (address-difference s48-*limit* s48-*hp*)))

(define (s48-heap-size)
  (address-difference *newspace-end* *newspace-begin*))

(define (store-next! descriptor)
  (store! s48-*hp* descriptor)
  (set! s48-*hp* (address1+ s48-*hp*)))

(define (allocate len)
  (let ((new s48-*hp*))
    (set! s48-*hp* (address+ s48-*hp* (bytes->a-units len)))
    new))

(define (s48-write-barrier stob address value)
  (address+ address (+ stob value))  ; for the type checker
  (unspecific))

;----------------
; Keeping track of all the areas.

(define *pure-areas*)
(define *impure-areas*)
(define *pure-sizes*)
(define *impure-sizes*)
(define *pure-area-count* 0)
(define *impure-area-count* 0)

(define (have-static-areas?)
  (or (< 0 *impure-area-count*)
      (< 0 *pure-area-count*)))

(define (s48-register-static-areas pure-count pure-areas pure-sizes
				   impure-count impure-areas impure-sizes)
  (set! *pure-area-count* pure-count)
  (set! *pure-areas* pure-areas)
  (set! *pure-sizes* pure-sizes)
  (set! *impure-area-count* impure-count)
  (set! *impure-areas* impure-areas)
  (set! *impure-sizes* impure-sizes))

(define (walk-areas proc areas sizes count)
  (let loop ((i 0))
    (cond ((>= i count)
	   #t)
	  ((proc (vector-ref areas i)
		 (address+ (vector-ref areas i)
			   (vector-ref sizes i)))
	   (loop (+ i 1)))
	  (else
	   #f))))

(define (walk-pure-areas proc)
  (if (< 0 *pure-area-count*)
      (walk-areas proc *pure-areas* *pure-sizes* *pure-area-count*)
      #t))

(define (walk-impure-areas proc)
  (if (< 0 *impure-area-count*)
      (walk-areas proc *impure-areas* *impure-sizes* *impure-area-count*)
      #t))

;----------------------------------------------------------------
; Finding things in the heap.

(define *finding-type* (enum stob symbol))    ; work around lack of closures

; Call PREDICATE on all objects of type *FINDING-TYPE* found between START and
; END.  The objects for which PREDICATE returns #T are pushed onto the heap
; using STORE-NEXT!.  Returns #T for success and #F for failure.

(define (collect-type-in-area predicate)
  (lambda (start end)
    (let ((type *finding-type*))
      (let loop ((addr start))
	(if (address>= addr end)
	    #t
	    (let* ((d (fetch addr))
		   (next (address+ addr
				   (+ (cells->a-units stob-overhead)
				      (header-length-in-a-units d)))))
	      (cond ((not (header? d))
		     (write-string "heap is in an inconsistent state."
				   (current-error-port))
		     #f)
		    ((not (= type (header-type d)))
		     (loop next))
		    (else
		     (let ((obj (address->stob-descriptor (address1+ addr))))
		       (cond ((not (predicate obj))
			      (loop next))
			     ((s48-available? (cells->a-units 1))
			      (store-next! obj)
			      (loop next))
			     (else
			      #f)))))))))))

; Returns a procedure that will walk the heap calling PREDICATE on every
; object of a particular type.  Returns a vector containing all objects
; for which PREDICATE returns #t.  If the heap is screwed up or there isn't
; room for the vector we return FALSE.

(define (generic-find-all predicate)
  (let ((proc (collect-type-in-area predicate)))
    (lambda (type)
      (set! *finding-type* type)                     ; we don't have closures
      (let ((start-hp s48-*hp*))
	(store-next! 0)                              ; reserve space for header
	(cond ((and (proc *newspace-begin* start-hp)
		    (walk-impure-areas proc)
		    (walk-pure-areas proc))
	       (let ((size (address-difference s48-*hp* (address1+ start-hp))))
		 (store! start-hp (make-header (enum stob vector) size) )
		 (address->stob-descriptor (address1+ start-hp))))
	      (else
	       (set! s48-*hp* start-hp) ; out of space, so undo and give up
	       false))))))

; Find everything with a given type.

(define s48-find-all
  (generic-find-all (lambda (thing) #t)))

; Find all records of a given record type (as determined by the first slot
; in each record).

(define s48-find-all-records
  (let* ((the-record-type false)
	 (finder (generic-find-all
		  (lambda (record)
		    (vm-eq? (record-type record)
			    the-record-type)))))
    (lambda (record-type)
      (set! the-record-type record-type)
      (finder (enum stob record)))))

; Functions for accessing records.  Getting these from STRUCT would introduce
; a circular module dependency.

(define (record-type record)
  (record-ref record -1))

(define (record-ref record offset)
  (fetch (address+ (address-after-header record)
		   (cells->a-units (+ offset 1)))))

;----------------
; Checks for heap consistency.  Quits after ERROR-COUNT problems have been
; found.

(define (s48-check-heap error-count)
  (set! *heap-errors-left* error-count)
  (and (check-area *newspace-begin* s48-*hp*)
       (walk-impure-areas check-area)
       (walk-pure-areas check-area)))

(define *heap-errors-left* 0)

(define (check-area start end)
  (let loop ((addr start))
    (if (address>= addr end)
	#t
	(let* ((d (fetch addr))
	       (next (address+ addr
			       (+ (cells->a-units stob-overhead)
				  (header-length-in-a-units d)))))
	  (cond ((not (header? d))
		 (check-lost "Heap-check: unexpected non-header."))
		((address< end next)
		 (check-lost "Heap-check: header too large."))
		((b-vector-header? d)
		 (loop next))
		((check-stob-contents (address1+ addr) next)
		 (loop next))
		(else
		 #f))))))

; Check the descriptors from START (inclusive) to END (exclusive).  This does
; not accept internal headers, which are normally allowed but not currently
; used by the system.

(define (check-stob-contents start end)
  (let loop ((addr start))
    (if (address= addr end)
	#t
	(let ((x (fetch addr)))
	  (cond ((header? x)
		 (check-lost "Heap-check: unexpected header."))
		((or (not (stob? x))
		     (check-stob x))
		 (loop (address1+ addr)))
		(else
		 #f))))))
		 
; Check that STOB points into the heap just after a header.  This will fail
; if there are any pure or impure areas.

(define (check-stob stob)
  (let ((addr (address-at-header stob)))
    (cond ((or (address< addr *newspace-begin*)
	       (address<= s48-*hp* addr))
	   (check-lost "Heap-check: address out of bounds."))
	  ((not (header? (fetch addr)))
	   (check-lost "Heap-check: stob has no header."))
	  (else
	   #t))))

(define (check-lost message)
  (write-string message (current-error-port))
  (newline (current-error-port))
  (set! *heap-errors-left* (- *heap-errors-left* 1))
  (< *heap-errors-left* 1))
