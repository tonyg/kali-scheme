; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (start structs-to-open)
  (lambda (structs-thunk)
    (usual-resumer
     (lambda (arg)
       (let* ((structs (structs-thunk))
	      (b (make-built-in-structures structs)))
	 (initialize-interaction-environment! b)
	 (with-interaction-environment
	     (make-initial-package b structs-to-open)
	   (lambda ()
	     (command-processor (cond ((assq 'usual-commands structs)
				       => (lambda (z)
					    (structure-package (cdr z))))
				      (else #f))
				arg))))))))

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

(define (initialize-interaction-environment! built-in-structures)
  (let ((scheme (*structure-ref built-in-structures 'scheme))
	(tower (make-tower built-in-structures 'interaction)))
    (set-interaction-environment!
      (make-simple-package (list scheme) #t tower 'interaction))

    (set-scheme-report-environment!
      5
      (make-simple-package (list scheme) #t tower 'r5rs))))

; Intended for bootstrapping the command processor.

(define (make-initial-package built-in-structures structs-to-open)
  (let ((p (make-simple-package
	    (cons built-in-structures
		  (map (lambda (name)
			 (*structure-ref built-in-structures name))
		       structs-to-open))
	    eval
	    (make-tower built-in-structures 'initial)
	    'initial)))
    (environment-define! p 'built-in-structures built-in-structures)
    p))

(define (make-tower built-in-structures id)
  (make-reflective-tower eval
			 (list (*structure-ref built-in-structures
					       'scheme))
			 id))
