; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Allocation
;  *hp* is the heap pointer and *limit* is the limit beyond which no
;  storage should be allocated.  Both of these are addresses (not
;  descriptors).

(define *hp* 0)
(define *limit* 0)

; These are all in address units
(define *newspace-begin* (unspecific))
(define *newspace-end*   (unspecific))
(define *oldspace-begin* (unspecific))
(define *oldspace-end*   (unspecific))

(define (initialize-heap start size)
  (let ((semisize (cells->a-units (quotient size 2))))
    (set! *newspace-begin* start)
    (set! *newspace-end* (addr+ *newspace-begin* semisize))
    (set! *oldspace-begin* *newspace-end*)
    (set! *oldspace-end* (addr+ *oldspace-begin* semisize))
    (set! *hp* *newspace-begin*)
    (set! *limit* *newspace-end*)))

(define (available? cells)
  (addr< (addr+ *hp* (cells->a-units cells)) *limit*))

(define (available)
  (a-units->cells (addr- *limit* *hp*)))

(define (heap-size)
  (- *newspace-end* *newspace-begin*))

(define (store-next! descriptor)
  (store! *hp* descriptor)
  (set! *hp* (addr1+ *hp*)))

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
(define universal-key 0)
(define *okayed-space* 0)

(define (preallocate-space cells)
  (cond (check-preallocation?
	 (assert (available? cells))
	 (set! *heap-key* (+ *heap-key* 1))
	 (set! *okayed-space* cells)
	 *heap-key*)
	(else
	 universal-key)))

(define (allocate-space type len key)	;len is in bytes
  (= type 0)      ; declaration for type-checker
  (if check-preallocation?
      (let ((cells (+ (bytes->cells len) 1)))
	(assert (available? cells))
	(cond ((not (= key universal-key))
	       (if (not (and (= key *heap-key*)
			     (>= *okayed-space* cells)))
		   (error "invalid heap key" key cells))
	       (set! *okayed-space* (- *okayed-space* cells))))))
  (let ((new *hp*))
    (set! *hp* (addr+ *hp* (bytes->a-units len)))
    new))

(define (write-barrier address value)
  (+ address value)  ; for the type checker
  (unspecific))

;----------------
; Keeping track of all the areas.

(define *pure-areas*   (unspecific))
(define *impure-areas* (unspecific))
(define *pure-sizes*   (unspecific))
(define *impure-sizes* (unspecific))
(define *pure-area-count*   0)
(define *impure-area-count* 0)

(define (register-static-areas pure-count pure-areas pure-sizes
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
		 (+ (vector-ref areas i)
		    (vector-ref sizes i)))
	   (loop (+ i 1)))
	  (else
	   #f))))

(define (walk-pure-areas proc)
  (walk-areas proc *pure-areas* *pure-sizes* *pure-area-count*))

(define (walk-impure-areas proc)
  (walk-areas proc *impure-areas* *impure-sizes* *impure-area-count*))

;----------------------------------------------------------------
; Finding things in the heap.

(define *finding-type* (enum stob symbol))    ; work around lack of closures

(define (walk-over-type-in-area proc)
  (lambda (start end)
    (let ((type *finding-type*))
      (let loop ((addr start))
	(cond ((addr< addr end)
	       (let ((d (fetch addr)))
		 (cond ((not (header? d))
			(write-string "heap is in an inconsistent state."
				      (current-error-port))
			#f)
		       ((or (not (= type (header-type d)))
			    (proc (address->stob-descriptor (addr1+ addr))))
			(loop (addr1+ (addr+ addr (header-a-units d)))))
		       (else
			#f))))
	      (else
	       #t))))))

; Used to find end of an object
(define (header-a-units h)
  (bytes->a-units (header-length-in-bytes h)))

;(define (walk-over-symbols proc)
;  (let ((proc (walk-over-type-in-area (enum stob symbol) proc))
;        (start-hp *hp*))
;    (cond ((and (proc *newspace-begin* *hp*)
;                (walk-pure-areas proc))
;           #t)
;          (else
;           (set! *hp* start-hp) ; out of space, so undo and give up
;           #f))))

; Returns a procedure that will walk the heap calling PUSHER on every
; object of a particular type.  If PUSHER returns #f the walk is aborted
; and false is returned.  If the walk finishes everything pushed on the
; heap by PUSHER is returned in a vector.

(define (generic-find-all pusher)
  (let ((proc (walk-over-type-in-area pusher)))
    (lambda (type)
      (set! *finding-type* type)                     ; we don't have closures
      (let ((start-hp *hp*))
	(store-next! 0)                              ; reserve space for header
	(cond ((and (proc *newspace-begin* start-hp)
		    (walk-impure-areas proc)
		    (walk-pure-areas proc))
	       (let ((size (addr- *hp* (addr1+ start-hp))))
		 (store! start-hp (make-header (enum stob vector) size) )
		 (address->stob-descriptor (addr1+ start-hp))))
	      (else
	       (set! *hp* start-hp) ; out of space, so undo and give up
	       false))))))

; Find everything with a given type.

(define find-all
  (generic-find-all (lambda (thing)
		      (cond ((available? (cells->a-units 1))
			     (store-next! thing)
			     #t)
			    (else #f)))))

; Find all records of a given record type (as determined by the first slot
; in each record).

(define find-all-records
  (let* ((the-record-type greatest-fixnum-value) ; full-sized integer constant
	 (finder (generic-find-all
		  (lambda (record)
		    (cond ((not (vm-eq? the-record-type
					(fetch (address-after-header record))))
			   #t)
			  ((available? (cells->a-units 1))
			   (store-next! record)
			   #t)
			  (else #f))))))
    (lambda (record-type)
      (set! the-record-type record-type)
      (finder (enum stob record)))))

;----------------------------------------------------------------
; Write-image and read-image

(define (image-writing-okay?)
  (and (= 0 *pure-area-count*)
       (= 0 *impure-area-count*)))

(define *status* (enum errors no-errors))

(define-syntax write-check
  (syntax-rules ()
    ((write-check exp)
     (if (eq? *status* (enum errors no-errors))
	 (set! *status* exp)))))

(define (write-heap-integer n port)
  (write-check (write-integer n port))
  (write-check (write-char #\newline port)))

(define (write-image resume-proc port mark-traced-channels-closed!)
  (begin-collection)
  (let ((resume-proc (trace-value resume-proc)))
    (really-do-gc *newspace-begin*)
    (clean-weak-pointers)
    (mark-traced-channels-closed!)
    (let ((status (really-write-image port resume-proc)))
      (abort-collection)
      status)))

(define (really-write-image port restart-proc)
  (set! *status* (enum errors no-errors))
  (write-check (write-char #\newline port))
  (write-check (write-page port))
  (write-check (write-char #\newline port))
  (write-check (write-string architecture-version port))
  (write-check (write-char #\newline port))
  (write-heap-integer bytes-per-cell port)
  (write-heap-integer (a-units->cells *newspace-begin*) port)
  (write-heap-integer (a-units->cells *hp*) port)
  (write-heap-integer restart-proc port)
  (write-check (write-page port))
  (store! *hp* 1)  ; used to detect endianess of image
  (write-check (write-block port *hp* (- (addr1+ *hp*) *hp*)))
  (write-check (write-block port *newspace-begin* (- *hp* *newspace-begin*)))
  *status*)

; Make sure the image file is okay and return the size of the heap it
; contains.

(define *eof?* #f)

(define-syntax read-check
  (syntax-rules ()
    ((read-check exp)
     (read-check exp -1))
    ((read-check exp losing-value)
     (let ((lose losing-value))
       (if *eof?*
	   lose
	   (receive (thing eof? status)
	       exp
	     (cond (eof?
		    (set! *eof?* #t)
		    lose)
		   ((eq? *status* (enum errors no-errors))
		    thing)
		   (else
		    (set! *eof?* #t)
		    (set! *status* status)
		    lose))))))))

(define (check-image-header filename)
  (receive (port status)
      (open-input-file filename)
    (cond ((error? status)
	   (error-message  "Can't open heap image file")
	   (error-message (error-string status))
	   -1)
	  (else
	   (set! *status* (enum errors no-errors))
	   (set! *eof?* #f)
	   (read-check (read-page port))
	   (read-check (read-newline port)) ; version starts on next line
	   (let* ((same-version? (read-check (check-image-version port) #f))
		  (old-bytes-per-cell (read-check (read-integer port))))
	     (set! *old-begin* (cells->a-units (read-check (read-integer port))))
	     (set! *old-hp* (cells->a-units (read-check (read-integer port))))
	     (set! *startup-proc* (read-check (read-integer port)))
	     (set! *image-port* port)
	     (read-check (read-page port)) ; read to beginning of heap
	     (cond ((error? *status*)
		    (read-lost "Error reading from image file" port))
		   (*eof?*   ; has to come after *status* check
		    (set! *status* (enum errors parse-error))
		    (read-lost "Premature EOF when reading image file" port))
		   ((not same-version?)
		    (read-lost "Format of image is incompatible with this version of system" port))
		   ((not (= old-bytes-per-cell bytes-per-cell))
		    (read-lost "Incompatible bytes-per-cell in image" port))
		   (else
		    (- *old-hp* *old-begin*))))))))

(define (read-page port)
  (read-this-character page-character port))

(define (read-newline port)
  (read-this-character #\newline port))

(define (read-this-character char port)
  (let loop ()
    (receive (ch eof? status)
	(read-char port)
      (cond ((or eof? (error? status))
	     (values -1 eof? status))
	    ((char=? char ch)
	     (values -1 #f status))
	    (else
	     (loop))))))

(define (check-image-version port)
  (let ((len (string-length architecture-version)))
    (let loop ((i 0))
      (receive (ch eof? status)
	  (read-char port)
	(cond ((or eof? (error? status))
	       (values #f eof? status))
	      ((= i len)
	       (values (char=? #\newline ch) #f status))
	      ((char=? ch (string-ref architecture-version i))
	       (loop (+ i 1)))
	      (else
	       (values #f #f status)))))))

(define *image-port* (unspecific))
(define *old-begin* (unspecific))
(define *old-hp* (unspecific))
(define *startup-proc* (unspecific))
  
(define (read-image startup-space)
  (let ((port *image-port*))
    (receive (okay? string)
	(image-read-block port (cells->a-units 1))
      (cond ((not okay?)
	     (read-lost string port))
	    ((= (fetch *hp*) 1)
	     (really-read-image port #f startup-space))
	    (else
	     (reverse-descriptor-byte-order! *hp*)
	     (if (= (fetch *hp*) 1)
		 (really-read-image port #t startup-space)
		 (read-lost "Unable to correct byte order" port)))))))

(define (really-read-image port reverse? startup-space)
  (let* ((delta (- *hp* *old-begin*))
	 (new-hp (+ *old-hp* delta))
	 (new-limit *newspace-end*)
	 (start *hp*))
    (if (addr>= (+ startup-space new-hp) new-limit)
	(read-lost "Heap not big enough to restore this image" port)
	(receive (okay? string)
	    (image-read-block port (- *old-hp* *old-begin*))
	  (cond ((not okay?)
		 (read-lost string port))
		((error? (close-input-port port))
		 (read-lost "Error closing image file" port))
		(else
		 (if reverse?
		     (reverse-byte-order start new-hp))
		 (if (not (= delta 0))
		     (relocate-image delta start new-hp))
		 (set! *hp* new-hp)
		 (adjust *startup-proc* delta)))))))

(define (image-read-block port need)
  (receive (got eof? status)
      (read-block port *hp* need)
    (cond ((error? status)
	   (set! *status* status)
	   (values #f "Error reading from image file"))
	  (eof?
	   (values #f "Premature EOF when reading image file"))
	  ((< got (cells->a-units 1))
	   (values #f "Read returned too few bytes"))
	  (else
	   (values #t "")))))

(define (read-lost message port)
  (error-message message)
  (if (error? *status*)
      (error-message (error-string *status*)))
  (if (error? (close-input-port port))
      (error-message "Error closing image file"))
  -1)

; ABCD => DCBA

; memory intensive, but independent of Scheme's integer size

(define (reverse-descriptor-byte-order! addr)
  (let ((x (fetch-byte addr)))
    (store-byte! addr (fetch-byte (addr+ addr 3)))
    (store-byte! (addr+ addr 3) x))
  (let ((x (fetch-byte (addr+ addr 1))))
    (store-byte! (addr+ addr 1) (fetch-byte (addr+ addr 2)))
    (store-byte! (addr+ addr 2) x)))

(define (reverse-byte-order start end)
  (error-message "Correcting byte order of resumed image.")
  (let loop ((ptr start))
    (reverse-descriptor-byte-order! ptr)
    (let ((value (fetch ptr)))
      (if (addr< ptr end)
	  (loop (if (b-vector-header? value)
		    (addr+ (addr1+ ptr) (header-a-units value))
		    (addr1+ ptr)))))))

(define (adjust descriptor delta)
  (if (stob? descriptor)
      (address->stob-descriptor (addr+ (address-after-header descriptor) delta))
      descriptor))

(define (relocate-image delta start end)
  (let loop ((ptr start))
    (if (addr< ptr end)
	(let ((d (adjust (fetch ptr) delta)))
	  (store! ptr d)
	  (loop (if (b-vector-header? d)
		    (addr+ (addr1+ ptr) (header-a-units d))
		    (addr1+ ptr)))))))

; The page character is used to mark the ends of the user and prelude sections
; of image files.

(define page-character (ascii->char 12))

(define (write-page port)
  (write-char page-character port))

