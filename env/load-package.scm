; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define (ensure-loaded . structs)
  (scan-structures
       structs
       (lambda (p)
	 (not (package-loaded? p)))
       (lambda (stuff p) ;stuff = list of (file . ((loc . form) ...))
	 (with-interaction-environment p
	   (lambda ()
	     (noting-undefined-variables p
	       (lambda ()
		 (for-each (lambda (filename+scanned-forms)
			     (eval-from-file
				(cdr filename+scanned-forms)
				p
				(car filename+scanned-forms)))
			   stuff)
		 (set-package-loaded?! p #t))))))))
