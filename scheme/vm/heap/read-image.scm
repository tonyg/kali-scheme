; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Read-image

(define *status* (enum errors no-errors))

(define *eof?* #f)

; Make sure the image file is okay and return the size of the heap it
; contains.

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

; Read in the ASCII portion of the image and make sure that it is compatible
; with this version of the VM.

(define (s48-read-image filename heap-size)
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
		  (old-bytes-per-cell (read-check (read-integer port)))
		  (old-begin
		    (integer->address
		      (cells->a-units (read-check (read-integer port)))))
		  (old-hp
		    (integer->address
		      (cells->a-units (read-check (read-integer port))))))
	     (set! *symbols*           (read-check (read-integer port)))
	     (set! *imported-bindings* (read-check (read-integer port)))
	     (set! *exported-bindings* (read-check (read-integer port)))
	     (set! *resumer-records*   (read-check (read-integer port)))
	     (set! *startup-procedure* (read-check (read-integer port)))
	     (read-check (read-page port)) ; read to beginning of heap
	     (cond ((error? *status*)
		    (read-lost "Error reading from image file" port))
		   (*eof?*   ; has to come after *status* check
		    (set! *status* (enum errors parse-error))
		    (read-lost "Premature EOF when reading image file" port))
		   ((not same-version?)
		    (read-lost "Format of image is incompatible with this version of system"
			       port))
		   ((not (= old-bytes-per-cell bytes-per-cell))
		    (read-lost "Incompatible bytes-per-cell in image" port))
		   (else
		    (let* ((old-heap-size
			     (bytes->cells (address-difference old-hp
							       old-begin)))
			   (new-begin (s48-initialize-heap heap-size
							   old-heap-size
							   old-begin)))
		      (read-old-heap port new-begin old-begin old-hp)))))))))

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

;----------------
; Values provided by the image file.

(define *startup-procedure*)
(define *symbols*)
(define *imported-bindings*)
(define *exported-bindings*)
(define *resumer-records*)

(define *initializing?* #t)

; Save initial values across any GC's.

(define (s48-initializing-gc-root)
  (if *initializing?*
      (begin
	(set! *startup-procedure* (s48-trace-value *startup-procedure*))
	(set! *symbols*           (s48-trace-value *symbols*))
	(set! *imported-bindings* (s48-trace-value *imported-bindings*))
	(set! *exported-bindings* (s48-trace-value *exported-bindings*))
	(set! *resumer-records*   (s48-trace-value *resumer-records*)))))
  
; For the outside world.

(define (s48-startup-procedure)         *startup-procedure*)
(define (s48-initial-symbols)           *symbols*)
(define (s48-initial-imported-bindings) *imported-bindings*)
(define (s48-initial-exported-bindings) *exported-bindings*)
(define (s48-resumer-records)           *resumer-records*)

(define (s48-initialization-complete!)
  (set! *initializing?* #f))

; For resuming static images.

(define (s48-set-image-values! startup-proc symbols imports exports records)
  (set! *startup-procedure* startup-proc)
  (set! *symbols*           symbols)
  (set! *imported-bindings* imports)
  (set! *exported-bindings* exports)
  (set! *resumer-records*   records))

;----------------
; Check the byte order before actually reading the image.

(define (read-old-heap port new-begin old-begin old-hp)
  (receive (okay? string)
      (image-read-block port new-begin (cells->a-units 1))
    (cond ((not okay?)
	   (read-lost string port))
	  ((= (fetch new-begin) false)
	   (really-read-image port new-begin old-begin old-hp #f))
	  (else
	   (reverse-descriptor-byte-order! new-begin)
	   (if (= (fetch new-begin) false)
	       (really-read-image port new-begin old-begin old-hp #t)
	       (read-lost "Unable to correct byte order" port))))))

; We finally read the image and then check that everything is okay.  We
; adjust the byte order and relocate the image if necessary.

(define (really-read-image port new-begin old-begin old-hp reverse?)
  (let* ((delta (address-difference new-begin old-begin))
	 (new-hp (address+ old-hp delta)))
    (receive (okay? string)
	(image-read-block port new-begin (address-difference old-hp old-begin))
      (receive (ch eof? status)
	  (read-char port)
	(cond ((not okay?)
	       (read-lost string port))
	      ((error? status)
	       (read-lost "Error reading from image file" port))
	      ((not eof?)
	       (read-lost "Image file has extraneous data after image" port))
	      ((error? (close-input-port port))
	       (read-lost "Error closing image file" port))
	      (else
	       (set! *startup-procedure* (adjust *startup-procedure* delta))
	       (set! *symbols*           (adjust *symbols* delta))
	       (set! *imported-bindings* (adjust *imported-bindings* delta))
	       (set! *exported-bindings* (adjust *exported-bindings* delta))
	       (set! *resumer-records*   (adjust *resumer-records* delta))
	       (if reverse?
		   (s48-reverse-byte-order! new-begin new-hp))
	       (if (not (= delta 0))
		   (begin
		     (relocate-symbol-table! *symbols* delta)
		     (relocate-binding-table! *imported-bindings* delta)
		     (relocate-binding-table! *exported-bindings* delta)
		     (relocate-image delta new-begin new-hp)))
	       0))))))

(define (s48-relocate-all delta new-begin new-hp symbols imported exported)
  (relocate-symbol-table! symbols delta)
  (relocate-binding-table! imported delta)
  (relocate-binding-table! exported delta)
  (relocate-image delta new-begin new-hp))

(define relocate-symbol-table!
  (table-relocator vm-symbol-next
		   vm-set-symbol-next!))

(define relocate-binding-table!
  (table-relocator shared-binding-next
		   set-shared-binding-next!))

;----------------

(define (image-read-block port address need)
  (receive (got eof? status)
      (read-block port address need)
    (cond ((error? status)
	   (set! *status* status)
	   (values #f "Error reading from image file"))
	  (eof?
	   (values #f "Premature EOF when reading image file"))
	  ((< got need)
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
    (store-byte! addr (fetch-byte (address+ addr 3)))
    (store-byte! (address+ addr 3) x))
  (let ((x (fetch-byte (address+ addr 1))))
    (store-byte! (address+ addr 1) (fetch-byte (address+ addr 2)))
    (store-byte! (address+ addr 2) x)))

(define (s48-reverse-byte-order! start end)
  (error-message "Correcting byte order of resumed image.")
  (let loop ((ptr start))
    (if (address< ptr end)
	(begin
	  (reverse-descriptor-byte-order! ptr)
	  (loop (let ((value (fetch ptr))
		      (next (address1+ ptr)))
		  (if (b-vector-header? value)
		      (address+ next (header-length-in-a-units value))
		      next)))))))

(define (adjust descriptor delta)
  (if (stob? descriptor)
      (address->stob-descriptor
        (address+ (address-after-header descriptor) delta))
      descriptor))

(define (relocate-image delta start end)
  (let loop ((ptr start))
    (if (address< ptr end)
	(let ((d (adjust (fetch ptr) delta)))
	  (store! ptr d)
	  (loop (if (b-vector-header? d)
		    (address+ (address1+ ptr) (header-length-in-a-units d))
		    (address1+ ptr)))))))

; The page character is used to mark the ends of the user and prelude sections
; of image files.

(define page-character (ascii->char 12))

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

