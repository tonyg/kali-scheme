; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.



; System entry and exit

; Entry point from OS executive.  Procedures returned by USUAL-RESUMER
; are suitable for use as the second argument to WRITE-IMAGE.

(define (usual-resumer entry-point)
  (lambda (resume-arg in out)
    (initialize-dynamic-state!)
    (with-initial-ports in out
      (lambda ()
	(initialize-exceptions!
	 (lambda ()
	   (initialize-interrupts!)
	   (entry-point (vector->list resume-arg))))))))
