; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Glue to connect the threads package with command processor.

(define (threads)
  (with-threads breakpoint))

(define (with-threads thunk)
  (with-handler (simple-thread-condition-handler)
    (lambda ()
      (with-multitasking
       (lambda ()
	 (with-keyboard-interrupt-thread (current-thread)
	   thunk))))))

(define-command-syntax 'start-threads "" "initiate multitasking"
  '())

(define (start-threads)
  (let ((context (user-context))
	(env (environment-for-commands)))
    (exit-command-processor
     (lambda ()
       (with-threads
	(lambda ()
	  (start-command-processor
	   #f context
	   env
	   (lambda ()
	     (write-line "Multitasking started"
			 (command-output))))))))))



; For using threads in a system that has a command processor.
; Interrupts will be disabled, I think, when the designated thread gets
; its signal.

(define (with-keyboard-interrupt-thread thread thunk)
  (let ((save #f))
    (dynamic-wind
       (lambda ()
	 (set! save (vector-ref interrupt-handlers interrupt/keyboard))
	 (vector-set! interrupt-handlers
		      interrupt/keyboard
		      (lambda (ei)
			(interrupt-thread thread
			  (lambda ()
			    (signal 'interrupt interrupt/keyboard ei))))))
       thunk
       (lambda ()
	 (vector-set! interrupt-handlers interrupt/keyboard save)))))

(define interrupt/keyboard (enum interrupt keyboard))


; A simple handler for non-command-processor threads.

(define (simple-thread-condition-handler)
  (let ((port (current-output-port)))
    (lambda (c punt)
      (cond ((or (error? c) (interrupt? c))
	     (random-thread-error c port))
            (else (punt))))))

(define (random-thread-error c port)
  (display "*** " port)
  (write (current-thread) port)
  (display " got an error:" port) ;(newline port)
  (display-condition c port)
  (terminate-current-thread))

; Can we do better?...

;(define (cp-start-multitasking)
;  (let ((mbx (make-mailbox)))
;    (lambda ()
;      (with-multitasking
;          (errant-thread-condition-handler mbx (current-output-port))
;        (lambda ()
;          ;; (add-sentinel! (errant-thread-sentinel mbx))
;          (with-keyboard-interrupt-thread
;              (current-thread)
;            breakpoint))))))            ;???
;
;(define (errant-thread-condition-handler mbx port)
;  (lambda (c punt)
;    (cond ((error? c)
;           (random-thread-error c mbx))
;          ((warning? c)                 ;Proceed
;           (display-condition c port)
;           (newline port)
;           (unspecific))
;          (else                         ;Proceed
;           (punt)))))
;
;(define (random-thread-error c mbx)
;  (let ((cv (make-condvar)))
;    (mailbox-write mbx (list c cv (current-thread)))
;    ((condvar-ref cv))))


; To do: make the command processor check the errant-thread mailbox.

;(define (errant-thread-sentinel mbx)
;  (lambda ()
;    (if (not (mailbox-empty? mbx))
;        (begin (display .... ? ...) (newline)))))
;
;(add-sentinel! errant-thread-sentinel)

