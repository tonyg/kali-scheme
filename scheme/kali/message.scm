
; Start a server.  MAYBE-REPORT-PROC is an optional argument which is passed
; the new servers port number.  If it is not specified we just print out the
; port number.

; Dispatcher is passed avoid module circularities.

(define (start-server . maybe-report-proc)
  (connection-server dispatcher
		     (if (null? maybe-report-proc)
			 (lambda (port-number)
			   (display "Waiting for connection on port ")
			   (display port-number)
			   (newline))
			 (car maybe-report-proc))))

; Repeatedly reads and process messages sent by OTHER-ASPACE.  READER is
; called to read messages.  We check for proxies with too-low or too-high
; counts, releasing pending sends or returning counts as appropriate.
; Messages without missing uids are evaluated, those with missing uids
; have to wait.

(define (dispatcher other-aspace reader)
  (let loop ()
    (call-with-values
     (lambda ()
       (reader other-aspace))
     (lambda (message missing-uids bad-count-proxies)
       (if (not (null? bad-count-proxies))
	   (for-each return-counts (adjust-proxy-counts! bad-count-proxies)))
       (if (null? missing-uids)
	   (process-message message other-aspace)
	   (process-missing-uids message missing-uids other-aspace))
       (loop)))))

; UID replies are processed immediately (because of possible circular
; dependencies), all other messages have to wait.

(define (process-missing-uids message missing-uids other-aspace)
  ;(debug-message "[missing uids]") ;; chnx debug
  (if (eq? (car message)
	   (enum message-type uid-reply))
      (process-uid-replies (cdr message) missing-uids other-aspace)
      (wait-for-missing-uids missing-uids
			     (lambda ()
			       (process-message message other-aspace))
			     other-aspace)))

; Messages are pairs with the type in the car and the data in the cdr.
; REMOTE-APPLY has its own type so that it works even when the two address
; spaces do not agree on the UID of REMOTE-RUN!.

;; begin chnx debug

(define *received-available-values* '())

(define (all-received)
  *received-available-values*)

(define (last-received)
  (if (null? *received-available-values*)
      'nothing
      (car *received-available-values*)))

;; end chnx debug

(define (process-message message other-aspace)
  (let ((data (cdr message)))
    (enum-case message-type (car message)
      ((run)
       (spawn (lambda ()
		(apply (car data)
		       (cdr data)))))
      ((apply)
       ;(debug-message "[apply]") ;; chnx debug
       (spawn (lambda ()
		(call-with-values
		  (lambda ()
		    (apply (cadr data) (cddr data)))
		  (lambda values
		    (send-message (enum message-type results)
				  (cons (car data) values)
				  other-aspace))))))
      ;; begin chnx debug
      ((available)
       ;(debug-message "[available]")  ;; chnx debug
       (set! *received-available-values*
	     (cons data ;(car data) ;; no integers any more...
		   *received-available-values*)))
      ;; end chnx debug
      ((results)
       ;(debug-message "[results]") ;; chnx debug
       (remote-return (car data) (cdr data)))
      ((uid-request)
       ;(debug-message "[uid request]") ;; chnx debug
       (send-admin-message (enum message-type uid-reply)
			   (map make-uid-reply data)
			   other-aspace))
      ((uid-reply)
       ;(debug-message "[uid reply]") ;; chnx debug
       (process-uid-replies data '() other-aspace))
      ((proxy-counts-request)
       (send-admin-message (enum message-type proxy-counts)
			   (map proxy-uid->proxy data)
			   other-aspace))
      ((proxy-counts)
       (let ((requests (make-proxy-rerequests data)))
	 (if (not (null? requests))
	     (send-admin-message (enum message-type proxy-counts-request)
				 requests
				 other-aspace))))
      ((return-proxy-counts)
       (debug-message "[return-proxy-counts]") ;; chnx debug
       (for-each (lambda (p)
		   (add-proxy-counts! (car p) (cdr p)))
		 data))
      (else
       (warn "Dropping unrecognized message" message)))))

(define (return-counts list)
  (send-admin-message (enum message-type return-proxy-counts)
		      (cdr list)
		      (car list)))

;----------------
; REMOTE-RUN! and friends.  These just send the appropriate messages.

(define (remote-run! aspace proc . args)
  (if (eq? aspace (local-address-space))
      (spawn (lambda () (apply proc args)))
      (send-message (enum message-type run)
		    (cons proc args)
		    aspace)))

(define (remote-apply target proc . args)
  (if (eq? target (local-address-space))
      (apply proc args)
      (let ((id (next-remote-apply-id)))
	(send-message (enum message-type apply)
		      (cons id (cons proc args))
		      target)
	(wait-for-remote-return id))))

;; begin chnx debug
(define (remote-available! aspace value)
  (if (eq? aspace (local-address-space))
      (begin
	(display "already available here!")
	(newline))
      (if (number? value)
	  (begin
	    (display "no numbers! wrap them in a list or something.")
	    (newline))
	  (send-message (enum message-type available)
			value ;(cons value '()) ;; no integers any more
			aspace))))
;; end chnx debug

(define (remote-return id return-values)
  (with-interrupts-inhibited
   (lambda ()
     (let ((status
	    (vector-ref *apply-waiters* id)))
       (vector-set! *apply-waiters*
		    id
		    (list return-values))
       (if (not (eq? status 'pending))
	   (make-ready status))))))

(define (wait-for-remote-return id)
  (with-interrupts-inhibited
   (lambda ()
     (let ((status (vector-ref *apply-waiters* id)))
       (cond ((eq? status 'pending)
	      (vector-set! *apply-waiters* id (current-thread))
	      (block)
	      (wait-for-remote-return id))
	     (else
	      (set! *free-apply-ids*
		    (cons id *free-apply-ids*))
	      (vector-set! *apply-waiters* id #F)
	      (apply values (car status))))))))

; A supply of UID's for remote calls.

(define *free-apply-ids* '())
(define *next-apply-id* 0)
(define *apply-waiters* (make-vector 100 #f))

(define (next-remote-apply-id)
  (with-interrupts-inhibited
   (lambda ()
     (cond ((null? *free-apply-ids*)
	    (let ((uid *next-apply-id*))
	      (set! *next-apply-id* (+ 1 uid))
	      (if (>= uid (vector-length *apply-waiters*))
		  (let ((new (make-vector (+ uid 100) #f)))
		    (do ((i 0 (+ i 1)))
			((= i (vector-length *apply-waiters*)))
		      (vector-set! new i (vector-ref *apply-waiters* i)))
		    (set! *apply-waiters* new)))
	      (vector-set! *apply-waiters* uid 'pending)
	      uid))
	   (else
	    (let ((uid (car *free-apply-ids*)))
	      (set! *free-apply-ids* (cdr *free-apply-ids*))
	      (vector-set! *apply-waiters* uid 'pending)
	      uid))))))

;; chnx debug
(define (debug-message . args)
  (for-each display 
	    args)
  (newline))
