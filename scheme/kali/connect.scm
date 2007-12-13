
; Exports:
;   connection-server
;   send-message
;   send-admin-message

; Making and using connections between address spaces.

;; connection-port-buffer-size
(define kali-port-buffer-size 4096) ;; 16384 the default is 4096

; Open a socket.  Copied from big-socket.scm.

(define (open-socket port)
  (let ((sock (make-socket (address-family inet) (socket-type stream))))
    (set-socket-reuse-address?! sock #t)
    (bind-socket sock (make-ipv4-socket-address (ipv4-address-any) port))
    (socket-listen sock)
    sock))

; Every address space runs a connection-server listening on a socket.  The
; machine name and that socket's number are used to identify the address space.
;
; We open the socket, initialize the address space, and then wait for
; connections.
;
; This also installs a post-gc-hook for GCing proxies.

(define (connection-server dispatcher report-proc)
  (set! *dispatch-procedure* dispatcher)
  (let* ((socket (open-socket 0))
         (port (socket-address-ipv4-port (socket-address socket))))
    (report-proc port)
    (initialize-local-address-space! port)
    (call-after-gc! gc-proxies)
    (let loop ()
      (call-with-values
       (lambda ()
	 (socket-listen socket kali-port-buffer-size)
	 (let ((incoming (socket-accept socket)))
	   (values port incoming)))
       (lambda (from-port to-port)
	 (server-make-connection! from-port to-port)
	 (loop))))))

; This is passed in during initialization to avoid circular module dependencies.

(define *dispatch-procedure*)

; Protocol:
;  If client is superior or client's lock is free on the server:
;    Server gets lock, sends 0 and both start dispatchers.
;  else
;    Server sends 1, closes channels
;    Client closes channels, releases server's lock, relinquishes timeslice,
;     and then tries again.

(define (server-make-connection! from-port to-port)
  (let ((reader (make-message-reader from-port to-port)))
    (call-with-values
     (lambda () (reader (local-address-space))) ; aspace won't be used
     (lambda (data missing-uids low-count-proxies)
       (let ((client-superior? (vector-ref data 0))
	     (machine-name     (vector-ref data 1))
	     (socket-number    (vector-ref data 2)))
	 (let* ((aspace (socket-id->address-space machine-name socket-number))
		(lock (address-space-lock aspace)))
	   (if (if client-superior?
		   (begin
		     (obtain-lock lock)
		     #t)
		   (maybe-obtain-lock lock))
	       (begin
		 (port-write-byte to-port 0)
		 (finish-connection! aspace from-port to-port reader))
	       (begin
		 (port-write-byte to-port 1)
		 (close-channel (port->channel from-port))
		 (close-channel (port->channel to-port))
		 (obtain-lock lock)
		 (if (not (address-space-out-port aspace))
		     (make-connection! aspace))
		 (release-lock lock)))))))))

; Copied from big-socket.scm.  Added the buffer-size argument, which
; determines the size of both the send and receive buffers.

(define (socket-client host-name port buffer-size)
  (let* ((ai (car
              (get-address-info host-name #f
                                (address-info-flags) (address-family inet)
                                (socket-type stream))))
	 (sa (address-info-socket-address ai))
	 (address (socket-address-ipv4-address sa))
	 (socket (make-socket (address-family inet)
			      (socket-type stream))))
    (set-socket-send-buffer-size! socket buffer-size)
    (set-socket-receive-buffer-size! socket buffer-size)
    (socket-connect socket
		    (make-ipv4-socket-address address port))
    (values (socket-input-port socket)
	    (socket-output-port socket))))

; Brings up the connection with ASPACE.
;
; The caller should already have aquired ASPACE's lock.
;
; We open a connection to ASPACE's socket and wait for the server to either
; lock our aspace over there and signal us to continue, or to tell us that
; our aspace is busy over there and we should wait.

(define (make-connection! aspace)
  (call-with-values
   (lambda ()
     (socket-client (address-space-ip-address aspace)
		    (address-space-socket aspace)
		    kali-port-buffer-size))
   (lambda (from-port to-port)
     (let ((reader (make-message-reader from-port to-port)))
       (call-with-values
	(lambda ()
	  (encode (vector (address-space< (local-address-space) aspace)
			  (address-space-ip-address (local-address-space))
			  (address-space-socket (local-address-space)))
		  aspace))
	(lambda (bytes need-counts)
	  (if (not (null? need-counts))
	      (raise (make-kali-connect-error 'need-counts need-counts)))
	  (let ((n (code-vector-length bytes)))
	    (really-port-write! bytes n to-port))
	  (if (= 0 (port-read-byte from-port))
	      (finish-connection! aspace from-port to-port reader)
	      (retry-connection! aspace from-port to-port))))))))

(define (finish-connection! aspace from-port to-port reader)
  (set-address-space-out-port! aspace to-port)
  (set-address-space-in-port! aspace from-port)
  (if (address-space-placeholder aspace)
      (placeholder-set! (address-space-placeholder aspace) #t))
  (release-lock (address-space-lock aspace))
  (spawn (lambda ()
	   (with-except-handler (dispatch-handler aspace)
	     (lambda ()
	       (*dispatch-procedure* aspace reader))))
	 (string-append "dispatcher-"
			(address-space-ip-address aspace)
			":"
			(number->string (address-space-socket aspace)))))

(define (retry-connection! aspace from-port to-port)
  (let ((placeholder (or (address-space-placeholder aspace)
			 (let ((ph (make-placeholder)))
			   (set-address-space-placeholder! aspace ph)
			   ph))))
    (close-input-port from-port)
    (close-output-port to-port)
    (release-lock (address-space-lock aspace))
    (placeholder-value placeholder)
    (obtain-lock (address-space-lock aspace))
    (make-connection! aspace)))

;----------------
; Handling errors in the dispatcher.  We print warnings and continue.  Errors
; kill us.

(define (dispatch-handler aspace)
  (lambda (condition)
    (cond ((warning? condition)
	   (display-dispatcher-condition condition aspace)
	   (unspecific))		;proceed
	  (else 
           (display-dispatcher-condition condition aspace)
	   (let ((out (current-error-port)))
	     (display "Terminating dispatcher!" out)
	     (newline out))
           (obtain-lock (address-space-lock aspace))
           (close-input-port (address-space-in-port aspace))
           (close-output-port (address-space-out-port aspace))
           (set-address-space-out-port! aspace #f)
           (set-address-space-in-port! aspace #f)
           (release-lock (address-space-lock aspace))
           (terminate-current-thread)))))

;	   (let ((port (current-error-port)))
;	     (display "Terminating dispatcher for address-space " port)
;	     (display other-aspace port)
;	     (newline port)
;	     (display "because of ")
;	     (display (if  (kali-reader-error? c)
;			   "reader-error: "
;			   "unknown error: ")
;		      port)
;	     (display c port)
;	     (newline port)
;	     (terminate-current-thread))))))

(define (display-dispatcher-condition condition aspace)
  (let ((out (current-error-port)))
    (newline out)
    (display "Signal caught from dispatcher for " out)
    (display aspace out)
    (display ":" out)
    (display-condition condition out)))

;----------------
; This returns a function that reads and decodes messages from IN-CHANNEL.
; OUT-CHANNEL is used in determining the byte-ordering of the other aspace.
;
; Every message requires two reads, one to get the size and the second
; for the body.
;
; Note, this depends on enough buffering that both sides can send a word's worth
; of bytes before either receives anything.

(define (make-message-reader in-port out-port)
  (let* ((my-vector (make-memory-layout-vector))
	 (bytes-per-word (vector-length my-vector)))
    (write-memory-layout-vector my-vector out-port)
    (let ((alien-vector (read-memory-layout-vector in-port)))
      (let ((my-get-size (memory-layout-vector->get-size my-vector))
	    (reverse? (need-to-reverse? my-vector alien-vector))
	    (size-buffer (make-code-vector bytes-per-word 0)))
	(lambda (aspace)
	  (really-port-read! in-port size-buffer bytes-per-word)
	  (let* ((size (my-get-size size-buffer))
		 (buffer (make-code-vector (- size bytes-per-word) 0)))
	    (really-port-read! in-port buffer (code-vector-length buffer))
	;(show-cv buffer)
	;(force-output (current-output-port))
	    (decode buffer aspace reverse?)))))))

(define (make-memory-layout-vector)
  (memory-status (enum memory-status-option
		       memory-layout)
		 #f))        ; ignored

; Writing and reading vectors of small integers.

(define (write-memory-layout-vector vector port)
  (port-write-byte port (vector-length vector))
  (do ((i 0 (+ i 1)))
      ((= i (vector-length vector)))
    (port-write-byte port (vector-ref vector i))))

(define (read-memory-layout-vector port)
  (let* ((size (port-read-byte port))
	 (vector (make-vector size)))
    (do ((i 0 (+ i 1)))
	((= i (vector-length vector)))
      (vector-set! vector i (port-read-byte port)))
    vector))

; Given a memory-layout vector (which has the byte offsets of sequential
; memory references within a word) this returns a procedure that extracts
; a word-sized integer from the beginning of a code vector.

(define (memory-layout-vector->get-size layout-vector)
  (let* ((bytes-per-word (vector-length layout-vector))
	 (bit-offset-vector (make-vector bytes-per-word)))
    (do ((i 0 (+ i 1)))
	((= i bytes-per-word))
      (vector-set! bit-offset-vector i (* 8 (vector-ref layout-vector i))))
    (lambda (buffer)
      (do ((i 0 (+ i 1))
	   (res 0 (+ res
		     (arithmetic-shift (code-vector-ref buffer i)
				       (vector-ref bit-offset-vector i)))))
	  ((= i bytes-per-word)
	   res)))))

; Returns #t if ALIEN-VECTOR is a reversal of LOCAL-VECTOR and #f if they
; are the same.  Signals an error if neither is the case.

(define (need-to-reverse? local-vector alien-vector)
  (cond ((equal? local-vector alien-vector)
	 #f)
	((equal? (reverse (vector->list local-vector))
		 (vector->list alien-vector))
	 #t)
	(else
	 (raise (make-kali-memory-layout-error
                 'alien-vector alien-vector
                 'local-vector local-vector)))))

;----------------
; Actually sending messages.
;
; 1. Encode the message
; 2. Wait for any needed reference counts
; 3. Get the lock on the address space
; 4. Establish a connection if none exists
; 5. Send the message
; 6. Release the lock
;
; There is a special version for administrative messages that spawns a
; separate thread instead of blocking.  Dispatcher use it to avoid deadlocking
; with another dispatcher.

(define (send-message type message aspace)
  (if (eq? aspace (local-address-space))
      (raise (make-kali-send-message-to-self-error
              'type type
              'message message)))
  (call-with-values
   (lambda ()
     (encode (cons type message)
	     (local-address-space)))
   (lambda (bvector need-counts)
     (really-send-message bvector need-counts aspace))))

(define (really-send-message bvector need-counts aspace)
  (if (not (null? need-counts))
      (wait-for-proxy-counts need-counts))
  (obtain-lock (address-space-lock aspace))
  (if (not (address-space-out-port aspace))
      (make-connection! aspace))
  (just-send-message bvector aspace))

(define (wait-for-proxy-counts need-counts)
  (call-with-values
   (lambda ()
     (make-proxy-requests need-counts))
   (lambda (requests placeholder)
     (if (not (null? requests))
	 (for-each (lambda (request)
		     (send-admin-message (enum message-type proxy-counts-request)
					 (cdr request)
					 (car request)))
		   requests))
     (if placeholder
	 (placeholder-value placeholder)))))

; Whomever called us obtained the lock.

(define (just-send-message bvector aspace)
  (really-port-write!  bvector
		       (code-vector-length bvector)
		       (address-space-out-port aspace))
  (release-lock (address-space-lock aspace)))

; Same as the above except that it will spawn instead of blocking.
; This is used by dispatchers to avoid deadlocking.

(define (send-admin-message type message aspace)
  (call-with-values
   (lambda ()
     (encode (cons type message)
	     (local-address-space)))
   (lambda (bvector need-counts)
     (if (and (null? need-counts)
	      (maybe-obtain-lock (address-space-lock aspace))
	      (address-space-out-port aspace))
	 (just-send-message bvector aspace)
	 (spawn (lambda ()
		  (really-send-message bvector need-counts aspace)))))))

;----------------
; GCing proxies.
;
; There is a little problem here.  If this is called from the root-scheduler's
; post-gc-hook, then any spawns done by SEND-ADMIN-MESSAGE will be in the wrong
; place.  I doubt that it matters, but it is a little weird.  The post-gc-handler
; should really just send an event, and not execute the code itself.

(define (gc-proxies)
  (for-each (lambda (aspace)
	      (if (eq? aspace (local-address-space))
		  (find-dead-proxies aspace #t)
		  (let ((dead (find-dead-proxies aspace #f)))
		    (if (not (null? dead))
			(send-admin-message (enum message-type return-proxy-counts)
					    dead
					    aspace)))))
	    (all-address-spaces)))

;----------------
; Low-level I/O stuff

; Return a copy of message code-vector CV minus its size field (the first
; four bytes) (for debugging).

(define (chop! cv)
   (let ((new (make-code-vector (- (code-vector-length cv) 4) 0)))
      (do ((i 0 (+ i 1)))
	  ((>= i (code-vector-length new)))
	 (code-vector-set! new i (code-vector-ref cv (+ i 4))))
      new))

; Utilities for reading and writing single bytes on channels.

(define (port-read-byte port)
  (char->integer (read-char port)))

(define (really-port-read! port buffer length)
  (let ((got (read-block buffer 0 length port)))
    (cond ((eof-object? got)
	   (raise (make-kali-reader-eof-error 'port port)))
	  ((< got length)
	   (raise (make-kali-reader-insufficient-error
                   'port port
                   'got got 
                   'length length))))))

(define (really-port-write! buffer length port)
  (write-block buffer 0 length port))

(define (port-write-byte port byte)
  (write-block (make-code-vector 1 byte) 0 1 port))

