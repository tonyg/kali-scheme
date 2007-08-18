
(define (spawn-supervised thunk . args)
  (let ((max-restarts (if (> (length args) 0)
			  (car args)
			  5))
	(report? (if (> (length args) 1)
		     (cadr args)
		     #t)))
    (spawn (lambda ()
	     (let loop ((max-restarts max-restarts))
	       ((call-with-current-continuation
		 (lambda (k)
		   (with-handler 
		    (lambda (c prop)
		      (k (lambda ()
			   (report "process failed")
			   (if (zero? max-restarts)
			       (begin
				 (report "not going to start again - too many errors!")
				 (report "terminate supervisor")
				 (terminate-current-thread))
			       (begin
				 (report "restarting...")
				 (loop (- max-restarts 1)))))))
		    (lambda ()
		      (report  "starting up supervised process")
		      (thunk)))))))))))

(define (report str)
  (display (string-append "INFO: " str))
  (newline))

		  