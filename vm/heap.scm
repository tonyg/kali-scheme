; Allocation
;  *hp* is the heap pointer and *limit* is the limit beyond which no
;  storage should be allocated.  Both of these are addresses (not
;  descriptors).

(define check-preallocation? #f)

(define *hp* 0)
(define *limit* 0)

; These are all in address units
(define *newspace-begin* (unassigned))
(define *newspace-end*   (unassigned))
(define *oldspace-begin* (unassigned))
(define *oldspace-end*   (unassigned))

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

(define (make-stob type len key)	;len is in bytes
  (if check-preallocation?
      (let ((cells (+ (bytes->cells len) 1)))
	(assert (available? cells))
	(cond ((not (= key universal-key))
	       (if (not (and (= key *heap-key*)
			     (>= *okayed-space* cells)))
		   (error "invalid heap key" key cells))
	       (set! *okayed-space* (- *okayed-space* cells))))))
  (store! *hp* (make-header type len))
  (set! *hp* (addr1+ *hp*))
  (let ((new (address->stob-descriptor *hp*)))
    (set! *hp* (addr+ *hp* (bytes->a-units len)))
    (if (> len 0)	 ; for B-VECTORs that don't want to use all of the
        (store! (addr+ *hp* (cells->bytes -1)) 0))       ; last descriptor
    new))


(define *pure-areas*   (unassigned))
(define *impure-areas* (unassigned))
(define *pure-sizes*   (unassigned))
(define *impure-sizes* (unassigned))
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



; Used to find end of an object
(define (header-a-units h)
  (bytes->a-units (header-length-in-bytes h)))

(define (walk-over-type-in-area type proc)
  (lambda (start end)
    (let loop ((addr start))
      (cond ((addr< addr end)
	     (let ((d (fetch addr)))
	       (cond ((not (header? d))
		      (write-string "heap is in an inconsistent state."
				    (current-output-port))
		      #f)
		     ((or (not (= type (header-type d)))
			  (proc (address->stob-descriptor (addr1+ addr))))
		      (loop (addr1+ (addr+ addr (header-a-units d)))))
		     (else
		      #f))))
	    (else
	     #t)))))

(define (walk-over-symbols proc)
  (let ((proc (walk-over-type-in-area (enum stob symbol) proc))
	(start-hp *hp*))
    (cond ((and (proc *newspace-begin* *hp*)
		(walk-pure-areas proc))
	   #t)
	  (else
	   (set! *hp* start-hp) ; out of space, so undo and give up
	   #f))))

(define (find-all-xs type)
  (let ((proc (walk-over-type-in-area type maybe-push-obj))
	(start-hp *hp*))
    (store-next! 0)                              ; reserve space for header
    (cond ((and (proc *newspace-begin* start-hp)
		(walk-impure-areas proc)
		(walk-pure-areas proc))
	   (let ((size (addr- *hp* (addr1+ start-hp))))
	     (store! start-hp (make-header (enum stob vector) size) )
	     (address->stob-descriptor (addr1+ start-hp))))
	  (else
	   (set! *hp* start-hp) ; out of space, so undo and give up
	   false))))

(define (maybe-push-obj thing)
  (cond ((available? (cells->a-units 1))
	 (store-next! thing)
	 #t)
	(else #f)))


;;;; Write-image and read-image

(define level 15)

(define (image-writing-okay?)
  (and (= 0 *pure-area-count*)
       (= 0 *impure-area-count*)))

(define (write-image port restart-proc)
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

; Make sure the image file is okay and return the size of the heap it
; contains.

(define (check-image-header filename)
  (call-with-input-file filename
    (lambda (port)
      (let ((lose (lambda (message)
		    (let ((out (current-output-port)))
		      (write-string message out)
		      (newline out)
		      -1))))
	(cond ((null-port? port)
	       (lose "Can't open heap image file"))
	      (else
	       (read-page port) ; read past any user cruft at the beginning of the file
	       (let* ((old-level          (read-number port))
		      (old-bytes-per-cell (read-number port))
		      (old-begin (cells->a-units (read-number port)))
		      (old-hp    (cells->a-units (read-number port))))
		 (cond ((not (= old-level level))
			(lose "format of image is incompatible with this version of system"))
		       ((not (= old-bytes-per-cell bytes-per-cell))
			(lose "incompatible bytes-per-cell in image"))
		       (else
			(- old-hp old-begin))))))))))

(define (read-image filename startup-space)
  (call-with-input-file filename
    (lambda (port)
      (if (null-port? port)
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
        (let* ((delta (- *hp* old-begin))
               (new-hp (+ old-hp delta))
               (new-limit *newspace-end*)
	       (start *hp*))
          (if (addr>= (+ startup-space new-hp) new-limit)
	      (error "heap not big enough to restore this image"
		     new-hp new-limit))
	  (let ((reverse? (check-image-byte-order port)))
	    (read-bytes *hp* (- old-hp old-begin) port)
	    (if reverse?
		(reverse-byte-order start new-hp))
	    (if (not (= delta 0))
		(relocate-image delta start new-hp))
	    (set! *hp* new-hp)
	    (adjust startup-proc delta)))))))

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

(define (reverse-byte-order start end)
  (write-string "Correcting byte order of resumed image."
		 (current-output-port))
  (newline (current-output-port))
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

(define (read-page port)
  (let loop ()
    (ps-read-char port
		  (lambda (ch)
		    (if (not (char=? ch page-character))
			(loop)))
		  (lambda ()
		    (error "end of file while looking for page break")))))

