; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Scheme analogues of Posix popen() and pclose() library calls.

(define (call-with-mumble-pipe input?)
  (lambda (command proc)
    (call-with-values pipe
      (lambda (pipe-for-read pipe-for-write)
	(let ((winner (if input? pipe-for-read pipe-for-write))
	      (loser  (if input? pipe-for-write pipe-for-read))
	      (pid (fork)))
	  (if (= pid 0)
	      (dynamic-wind
	        (lambda () #f)
		(lambda ()
		  (close winner)
		  (let ((foo (if input? 1 0)))
		    (close foo)
		    (if (not (= (dup loser) foo))
			(error "dup lost" loser foo)))
		  (execv "/bin/sh"
			 (vector "sh" "-c" command)))
		(lambda () (exit 1))))
	  ;; (write `(pid = ,pid)) (newline)
	  (close loser)
	  (let* ((channel (open-channel winner
					(if input?
					    (enum open-channel-option
						  raw-input-channel)
					    (enum open-channel-option
						  raw-output-channel))))
		 (port (if input?
			   (input-channel->port channel 1024)
			   (output-channel->port channel 1024))))
	    (call-with-values (lambda () (proc port))
	      (lambda vals
		(if input?
		    (close-input-port port)
		    (close-output-port port))
		;; (display "Waiting.") (newline)
		(call-with-values (lambda () (waitpid pid 0))
		  (lambda (pid status)
		    ;; (write `(status = ,status)) (newline)
		    (apply values vals)))))))))))

(define call-with-input-pipe
  (call-with-mumble-pipe #t))

(define call-with-output-pipe
  (call-with-mumble-pipe #f))

