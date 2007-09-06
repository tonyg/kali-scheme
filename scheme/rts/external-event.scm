; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;----------------
; External events

(define (initialize-external-events!)
  (set-interrupt-handler! (enum interrupt external-event)
			  external-event-handler))

; Exported procedure

(define (waiting-for-external-events?)
  (zap-condvar-orphans!)
  (not (null? (external-event-condvars))))

;----------------

; A session slot contains an alist mapping external-event uids to
; condvars for external events on that uid.  This works analogously to
; channels.

(define external-events-wait-condvars-slot
  (make-session-data-slot! '()))

(define (external-event-condvars)
  (session-data-ref external-events-wait-condvars-slot))

(define (set-external-event-condvars! condvars)
  (session-data-set! external-events-wait-condvars-slot condvars))
  
; Adding a condvar and uid - the caller has already determined there
; is no existing condvar for this uid.

(define (add-external-event-condvar! uid condvar)
  (set-external-event-condvars! (cons (cons uid condvar)
				      (external-event-condvars))))

(define (notify-external-event-condvar! condvar)
  (with-new-proposal (lose)
    (or (maybe-commit-and-set-condvar! condvar #t)
	(lose))))

(define (external-event-handler uid enabled-interrupts)
  (cond
   ((fetch-external-event-condvar! uid)
    => notify-external-event-condvar!)))

; PROC takes a procedure RETURN and a thunk WAIT performs the wait
; itself.  The sequence is supposed to go as follows:

; - PROC sets up whatever will cause the external event
; - it then either calls RETURN on the return values, meaning that the
;   result is already available and no event will come
; - ... or it calls WAIT which waits for the event and then returns
;   the results.

(define (wait-for-external-event uid proc)
  (let ((ints #f))
    (dynamic-wind
	(lambda ()
	  (set! ints (disable-interrupts!)))
	(lambda ()
	  (let ((condvar (make-condvar)))
	    (add-external-event-condvar! uid condvar)
	    (proc
	     (lambda results
	       (fetch-external-event-condvar! uid) ; ditch
	       (apply values results))
	     (lambda () ; actually wait
	       (with-new-proposal (lose)
		 (maybe-commit-and-wait-for-condvar condvar))))))
	(lambda ()
	  (set-enabled-interrupts! ints)))))
     
; This just deletes from the alist.

(define (fetch-external-event-condvar! uid)
  (let ((condvars (external-event-condvars)))
    (cond ((null? condvars)
	   #f)
	  ((= uid (caar condvars))
	   (set-external-event-condvars! (cdr condvars))
	   (cdar condvars))
	  (else
	   (let loop ((condvars (cdr condvars)) (prev condvars))
	     (cond ((null? condvars)
		    #f)
		   ((= uid (caar condvars))
		    (set-cdr! prev (cdr condvars))
		    (cdar condvars))
		   (else
		    (loop (cdr condvars) condvars))))))))

; Zap the condvars that no longer have waiters.

(define (zap-condvar-orphans!)
  (with-interrupts-inhibited
   (lambda ()
     (let loop ((condvars (external-event-condvars)) (okay '()))
       (if (null? condvars)
	   (set-external-event-condvars! okay)
	   (let ((condvar (cdar condvars)))
	     (loop (cdr condvars)
		   (if (condvar-has-waiters? condvar)
		       (cons (car condvars) okay)
		       (begin
			 (notify-external-event-condvar! condvar)
			 okay)))))))))
