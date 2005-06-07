; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Locks (= semaphores)

; Each lock has:
;   The owning thread, or #f if not locked.  We use the owning thread instead
;     of #t as an aid to debugging.
;   A queue of waiting threads

(define-synchronized-record-type lock :lock
  (really-make-lock owner queue uid)
  (owner)
  lock?
  (owner lock-owner set-lock-owner!)
  (queue lock-queue)
  (uid lock-uid))     ; for debugging

(define lock-uid (list 0))

(define (next-uid)
  (atomically
    (let ((uid (provisional-car lock-uid)))
      (provisional-set-car! lock-uid (+ uid 1))
      uid)))

(define (make-lock)
  (really-make-lock #f (make-queue) (next-uid)))

(define (obtain-lock lock)
  (with-new-proposal (lose)
    (or (cond ((lock-owner lock)
	       (maybe-commit-and-block-on-queue (lock-queue lock)))
	      (else
	       (set-lock-owner! lock (current-thread))
	       (maybe-commit)))
	(lose))))

; Returns #T if the lock is obtained and #F if not.  Doesn't block.

(define (maybe-obtain-lock lock)
  (with-new-proposal (lose)
    (cond ((lock-owner lock)	; no need to commit - we have only done
	   #f)			; a single read
	  (else
	   (set-lock-owner! lock (current-thread))
	   (or (maybe-commit)
	       (lose))))))

; Returns #t if the lock has no new owner.

(define (release-lock lock)
  (with-new-proposal (lose)
    (let ((next (maybe-dequeue-thread! (lock-queue lock))))
      (cond (next
	     (set-lock-owner! lock next)
	     (or (maybe-commit-and-make-ready next)
		 (lose)))
	    (else
	     (set-lock-owner! lock #f)
	     (or (maybe-commit)
		 (lose)))))))




