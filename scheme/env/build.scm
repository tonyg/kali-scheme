; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Commands for writing images.

; A heap image written using ,dump or ,build can be invoked with
;    s48 -i <filename> [-h <heap size>] [-a <argument>]
; For images made with ,build <exp> <filename>, <argument> is passed as
; a string to the procedure that is the result of <exp>.


; dump <filename>

(define-command-syntax 'dump "<filename>"
  "write the current heap to an image file"
  '(filename &opt form))

(define (dump filename . maybe-info)
  (let ((info (if (null? maybe-info) "(suspended image)" (car maybe-info)))
	(context (user-context))
	(env (environment-for-commands)))
    (build-image #f
		 (lambda (arg)
		   (with-interaction-environment env
		     (lambda ()
		       (restart-command-processor arg
						  context
						  (lambda ()
						    (greet-user info))
						  values))))
		 filename)))

; build <exp> <filename>

(define-command-syntax 'build "<exp> <filename> <option> ..."
  "build a heap image file with <exp> as entry procedure, <option> can be no-warnings"
  '(expression filename &rest name))

(define (build exp filename . options)
  (build-image (not (memq 'no-warnings options))
	       (eval exp (environment-for-commands))
	       filename))

(define (build-image no-warnings? start filename)
  (let ((filename (translate filename)))
    (write-line (string-append "Writing " filename) (command-output))
    (write-image (thing->file-name-byte-string filename)
		 (stand-alone-resumer no-warnings? start)
		 (string->byte-string ""))
    #t))

(define (stand-alone-resumer warnings? start)
  (make-usual-resumer  ;sets up exceptions, interrupts, and current input & output
   warnings?
   signal-condition
   (lambda (arg)
     (call-with-current-continuation
       (lambda (halt)
	 (with-handler (simple-condition-handler halt (current-error-port))
	   (lambda ()
	     (start arg))))))))

; Simple condition handler for stand-alone programs.

(define (simple-condition-handler halt port)
  (lambda (c punt)
    (let ((c (coerce-to-condition c)))
      (cond ((error? c)
	     (display-condition c port)
	     (halt 1))
	    ((warning? c)
	     (display-condition c port)) ;Proceed
	    ((interrupt? c)
	     ;; (and ... (= (cadr c) interrupt/keyboard)) ?
	     (halt 2))
	    ((bug? c)
	     (display-condition c port)
	     (halt 3))
	    (else
	     (punt))))))

;(define interrupt/keyboard (enum interrupt keyboard))
