; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; System entry and exit

; Entry point from OS executive.  Procedures returned by USUAL-RESUMER
; are suitable for use as the second argument to WRITE-IMAGE.

(define (usual-resumer entry-point)
  (lambda (resume-arg in out)
    (initialize-dynamic-state!)
    (initialize-exceptions!
     (lambda ()
       (initialize-interrupts!)
       (with-initial-ports in out
	 (lambda ()
	   (entry-point resume-arg)))))))
