; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Emulation of old socket interface.

; Server interface
;   (open-socket [socket-number]) -> socket
;   (close-socket socket)
;   (socket-accept socket) -> [input-port output-port]
;   (get-host-name) -> string
;   (socket-port-number socket) -> integer

; Client interface
;   (socket-client host-name socket-number) -> [input-port output-port]

(define open-socket
  (opt-lambda ((port 0))
    (let ((sock (make-socket (address-family inet) (socket-type stream))))
      (set-socket-reuse-address?! sock #t)
      (bind-socket sock (make-ipv4-socket-address (ipv4-address-any) port))
      (socket-listen sock)
      sock)))

(define (socket-port-number sock)
  (socket-address-ipv4-port (socket-address sock)))
    
(define (socket-client host-name port)
  (let* ((ai
	  (car
	   (get-address-info host-name #f
			     (address-info-flags) (address-family inet)
			     (socket-type stream))))
	 (sa (address-info-socket-address ai))
	 (address (socket-address-ipv4-address sa))
	 (socket (make-socket (address-family inet)
			      (socket-type stream))))
    (socket-connect socket
		    (make-ipv4-socket-address address port))
    (values (socket-input-port socket)
	    (socket-output-port socket))))

(define (socket-accept socket)
  (let ((newsock (net:socket-accept socket)))
    (values (socket-input-port newsock)
	    (socket-output-port newsock))))

; UDP sockets

; Open a UDP socket, returning the two sides.  If a socket port is specified
; it is given to the input half.

(define open-udp-socket
  (opt-lambda ((port 0))
    (let ((sock (make-socket (address-family inet) (socket-type dgram))))
      (set-socket-reuse-address?! sock #t)
      (bind-socket sock (make-ipv4-socket-address (ipv4-address-any) port))
      (values sock (dup-socket sock)))))

(define (udp-send socket address buffer count)
  (socket-send socket buffer 0 count address))

(define (udp-receive socket buffer)
  (socket-receive socket buffer))

(define udp-address? ipv4-socket-address?)
(define udp-address-address socket-address-ipv4-address)
(define udp-address-port socket-address-ipv4-port)
;; The old code would cache these addresses.
(define (lookup-udp-address host-name port)
  (let* ((sa
	  (address-info-socket-address
	   (car
	    (get-address-info host-name #f
			      (address-info-flags) (address-family inet)
			      (socket-type dgram)))))
	 (inet (socket-address-ipv4-address sa)))
    (make-ipv4-socket-address inet port)))

