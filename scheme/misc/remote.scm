; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is a small mock-up of the Cornell mobile robot system.
; It is not intended as a remote procedure call mechanism.

; ,load-config =scheme48/misc/packages.scm
; ,open remote

; To start a server, do
;   (define sock (make-socket))
;   (serve sock)
; To start a client, do
;   (remote-repl "hostname" <number>)
; where <number> is the number displayed by the server when it starts up.


; Server side

(define (note-structure-locations! s)
  (define (recur name env trail)
    (let ((b (generic-lookup env name)))
      (if (binding? b)
	  (begin
	    (note-location! (binding-place b))
	    (let ((t (binding-static b)))
	      (if (and (transform? t) (not (member t trail)))
		  (let ((trail (cons t trail))
			(env (transform-env t)))
		    (for-each (lambda (name)
				(recur name env trail))
			      (transform-aux-names (binding-static b))))))))))
  (for-each-declaration (lambda (name package-name type)
			  (recur package-name s '())) 
			(structure-interface s)))

(note-structure-locations! scheme-level-2)

(define (make-socket)
  (call-with-values socket-server cons))

(define (serve sock)
  (let ((port-number (car sock))
	(accept (cdr sock)))
    (display "Port number is ")
    (write port-number)
    (newline)
    (let ((in #f)
	  (out #f))
      (dynamic-wind (lambda ()
		      (call-with-values accept
			(lambda (i-port o-port)
			  (display "Open") (newline)
			  (set! in i-port)
			  (set! out o-port))))
		    (lambda ()
		      (start-server in out))
		    (lambda ()
		      (if in (close-input-port in))
		      (if out (close-output-port out)))))))

(define (start-server in out)
  (let loop ()
    (let ((message (restore-carefully in)))
      (case (car message)
	((run)
	 (dump (run-carefully (cdr message))
	       (lambda (c) (write-char c out))
	       -1)
	 (force-output out)
	 (loop))
	((eof) (cdr message))
	(else (error 'start-server "unrecognized message" message))))))

(define (run-carefully template)
  (call-with-current-continuation
    (lambda (escape)
      (with-handler
	  (lambda (c punt)
	    (if (serious-condition? c)
		(escape (cons 'condition c))
		(punt)))
	(lambda ()
	  (call-with-values (lambda ()
			      (invoke-closure (make-closure template #f)))
	    (lambda vals
	      (cons 'values vals))))))))


; Client side

(define (make-remote-eval in out)
  (lambda (form p)
    (compile-and-run-forms (list form)
			   p
			   #f
			   (lambda (template)
			     (dump (cons 'run template)
				   (lambda (c) (write-char c out))
				   -1)
			     (force-output out)
			     (let ((reply (restore-carefully in)))
			       (case (car reply)
				 ((values)
				  (apply values (cdr reply)))
				 ((condition)
				  (signal-condition (cdr reply)))
				 ((eof)
				  (error 'make-remote-eval "eof on connection")))))
			   #f)))


(define (make-remote-package in out opens id)
  (let ((p (make-simple-package opens
 				#t
 				(reflective-tower
 				 (package->environment (interaction-environment)))
 				id)))
    (set-package-evaluator! p (make-remote-eval in out))
    p))

(define (remote-repl host-name socket-port-number)
  (let ((in #f) (out #f))
    (dynamic-wind
     (lambda ()
       (call-with-values (lambda ()
			   (socket-client host-name socket-port-number))
	 (lambda (i-port o-port)
	   (set! in i-port)
	   (set! out o-port))))
     (lambda ()
       (with-interaction-environment (make-remote-package in out (list scheme) 'remote)
				     (lambda () (command-loop list #f))))
     (lambda ()
       (if in (close-input-port in))
       (if out (close-output-port out))))))


; Common auxiliary

(define (restore-carefully in)
  (call-with-current-continuation
    (lambda (exit)
      (restore (lambda ()
		 (let ((c (read-char in)))
		   (if (eof-object? c)
		       (exit (cons 'eof c))
		       c)))))))
