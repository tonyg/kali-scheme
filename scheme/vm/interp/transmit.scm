;; chnx - just for the time of development...:

(define debug-prio-level 9)

(define (debug-message prio str)
  (if (< prio debug-prio-level)
      (begin
	(write-error-string str)
	(write-error-newline)
	(unspecific))
      (unspecific)))

;; chnx end

; All of this file is only for Kali.

; VM code for linearizing objects.

; Requirements:
;  Template and location UIDs are either #f or a number for local templates,
;   or a pair of the owning aspace and owner's uid.
;  Aspace decode vector has next-free-uid in first slot and the aspace itself
;   in the second slot.
;
; Transmitted by uid:
;  symbols
;  externals
;  address-spaces
;  templates       uid + uid of owning aspace
;  locations       uid + uid of owning aspace
;
; Transmitted specially:
;  proxies         uid + uid of owning aspace + reference count
;  Receivers create previously unseen proxies on the spot.

; Encoding:
;  Fixnums, immediates, headers and raw bytes sent as-is.
;  Stobs are sent using the stob tag, six bits of type, and a three-byte offset
;  from the start of the encoded message.  The six type bits indicate one of
;  the following: 

(define-enumeration element  ; `element' is short for `message-element'
  (
   local     ; sent in the message
   uid       ; (symbols, address spaces, externals)
   uid+owner ; local id of address space + id (templates and locations)
   proxy     ; local id of address space + id + reference count
   ))

;----------------
; Encode THING as a message.  This is a simple two-finger GC.
; Pointers to copied objects and copies of their headers are kept
; in the "hotel-space" ([old] currently unused heap) and are used 
; to repair the object once the message has been made. The hotel-space 
; ([old] unused heap) is also used to keep a list of the objects that 
; have received new UID's but for which there is no room in the 
; appropriate decode vector.  Once the message is complete the vectors 
; are enlarged and the objects are added.  If there is insufficient 
; space left in the (active) heap to enlarge the decode vectors the 
; list is used to remove the uids.
;
; The first four bytes of the message are the length of the message in
; a-units, the second four are THING.
;
; THING must be a stob and not a UID object (not a symbol, template,
; proxy, etc.).
;
; This returns the encode message and a list of proxy-datas that have
; nonpositive reference counts.  If it fails because of lack of space
; the returned message is FALSE.

(define (encode thing address-space pair)
  (debug-message 3 "encode start...")

  (set! *message-size* (vm-car pair))
  (set! *hotel-size* (vm-cdr pair))

  (receive (hotel-start hotel-limit)
      (allocate-hotel-space *hotel-size*)
    (set! *hotel-pointer* hotel-start)
    (set! *hotel-limit* hotel-limit)
    (receive (message-start message-limit)
	(allocate-message-space *message-size*)
      (if (or (null-address? message-start)
	      (null-address? hotel-start))
	  (begin
	    (debug-message 0 "encoding-failed (1)")
	    #f)
	  (begin
	    (set! *message-start* message-start)
	    (set! *scan-to* message-start)
	    (set! *encode-next/addr* message-start)
	    (set! *our-address-space* address-space)
	    ;; add space for header, number of bytes, and THING
	    (set! *message-pointer* (address+ *message-start* (cells->a-units 3)))
	    ;; ADDRESS- accounts for room for the header of the last object
	    (set! *message-limit* (address- message-limit
				     (cells->a-units 1)))
	    (set! *hotel-pointer* hotel-start)
	    (set! *heartbreak-hotel* null-address)
	    (set! *new-id-hotel* null-address)
	    (set! *losing-proxy-hotel* null-address)
	    (let ((code (encode-object thing)))
	      (store! (address+ *message-start* (cells->a-units 2)) code)
	      (cond ((or (waterloo? code)
			 (not (do-encoding (address+ *message-start* (cells->a-units 3)))))
		     (mend-hearts! hotel-start)
		     (drop-new-ids! hotel-start)
		     (debug-message 0 "encoding-failed (2)")
		     (vm-set-car! pair (* 2 *message-size*))
		     (vm-set-cdr! pair (* 2 *hotel-size*))
		     #f)
		    (else
		     (let ((result (make-message-vector *message-start*)))
		       (store! (address+ *message-start* (cells->a-units 1))
			       (address-difference *message-pointer* (address1+ *message-start*)))
		       (mend-hearts! hotel-start)
		       (if (update-decode-vectors! address-space hotel-start)
			   (let ((losers (get-losing-proxies)))
			     (if (false? losers)
				 (begin
				   (debug-message 0 "encoding-failed (3)")
				   (vm-set-car! pair (* 2 *message-size*))
				   (vm-set-cdr! pair (* 2 *hotel-size*))
				   #f)
				 (begin
				   (vm-set-car! pair result)
				   (vm-set-cdr! pair losers)
				   (debug-message 5 "encode done.")
				   #t)))
			   (begin
			     (drop-new-ids! hotel-start)
			     (debug-message 0 "encoding-failed (3)")
			     (vm-set-car! pair (* 2 *message-size*))
			     (vm-set-cdr! pair (* 2 *hotel-size*))
			     #f)))))))))))

;; ----------------------------------------------------------

(define-extensible-proc (adjust-message-values! offset old-size new-size)
  (unspecific)
  add-message-value!
  *adjust-message-values-proc*)

(define-syntax define-message-offset-value
  (syntax-rules ()
    ((define-message-offset-value value)
     (begin
       (define value)
       (add-message-value! (lambda (offset old-size new-size)
			    (set! value 
				  (address+ value
					    offset))))))))



;; -------------------

;; enlarged in store-next!
(define-message-offset-value *message-pointer*)

(define *message-limit*)
(add-message-value! (lambda (offset old-size new-size)
		      (set! *message-limit*
			    (address+ *message-limit*
				      (+ offset
					 (- new-size
					    old-size))))))

(define (ensure-message-space cells)
  (or (address<= (address+ *message-pointer* (cells->a-units cells))
		 *message-limit*)
      (enlarge-message-space cells)))

;; -----------------------------
(define (make-message-vector start)
  (debug-message 30 "make-message-vector")
  (store! *message-vector-header-address*
	  (make-header (enum stob byte-vector)
		       0))
  ;; clean up unused space:
  (space->byte-vector *message-pointer*
		      (address2+ *message-limit*))
  ;; return the used space as a vector
  (space->byte-vector start 
		      *message-pointer*))
  
(define (space->byte-vector start end)
  (store! start
	  (make-header (enum stob byte-vector)
		       (address-difference end (address1+ start))))
  (address->stob-descriptor (address1+ start)))

;----------------
; Aspace decode vectors have two distinguished entries: the next index on the
; free list and the address space itself.

(define freelist-index 0)
(define aspace-id-in-self 1)

;----------------
; We use the currently unused half of the heap for various lists.
;; not any more. We allocate "normal" heap-space for our hotels.

(define-extensible-proc (adjust-hotel-values! start end size)
  (unspecific)
  add-hotel-value!
  *adjust-hotel-value-proc*)

(define *hotel-size*)
(add-hotel-value! (lambda (start end size)
		    (set! *hotel-size* size)))

(define *hotel-pointer*)
(add-hotel-value! (lambda (start end size)
		    (set! *hotel-pointer* start)))

(define *hotel-limit*)
(add-hotel-value! (lambda (start end size)
		    (set! *hotel-limit* end)))

;; this only ensures the space as long as cells
;; is <= 128 - that's ok since it is called with
;; a maximum of 3.
(define (ensure-hotel-space cells)
  (or (address<= (address+ *hotel-pointer* (cells->a-units cells))
		 *hotel-limit*)
      (enlarge-hotel-space)))

(define (alloc-list-elt! cells old)
  (debug-message 40 "alloc-list-elt!")
  (if (not (ensure-hotel-space (+ cells 1)))
      null-address
      (let ((start *hotel-pointer*))
	(store! (address+ start (cells->a-units cells))
		(address->integer old))
	(set! *hotel-pointer* (address+ start (cells->a-units (+ cells 1))))
	start)))

(define (walk-list start cells proc)
  (do ((ptr start (list-elt-next ptr cells)))
      ((null-address? ptr))
    (proc ptr)))

(define (reduce-list start cells data proc)
  (do ((ptr start (list-elt-next ptr cells))
       (data data (proc ptr data)))
      ((null-address? ptr)
       data)))

(define (list-elt-next ptr cells)
  (integer->address (fetch (address+ ptr (cells->a-units cells)))))

;----------------
; A list of the copied objects.  This is used to restore the objects' headers
; when the message is complete.  Each element has the object and the object's
; original header.

(define *heartbreak-hotel*)

(define (remember-heartbreak thing header)
  (debug-message 30 "remember-heartbreak")
  (let ((room-number (alloc-list-elt! 2 *heartbreak-hotel*)))
    (if (null-address? room-number)
	*waterloo*
	(begin
	  (store! room-number thing)
	  (store! (address1+ room-number) header)
	  (set! *heartbreak-hotel* room-number)
	  *success*))))

(define (mend-hearts! start)
  (debug-message 40 "mend-hearts!")
  (walk-list *heartbreak-hotel*
	     2
	     (lambda (ptr)
	       (stob-header-set! (fetch ptr) (fetch (address1+ ptr))))))

;----------------
; Adding new UIDs.

(define *our-address-space* (unspecific))

; Address of next slot in the vector of objects given uids but not placed in
; the decode vector.

(define *new-id-hotel* (unspecific))

; Get the next id from the freelist in the decode vector.  If the id is off
; the end of the vector we save the new object to be added to the decode vector
; later.

(define (next-id thing)
  (real-next-id thing (address-space-decode-vector *our-address-space*)))

(define (next-proxy-id thing)
  (real-next-id thing (address-space-proxy-vector *our-address-space*)))

; Get the next uid from the decode vector's freelist and either add THING
; to the decode vector or put it in the *NEW-ID-HOTEL*.

(define (real-next-id thing decode-vector)
  (debug-message 40 "real-next-id")
  (let* ((next-available (vm-vector-ref decode-vector freelist-index))
	 (extracted (extract-fixnum next-available))
	 (next (if (< extracted (vm-vector-length decode-vector))
		   (let ((next (vm-vector-ref decode-vector extracted)))
		     (vm-vector-set! decode-vector extracted thing)
		     next)
		   (let ((room-number (alloc-list-elt! 1 *new-id-hotel*)))
		     (if (null-address? room-number)
			 *waterloo*
			 (begin
			   (store! room-number thing)
			   (set! *new-id-hotel* room-number)
			   (enter-fixnum (+ extracted 1))))))))
    (if (waterloo? next)
	*waterloo*
	(begin
	  (vm-vector-set! decode-vector freelist-index next)
	  next-available))))

; Set the UID slots of objects that just received new UID's back to false.
; Used when we don't have room to add the new objects to the decode vector.

(define (drop-new-ids! start)
  (debug-message 40 "drop-new-ids!")
  (walk-list *new-id-hotel*
	     1
	     (lambda (ptr)
	       (let ((thing (fetch ptr)))
		 (enum-case stob (header-type (stob-header thing))
		   ((symbol)
		    (vm-set-symbol-uid! thing false))
		   ((template)
		    (set-template-uid! thing false))
		   ((address-space)
		    (set-address-space-uid! thing false))
		   ((address-space)
		    (set-address-space-uid! thing false))
		   ((location)
		    (set-location-uid! thing false))
		   ((proxy)
		    (set-proxy-data-uid! (proxy-data thing) false)
		    (set-proxy-data-reference-count! (proxy-data thing)
						     proxy-base-count)))))))

; Extend the decode vectors as necessary, returning everything to the start
; state if we cannot complete the update (because we ran out of room).

(define (update-decode-vectors! address-space start)
  (debug-message 40 "update-message-vectors!")
  (let ((old-decode (address-space-decode-vector address-space))
	(old-proxy (address-space-proxy-vector address-space)))
    (let ((decode-okay? (<= (extract-fixnum
			     (vm-vector-ref old-decode freelist-index))
			    (vm-vector-length old-decode)))
	  (proxy-okay? (<= (extract-fixnum
			    (vm-vector-ref old-proxy freelist-index))
			   (vm-vector-length old-proxy))))
      (if decode-okay?
	  (or proxy-okay?
	      (extend-decode-vector address-space start #t))
	  (and (extend-decode-vector address-space start #f)
	       (or proxy-okay?
		   (extend-decode-vector address-space start #t)
		   (begin
		     (set-address-space-proxy-vector! address-space old-proxy)
		     #f)))))))

; See if there is room to extend ADDRESS-SPACE's decode vectors to hold any
; objects that just received uids.  If the space is there, then make a new
; decode vector.

(define (extend-decode-vector address-space start proxies?)
  (debug-message 40 "extend-decode-vector")
  (let* ((decode-vector (if proxies?
			    (address-space-proxy-vector address-space)
			    (address-space-decode-vector address-space)))
	 (old-length (vm-vector-length decode-vector))
	 (next-uid (extract-fixnum (vm-vector-ref decode-vector freelist-index)))
	 (minimum-new-length next-uid)
	 (want-length (max (* old-length 2) minimum-new-length))
	 (new-length (cond ((cells-available? (vm-vector-size want-length))
			    want-length)
			   ((cells-available? (vm-vector-size minimum-new-length))
			    minimum-new-length)
			   (else
			    0))))
    (if (= new-length 0)
	#f
	(let ((new-vector (vm-make-vector new-length
					  (ensure-space
					   (vm-vector-size new-length)))))
	  (setup-new-decode-vector! new-vector decode-vector start proxies?)
	  (if proxies?
	      (set-address-space-proxy-vector! address-space new-vector)
	      (set-address-space-decode-vector! address-space new-vector))
	  #t))))

; 1. Copy contents of OLD-VECTOR into NEW-VECTOR.
; 2. Copy objects with new uids into NEW-VECTOR.
; 3. Link any unused slots into a freelist.

(define (setup-new-decode-vector! new-vector old-vector start proxies?)
  (debug-message 40 "setup-new-decode-vector!")
  (let ((old-length (vm-vector-length old-vector))
	(new-length (vm-vector-length new-vector))
	(next-uid (extract-fixnum (vm-vector-ref old-vector freelist-index))))
    (do ((i 0 (+ i 1)))
	((= i old-length))
      (vm-vector-set! new-vector i (vm-vector-ref old-vector i)))
    (reduce-list *new-id-hotel*
		 1			; cells per element
		 (- next-uid 1)		; first index
		 (lambda (ptr i)
		   (let* ((next (fetch ptr))
			  (is-proxy? (proxy-data? next)))
		     (if (if proxies?
			     is-proxy?
			     (not is-proxy?))
			 (begin
			   (vm-vector-set! new-vector i next)
			   (- i 1))
			 i))))
    (do ((i next-uid (+ i 1)))
	((= i new-length))
      (vm-vector-set! new-vector i (enter-fixnum (+ i 1))))))

;----------------
; Scan the heap, encoding pointed to objects, starting from START.  Quit once
; the scanning pointer catches up with the heap pointer.

(define-message-offset-value *scan-to*)

(define (do-encoding start)
  (debug-message 10 "do-encoding")
  (let loop ((start start))
    (set! *scan-to* *message-pointer*)
    (cond ((not (encode-locations start)) ; lost
	   #f)
	  ((address< *scan-to* *message-pointer*)       ; still playing
	   (loop *scan-to*))
	  (else #t))))           ; won

; Encode everything pointed to from somewhere between START and END.

(define (encode-locations start)
  (debug-message 30 "encode-locations")
  (let loop ((addr start))
    (if (address< addr *scan-to*)
	(let ((next (encode-next addr)))
	  (if (null-address? next)
	      #f
	      (loop next)))
	#t)))

; Encode the thing pointed to from ADDR, returning the next address to copy.

(define-message-offset-value *encode-next/addr*)

(define (encode-next addr)
  (debug-message 30 "encode-next")
  (set! *encode-next/addr* addr)
  (let ((thing (fetch *encode-next/addr*)))
    (cond ((b-vector-header? thing)
	   ;;(debug-message 3 "encode-next: [b-vector-header]")
	   (address+ (address1+ *encode-next/addr*) 
		     (header-a-units thing)))
          ((stob? thing)
	   ;;(debug-message 3 "encode-next: [stob]")
	   (let ((encoded-thing (encode-object thing)))
	     (if (waterloo? encoded-thing)
		 null-address
		 (begin
		   (store! *encode-next/addr* encoded-thing)
		   (address1+ *encode-next/addr*)))))
          (else
	   ;;(debug-message 3 "encode-next: [else]")
	   (address1+ *encode-next/addr*)))))

; Encode THING if it has not already been encoded.

;; and return *success* or *waterloo*

(define *success* 1)
(define *waterloo* -1)

(define (waterloo? val)
  (= *waterloo* val))

(define (encode-object thing)
  (debug-message 3 "encode-object")
  (let ((h (stob-header thing)))
    (if (stob? h)            ;***Broken heart
	h
	(enum-case stob (header-type h)
	  ((symbol)
	   (debug-message 2 "[symbol]")
	   (make-element (enum element uid)
			 (extract-fixnum
			  (get-uid thing vm-symbol-uid vm-set-symbol-uid!))))
	  ((address-space)
	   (debug-message 2 "[address-space]") 
	   (make-element (enum element uid)
			 (extract-fixnum
			  (get-uid thing address-space-uid set-address-space-uid!))))
	  ((template)
	   (debug-message 2 "[template]")
	   (encode-two-part-uid
	     (get-uid thing template-uid set-template-uid!)))
	  ((location)
	   (debug-message 2 "[location]")
	   (encode-two-part-uid
	      (get-uid thing location-uid set-location-uid!)))
	  ((proxy)
	   (debug-message 2 "!!![proxy]!!!")
	   (if (not (ensure-message-space 3))
	       *waterloo*
	       (let ((new (address->message-offset (enum element proxy)))
		     (data (proxy-data thing)))
		 ;; Not extracted as this stuff is later scanned
		 (store-next! (let ((owner (proxy-data-owner data)))
				(if (false? owner)
				    (enter-fixnum aspace-id-in-self)
				    (address-space-uid owner))))
		 (store-next! (get-proxy-uid data))
		 (store-next! (debit-proxy-count! data))
		 new)))
	  (else
	   (debug-message 2 "[else]")
	   (encode-full-object h thing))))))

(define (encode-two-part-uid uid)
  (debug-message 30 "encode-two-part-uid")
  (if (not (ensure-message-space 2))
      *waterloo*
      (let ((new (address->message-offset (enum element uid+owner))))
	;; Not extracted as this stuff is later scanned
	(cond ((vm-pair? uid)
	       (store-next! (vm-car uid))
	       (store-next! (vm-cdr uid)))
	      (else
	       (store-next! (enter-fixnum aspace-id-in-self))
	       (store-next! uid)))
	new)))

(define (encode-full-object header thing)
  (debug-message 30 "encode-full-object")
  (if (not (ensure-message-space (header-a-units header)))
      *waterloo*
      (let ((new (address->message-offset (enum element local))))
	(if (waterloo? (remember-heartbreak thing header))
	    *waterloo*
	    (begin
	      (store-next! header)
	      (stob-header-set! thing new)	;***Break heart
	      (let ((new-hp (address+ *message-pointer* (header-a-units header))))
		(do ((o (address-after-header thing) (address1+ o)))
		    ((address>= *message-pointer* new-hp))
		  (store-next! (fetch o))))
	      new)))))


(define-message-offset-value *message-start*)

(define (address->message-offset type)
  (debug-message 30 "address->message-offset")
  (make-element type (address-difference *message-pointer* *message-start*)))

; Utility for getting uids.

(define (get-uid thing accessor setter)
  (debug-message 30 "get-uid")
  (let ((uid (accessor thing)))
    (if (false? uid)
	(let ((uid (next-id thing)))
	  (setter thing uid)
	  uid)
	uid)))

(define (get-proxy-uid thing)
  (debug-message 30 "get-proxy-uid")
  (let ((uid (proxy-data-uid thing)))
    (if (false? uid)
	(let ((uid (next-proxy-id thing)))
	  (set-proxy-data-uid! thing uid)
	  uid)
	uid)))

;----------------
; Keeping track of proxy reference counts.  When a proxy is sent it gets
; half of its local counts or MAX-PROXY-DEBIT, whichever is less.  Any proxy
; whose reference count goes below one must be reported, since the message
; cannot be sent until the count is again positive.  The reporting is done
; by the code that patches up the headers after encoding is complete.

(define *losing-proxy-hotel*)

; This is also defined in kali/aspace.scm.
(define shared-address-space-uid 2)

(define (debit-proxy-count! proxy-data)
  (debug-message 4 "debit-proxy-count")
  (let ((owner (proxy-data-owner proxy-data)))
    (if (and (not (eq? owner false))		; local proxies have #f as owner
	     (= (address-space-uid owner)
		(enter-fixnum shared-address-space-uid)))
	(enter-fixnum 0)
	(let* ((count (extract-fixnum (proxy-data-reference-count proxy-data)))
	       (send (cond ((<= count 1)
			    (debug-message 4 "[<= 1] => 1 && losing-proxy!")
			    (let ((room-number
				    (alloc-list-elt! 1 *losing-proxy-hotel*)))
			      (if (null-address? room-number)
				  *waterloo*
				  (begin
				    (store! room-number proxy-data)
				    (set! *losing-proxy-hotel* room-number)
				    1))))
			   ((< count (* 2 max-proxy-debit))
			    (debug-message 4 "[< (* 2 max-proxy-debit)] => (/ count 2)")
			    (logical-shift-right count 1))
			   (else
			    (debug-message 4 "[else = >= max-proxy-debit] => max-proxy-debit")
			    max-proxy-debit))))
	  (if (waterloo? send)
	      *waterloo*
	      (begin
		(set-proxy-data-reference-count! proxy-data
						 (enter-fixnum (- count send)))
		(enter-fixnum send)))))))

; Make a list of the proxies with non-positive reference counts.  This
; returns FALSE if it runs out of space.

(define (get-losing-proxies)
  (debug-message 4 "get-losing-proxies")
  (reduce-list *losing-proxy-hotel*
	       1     ; cells
	       null  ; initial result list
	       (lambda (ptr result)
		 (if (cells-available? vm-pair-size)
		     (vm-cons (fetch ptr)
			      result
			      (ensure-space vm-pair-size))
		     false))))

;; -------------------------------------------
;; getting rid of two-space--gc depependencies
;; we allocate the space for message and the hotel
;; ourself...

;; ------------------
;; the message-space:

(define *message-size*)
(add-message-value! (lambda (offset old-size new-size)
		      (set! *message-size* new-size)))

(define *message-vector-header-address*)

(define (allocate-message-space bytes)
  (debug-message 1 "allocate-message-space")
  (if (not (bytes-available? (+ bytes 4))) ;; 4 for the header
      (values null-address
	      null-address)
      (let ((message-vector (make-byte-vector bytes)))
	(set! *message-vector-header-address*
	      (address-at-header message-vector))
	(values (address-after-header message-vector)
		(address- (address-after-stob message-vector)
			  (cells->a-units 1))))))

(define (enlarge-message-space needed-bytes)
  (debug-message 1 "enlarge-message-space")
  (let ((max-stob-size-in-bytes (cells->bytes max-stob-size-in-cells)))
  ;;(debug-message 3 "no ENLARGING jet!!!")
    (if (>= *message-size* max-stob-size-in-bytes)
	(begin
	  (debug-message 0 "cannot enlarge-message-space (1)")
	  #f)
	(let ((old-start *message-start*)
	      (want-space (let loop ((want-perhaps (* *message-size* 2)))
			    (let ((want (if (<= want-perhaps
						max-stob-size-in-bytes)
					    want-perhaps
					    max-stob-size-in-bytes)))
			      (if (< (- want *message-size*)
				     needed-bytes)
				  (if (= want 
					 max-stob-size-in-bytes)
				      0
				      (loop (* want 2)))
				  want)))))
	  (if (= want-space 0)
	      (begin
		(debug-message 0 "cannot enlarge-message-space (2)")
		#f)
	      (receive (new-start new-end)
		  (allocate-message-space want-space)
		(if (null-address? new-start)
		    (begin
		      (debug-message 0 "cannot enlarge-message-space (3)")
		      #f)
		    (let ((offset (address-difference new-start old-start)))
		      ;(debug-message 3 "really enlarging!")
		      (copy-memory! old-start new-start 
				    (address-difference *message-pointer* *message-start*))
		      (adjust-message-values! offset *message-size* want-space)
		      #t))))))))

;; ----------------
;; the hotel-space:

(define (allocate-hotel-space bytes)
  (debug-message 1 "allocate-hotel-space")
  (if (not (bytes-available? (+ bytes 4))) ;; 4 for the header
      (values null-address
	      null-address)
      (let ((hotel-vector (make-byte-vector bytes)))
	(values (address-after-header hotel-vector)
		(address- (address-after-stob hotel-vector)
			  (cells->a-units 1))))))

;; chnx ??? should have a get-what-you-can-feature
(define (enlarge-hotel-space)
  (debug-message 1 "enlarge-hotel-space")
  (let ((max-stob-size-in-bytes (cells->bytes max-stob-size-in-cells)))
    (if (= *hotel-size* max-stob-size-in-bytes)
	(begin
	  (debug-message 0 "cannot enlarge-hotel-space (1)")
	  #f)
	(let ((want-space (let ((want (* *hotel-size* 2)))
			    (if (< want max-stob-size-in-bytes)
				want
				max-stob-size-in-bytes))))
	  (receive (new-start new-end)
	      (allocate-hotel-space want-space)
	    (if (null-address? new-start)
		(begin
		  (debug-message 0 "cannot enlarge-hotel-space (2)")
		  #f)
		(begin
		  (adjust-hotel-values! new-start new-end want-space)
		  #t)))))))

;----------------------------------------------------------------
; Decoding messages.  We destroy the message in the process of
; decoding it.
;
; (DECODE byte-vector address-space reverse? key)
;       -> object + missing-objects + newly-positive-proxies
; Each missing object is represented as a vector containing:
;  kind      ; One of proxy, proxy-count, other.  If the missing thing is a
;              proxy it is only missing because we don't have the aspace or
;              its decode vector wasn't large enough.  PROXY-COUNT is used
;              when a proxy's reference count goes above the allowed maximum.
;              PROXY-COUNT vectors don't have STOB and OFFSET fields.
;  aspace-id ; The sender's uid for the address space that the object (only for
;              proxies, templates, locations).
;  local-id  ; ID whose value is needed
;  stob      ; the object containing the reference to the uid object
;  offset    ; the offset of the uid object in the containing object
; Each newly-positive proxy is a pair (proxy . count) saying that we just
; received COUNT references for PROXY, whose current count is less than zero.
; The newly arrived counts are used for releasing waiting messages.
;
; UID objects in the message may require a missing-uid vector.  Each such
; vector has five slots and is consed on a list.  It takes three descriptors
; to encode each uid object.
; Each new proxy created may require making the proxy, proxy-data, weak, or to
; make a pair, and add whichever to a list.  It takes four descriptors to encode
; each proxy in the message.
;
; It might be possible to scan a message to determine the required space.

(define decode-space-multiplier
  (max (quotient (+ (vm-vector-size 5)		; Make a missing uid vector,
		    vm-pair-size		; put it on a list,
		    2)				; and round up.
		 3)				; uses up three cells of message
       (quotient (+ (max (+ proxy-data-size	; Make a proxy-data,
			    proxy-size		; and rejuvenate it,
			    weak-pointer-size)
			 (+ vm-pair-size	; or make a pair,
			    proxy-size		; and rejuvenate a proxy,
			    weak-pointer-size))
		    vm-pair-size		; put whichever on a list,
		    3)				; and round up.
		 4)))				; uses up four cells of message

; DECODE assumes that the size has been stripped off the front of the message.

(define (decode message aspace reverse? key)
  (debug-message 5 "start decode...")
  (let ((start (address-at-header message))
	(limit (address-after-stob message)))
    (if reverse?
	(reverse-byte-order! (address1+ start) limit))
    (set! *message-start* start)
    (set! *new-uids* null)
    (set! *bad-count-proxies* null)
    (decode-message-body (address1+ (address1+ start)) limit aspace key)
    (stob-header-set! message (make-header (enum stob byte-vector)
					   (cells->bytes 1)))
    (debug-message 5 "decode done")
    (values (address->stob-descriptor
	     (address+ *message-start* (element-info (fetch (address1+ start)))))
	    *new-uids*
	    *bad-count-proxies*)))

;; The following two procedures are taken from heap.scm.

(define reverse-byte-order!
   (lambda (start end)
      (let loop
	   ((ptr start))
	 (if (address< ptr end)
	     (begin
		(reverse-descriptor-byte-order! ptr)
		(loop
		   (let ((value
			    (fetch ptr))
			 (next
			    (address1+ ptr)))
		      (if (b-vector-header? value)
			  (address+ next (header-a-units value))
			  next))))))))

(define (reverse-descriptor-byte-order! addr)
  (let ((x (fetch-byte addr)))
    (store-byte! addr (fetch-byte (address+ addr 3)))
    (store-byte! (address+ addr 3) x))
  (let ((x (fetch-byte (address+ addr 1))))
    (store-byte! (address+ addr 1) (fetch-byte (address+ addr 2)))
    (store-byte! (address+ addr 2) x)))

;; Actually do the decode.  We loop through the message decoding the contents
;; of descriptor vectors, looping over byte vectors.

(define (decode-message-body start limit aspace key)
  (let loop ((addr start))
    (if (address< addr limit)
	(let ((thing (fetch addr))
	      (next (address1+ addr)))
	  (if (d-vector-header? thing)
	      (decode-stob-contents next
				    (header-length-in-cells thing)
				    aspace
				    key))
	  (loop (address+ next (header-a-units thing)))))))

;; Loop throught the object.

(define (decode-stob-contents start count aspace key)
  (let ((end (address+ start (cells->a-units count))))
    (do ((addr start (address1+ addr)))
	((address= addr end))
      (let ((thing (fetch addr)))
	(if (stob? thing)
	    (store! addr
		    (decode-object thing aspace addr start key))))))
  (unspecific)) ; for the type checker

; Dispatch on the type of the thing.

(define (decode-object thing aspace addr stob-start key)
  (debug-message 4 "decode-object")
  (let ((data (element-info thing)))
    (enum-case element (element-type thing)
      ((local)
       (debug-message 3 "[local]")
       (address->stob-descriptor (address+ *message-start* data)))
      ((uid)
       (debug-message 3 "[uid]")
       (decode-uid data aspace aspace-id-in-self addr stob-start key))
      ((uid+owner)
       (debug-message 3 "[uid+owner]")
       (receive (aspace-uid uid)
	   (get-full-uid-data data)
	 (let ((aspace (lookup-uid aspace-uid aspace)))
	   (if (address-space? aspace)
	       (decode-uid uid aspace aspace-uid addr stob-start key)
	       (add-pending! aspace-uid uid addr stob-start key)))))
      (else   ; (proxy)   ; Type checker can't handler missing ELSE clause
       (debug-message 3 "!!![proxy]!!! - (else)")
       (receive (aspace-uid uid count)
	   (get-proxy-data data)
	 (let ((aspace (lookup-uid aspace-uid aspace)))
	   (if (address-space? aspace)
	       (lookup-proxy aspace aspace-uid uid count addr stob-start key)
	       (add-pending-proxy! aspace-uid uid count addr stob-start key))))))))

;;----------------
;; Get the address space and uid for a template or location.

(define (get-full-uid-data data)
  (let* ((data-addr (address- (address+ *message-start* data)
			      (cells->a-units 1)))
	 (aspace (extract-fixnum (fetch data-addr)))
	 (uid (extract-fixnum (fetch (address1+ data-addr)))))
    ;; make data look like a code vector to the heap scanner
    (store! data-addr (make-header (enum stob byte-vector)
				   (cells->bytes 1)))
    (values aspace uid)))

; Same deal, except that there are three values.

(define (get-proxy-data data)
  (debug-message 4 "get-proxy-data")
  (let ((data-addr (address- (address+ *message-start* data)
			     (cells->a-units 1))))
    (let ((aspace (extract-fixnum (fetch data-addr)))
	  (uid (extract-fixnum (fetch (address1+ data-addr))))
	  (count (extract-fixnum (fetch (address1+ (address1+ data-addr))))))
      ;; make data look like a code vector to the heap scanner
      (store! data-addr (make-header (enum stob byte-vector)
				     (cells->bytes 2)))
      (values aspace uid count))))

;----------------
; Getting proxies.

(define (lookup-proxy aspace aspace-uid uid count addr stob-start key)
  (debug-message 4 "lookup-proxy")
  (let ((proxy-vector (address-space-proxy-vector aspace)))
    (if (< uid (vm-vector-length proxy-vector))
	(let ((thing (vm-vector-ref proxy-vector uid)))
	  (debug-message 4 "[< uid proxy-vector]")
	  (if (proxy-data? thing)
	      (let ((proxy (weak-pointer-ref (proxy-data-self thing))))
		(debug-message 4 "[we know proxy allready]")
		(add-to-proxy-count! thing count key)
		(if (proxy? proxy)
		    proxy
		    (rejuvenate-proxy! thing key)))
	      (let ((proxy-data (make-proxy-data (enter-fixnum uid)
						 aspace
						 unassigned-marker  ; value
						 (enter-fixnum count)
						 false              ; self
						 null		    ; waiters
						 key)))
		(vm-vector-set! proxy-vector uid proxy-data)
		(rejuvenate-proxy! proxy-data key))))
	(add-pending-proxy! aspace-uid uid count addr stob-start key))))

; We have to return a list of proxies whose reference counts are less
; than one or greater than the allowable maximum.

(define *bad-count-proxies*)

(define (add-to-proxy-count! proxy-data more key)
  (debug-message 4 "add-to-proxy-count!")
  (let ((count (extract-fixnum (proxy-data-reference-count proxy-data))))
    (if (< count 1)
	(begin
	  (debug-message 4 "[< count 1] add to bad-count-proxies")
	  (set! *bad-count-proxies*
		(vm-cons (vm-cons proxy-data more key)
			 *bad-count-proxies*
			 key)))
	(begin
	  (debug-message 4 "[>= count 1]")
	  (if (and (< max-proxy-count (+ count more))
		   (not (false? (proxy-data-owner proxy-data))))
	      (begin
		(debug-message 4 "[(< max-proxy-count (+ count more)) and non-local] add to bad-count proxies")
		(set! *bad-count-proxies*
		      (vm-cons proxy-data
			       *bad-count-proxies*
			       key))))
	  (set-proxy-data-reference-count! proxy-data
					   (enter-fixnum (+ count more)))))))

; Make a proxy corresponding to PROXY-DATA.

(define (rejuvenate-proxy! proxy-data key)
  (let* ((proxy (really-make-proxy proxy-data key))
	 (weak (make-weak-pointer proxy weak-pointer-size)))
    (set-proxy-data-self! proxy-data weak)
    proxy))

;----------------

(define (decode-uid uid aspace aspace-uid addr stob-start key)
  (let ((thing (lookup-uid uid aspace)))
    (if (stob? thing)
	thing
	(add-pending! aspace-uid uid addr stob-start key))))

;; Returns either the thing or false.

(define (lookup-uid uid aspace)
  (let ((decode-vector (address-space-decode-vector aspace)))
    (if (>= uid (vm-vector-length decode-vector))
	false
	(vm-vector-ref decode-vector uid))))

; A list of vectors, one for each newly encounterd uid.  Each vector has
; five slots:
;   aspace-uid
;   uid
;   reference cout for proxies, #f for non-proxies
;   the stob contining the object
;   the offset of the object within the stob

(define *new-uids*)

(define (add-pending! aspace-uid uid addr stob-start key)
  (debug-message 4 "add-pending!")
  (really-add-pending! aspace-uid uid false addr stob-start key))

(define (add-pending-proxy! aspace-uid uid count addr stob-start key)
  (debug-message 4 "add-pending-proxy!")
  (really-add-pending! aspace-uid uid (enter-fixnum count) addr stob-start key))

(define (really-add-pending! aspace-uid uid count addr stob-start key)
  (let ((stob (address->stob-descriptor stob-start))
	(offset (a-units->cells (address-difference addr stob-start)))
	(vector (vm-make-vector 5 key)))
    (vm-vector-set! vector 0 (enter-fixnum aspace-uid))
    (vm-vector-set! vector 1 (enter-fixnum uid))
    (vm-vector-set! vector 2 count)   ; FALSE for non-proxies
    (vm-vector-set! vector 3 stob)
    (vm-vector-set! vector 4 (enter-fixnum offset))
    (set! *new-uids* (vm-cons vector *new-uids* key))
    false))

;----------------
; Address utilities.

(define (store-next! descriptor)
  (store! *message-pointer* descriptor)
  (set! *message-pointer* (address1+ *message-pointer*)))

; The following are also in gc.  Perhaps they should be in data.

(define (header-a-units h)
  (bytes->a-units (header-length-in-bytes h)))

(define (address-at-header stob)
  (address- (address-after-header stob) (cells->a-units 1)))

(define (address-after-stob stob)
  (address+ (address-after-header stob)
	    (bytes->a-units (header-length-in-bytes (stob-header stob)))))

;; ------------------

(define (make-byte-vector bytes)
  (let ((start (s48-allocate-untraced+gc (+ bytes 
					    (cells->a-units 1)))))
    (store! start (make-header (enum stob byte-vector)
			       bytes))
    (address->stob-descriptor (address1+ start))))

(define (bytes-available? bytes)
  (<= (a-units->cells (bytes->a-units bytes))
      (s48-available)))

(define (cells-available? cells)
  (<= cells
      (s48-available)))
