; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Locks (= semaphores)

; Each lock has:
;   The owning thread's uid, or #f if not locked.  The uid can be used
;     to aid debugging without introducing the overhead of a weak pointer
;     to the actual thread (a non-weak pointer would introduce an unfortunate
;     circularity involving the locks and finalizers of ports).
;   A queue of waiting threads

(define-record-type lock :lock
  (really-make-lock owner-uid queue uid)
  lock?
  (owner-uid lock-owner-uid set-lock-owner-uid!)
  (queue lock-queue)
  (uid lock-uid))     ; for debugging

(define *lock-uid* 0)

(define (make-lock)
  (let ((uid *lock-uid*))
    (set! *lock-uid* (+ uid 1))
    (really-make-lock #f (make-thread-queue) uid)))

(define (obtain-lock lock)
  (with-interrupts-inhibited
   (lambda ()
     (if (lock-owner-uid lock)
	 (begin
	   (enqueue-thread! (lock-queue lock) (current-thread))
	   (block))
	 (set-lock-owner-uid! lock (thread-uid (current-thread)))))))

(define (maybe-obtain-lock lock)
  (with-interrupts-inhibited
   (lambda ()
     (if (lock-owner-uid lock)
	 #f
	 (begin
	   (set-lock-owner-uid! lock (thread-uid (current-thread)))
	   #t)))))

; Returns #t if the lock has no new owner.

(define (release-lock lock)
  (with-interrupts-inhibited
   (lambda ()
     (let ((queue (lock-queue lock)))
       (if (thread-queue-empty? queue)
	   (begin
	     (set-lock-owner-uid! lock #f)
	     #t)
	   (let ((next (dequeue-thread! queue)))
	     (set-lock-owner-uid! lock (thread-uid next))
	     (make-ready next)
	     #f))))))
