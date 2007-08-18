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
			  (address+ next (header-length-in-a-units value))
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
	  (loop (address+ next (header-length-in-a-units thing)))))))

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
       ;;;(debug-message 3 "[local]")
       (address->stob-descriptor (address+ *message-start* data)))
      ((uid)
       ;;;(debug-message 3 "[uid]")
       (decode-uid data aspace aspace-id-in-self addr stob-start key))
      ((uid+owner)
       ;;;(debug-message 3 "[uid+owner]")
       (receive (aspace-uid uid)
	   (get-full-uid-data data)
	 (let ((aspace (lookup-uid aspace-uid aspace)))
	   (if (address-space? aspace)
	       (decode-uid uid aspace aspace-uid addr stob-start key)
	       (add-pending! aspace-uid uid addr stob-start key)))))
      (else   ; (proxy)   ; Type checker can't handler missing ELSE clause
       ;;;(debug-message 3 "[proxy] - (else)")
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
