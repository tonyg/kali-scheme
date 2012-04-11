; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Using sockets from Scheme 48.

; Server side routines:
;   (open-socket port-number-or-zero) => socket (socket is O_NONBLOCK)
;   (socket-port-number socket) => port number
;     This is essential if the argument to open-socket was zero.
;   (socket-listen s) => two values: input port and output port
;   (socket-listen-channels s) => two values: input channel and output channel
;     Blocks until an outside party connects to the socket's port number.
;     Use socket-listen to get ports and socket-listen-channels to get
;	channels.
;   (close-socket s)
;     Do this after you've done all the socket-listens you want to.

; Client side routine:
;   (socket-client host-name port-number) =>
;       two values: input port and output port
;   (socket-client-channels host-name port-number) =>
;       two values: input channel and output channel


; --------------------
; Client.


(define socket-client
   (lambda (host-name port-number)
      (call-with-values
	 (lambda ()
	    (socket-client-channels host-name
				    port-number))
	 (lambda (i-chan o-chan)
	    (values (input-channel->port i-chan bufsiz)
		    (output-channel->port o-chan bufsiz))))))

(define socket-client-channels
   (let ((try
	    (lambda (fd host-name port-number)
	       (not (= -1
		       (vm-extension 23
				     `(,fd ,host-name
					   . ,port-number)))))))
      (lambda (host-name port-number)
	 (let* ((fd
		   (internet-stream-socket))
		(o-chan
		   (open-channel fd
				 (enum channel-status-option output))))
	    (let loop ()
	       (disable-interrupts!)
	       (cond ((try fd host-name port-number)
			(enable-interrupts!)
			(values (open-channel (duplicate-fd fd)
					      (enum channel-status-option input))
				o-chan))
		     (else
			(wait-for-channel o-chan)	; re-enables interrupts
			(loop))))))))

; --------------------
; Server calls

(define-record-type socket :socket
  (make-socket ch pn)
  socket?
  (ch socket-channel)
  (pn socket-port-number))

(define-record-discloser :socket
  (lambda (s) `(socket ,(socket-port-number s))))

(define (open-socket port-number)
  (let* ((socket-fd (internet-stream-socket))
	 (channel (open-channel socket-fd (enum channel-status-option
						input))))
    (make-socket channel (socket-bind socket-fd port-number))))

(define (close-socket socket)
  (close-channel (socket-channel socket)))

(define (socket-listen socket)
  (fd->ports (socket-accept (socket-channel socket))))

(define (socket-listen-channels socket)
  (fd->channels (socket-accept (socket-channel socket))))

; --------------------
; vm-extension calls

(define (internet-stream-socket)
  (vm-extension 20 #f))

(define (socket-bind s port-number)
  (vm-extension 21 (cons s port-number)))

(define (socket-accept socket-channel)
  (disable-interrupts!)
  (let ((fd (vm-extension 22 (channel-os-index socket-channel))))
    (cond ((= fd -1)  ; not ready
	   (wait-for-channel socket-channel)  ; re-enables interrupts
	   (socket-accept socket-channel))
	  (else
	   (enable-interrupts!)
	   fd))))

; We duplicate the fd so that the input and output channels can be
; closed independently.

(define (fd->channels fd)
  (let ((out-fd (duplicate-fd fd)))
    (values (open-channel fd (enum channel-status-option input))
	    (open-channel out-fd (enum channel-status-option output)))))

(define fd->ports
   (let ((bufsiz 1024))
      (lambda (fd)
	 (call-with-values
	    (lambda ()
	       (fd->channels fd))
	    (lambda (in out)
	       (values (input-channel->port in bufsiz)
		       (output-channel->port out bufsiz)))))))

(define (duplicate-fd fd)
  (vm-extension 29 fd))

(define bufsiz 1024)
