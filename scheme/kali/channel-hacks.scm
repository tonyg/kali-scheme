(define open-input-channel
   (lambda (name)
      (open-channel name
		    (enum channel-status-option input))))

(define open-output-channel
   (lambda (name)
      (open-channel name
		    (enum channel-status-option output))))

(define channel->fd
   channel-os-index)

(define make-channel-non-blocking!
   (lambda (channel)
      (vm-extension 214
		    (channel-os-index channel))))

; This really should be smaller on Linux, but only costs space.
(define bufsiz
   (* 8 (expt 2 10)))

(define fd->input-port
   (lambda (fd)
      (input-channel->port
	 (open-channel fd
		       (enum channel-status-option
			     input))
	 bufsiz)))

; Note, the result of fd->output-port is non-blocking and does
; NOT automatically flush buffered output.
(define fd->output-port
   (lambda (fd)
      (let ((chan
	       (open-channel fd
			     (enum channel-status-option
				   output))))
	 (make-channel-non-blocking! chan)
	 (output-channel->port chan bufsiz #F))))

; Given a channel associated to a file descriptor which is a TCP/IP
; socket, set the TCP_NODELAY appropriately.
; Note, file descriptors which share an origin (e.g., dup'd file
; descriptors) share a TCP_NODELAY mode, and thus setting one will
; set the other.
(define set-tcp-nodelay!
   (lambda (channel nodelay?)
      (vm-extension 300
		    (cons (channel-os-index channel)
			  nodelay?))))

; Given a file descriptor, return input and output channels associated
; to it.
(define fd->channels
   (lambda (fd)
      (let ((out-fd
	       (duplicate-fd fd)))
	 (values
	    (open-channel fd
			  (enum channel-status-option input))
	    (open-channel out-fd
			  (enum channel-status-option output))))))

; Given a file descriptor, dup it.
(define duplicate-fd
   (lambda (fd)
      (vm-extension 29 fd)))
