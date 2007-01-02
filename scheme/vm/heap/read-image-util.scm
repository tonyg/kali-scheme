; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Utilities for reading images.

(define (init-read-image!)
  (set! *status* (enum errors no-errors))
  (set! *eof?* #f))

(define *status* (enum errors no-errors))
(define *eof?* #f)

(define (get-status) *status*)
(define (is-eof?) *eof?*)

;; Setters
(define (set-status! status)
  (set! *status* status))

(define-syntax read-check
  (syntax-rules ()
    ((read-check exp)
     (read-check exp -1))
    ((read-check exp losing-value)
     (let ((lose losing-value))
       (if (or (error? (get-status))
	       (is-eof?))
	   lose
	   (receive (thing eof? status)
	       exp
	     (cond (eof?
		    (set! *eof?* #t)
		    lose)
		   ((eq? status (enum errors no-errors))
		    thing)
		   (else
		    (set! *eof?* #t)
		    (if (eq? *status* (enum errors no-errors))
			(set! *status* status))
		    lose))))))))

(define (got-error? expr)
  (not (and (= expr 0)
	    (not (error? *status*)))))

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

;; Reader exception handler

(define (read-lost message port)
  (error-message message)
  (if (error? (get-status))
      (begin 
        (error-message (error-string (get-status)))
        (unspecific)))
  (if (error? (close-input-port port))
      (begin
        (error-message "Error closing image file")
        (unspecific)))
  -1)

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
	  (if (b-vector-header? d)
	      (loop (address+ (address1+ ptr) (header-length-in-a-units d)))
	      (loop (address1+ ptr)))))))

(define (alloc-space size-in-byte)
  (s48-allocate-traced+gc size-in-byte))

; Add DELTA to all hidden pointers.

(define (table-relocator-two-space foo-next set-foo-next!)
  (lambda (table delta)
    (relocate-table table
		    (lambda (address)
		      (+ address delta))
		    foo-next
		    set-foo-next!)))

(define relocate-symbol-table-two-space!
  (table-relocator-two-space vm-symbol-next
			     vm-set-symbol-next!))

(define relocate-binding-table-two-space!
  (table-relocator-two-space shared-binding-next
			     set-shared-binding-next!))

; Variables

;; Common (from image)

(define *startup-procedure* 0) ; 0 is for the type checker
(define *symbols* 0)
(define *imported-bindings* 0)
(define *exported-bindings* 0)
(define *resumer-records* 0)

(define (get-startup-procedure) *startup-procedure*)
(define (get-symbols) *symbols*)
(define (get-imported-bindings) *imported-bindings*)
(define (get-exported-bindings) *exported-bindings*)
(define (get-resumer-records) *resumer-records*)

(define (set-startup-procedure! expr)
  (set! *startup-procedure* expr))

(define (set-symbols! expr)
  (set! *symbols* expr))

(define (set-imported-bindings! expr)
  (set! *imported-bindings* expr))

(define (set-exported-bindings! expr)
  (set! *exported-bindings* expr))

(define (set-resumer-records! expr)
  (set! *resumer-records* expr))

; Two-space format

(define *img-start-addr* null-address)	; image start address
(define *img-end-addr* null-address)	; image end address
(define *img-heap-size* 0)		; heap size from the image

(define (get-img-start-addr) *img-start-addr*)
(define (get-img-end-addr) *img-end-addr*)
(define (get-img-heap-size) *img-heap-size*)

(define (set-img-start-addr! value)
  (set! *img-start-addr* value))

(define (set-img-end-addr! value)
  (set! *img-end-addr* value))

(define (set-img-heap-size! value)
  (set! *img-heap-size* value))

; BIBOP format

(define *small-img-start-addr* null-address) ; small-image start address
(define *small-img-hp-addr* null-address) ; small-image hp address
(define *small-img-end-addr* null-address) ; small-image end address
(define *small-img-heap-size* 0)      ; small-heap size from the image

(define (get-small-img-start-addr) *small-img-start-addr*)
(define (get-small-img-hp-addr) *small-img-hp-addr*)
(define (get-small-img-end-addr) *small-img-end-addr*)
(define (get-small-img-heap-size) *small-img-heap-size*)

(define (set-small-img-start-addr! value)
  (set! *small-img-start-addr* value))

(define (set-small-img-hp-addr! value)
  (set! *small-img-hp-addr* value))

(define (set-small-img-end-addr! value)
  (set! *small-img-end-addr* value))

(define (set-small-img-heap-size! value)
  (set! *small-img-heap-size* value))

(define *large-img-start-addr* null-address) ; lagre-image start address
(define *large-img-hp-addr* null-address) ; large-image hp address
(define *large-img-end-addr* null-address) ; large-image end address
(define *large-img-heap-size* 0)      ; large-heap size from the image

(define (get-large-img-start-addr) *large-img-start-addr*)
(define (get-large-img-hp-addr) *large-img-hp-addr*)
(define (get-large-img-end-addr) *large-img-end-addr*)
(define (get-large-img-heap-size) *large-img-heap-size*)

(define (set-large-img-start-addr! value)
  (set! *large-img-start-addr* value))

(define (set-large-img-hp-addr! value)
  (set! *large-img-hp-addr* value))

(define (set-large-img-end-addr! value)
  (set! *large-img-end-addr* value))

(define (set-large-img-heap-size! value)
  (set! *large-img-heap-size* value))

(define *weaks-img-start-addr* null-address) ; weaks-image start address
(define *weaks-img-hp-addr* null-address) ; weaks-image hp address
(define *weaks-img-end-addr* null-address) ; weaks-image end address
(define *weaks-img-heap-size* 0)      ; weaks-heap size from the image

(define (get-weaks-img-start-addr) *weaks-img-start-addr*)
(define (get-weaks-img-hp-addr) *weaks-img-hp-addr*)
(define (get-weaks-img-end-addr) *weaks-img-end-addr*)
(define (get-weaks-img-heap-size) *weaks-img-heap-size*)

(define (set-weaks-img-start-addr! value)
  (set! *weaks-img-start-addr* value))

(define (set-weaks-img-hp-addr! value)
  (set! *weaks-img-hp-addr* value))

(define (set-weaks-img-end-addr! value)
  (set! *weaks-img-end-addr* value))

(define (set-weaks-img-heap-size! value)
  (set! *weaks-img-heap-size* value))

