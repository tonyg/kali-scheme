; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Two-space-GC-specific part of the dumper

(define image-start-address 0)

(define *image-begin*)		; Starting address.
(define *image-hp*)		; Current ending address.

(define (write-header resumer-records resume-proc image-descriptor port)
  (write-check (write-char #\newline port))
  (write-page)
  (write-check (write-char #\newline port))
  (write-check (write-string architecture-version port))
  (write-check (write-char #\newline port))
  (write-check (write-integer (enum image-format two-space) port))
  (write-check (write-char #\newline port))
  (write-header-integer bytes-per-cell)

  ;;Start Address
  (write-header-integer (a-units->cells image-start-address))
  ;;End Address
  (write-header-integer (a-units->cells (+ image-start-address (image-size))))

  (write-header-integer (image-descriptor (s48-symbol-table)))
  (write-header-integer (image-descriptor (s48-imported-bindings)))
  (write-header-integer (image-descriptor (s48-exported-bindings)))
  (write-header-integer resumer-records)
  (write-header-integer resume-proc)
  (write-page))

(define (image-size)
  (address-difference *image-hp* *image-begin*))

(define (begin-making-image/gc-specific)
;  (let ((cand-addr (get-candidate-address)))
;    (if (not (null-address? cand-addr))
;	(set! image-start-address (address->integer cand-addr))))
  ;; There seems to be no problem with a possible null-address - David
  (set! image-start-address (address->integer (get-candidate-address)))

  (set! *image-begin* (integer->address image-start-address))
  (set! *image-hp*    (integer->address image-start-address)))

(define (allocate-new-image-object stob)
  (let* ((new-descriptor (image-alloc (enum area-type-size small)
				      (header-length-in-a-units (stob-header stob)))))
    (values new-descriptor
	    (make-image-location new-descriptor))))

(define (finalize-new-image-object stob)
  (unspecific))

(define (get-candidate-address)
  (get-new-heap-start-addr))

(define (image-alloc area-type-size length-in-a-units)
  (let ((data-addr (address+ *image-hp* (cells->a-units stob-overhead))))
    (set! *image-hp* (address+ data-addr length-in-a-units))
    (address->stob-descriptor data-addr)))

(define (write-image-areas first-stob stob-table write-stob)
  (let loop ((stob first-stob))
    (if (stob? stob)
	(let ((location (table-ref stob-table stob)))
	  (if (null-pointer? location)
	      (error "traced stob has no image-table entry"))
	  (write-stob stob)
	  (loop (image-location-next location))))))

(define (deallocate-areas)
  (unspecific))

(define (adjust-descriptors! stob-table)
  (unspecific))

(define (note-traced-last-stob!)
  (unspecific))
