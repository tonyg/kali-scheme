;; Rendezvous that happen at a specified time

(define (at-real-time-rv time)
  (make-time-rv (lambda () time)))

(define (after-time-rv time-difference)
  (make-time-rv (lambda () (+ (real-time) time-difference))))

(define (make-time-rv compute-time)
  (make-base
   (lambda ()
     (let ((time (compute-time)))
       (if (> (real-time) time)
	   (make-enabled 0
			 (lambda (queue)
			   (unspecific)))
	   (make-blocked
	    (lambda (trans-id cleanup-proc wrap-proc)
	      (register-dozer!
	       time
	       ;; alive?
	       (lambda ()
		 (not (trans-id-cancelled? trans-id)))
	       ;; wakeup

	       ;; It doesn't matter if this is repeated due to a
	       ;; failed commit or due to a race.  A stillborn commit
	       ;; will result in a no-op, and trans-ids can deal with
	       ;; a premature set.
	       (lambda ()
		 (with-new-proposal (lose)
		   (let ((thread-queue (make-queue)))
		     (cleanup-proc thread-queue)
		     (trans-id-set-value! trans-id
					  (cons (unspecific)
						wrap-proc))
		     (enqueue! thread-queue
			       (trans-id-thread-cell trans-id))
		     ;; we can't lose because we're running in the
		     ;; root scheduler
		     (maybe-commit-and-make-ready thread-queue))))))))))))
