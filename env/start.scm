; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define (start structs-to-open)
  (lambda (structs-thunk)
    (usual-resumer
     (lambda (arg)
       (with-interaction-environment
	   ;; This condition handler isn't really necesssary, but it sure
	   ;; makes it easier to debug problems in the package reification
	   ;; code.  Without it, any errors will just show up as an exit
	   ;; with status code 123 -- not very helpful.
	   (call-with-current-continuation
	     (lambda (k)
	       (with-handler
		   (lambda (c punt)
		     (if (error? c)
			 (primitive-catch
			  (lambda (cont)
			    (begin (write c) (newline)
				   (write (continuation-preview cont))
				   (newline)
				   (k #f))))
			 (punt)))
		 (lambda ()
		   (let ((b (make-built-in-structures (structs-thunk))))
		     (initialize-interaction-environment! b)
		     (make-initial-package b structs-to-open))))))
	 (lambda ()
	   (command-processor arg)))))))

; The structs argument is an a-list of (name . structure), as computed
; by the expression returned by reify-structures.

(define (make-built-in-structures structs)
  (let* ((p (make-simple-package '() #f #f 'built-in-structures))
	 (s (make-structure 'built-in-structures
		  (make-simple-signature
		     (cons 'built-in-structures (map car structs)))
		  p)))
    (for-each (lambda (name+struct)
		(environment-define! p
				     (car name+struct)
				     (cdr name+struct)))
	      structs)
    (environment-define! p 'built-in-structures s)
    s))

(define (initialize-interaction-environment! built-in-structures)
  (let ((scheme (*structure-ref built-in-structures 'scheme)))
    (set-interaction-environment!
     (make-simple-package (list scheme)
			  eval
			  (make-env-for-syntax-promise built-in-structures)
			  'interaction))

    (set-scheme-report-environment!
     5
     (make-simple-package (list scheme)
			  eval
			  (make-env-for-syntax-promise built-in-structures)
			  'r5rs))))

; Intended for bootstrapping the command processor.

(define (make-initial-package built-in-structures structs-to-open)
  (let ((p (make-simple-package
		    (cons built-in-structures
			  (map (lambda (name)
				 (*structure-ref built-in-structures name))
			       structs-to-open))
		    eval
		    (make-env-for-syntax-promise built-in-structures)
		    'initial)))
    (environment-define! p 'built-in-structures built-in-structures)
    p))

(define (make-env-for-syntax-promise built-in-structures)
  (let ((scheme (*structure-ref built-in-structures 'scheme)))
    (delay (make-package-for-syntax eval (list scheme)))))
