; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Sockets

(define-record-type socket :socket
  (really-make-socket address-family type
		      channel condvar
		      input-port output-port)
  socket?
  (address-family socket-address-family)
  (type socket-socket-type)
  (channel socket-channel)
  (condvar socket-condvar) ; for blocking until a connection arrives
  (input-port socket-input-port set-socket-input-port!)
  (output-port socket-output-port set-socket-output-port!))

(define-record-discloser :socket
  (lambda (s)
    (list 'socket 
	  (socket-address-family s)
	  (socket-socket-type s)
	  (socket-channel s))))

(define (channel->socket family type channel)
  (really-make-socket family type
		      channel
		      (make-condvar) #f #f))

(define (attach-socket-ports! socket output-channel)
  (let ((input-channel (socket-channel socket)))
    (set-socket-input-port!
     socket
     (input-channel+closer->port input-channel close-socket-input-channel))
    (set-socket-output-port!
     socket
     (output-channel+closer->port output-channel close-socket-output-channel))))
			
(define make-socket
  (opt-lambda (family type (protocol 0)) ; ####
    (channel->socket family type
		     (external-socket (address-family->raw family)
				      (socket-type->raw type)
				      protocol))))

(import-lambda-definition external-socket (family type protocol)
			  "s48_socket")

(define (dup-socket sock)
  (channel->socket (socket-address-family sock)
		   (socket-socket-type sock)
		   (external-dup-socket-channel (socket-channel sock))))

(define make-socket-pair
  (opt-lambda (family type (protocol 0))
    (let ((p (external-socketpair (address-family->raw family)
				  (socket-type->raw type)
				  protocol)))
      (let ((s1 (channel->socket family type (car p)))
	    (s2 (channel->socket family type (cdr p))))
	(attach-socket-ports! s1 (external-dup-socket-channel (car p)))
	(attach-socket-ports! s2 (external-dup-socket-channel (cdr p)))
	(values s1 s2)))))

(import-lambda-definition external-socketpair (family type protocol)
			  "s48_socketpair")

; Close the channel, notifying any waiters that this has happened.

(define (close-socket socket)
  (cond
   ((or (socket-input-port socket) (socket-output-port socket))
    (cond 
     ((socket-input-port socket) => close-input-port)
     ((socket-output-port socket) => close-output-port)))
   (else
    (let ((channel (socket-channel socket)))
      (with-new-proposal (lose)
	(or (channel-maybe-commit-and-close channel close-channel)
	    (lose)))))))

(define (bind-socket socket address)
  (external-bind (socket-channel socket)
		 (socket-address-raw address)))

(import-lambda-definition external-bind (channel address)
			  "s48_bind")

(define socket-listen
  (opt-lambda (socket (queue-size (max-socket-connection-count)))
    (external-listen (socket-channel socket)
		     queue-size)))

(import-lambda-definition external-listen (channel queue-size)
			  "s48_listen")
(import-lambda-definition max-socket-connection-count ()
			  "s48_max_connection_count")

; FreeBSD's connect() behaves oddly.  If you get told to wait, wait for select()
; to signal the all-clear, and then try to connect again, you get an `already
; connected' error.  To handle this we pass in a RETRY? flag.  If RETRY? is
; true the `already connected' error is ignored.

(define (socket-connect socket address)
  (let ((channel (socket-channel socket))
	(raw-address (socket-address-raw address)))
    (let loop ((retry? #f))
      (disable-interrupts!)
      (let ((output-channel (external-connect channel raw-address retry?)))
	(cond ((channel? output-channel)
	       (enable-interrupts!)
	       (attach-socket-ports! socket output-channel))
	      ((eq? output-channel #t)
	       (assertion-violation 'socket-client
				    "client socket already connected"
				    socket address))
	      (else
	       (let ((condvar (make-condvar)))
		 (wait-for-channel channel condvar)
		 (with-new-proposal (lose)
		   (maybe-commit-and-wait-for-condvar condvar))
		 (enable-interrupts!)
		 (loop #t))))))))

(import-lambda-definition external-connect (channel address retry?)
			  "s48_connect")

(define (socket-accept socket)
  (let* ((channel (blocking-socket-op socket external-accept))
	 (newsock (channel->socket (socket-address-family socket)
				   (socket-socket-type socket)
				   channel)))
    (attach-socket-ports! newsock (external-dup-socket-channel channel))
    newsock))

(import-lambda-definition external-accept (channel retry?)
			  "s48_accept")
(import-lambda-definition external-dup-socket-channel (channel)
			  "s48_dup_socket_channel")

; Keep performing OP until it returns a non-#F value.  In between attempts we
; block on the socket's channel.

(define (blocking-socket-op socket op)
  (let ((channel (socket-channel socket))
	(condvar (socket-condvar socket)))
    (let loop ((retry? #f))
      (disable-interrupts!)
      (cond ((op channel retry?)
	     => (lambda (result)
		  (enable-interrupts!)
		  result))
	    (else
	     (wait-for-channel channel condvar)
	     (with-new-proposal (lose)
	       (maybe-commit-and-wait-for-condvar condvar))
	     (enable-interrupts!)
	     (loop #t))))))

;----------------
; We need to explicitly close socket channels.

(define-enumeration shutdown-option
  (read write read/write)
  shutdown-option-set)

(define shutdown-option->raw (enum-set-indexer (shutdown-option-set)))

(define (shutdown-socket socket how)
  (shutdown-socket-channel (socket-channel socket) how))

(define (shutdown-socket-channel channel how)
  (external-shutdown channel (shutdown-option->raw how)))

(import-lambda-definition external-shutdown (channel how)
			  "s48_shutdown")

(define (close-socket-input-channel channel)
  (shutdown-socket-channel channel (shutdown-option read))
  (close-channel channel))

(define (close-socket-output-channel channel)
  (shutdown-socket-channel channel (shutdown-option write))
  (close-channel channel))

(define (socket-address socket)
  (raw->socket-address
   (external-getsockname (socket-channel socket))))

(import-lambda-definition external-getsockname (channel)
			  "s48_getsockname")

(define (socket-peer-address socket)
  (raw->socket-address
   (external-getpeername (socket-channel socket))))

(import-lambda-definition external-getpeername (channel)
			  "s48_getpeername")


(define-syntax define-socket-option-setter
  (syntax-rules ()
    ((define-socket-option-setter ?name ?external-name)
     (begin
       (define (?name socket val)
	 (external-setsockopt (socket-channel socket) val))
       
       (import-lambda-definition external-setsockopt (channel val)
				 ?external-name)))))

(define-syntax define-socket-option-getter
  (syntax-rules ()
    ((define-socket-option-getter ?name ?external-name)
     (begin
       (define (?name socket)
	 (external-getsockopt (socket-channel socket)))
       
       (import-lambda-definition external-getsockopt (channel)
				 ?external-name)))))


(define-socket-option-setter set-socket-debug?!
  "s48_setsockopt_SO_DEBUG")
(define-socket-option-getter socket-debug?!
  "s48_getsockopt_SO_DEBUG")
(define-socket-option-setter set-socket-accept-connections?!
  "s48_setsockopt_SO_ACCEPTCONN")
(define-socket-option-getter socket-accept-connections?
  "s48_getsockopt_SO_ACCEPTCONN")
(define-socket-option-setter set-socket-broadcast?!
  "s48_setsockopt_SO_BROADCAST")
(define-socket-option-getter socket-broadcast?
  "s48_getsockopt_SO_BROADCAST")
(define-socket-option-setter set-socket-reuse-address?!
  "s48_setsockopt_SO_REUSEADDR")
(define-socket-option-getter socket-reuse-address?
  "s48_getsockopt_SO_REUSEADDR")
(define-socket-option-setter set-socket-keepalive?!
  "s48_setsockopt_SO_KEEPALIVE")
(define-socket-option-getter socket-keepalive?
  "s48_getsockopt_SO_KEEPALIVE")
(define-socket-option-setter set-socket-oob-inline?!
  "s48_setsockopt_SO_OOBINLINE")
(define-socket-option-getter socket-oob-inline?
  "s48_getsockopt_SO_OOBINLINE")
(define-socket-option-setter set-socket-send-buffer-size!
  "s48_setsockopt_SO_SNDBUF")
(define-socket-option-getter socket-send-buffer-size
  "s48_getsockopt_SO_SNDBUF")
(define-socket-option-setter set-socket-receive-buffer-size!
  "s48_setsockopt_SO_RCVBUF")
(define-socket-option-getter socket-receive-buffer-size
  "s48_getsockopt_SO_RCVBUF")
(define-socket-option-getter socket-error
  "s48_getsockopt_SO_ERROR")
(define-socket-option-setter set-socket-dontroute?!
  "s48_setsockopt_SO_DONTROUTE")
(define-socket-option-getter socket-dontroute?
  "s48_getsockopt_SO_DONTROUTE")
(define-socket-option-setter set-socket-minimum-receive-count!
  "s48_setsockopt_SO_RCVLOWAT")
(define-socket-option-getter socket-minimum-receive-count
  "s48_getsockopt_SO_RCVLOWAT")
(define-socket-option-setter set-socket-minimum-send-count!
  "s48_setsockopt_SO_SNDLOWAT")
(define-socket-option-getter socket-minimum-send-count
  "s48_getsockopt_SO_SNDLOWAT")

(define-socket-option-setter set-socket-ipv6-unicast-hops!
  "s48_setsockopt_IPV6_UNICAST_HOPS")
(define-socket-option-getter socket-ipv6-unicast-hops
  "s48_getsockopt_IPV6_UNICAST_HOPS")
(define-socket-option-setter set-socket-ipv6-multicast-interface!
  "s48_setsockopt_IPV6_MULTICAST_IF")
(define-socket-option-getter socket-ipv6-multicast-interface
  "s48_getsockopt_IPV6_MULTICAST_IF")
(define-socket-option-setter set-socket-ipv6-multicast-hops!
  "s48_setsockopt_IPV6_MULTICAST_HOPS")
(define-socket-option-getter socket-ipv6-multicast-hops
  "s48_getsockopt_IPV6_MULTICAST_HOPS")
(define-socket-option-setter set-socket-ipv6-multicast-loop?!
  "s48_setsockopt_IPV6_MULTICAST_LOOP")
(define-socket-option-getter socket-ipv6-multicast-loop?
  "s48_getsockopt_IPV6_MULTICAST_LOOP")

(define (socket-ipv6-join-group! socket address interface)
  (external-ipv6-socket-join-group (socket-channel socket)
				   (socket-address-raw address)
				   (interface-index interface)))

(import-lambda-definition external-ipv6-socket-join-group (channel address if-index)
			  "s48_ipv6_socket_join_group")

(define (socket-ipv6-leave-group! socket address interface)
  (external-ipv6-socket-leave-group (socket-channel socket)
				    (socket-address-raw address)
				    (interface-index interface)))

(import-lambda-definition external-ipv6-socket-leave-group (channel address if-index)
			  "s48_ipv6_socket_leave_group")
			  

; Messages

(define-enumeration message-option
  (oob peek dontroute)
  message-options)

(define socket-send
  (opt-lambda (socket
	       buffer
	       (start 0)
	       (count (byte-vector-length buffer))
	       (address (socket-peer-address socket)) ; cache this?
	       (flags (message-options)))
    (blocking-socket-op socket
			(lambda (channel retry?)
			  (external-sendto channel buffer start count
					   (enum-set->integer flags)
					   (socket-address-raw address)
					   retry?)))))

(import-lambda-definition external-sendto (channel
					   buffer start count flags address
					   retry?)
			  "s48_sendto")

(define socket-receive
  (opt-lambda (socket
	       buffer
	       (start 0)
	       (count (byte-vector-length buffer))
	       (want-sender? #t)
	       (flags (message-options)))
    (let ((got
	   (blocking-socket-op socket
			       (lambda (channel retry?)
				 (external-recvfrom channel buffer start count
						    (enum-set->integer flags)
						    want-sender?
						    retry?)))))
      (if want-sender?
	  (values (car got) (raw->socket-address (cdr got)))
	  got))))

(import-lambda-definition external-recvfrom (channel
					     buffer start count flags
					     want-sender? retry?)
			  "s48_recvfrom")
