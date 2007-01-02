; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.



(define-syntax require
  (syntax-rules (quote)
    ((require '(name1 name2 ...))
     (*require '(name1 name2 ...)))))

(define (*require interface-id)
  (let ((start-thunk
	 (case (car interface-id)
	   ((scheme-48)
	    (let ((p (config-package)))
	      (lambda () p)))
	   ((scheme-library-1)
	    (let* ((p (config-package))
		   (thunk
		    (lambda ()
		      (environment-ref p 'scheme-library-1))))
	      (ensure-loaded (thunk))
	      (thunk)))
	   (else
	    (error "unrecognized interface identifier" interface-id)))))
    (package-open! (interaction-environment)
		   (let loop ((names (cdr interface-id))
			      (thunk start-thunk))
		     (if (null? names)
			 thunk
			 (let ((new-thunk 
				(lambda ()
				  (let ((source (thunk)))
				    (if (package? source)
					(environment-ref source
							 (car names))
					(*structure-ref source
							(car names)))))))
			   (ensure-loaded (new-thunk))
			   (loop (cdr names)
				 new-thunk)))))))
