; Utilities for writing images.

; The page character is used to mark the ends of the user and prelude sections
; of image files.

(define *status* (enum errors no-errors))

(define (image-write-status)
  *status*)

(define-syntax write-check
  (syntax-rules ()
    ((write-check exp)
     (if (eq? *status* (enum errors no-errors))
	 (set! *status* exp)))))

(define image-buffer-size 4096)
(define *image-port*)
(define *image-buffer*)
(define *image-buffer-pointer*)

(define (write-header-integer n)
  (write-check (write-integer n *image-port*))
  (write-check (write-char #\newline *image-port*)))

(define page-character (ascii->char 12))

(define (write-page)
  (write-check (write-char page-character *image-port*)))

(define (buffer-contents)
  (address-difference *image-buffer-pointer*
		      *image-buffer*))

(define (image-write-init port)
  (set! *image-port* port)
  (set! *image-buffer* (allocate-memory image-buffer-size))
  (set! *image-buffer-pointer* *image-buffer*)
  (set! *status* (enum errors no-errors))
  (not (null-address? *image-buffer*)))

(define (image-write-terminate)
  (deallocate-memory *image-buffer*))

; Writing buffers.

(define (write-descriptor descriptor)
  (word-set! *image-buffer-pointer* descriptor)
  (set! *image-buffer-pointer* (address1+ *image-buffer-pointer*))
  (maybe-empty-image-buffer))

(define (write-image-block start size)
  (let ((available (- image-buffer-size (buffer-contents)))) 
    (cond ((<= size available)
	   (copy-image-data start size))
	  ((<= size image-buffer-size)
	   (copy-image-data start available)
	   (copy-image-data (address+ start available) (- size available)))
	  (else
	   (empty-image-buffer!)
	   (write-check (write-block *image-port* start size))))
    (unspecific)))

(define (copy-image-data start size)
  (copy-memory! start *image-buffer-pointer* size)
  (set! *image-buffer-pointer* (address+ *image-buffer-pointer* size))
  (maybe-empty-image-buffer))

(define (maybe-empty-image-buffer)
  (if (= (buffer-contents)
	 image-buffer-size)
      (empty-image-buffer!)))

(define (empty-image-buffer!)
  (let ((have (buffer-contents)))
    (cond ((< 0 have)
	   (write-check (write-block *image-port* *image-buffer* have))
	   (set! *image-buffer-pointer* *image-buffer*)))))


