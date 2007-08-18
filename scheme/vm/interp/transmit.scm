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
  ;(debug-message "encode start...")
  (set! *message-space* 128)
  (set! *hotel-space* 128)

  (receive (hotel-start hotel-end)
    (allocate-hotel-space *hotel-space*) 

    (receive (message-start current-end)
      (allocate-message-space *message-space*)

      (set! *our-address-space* address-space)
      (set! *message-start* message-start)
      ;; add space for header, number of bytes, and THING
      (set! *transmit-hp* (address+ message-start (cells->a-units 3)))
      ;; ADDRESS- accounts for room for the header of the last object
;      (set! *max-hp* (address- current-end
;			       (cells->a-units 1)))
;      (set! *max-hp* (address- (let ((end (address+ start-hp
;						    (cells->a-units
;						     max-stob-size-in-cells))))
;				 (if (address< end current-end)
;				     end
;				     current-end))
;			       (cells->a-units 1)))
      (set! *hotel-p* hotel-start)
      (set! *heartbreak-hotel* null-address)
      (set! *new-id-hotel* null-address)
      (set! *losing-proxy-hotel* null-address)
      (let ((code (encode-object thing)))
	(store! (address+ message-start (cells->a-units 2)) code)
	(cond ((or (address>= *transmit-hp* *message-end*)
		   (not (do-encoding (address+ message-start (cells->a-units 3)))))
	       (mend-hearts! hotel-start)
	       (drop-new-ids! hotel-start)
	       (space->byte-vector *message-start* *message-end*) ;; chnx heap
	       (space->byte-vector *hotel-start* *hotel-end*) ;; chnx heap
	       #f)
	      (else
	       (let ((result (make-message-vector message-start)))
		 (store! (address+ message-start (cells->a-units 1))
			 (address-difference *transmit-hp* (address1+ message-start)))
		 ;(s48-set-heap-pointer! *transmit-hp*) ;; chnx heap
		 (mend-hearts! hotel-start)
		 (if (update-decode-vectors! address-space hotel-start)
		     (let ((losers (get-losing-proxies)))
		       (space->byte-vector *hotel-start* *hotel-end*) ;; chnx heap
		       (if (false? losers)
			   #f
			   (begin
			     ;(debug-message "encode done!!!")
			     (vm-set-car! pair result)
			     (vm-set-cdr! pair losers)
			     #t)))
		     (begin
		       (drop-new-ids! hotel-start)
		       (space->byte-vector *hotel-start* *hotel-end*)
		       #f)))))))))
     
(define *message-space*) ;; chnx heap
(define *hotel-space*) ;; chnx heap
(define *message-start*) ;;chnx heap
(define *message-end*) ;; chnx heap
(define *hotel-start*) ;; chnx heap
(define *hotel-end*) ;; chnx heap

(define *transmit-hp*)
;(define *max-hp*) ;; -> message-end
;(define *start*) ;; -> message-start

(define (make-message-vector start)
  ;(debug-message "make-message-vector")
  (let ((size (address-difference *transmit-hp* (address1+ start))))
    (store! start (make-header (enum stob byte-vector) size))
    (space->byte-vector *transmit-hp* *message-end*) ;; chnx heap
    (address->stob-descriptor (address1+ start))))

(define (encoding-lost!)
  ;(debug-message "encoding-lost!")
  (set! *transmit-hp* *message-end*)
  0)   ; stored in encoded vector, which will never be used

;----------------
; Aspace decode vectors have two distinguished entries: the next index on the
; free list and the address space itself.

(define freelist-index 0)
(define aspace-id-in-self 1)

;----------------
; We use the currently unused half of the heap for various lists.

(define *hotel-p*)

(define (alloc-list-elt! cells old)
  ;(debug-message "alloc-list-elt!")
  (let ((start *hotel-p*))
    (store! (address+ start (cells->a-units cells))
	    (address->integer old))
    (set! *hotel-p* (address+ start (cells->a-units (+ cells 1))))
    start))

(define (walk-list start cells proc)
  ;(debug-message "walk-list")
  (do ((ptr start (list-elt-next ptr cells)))
      ((null-address? ptr))
    (proc ptr)))

(define (reduce-list start cells data proc)
  ;(debug-message "reduce-list")
  (do ((ptr start (list-elt-next ptr cells))
       (data data (proc ptr data)))
      ((null-address? ptr)
       data)))

(define (list-elt-next ptr cells)
  ;(debug-message "list-elt-nect")
  (integer->address (fetch (address+ ptr (cells->a-units cells)))))

;----------------
; A list of the copied objects.  This is used to restore the objects' headers
; when the message is complete.  Each element has the object and the object's
; original header.

(define *heartbreak-hotel*)

(define (remember-heartbreak thing header)
  ;(debug-message "remember-heartbreak")
  (let ((room-number (alloc-list-elt! 2 *heartbreak-hotel*)))
    (store! room-number thing)
    (store! (address1+ room-number) header)
    (set! *heartbreak-hotel* room-number)))

(define (mend-hearts! start)
  ;(debug-message "mend-hearts!")
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
  ;(debug-message "next-id")
  (real-next-id thing (address-space-decode-vector *our-address-space*)))

(define (next-proxy-id thing)
  ;(debug-message "next-proxy-id")
  (real-next-id thing (address-space-proxy-vector *our-address-space*)))

; Get the next uid from the decode vector's freelist and either add THING
; to the decode vector or put it in the *NEW-ID-HOTEL*.

(define (real-next-id thing decode-vector)
  ;(debug-message "real-next-id")
  (let* ((next-available (vm-vector-ref decode-vector freelist-index))
	 (extracted (extract-fixnum next-available))
	 (next (if (< extracted (vm-vector-length decode-vector))
		   (let ((next (vm-vector-ref decode-vector extracted)))
		     (vm-vector-set! decode-vector extracted thing)
		     next)
		   (let ((room-number (alloc-list-elt! 1 *new-id-hotel*)))
		     (store! room-number thing)
		     (set! *new-id-hotel* room-number)
		     (enter-fixnum (+ extracted 1))))))
    (vm-vector-set! decode-vector freelist-index next)
    next-available))

; Set the UID slots of objects that just received new UID's back to false.
; Used when we don't have room to add the new objects to the decode vector.

(define (drop-new-ids! start)
  ;(debug-message "drop-new-ids!")
  (walk-list *new-id-hotel*
	     1
	     (lambda (ptr)
	       (let ((thing (fetch ptr)))
		 (enum-case stob (header-type (stob-header thing))
		   ((symbol)
		    (vm-set-symbol-uid! thing false))
		   ((template)
		    (set-template-uid! thing false))
		   ;;((external)                        ;; taken out - there are no 
		   ;; (set-external-uid! thing false))  ;; external-stobs in scheme48-1.4T
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
  ;(debug-message "update-decode-vectors!")
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
  ;(debug-message "extend-decode-vector")
  (let* ((decode-vector (if proxies?
			    (address-space-proxy-vector address-space)
			    (address-space-decode-vector address-space)))
	 (old-length (vm-vector-length decode-vector))
	 (next-uid (extract-fixnum (vm-vector-ref decode-vector freelist-index)))
	 (minimum-new-length next-uid)
	 (want-length (max (* old-length 2) minimum-new-length))
	 (new-length (cond ((s48-available? (vm-vector-size want-length))
			    want-length)
			   ((s48-available? (vm-vector-size minimum-new-length))
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
  ;(debug-message "setup-new-decode-vector!")
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
  ;(debug-message "do-encoding")
  (let loop ((start start))
    (let ((end *transmit-hp*))
      (encode-locations start end)
      (cond ((address>= *transmit-hp* *message-end*) ; lost
	     #f)                   
	    ((address< end *transmit-hp*)       ; still playing
	     (loop end))
	    (else #t)))))           ; won

; Encode everything pointed to from somewhere between START and END.

(define (encode-locations start end)
  ;(debug-message "encode-locations")
  (let loop ((addr start))
    (if (address< addr end)
	(loop (encode-next addr)))))

; Encode the thing pointed to from ADDR, returning the next address to copy.

(define (encode-next addr)
  ;(debug-message "encode-next")
  (let ((thing (fetch addr))
	(next (address1+ addr)))
    (cond ((b-vector-header? thing)
	   (address+ next (header-a-units thing)))
          ((stob? thing)
	   (store! addr (encode-object thing))
	   next)
          (else
	   next))))

; Encode THING if it has not already been encoded.

(define (encode-object thing)
  ;(debug-message "encode-object")
  (let ((h (stob-header thing)))
    (if (stob? h)            ;***Broken heart
	h
	(enum-case stob (header-type h)
	  ((symbol)
	   ;(debug-message "[symbol]")
	   (make-element (enum element uid)
			 (extract-fixnum
			  (get-uid thing vm-symbol-uid vm-set-symbol-uid!))))
;;	  ((external)                                                        ;; taken out -
;;	   (make-element (enum element uid)                                  ;; there are no external-stobs
;;			 (extract-fixnum                                     ;; in
;;			  (get-uid thing external-uid set-external-uid!))))  ;; scheme48-1.4T
	  ((address-space)
	   ;(debug-message "[address-space]") 
	   (make-element (enum element uid)
			 (extract-fixnum
			  (get-uid thing address-space-uid set-address-space-uid!))))
	  ((template)
	   ;(debug-message "[template]")
	   (encode-two-part-uid
	     (get-uid thing template-uid set-template-uid!)))
	  ((location)
	   ;(debug-message "[location]")
	   (encode-two-part-uid
	      (get-uid thing location-uid set-location-uid!)))
	  ((proxy)
	   ;(debug-message "[proxy]")
	   (if (address<= *message-end*
			  (address+ *transmit-hp* (cells->a-units 3)))
	       (encoding-lost!)
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
	   ;(debug-message "[else]")
	   (encode-full-object h thing))))))

(define (encode-two-part-uid uid)
  ;(debug-message "encode-two-part-uid")
  (if (address<= *message-end*
		 (address+ *transmit-hp* (cells->a-units 2)))
      (encoding-lost!)
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
  ;(debug-message "encode-full-object")
  (if (address<= *message-end*
		 (address+ *transmit-hp* (header-a-units header)))
      (encoding-lost!)
      (let ((new (address->message-offset (enum element local))))
	(store-next! header)
	(stob-header-set! thing new) ;***Break heart
	(remember-heartbreak thing header)
	(let ((new-hp (address+ *transmit-hp* (header-a-units header))))
	  (do ((o (address-after-header thing) (address1+ o)))
	      ((address>= *transmit-hp* new-hp))
	    (store-next! (fetch o))))
	new)))

(define (address->message-offset type)
  ;(debug-message "address->message-offset")
  (make-element type (address-difference *transmit-hp* *message-start*)))

; Utility for getting uids.

(define (get-uid thing accessor setter)
  ;(debug-message "get-uid")
  (let ((uid (accessor thing)))
    (if (false? uid)
	(let ((uid (next-id thing)))
	  (setter thing uid)
	  uid)
	uid)))

(define (get-proxy-uid thing)
  ;(debug-message "get-proxy-uid")
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
  ;(debug-message "debit-proxy-count!")
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
  ;(debug-message "get-losing-proxies")
  (reduce-list *losing-proxy-hotel*
	       1     ; cells
	       null  ; initial result list
	       (lambda (ptr result)
		 (if (s48-available? vm-pair-size)
		     (vm-cons (fetch ptr)
			      result
			      (ensure-space vm-pair-size))
		     false))))

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
  (let ((data (element-info thing)))
    (enum-case element (element-type thing)
      ((local)
       ;;(debug-message "[local]")
       (address->stob-descriptor (address+ *message-start* data)))
      ((uid)
       ;;(debug-message "[uid]")
       (decode-uid data aspace aspace-id-in-self addr stob-start key))
      ((uid+owner)
       ;;(debug-message "[uid+owner]")
       (receive (aspace-uid uid)
	   (get-full-uid-data data)
	 (let ((aspace (lookup-uid aspace-uid aspace)))
	   (if (address-space? aspace)
	       (decode-uid uid aspace aspace-uid addr stob-start key)
	       (add-pending! aspace-uid uid addr stob-start key)))))
      (else   ; (proxy)   ; Type checker can't handler missing ELSE clause
       ;;(debug-message "[proxy] - (else)")
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
  (let ((proxy-vector (address-space-proxy-vector aspace)))
    (if (< uid (vm-vector-length proxy-vector))
	(let ((thing (vm-vector-ref proxy-vector uid)))
	  (if (proxy-data? thing)
	      (let ((proxy (weak-pointer-ref (proxy-data-self thing))))
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
  (let ((count (extract-fixnum (proxy-data-reference-count proxy-data))))
    (if (< count 1)
	(set! *bad-count-proxies*
	      (vm-cons (vm-cons proxy-data more key)
		       *bad-count-proxies*
		       key))
	(begin
	  (if (and (< max-proxy-count (+ count more))
		   (not (false? (proxy-data-owner proxy-data))))
	      (set! *bad-count-proxies*
		    (vm-cons proxy-data
			     *bad-count-proxies*
			     key)))
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
  (really-add-pending! aspace-uid uid false addr stob-start key))

(define (add-pending-proxy! aspace-uid uid count addr stob-start key)
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
  (store! *transmit-hp* descriptor)
  (set! *transmit-hp* (address1+ *transmit-hp*)))

; The following are also in gc.  Perhaps they should be in data.

(define (header-a-units h)
  (bytes->a-units (header-length-in-bytes h)))

(define (address-at-header stob)
  (address- (address-after-header stob) (cells->a-units 1)))

(define (address-after-stob stob)
  (address+ (address-after-header stob)
	    (bytes->a-units (header-length-in-bytes (stob-header stob)))))


;; --------------------
;; allocate memory for the "hotels" and message-vector
;; getting rid of the unused half of the heap...

(define (allocate-message-space message-space)
  (let* ((message-start
	  (s48-allocate-untraced+gc message-space))
	 (message-end
	  (address+ message-start
		    (bytes->a-units message-space))))
    (set! *message-start* message-start)
    (set! *message-end* message-end)
    (values message-start
	    message-end)))

(define (clean-up-message-space)
  (let ((h (make-header (enum stob byte-vector)
			0)))
    (let lp ((start *transmit-hp*));(address- *transmit-hp*
			;      (cells->bytes 1))))
      (if (address= start *message-end*)
	  (unspecific)
	  (begin
	    (store! start h)
	    (lp (address+ start
			  (cells->a-units 1))))))))

(define (kill-message-space)
  #t)

(define (allocate-hotel-space hotel-space)
  (let* ((hotel-start
	  (s48-allocate-untraced+gc hotel-space))
	 (hotel-end
	    (address+ hotel-start
		      (bytes->a-units hotel-space))))
    (set! *hotel-start* hotel-start)
    (set! *hotel-end* hotel-end)
    (values hotel-start
	    hotel-end)))
	 
(define (clean-up-hotel-space)
  (kill-hotel-space))

(define (kill-hotel-space)
  (let ((h (make-header (enum stob byte-vector)
			0)))
    (let lp ((start *hotel-start*))
      (if (address= start *hotel-end*)
	  (unspecific)
	  (begin
	    (store! start h)
	    (lp (address+ start
			  (cells->a-units 1))))))))

(define (space->byte-vector start end)
  (let ((h (make-header (enum stob byte-vector)
			(- (address-difference end start)
			   (cells->a-units 1)))))
    (store! start h)
    (unspecific)))

;; -------------------

(define (debug-message str)
  (write-out-string str)
  (write-out-newline))

