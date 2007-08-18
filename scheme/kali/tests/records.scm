;; ,open kali address-spaces threads define-record-types srfi-78
;; ,open kali address-spaces threads srfi-9

(define-record-type rec :rec
;(define-record-type :rec
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


(define (record-test machine-name port)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn start-server 'test-server)
	(sleep 2000)))
  (let ((local-aspace (local-address-space))
	(other-aspace (socket-id->address-space machine-name
						port)))
    (really-record-test local-aspace
		 other-aspace)
    
    (remote-run! other-aspace
		 (lambda ()
		   (really-record-test other-aspace
				       local-aspace)))))


;(define-syntax my-check
;  (syntax-rules (=>)
;    ((my-check exp => val)
;     (begin
;       (display 'exp)
;       (display " => ")
;       (let ((res exp))
;	 (display res)
;	 (if (equal? res val)
;	     (display " *correct*")
;	     (display " *failed*"))
;	 (newline))))))
 
(define (really-record-test local-space other-aspace)
  (let ((rec-0 (make-rec "string" 'symbol))
	(rec-1 (remote-apply other-aspace make-rec 'symbol "string")))

    ;; chnx todo(?): hmm don't know why but without it wont work
    ;; perhaps this is because of tcp-nodelay (???)
    ;(sleep 500)

    (check (rec? rec-0) 
	   => 
	   #t)
    (check (rec? rec-1) 
	   => 
	   #t)

    (check (remote-apply other-aspace rec? rec-0) 
	   => 
	   #t)
    (check (remote-apply other-aspace rec? rec-1) 
	   => 
	   #t)

    (check (remote-apply other-aspace rec-a rec-0) 
	   => 
	   "string")

    (check (remote-apply other-aspace rec-a rec-1) 
	   => 
	   'symbol)

    (check (begin
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

    (check (begin
	     (set-rec-b! rec-1 'symbol)
	     (and (rec-equal? rec-0 rec-1)
		  (remote-apply other-aspace rec-equal? rec-0 rec-1)))
	   => 
	   #t)))
