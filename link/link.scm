; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; The static linker.

; link-simple-system:
; resumer-exp should evaluate to a procedure
;   (lambda (arg i-port o-port ...) ...)

(define (link-simple-system filename resumer-exp . structs)
  (link-system structs (lambda () resumer-exp) filename))


; resumer-exp should evaluate to a procedure
;   (lambda (structs) ... (lambda (arg i-port o-port ...) ...))

(define (link-reified-system some filename make-resumer-exp . structs)
  (link-system (append structs (map cdr some))
	       (lambda ()
		 `(,make-resumer-exp
		   (lambda ()
		     ,(reify-structures some
					(lambda (loc) loc)))))
	       filename))


; resumer-exp should evaluate to a procedure
;   (lambda (locs) ... (lambda (arg i-port o-port ...) ...))

(define (link-semireified-system some filename
				 make-resumer-exp . structs)
  (let ((locs (make-table))
	(loser #f))
    (link-system (append structs (map cdr some))
		 (lambda ()
		   (set! loser
			 (reify-structures some
				  (lambda (loc)
				    (let ((id (location-id loc)))
				      (table-set! locs id loc)
				      id))))
		   `(,make-resumer-exp
		     ',(let ((l '()))
			 (table-walk (lambda (id loc)
				       (set! l (cons loc l)))
				     locs)
			 (list->vector l))))
		 filename)
    (let ((f (namestring filename #f 'env)))
      (call-with-output-file f
	(lambda (port)
	  (display "Writing environment structure to ")
	  (display f)
	  (newline)
	  (write `(define the-structures ,loser) port))))))


; (link-system structs make-resumer filename)
;    structs is a list of structures to be compiled,
;    make-resumer is a thunk which should return an expression, to be
;      evaluated in a package that opens the given structures, that
;      evaluates to the procedure to be called after all
;      initializations are run, and
;    filename is the name of the file to which the image should be written.

(define (link-system structs make-resumer filename)
  (with-fresh-compiler-state
   (lambda ()
     (let* ((thunks (compile-structures structs))
	    (p (make-simple-package structs eval #f))
	    (r
	     (noting-undefined-variables p
	       (lambda ()
		 (compile-form (make-resumer) p)))))
       (check-package p)
       (write-image-file (make-startup-procedure thunks r)
			 (namestring filename #f 'image))
       (write-debug-info (namestring filename #f 'debug))))))

;

(define (compile-structures structs)
  (let ((thunks '()))
    (scan-structures
	   structs
	   (lambda (p) #t)
	   (lambda (stuff p) ;stuff = pair (file . (form1 form2 ...))
	     (set-package-integrate?! p #t)
	     (noting-undefined-variables p
	       (lambda ()
		 (for-each (lambda (file+forms)
			     (set! thunks
				   (cons (compile-scanned-forms
					  (cdr file+forms)
					  p
					  (car file+forms)
					  (current-output-port))
					 thunks)))
			   stuff)))))
    (reverse thunks)))


(define (write-image-file start filename)
  (write-image filename
	       start
	       "This is a heap image file made by the Scheme48 linker."))



(define (check-package p)
  (let ((names (undefined-variables p)))
    (if (not (null? names))
	(begin (display "Undefined: ") 
	       (write names) (newline)))))


; Handy utility for making arguments to link-reified-system

(define-syntax struct-list
  (syntax-rules ()
    ((struct-list name ...) (list (cons 'name name) ...))))

; Mumble.

(define (load-configuration filename)
  (let-fluid $source-file-name filename
    (lambda () (load filename))))
