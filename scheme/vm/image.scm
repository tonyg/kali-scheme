; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Write-image and read-image

(define (s48-image-writing-okay?)
  (not (have-static-areas?)))

(define *status* (enum errors no-errors))

(define-syntax write-check
  (syntax-rules ()
    ((write-check exp)
     (if (eq? *status* (enum errors no-errors))
	 (set! *status* exp)))))

(define (write-heap-integer n port)
  (write-check (write-integer n port))
  (write-check (write-char #\newline port)))

(define (s48-write-image resume-proc port)
  (begin-writing-image)
  (let ((resume-proc (s48-trace-value resume-proc))
	(exported-bindings (s48-trace-value (s48-exported-bindings))))
    (s48-do-gc)
    (s48-mark-traced-channels-closed!)
    (let ((symbols (s48-copy-symbol-table))
	  (imported-bindings (s48-cleaned-imported-bindings))
	  (resumer-records (find-resumer-records)))
      (if (vm-eq? false resumer-records)
	  (begin
	    (abort-collection)
	    (enum errors out-of-memory))
	  (let ((status (really-write-image port resume-proc
					    symbols
					    imported-bindings
					    exported-bindings
					    resumer-records)))
	    (abort-collection)
	    status)))))

(define (really-write-image port restart-proc
			    symbols imported-bindings exported-bindings
			    resumer-records)
  (set! *status* (enum errors no-errors))
  (write-check (write-char #\newline port))
  (write-check (write-page port))
  (write-check (write-char #\newline port))
  (write-check (write-string architecture-version port))
  (write-check (write-char #\newline port))
  (write-heap-integer bytes-per-cell port)
  (write-heap-integer (a-units->cells (address->integer (heap-start))) port)
  (write-heap-integer (a-units->cells (address->integer (heap-pointer))) port)
  (write-heap-integer symbols port)
  (write-heap-integer imported-bindings port)
  (write-heap-integer exported-bindings port)
  (write-heap-integer resumer-records port)
  (write-heap-integer restart-proc port)
  (write-check (write-page port))
  (store! (heap-pointer) 1)  ; used to detect endianess of image
  (write-check (write-block port
			    (heap-pointer)
			    (address-difference (address1+ (heap-pointer)) (heap-pointer))))
  (write-check (write-block port
			    (heap-start)
			    (address-difference (heap-pointer) (heap-start))))
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

; Read in the ASCII portion of the image and make sure that it is compatible
; with this version of the VM.

(define (s48-check-image-header filename)
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
	     (set! *old-begin*
		   (integer->address
		     (cells->a-units (read-check (read-integer port)))))
	     (set! *old-hp*
		   (integer->address
		     (cells->a-units (read-check (read-integer port)))))
	     (set! *symbols*           (read-check (read-integer port)))
	     (set! *imported-bindings* (read-check (read-integer port)))
	     (set! *exported-bindings* (read-check (read-integer port)))
	     (set! *resumer-records*   (read-check (read-integer port)))
	     (set! *startup-procedure* (read-check (read-integer port)))
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
		    (address-difference *old-hp* *old-begin*))))))))

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

(define *image-port*)
(define *old-begin*)
(define *old-hp*)

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

(define (s48-read-image)
  (let ((port *image-port*))
    (receive (okay? string)
	(image-read-block port (cells->a-units 1))
      (cond ((not okay?)
	     (read-lost string port))
	    ((= (fetch (heap-pointer)) 1)
	     (really-read-image port #f))
	    (else
	     (reverse-descriptor-byte-order! (heap-pointer))
	     (if (= (fetch (heap-pointer)) 1)
		 (really-read-image port #t)
		 (read-lost "Unable to correct byte order" port)))))))

(define (really-read-image port reverse?)
  (let* ((delta (address-difference (heap-pointer) *old-begin*))
	 (new-hp (address+ *old-hp* delta))
	 (new-limit (heap-limit))
	 (start (heap-pointer)))
    (if (address>= new-hp new-limit)
	(read-lost "Heap not big enough to restore this image" port)
	(receive (okay? string)
	    (image-read-block port (address-difference *old-hp* *old-begin*))
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
		   (if reverse?
		       (reverse-byte-order! start new-hp))
		   (if (not (= delta 0))
		       (relocate-image delta start new-hp))
		   (set-heap-pointer! new-hp)
		   (set! *startup-procedure* (adjust *startup-procedure* delta))
		   (set! *symbols*           (adjust *symbols* delta))
		   (set! *imported-bindings* (adjust *imported-bindings* delta))
		   (set! *exported-bindings* (adjust *exported-bindings* delta))
		   (set! *resumer-records*   (adjust *resumer-records* delta))
		   0)))))))

(define (image-read-block port need)
  (receive (got eof? status)
      (read-block port (heap-pointer) need)
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

(define (reverse-byte-order! start end)
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

(define (write-page port)
  (write-char page-character port))

