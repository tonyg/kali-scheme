; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Code to check the interaction between threads and sockets.

(define (run-server)
  (with-multitasking server))

(define (server)
  (call-with-values socket-server
    (lambda (port-number accept)
      (display "Port number is ") (write port-number) (newline)
      (let loop ()
	(call-with-values accept
	  (lambda (i-port o-port)
	    (spawn (service i-port o-port))
	    (loop)))))))

(define (service i-port o-port)
  (lambda ()
    (let loop ((total 0))
      (let ((next (read i-port)))
	(cond ((eof-object? next)
	       (close-input-port i-port)
	       (close-output-port o-port))
	      (else
	       (let ((total (+ total next)))
		 (write total o-port)
		 (newline o-port)
		 (loop total))))))))

(define (run-users machine port-number . data)
  (with-multitasking
   (lambda ()
     (do ((i 0 (+ i 1))
	  (d data (cdr d)))
	 ((null? d))
       (let ((l (car d)))
	 (spawn (lambda ()
		  (user (make-name i) (car l) (cadr l) machine port-number))))))))

(define (make-name i)
  (list->string (list (string-ref "ABCDEFGHIJKLMNOPQRSTUVWXYZ" i))))

(define (user id count delay machine port-number)
  (call-with-values
   (lambda ()
     (socket-client machine port-number))
   (lambda (i-port o-port)
     (let loop ((count count))
       (cond ((= 0 count)
	      (close-input-port i-port)
	      (close-output-port o-port))
	     (else
	      (write 1 o-port)
	      (newline o-port)
	      (for-each display (list id " got " (read i-port)))
	      (newline)
	      (sleep delay)
	      (loop (- count 1))))))))
