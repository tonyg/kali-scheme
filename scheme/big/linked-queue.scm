; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This file is no longer used.

; Queues implemented as doubly linked lists (because the thread package needs
; to delete queue entries quickly).

; The exported procedures are those of the simpler queue package, with the
; addition of DELETE-QUEUE-ENTRY!.  ENQUEUE! returns a queue-entry which can
; then be passed to DELETE-QUEUE-ENTRY! to remove the thing from the queue.

(define-record-type q-entry :q-entry
  (make-q-entry data prev next)
  q-entry?
  (data q-entry-data)
  (prev q-entry-prev set-q-entry-prev!)
  (next q-entry-next set-q-entry-next!))

(define queue? q-entry?)

(define (make-queue)
  (let ((e (make-q-entry #f #f #f)))
    (set-q-entry-prev! e e)
    (set-q-entry-next! e e)
    e))

(define (queue-empty? q)
  (eq? (q-entry-next q) q))

(define (enqueue! q thing)
  (let* ((prev (q-entry-prev q))
	 (e (make-q-entry thing prev q)))
    (set-q-entry-prev! q e)
    (set-q-entry-next! prev e)
    e))

(define (queue-head q)
  (let ((e (q-entry-next q)))
    (if (eq? q e)	;(queue-empty? q)
	(error "empty queue" q)
	(q-entry-data e))))

(define (dequeue! q)
  (let ((e (q-entry-next q)))
    (cond ((eq? q e)	;(queue-empty? q)
	   (error "empty queue" q))
	  (else
	   (set-q-entry-next! q (q-entry-next e))
	   (set-q-entry-prev! (q-entry-next q) q)
	   (q-entry-data e)))))

(define (delete-queue-entry! e)
  (let ((next (q-entry-next e))
	(prev (q-entry-prev e)))
    (set-q-entry-next! prev next)
    (set-q-entry-prev! next prev)))

(define (queue->list q)
  (do ((e (q-entry-prev q) (q-entry-prev e))
       (l '() (cons (q-entry-data e) l)))
      ((eq? q e) l)))

(define (queue-length q)
  (do ((e (q-entry-prev q) (q-entry-prev e))
       (l 0 (+ l 1)))
      ((eq? q e) l)))

(define (delete-from-queue! q v)
  (let loop ((e (q-entry-next q)))
    (cond ((eq? e q))
	  ((eq? (q-entry-data e) v)
	   (delete-queue-entry! e))
	  (else
	   (loop (q-entry-next e))))))
