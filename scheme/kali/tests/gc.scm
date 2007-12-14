;; ,open kali threads address-spaces primitives
;; (spawn-server)


(define (gc-test machine-name port)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn start-server 'test-server)
	(sleep 2000)))
  (let ((local-aspace (local-address-space))
	(other-aspace (socket-id->address-space machine-name
						port)))
    (display "1...")(newline)
    (remote-apply other-aspace display "1...")
    (remote-apply other-aspace newline)
    (gc-test-vector other-aspace)
    (display "2...")(newline)
    (remote-apply other-aspace display "2...")
    (remote-apply other-aspace newline)
    (gc-test-proxy other-aspace)
    (display "3...")(newline)
    (remote-apply other-aspace display "3...")
    (remote-apply other-aspace newline)
    (remote-apply other-aspace gc-test-vector local-aspace)
    (display "4...")(newline)
    (remote-apply other-aspace display "4...")
    (remote-apply other-aspace newline)
    (remote-apply other-aspace gc-test-proxy local-aspace)
    'finished))

;; =======================================================================

(define (id x) x)

;; =======================================================================

(define (gc-test-vector as)
  (let lp ((i 10000))
    (if (zero? i)
	(begin
	  (gimme-stat as)

	  (display "collect") (newline)
	  (collect)

	  (remote-apply as display "collect")
	  (remote-apply as newline)
	  (remote-apply as collect)

	  (gimme-stat as))
	(begin
	  (let ((v (vector 1 2 3 4 5 6 7 8 9 10)))
	    (remote-apply as id v)
	    (lp (- i 1)))))))

(define (gc-test-proxy as)
  (let lp ((i 5000))
    (if (zero? i)
	(begin
	  (gimme-stat as)

	  (display "collect") (newline)
	  (collect)

	  (remote-apply as display "collect")
	  (remote-apply as newline)
	  (remote-apply as collect)

	  (gimme-stat as))
	(begin
	  (if (zero? (modulo i 500))
	      (gimme-stat as))
	  (let ((pv (make-proxy (vector 1 2 3 4 5 6 7 8 9 10))))
	    (remote-apply as 
			  (lambda (p)
			    (id (any-proxy-value p)))
			  pv)
	    (lp (- i 1)))))))

(define (gimme-stat oas)
  (display (address-space-proxy-vector (local-address-space)))
  (newline)
  (remote-apply oas
		(lambda ()
		  (display (address-space-proxy-vector (local-address-space)))
		  (newline))))