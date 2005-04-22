; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; Image reader that works for both GCs

;; Local Values of the temporary heap-image-object (to read in the image)
(define *heap-image-pointer*)  ;; pointer to the image-object
(define *symbol-address*)
(define *heap-object-remaining-cells*)
(define *heap-object-pointer*)
(define *stob-table*)          ;; Table for mapping the heap <-> image relation

 
; local initializations to read the image
(define (init-locals!)
  (set! *heap-image-pointer* null-address)
  (set! *symbol-address* null-address) ;; Once for the type-checker
  (set! *symbol-address* (if (= false (get-symbols))
			     (get-img-end-addr)
			     (address-at-header (get-symbols))))
  (set! *heap-object-remaining-cells* 0)
  (set! *heap-object-pointer* null-address)
  (set! *stob-table* (make-table)))

; Using a heap-object and a  *stob-table*
(define (really-read-image-portable image-format reverse-byte-order? port)
  (init-locals!)
  (cond
   ((got-error? (image->heap image-format port))
    -1)
   (else
    (if reverse-byte-order?
	(reverse-image-byte-order! image-format))
    (allocate+parse+copy-objects! image-format)
    (read-tables image-format port)
    (deallocate-things)
    0)))

(define (reverse-image-byte-order! image-format)
  (reverse-byte-order! (old->new-addr (get-img-start-addr) image-format)
		       (old->new-addr (get-img-end-addr) image-format)))

(define (read-tables image-format port)
  (set-startup-procedure! 
   (address->stob-descriptor
    (address+
     (address-table-ref (address-at-header (get-startup-procedure)))
     (cells->a-units stob-overhead))))
  (if (not (= false (get-symbols)))
      (begin
	(set-symbols! (allocate-table (get-symbols) image-format))
	(relocate-symbol-table-bibop! (get-symbols) *stob-table*)))
  (if (not (= false (get-imported-bindings)))
      (begin
	(set-imported-bindings! (allocate-table (get-imported-bindings) image-format))
	(relocate-binding-table-bibop! (get-imported-bindings) *stob-table*)))
  (if (not (= false (get-exported-bindings)))
      (begin
	(set-exported-bindings! (allocate-table (get-exported-bindings) image-format))
	(relocate-binding-table-bibop! (get-exported-bindings) *stob-table*)))
  (if (not (= false (get-resumer-records)))
      (begin
	(set-resumer-records! (allocate-table (get-resumer-records) image-format))
	(relocate-resumer-record! (get-resumer-records))))
  (unspecific))


(define (deallocate-things)
  (deallocate-table *stob-table*)
  (deallocate-memory *heap-image-pointer*)
  0)



; address table abstraction
(define (address->non-zero-integer addr)
  (if (= 0 (address->integer addr))
      -1
      (address->integer addr)))

(define (address-table-ref address)
  (integer->address
   (image-location-new-descriptor
    (table-ref *stob-table* (address->non-zero-integer address)))))

(define (address-table-set! old-address new-address)
  (let ((val (make-image-location (address->integer new-address))))
    (table-set! *stob-table* (address->non-zero-integer old-address) val)))


; creates a new object of a given size in the heap, writes a 
; new entry into *STOB-TABLE*, and returns a pointer to the object.
(define (alloc-object current-address size-in-bytes)
  (let ((new-address (alloc-space size-in-bytes)))
    (address-table-set! current-address new-address)
    new-address))

(define (allocate-table tab image-format)
  (let* ((addr (old->new-addr (address-at-header tab) image-format))
	 (cell (fetch addr)))
    (if (header? cell)
	(let* ((size (header-length-in-cells cell))
	       (pointer (alloc-space 
			 (cells->bytes 
			  (+ size stob-overhead)))))
	  (copy-memory! addr pointer 
			(cells->bytes
			 (+ size stob-overhead)))
	  (address->stob-descriptor
	   (address+ pointer (cells->a-units stob-overhead))))
	(error "read-tables! no header"))))

(define (relocate-resumer-record! resumer-records)
  (let ((cell (fetch (address-at-header resumer-records))))
    (if (header? cell)
	(let loop ((address (address-after-header resumer-records))
		   (size (header-length-in-cells cell)))
	  (if (= size 0)
	      (unspecific)
	      (let ((cell (fetch address)))
		(if (stob? cell)
		    (let ((new-addr (address-table-ref (address-at-header cell))))
		      (store! address
			      (address->stob-descriptor
			       (address+ new-addr 
					 (cells->a-units stob-overhead))))
		      (loop (address+ address (cells->a-units 1))
			    (- size 1)))
		    (error "Could this happen?")))))
	(error "relocate-resumer-record! - no header"))))

; does the key *STOB-TABLE* have an associated value?
(define (not-allocated? current-addr)
  (null-pointer? (table-ref *stob-table* 
			    (address->non-zero-integer current-addr))))


; writes a b-vector
(define (write-b-vector current-addr size-in-cells image-format)
  (let ((new-address (alloc-object current-addr (cells->bytes size-in-cells))))
    (copy-memory! (old->new-addr current-addr image-format) new-address 
		  (cells->bytes size-in-cells))))


; writes a header for a new non-b-vector object
; The current heap object address is stored into *heap-object-pointer*
(define (write-new-header current-addr size-in-cells cell) 
  (let ((new-address (alloc-object current-addr (cells->bytes size-in-cells))))
    (if (not (header? cell))
	(error "cell was not a header"))
    (store! new-address cell)
    (set! *heap-object-pointer* new-address)
    (set! *heap-object-remaining-cells* (+ stob-overhead (header-length-in-cells cell)))
    (obj-address+)))


; writes a header for an existing non-b-vector object
(define (write-header current-address cell)
  (let* ((new-address (address-table-ref current-address)))
    (if (not (header? cell))
	(error "cell was not a header"))
    (store! new-address cell)
    (set! *heap-object-pointer* new-address)
    (set! *heap-object-remaining-cells* (+ stob-overhead (header-length-in-cells cell)))
    (obj-address+)))
	

; This gets used when, going through the image, we hit a stob descriptor
; whose stob isn't in *STOB-TABLE* yet.
; For a b-vector, we write the complete object, for all others, we
; generate a dummy object.
(define (write-pointed-object stob-cell image-format)
  (let* ((current-addr (address-at-header stob-cell))
	 (header-cell (fetch (old->new-addr current-addr image-format)))
	 (size-in-cells (+ stob-overhead (header-length-in-cells header-cell)))
	 (size-in-bytes (cells->bytes size-in-cells)))
    (if (b-vector-header? header-cell)
	(write-b-vector current-addr size-in-cells image-format)
	(let ((new-address (alloc-object current-addr size-in-bytes)))
	  (store! new-address (make-header (enum stob byte-vector)
					   (- size-in-bytes
					      (cells->bytes stob-overhead))))
	  (let ((new-end (address+ new-address (bytes->a-units size-in-bytes))))
	    (let lp ((index (address+ new-address (cells->a-units stob-overhead))))
	      (if (not (address= index new-end))
		  (begin (store! index 0)
			 (lp (address1+ index))))))))))


; Writes a pointer to an existing object into the object currently being worked on.
; increments *HEAP-OBJECT-POINTER*
(define (write-fixed-stob stob current-addr)
  (let* ((new-address (address-table-ref (address-at-header stob)))
	 (fixed-stob (address->stob-descriptor
		      (address+ new-address (cells->a-units stob-overhead )))))
    (store! *heap-object-pointer* fixed-stob)
    (obj-address+)))

(define (allocate+parse+copy-objects! format)
  (enum-case image-format format
    ((two-space)
     (parse-reachable-objects (get-img-start-addr) *symbol-address* format))
    ((bibop)
	 (parse-reachable-objects (get-small-img-start-addr) *symbol-address*
				  format)
	 (parse-reachable-objects (get-large-img-start-addr)
				  (address+ (get-large-img-start-addr)
					    (cells->a-units (get-large-img-heap-size)))
				  format)
	 (parse-reachable-objects (get-weaks-img-start-addr)
				  (address+ (get-weaks-img-start-addr)
					    (cells->a-units (get-weaks-img-heap-size)))
				  format))
    (else (error "allocate+parse+copy-objects!: Unknown image format"))))

(define (parse-reachable-objects from-addr to-addr image-format)
  (let loop ((current-addr from-addr))    
    (if (address= current-addr to-addr)
	0
	(begin
	  (let* ((cell (fetch (old->new-addr current-addr image-format))))
	    (if (header? cell)
		(let ((size-in-cells (header-length-in-cells cell)))
		  (if (not (= 0 *heap-object-remaining-cells*))
		      (error "Encountered an header within an d-vector."))
		  (if (b-vector-header? cell)
		      (begin
			(if (not-allocated? current-addr)
			    (write-b-vector current-addr 
					    (+ stob-overhead size-in-cells)
					    image-format))
			(loop (address+ current-addr 
					(cells->a-units 
					 (+ stob-overhead size-in-cells)))))
		      (begin
			(if (not-allocated? current-addr)
			    (write-new-header current-addr 
					      (+ 1 size-in-cells) cell)
			    (write-header current-addr cell))
			(loop (address+ current-addr (cells->a-units 1))))))
		;; content of the object:
		(begin
		  (if (stob? cell)
		      (begin
			(if (not-allocated? (address-at-header cell))
			    (write-pointed-object cell image-format))
			(write-fixed-stob cell current-addr))
		      (begin
			(store! *heap-object-pointer* cell)
			(obj-address+)))
		  (loop (address+ current-addr (cells->a-units 1))))))))))

;; address-conversion between heap and image-address:
;; type checker doesn't allow these variables as locals with let :-(
(define *offset* 0)
(define *area-start*)

(define (old->new-addr addr format)
  (enum-case image-format format
    ((two-space)
     (address+ *heap-image-pointer* 
	       (address-difference addr (get-img-start-addr))))
    ((bibop)
     (cond ((and (address>= addr (get-small-img-start-addr))
		 (address<= addr (get-small-img-hp-addr)))
	    (set! *offset*
		  (+ (address-difference (get-weaks-img-end-addr)
					 (get-weaks-img-start-addr))
		     (address-difference (get-large-img-end-addr)
					 (get-large-img-start-addr))))
	    (set! *area-start* (get-small-img-start-addr)))
	   ((and (address>= addr (get-large-img-start-addr))
		 (address<= addr (get-large-img-hp-addr)))
	    (set! *offset*
		  (address-difference (get-weaks-img-end-addr)
				      (get-weaks-img-start-addr)))
	    (set! *area-start* (get-large-img-start-addr)))
	   ((and (address>= addr (get-weaks-img-start-addr))
		 (address<= addr (get-weaks-img-hp-addr)))
	    (set! *offset* 0)
	    (set! *area-start* (get-weaks-img-start-addr)))
	   (else (error "Unknown address area!")))
     (address+ *heap-image-pointer* 
	       (+ *offset*
		  (address-difference addr *area-start*))))
    (else (error "old->new-addr: Unknown image format"))))

(define (obj-address+)
  (set! *heap-object-remaining-cells* (- *heap-object-remaining-cells* 1))
  (set! *heap-object-pointer* 
	(address+ *heap-object-pointer* (cells->a-units 1))))

; saves all reachable objects from the image file in a heap object
(define (image->heap format port)
  (set! *heap-image-pointer* 
	(allocate-memory (cells->a-units (get-img-heap-size))))
  
  (cond
   ((got-error?
     (enum-case image-format format
       ((two-space) ; with BIBOP GC
	(receive (okay? string)
	    (image-read-block port *heap-image-pointer* 
			      (cells->a-units (get-img-heap-size)))
	  (if okay?
	      0
	      (read-lost string port))))
       ((bibop)				; with two-space copier
	(cond
	 ((got-error? (really-read-weaks-image port))
	  -1)
	 ((got-error? (really-read-large-image port))
	  -1)
	 ((got-error? (really-read-small-image port))
	  -1)
	 (else
	  0)))
       (else
	(read-lost "this can't happen: invalid image format" port))))
    -1)
   (else
    (receive (ch eof? status)
	(read-char port)
      (cond
       ((error? status)
	(read-lost "Error reading from image file" port))
       ((not eof?)
	;; debugging
	(read-lost "Image file has extraneous data after image" port))
       ((error? (close-input-port port))
	(read-lost "Error closing image file" port))
       (else
	0))))))

(define (table-relocator-bibop foo-next set-foo-next!)
  (lambda (table stob-table)

    (define (address->non-zero-integer addr)
      (if (= 0 (address->integer addr))
	  -1
	  (address->integer addr)))

    (define (address-table-ref address)
      (integer->address
       (image-location-new-descriptor
	(table-ref stob-table (address->non-zero-integer address)))))

    (relocate-table table
		    (lambda (address)
		      (address->stob-descriptor
		       (address+ (address-table-ref (address-at-header address))
				 (cells->a-units stob-overhead))))
		    foo-next
		    set-foo-next!)))

(define relocate-symbol-table-bibop!
  (table-relocator-bibop vm-symbol-next
			 vm-set-symbol-next!))

(define relocate-binding-table-bibop!
  (table-relocator-bibop shared-binding-next
			 set-shared-binding-next!))

(define (really-read-image-area new-start-addr img-heap-size port)
  (receive (okay? string)
      (image-read-block port 
			new-start-addr
			(cells->a-units img-heap-size))
    (cond ((not okay?)
	   (read-lost string port))
	  (else 0))))
 
(define (really-read-small-image port)
  (really-read-image-area (get-small-start-addr *heap-image-pointer*)
			  (get-small-img-heap-size)
			  port))

(define (really-read-large-image port)
  (really-read-image-area (get-large-start-addr *heap-image-pointer*)
			  (get-large-img-heap-size)
			  port))

(define (really-read-weaks-image port)
  (really-read-image-area (get-weaks-start-addr *heap-image-pointer*)
			  (get-weaks-img-heap-size)
			  port))

