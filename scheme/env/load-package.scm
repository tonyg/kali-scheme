; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


(define (ensure-loaded . structs)
  (scan-structures
       structs
       (lambda (p)
	 (not (package-loaded? p)))
       (lambda (stuff p)  ;stuff = pair (file . (node1 node2 ...))
	 (noting-undefined-variables p
	   (lambda ()
	     (with-interaction-environment p
	       (lambda ()
		 (for-each (lambda (filename+scanned-forms)
			     (eval-scanned-forms (cdr filename+scanned-forms)
						 p
						 (car filename+scanned-forms)))
			   stuff)
		 (set-package-loaded?! p #t))))))))
