
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
  ;(debug-message "[missing uids]")
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

;; begin chnx available!

(define *received-available-values* '())

(define (all-received)
  *received-available-values*)

(define (last-received)
  (if (null? *received-available-values*)
      'nothing
      (car *received-available-values*)))

;; end chnx available!

(define (process-message message other-aspace)
  (let ((data (cdr message)))
    (enum-case message-type (car message)
      ((run)
       (spawn (lambda ()
		(apply (car data)
		       (cdr data)))))
      ;; begin chnx available!
      ((available)
       ;(debug-message "[available]")
       (set! *received-available-values*
	     (cons data ;(car data) ;; no integers any more...
		   *received-available-values*)))
      ;; end chnx available!
      ((uid-request)
       ;(debug-message "[uid request]")
       (send-admin-message (enum message-type uid-reply)
			   (map make-uid-reply data)
			   other-aspace))
      ((uid-reply)
       ;(debug-message "[uid reply]")
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
       ;(debug-message "[return-proxy-counts]")
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

(define (remote-apply aspace proc . args)
  (if (eq? aspace (local-address-space))
      (apply proc args)
      (let* ((placeholder (make-placeholder))
	     (proxy (make-proxy placeholder)))
	(remote-run! aspace
	  (lambda ()
	    (remote-run! (proxy-data-owner (proxy-data proxy))
	       (lambda (value)
		 (placeholder-set! (proxy-local-ref proxy)
				   value))
	       (apply proc args))))
	(placeholder-value placeholder))))

;; begin chnx available!
(define (make-available! aspace value)
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
;; end chnx available!

