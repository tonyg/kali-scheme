; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


(define (start structs-thunk)
  (usual-resumer
   (lambda (arg)
     ;; This condition handler isn't really necesssary, but it sure
     ;; makes it easier to debug problems in the package reification
     ;; code.  Without it, any errors will just show up as an exit
     ;; with status code 123 -- not very helpful.
     (call-with-current-continuation
	   (lambda (k)
	     (with-handler (lambda (c punt)
			     (if (error? c)
				 (begin (write c) (newline)
					(k #f))
				 (punt)))
	       (lambda ()
		 (initialize-interaction-environment!
		    (cdr (assq 'scheme (structs-thunk))))))))
     (command-processor arg))))


(define (initialize-interaction-environment! scheme)
  (let ((eval-for-syntax
	  (lambda (form) (eval form (scheme-report-environment 5)))))
    (set-interaction-environment!
         (make-simple-package (list scheme) #f #f 'interaction))
    (set-scheme-report-environment!
         5
	 (make-simple-package (list scheme) #f #f 'r5rs))
    ))
