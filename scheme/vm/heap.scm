; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

; For the GC (which is in another module)

(define (heap-pointer)
  s48-*hp*)

(define (set-heap-pointer! new-hp)
  (set! s48-*hp* new-hp))

(define (heap-start)
  *newspace-begin*)

(define (heap-limit)
  *newspace-end*)

;----------------

(define (s48-initialize-heap start size)
  (let ((semisize (cells->a-units (quotient size 2))))
    (set! *newspace-begin* start)
    (set! *newspace-end* (address+ *newspace-begin* semisize))
    (set! s48-*hp* *newspace-begin*)
    (set! s48-*limit* *newspace-end*)
    (set! *oldspace-begin* *newspace-end*)
    (set! *oldspace-end* (address+ *oldspace-begin* semisize))
    (set! *oldspace-hp* *oldspace-begin*)
    (set! *oldspace-limit* *oldspace-end*)))

; To write images we need to be able to undo the swapping.

(define-syntax swap!
  (syntax-rules ()
    ((swap! a b)
     (let ((temp a))
       (set! a b)
       (set! b temp)))))

(define (swap-spaces)
  (swap! s48-*limit*          *oldspace-limit*)
  (swap! s48-*hp*             *oldspace-hp*)
  (swap! *newspace-begin* *oldspace-begin*)
  (swap! *newspace-end*   *oldspace-end*))

;----------------

(define (s48-available? cells)
  (address< (address+ s48-*hp* (cells->a-units cells)) s48-*limit*))

(define (s48-available)
  (a-units->cells (address-difference s48-*limit* s48-*hp*)))

(define (s48-heap-size)
  (address-difference *newspace-end* *newspace-begin*))

(define (store-next! descriptor)
  (store! s48-*hp* descriptor)
  (set! s48-*hp* (address1+ s48-*hp*)))

; Pre-Allocation
;
; Preallocation and keys are used to ensure that for every call to MAKE-STOB
; there is a corresponding call to ENSURE-SPACE to see if there is sufficient
; heap space.  ENSURE-SPACE returns a key and MAKE-STOB checks that the
; key it is passed is the most recently allocated key and that the space
; needed is no greater than the argument to ENSURE-SPACE.
; 
; Another solution would be to make ENSURE-SPACE and MAKE-STOB a single
; procedure.  The difficulty is that ENSURE-SPACE may trigger a garbage
; collection, which in turn requires that all live data be reachable
; from the VM's registers.  The VM solves this by only calling ENSURE-SPACE
; at the beginning of an instruction, before any values have been removed
; from the stack or any of the registers.  Once the key has been obtained
; the instruction is free to make any number of calls to MAKE-STOB, as long
; as the total heap space required is no more than what was checked for.
; 
; There is a flag, CHECK-PREALLOCATION?, that determines whether MAKE-STOB
; actually checks the keys.  In the VM as seen by the Pre-Scheme compiler
; this flag is defined to be #f and never set, so all of the key code is
; constant-folded into oblivion.
; 
; The main virtue of the keys is not that they can be checked but
; that they exist at all.  MAKE-STOB requires a key argument, and
; if there is none available you know that you forgot an ENSURE-SPACE.
; Occasionally I run the VM in Scheme with checking enabled, just
; to see if it all still works.

(define check-preallocation? #f)

(define *heap-key* 0)
(define *okayed-space* 0)

(define (s48-preallocate-space cells)
  (cond (check-preallocation?
	 (assert (s48-available? cells))
	 (set! *heap-key* (+ *heap-key* 1))
	 (set! *okayed-space* cells)
	 *heap-key*)
	(else
	 0)))

(define (s48-allocate-space type len key)	;len is in bytes
  (= type 0)      ; declaration for type-checker
  (if check-preallocation?
      (let ((cells (+ (bytes->cells len) 1)))
	(assert (s48-available? cells))
	(if (not (and (= key *heap-key*)
		      (>= *okayed-space* cells)))
	    (error "invalid heap key" key cells))
	(set! *okayed-space* (- *okayed-space* cells))))
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
      (walk-areas proc *pure-areas* *pure-sizes* *pure-area-count*)))

(define (walk-impure-areas proc)
  (if (< 0 *impure-area-count*)
      (walk-areas proc *impure-areas* *impure-sizes* *impure-area-count*)))

;----------------------------------------------------------------
; Finding things in the heap.

(define *finding-type* (enum stob symbol))    ; work around lack of closures

; Call PREDICATE on all objects of type *FINDING-TYPE* found between START and END.
; The objects for which PREDICATE returns #T are pushed onto the heap using STORE-NEXT!.
; Returns #T for success and #F for failure.

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

(define find-resumer-records
  (let ((finder (generic-find-all
		  (lambda (record)
		    (let ((type (record-type record)))
		      (and (record? type)
			   (stob? (record-type-resumer type))))))))
    (lambda ()
      (finder (enum stob record)))))

; Functions for accessing records.  Getting these from STRUCT would introduce
; a circular module dependency.

(define (record? x)
  (and (stob? x)
       (= (header-type (stob-header x))
	  (enum stob record))))
    
(define (record-type record)
  (record-ref record -1))

(define (record-type-resumer record-type)
  (record-ref record-type 0))

(define (record-ref record offset)
  (fetch (address+ (address-after-header record)
		   (cells->a-units (+ offset 1)))))
