; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; System entry and exit

; Entry point from OS executive.  Procedures returned by USUAL-RESUMER
; are suitable for use as the second argument to WRITE-IMAGE.
;
; The placement of INITIALIZE-RECORDS! is questionable.  Important parts
; of the system are not in place when it is run.

(define (make-usual-resumer warn-about-undefined-imported-bindings? signal-condition-proc
			    entry-point)
  ;; The argument list needs to be in sync with
  ;; S48-CALL-STARTUP-PROCEDURE in vm/interp/resume.scm, and
  ;; MAKE-STARTUP-PROCEDURE in bcomp/comp.scm.
  (lambda (resume-arg
	   in in-encoding out out-encoding error error-encoding
	   records)
    (initialize-rts signal-condition-proc
		    in in-encoding out out-encoding error error-encoding
		    (lambda ()
		      (run-initialization-thunks)
		      (initialize-records! records)
		      (if warn-about-undefined-imported-bindings?
			  (warn-about-undefined-imported-bindings))
		      (entry-point (vector->list resume-arg))))))

(define (usual-resumer entry-point)
  (make-usual-resumer #t really-signal-condition entry-point))

(define (warn-about-undefined-imported-bindings)
  (let ((undefined-bindings (find-undefined-imported-bindings)))
    (do ((size (vector-length undefined-bindings))
	 (i 0 (+ 1 i)))
	((= i size))
      (debug-message "undefined imported binding "
		     (shared-binding-name (vector-ref undefined-bindings i))))))

(define (initialize-rts signal-condition-proc
			in in-encoding out out-encoding error error-encoding
			thunk)
  (initialize-session-data!)
  (initialize-dynamic-state!)
  (initialize-vm-exceptions! signal-condition-proc)
  (initialize-exceptions!
   (lambda ()
     (initialize-interrupts!
      spawn-on-root
      (lambda ()
	(let ((in-port (input-channel->port in))
	      (out-port (output-channel->port out))
	      (error-port (output-channel->port error 0))) ; zero-length buffer

	  (set-encoding! in-port in-encoding)
	  (set-encoding! out-port out-encoding)
	  (set-encoding! error-port error-encoding)

	  (initialize-i/o 
	   in-port out-port error-port
	   (lambda ()
	     (with-threads
	      (lambda ()
		(root-scheduler thunk
				200	; thread quantum, in msec
				300))))))))))) ; port-flushing quantum

; Leave the default if we can't find a suitable codec
(define (set-encoding! port encoding)
  (cond
   ((find-text-codec encoding) => 
    (lambda (codec)
      (set-port-text-codec! port codec)))))

; This is primarily for LOAD-DYNAMIC-EXTERNALS; we don't want to
; refer to it directly here, because that would increase the size of
; the image by 100k.

; Use this with care: no efforts are being made to remove duplicates.

(define *initialization-thunks* '())

(define (add-initialization-thunk! thunk)
  (set! *initialization-thunks*
	(cons thunk *initialization-thunks*)))

(define (run-initialization-thunks)
  (for-each (lambda (thunk) (thunk))
	    *initialization-thunks*))

; Add the full/empty buffer handlers.

(initialize-i/o-handlers! define-vm-exception-handler signal-vm-exception)
