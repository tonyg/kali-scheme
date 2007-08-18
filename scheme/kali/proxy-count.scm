; Exports:
;  make-proxy-requests
;  make-proxy-rerequests
;  proxy-uid->proxy
;  add-proxy-counts
;  find-dead-proxies
;  adjust-proxy-counts

; Threads waiting for proxy counts to arrive.

(define-record-type count-waiter :count-waiter
  (make-count-waiter count placeholder)
  count-waiter?
  (count count-waiter-count set-count-waiter-count!)
  (placeholder count-waiter-placeholder))

; The value of a PROXY-DATA-WAITER field is one of:
;   ()  - no one is waiting and there is no pending request
;   #T  - no one is waiting and there is a pending request
;   COUNT-WAITER* - these threads are waiting for additional counts to arrive

; This returns two values:
;  - A list of requests of the form (<aspace> . <proxy-uid>*)
;  - A placeholder if the outgoing message has to block or #F if it doesn't.
;
; This code fails to take into account the possibility that more counts have
; arrived since the message was encoded.  If so, we may not need to wait.
; I am not going to mess about making the change now.

(define (make-proxy-requests proxies)
  (let* ((placeholder (make-placeholder))
	 (waiter (make-count-waiter (length proxies) placeholder)))
    (values (with-interrupts-inhibited
	     (lambda ()
	       (really-make-proxy-requests proxies waiter)))
	    placeholder)))

(define (really-make-proxy-requests proxies waiter)	    
  (fold proxies
	(lambda (proxy-data requests)
	  (let ((waiters (proxy-data-waiters proxy-data)))
	    (case waiters
	      ((#t)
	       (set-proxy-data-waiters! proxy-data (list waiter))
	       requests)
	      ((())
	       (set-proxy-data-waiters! proxy-data (list waiter))
	       (add-request proxy-data requests))
	      (else
	       (set-proxy-data-waiters! proxy-data (cons waiter waiters))
	       requests))))
	'()))

; Add PROXY-DATA to the appropriate sublist in REQUESTS, an a-list of the form
; (<address-space> . <proxy-uids>)*

(define (add-request proxy-data requests)
  (let ((aspace (proxy-data-owner proxy-data))
	(uid (proxy-data-uid proxy-data)))
    (let ((have (assq aspace requests)))
      (if have
	  (begin
	    (set-cdr! have (cons uid (cdr have)))
	    requests)
	  (cons (list aspace uid) requests)))))

;----------------
; Counts just arrived for PROXIES.  This returns a list of the uids of any
; of PROXIES for which there are still waiting messages.

(define (make-proxy-rerequests proxies)
  (with-interrupts-inhibited
   (lambda ()
     (fold proxies
	   (lambda (proxy requests)
	     (let ((proxy-data (proxy-data proxy)))
	       (case (proxy-data-waiters proxy-data)
		 ((#t)
		  (set-proxy-data-waiters! proxy-data '())
		  requests)
		 ((())
		  (warn "proxy count reply finds no waiters!" proxy)
		  requests)
		 (else
		  (cons (proxy-data-uid proxy-data) requests)))))
	   '()))))

;----------------
; PROXIES is a list of <proxy-data>s whose reference count is above the
; allowable maximum and pairs (<proxy-data> . <count>) where <proxy-data>'s
; current count is less than one and <count> more just arrived.
;
; For the too-high counts we decrement the count and return a chunk to the
; the owner (the caller does the actual message sending).
;
; For each too-low proxy-data we reduce the counts of its waiters, releasing
; any whose counts go to zero.  Any leftover proxy counts are given to the
; proxy.

(define (adjust-proxy-counts! proxies)
  (call-with-values
    (lambda ()
      (partition-list pair? proxies))
    (lambda (pairs proxies)
      (release-proxy-count-waiters! pairs)
      (make-return-messages proxies))))

(define (release-proxy-count-waiters! pairs)
  (for-each (lambda (placeholder)
	      (placeholder-set! placeholder #f))
	    (with-interrupts-inhibited
	     (lambda ()
	       (fold pairs
		     (lambda (pair ready)
		       (append (check-proxy-waiters (car pair) (cdr pair))
			       ready))
		     '())))))

; The two reversals of the waiters list ensures that the waiters are removed
; in the order they were added (or we could just use a queue).

(define (check-proxy-waiters data count)
  (let loop ((waiters (reverse (proxy-data-waiters data)))
	     (count count)
	     (ready '()))
    (if (or (null? waiters)
	    (= count 0))
	(begin
	  (set-proxy-data-reference-count! data count)
	  (set-proxy-data-waiters! data (if (null? waiters)
					    #t
					    (reverse waiters)))
	  ready)
	(loop (cdr waiters)
	      (- count 1)
	      (let* ((next (car waiters))
		     (wait-count (count-waiter-count next)))
		(if (= wait-count 1)
		    (cons (count-waiter-placeholder next) ready)
		    (begin
		      (set-count-waiter-count! next (- wait-count 1))
		      ready)))))))

; Make messages for returning any excess counts for PROXY-DATAS.

(define (make-return-messages proxy-datas)
  (with-interrupts-inhibited
   (lambda ()
     (fold proxy-datas
	   (lambda (proxy-data returns)
	     (let ((counts (proxy-data-reference-count proxy-data))
		   (aspace (proxy-data-owner proxy-data))
		   (uid (proxy-data-uid proxy-data)))
	       (let ((returned (return-counts counts)))
		 (set-proxy-data-reference-count! proxy-data (- counts returned))
		 (cond ((= returned 0)
			returns)
		       ((assq aspace returns)
			=> (lambda (p)
			     (set-cdr! p (cons (cons uid returned) (cdr p)))
			     returns))
		       (else
			(cons (list aspace (cons uid returned)) returns))))))
	   '()))))

; How many counts to return.

(define (return-counts counts)
  (if (< max-proxy-count counts)
      (- counts max-proxy-debit)
      0))

;----------------
; For constructing replies.

(define (proxy-uid->proxy uid)
  (proxy-data->proxy
   (vector-ref (address-space-proxy-vector (local-address-space))
	       uid)))

;----------------
; COUNTS reference counts have arrived for PROXY-UID, so we update its counts.

(define (add-proxy-counts! proxy-uid counts)
  (let* ((proxy-vector (address-space-proxy-vector (local-address-space)))
	 (data (vector-ref proxy-vector proxy-uid))
	 (counts (+ counts (proxy-data-reference-count data))))
    (set-proxy-data-reference-count! data counts)
    (if (and (= counts proxy-base-count)
	     (not (weak-pointer-ref (proxy-data-self data))))
	(deallocate-proxy-uid proxy-vector proxy-uid))))

; Here we are depending on not having interrupt checks in a basic block.

(define (deallocate-proxy-uid proxy-vector proxy-uid)
  (vector-set! proxy-vector proxy-uid (vector-ref proxy-vector 0))
  (vector-set! proxy-vector 0 proxy-uid))

;----------------
; Returns a list of (proxy-uid . reference-count) pairs for proxies that are no
; longer pointed to from the local heap.  If ASPACE is the local address space,
; then they must be no outstanding reference counts.  If ASPACE is not the local
; address space, then they must not have a local value.
;
; If LOCAL? the returned value is ignored.
;
; The proxy-datas are removed from the decode vector.  For the local case this
; requires adding the slot to the freelist.
;
; Remote proxies that have local values can also be removed, but that requires
; a more complex protocol.  The value must be retained until the owner has sent
; out a message saying the proxy is dead, and the owner must wait until this
; message has been acknowledged before reusing the uid.

(define (find-dead-proxies aspace local?)
  (with-interrupts-inhibited
   (lambda ()
     (let ((vector (address-space-proxy-vector aspace)))
       (vector-fold
	 vector
	 (lambda (entry proxies)
	   (if (and (proxy-data? entry)
		    (not (weak-pointer-ref (proxy-data-self entry)))
		    (if local?
			(= (proxy-data-reference-count entry)
			   proxy-base-count)
			(not (proxy-data-has-local-value? entry))))
	       (let ((uid (proxy-data-uid entry)))
		 (cond (local?
			(deallocate-proxy-uid vector uid)
			proxies)
		       (else
			(vector-set! vector (proxy-data-uid entry) #f)
			(cons (cons uid (proxy-data-reference-count entry))
			      proxies))))
	       proxies))
	 '())))))

;----------------

(define (fold list function state)
  (do ((list list (cdr list))
       (state state (function (car list) state)))
      ((null? list)
       state)))

(define (vector-fold vec folder state)
  (let ((len (vector-length vec)))
    (do ((i 0 (+ i 1))
	 (state state (folder (vector-ref vec i)
			      state)))
	((= i len)
	 state))))

(define (partition-list pred l)
  (let loop ((l l) (yes '()) (no '()))
    (cond ((null? l)
           (values (reverse yes) (reverse no)))
          ((pred (car l))
           (loop (cdr l) (cons (car l) yes) no))
          (else
           (loop (cdr l) yes (cons (car l) no))))))


