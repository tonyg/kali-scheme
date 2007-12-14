;; ,open kali address-spaces threads srfi-9 srfi-42 srfi-78
;; ,open kali address-spaces threads define-record-types srfi-42 srfi-78

;; this test needs another server running


;; ---------------------------------------------------------------

(define (id x) x)

(define (twice x) 
  (* 2 x))

;; ---------------------------------------------------------------

(define (make-adder a)
  (lambda (b)
    (+ a b)))

(define add3 (make-adder 3))
(define add19 (make-adder 19))

;; ---------------------------------------------------------------

(define (remote-test machine-name port)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn-server)))
  (let ((local-aspace (local-address-space))
	(other-aspace (socket-id->address-space machine-name
						port)))
    (really-remote-test local-aspace
			other-aspace)
    (proxy-test local-aspace
		other-aspace)
    
    (remote-run! other-aspace
		 (lambda ()
		   (really-remote-test other-aspace
				       local-aspace)
		   (proxy-test other-aspace
			       local-aspace)))))

(define (really-remote-test local-aspace other-aspace)
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
	    => i))
  

(define (proxy-test local-aspace other-aspace)
  (let ((proxy-0 (make-proxy 'proxy-0-local))
	(proxy-1 (make-proxy 'proxy-1-local))
	(proxy-2 (remote-apply other-aspace make-proxy 'proxy-2-local-alien)))

    (check (proxy? proxy-0) => #t)
    (check (proxy? proxy-1) => #t)
    (check (proxy? 1) => #f)

    (check (proxy-owner proxy-0) => local-aspace)
    (check (proxy-owner proxy-2) => other-aspace)

    (check (proxy-local-ref proxy-0) => 'proxy-0-local)
    (check (proxy-local-ref proxy-1) => 'proxy-1-local)

    (check (any-proxy-value proxy-0) => 'proxy-0-local)
    (check (any-proxy-value proxy-1) => 'proxy-1-local)

    (check (remote-apply other-aspace any-proxy-value proxy-0) => 'proxy-0-local)
    (check (remote-apply other-aspace any-proxy-value proxy-1) => 'proxy-1-local)
    
    (check (begin
	     (remote-apply other-aspace proxy-local-set! proxy-0 'proxy-0-alien)
	     (remote-apply other-aspace proxy-local-ref  proxy-0))
	   => 'proxy-0-alien)
    
    
    (check (begin
	     (remote-apply other-aspace proxy-remote-set! proxy-0 'proxy-0-local-changed)
	     (proxy-local-ref  proxy-0))
	   => 'proxy-0-local-changed)

    (check (remote-apply other-aspace proxy-remote-ref proxy-0) => 'proxy-0-local-changed)

    (check (begin
	     (remote-apply other-aspace proxy-remote-set! proxy-1 'proxy-1-local-changed)
	     (remote-apply other-aspace any-proxy-value  proxy-1))
	   => 'proxy-1-local-changed)
    
    (check (remote-apply other-aspace add3 5) 
	   => 8)

    (check (remote-apply other-aspace add19 23)
	   => 42)))

