; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; The static linker.

; link-simple-system:
; resumer-exp should evaluate to a procedure
;   (lambda (arg i-port o-port ...) ...)

(define (link-simple-system filename resumer-exp . structs)
  (link-system structs (lambda () resumer-exp) filename))


; resumer-exp should evaluate to a procedure
;   (lambda (structs-thunk) ... (lambda (arg i-port o-port ...) ...))

(define (link-reified-system some filename make-resumer-exp . structs)
  (link-system (append structs (map cdr some))
	       (lambda ()
		 `(,make-resumer-exp
		   (lambda ()
		     ,(call-with-values
			  (lambda () (reify-structures some))
			(lambda (exp locs least-uid)
			  `(,exp (lambda (i)
				   (vector-ref ,(strange-quotation locs)
					       (- i ,least-uid)))))))))
	       filename))

(define strange-quotation
  (let ((operator/quote (get-operator 'quote)))
    (lambda (thing)
      (make-node operator/quote `',thing))))


; `(,make-resumer-exp ',vector) should evaluate to a procedure
;   (lambda (locs) ... (lambda (arg i-port o-port ...) ...))

(define (link-semireified-system some filename
				 make-resumer-exp . structs)
  (let ((loser #f))
    (link-system (append structs (map cdr some))
		 (lambda ()
		   (call-with-values (lambda ()
				       (reify-structures some))
		     (lambda (exp locs least)
		       (set! loser exp)
		       `(,make-resumer-exp ,(strange-quotation locs)
					   ,least))))
		 filename)
    (let ((f (namestring filename #f 'env)))
      (call-with-output-file f
	(lambda (port)
	  (display "Writing environment structure to ")
	  (display f)
	  (newline)
	  ;; loser evaluates to a procedure
	  ;;  (lambda (uid->location) struct-alist)
	  (write `(define make-the-structures
		    (,loser location-from-id))
		 port))))))


; (link-system structs make-resumer filename)
;    structs is a list of structures to be compiled,
;    make-resumer is a thunk which should return an expression, to be
;      evaluated in a package that opens the given structures, that
;      evaluates to the procedure to be called after all
;      initializations are run, and
;    filename is the name of the file to which the image should be written.

(define (link-system structs make-resumer filename)
  (with-fresh-compiler-state
   (if *debug-linker?* 100000 0)    ;Location uid
   (lambda ()
     (set! *loser* #f)
     (let* ((location-info (make-table))
	    (generator (make-location-generator location-info
						(if *debug-linker?* 10000 0)))
	    (thunks (compile-structures structs
					generator
					package->environment))
	    (p (make-simple-package structs #f #f))
	    (r (noting-undefined-variables p
					   (lambda ()
					     (set-package-get-location! p generator)
					     (compile-form (make-resumer) p)))))
       (let ((startup (make-closure 
		       (make-startup-procedure thunks r)
		       0)))
	 (if *debug-linker?* (set! *loser* startup))
	 (write-image-file startup
			   (namestring filename #f 'image)))
       (write-debug-info location-info
			 (namestring filename #f 'debug))))))
(define *loser* #f)
(define *debug-linker?* #f)

;

(define (compile-structures structs generator package->env)
  (let ((thunks '())
	(out (current-output-port)))
    (scan-structures
	   structs
	   (lambda (p)
	     (set-package-get-location! p generator)
	     #t)
	   (lambda (stuff p)  ;stuff = pair (file . (node1 node2 ...))
	     (for-each (lambda (file+forms)
			 (set! thunks
			       (cons (compile-scanned-forms (cdr file+forms)
							    p
							    (car file+forms)
							    out
							    (package->env p))
				     thunks)))
		       stuff)))
    (reverse thunks)))

; Locations in new image will have their own sequence of unique id's.

(define (make-location-generator location-info start)

  (let ((*location-uid* start))

    (define (make-new-location p name)
      (let ((uid *location-uid*))
	(set! *location-uid* (+ *location-uid* 1))
	(table-set! location-info uid
		    (cons (name->symbol name) (package-uid p))) ;?
	(make-undefined-location uid)))

    make-new-location))


(define (write-image-file start filename)
  (write-image filename
	       start
	       "This heap image was made by the Scheme 48 linker."))



; Handy utility for making arguments to link-reified-system

(define-syntax struct-list
  (syntax-rules ()
    ((struct-list name ...) (list (cons 'name name) ...))))
