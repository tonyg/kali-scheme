; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (ensure-loaded . structs)
  (force-output (current-output-port))		; avoid interleaved output
  (let ((out (current-noise-port)))
    (for-each (lambda (package)
		(display #\[ out)
		(display (package-name package) out)
		(with-interaction-environment package
		  (lambda ()
		    (invoke-closure
		      (make-closure (compile-package package)
				    (package-uid package)))))
		(set-package-loaded?! package #t)
		(walk-population check-structure
				 (package-clients package))
		(display #\] out)
		(newline out))
	      (collect-packages structs
				(lambda (package)
				  (not (package-loaded? package)))))))

