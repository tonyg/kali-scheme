; A comrade is a Kali process with no REPL.
;
; The idea is to provide Kali as a service.
;   - add a line to /etc/inetd.conf to start comrades
;   - add a line to /etc/services to get the port number
;   - make a comrade image that talks to stdin and stdout, telling them
;     the port on which it is listening for connections.  Stdout should
;     be redirected to stderr.

(define (start-comrade port-number)
  (run-threads (make-comrade-event-handler
		 (lambda ()
		   (start-server (lambda (server-port)
				   (write-number-to-port-number server-port
								port-number)))))))
  
(define (write-number-to-port-number n port-number)
  (let ((channel (open-channel port-number (enum channel-status-option input)))
	(string (number->string n)))
    (channel-write string 0 (string-length string) channel)
    (close-channel channel)))

(define comrade-quantum 200)  ; thread quantum in msec

(define (make-comrade-event-handler initial-thunk)
  (let ((runnable (make-thread-queue))
	(thread-count (make-counter)))
    (increment-counter! thread-count)
    (exclusively-enqueue-thread!
       runnable
       (make-thread initial-thunk dynamic-env-link 'comrade-server)
       '())
    (save-base-fluid-env! (with-handler report-handler dynamic-env))
    (round-robin-event-handler runnable
			       comrade-quantum
			       dynamic-env-link
			       thread-count
			       (lambda args #f)		; we handle no events
			       propogate-upcall		;  or upcalls
			       wait)))

(define report-handler
   (lambda (c next-handler)
      (for-each display
		`(#\newline
		  "HCC: in report-handler."
		  #\newline
		  "	c: " ,c
		  #\newline
		  "	next-handler: " ,next-handler
		  #\newline
		  "	(error? c): " ,(error? c)
		  #\newline
		  "	(interrupt? c): " ,(interrupt? c)
		  #\newline
		  "	(current-thread): " ,(current-thread)
		  #\newline))
      (force-output (current-output-port))
      (next-handler)))
  
