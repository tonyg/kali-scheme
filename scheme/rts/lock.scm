; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Locks (= semaphores)

; Each lock has:
;   The owning thread's uid, or #f if not locked.  The uid can be used
;     to aid debugging without introducing the overhead of a weak pointer
;     to the actual thread (a non-weak pointer would introduce an unfortunate
;     circularity involving the locks and finalizers of ports).
;   A queue of waiting threads

(define-synchronized-record-type lock :lock
  (really-make-lock owner-uid queue uid)
  (owner-uid)
  lock?
  (owner-uid lock-owner-uid set-lock-owner-uid!)
  (queue lock-queue)
  (uid lock-uid))     ; for debugging

(define lock-uid (list 0))

(define (next-uid)
  (atomically
    (let ((uid (provisional-car lock-uid)))
      (provisional-set-car! lock-uid (+ uid 1))
      uid)))

(define (make-lock)
  (really-make-lock #f
		    (make-thread-queue)
		    (next-uid)))

(define (obtain-lock lock)
  (with-new-proposal (lose)
    (or (cond ((lock-owner-uid lock)
	       (enqueue-thread! (lock-queue lock)
				(current-thread))
	       (maybe-commit-and-block))
	      (else
	       (set-lock-owner-uid! lock (thread-uid (current-thread)))
	       (maybe-commit)))
	(lose))))

; Returns #T if the lock is obtained and #F if not.  Doesn't block.

(define (maybe-obtain-lock lock)
  (with-new-proposal (lose)
    (cond ((lock-owner-uid lock)	; no need to commit - we have only done
	   #f)				; a single read
	  (else
	   (set-lock-owner-uid! lock (thread-uid (current-thread)))
	   (or (maybe-commit)
	       (lose))))))

; Returns #t if the lock has no new owner.

(define (release-lock lock)
  (with-new-proposal (lose)
    (let ((queue (lock-queue lock)))
      (if (thread-queue-empty? queue)
	  (begin
	    (set-lock-owner-uid! lock #f)
	    (or (maybe-commit)
		(lose)))
	  (let ((next (dequeue-thread! queue)))
	    (set-lock-owner-uid! lock (thread-uid next))
	    (or (maybe-commit-and-make-ready next)
		(lose)))))))




