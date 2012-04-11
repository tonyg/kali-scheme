; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; ,open primitives


(define getenv
  (let ((buffer (make-string 2000)))
    (lambda (var-name)
      (let ((len (vm-extension 26 (cons var-name buffer))))
	(if len
	    (substring buffer 0 len)
	    #f)))))


