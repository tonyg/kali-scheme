; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Using sockets from Scheme 48.

; Server.  To make your program be a server, do
;  (call-with-values socket-server
;    (lambda (port-number accept)
;      (display "Port number is ") (write port-number) (newline)
;      (call-with-values accept
;        (lambda (i-port o-port)
;          ... have fun ...))))

(define (socket-server)
  (let ((s (internet-stream-socket)))
    (values (socket-bind s)
	    (lambda ()
	      (fd->ports (socket-accept s))))))

; Client.  To make your program be a client, do
;  (call-with-values (lambda ()
;                      (socket-client "foo.mit.edu" port-number))
;    (lambda (i-port o-port)
;      ... have fun ...))

(define (socket-client host-name port-number)
  (fd->ports (socket-connect host-name port-number)))


; vm-extension calls

(define (internet-stream-socket)
  (vm-extension 20 #f))

(define (socket-bind s)
  (vm-extension 21 s))

(define (socket-accept s)
  (vm-extension 22 s))

(define (socket-connect host-name port-number)
  (vm-extension 23 (cons host-name port-number)))

(define (fd->ports fd)
  (values (fd->input-port fd) (fd->output-port fd)))

(define (fd->input-port fd)
  (let ((port (open-input-file "/dev/null")))
    (vm-extension 24 (cons port fd))
    port))

(define (fd->output-port fd)
  (let ((port (open-output-file "/dev/null")))
    (vm-extension 25 (cons port fd))
    port))
