; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Server interface
;   (open-socket [socket-number]) -> socket
;   (close-socket socket)
;   (socket-accept socket) -> [input-port output-port]
;   (get-host-name) -> string
;   (socket-port-number socket) -> integer

; Client interface
;   (socket-client host-name socket-number) -> [input-port output-port]

; Old calls I would like to get rid off.
;   (socket-listen socket) -> [input-port output-port]
;   (socket-listen-channels socket) -> [input-channel output-channel]
;   (socket-client-channels host-name socket-number) -> [input-channels output-channels]

;--------------------
; Socket type
;
; A socket has a channel (for accepting connections) and a port number.
; These are only used for servers; clients don't need them.

(define-record-type socket :socket
  (make-socket channel port-number)
  socket?
  (channel socket-channel)
  (port-number socket-port-number))

(define-record-discloser :socket
  (lambda (s) `(socket ,(socket-port-number s))))

(define (close-socket socket)
  (close-channel (socket-channel socket)))

; Makes a server socket.

(define (open-socket . maybe-number)
  (let ((channel (new-socket #t)))
    (bind-socket channel (if (or (null? maybe-number)
				 (= (car maybe-number) 0))  ; old, crappy spec
			     #f
			     (car maybe-number)))
    (real-socket-listen channel)
    (make-socket channel (socket-number channel))))

(define (socket-accept socket)
  (call-with-values
   (lambda ()
     (socket-listen-channels socket))
   (lambda (in out)
     (values (input-channel+closer->port in close-socket-input-channel)
	     (output-channel+closer->port out close-socket-output-channel)))))
  
(define socket-listen socket-accept)

(define (socket-listen-channels socket)
  (let ((channel (socket-channel socket)))
    (let loop ()
      (disable-interrupts!)
      (let ((channels (real-socket-accept channel)))
	(cond (channels
	       (enable-interrupts!)
	       (values (car channels)
		       (cdr channels)))
	      (else
	       (wait-for-channel channel)		; enables interrupts
	       (loop)))))))

; Connect to the socket and return input and output ports.

(define (socket-client host-name port-number)
  (call-with-values
   (lambda ()
     (socket-client-channels host-name port-number))
   (lambda (in out)
     (values (input-channel+closer->port in close-socket-input-channel)
	     (output-channel+closer->port out close-socket-output-channel)))))

(define (socket-client-channels host-name port-number)
  (let ((channel (new-socket #f)))
    (let loop ()
      (disable-interrupts!)
      (let ((output-channel (real-socket-connect channel host-name port-number)))
	(cond ((channel? output-channel)
	       (enable-interrupts!)
	       (values channel output-channel))
	      ; This should never happen.
	      ((eq? output-channel #t)
	       (error "client socket already connected" host-name port-number))
	      (else
	       (wait-for-channel channel) ; enables interrupts
	       (loop)))))))

; We need to explicitly close the channel.

(define (close-socket-input-channel channel)
  (close-socket-half channel #t)
  (close-channel channel))

(define (close-socket-output-channel channel)
  (close-socket-half channel #f)
  (close-channel channel))

; The C calls we use.  These are in c/unix/socket.c.

(import-lambda-definition new-socket (server?) "s48_socket")
(import-lambda-definition bind-socket (socket number) "s48_bind")
(import-lambda-definition socket-number (socket) "s48_socket_number")
(import-lambda-definition real-socket-listen (socket) "s48_listen")
(import-lambda-definition real-socket-accept (socket) "s48_accept")
(import-lambda-definition real-socket-connect (socket machine port-number)
			  "s48_connect")
(import-lambda-definition close-socket-half (socket input?)
			  "s48_close_socket_half")
(import-lambda-definition get-host-name () "s48_get_host_name")
