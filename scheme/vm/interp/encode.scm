
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
; in the currently unused heap and are used to repair the object
; once the message has been made.  The unused heap is also used to
; keep a list of the objects that have received new UID's but for
; which there is no room in the appropriate decode vector.  Once the
; message is complete the vectors are enlarged and the objects are
; added.  If there is insufficient space left in the (active) heap
; to enlarge the decode vectors the list is used to remove the uids.
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
;  (kali-check-heap-n 0 20)
  (let* ((gc-count (s48-gc-count))
	 (encode-start (initialize-message-space! 
			(extract-fixnum (vm-car pair))))
	 (hotel-start (initialize-hotel-space! 
		       (extract-fixnum (vm-cdr pair)))))

    (if (or (< gc-count (s48-gc-count))
	    (null-address? encode-start)
	    (null-address? hotel-start))
	(begin
	  (debug-message 0 "encoding-failed (1)")
	  #f)
	(begin
	  (set! *our-address-space* address-space)

	  (set! *heartbreak-hotel* null-address)
	  (set! *new-id-hotel* null-address)
	  (set! *losing-proxy-hotel* null-address)
	  
	  (store! (address2+ *message-start*)
		  (encode-object thing))

;	  (kali-check-heap-n 1 20)
	  
	  (do-encoding encode-start);(address+ message-start
				             ;(cells->a-units 3)))

	  (let ((result (address->stob-descriptor (address1+ *message-start*))))
	    (mend-hearts! *heartbreak-hotel*)
;	    (kali-check-heap-n 2 20)
	    (if (update-decode-vectors! address-space hotel-start)
		(let ((losers (get-losing-proxies)))
		  (if (false? losers)
		      (begin
			(debug-message 0 "encoding-failed (3)")
			#f)
		      (begin
;			(kali-check-heap-n 3 20)
			(vm-set-car! pair result)
			(vm-set-cdr! pair losers)
;			(kali-check-heap-n 4 20)
			#t)))
		(begin
		  (drop-new-ids! hotel-start)
		  (debug-message 0 "encoding-failed (3)")
		  #f)))))))

;; -------------------

(define *message-pointer*)

(define (store-next! descriptor)
  (debug-message 2 "store-next!")
  (store! *message-pointer* descriptor)
  (set! *message-pointer* (address1+ *message-pointer*)))

;; -------------------
; Aspace decode vectors have two distinguished entries: 
; the next index on the free list and the address space itself.

(define freelist-index 0)
(define aspace-id-in-self 1)

;; --------------------
;; the "hotel-lists"

(define (alloc-list-elt! cells old)
  (let ((start *hotel-pointer*))
    (store! (address+ start (cells->a-units cells))
	    (address->integer old))
    (set! *hotel-pointer* (address+ start (cells->a-units (+ cells 1))))
    start))

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
  (debug-message 1 "remember-heartbreak")
  (let ((room-number (alloc-list-elt! 2 *heartbreak-hotel*)))
    (store! room-number thing)
    (store! (address1+ room-number) header)
    (set! *heartbreak-hotel* room-number)))

(define (mend-hearts! start)
  (debug-message 4 "mend-hearts!")
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
  (debug-message 4 "real-next-id")
  (let* ((next-available (vm-vector-ref decode-vector freelist-index))
	 (extracted (extract-fixnum next-available))
	 (next (if (< extracted (vm-vector-length decode-vector))
		   (let ((next (vm-vector-ref decode-vector extracted)))
		     (vm-vector-set! decode-vector extracted thing)
		     next)
		   (let ((room-number (alloc-list-elt! 1 
						       *new-id-hotel*)))
		     (store! room-number thing)
		     (set! *new-id-hotel* room-number)
		     (enter-fixnum (+ extracted 1))))))
    (vm-vector-set! decode-vector freelist-index next)
    next-available))

; Set the UID slots of objects that just received new UID's back to false.
; Used when we don't have room to add the new objects to the decode vector.

(define (drop-new-ids! start)
  (debug-message 4 "drop-new-ids!")
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
		   ((location)
		    (set-location-uid! thing false))
		   ((proxy)
		    (set-proxy-data-uid! (proxy-data thing) false)
		    (set-proxy-data-reference-count! (proxy-data thing)
						     proxy-base-count)))))))

; Extend the decode vectors as necessary, returning everything to the start
; state if we cannot complete the update (because we ran out of room).

(define (update-decode-vectors! address-space start)
  (debug-message 4 "update-message-vectors!")
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
  (debug-message 4 "extend-decode-vector")
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
  (debug-message 4 "setup-new-decode-vector!")
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

(define (do-encoding start)
  (debug-message 1 "do-encoding")
  (let loop ((start start))
    (let ((end *message-pointer*))
      (encode-locations start end)
      (if (address< end *message-pointer*) ; still playing
	  (loop end)))))

; Encode everything pointed to from somewhere between START and END.

(define (encode-locations start end)
  (debug-message 1 "encode-locations")
  (let loop ((addr start))
    (if (address< addr end)
	(loop (encode-next addr)))))

; Encode the thing pointed to from ADDR, returning the next address to copy.

(define (encode-next addr)
  (debug-message 1 "encode-next")
  (let ((thing (fetch addr))
	(next (address1+ addr)))
    (cond ((b-vector-header? thing)
	   (debug-message 2 "[b-vector-header]")
	   (address+ next 
		     (header-length-in-a-units thing)))
          ((stob? thing)
	   (debug-message 2 "[stob]")
	   (store! addr (encode-object thing))
	   next)
          (else
	   (debug-message 2 "[else]")
	   next))))



; Encode THING if it has not already been encoded.

(define (encode-object thing)
  (debug-message 1 "encode-object")
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
	   (debug-message 2 "[proxy]")
	   (let ((new (address->message-offset (enum element proxy)))
		 (data (proxy-data thing)))
		 ;; Not extracted as this stuff is later scanned
		 (store-next! (let ((owner (proxy-data-owner data)))
				(if (false? owner)
				    (enter-fixnum aspace-id-in-self)
				    (address-space-uid owner))))
		 (store-next! (get-proxy-uid data))
		 (store-next! (debit-proxy-count! data))
		 new))
	  (else
	   (debug-message 2 "[else]")
	   (encode-full-object h thing))))))

(define (encode-two-part-uid uid)
  (debug-message 3 "encode-two-part-uid")
  (let ((new (address->message-offset (enum element uid+owner))))
    ;; Not extracted as this stuff is later scanned
    (cond ((vm-pair? uid)
	   (store-next! (vm-car uid))
	   (store-next! (vm-cdr uid)))
	  (else
	   (store-next! (enter-fixnum aspace-id-in-self))
	   (store-next! uid)))
    new))

(define (encode-full-object header thing)
  (debug-message 1 "encode-full-object")
  (let ((new (address->message-offset (enum element local))))
    (remember-heartbreak thing header)
    (store-next! header)
    (stob-header-set! thing new)	;***Break heart
    (let ((new-hp (address+ *message-pointer* 
			    (header-length-in-a-units header))))
      (do ((o (address-after-header thing) 
	      (address1+ o)))
	  ((address>= *message-pointer* new-hp))
	(store-next! (fetch o))))
    new))

(define (address->message-offset type)
  (debug-message 3 "address->message-offset")
  (make-element type 
		(address-difference *message-pointer* 
				    *message-start*)))

; Utility for getting uids.

(define (get-uid thing accessor setter)
  (debug-message 3 "get-uid")
  (let ((uid (accessor thing)))
    (if (false? uid)
	(let ((uid (next-id thing)))
	  (setter thing uid)
	  uid)
	uid)))

(define (get-proxy-uid thing)
  (debug-message 3 "get-proxy-uid")
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
			    (let ((room-number
				    (alloc-list-elt! 1 *losing-proxy-hotel*)))
			      (store! room-number proxy-data)
			      (set! *losing-proxy-hotel* room-number)
			      1))
			   ((< count (* 2 max-proxy-debit))
			    (logical-shift-right count 1))
			   (else
			    max-proxy-debit))))
	  (set-proxy-data-reference-count! proxy-data
					   (enter-fixnum (- count send)))
	  (enter-fixnum send)))))

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


;; =========================================================================

(define *message-start*) 
(define *message-limit*)

(define (initialize-message-space! cells)
  (let* ((vector (maybe-make-b-vector+gc (enum stob byte-vector)
					 (cells->bytes (- cells 1))))
	 (start (address-at-header vector)))
    (store! (address1+ start)
	    (cells->a-units (- cells 1)))
    
    (set! *message-start* start)
    (set! *message-limit* (address+ start 
				    (cells->a-units cells)))
    (set! *message-pointer* (address+ start 
				      (cells->a-units 3)))
    (address+ start
	      (cells->a-units 3))))
;; -------------------

(define *hotel-pointer*)

(define (initialize-hotel-space! cells)
  (let* ((vector (maybe-make-b-vector+gc (enum stob byte-vector)
					 (cells->bytes cells)))
	(start (address-at-header vector)))
    (set! *hotel-pointer* (address1+ start))
    *hotel-pointer*))

;; -------------------

(define (cells-available? cells)
  (<= cells
      (s48-available)))

;; ===============================================================
;; scan objects to determine the space required for encoding it...
;; ===============================================================

(define (scan-check-heap n x)
  (write-error-string "scan ")
  (write-error-integer n)
  (write-error-string " going to check heap...")
  (write-error-newline)
  (s48-check-heap x)
  (write-error-string "scan ")
  (write-error-integer n)
  (write-error-string " ...checked heap!")
  (write-error-newline))
	  
;; -------------------------------------------------------------

(define *scan-lost*)

(define (scan thing pair)
;  (debug-message 1 "scan")
;  (scan-check-heap 0 20)
  (if (not (init-have-seen! (extract-fixnum (vm-car pair))))
      #f
      (call-with-values
	  (lambda ()
	    (set! *scan-lost* #f)
	    (scan-object thing))
	(lambda (msg-size hotel-size)
;	  (scan-check-heap 1 20)
	  (if *scan-lost*
	      (begin
		(vm-set-car! pair (enter-fixnum (* (vm-car pair) 2)))
		#f)
	      (begin
;		(scan-check-heap 2 20)
;		(write-error-string "msg/hotel-size: ")
;		(write-error-integer (+ msg-size 2))
;		(write-error-string "/")
;		(write-error-integer hotel-size)
;		(write-error-newline)
;		(scan-check-heap 3 20)
		(vm-set-car! pair (enter-fixnum (+ msg-size 2)))
		(vm-set-cdr! pair (enter-fixnum hotel-size))
;		(scan-check-heap 4 20)
		#t))))))

(define (scan-object thing)
  (debug-message 0 "scan-object")
  (let ((h (stob-header thing)))
    (cond ((have-seen? thing)
	   (debug-message 0 "[header was stob]")
	   (values 1 0))
	  ((b-vector-header? h)
	   (debug-message 0 "[header was b-vector-header]")
	   (remember-visit! thing)
	   (values (+ (a-units->cells (header-length-in-a-units h))
		      2) ;; chnx ? warum 2 und nicht 1
		   3))
	  (else
	   (enum-case stob (header-type h)
	     ((symbol)
	      (debug-message 0 "[symbol]")
	      (values 1 2))
	     ((address-space)
	      (debug-message 0 "[address-space]") 
	      (values 1 2))
	     ((template)
	      (debug-message 0 "[template]")
	      (values 3 2))
	     ((location)
	      (debug-message 0 "[location]")
	      (values 3 2))
	     ((proxy)
	      (debug-message 0 "[proxy]")
	      (values 4 4)) ;; chnx hotel 4 -> 5 ?
	     (else
	      (debug-message 0 "[else]")
	      (scan-full-object thing 1 3)))))))

(define (scan-full-object thing msg-size hotel-size)
  (debug-message 1 "scan-full-object")
  (let ((end (address-after-stob thing)))
    (remember-visit! thing)
    (let loop ((start (address-after-header thing))
	       (msg-size (+ msg-size 1));; chnx ? +1 | 1 - 1/elem zu wenig
	       (hotel-size hotel-size))
      (if (address< start end)
	  (call-with-values
	      (lambda ()
		(scan-location start))
	    (lambda (delta-msg-size delta-hotel-size)
	      (loop (address1+ start)
		    (+ msg-size delta-msg-size) 
		    (+ hotel-size delta-hotel-size))))
	  (values msg-size hotel-size)))))

(define (scan-location addr)
  (let ((thing (fetch addr))
	(next (address1+ addr)))
    (if (stob? thing)
	(scan-object thing)
	(values 1 0))))

;; -------------------------------

(define *have-seen-start*)
(define *have-seen-next*)
(define *have-seen-limit*)

(define (init-have-seen! cells)
  (let ((gc-count (s48-gc-count))
	(vector (maybe-make-b-vector+gc (enum stob byte-vector)
					(cells->bytes cells))))
      (if (or (false? vector)
	      (> (s48-gc-count) gc-count))
	  #f
	  (begin
	    (set! *have-seen-start* (address-after-header vector))
	    (set! *have-seen-next* *have-seen-start*)
	    (set! *have-seen-limit* (address-after-stob vector))
	    #t))))

(define (remember-visit! thing)
  (if (address= *have-seen-next* *have-seen-limit*)
      (begin
	(set! *scan-lost* #t)
	#f)
      (begin
	(store! *have-seen-next* 
		(address->integer (address-after-header thing)))
	(set! *have-seen-next* 
	      (address1+ *have-seen-next*)))))

(define (have-seen? thing)
  (or *scan-lost*
      (let ((thing-iaddr (address->integer (address-after-header thing))))
	(let walk ((walker *have-seen-start*))
	  (if (address= walker *have-seen-next*)
	      #f
	      (or (= thing-iaddr (fetch walker))
		  (walk (address1+ walker))))))))



;;==========================================================================


;; chnx - just for the time of development...:

(define debug-prio-level -1)

(define (debug-message prio str)
  (if (< prio debug-prio-level)
      (begin
	(write-error-string str)
	(write-error-newline)
	(unspecific))
      (unspecific)))

;; =======================================


(define (kali-check-heap x)
  (write-error-string "kali going to check heap...")
  (write-error-newline)
  (s48-check-heap x)
  (write-error-string "kali ...checked heap!")
  (write-error-newline))


(define (kali-check-heap-n n x)
  (write-error-string "encode ")
  (write-error-integer n)
  (write-error-string " going to check heap...")
  (write-error-newline)
  (s48-check-heap x)
  (write-error-string "encode ")
  (write-error-integer n)
  (write-error-string " ...checked heap!")
  (write-error-newline))

;; chnx end
