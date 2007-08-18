     
(define (fluid-test machine-name port)
  (let ((other-aspace (socket-id->address-space machine-name
						port)))
    (really-fluid-test)
    (remote-run! other-aspace really-fluid-test)))

(define (really-fluid-test)
  (let (($mumble (make-fluid 0)))
    (check (let ((a (fluid $mumble))
		 (b (let-fluid $mumble 1
			       (lambda () (fluid $mumble))))
		 (c (fluid $mumble))
		 (d (let-fluid $mumble 2
			       (lambda ()
				 (let-fluid $mumble 3
					    (lambda () (fluid $mumble)))))))
	     (list a b c d))
	   => '(0 1 0 3))
     
    (check (fluid $mumble)
	   => 0)			;0

    (let-fluid $mumble 1 
	       (lambda () 
		 (check (fluid $mumble)
			=> 1)))		;1

    (check (fluid $mumble)
	   => 0)			;0

    (let-fluid $mumble 1
	       (lambda ()
		 (check (fluid $mumble)
			=> 1)		;1
		 (let-fluid $mumble 2 
			    (lambda () 
			      (check (fluid $mumble)
				     => 2))))) ;2
    (check (fluid $mumble) 
	   => 0)			;0

    ((call-with-current-continuation
      (lambda (k)
	(lambda ()
	  (let-fluid $mumble 1
		     (lambda ()
		       (check (fluid $mumble)
			      => 1)	;1
		       (let-fluid $mumble 2
				  (lambda () 
				    (check (fluid $mumble)
					   => 2))) ;2
		       (k (lambda () 
			    (check (fluid $mumble)
				   => 0))))))))) ;0
    (check (fluid $mumble)
	   => 0)))			;0
