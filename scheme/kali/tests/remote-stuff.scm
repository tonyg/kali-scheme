;; ,open kali address-spaces threads srfi-9 srfi-42 srfi-78
;; ,open kali address-spaces threads define-record-types srfi-42 srfi-78

;; this test needs another server running


;; ---------------------------------------------------------------

(define (id x) x)

(define (twice x) 
  (* 2 x))

;(define-record-type :rec
(define-record-type rec :rec
  (make-rec a b)
  rec?
  (a rec-a set-rec-a!)
  (b rec-b set-rec-b!))

(define (rec-equal? r1 r2)
  (and (rec? r1)
       (rec? r2)
       (let ((a1 (rec-a r1))
	     (a2 (rec-a r2))
	     (b1 (rec-b r1))
	     (b2 (rec-b r2)))
	 ((if (rec? a1)
	      rec-equal?
	      equal?) 
	  a1 a2)
	 ((if (rec? b1)
	      rec-equal?
	      equal?) 
	  b1 b2))))

;; ---------------------------------------------------------------

(define (test machine-name port)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn start-server 'test-server)
	(sleep 2000)))
  (let ((local-aspace (local-address-space))
	(other-aspace (socket-id->address-space machine-name
						port)))
    (remote-test local-aspace
		 other-aspace)
    (proxy-test local-aspace
		other-aspace)
;    (record-test local-aspace
;		 other-aspace)
    
    (remote-run! other-aspace
		 (lambda ()
		   (remote-test other-aspace
				local-aspace)
		   (proxy-test other-aspace
			       local-aspace)))))
;		   (record-test other-aspace
;				local-aspace)))))

;; see records.scm for record-testing

(define (remote-test local-aspace other-aspace)
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
	   => 'proxy-1-local-changed)))

;; *GROWL* 
;; the record test isn't working with the srfi-78-check. (?)
;; so we're using a more primitive version my-check for record tests.

(define-syntax my-check
  (syntax-rules (=>)
    ((my-check exp => val)
     (begin
       (display 'exp)
       (display " => ")
       (let ((res exp))
	 (display res)
	 (if (equal? res val)
	     (display " *correct*")
	     (display " *failed*"))
	 (newline))))))

(define (record-test local-space other-aspace)
  (let ((rec-0 (make-rec "string" 'symbol))
	(rec-1 (remote-apply other-aspace make-rec 'symbol "string")))

    ;; chnx todo(?): hmm don't know why but without it wont work
    ;; perhaps this is because of tcp-nodelay (???)
    (sleep 500)

    (my-check (rec? rec-0) 
	      => 
	      #t)
    (my-check (rec? rec-1) 
	      => 
	      #t)

    (my-check (remote-apply other-aspace rec? rec-0) 
	      => 
	      #t)
    (my-check (remote-apply other-aspace rec? rec-1) 
	      => 
	      #t)

    (my-check (remote-apply other-aspace rec-a rec-0) 
	      => 
	      "string")

    (my-check (remote-apply other-aspace rec-a rec-1) 
	      => 
	      'symbol)

    (my-check (begin
		(remote-apply other-aspace set-rec-a! rec-0 'symbol)
		(remote-apply other-aspace 
			      rec-equal? 
			      rec-0
			      (remote-apply other-aspace 
					    make-rec 
					    'symbol
					    'symbol)))
	      => 
	      #t)

    (my-check (begin
		(set-rec-b! rec-1 'symbol)
		(and (rec-equal? rec-0 rec-1)
		     (remote-apply other-aspace rec-equal? rec-0 rec-1)))
	      => 
	      #t)))
