; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define (start structs-to-open)
  (lambda (structs-thunk)
    (usual-resumer
     (lambda (arg)
       (with-interaction-environment
	   (let ((b (make-built-in-structures (structs-thunk))))
	     (initialize-interaction-environment! b)
	     (make-initial-package b structs-to-open))
	 (lambda ()
	   (command-processor arg)))))))

; The structs argument is an a-list of (name . structure), as computed
; by the expression returned by reify-structures.

(define (make-built-in-structures structs)
  (let* ((p (make-simple-package '() #f #f 'built-in-structures))
	 (s (make-structure p
		  (lambda ()
		    (make-simple-interface
		       #f			;name
		       (cons 'built-in-structures (map car structs))))
		  'built-in-structures)))
    (for-each (lambda (name+struct)
		(environment-define! p
				     (car name+struct)
				     (cdr name+struct)))
	      structs)
    (environment-define! p 'built-in-structures s)
    s))

(define scheme-for-syntax 'scheme)

(define (initialize-interaction-environment! built-in-structures)
  (let ((scheme (*structure-ref built-in-structures 'scheme))
	(for-syntax (*structure-ref built-in-structures scheme-for-syntax)))
    (set-interaction-environment!
     (make-simple-package (list scheme)
			  eval
			  (make-env-for-syntax-promise for-syntax)
			  'interaction))

    (set-scheme-report-environment!
     5
     (make-simple-package (list scheme)
			  eval
			  (make-env-for-syntax-promise for-syntax)
			  'r5rs))))

; Intended for bootstrapping the command processor.

(define (make-initial-package built-in-structures structs-to-open)
  (let ((p (make-simple-package
	    (cons built-in-structures
		  (map (lambda (name)
			 (*structure-ref built-in-structures name))
		       structs-to-open))
	    eval
	    (make-env-for-syntax-promise
	     (*structure-ref built-in-structures scheme-for-syntax))
	    'initial)))
    (environment-define! p 'built-in-structures built-in-structures)
    p))

(define (make-env-for-syntax-promise scheme)
  (delay (make-package-for-syntax eval (list scheme))))
