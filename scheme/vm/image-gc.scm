; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Collector for making images.

; The interface to the GC consists of
; (BEGIN-MAKING-IMAGE undumpable)		; this first
;
; (S48-TRACE-IMAGE-VALUE value) => copied value	; trace roots
;
; (MAKE-IMAGE)					; then make the image
;
; (RESTORE-HEAP)				; put the heap back in shape

(define *undumpable-records*)
(define *undumpable-count*)

(define (begin-making-image undumpable)
  (set! *from-begin* (s48-heap-begin))
  (set! *from-end*   (s48-heap-pointer))
  (set! *to-begin*   (s48-oldspace-begin))
  (set! *to-end*     (s48-oldspace-end))
  (set! *hp* *to-begin*)
  (set! *undumpable-records* undumpable)
  (set! *undumpable-count* 0))

(define *from-begin*)
(define *from-end*)
(define *to-begin*)
(define *to-end*)
(define *hp*)

(define (image-start)
  *to-begin*)

(define (image-pointer)
  *hp*)

(define (in-oldspace? descriptor)
  (and (stob? descriptor)
       (let ((a (address-after-header descriptor)))
         (and (address>= a *from-begin*)
              (address< a *from-end*)))))

(define (in-image? descriptor)
  (and (stob? descriptor)
       (let ((a (address-after-header descriptor)))
         (and (address>= a *to-begin*)
              (address< a *hp*)))))

(define (image-extant? thing)
  (or (not (stob? thing))		; not a pointer
      (in-image? thing)			; points into image
      (stob? (stob-header thing))))	; has copy in image

(define (trace-image-value stob)
  (cond ((undumpable? stob)
	 (note-undumpable! stob)
	 (trace-image-value (undumpable-alias stob)))
	((in-oldspace? stob)
	 (copy-object stob))
	(else
	 stob)))
    
; Work around a bug in the PreScheme compiler.

(define (hack-trace-image-value stob)
  (trace-image-value stob))
    
; Undo the effects of the current collection (assuming that it did not
; modify any VM registers or the stack).

(define (restore-heap)
  (let loop ((addr *from-begin*))
    (if (address< addr *from-end*)
	(let* ((d (fetch addr))
	       (h (if (header? d)
		      d
		      (let ((h (stob-header d)))
			(store! addr h)            ; mend heart
			h))))
	  (loop (address+ addr
			  (+ (cells->a-units stob-overhead)
			     (header-length-in-a-units h))))))))

; Scan the heap, copying pointed to objects.  Quit once the scanning pointer
; catches up with the heap pointer.

(define (make-image)
  (let loop ((start *to-begin*))
    (let ((end *hp*))
      (trace-image-locations! start end)
      (cond ((address<= *to-end* *hp*)
	     (error "Image writing error: ran out of space in new heap"))
	    ((address< end *hp*)
	     (loop end)))))
  (clean-weak-pointers))

; Copy everything pointed to from somewhere between START (inclusive)
; and END (exclusive).

(define (trace-image-locations! start end)
  (let loop ((addr start) (frontier *hp*))
    (if (address< addr end)
	(let ((thing (fetch addr))
	      (next (address1+ addr)))
	  (cond ((header? thing)
		 (loop (if (or (b-vector-header? thing)
			       (vm-eq? weak-pointer-header thing))
			   (address+ next (header-length-in-a-units thing))
			   next)
		       frontier))
		((undumpable? thing)
		 (note-undumpable! thing)
		 (store! addr (undumpable-alias thing))
		 (loop addr frontier))
		((in-oldspace? thing)
		 (receive (new-thing frontier)
		     (real-copy-object thing frontier)
		   (store! addr new-thing)
		   (loop next frontier)))
		(else
		 (loop next frontier))))
	(set! *hp* frontier)))
  0)  ; for the type-checker

; Copy THING if it has not already been copied.

(define (copy-object thing)
  (receive (new-thing new-hp)
      (real-copy-object thing *hp*)
    (set! *hp* new-hp)
    new-thing))

; Non-heap-pointer version for better code in TRACE-LOCATIONS

(define (real-copy-object thing frontier)
  (let ((h (stob-header thing)))
    (if (stob? h)            ;***Broken heart
	;; (assert (in-newspace? h))
	(values h frontier)
	(let* ((data-addr (address+ frontier (cells->a-units stob-overhead)))
	       (new (address->stob-descriptor data-addr))
	       (new-frontier (address+ data-addr
				       (header-length-in-a-units h))))
	  (if (address<= *to-end* new-frontier)
	      (error "ran out of memory while writing image"))
	  (stob-header-set! thing new)			;***Break heart
	  (store! frontier h)				;copy header
	  (copy-memory! (address-after-header thing)	;copy contents
			data-addr
			(header-length-in-bytes h))
	  (values new new-frontier)))))

; Used both to detect weak pointers.

(define weak-pointer-header
  (make-header (enum stob weak-pointer)
	       (cells->bytes (- weak-pointer-size 1))))

(define (clean-weak-pointers)
  (let loop ((addr *to-begin*))
    (if (address< addr *hp*)
	(let ((header (fetch addr)))
	  (if (vm-eq? header weak-pointer-header)
	      (let ((contents (fetch (address1+ addr))))
		(if (stob? contents)
		    (store! (address1+ addr)
			    (if (and (in-oldspace? contents)
				     (stob? (stob-header contents)))
				(stob-header contents)
				false)))))
	  (loop (address+ addr
			  (+ (cells->a-units stob-overhead)
			     (header-length-in-a-units header))))))))

;----------------
; Undumpable records
;
; Record types may be marked as `undumpable', in which case they are replaced in
; images by the value of their first slot.

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

;----------------
; Locating records that have resumers.
; We walk through the image while pushing any records whose type has a
; resumer procedure onto a vector at the end.

(define (find-resumer-records)
  (let ((start-hp *hp*)
        (data-start (address1+ *hp*)))	; reserve space for header
    (set! *hp* data-start)
    (if (scan-for-resumer-records *to-begin* start-hp)
 	(let ((size (address-difference *hp* data-start)))
 	  (store! start-hp (make-header (enum stob vector) size))
 	  (address->stob-descriptor data-start))
 	(begin
	  (set! *hp* start-hp)		; out of space, so undo and give up
	  false))))
 
(define (scan-for-resumer-records begin end)
  (let loop ((addr begin))
    (if (address< addr end)
	(let* ((header (fetch addr))
	       (next (address+ addr
			       (+ (cells->a-units stob-overhead)
				  (header-length-in-a-units header)))))
	  (cond ((not (header? header))
		 (write-string "new image is not in a consistent state"
			       (current-error-port))
		 #f)
		((not (= (header-type header)
			 (enum stob record)))
		 (loop next))
		(else
		 (let* ((record (address->stob-descriptor (address1+ addr)))
			(type (record-type record)))
		   (cond ((not (and (record? type)
				    (stob? (record-type-resumer type))))
			  (loop next))
			 ((< (cells->a-units 1)
			     (address-difference *to-end* *hp*))
			  (store! *hp* record)
			  (set! *hp* (address1+ *hp*))
			  (loop next))
			 (else
			  #f))))))
	#t)))

; A record's type is at offset 0 and a type's resumer is at offset 1.

(define (record-type record)
  (record-ref record 0))

(define (record-type-resumer record-type)
  (record-ref record-type 1))

;----------------
; Clean the symbol table for inclusion in the image.

(define make-image-symbol-table
  (table-cleaner vm-symbol-next
		 vm-set-symbol-next!
		 image-extant?
		 hack-trace-image-value))

; Copy the exported bindings as-is.

(define copy-exported-bindings
  (table-tracer shared-binding-next
		set-shared-binding-next!
		hack-trace-image-value))

; This returns a version of the import table that contains only those bindings
; that have been traced, and in which the values of the bindings are all set to
; UNASSIGNED-VALUE.

(define clean-imported-bindings
  (let ((cleaner (table-cleaner shared-binding-next
				set-shared-binding-next!
				image-extant?
				hack-trace-image-value))
	(walker (table-walker shared-binding-next)))
    (lambda (imported-bindings)
      (let ((table (cleaner imported-bindings)))
	(walker (lambda (binding)
		  (shared-binding-set! binding unassigned-marker))
		table)
	table))))

;----------------
; Mark channels in about-to-be-dumped heaps as closed.

(define (mark-image-channels-closed! channels count)
  (do ((i 0 (+ i 1)))
      ((= i count))
    (let ((channel (vector-ref channels i)))
      (if (and (not (false? channel))
	       (image-extant? channel))
	  (mark-channel-closed! (trace-image-value channel)))))
  (unspecific))

; Mark CHANNEL as closed, warning the user that we are doing so.

(define (mark-channel-closed! channel)
  (write-error-string "Channel closed in dumped image: ")
  (let ((id (channel-id channel)))
    (if (fixnum? id)
	(write-error-integer (extract-fixnum id))
	(write-error-string (extract-string id)))
    (write-error-newline)
    (set-channel-status! channel
			 (enter-fixnum (enum channel-status-option closed)))
    (set-channel-os-index! channel
			   (enter-fixnum -1))))
