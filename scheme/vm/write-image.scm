; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Write-image

(define *status* (enum errors no-errors))

(define-syntax write-check
  (syntax-rules ()
    ((write-check exp)
     (if (eq? *status* (enum errors no-errors))
	 (set! *status* exp)))))

(define (write-heap-integer n port)
  (write-check (write-integer n port))
  (write-check (write-char #\newline port)))

(define (s48-write-image resume-proc undumpables port)
  (begin-making-image undumpables)
  (let ((resume-proc (trace-image-value resume-proc))
	(exported-bindings (copy-exported-bindings (s48-exported-bindings))))
    (make-image)
    (mark-image-channels-closed! (s48-channels) (s48-channel-count))
    (let ((symbols (make-image-symbol-table (s48-symbol-table)))
	  (imported-bindings (clean-imported-bindings (s48-imported-bindings)))
	  (resumer-records (find-resumer-records)))
      (if (false? resumer-records)
	  (begin
	    (restore-heap)
	    (enum errors out-of-memory))
	  (let ((status (really-write-image port
					    resume-proc
					    symbols
					    imported-bindings
					    exported-bindings
					    resumer-records)))
	    (restore-heap)
	    status)))))

(define (really-write-image port
			    restart-proc
			    symbols imported-bindings exported-bindings
			    resumer-records)
  (set! *status* (enum errors no-errors))
  (write-check (write-char #\newline port))
  (write-check (write-page port))
  (write-check (write-char #\newline port))
  (write-check (write-string architecture-version port))
  (write-check (write-char #\newline port))
  (write-heap-integer bytes-per-cell port)
  (write-heap-integer (a-units->cells (address->integer (image-start))) port)
  (write-heap-integer (a-units->cells (address->integer (image-pointer))) port)
  (write-heap-integer symbols port)
  (write-heap-integer imported-bindings port)
  (write-heap-integer exported-bindings port)
  (write-heap-integer resumer-records port)
  (write-heap-integer restart-proc port)
  (write-check (write-page port))
  (store! (image-pointer) 1)  ; used to detect endianess of image
  (write-check (write-block port
			    (image-pointer)
			    (address-difference (address1+ (image-pointer))
						(image-pointer))))
  (write-check (write-block port
			    (image-start)
			    (address-difference (image-pointer)
						(image-start))))
  *status*)

; The page character is used to mark the ends of the user and prelude sections
; of image files.

(define page-character (ascii->char 12))

(define (write-page port)
  (write-char page-character port))

