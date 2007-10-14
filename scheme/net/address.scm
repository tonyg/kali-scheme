; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; We still need IDNA support.

(define-external-enum-type-with-unknowns address-family
  (inet inet6 unix unspec)
  unknown-address-family :unknown-address-family
  make-unknown-address-family unknown-address-family? unknown-address-family-number
  100
  address-family? address-family->raw raw->address-family)

;; IPv4

(define-record-type ipv4-address :ipv4-address
  (really-make-ipv4-address ip)
  ipv4-address?
  ;; 32-bit number
  (ip ipv4-address-ip
      set-ipv4-address-ip!)) ; internal use only

(define (split-ip ip)
  (list (arithmetic-shift ip -24)
	(bitwise-and (arithmetic-shift ip -16) #xFF)
	(bitwise-and (arithmetic-shift ip -8) #xFF)
	(bitwise-and ip #xFF)))

(define-record-discloser :ipv4-address
  (lambda (r)
    (cons 'ipv4-address (split-ip (ipv4-address-ip r)))))

;; This works the same way as the dot notation for IP addresses
(define (make-ipv4-address a . rest)
  (really-make-ipv4-address
   (cond
    ((null? rest) a)
    ((null? (cdr rest))
     (bitwise-ior (arithmetic-shift a 24) 
		  (car rest)))
    ((null? (cddr rest))
     (bitwise-ior (arithmetic-shift a 24)
		  (arithmetic-shift (car rest) 16)
		  (cadr rest)))
    (else
     (bitwise-ior (arithmetic-shift a 24)
		  (arithmetic-shift (car rest) 16)
		  (arithmetic-shift (cadr rest) 8)
		  (caddr rest))))))

(import-lambda-definition external-get-inaddr-any () "s48_get_inaddr_any")
(import-lambda-definition external-get-inaddr-broadcast () "s48_get_inaddr_broadcast")

(define *ipv4-address-any*
  (make-ipv4-address (external-get-inaddr-any)))
(define *ipv4-address-broadcast*
  (make-ipv4-address (external-get-inaddr-broadcast)))
(define-reinitializer ipv4-predefined-addresses
  (lambda ()
    (set-ipv4-address-ip! *ipv4-address-any* (external-get-inaddr-any))
    (set-ipv4-address-ip! *ipv4-address-broadcast* (external-get-inaddr-broadcast))))

(define (ipv4-address-any) *ipv4-address-any*)
(define (ipv4-address-broadcast) *ipv4-address-broadcast*)

;; IPv6

(define-record-type ipv6-address :ipv6-address
  (make-ipv6-address elements)
  ipv6-address?
  ;; bytevector with 16 elements
  (elements ipv6-address-elements
	    set-ipv6-address-elements!)) ; internal use

(define-record-discloser :ipv6-address
  (lambda (r)
    (list 'ipv6-address (ipv6-address-elements r))))

(import-lambda-definition external-get-in6addr-any () "s48_get_in6addr_any")
(import-lambda-definition external-get-in6addr-loopback () "s48_get_in6addr_loopback")

(define *ipv6-address-any*
  (make-ipv6-address (external-get-in6addr-any)))
(define *ipv6-address-loopback*
  (make-ipv6-address (external-get-in6addr-loopback)))
(define-reinitializer ipv6-predefined-addresses
  (lambda ()
    (set-ipv6-address-elements! *ipv6-address-any* (external-get-in6addr-any))
    (set-ipv6-address-elements! *ipv6-address-loopback* (external-get-in6addr-loopback))))

(define (ipv6-address-any) *ipv6-address-any*)
(define (ipv6-address-loopback) *ipv6-address-loopback*)

;; Socket addresses

(define-record-type socket-address :socket-address
  (make-socket-address family data raw)
  socket-address?
  (family socket-address-family)
  ;; #f or address-family-specific object
  (data socket-address-data)
  ;; external value containing the sockaddr_storage object
  (raw real-socket-address-raw set-socket-address-raw!))

(define-record-resumer :socket-address
  (lambda (r)
    (set-socket-address-raw! r #f)))

(define (socket-address-raw sa)
  (or (real-socket-address-raw sa)
      (cond
       ((socket-address-data sa)
	=> (lambda (data)
	     (let ((raw (make-socket-address-raw data)))
	       (set-socket-address-raw! sa raw)
	       raw)))
       (else
	(assertion-violation 'socket-address-raw
			     "socket address of unknown address family couldn't be resumed"
			     sa)))))

(define-record-discloser :socket-address
  (lambda (r)
    (list 'socket-address (socket-address-data r))))

(define (make-socket-address-raw data)
  (cond
   ((socket-address-data/ipv4? data)
    (socket-address-data/ipv4->raw data))
   ((socket-address-data/ipv6? data)
    (socket-address-data/ipv6->raw data))
   ((socket-address-data/unix? data)
    (socket-address-data/unix->raw data))
   (else
    (assertion-violation 'make-socket-address-raw
			 "unknown socket-address data"
			 data))))

;; IPv4

(define-record-type socket-address-data/ipv4 :socket-address-data/ipv4
  (make-socket-address-data/ipv4 address port)
  socket-address-data/ipv4?
  (address socket-address-data/ipv4-address)
  (port socket-address-data/ipv4-port))

(define-record-discloser :socket-address-data/ipv4
  (lambda (r)
    (list 'socket-address-data/ipv4
	  (socket-address-data/ipv4-address r)
	  (socket-address-data/ipv4-port r))))

(define (make-ipv4-socket-address address port)
  (make-socket-address
   (address-family inet)
   (make-socket-address-data/ipv4 address port)
   #f))

(define (ipv4-socket-address? obj)
  (and (socket-address? obj)
       (socket-address-data/ipv4? (socket-address-data obj))))

(define (socket-address-data/ipv4->raw data)
  (external-make-sockaddr-in-raw
   (ipv4-address-ip (socket-address-data/ipv4-address data))
   (socket-address-data/ipv4-port data)))

(import-lambda-definition external-make-sockaddr-in-raw (addr port)
			  "s48_make_sockaddr_in_raw")


(define (socket-address-ipv4-address sa)
  (socket-address-data/ipv4-address (socket-address-data sa)))
(define (socket-address-ipv4-port sa)
  (socket-address-data/ipv4-port (socket-address-data sa)))

;; IPv6

(define-record-type socket-address-data/ipv6 :socket-address-data/ipv6
  (make-socket-address-data/ipv6 address port scope-id)
  socket-address-data/ipv6?
  (address socket-address-data/ipv6-address)
  (port socket-address-data/ipv6-port)
  (scope-id socket-address-data/ipv6-scope-id))

(define-record-discloser :socket-address-data/ipv6
  (lambda (r)
    (list 'socket-address-data/ipv6
	  (socket-address-data/ipv6-address r)
	  (socket-address-data/ipv6-port r)
	  (socket-address-data/ipv6-scope-id r))))

(define (make-ipv6-socket-address port address scope-id)
  (make-socket-address
   (address-family inet6)
   (make-socket-address-data/ipv6 address port scope-id)
   #f))

(define (ipv6-socket-address? obj)
  (and (socket-address? obj)
       (socket-address-data/ipv6? (socket-address-data obj))))

(define (socket-address-data/ipv6->raw data)
  (external-make-sockaddr-in6-raw
   (socket-address-data/ipv6-port data)
   (ipv6-address-elements (socket-address-data/ipv6-address data))
   (socket-address-data/ipv6-scope-id data)))

(import-lambda-definition external-make-sockaddr-in6-raw (addr port scope-id)
			  "s48_make_sockaddr_in6_raw")

(define (socket-address-ipv6-address sa)
  (socket-address-data/ipv6-address (socket-address-data sa)))
(define (socket-address-ipv6-port sa)
  (socket-address-data/ipv6-address (socket-address-data sa)))
(define (socket-address-ipv6-scope-id sa)
  (socket-address-data/ipv6-scope-id (socket-address-data sa)))

;; Unix domain

(define-record-type  socket-address-data/unix :socket-address-data/unix
  (make-socket-address-data/unix path)
  socket-address-data/unix?
  ;; OS-string
  (path socket-address-data/unix-path))

(define-record-discloser :socket-address-data/unix
  (lambda (r)
    (list 'socket-address-data/unix
	  (socket-address-data/unix-path r))))

(define (make-unix-socket-address path)
  (make-socket-address
   (address-family unix)
   (make-socket-address-data/unix (x->os-string path))
   #f))

(define (unix-socket-address? obj)
  (and (socket-address? obj)
       (socket-address-data/unix? (socket-address-data obj))))

(define (socket-address-data/unix->raw data)
  (external-make-sockaddr-un-raw
   (os-string->byte-vector (socket-address-data/unix-path data))))

(import-lambda-definition external-make-sockaddr-un-raw (path)
			  "s48_make_sockaddr_un_raw")

(define (socket-address-unix-path sa)
  (socket-address-data/unix-path (socket-address-data sa)))

;; Generic

(define (raw->socket-address raw)
  (let* ((family (raw->address-family (vector-ref raw 1)))
	 (data
	  (case family
	    ((inet)
	     (make-socket-address-data/ipv4
	      (make-ipv4-address (vector-ref raw 3))
	      (vector-ref raw 2)))
	    ((inet6)
	     (make-socket-address-data/ipv6
	      (vector-ref raw 2)
	      (make-ipv6-address (vector-ref raw 3))
	      (vector-ref raw 4)))
	    ((unix)
	     (make-socket-address-data/unix
	      (byte-vector->os-string (vector-ref raw 2))))
	    (else #f))))
    (make-socket-address family data (vector-ref raw 0))))


;; Interfaces

(define-record-type interface :interface
  (make-interface name index)
  interface?
  (name interface-name)
  (index interface-index))

(define-record-discloser :interface
  (lambda (r)
    (list 'interface
	  (interface-name r) (interface-index r))))

(define (index->interface idx)
  (if (and (integer? idx) (exact? idx) (positive? idx))
      (make-interface (external-interface-index->name idx)
		      idx)
      (assertion-violation 'index->interface "invalid argument" idx)))

(define (name->interface name)
  (let ((index (external-interface-name->index name)))
    (if (zero? index)
	#f
	(make-interface name index))))

(define (get-all-interfaces)
  (let* ((v (external-interface-index-table))
	 (count (quotient (vector-length v) 2)))
    (let loop ((i 0) (rev '()))
      (if (>= i count)
	  (reverse rev)
	  (loop (+ 1 i)
		(cons (make-interface (vector-ref v (+ 1 (* i 2)))
				      (vector-ref v (* i 2)))
		      rev))))))
      
(import-lambda-definition external-interface-name->index (name)
			  "s48_if_nametoindex")
(import-lambda-definition external-interface-index->name (index)
			  "s48_if_indextoname")
(import-lambda-definition external-interface-index-table ()
			  "s48_if_nameindex")

; Nodename translation

(define-enumeration address-info-flag
  (passive
   canonname
   numerichost)
  address-info-flags)

(define address-info-flag-set-type (enum-set-type (address-info-flags)))

(define-external-enum-type-with-unknowns ip-protocol
  (ip ipv6 icmp raw tcp udp)
  unknown-ip-protocol :unknown-ip-protocol
  make-unknown-ip-protocol unknown-ip-protocol? unknown-ip-protocol-number
  100
  ip-protocol? ip-protocol->raw raw->ip-protocol)

(define-external-enum-type-with-unknowns socket-type
  (stream dgram raw seqpacket)
  unknown-socket-type :unknown-socket-type
  make-unknown-socket-type unknown-socket-type? unknown-socket-type-number
  100
  socket-type? socket-type->raw raw->socket-type)

(define-record-type address-info :address-info
  (make-address-info family socket-type protocol
		     canonical-name socket-address)
  address-info?
  (family address-info-family)
  (socket-type address-info-socket-type)
  (protocol address-info-protocol)
  (canonical-name address-info-canonical-name)
  (socket-address address-info-socket-address))

(define-record-discloser :address-info
  (lambda (r)
    (list 'address-info
	  (address-info-family r)
	  (address-info-socket-type r)
	  (address-info-protocol r)
	  (address-info-canonical-name r)
	  (address-info-socket-address r))))

(define (raw->address-info raw)
  (make-address-info (raw->address-family (vector-ref raw 0))
		     (raw->socket-type (vector-ref raw 1))
		     (raw->ip-protocol (vector-ref raw 2))
		     (vector-ref raw 3)
		     (raw->socket-address (vector-ref raw 4))))

(define (get-xxx-info retval get-result)
  (if (pair? retval)
      (let ((result #f))
	(dynamic-wind ; we need to release the uid in case the thread gets killed
	    values
	    (lambda ()
	      (wait-for-external-event (car retval)))
	    (lambda ()
	      (set! result (get-result (cdr retval)))))
	result)
      retval))

(define get-address-info
  (opt-lambda (node
	       (server #f)
	       (hint-flags (address-info-flags))
	       (hint-family (address-family unspec))
	       (hint-socket-type #f)
	       (hint-protocol 'f))
    (cond
     ((get-xxx-info
       (external-getaddrinfo
	node server
	(enum-set->integer hint-flags)
	(address-family->raw hint-family)
	(and hint-socket-type
	     (socket-type->raw hint-socket-type))
	(and hint-protocol
	     (ip-protocol->raw hint-protocol)))
       external-getaddrinfo-result)
      => (lambda (result)
	   (map raw->address-info
		(vector->list result))))
     (else #f))))

(import-lambda-definition external-getaddrinfo (nodename
						servname
						hint-flags hint-family
						hint-socktype hint-protocol)
			  "s48_getaddrinfo")
(import-lambda-definition external-getaddrinfo-result (handshake)
			  "s48_getaddrinfo_result")

(define-enumeration name-info-flag
  (nofqdn numerichost namereqd numericserv dgram)
  name-info-flags)

(define get-name-info
  (opt-lambda (socket-address (flags (name-info-flags)))
    (let ((p (get-xxx-info
	      (external-getnameinfo
	       (socket-address-raw socket-address)
	       (enum-set->integer flags))
	      external-getnameinfo-result)))
      (values (vector-ref p 0) (vector-ref p 1)))))

(import-lambda-definition external-getnameinfo (sock-address flags)
			  "s48_getnameinfo")
(import-lambda-definition external-getnameinfo-result (handshake)
			  "s48_getnameinfo_result")

;; Address conversion

(define (address->string addr)
  (cond
   ((ipv4-address? addr)
    (external-inet-ntop (address-family->raw (address-family inet))
			(ipv4-address-ip addr)))
   ((ipv6-address? addr)
    (external-inet-ntop (address-family->raw (address-family inet6))
			(ipv6-address-elements addr)))
   (else
    (assertion-violation 'address->string "invalid address" addr))))

(define (string->address family rep)
  (let ((make
	    (case family
	      ((inet) make-ipv4-address)
	      ((inet6) make-ipv6-address)
	      (else
	       (assertion-violation 'string->address "invalid address family"
				    family)))))
    (cond
     ((external-inet-pton (address-family->raw family) rep) => make)
     (else #f))))

(import-lambda-definition external-inet-pton (family rep)
			  "s48_inet_pton")
(import-lambda-definition external-inet-ntop (family address)
			  "s48_inet_ntop")

;; Address testing

(define-syntax define-address-predicate
  (syntax-rules ()
    ((define-address-predicate ?name ?external-name)
     (begin
       (import-lambda-definition external? (address) ?external-name)
       (define (?name addr)
	 (external? (ipv6-address-elements addr)))))))

(define-address-predicate ipv6-address-unspecified? "s48_IN6_IS_ADDR_UNSPECIFIED")
(define-address-predicate ipv6-address-loopback? "s48_IN6_IS_ADDR_LOOPBACK")
(define-address-predicate ipv6-address-multicast? "s48_IN6_IS_ADDR_MULTICAST")
(define-address-predicate ipv6-address-link-local? "s48_IN6_IS_ADDR_LINKLOCAL")
(define-address-predicate ipv6-address-site-local? "s48_IN6_IS_ADDR_SITELOCAL")
(define-address-predicate ipv6-address-v4-mapped? "s48_IN6_IS_ADDR_V4MAPPED")
(define-address-predicate ipv6-address-v4-compat? "s48_IN6_IS_ADDR_V4COMPAT")

(define-address-predicate ipv6-address-multicast-unspecified?
  "s48_IN6_IS_ADDR_MC_NODELOCAL")
(define-address-predicate ipv6-address-multicast-unspecified?
  "s48_IN6_IS_ADDR_MC_LINKLOCAL")
(define-address-predicate ipv6-address-multicast-org-local?
  "s48_IN6_IS_ADDR_MC_ORGLOCAL")
(define-address-predicate ipv6-address-multicast-global?
  "s48_IN6_IS_ADDR_MC_GLOBAL")

