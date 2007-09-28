; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-structure external-enum-types
  (export (define-external-enum-type-with-unknowns :syntax))
  (open scheme
	define-record-types
	r6rs-enums enum-sets-internal
	os-strings)
  (files external-enum-type))

(define-interface net-addresses-interface
  (export make-unknown-address-family unknown-address-family?
	  unknown-address-family-number
	  (address-family :syntax)

	  make-ipv4-address ipv4-address?
	  ipv4-address-any ipv4-address-broadcast
	  ipv4-address-ip
	  make-ipv4-socket-address
	  ipv4-socket-address?
	  socket-address-ipv4-address
	  socket-address-ipv4-port

	  make-ipv6-address ipv6-address?
	  ipv6-address-any ipv6-address-loopback
	  make-ipv6-socket-address ipv6-socket-address?
	  socket-address-ipv6-address socket-address-ipv6-port
	  socket-address-ipv6-scope-id

	  make-unix-socket-address unix-socket-address?
	  socket-address-unix-path

	  get-address-info
	  (address-info-flags :syntax)
	  (ip-protocol :syntax)
	  make-unknown-ip-protocol unknown-ip-protocol?

	  address-info?
	  address-info-family
	  address-info-socket-type address-info-protocol
	  address-info-canonical-name address-info-socket-address

	  get-name-info
	  (name-info-flags :syntax)

	  (socket-type :syntax)
	  make-unknown-socket-type unknown-socket-type?

	  address->string string->address

	  ipv6-address-unspecified?
	  ipv6-address-loopback?
	  ipv6-address-multicast?
	  ipv6-address-link-local?
	  ipv6-address-site-local?
	  ipv6-address-v4-mapped?
	  ipv6-address-v4-compat?

	  ipv6-address-multicast-unspecified?
	  ipv6-address-multicast-unspecified?
	  ipv6-address-multicast-org-local?
	  ipv6-address-multicast-global?

	  interface?
	  index->interface name->interface
	  interface-index interface-name
	  get-all-interfaces))

(define-interface net-addresses-internal-interface
  (export raw->socket-address
	  address-family->raw
	  socket-type->raw
	  socket-address-raw
	  ))

(define-structures ((net-addresses net-addresses-interface)
		    (net-addresses-internal net-addresses-internal-interface))
  (open scheme
	variable-argument-lists
	external-calls external-events
	define-record-types
	bitwise byte-vectors
	os-strings
	external-enum-types
	reinitializers
	r6rs-enums enum-sets-internal
	exceptions)
  (files address))

(define net-sockets-interface
  (export make-socket socket?
	  make-socket-pair
	  socket-address-family socket-socket-type
	  dup-socket socket-accept close-socket
	  bind-socket socket-listen
	  socket-connect
	  socket-input-port socket-output-port
	  shutdown-socket
	  socket-address socket-peer-address
	  (message-options :syntax)
	  socket-send socket-receive

	  set-socket-debug?!
	  socket-debug?!
	  set-socket-accept-connections?!
	  socket-accept-connections?
	  set-socket-broadcast?!
	  socket-broadcast?
	  set-socket-reuse-address?!
	  socket-reuse-address?
	  set-socket-keepalive?!
	  socket-keepalive?
	  set-socket-oob-inline?!
	  socket-oob-inline?
	  set-socket-send-buffer-size!
	  socket-send-buffer-size
	  set-socket-receive-buffer-size!
	  socket-receive-buffer-size
	  socket-error
	  set-socket-dontroute?!
	  socket-dontroute?
	  set-socket-minimum-receive-count!
	  socket-minimum-receive-count
	  set-socket-minimum-send-count!
	  socket-minimum-send-count

	  set-socket-ipv6-unicast-hops!
	  socket-ipv6-unicast-hops
	  set-socket-ipv6-multicast-interface!
	  socket-ipv6-multicast-interface
	  set-socket-ipv6-multicast-hops!
	  socket-ipv6-multicast-hops
	  set-socket-ipv6-multicast-loop?!
	  socket-ipv6-multicast-loop?

	  socket-ipv6-join-group!
	  socket-ipv6-leave-group!  
	  ))

(define-structure net-sockets net-sockets-interface
  (open scheme
	variable-argument-lists
	byte-vectors
	external-calls 
	proposals condvars interrupts
	channels channel-i/o channel-ports
	define-record-types
	r6rs-enums enum-sets-internal
	exceptions
	net-addresses net-addresses-internal)
  (files socket))

(define-structures ((sockets (export open-socket
				     close-socket
				     socket-accept
				     socket-port-number
				     socket-client))
		    (udp-sockets (export close-socket
					 open-udp-socket
					 udp-send
					 udp-receive
					 lookup-udp-address
					 socket-port-number
					 udp-address?
					 udp-address-address
					 udp-address-port)))
  (open scheme
	variable-argument-lists
	net-addresses
	(modify net-sockets (rename (socket-accept net:socket-accept))))
  (files big-socket))
