; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.

; The value of $NOTE-FILE-PACKAGE is called whenever a file is loaded into
; a package.  env/debug.scm uses this to associate packages with files so
; that code stuffed to the REPL will be eval'ed in the correct package.
;
; Is there any point in having this be a fluid?

(define $note-file-package
  (make-fluid (lambda (filename package)
		(values))))

(define (read-forms pathname package)
  (let* ((filename (namestring pathname #f *scheme-file-type*))
	 (truename (translate filename)))
    (call-with-input-file truename
      (lambda (port)
	((fluid $note-file-package) filename package)
	(let ((o-port (current-noise-port)))
	  (display truename o-port)
	  (force-output o-port)
	  (really-read-forms port))))))

(define (really-read-forms port)	    
  (let loop ((forms '()))
    (let ((form (read port)))
      (if (eof-object? form)
	  (reverse forms)
	  (loop (cons form forms))))))


