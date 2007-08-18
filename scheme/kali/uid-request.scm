; All of this file is only for Kali.

; Interface:
;   (WAIT-FOR-MISSING-UIDS missing-uids thunk from-aspace)
;   (PROCESS-UID-REPLIES replies missing-uids from-aspace)
;   (MAKE-UID-REPLY request)

; This code patches up messages that arrive with references to UIDs for which
; we do not have values.  The sequence of events is as follows:
;
;   1. The DECODE instruction uses the decode vectors in address spaces to
;      get the values of <uid>s.  The instruction returns both the decoded
;      message and a list of vectors:
;
;              #(<address-space-uid> <uid> <count-or-#f> <thing> <offset>)
;
;      The meaning is that the object identified as <uid> in the indicated
;      address space needs to be put at <offset> in <thing> but the object
;      is not in the address space's decode vector (or the address space
;      itself is not present).  <Address-space-uid> is the <uid> used for
;      the relevent address in the decode vector of the address space that
;      sent the message.
;
;   2. For each missing object we construct a <missing-object> record that
;      holds the relevent <thing>s and <offset>s.  These records is stored
;      in a second vector in address spaces, the pending vector.  If a
;      <missing-object> is already in place for a given <uid>, then there
;      was an earlier reference and the object has already been requested.
;      The current message still needs to wait until the value arrives.
;
;   3. A request is sent back to the sending address space asking for the
;      value of any missing <uid> for which there is not already a <missing-
;      object> record.
;
;      For each set of <uid> requests sent out a <pending-request> record is
;      created.  The <pending-request> contains threes lists: the <uid>s
;      requested, the <missing-object>s for those <uid>s, and the messages
;      that are waiting for those objects.
;
;      A thunk to execute the message is placed in a <pending-message> record
;      along with a count of the number of <pending-requests> that message
;      has to wait for.
;
;   4. When the reply to the request arrives we construct the objects
;      for which it has values, store them into the <thing>s, and decrement
;      the counts in the <pending-message>s waiting for those objects.  If
;      any <pending-message>'s count reaches zero the message's thunk is
;      called.
;
; This can go wrong in two ways: we may not know the meaning of the
; <address-space-uid> reported by the DECODE isntruction, and a reply to
; a <uid> request may itself have missing <uid>s.
;
; If an address space is missing we go through steps 1 through 4 twice, the
; first time to get the address space and then again to get any missing <uid>s.
; It may be the case that we already had the address space, but didn't know the
; senders' <uid> for it, and thus the second pass may be unnecessary.
;
; When an <uid> reply arrives that itself has missing <uid>s, we cannot release
; the messages in the corresponding <pending-request>.  Instead we do the
; following:
;
;   1. For each <missing-object> in the <pending-request>, use the reply to
;      create the object and store it in any holes.  If the object is atomic,
;      such as <symbol> or <external>(*), then remove the <missing-object> from
;      the pending vector and put the object into the decode vector.  If the
;      object is not atomic, then replace the <missing-object> record with a
;      a <skeleton> record containing the (incomplete) object and a thunk that
;      will finish the object once the needed <uid>s are known.
;
;      (*) not <external> any more, since threre is no... 
;
;   2. Create a <closet> record to replace the original <pending-request>.
;      The <closet> contains three lists: the <skeleton>s created in step 1,
;      the messages from the <pending-request>, and a list of the <pending-
;      requests> that must arrive before the objects will be complete.  The
;      <pending-request>s list is initialized to be the <pending-request>s
;      that will have the values of the <uid>s missing from the <uid> reply.
;
;      <Pending-request>s now contain a list of the <closet>s waiting for them.
;
;      When a message arrives it may need a <uid> whose current value is a
;      <skeleton>. The incomplete value from the <skeleton> is placed in the
;      relevent slot and the <pending-message> is added to the <skeleton>'s
;      <closet>'s list of waiting messages.  No <missing-object> or <hole>
;      is needed.
;
;   3. Send out the requests and create a <pending-request> as in step 3
;      above.  This <pending-request> is added to the list in the <closet>.
;      just created.
;
;   4. When a reply arrives we walk through the waiting <closet>s in the
;      <pending-request> and in each we delete that <pending-request> from the
;      list for which the <closet> is waiting.  If the values in the reply have
;      to wait for other <pending-request>s, those <pending-request>s are added
;      to the lists in the <closet>s.  This allows the system to handle
;      circularities in objects with <uid>s.
;
;      When a <closet>s list of <pending-request>s is empty, we call the
;      finishing thunks of its <skeleton>s and decrement the counts in its
;      <pending-message>s.

(define (wait-for-missing-uids missing-uids thunk from-aspace)
  (process-missing-uids missing-uids from-aspace
    (lambda (pending-requests requests)
      (if (null? pending-requests)
	  (thunk)
	  (make-pending-message thunk pending-requests))
      (send-uid-requests requests from-aspace))))

(define (process-uid-replies replies missing-uids from-aspace)
  (process-missing-uids missing-uids from-aspace
    (lambda (pending-requests requests)
      (let* ((our-objects (map (lambda (reply)
				 (uid-reply->missing-object reply from-aspace))
			       replies))
	     (our-pending (missing-object-pending-request (car our-objects)))
	     (interrupts (disable-interrupts!)) ; preserve all these data structs
	     (waiting-for (delete our-pending pending-requests))
	     (skeletons (make-reply-objects replies
					    our-objects
					    from-aspace
					    (null? waiting-for)))
	     (ready (if (null? skeletons)
			(find-ready our-pending)
			(begin
			  (advance-closets! our-pending waiting-for)
			  (make-closet (pending-request-requests our-pending)
				       skeletons
				       (pending-request-messages our-pending)
				       waiting-for)
			  '()))))
	(enable-interrupts! interrupts)
	(if (null? skeletons)
	    (for-each (lambda (message)
			((pending-message-thunk message)))
		      ready)
	    (send-uid-requests requests from-aspace))))))

; Use the uids from the reply to get the missing object.

(define (uid-reply->missing-object reply from-aspace)
  (vector-ref (address-space-pending-vector
	         (uid-reply-aspace reply from-aspace))
	      (uid-reply-uid reply)))

; Use data from REPLIES to make values and possibly skeletons for MISSING-
; OBJECTS.  For each missing object we make the value store it in the
; missing-object's holes.
;
; If NO-WAIT? is false and the object is not atomic we create a skeleton
; and install it in the pending vector.

(define (make-reply-objects replies missing-objects from-aspace no-wait?)
  (fold2 replies
	 missing-objects
	 (lambda (reply missing-object skeletons)
	   (receive (value finish!)
	       (make-uid-object reply from-aspace)
	     (for-each (lambda (hole)
			 (fill-hole! hole value))
		       (missing-object-holes missing-object))
	     (let ((install! (lambda ()
			       (if finish! (finish!))
			       (install-value! from-aspace reply value))))
	       (cond ((or no-wait?
			  (not finish!))
		      (install!)
		      skeletons)
		     (else
		      (let ((skeleton (make-skeleton value install!)))
			(install-pending-value! from-aspace reply skeleton)
			(cons skeleton skeletons)))))))
	 '()))

; Putting values in the decode and pending vectors of address spaces.

(define (install-value! from-aspace uid-reply value)
  (let ((aspace (uid-reply-aspace uid-reply from-aspace))
	(uid (uid-reply-uid uid-reply)))
    (vector-set! (address-space-decode-vector aspace) uid value)
    (vector-set! (address-space-pending-vector aspace) uid #f)))

(define (install-pending-value! from-aspace uid-reply value)
  (let ((aspace (uid-reply-aspace uid-reply from-aspace)))
    (vector-set! (address-space-pending-vector aspace)
		 (uid-reply-uid uid-reply)
		 value)))

;----------------
; Make sure that we have all of the address spaces mentioned in MISSING-UIDS
; and then call THUNK.  If there are any we don't have we make our own
; MISSING-UID records for them and start again from the top.

(define (get-address-spaces missing-uids from-aspace thunk)
  (let* ((interrupts (disable-interrupts!))
	 (missing-aspaces (find-missing-address-spaces missing-uids from-aspace)))
    (enable-interrupts! interrupts)
    (if (null? missing-aspaces)
	(thunk)
	(wait-for-missing-uids (map make-aspace-missing-uid missing-aspaces)
			       thunk
			       from-aspace))))

; Return the missing-object records for any address spaces needed for
; MISSING-UIDS along with any requests that need to be sent.

(define (find-missing-address-spaces missing-uids from-aspace)
  (let ((decode (address-space-decode-vector from-aspace)))
    (fold missing-uids
	  (lambda (missing-uid missing-aspaces)
	    (let ((aspace-uid (missing-uid-aspace-uid missing-uid)))
	      (if (or (memq aspace-uid missing-aspaces)
		      (and (< aspace-uid (vector-length decode))
			   (vector-ref decode aspace-uid)))
		  missing-aspaces
		  (cons aspace-uid missing-aspaces))))
	  '())))

;----------------
; This uses SEND-ADMIN-MESSAGE to avoid deadlock.

(define (send-uid-requests requests aspace)
  ; (apply debug-message "requests " requests)
  (if (not (null? requests))
      (send-admin-message (enum message-type uid-request)
			  requests
			  aspace)))

;----------------
; Get the pending-requests that we need to wait for, along with any requests
; that have to be sent.  If there are requests, we make a new pending-request
; to catch the results.

(define (process-missing-uids missing-uids from-aspace proc)
  (get-address-spaces missing-uids from-aspace
    (lambda ()
      (let ((interrupts (disable-interrupts!))) ;maintain shared data's integrity
	(call-with-values
	 (lambda ()
	   (really-find-missing-uids missing-uids from-aspace))
	 (lambda (pending-requests requests)
	   (enable-interrupts! interrupts)
	   (proc (if (null? requests)
		     pending-requests
		     (cons (make-pending-request requests from-aspace)
			   pending-requests))
		 requests)))))))

; Gather two lists, pending-requests and requests.  In order to detect duplicates
; efficiently, KEY is used to mark each closet and pending-request as it is
; found.

(define (really-find-missing-uids missing-uids from-aspace)
  (let ((key (cons 'key 'key)))
    (fold->2 missing-uids
	     (lambda (missing-uid pending-requests requests)
	       (receive (pending-requests request)
		   (process-missing-uid missing-uid
					from-aspace
					pending-requests
					key)
		 (values pending-requests
			 (if request
			     (cons request requests)
			     requests))))
	     '()
	     '())))

; Look up MISSING-UID and fill in a value if possible.  This returns a possibly
; extended value of SO-FAR and either a request or #f.  All current members of
; SO-FAR are marked with KEY.
; The options are:
; 1. We already have a value (we just located the address space or it showed up
;    after the message was decoded).
; 2. The missing thing is a proxy, in which case we just make it.
; 3. There is a skeleton for the object, so we stick the skeleton in place
;    and return the missing-objects that are needed to fill it in.
; 4. There is already a missing-object record.  We add a hole for MISSING-UID
;    and add the object to the list.
; 5. We need to make a missing object record.  This is the only time we make a
;    request.

(define (process-missing-uid missing-uid from-aspace so-far key)
  (let ((aspace (missing-uid-aspace missing-uid from-aspace))
	(uid (missing-uid-uid missing-uid)))
    (maybe-extend-decode-vectors! aspace uid)
    (cond ((missing-uid-is-proxy? missing-uid)
	   (fill-missing-uid! (find-missing-proxy missing-uid aspace)
			      missing-uid)
	   (values so-far #f))
	  ((vector-ref (address-space-decode-vector aspace) uid)
	   => (lambda (thing)
		(fill-missing-uid! thing missing-uid)
		(values so-far #f)))
	  ((vector-ref (address-space-pending-vector aspace) uid)
	   => (lambda (partial)
		(values (if (skeleton? partial)
			    (process-skeleton partial missing-uid key so-far)
			    (process-missing-object partial missing-uid key so-far))
			#f)))
	  (else
	   (let ((missing (make-missing-object)))
	     (add-missing-object-hole! missing missing-uid)
	     (vector-set! (address-space-pending-vector aspace)
			  uid
			  missing)
	     (values so-far
		     (cons (missing-uid-aspace-uid missing-uid)
			   uid)))))))

; We have a skeleton for MISSING-UID, so we fill in the (incomplete) value
; and add any new pending-requests to SO-FAR.

(define (process-skeleton skeleton missing-uid key so-far)
  (let ((closet (skeleton-closet skeleton)))
    (fill-missing-uid! (skeleton-value skeleton) missing-uid)
    (if (eq? key (closet-key closet))
	so-far
	(begin
	  (set-closet-key! closet key)
	  (key-append (closet-pending-requests closet)
		      so-far
		      key)))))
  
; We have a missing-object for MISSING-UID, so we add a hole to the
; missing-object and its pending-request to SO-FAR if it isn't already there.
; If the missing object were just found it won't have a pending-request yet.

(define (process-missing-object missing-object missing-uid key so-far)
  (add-missing-object-hole! missing-object missing-uid)
  (if (missing-object-pending-request missing-object)
      (key-cons (missing-object-pending-request missing-object)
		so-far
		key)
      so-far))

; Make the missing proxy if we don't already have it.

(define (find-missing-proxy missing-uid aspace)
  (let ((uid (missing-uid-uid missing-uid)))
    (maybe-extend-proxy-vector! aspace uid)
    (let ((proxy-data (vector-ref (address-space-proxy-vector aspace) uid)))
      (if proxy-data
	  (begin
	    (set-proxy-data-reference-count! proxy-data
					     (+ (missing-uid-count missing-uid)
						(proxy-data-reference-count proxy-data)))
	    (proxy-data->proxy proxy-data))
	  (make-missing-proxy missing-uid aspace)))))

; Make a missing proxy and put it into the appropriate decode vector.

(define (make-missing-proxy missing-uid aspace)
  (let* ((uid (missing-uid-uid missing-uid))
	 (new (make-nonlocal-proxy aspace
				   uid
				   (missing-uid-count missing-uid))))
    (vector-set! (address-space-proxy-vector aspace) uid (proxy-data new))
;    (debug-message " filling proxy " uid " with " new)
    new))

;----------------
; This really should be coordinated via the architecture file.  The missing-uid
; vectors are made by the decode instruction.

(define (missing-uid-aspace-uid missing-uid) (vector-ref missing-uid 0))
(define (missing-uid-uid        missing-uid) (vector-ref missing-uid 1))
(define (missing-uid-count      missing-uid) (vector-ref missing-uid 2))
(define (missing-uid-stob       missing-uid) (vector-ref missing-uid 3))
(define (missing-uid-offset     missing-uid) (vector-ref missing-uid 4))

(define missing-uid-is-proxy? missing-uid-count)

(define (make-aspace-missing-uid aspace-uid)
  (vector 1 aspace-uid #f (vector #f) 0))

(define (missing-uid-aspace missing-uid from-aspace)
  (vector-ref (address-space-decode-vector from-aspace)
	      (missing-uid-aspace-uid missing-uid)))

(define (fill-missing-uid! thing missing-uid)
  (untyped-indexed-set! (missing-uid-stob missing-uid)
			(missing-uid-offset missing-uid)
			thing))

;----------------
; A pending message is one that is waiting for various uid objects to arrive.
; The THUNK is the message and COUNT is the number of objects that must arrive.
; As each arrives the count is decremented.  When it reaches zero the thunk is
; called.  If only it were all this simple.

(define-record-type pending-message :pending-message
  (really-make-pending-message thunk count)
  pending-message?
  (thunk pending-message-thunk)
  (count pending-message-count set-pending-message-count!))

(define (make-pending-message thunk pending-requests)
  (let ((message (really-make-pending-message thunk (length pending-requests))))
    (for-each (lambda (pending-request)
		(set-pending-request-messages!
		   pending-request
		   (cons message
			 (pending-request-messages pending-request))))
	      pending-requests)))

(define-record-discloser :pending-message
  (lambda (pm)
    (list 'pending-message (pending-message-thunk pm))))

(define (decrement-messages! messages)
  (fold messages
	(lambda (message ready)
	  (let ((count (pending-message-count message)))
	    (if (= 1 count)
		(cons message ready)
		(begin
		  (set-pending-message-count! message (- count 1))
		  ready))))
	'()))

;----------------
; A missing object has two parts:
;  - holes in which to put the value when it is made
;  - the pending-request that will have the value of this message

(define-record-type missing-object :missing-object
  (really-make-missing-object holes pending-request)
  missing-object?
  (holes missing-object-holes set-missing-object-holes!)
  (pending-request missing-object-pending-request set-missing-object-pending-request!))

(define (make-missing-object)
  (really-make-missing-object '() #f))

; A place to put a value once it arrives.

(define-record-type hole :hole
  (make-hole stob offset)
  hole?
  (stob hole-stob)
  (offset hole-offset))

(define (add-missing-object-hole! missing-object missing-uid)
  (set-missing-object-holes! missing-object
			     (cons (make-hole (missing-uid-stob missing-uid)
					      (missing-uid-offset missing-uid))
				   (missing-object-holes missing-object))))

(define (fill-hole! hole thing)
  (untyped-indexed-set! (hole-stob hole)
			(hole-offset hole)
			thing))

;----------------

(define-record-type pending-request :pending-request
  (really-make-pending-request requests messages closets)
  pending-request?
  (requests pending-request-requests)   ; debugging
  (messages pending-request-messages set-pending-request-messages!)
  (closets pending-request-closets set-pending-request-closets!)
  (key pending-request-key set-pending-request-key!))

(define-record-discloser :pending-request
  (lambda (var)
    (list 'pending-request (pending-request-requests var))))

(define (make-pending-request requests from-aspace)
  (let ((pending-request (really-make-pending-request requests '() '())))
    (for-each (lambda (request)
		(set-missing-object-pending-request!
		  (request->missing-object request from-aspace)
		  pending-request))
	      requests)
    pending-request))

(define (request->missing-object request from-aspace)
  (let ((aspace (vector-ref (address-space-decode-vector from-aspace)
			    (car request))))
    (vector-ref (address-space-pending-vector aspace)
		(cdr request))))

(define (add-pending-request-closet! pending-request closet)
  (set-pending-request-closets! pending-request
			      (cons closet
				    (pending-request-closets pending-request))))

; The closets that were waiting for PENDING-REQUEST now have to wait for the
; pending-requests in WAITING-FOR instead.

(define (advance-closets! pending-request waiting-for)
  (for-each (lambda (closet)
	      (let* ((old (delete pending-request (closet-pending-requests closet)))
		     (new (difference waiting-for old)))
		(for-each (lambda (pending-request)
			    (add-pending-request-closet! pending-request closet))
			  new)
		(set-closet-pending-requests! closet (append new old))))
	    (pending-request-closets pending-request)))

; This reply has been completed, so find any closets and messages that can
; be released.

(define (find-ready pending-request)
  (fold (pending-request-closets pending-request)
	(lambda (closet messages)
	  (let ((waiting-for (delete pending-request (closet-pending-requests closet))))
	    (if (null? waiting-for)
		(append (finish-closet closet)
			messages)
		messages)))
	(decrement-messages! (pending-request-messages pending-request))))

; Various set manipulation utilities for pending replies, all of which use
; keys to maintain the sets.  The first two depend on a list already having
; been marked with a particular key.

(define (key-cons pending-request list key)
  (if (eq? key (pending-request-key pending-request))
      list
      (begin
	(set-pending-request-key! pending-request key)
	(cons pending-request list))))

; LIST2 has already been marked with KEY.

(define (key-append list1 list2 key)
  (do ((list1 list1 (cdr list1))
       (res list2 (key-cons (car list1) list2 key)))
      ((null? list1)
       res)))

(define (mark-pending-requests! pending-requests key)    
  (for-each (lambda (object)
	      (set-pending-request-key! object key))
	    pending-requests))

(define (difference missing1 missing2)
  (let ((key (cons 1 2)))
    (mark-pending-requests! missing2 key)
    (fold missing1
	  (lambda (pending-request list)
	    (key-cons pending-request list key))
	  '())))

; Only deletes the first entry, but that is all we need.

(define (delete thing list)
  (cond ((null? list)
	 list)
	((eq? thing (car list))
	 (cdr list))
	(else
	 (cons (car list)
	       (delete thing (cdr list))))))

;----------------
; A skeleton is a uid-object that has arrived but whose value has missing uids.
; Each has a list of messages waiting for the skeleton to be completed, a
; list of objects whose values will complete the skeleton, and a thunk to
; finish the value once the objects have arrived.
;
; The object list is really the transitive closure of the necessary objects.
; If one of the objects arrives but has its own missing objects, the newly
; arrived object is removed from all skeleton lists and replace by the
; missing ones.

(define-record-type skeleton :skeleton
  (make-skeleton value finish)
  skeleton?
  (value skeleton-value set-skeleton-value!)
  (closet skeleton-closet set-skeleton-closet!)
  (finish skeleton-finish)) ; call this thunk when there are no more missing objects

;----------------
; A closet is a collection of skeletons that are waiting for the same
; uid-replies (because they were all originally requested at the same time).

(define-record-type closet :closet
  (really-make-closet requests skeletons messages pending-requests)
  closet?
  (requests closet-requests)   ; debugging
  (skeletons closet-skeletons set-closet-skeletons!)
  (messages closet-messages)
  (pending-requests closet-pending-requests set-closet-pending-requests!)
  (key closet-key set-closet-key!))

(define-record-discloser :closet
  (lambda (var)
    (list 'closet (closet-requests var))))

(define (make-closet requests skeletons messages pending-requests)
  (let ((closet (really-make-closet requests skeletons messages pending-requests)))
    (for-each (lambda (pending-request)
		(add-pending-request-closet! pending-request closet))
	      pending-requests)
    (for-each (lambda (skeleton)
		(set-skeleton-closet! skeleton closet))
	      skeletons)
    closet))

(define (finish-closet closet)
;  (debug-message "finishing " closet)
  (for-each (lambda (skeleton)
	      ((skeleton-finish skeleton)))
	    (closet-skeletons closet))
  (decrement-messages! (closet-messages closet)))

;----------------
; A type dispatch to make an object once the data for it arrives.  For locations
; and template the object is made and filled in as two separate operations.  The
; filling in may have to wait until other objects have arrived.

(define (make-uid-object reply from-aspace)
  (let ((data (uid-reply-data reply))
	(uid  (uid-reply-uid  reply))
	(aspace-uid (address-space-uid (uid-reply-aspace reply from-aspace))))
    (enum-case stob (uid-reply-type reply)
      ((symbol)
       (values (string->symbol data)
	       #f))
 ;     ((external)     ;; no external in scheme 1.4
 ;      (error "don't know how to send externals yet"))
      ((address-space)
       (values (socket-id->address-space (car data) (cdr data))
	       #f))
      ((template)
       (let ((template (make-template (vector-length data) 0)))
	 (values template
		 (lambda ()
;		   (debug-message "finishing template " (cons aspace-uid uid))
		   (do ((i 0 (+ i 1)))
		       ((= i (vector-length data)))
		     (template-set! template i (vector-ref data i)))
		   (set-template-id! template (cons aspace-uid uid))))))
      ((location)
       (let ((location (make-undefined-location #f))) ; no ID
	 (values location
		 (lambda ()
;		   (debug-message "finishing location " (cons aspace-uid uid)
;			  " with " data)
		   (set-contents! location data)
		   (set-location-uid! location (cons aspace-uid uid))))))
      (else
       (error "bad stob type in MAKE-UID-OBJECT" (uid-reply-type reply))))))
  
;----------------
; Make ASPACE's decode and pending vectors large enough to hold UID.  The decode
; vector may be larger than the pending vector.

(define (maybe-extend-decode-vectors! aspace uid)
  (let ((old-decode (address-space-decode-vector aspace))
	(old-pending (address-space-pending-vector aspace)))
    (if (>= uid (vector-length old-decode))
	(let ((new-length (max (* (vector-length old-decode) 2)
			       (+ uid 1))))
	  (set-address-space-decode-vector! aspace
					    (extend-vector old-decode
							   new-length))))
    (if (>= uid (vector-length old-decode))
	(let ((new-length (vector-length (address-space-decode-vector aspace))))
	  (set-address-space-pending-vector! aspace
					     (extend-vector old-pending
							    new-length))))))
    
(define (maybe-extend-proxy-vector! aspace uid)
  (let ((old-proxy (address-space-proxy-vector aspace)))
    (if (>= uid (vector-length old-proxy))
	(let ((new-length (max (* (vector-length old-proxy) 2)
			       (+ uid 1))))
	  (set-address-space-proxy-vector! aspace
					   (extend-vector old-proxy new-length))))))
    
(define (extend-vector vector new-length)
  (let ((new (make-vector new-length #f)))
    (do ((i 0 (+ i 1)))
	((= i (vector-length vector)))
      (vector-set! new i (vector-ref vector i)))
    new))

;----------------
; Construct a reply to a UID request.  PAIR is (aspace-uid . uid).
; A uid-reply is a vector of: aspace-uid, uid, type, contents

(define (make-uid-reply pair)
  (let ((aspace-uid (car pair))
	(uid (cdr pair)))
    (let ((thing (vector-ref (address-space-decode-vector
			      (vector-ref (address-space-decode-vector
					   (local-address-space))
					  aspace-uid))
			     uid)))
      (receive (type data)
	  (encode-uid-reply thing)
;	(debug-message "sending reply " uid " from " aspace-uid " is " type " with " data)
	(really-make-uid-reply aspace-uid uid type data)))))

(define (encode-uid-reply thing)
  (cond ((symbol? thing)
	 (values (enum stob symbol)
		 (symbol->string thing)))
	((address-space? thing)
	 (values (enum stob address-space)
		 (cons (address-space-ip-address thing)
		       (address-space-socket thing))))
	((location? thing)
	 (values (enum stob location)
		 (contents thing)))
	((template? thing)
	 (values (enum stob template)
		 (template->vector thing)))
	(else
	 (error "bad object for uid reply"
		thing))))

; Make a vector copy of a template.  We have to hack around the id which may
; be an inconveniently large record.

(define (template->vector template)
  (let ((contents (make-vector (template-length template) 0)))
    (vector-set! contents 0 (template-ref template 0))  ; code vector
    (vector-set! contents 1 (template-id template))
    (do ((i 2 (+ i 1)))
	((= i (vector-length contents)))
      (vector-set! contents i (template-ref template i)))
    contents))

; Abstraction violation.

(define (template-id template)
  (let ((uid (template-ref template 1)))
    (if (record? uid)
	(record-ref uid 1)
	uid)))

;----------------
; Vectors are used instead of records because the only purpose of these is
; to transmit them between address spaces.

(define really-make-uid-reply vector)

(define (uid-reply-aspace-uid reply) (vector-ref reply 0))
(define (uid-reply-uid        reply) (vector-ref reply 1))
(define (uid-reply-type       reply) (vector-ref reply 2))
(define (uid-reply-data       reply) (vector-ref reply 3))

(define (uid-reply-aspace reply from-aspace)
  (vector-ref (address-space-decode-vector from-aspace)
	      (uid-reply-aspace-uid reply)))

;----------------
; Used to maintain the integrity of all these shared data structures.

(define (disable-interrupts!)
  (set-enabled-interrupts! 0))

(define (enable-interrupts! interrupts)
  (set-enabled-interrupts! interrupts))

;----------------
; Three versions of FOLD.

(define (fold list function state)
  (do ((list list (cdr list))
       (state state (function (car list) state)))
      ((null? list)
       state)))

(define (fold2 list1 list2 function state)
  (do ((list1 list1 (cdr list1))
       (list2 list2 (cdr list2))
       (state state (function (car list1) (car list2) state)))
      ((null? list1)
       state)))

(define (fold->2 list function state1 state2)
  (let loop ((list list) (state1 state1) (state2 state2))
    (if (null? list)
	(values state1 state2)
	(call-with-values
	 (lambda ()
	   (function (car list) state1 state2))
	 (lambda (state1 state2)
	   (loop (cdr list) state1 state2))))))
