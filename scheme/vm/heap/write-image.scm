; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Steps:
;   1. Trace everything reachable from RESUME-PROC and from the exported
;      bindings (which we must assume are reachable because we cannot trace
;      from the imported bindigns via external data structures to the
;      exported bindings).  All reachable objects are assigned addresses in
;      the image and put in a list in the order they will appear in the image.
;   2. Write the ASCII image header.
;   3. Traverse the list of reachable objects writing them out into the
;      image file.
;   4. Write out the symbol, imported binding, and exported binding tables,
;      updating the buckets to only include those values which were traced.
;   5. Write out a vector containing all records with resume methods.
;
; Several types of objects do not get copied as-is:
;  - Weak pointers
;     The value is written as #F if it is not in the image.
;  - Symbols, shared-bindings
;     The next fields, which link the buckets in the hash tables holding
;     these, are written out so as to elide any bucket entries that were
;     not found during tracing.  Imported bindings have their values
;     written as unassigned.
;  - Channels
;     Written out as closed.
;  - Undumpable records
;     These are treated as if they were the value in the record's first slot.
;
; We can run out of memory.  When this happens we disable the hash table
; to keep from trying to make further progress.  We can also get i/o errors,
; but these are mostly taken care by the image-writing utilities.

(define (s48-write-image resume-proc undumpables port)
  (begin-making-image undumpables)
  (if (image-write-init port)
      (let ((resume-proc (trace-image-value resume-proc)))
	(trace-exported-bindings (s48-exported-bindings))
	(make-image)
	(cond ((table-okay? *stob-table*)
	       (write-header *resumer-records* resume-proc image-descriptor port)
	       (write-descriptor false)	; for endianess check
	       (write-image)
	       (empty-image-buffer!)
	       (deallocate-areas)
	       (deallocate-table *stob-table*)
	       (image-write-terminate)
	       (image-write-status))
	      (else
	       (deallocate-table *stob-table*)
	       (image-write-terminate)
	       (enum errors out-of-memory))))
      (enum errors out-of-memory)))

; The interface to the GC consists of the following, listed in the order in
; which they are called.
;
; (BEGIN-MAKING-IMAGE start-address undumpable)	; UNDUMPABLE is a vector
;
; (TRACE-IMAGE-VALUE value) -> value in image   ; identifies roots
;
; (MAKE-IMAGE)					; scan
;   Scan everything, then make the symbol and imported/exported tables.
;   Walk the stob table to count the resumer records and create that table
;   as well.
;
; (IMAGE-SIZE)
; (IMAGE-SYMBOL-TABLE)      -> stob in image
; (IMAGE-EXPORTED-BINDINGS) -> stob in image
; (IMAGE-IMPORTED-BINDINGS) -> stob in image
; (RESUMER-RECORDS)         -> stob in image
;   To find the resumer records we walk the table looking for them.
;   First to find out how many and then a second time when we write
;   the vector out.
;
; (WRITE-IMAGE)				; must come last
;   Write out all the objects in the stob table and then the symbol and
;   imported/exported tables and finally the resumer records.

(define (begin-making-image undumpable)
  (set! *stob-table*  (make-table))
  (set! *first-stob*  false)
  (set! *last-stob*   (null-pointer))
  (set! *undumpable-records* undumpable)
  (set! *undumpable-count* 0)
  (set! *resumer-count* 0)

  (begin-making-image/gc-specific))

(define *stob-table*)		; Table mapping stobs to image-location records.
(define *first-stob*)		; The beginning and end of the list
(define *last-stob*)		;  of image-location records.
(define *resumer-count*)	; Number of resumer records found so far.
(define *resumer-records*)	; Vector of resumer records created in image.
(define *undumpable-records*)   ; Vector passed to us for undumpable records.
(define *undumpable-count*)	; How many we have found so far.

; Is THING in the image.

(define (image-extant? thing)
  (not (and (stob? thing)
	    (null-pointer? (table-ref *stob-table* thing)))))

; Add THING to the image if it is not already there.  Returns the value of
; THING in the image.  If there has been an error in table we proceed without
; doing anything.

(define (trace-image-value thing)
  (if (stob? thing)
      (let ((have (table-ref *stob-table* thing)))
	(cond ((not (null-pointer? have))
	       (image-location-new-descriptor have))
	      ((undumpable? thing)
	       (trace-undumpable thing))
	      (else
	       (add-new-image-object thing))))
      thing))
    
; Note that we have seen THING and then trace its alias.  We add an entry to
; the table so that THING will not be traced again and so that references to
; it will be written out as the alias.

(define (trace-undumpable thing)
  (note-undumpable! thing)
  (let* ((alias (undumpable-alias thing))
	 (new-alias (trace-image-value alias))
	 (new (make-image-location new-alias)))
    (if (null-pointer? new)
	(break-table! *stob-table*)
	(table-set! *stob-table* thing new))
    new-alias))

; Allocate space for STOB in the image and create a new-descriptor record
; for it.

(define (add-new-image-object stob)
  (receive (new-descriptor new)
      (allocate-new-image-object stob)
    (if (null-pointer? new)
	(break-table! *stob-table*)
	(begin
	  (if (false? *first-stob*)
	      (set! *first-stob* stob)
	      (set-image-location-next! *last-stob* stob))
	  (set! *last-stob* new)
	  (set-image-location-next! new false)
	  (table-set! *stob-table* stob new)
	  (if (resumer-record? stob)
	      (set! *resumer-count* (+ *resumer-count* 1)))
	  (finalize-new-image-object stob)))
    new-descriptor))

; Return the value of THING in the image.  If there has been an error the
; table is gone and we proceed without doing anything.

(define (image-descriptor thing)
  (if (stob? thing)
      (let ((have (table-ref *stob-table* thing)))
	(if (null-pointer? have)
	    (error "traced object has no descriptor in image"))
	(image-location-new-descriptor have))
      thing))

;----------------------------------------------------------------
; Walk the list of objects in the image, tracing the contents of each
; (which may add more objects to the list).  The tables are traced at
; the end and are left off of the list of image objects because they
; have to be written out in a nonstandard way.

(define (make-image)
  (let loop ((stob *first-stob*))
    (trace-contents stob)
    (if (table-okay? *stob-table*)
	(let ((next (image-location-next (table-ref *stob-table* stob))))
	  (if (stob? next)
	      (loop next)))))
  (let ((last *last-stob*))
    (note-traced-last-stob!)
    (trace-image-value (s48-symbol-table))
    (trace-image-value (s48-imported-bindings))
    (trace-image-value (s48-exported-bindings))
    (set-image-location-next! last false))
  
  (set! *resumer-records*
	(image-alloc (enum area-type-size small) (cells->a-units *resumer-count*)))
  (adjust-descriptors! *stob-table*))

(define (trace-contents stob)
  (let ((header (stob-header stob)))
    (if (not (or (b-vector-header? header)
		 (vm-eq? header weak-pointer-header)))
	(let* ((start (address-after-header stob))
	       (end (address+ start (header-length-in-a-units header))))
	  (do ((addr start (address1+ addr)))
	      ((address= addr end))
	    (trace-image-value (fetch addr)))
	  (unspecific)))))

; The exported binding table does not contain normal pointers.  This does
; the appropriate magic for finding the objects it contains.

(define trace-exported-bindings
  (let ((walker (table-walker shared-binding-next)))
    (lambda (table)
      (walker (lambda (binding)
		(trace-image-value binding))
	      table))))

;----------------------------------------------------------------
; Actually write out the image.  This is follows the same sequence as
; MAKE-IMAGE.

(define (write-image)
  (write-image-areas *first-stob* *stob-table* write-stob)
  (write-symbol-table (s48-symbol-table))
  (write-shared-table (s48-imported-bindings))
  (write-shared-table (s48-exported-bindings))
  (write-resumer-records))

(define (write-stob stob)
  (cond ((weak-pointer? stob)
	 (write-descriptor weak-pointer-header)
	 (if (image-extant? (weak-pointer-ref stob))
	     (write-descriptor (image-descriptor (weak-pointer-ref stob)))
	     (write-descriptor false)))
	((channel? stob)
	 (write-channel stob))
	((vm-symbol? stob)
	 (write-symbol stob))
	((shared-binding? stob)
	 (write-shared stob))
	(else
	 (let* ((header (stob-header stob))
		(start (address-after-header stob)))
	   (write-descriptor header)
	   (if (b-vector-header? header)
	       (write-image-block start (header-length-in-a-units header))
	       (write-descriptors start (header-length-in-cells header)))))))

(define (write-descriptors start cells)
  (let ((end (address+ start (cells->a-units cells))))
    (do ((addr start (address1+ addr)))
	((address= addr end))
      (write-descriptor (image-descriptor (fetch addr))))
    (unspecific)))

; Walk the list of objects one last time to find the resumer records.

(define (write-resumer-records)
  (write-descriptor (make-header (enum stob vector)
				 (cells->bytes *resumer-count*)))
  (let loop ((stob *first-stob*))
    (if (stob? stob)
	(let ((location (table-ref *stob-table* stob)))
	  (if (resumer-record? stob)
	      (write-descriptor (image-location-new-descriptor location)))
	  (loop (image-location-next location))))))

; Write out as closed.  The status is the first slot, for what it's worth.
; It would be nice to clobber the os-index as well, but I don't want to add
; any extra assumptions here about what is where.

(define (write-channel channel)
  (let ((header (stob-header channel)))
    (write-descriptor header)
    (write-descriptor closed-status)
    (write-descriptors (address1+ (address-after-header channel))
		       (- (header-length-in-cells header) 1))
    (write-error-string "Channel closed in dumped image: ")
    (let ((id (channel-id channel)))
      (if (fixnum? id)
	  (write-error-integer (extract-fixnum id))
	  (write-error-string (extract-low-string id)))
      (write-error-newline))
    (unspecific)))

(define closed-status
  (enter-fixnum (enum channel-status-option closed)))

; The value of the next field is disguised as a non-pointer to fool the GC.  
; We have to follow it until we find the next symbol that is in the image.

(define (write-symbol symbol)
  (let ((header (stob-header symbol))
	(next (next-extant-symbol (link->value (vm-symbol-next symbol)))))
    (write-descriptor header)
    (write-descriptors (address-after-header symbol)
		       (- (header-length-in-cells header) 1))
    (write-descriptor (value->link (image-descriptor next)))))

(define (next-extant-symbol symbol)
  (do ((next symbol (link->value (vm-symbol-next next))))
      ((image-extant? next)
       next)))

; Follow next, which is another disguised pointer.  We drop the value of
; imports.  The old value will still be in the image, but it's only a
; pointer-sized byte vector.

(define (write-shared shared)
  (let ((header (stob-header shared))
	(next (next-extant-shared (link->value (shared-binding-next shared)))))
    (write-descriptor header)
    (write-descriptors (address-after-header shared)
		       (- (header-length-in-cells header) 2))
    (write-descriptor (if (vm-eq? (shared-binding-is-import? shared)
				  true)
			  unassigned-marker
			  (image-descriptor (shared-binding-ref shared))))
    (write-descriptor (value->link (image-descriptor next)))))

(define (next-extant-shared shared)
  (do ((next shared (link->value (shared-binding-next next))))
      ((image-extant? next)
       next)))

; More disguised pointers.

(define (table-writer next)
  (lambda (table)
    (write-descriptor (stob-header table))
    (do ((i 0 (+ i 1)))
	((= i (vm-vector-length table)))
      (write-descriptor
       (value->link (image-descriptor
		     (next (link->value (vm-vector-ref table i)))))))))

(define write-symbol-table (table-writer next-extant-symbol))
(define write-shared-table (table-writer next-extant-shared))

;----------------
; Undumpable records
;
; Record types may be marked as `undumpable', in which case they are replaced
; in images by the value of their first slot.  Any that are found are put in a
; vector provided by our caller for that purpose.  They are eventually reported
; back to the user.

(define (undumpable? x)
  (and (gc-record? x)
       (let ((type (record-ref x 0)))
	 (and (gc-record? type)
	      (= false (record-ref type 1))))))

(define (gc-record? x)
  (and (stob? x)
       (let ((header (stob-header x)))
	 (if (stob? header)
	     (record? header)
	     (record? x)))))

(define (undumpable-alias record)
  (record-ref record 1))

; We add undumpable records to a vector provided by the caller.

(define (note-undumpable! thing)
  (if (and (< *undumpable-count*
	      (vm-vector-length *undumpable-records*))
	   (not (vector-memq? thing *undumpable-records*)))
      (begin
	(vm-vector-set! *undumpable-records*
			*undumpable-count*
			thing)
	(set! *undumpable-count* (+ 1 *undumpable-count*)))))

(define (vector-memq? thing vector)
  (let ((len (vm-vector-length vector)))
    (let loop ((i 0))
      (cond ((= i len)
	     #f)
	    ((vm-eq? (vm-vector-ref vector i) thing)
	     #t)
	    (else
	     (loop (+ i 1)))))))

;----------------------------------------------------------------
; More abstraction breaking, this time for finding records whose type has
; a resumer method.

(define (resumer-record? stob)
  (and (record? stob)
       (let ((type (record-type stob)))
	 (and (record? type)
	      (stob? (record-type-resumer type))))))

; A record's type is at offset 0 and a type's resumer is at offset 1.

(define (record-type record)
  (record-ref record 0))

(define (record-type-resumer record-type)
  (record-ref record-type 1))

;----------------------------------------------------------------
; Used to detect weak pointers.

(define weak-pointer-header
  (make-header (enum stob weak-pointer)
	       (cells->bytes (- weak-pointer-size 1))))





