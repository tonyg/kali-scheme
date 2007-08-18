;; channels in read/write functions to front (1st argument)
;;   ok

;; remove memory-layout-stuff
;;


; Exports:
;   connection-server
;   send-message
;   send-admin-message

; Making and using connections between address spaces.

; Every address space runs a connection-server listening on a socket.  The
; machine name and that socket's number are used to identify the address space.
;
; We open the socket, initialize the address space, and then wait for
; connections.
;
; This also installs a post-gc-hook for GCing proxies.

(define (connection-server dispatcher report-proc)
  (set! *dispatch-procedure* dispatcher)
  (let ((socket (open-socket 0)))
    (report-proc (socket-port-number socket))
    (initialize-local-address-space! (socket-port-number socket))
    (call-after-gc! gc-proxies)
    (let loop ()
      (call-with-values
       (lambda ()
	 (socket-listen-channels socket))
       (lambda (from-channel to-channel)
	 (server-make-connection! from-channel to-channel)
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

(define (server-make-connection! from-channel to-channel)
  (let ((reader (make-message-reader from-channel to-channel)))
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
		 (channel-write-byte to-channel 0)
		 (finish-connection! aspace from-channel to-channel reader))
	       (begin
		 (channel-write-byte to-channel 1)
		 (close-channel from-channel)
		 (close-channel to-channel)
		 (obtain-lock lock)
		 (if (not (address-space-out-channel aspace))
		     (make-connection! aspace))
		 (release-lock lock)))))))))

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
     (socket-client-channels (address-space-ip-address aspace)
			     (address-space-socket aspace)))
   (lambda (from-channel to-channel)
     (let ((reader (make-message-reader from-channel to-channel)))
       (call-with-values
	(lambda ()
	  (encode (vector (address-space< (local-address-space) aspace)
			  (address-space-ip-address (local-address-space))
			  (address-space-socket (local-address-space)))
		  aspace))
	(lambda (bytes need-counts)
	  (if (not (null? need-counts))
	      (error "MAKE-CONNECTION! encode needs proxy counts" need-counts))
	  (channel-write to-channel bytes 0 (code-vector-length bytes))
	  (if (= 0 (channel-read-byte from-channel))
	      (finish-connection! aspace from-channel to-channel reader)
	      (retry-connection! aspace from-channel to-channel))))))))

(define (finish-connection! aspace from-channel to-channel reader)
  (set-tcp-nodelay! from-channel tcp-nodelay?)
  (set-tcp-nodelay! to-channel tcp-nodelay?)
  (set-address-space-out-channel! aspace to-channel)
  (set-address-space-in-channel! aspace from-channel)
  (if (address-space-placeholder aspace)
      (placeholder-set! (address-space-placeholder aspace) #t))
  (release-lock (address-space-lock aspace))
  (spawn (lambda ()
	   (with-handler (dispatch-handler aspace)
	     (lambda ()
	       (*dispatch-procedure* aspace reader))))
	 (string-append "dispatcher-"
			(address-space-ip-address aspace)
			":"
			(number->string (address-space-socket aspace)))))

(define (retry-connection! aspace from-channel to-channel)
  (let ((placeholder (or (address-space-placeholder aspace)
			 (let ((ph (make-placeholder)))
			   (set-address-space-placeholder! aspace ph)
			   ph))))
    (close-channel from-channel)
    (close-channel to-channel)
    (release-lock (address-space-lock aspace))
    (placeholder-value placeholder)
    (obtain-lock (address-space-lock aspace))
    (make-connection! aspace)))

; Should we set the TCP_NODELAY mode?

(define tcp-nodelay? #t)

;----------------
; Handling errors in the dispatcher.  We print warnings and continue.  Errors
; kill us.

(define (dispatch-handler aspace)
  (lambda (condition next-handler)
    (cond ((warning? condition)
	   (display-dispatcher-condition condition aspace)
	   (unspecific))		;proceed
; This would allow other dispatchers to keep running.
;          ((or (error? condition)
;	    (interrupt? condition))
;           (display-dispatcher-condition condition aspace)
;           (obtain-lock (address-space-lock aspace))
;           (close-channel (address-space-in-channel aspace))
;           (close-channel (address-space-out-channel aspace))
;           (set-address-space-out-channel! aspace #f)
;           (set-address-space-in-channel! aspace #f)
;           (release-lock (address-space-lock aspace))
;           (terminate-current-thread))
	  (else                           
	   (next-handler)))))

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

(define (make-message-reader in-channel out-channel)
  (let* ((my-vector (make-memory-layout-vector))
	 (bytes-per-word (vector-length my-vector)))
    (write-memory-layout-vector my-vector out-channel)
    (let ((alien-vector (read-memory-layout-vector in-channel)))
      (let ((my-get-size (memory-layout-vector->get-size my-vector))
	    (reverse? (need-to-reverse? my-vector alien-vector))
	    (size-buffer (make-code-vector bytes-per-word 0)))
	(lambda (aspace)
	  (really-channel-read! in-channel size-buffer bytes-per-word)
	  (let* ((size (my-get-size size-buffer))
		 (buffer (make-code-vector (- size bytes-per-word) 0)))
            (really-channel-read! in-channel buffer (code-vector-length buffer))
	    ;(show-cv buffer)
	    ;(force-output (current-output-port))
	    (decode buffer aspace reverse?)))))))

(define (make-memory-layout-vector)
  (memory-status (enum memory-status-option
		       memory-layout)
		 #f))        ; ignored

; Writing and reading vectors of small integers.

(define (write-memory-layout-vector vector channel)
  (channel-write-byte channel (vector-length vector))
  (do ((i 0 (+ i 1)))
      ((= i (vector-length vector)))
    (channel-write-byte channel (vector-ref vector i))))

(define (read-memory-layout-vector channel)
  (let* ((size (channel-read-byte channel))
	 (vector (make-vector size)))
    (do ((i 0 (+ i 1)))
	((= i (vector-length vector)))
      (vector-set! vector i (channel-read-byte channel)))
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
	 (error "don't know how to convert alien memory model"
		alien-vector
		local-vector))))

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
      (error "attempt to send message to self" type message))
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
  (if (not (address-space-out-channel aspace))
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
  (channel-write (address-space-out-channel aspace)
		 bvector
		 0
		 (code-vector-length bvector))
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
	      (address-space-out-channel aspace))
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

(define (channel-read-byte channel)
  (let ((buffer (make-code-vector 1 0)))
    (really-channel-read! channel buffer 1)
    (code-vector-ref buffer 0)))

(define (really-channel-read! channel buffer length)
  (let ((got (channel-read channel buffer 0 length)))
    (cond ((eof-object? got)
	   (error "unexpected EOF" channel))
	  ((< got length)
	   (error "insufficient characters from CHANNEL-READ" channel got length)))))

(define (channel-write-byte channel byte)
  (channel-write channel (make-code-vector 1 byte) 0 1))

 
;; chnx todo channel-read
;; we implement channel-read _here_ because
;; we will take it out again ... probably ...

; CHANNEL-READ returns the number of characters read or the EOF object.
; BUFFER is either a string or code vector and START is the index at which
; to place the first character read.  NEEDED is one of
;  <integer> : the call returns when this many characters has been read or
;     an EOF is reached.
;  'IMMEDIATE : the call reads as many characters as are available and
;     returns immediately.
;  'ANY : the call returns as soon as at least one character has been read
;     or an EOF is reached.

(define (channel-read channel buffer start needed)
  (call-with-values
      (lambda ()
	(cond ((eq? needed 'immediate)
	       (values #f 0 (- (buffer-length buffer) start)))
	      ((eq? needed 'any)
	       (values #t 1 (- (buffer-length buffer) start)))
	      (else
	       (values #t needed needed))))
    (lambda (keep-trying? need max-chars)
      (let loop ((have 0))
	(let ((got (channel-maybe-read channel
				       buffer
				       (+ start have)
				       (- max-chars have)
				       keep-trying?)))
	  (cond ((eof-object? got)
		 (if (= have 0)
		     (eof-object)
		     have))
		((not got)
		 (let ((condvar (make-condvar)))
		   (wait-for-channel channel condvar)
		   (with-new-proposal 
		    (lose)
		    (cond ((maybe-commit-and-wait-for-condvar condvar)
			   #t)
			  (else (error "channel-read: condvar-commit failed"))))
		   (let ((have (+ have (condvar-value condvar))))
		     (if (and keep-trying? (< have need))
			 (loop have)
			 have))))
		(else
		 (let ((have (+ have got)))
		   (if (and keep-trying? (< have need))
		       (loop have)
		       have)))))))))

(define buffer? code-vector?)

(define buffer-length code-vector-length)
