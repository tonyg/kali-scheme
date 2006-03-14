; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Read-image

(define (s48-read-image image-filename max-heap-size)
  (init-read-image!)
  (receive (port status)
      (open-input-file image-filename)
    (if (error? status)
	(begin
	  (error-message  "Can't open heap image file")
	  (error-message (error-string status))
	  -1)
	(receive (status format)
	    (check-image-header port)
	  (cond
	   ;; Read in the ASCII portion of the image and make
	   ;; sure that it is compatible with this version of
	   ;; the VM.
	   ((got-error? status) -1)
		    
	   ;; now we can initialize the heap
	   ((got-error? (initialize-heap! max-heap-size)) -1)
		    
	   ;; allocate space and copy the objects (image =>  heap)
	   ((got-error? (read-image-to-heap format port)) -1)
	   (else 0))))))		; Everything OK

; reads image objects into the heap object
(define (read-image-to-heap image-format port)
  (really-read-image image-format (check-byte-order port) port))

(define (check-byte-order port)
  (let ((new-addr (allocate-memory (cells->bytes 1))))
    (receive (okay? msg)
	(image-read-block port new-addr (cells->a-units 1))
      (if (not okay?)
          (error "byte order check failed"))
      (cond ((= (fetch new-addr) false)
             (deallocate-memory new-addr)
             #f)
            (else 
             (reverse-descriptor-byte-order! new-addr)
             (if (not (= (fetch new-addr) false))
		 (error "Unable to correct byte order"))
             (deallocate-memory new-addr)
             #t)))))

(define (check-image-header port)

  ;; Skip page-char (start of the image-header)
  (read-check (read-page port))

  ;; Skip newline-char (version starts on next line)
  (read-check (read-newline port))

  ;; Read version, bytes/cell and co.
  (let* ((same-version? (read-check (check-image-version port) #f))
	 (format (read-check (check-image-format port) -1))
	 (old-bytes-per-cell (read-check (read-integer port))))

    ;; Is everything OK?
    (values (cond ((error? (get-status))
		   (read-lost "Error reading from image file" port))
		  ((is-eof?)	    ; has to come after *status* check
		   (set-status! (enum errors parse-error))
		   (read-lost "Premature EOF when reading image file" port))
		  ((not same-version?)
		   (read-lost "Format of image is incompatible with this version of system"
			      port))
		  ((not (valid-image-format? format))
		   (read-lost "Unknown image format" port))
		  ((not (= old-bytes-per-cell bytes-per-cell))
		   (read-lost "Incompatible bytes-per-cell in image" port))
		  (else
		   (check-all-data! format port)
		   (set-symbols!           (read-check (read-integer port)))
		   (set-imported-bindings! (read-check (read-integer port)))
		   (set-exported-bindings! (read-check (read-integer port)))
		   (set-resumer-records!   (read-check (read-integer port)))
		   (set-startup-procedure! (read-check (read-integer port)))
	  
		   ;; Skip page-char (end of the image-header = begin of image-heap)
		   (read-check (read-page port))
		   0))
	    format)))

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

(define (check-image-format port)
  (receive (n eof? status)
      (read-integer port)
    (values n eof? status)))

(define (check-all-data! format port)
  (enum-case image-format format
   ((two-space)
    (set-img-start-addr!
     (integer->address
      (cells->a-units (read-check (read-integer port)))))
    (set-img-end-addr!
     (integer->address
      (cells->a-units (read-check (read-integer port)))))
    (set-img-heap-size!
     (bytes->cells (address-difference (get-img-end-addr)
				       (get-img-start-addr)))))
   ((bibop)
    ;; Read all to calculate the real size
    (let* ((sb (cells->a-units (read-check (read-integer port))))
	   (sh (cells->a-units (read-check (read-integer port))))
	   (se (cells->a-units (read-check (read-integer port))))
	   (lb se)
	   (lh (cells->a-units (read-check (read-integer port))))
	   (le (cells->a-units (read-check (read-integer port))))
	   (wb le)
	   (wh (cells->a-units (read-check (read-integer port))))
	   (we (cells->a-units (read-check (read-integer port)))))
	   
      (set-small-img-start-addr! (integer->address sb))
      (set-small-img-hp-addr!    (integer->address sh))
      (set-small-img-end-addr!   (integer->address se))
	   
      (set-large-img-start-addr! (integer->address lb))
      (set-large-img-hp-addr!    (integer->address lh))
      (set-large-img-end-addr!   (integer->address le))
	   
      (set-weaks-img-start-addr! (integer->address wb))
      (set-weaks-img-hp-addr!    (integer->address wh))
      (set-weaks-img-end-addr!   (integer->address we))
	   
      ;; These are going to be read from the image (port)
      (set-small-img-heap-size! (a-units->cells (- sh sb)))
      (set-large-img-heap-size! (a-units->cells (- lh lb)))
      (set-weaks-img-heap-size! (a-units->cells (- wh wb)))
	   
      ;; This image is WLS
      (set-img-start-addr! (integer->address wb))
      (set-img-end-addr! (integer->address sh))
	   
      ;; This is going to be allocated (so we need the
      ;; real whole size to have 1:1 addresses)
      (set-img-heap-size! (+ (a-units->cells (- se sb))
			     (a-units->cells (- le lb))
			     (a-units->cells (- we wb))))))
   (else (error "check-all-data!: Unknown image format (this can't happen)"))))

(define (initialize-heap! max-heap-size)
  (s48-initialize-heap max-heap-size
		       (get-img-start-addr) (get-img-heap-size))

  (initialize-image-areas!)
  (s48-check-heap-size!)
  0)					; for the type checker

;----------------
; Save initial values across any GC's.

(define *initializing?* #t)

(define (s48-initializing-gc-root)
  (if *initializing?*
      (begin
	(set-startup-procedure! (s48-trace-value (get-startup-procedure)))
	(set-symbols!           (s48-trace-value (get-symbols)))
	(set-imported-bindings! (s48-trace-value (get-imported-bindings)))
	(set-exported-bindings! (s48-trace-value (get-exported-bindings)))
	(set-resumer-records!   (s48-trace-value (get-resumer-records))))))
  
; For the outside world.

(define (s48-startup-procedure)         (get-startup-procedure))
(define (s48-initial-symbols)           (get-symbols))
(define (s48-initial-imported-bindings) (get-imported-bindings))
(define (s48-initial-exported-bindings) (get-exported-bindings))
(define (s48-resumer-records)           (get-resumer-records))

(define (s48-initialization-complete!)
  (set! *initializing?* #f))

; For resuming static images.

(define (s48-set-image-values! startup-proc symbols imports exports records)
  (set-startup-procedure! startup-proc)
  (set-symbols!           symbols)
  (set-imported-bindings! imports)
  (set-exported-bindings! exports)
  (set-resumer-records!   records))

; #### needs to be generalized
; (define (s48-relocate-all delta new-begin new-hp symbols imported exported)
;   (relocate-symbol-table-two-space! symbols delta)
;   (relocate-binding-table-two-space! imported delta)
;   (relocate-binding-table-two-space! exported delta)
;   (relocate-image delta new-begin new-hp))

; for debugging
(define (get-all-globals)
  (write-out-string "(get-status): ")
  (if (eq? (get-status) 
	   (enum errors no-errors))
      (write-out-string "no-errors ")
      (write-out-string "errors "))
  (write-out-newline)
  
  (write-out-string "(is-eof?): ")
  (if (is-eof?)
      (write-out-string "EOF ")
      (write-out-string "NOT EOF "))
  (write-out-newline)

  (write-out-string "null-address: ")
  (write-out-integer (address->integer null-address))
  (write-out-newline)

  (write-out-string "(get-img-start-addr): ")
  (write-out-integer (address->integer (get-img-start-addr)))
  (write-out-newline)

  (write-out-string "(get-img-end-addr): ")
  (write-out-integer (address->integer (get-img-end-addr)))
  (write-out-newline)

  (write-out-string "(get-img-heap-size): ")
  (write-out-integer (get-img-heap-size))
  (write-out-newline)

  (write-out-string "(get-startup-procedure): ")
  (write-out-integer (get-startup-procedure))
  (write-out-newline)

  (write-out-string "(get-symbols): ")
  (write-out-integer (get-symbols))
  (write-out-newline)

  (write-out-string "(get-imported-bindings): ")
  (write-out-integer (get-imported-bindings))
  (write-out-newline)

  (write-out-string "(get-exported-bindings): ")
  (write-out-integer (get-exported-bindings))
  (write-out-newline)

  (write-out-string "(get-resumer-records): ")
  (write-out-integer (get-resumer-records))
  (write-out-newline)
  )

