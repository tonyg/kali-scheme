
; Table mapping addresses to areas.
;
; Memory is divided into metapages.
; Metapages are divided in into pages.
; We have a table mapping the high bits of an address to the metapage
; containing the page which contains the address.  Each metapage has vector
; that maps the middle bits of an address to an area.
; The memory table only has entries for metapages containing pages of interest.

(define log-pages-per-metapage 10)

(define metapage-index-mask
  (- (shift-left 1 log-pages-per-metapage)
     1))

(define (page-index-in-metapage address)
  (bitwise-and (arithmetic-shift-right (address->integer address)
				       log-bytes-per-page)
	       metapage-index-mask))

; The NEXT field is only needed if we are using hashing.
			      
(define-record-type metapage :metapage
  (make-metapage start-address contents next)
  (start-address address  metapage-start-address)
  (contents      (^ area) metapage-contents)
  (next	         metapage metapage-next set-metapage-next!))

; The Boehm collector also has a link to the next entry in ascending
; order.  This is used for sweeping etc..   I don't think we need
; such.  It would be hard to reconstruct dynamically.  It is easy enough
; to find adjacent blocks, which is all I would think matters to us.

(define log-table-size 10)

(define table-size (shift-left 1 log-table-size))
(define table-mask (- table-size 1))

(define table
  (let ((table (make-vector table-size (make-metapage null-address
						      (null-pointer)
						      (null-pointer)))))
    (vector+length-fill! table table-size (null-pointer))
    ; delete the metapage
    table))

;----------------------------------------------------------------
; We can use either s straight vector or a hash table, depending on the
; number of bits in an address.  With a small number of address bits,
; such as thirty-two, we can use a vector that maps all of memory directly.
; 
; 12 bits of page address
; 10 bits of metapage address
; 10 bits of hashtable address
; -> all 32 bits of address - there is no need to hash or search.
;
; Storage:
;  table - 4k
;  each metapage 4k+
;
; A metapage covers 4meg of memory.

(define (direct-table-index address)
  (logical-shift-right (address->integer address)
		       (+ log-bytes-per-page
			  log-pages-per-metapage)))

; Pick something.  This hashes 20 bits, and probably not too well.
; For 64 bit address and the numbers above, you would have to hash
; 42 bits of address.  Look to see what Hans Boehm uses.

(define (hash-table-index address)
  (let ((start (direct-table-index (address->integer address))))
    (bitwise-xor start
		 (logical-shift-right start log-table-size))))

(define table-index direct-table-index)

(define (direct-find-metapage start-address found not-found)
  (let ((metapage (vector-ref table (table-index start-address))))
    (if (null-pointer? metapage)
	(not-found)
	(found metapage))))

(define (hashed-find-metapage start-address found not-found)
  (let loop ((bucket (vector-ref table (hash-table-index start-address))))
    (cond ((null-pointer? bucket)
	   (not-found))
	  ((address= start-address (metapage-start-address bucket))
	   (found bucket))
	  (else
	   (loop (metapage-next bucket))))))

(define find-metapage direct-find-metapage)

;----------------------------------------------------------------
; REF and SET! 

(define (memory-map-ref address)
  (let ((start-address (page-start-address address)))
    (find-metapage (table-index start-address)
		   start-address
		   (lambda (metapage)
		     (vector-ref (metapage-contents metapage)
				 (page-index-in-metapage address)))
		   (lambda ()
		     (null-pointer)))))

(define (memory-map-set! address value)
  (let* ((start-address (page-start-address address))
	 (index (table-index start-address))
	 (metapage (find-metapage index
				  start-address
				  (lambda (metapage)
				    metapage)
				  (lambda ()
				    (add-metapage! index start-address)))))
    (vector-set! (metapage-contents metapage)
		 (page-index-in-metapage address)
		 value)))

(define (add-metapage! index start-address)
  (let ((metapage (make-metapage start-address
				 (vector-ref table index)
				 (null-pointer))))
    (vector-set! table index metapage)
    metapage))
