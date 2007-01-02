; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Channel interrupt stuff.

; Block reads and writes in terms of partial reads and writes.

; CHANNEL-READ returns the number of characters read or the EOF object.
; BUFFER is either a string or byte vector and START is the index at which
; to place the first character read.  COUNT is the maximum number of characters
; that may be read.  If WAIT? is true the thread should block if nothing
; is immediately available.

; We disable interrupts to be sure that we have installed CONDVAR before
; any completion interrupt arrives.
;
; Like all maybe-commits, this returns #T if it successfully committed and
; #F if the commit failed.

(define (channel-maybe-commit-and-read channel buffer start count condvar wait?)
  (maybe-commit-no-interrupts
   (lambda ()
     (let ((got (channel-maybe-read channel buffer start count wait?)))
       (cond
	((not got)
	 (add-channel-condvar! channel condvar))
	((cell? got)
	 (note-channel-result! condvar
			       (make-i/o-error (cell-ref got)
					       channel-maybe-read
					       (list channel buffer start count wait?))))
	(else
	 (note-channel-result! condvar got)))))))

(define (channel-maybe-commit-and-write channel buffer start count condvar wait?)
  (maybe-commit-no-interrupts
   (lambda ()
     (let ((got (channel-maybe-write channel buffer start count)))
       (cond
	((not got)
	 (add-channel-condvar! channel condvar)
	 (if wait?
	     (with-new-proposal (lose)
	       (maybe-commit-and-wait-for-condvar condvar))))
	((cell? got)
	 (note-channel-result! condvar
			       (make-i/o-error (cell-ref got)
					       channel-maybe-write
					       (list channel buffer start count))))
	(else
	 (note-channel-result! condvar got)))))))

; Set CONDVAR's value to be RESULT.

(define (note-channel-result! condvar result)
  (with-new-proposal (lose)
    (or (maybe-commit-and-set-condvar! condvar result)
	(lose))))

; Used for stderr, which is unbuffered both here and in C.

(define (channel-write channel buffer start count)
  (let ((ints (disable-interrupts!)))
    (let ((res (channel-maybe-write channel buffer start count)))
      (if res
	  (begin
	    (set-enabled-interrupts! ints)
	    res)
	  (let ((condvar (make-condvar)))
	    (add-channel-condvar! channel condvar)
            (with-new-proposal (lose)
              (or (maybe-commit-and-wait-for-condvar condvar)
                  (lose)))
	    (set-enabled-interrupts! ints)
	    (condvar-value condvar))))))
  
;----------------

(define (channel-maybe-commit-and-close channel close-channel)
  (maybe-commit-no-interrupts
   (lambda ()
     (let ((condvar (fetch-channel-condvar! channel)))
       (if condvar
	   (begin
	     (channel-abort channel)
	     (close-channel channel)
	     (note-channel-result! condvar
				   (if (input-channel? channel)
				       (eof-object)
				       0)))
	   (close-channel channel))))))

(define (input-channel? channel)
  (= (channel-status channel)
     (enum channel-status-option input)))

;----------------
; Install an interrupt handler that queues up the results of completed I/O
; operations and spawn a thread to cope with them.  This is written so as
; to avoid having state in top-level variables, because their values are
; saved in dumped images.

(define (initialize-channel-i/o!)
  (session-data-set! channel-wait-condvars-slot '())
  (set-interrupt-handler! (enum interrupt i/o-completion)
			  i/o-completion-handler))

; The warning message is printed using DEBUG-MESSAGE because to try to make
; sure it appears in spite of whatever problem's the I/O system is having.
;
; Called with interrupts disabled.

(define (i/o-completion-handler channel error? status enabled-interrupts)
  (let ((condvar (fetch-channel-condvar! channel)))
    (if condvar
	(note-channel-result! condvar
			      (if error?
				  (make-i/o-error status
						  i/o-completion-handler
						  (list channel error? status enabled-interrupts))
				  status)))))
				  

; Exported procedure
; This should check the list for condvars which have no waiters.

(define (waiting-for-i/o?)
  (abort-unwanted-reads!)
  (not (null? (channel-condvars))))

;----------------
; A session slot contains an alist mapping channels to condvars for the result
; of an i/o operation on that channel.

(define channel-wait-condvars-slot
  (make-session-data-slot! '()))

(define (channel-condvars)
  (session-data-ref channel-wait-condvars-slot))

(define (set-channel-condvars! condvars)
  (session-data-set! channel-wait-condvars-slot condvars))
  
; Adding a condvar and channel - the caller has already determined there is no
; existing condvar for this channel.

(define (add-channel-condvar! channel condvar)
  (set-channel-condvars! (cons (cons channel condvar)
			       (channel-condvars))))

; Exported interface.

(define wait-for-channel add-channel-condvar!)

; This just deletes from the alist.

(define (fetch-channel-condvar! channel)
  (let ((condvars (channel-condvars)))
    (cond ((null? condvars)
	   #f)
	  ((eq? channel (caar condvars))
	   (set-channel-condvars! (cdr condvars))
	   (cdar condvars))
	  (else
	   (let loop ((condvars (cdr condvars)) (prev condvars))
	     (cond ((null? condvars)
		    #f)
		   ((eq? channel (caar condvars))
		    (set-cdr! prev (cdr condvars))
		    (cdar condvars))
		   (else
		    (loop (cdr condvars) condvars))))))))
  
; Abort the read operations for any channel whose condvar no longer has waiters.

; The main purpose of ABORT-UNWANTED-READS is to abort reads after the
; reading threads have died.  The Scheme process sticks around until
; all I/O has been completed and there is no point in waiting for a
; read if no one wants the result.

; One upon a time, the intention was to have this procedure abort
; unwanted writes as well.  However, we must not abort writes which
; come from the automatic buffer flushing routine, which is hard to
; detect here.  Moreover, the automatic buffer flushing is currently
; hard to abort.

(define (abort-unwanted-reads!)
  (let ((ints (disable-interrupts!)))
    (let loop ((condvars (channel-condvars)) (okay '()))
      (if (null? condvars)
	  (begin
	    (set-channel-condvars! okay)
	    (set-enabled-interrupts! ints))
	  (let ((condvar (cdar condvars)))
	    (loop (cdr condvars)
		  (if (or (not (input-channel? (caar condvars)))
			  (condvar-has-waiters? condvar))
		      (cons (car condvars) okay)
		      (begin
			(channel-abort (caar condvars))
			(note-channel-result! condvar 0)
			okay))))))))

