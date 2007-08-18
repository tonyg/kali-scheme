;; ,open kali address-spaces threads srfi-42 srfi-78

;; this test needs another server running


;; ---------------------------------------------------------------

(define (id x) x)

(define (twice x) 
  (* 2 x))

;; ---------------------------------------------------------------

(define (remote-test machine-name port)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn start-server 'test-server)
	(sleep 2000)))
  (let ((other-aspace (socket-id->address-space machine-name
						port)))
    (really-remote-test (local-address-space)
			other-aspace
			#t)))

(define (really-remote-test local-aspace other-aspace run-test-on-other-aspace)
  (check (remote-run! other-aspace display "i'm taking part in a test")
	 => #t)

  (check (remote-run! other-aspace newline)
	 => #t)

  (check (remote-run! other-aspace + 1 2 3)
	 => #t)

  (check (remote-run! other-aspace - 1 2)
	 => #t)

  (check-ec (:range i 0 200)
	    ((lambda (x)
	       (remote-run! other-aspace id x))
	     i)
	    => #t)

  (check (remote-apply other-aspace + 1 2 3)
	 => 6)

  (check (remote-apply other-aspace - 1 2)
	 => -1)

  (check-ec (:range i 0 200)
	    ((lambda (x)
	       (remote-apply other-aspace id x))
	     i)
	    => i)

  (if run-test-on-other-aspace
      (remote-run! other-aspace
		   really-remote-test
		   other-aspace
		   local-aspace
		   #f)))
  

