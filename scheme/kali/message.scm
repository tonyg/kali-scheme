
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

(define (spawn-server . maybe-report-proc)
  (spawn (lambda ()
	   (apply start-server maybe-report-proc))))

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

(define (process-message message other-aspace)
  (let ((data (cdr message)))
    (enum-case message-type (car message)
      ((remote-return)
       (placeholder-set! (proxy-local-ref (car data))
			 (cdr data)))
      ((run)
       (spawn (lambda ()
		(let* ((proc (car data))
		       (args (cadr data))
		       (proxy (caddr data))
		       (aspace (local-address-space)))
		  (with-except-handler
		   (lambda (c)
		     (send-admin-message (enum message-type remote-return)
					 (cons proxy
					       (cons #f 
						     (make-kali-remote-error
                                                      'aspace aspace
                                                      'procedure proc
                                                      'arguments args
                                                      'condition c)))
					 other-aspace))
		   (lambda ()
		     (send-admin-message (enum message-type remote-return)
					 (cons proxy
					       (cons #t
						     (call-with-values
							 (lambda ()
							   (apply proc args))
						       (lambda results
							 results))))
					 other-aspace)))))))
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

(define (remote-run-handler placeholder aspace proc args c)
  (placeholder-set! placeholder
		    (cons #f (make-kali-remote-error
                              'aspace aspace
                              'procedure proc
                              'arguments args
                              'condition c))))

(define (remote-run! aspace proc . args)
  (if (eq? aspace (local-address-space))
      (let ((placeholder (make-placeholder)))
	(spawn (lambda ()
		 (with-except-handler
		  (lambda (c)
		    (remote-run-handler placeholder aspace proc args c))
		  (lambda ()
		    (placeholder-set! placeholder
				      (cons #t (apply proc args)))))))
	placeholder)
      (let* ((placeholder (make-placeholder))
	     (proxy (make-proxy placeholder)))
	(send-message (enum message-type run)
		      (list proc args proxy)
		      aspace)
	placeholder)))

(define (remote-apply aspace proc . args)
  (if (eq? aspace (local-address-space))
      (apply proc args)
      (with-except-handler
       (lambda (c)
	 (raise c))
       (lambda ()
	 (let* ((pair (placeholder-value (apply remote-run! 
						aspace
						proc
						args)))
		(all-right? (car pair))
		(result (cdr pair)))
	   (if all-right?
	       (apply values result)
	       (raise result)))))))

;; =====================================================================

(define (alive? aspace . maybe-timeout)
  (if (eq? aspace (local-address-space))
      #t
      (let* ((timeout (if (null? maybe-timeout)
			  3000
			  (car maybe-timeout)))
	     (placeholder (make-placeholder))
	     (thread-no (spawn (lambda ()
				 (with-except-handler
				  (lambda (c)
				    'ignor)
				  (lambda ()
				    (sleep timeout)
				    (placeholder-set! placeholder
						      #f))))))
	     (thread-yes (spawn (lambda ()
				  (with-except-handler
				   (lambda (c)
				     'ignore)
				   (lambda ()
				     (if (remote-run! aspace +)
					 (placeholder-set! placeholder
							   #t)))))))
	     (result (placeholder-value placeholder)))
	(if result
	    (terminate-thread! thread-no)
	    (terminate-thread! thread-yes))
	result)))
