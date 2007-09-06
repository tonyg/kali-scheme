; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; External events from C code

; Every type of external event gets a unique uid.  We use the shared
; bindings to preserve them in the image.

(define *number-of-event-types* 100)

(define *event-types*)

(define-record-type event-type :event-type
  (make-event-type uid used? next)
  (uid    integer event-type-uid)
  (used?  boolean event-type-used? set-event-type-used?!)
  ;; the pending external events form a queue, just like the channels
  (next   event-type event-type-next set-event-type-next!))

(define *pending-event-types-head*)
(define *pending-event-types-tail*)
(define *pending-event-types-ready*) ; last external event actually returned by event sysstem

(define (initialize-external-events)
  (set! *event-types* (make-vector *number-of-event-types* (null-pointer)))
  (if (null-pointer? *event-types*)
      (error "out of memory, unable to continue"))
  (let ((event-types-count *number-of-event-types*))
    (set! *number-of-event-types* 0)
    (if (not (add-external-event-types event-types-count))
	(error "out of memory, unable to continue")))
  (set! *pending-event-types-head* (null-pointer))
  (set! *pending-event-types-tail* (null-pointer))
  (set! *pending-event-types-ready* (null-pointer)))

(define (add-external-event-types min-count)
  (let ((old-event-types *event-types*)
	(old-count *number-of-event-types*)
	(new-event-types (make-vector min-count (null-pointer))))
    (if (null-pointer? new-event-types)
	#f
	(let loop ((i 0))
	  (cond
	   ((= i min-count)
	    (set! *event-types* new-event-types)
	    (set! *number-of-event-types* min-count)
	    (deallocate old-event-types)
	    #t)
	   ((< i old-count)
	    (vector-set! new-event-types i
			 (vector-ref old-event-types i))
	    (goto loop (+ 1 i)))
	   (else
	    (let ((t (make-event-type i #f (null-pointer))))
	      (if (null-pointer? t)
		  (begin
		    (set! *event-types* new-event-types)
		    (set! *number-of-event-types* i)
		    #f)
		  (begin
		    (vector-set! new-event-types i t)
		    (goto loop (+ 1 i)))))))))))

(define (use-event-type-uid! id)
  (set-event-type-used?! (vector-ref *event-types* id)
			 #t))

; returns -1 on out-of-memory
; this could be optimized via a free list, for example
(define (unused-event-type-uid)
  (let loop ((i 0))
    (cond
     ((= i *number-of-event-types*)
      (if (add-external-event-types (* 2 *number-of-event-types*))
	  (begin
	    (use-event-type-uid! i)
	    i)
	  -1))
     ((not (event-type-used? (vector-ref *event-types* i)))
      (use-event-type-uid! i)
      i)
     (else (loop (+ 1 i))))))
		   
(define (s48-external-event-uid name)
  (let* ((binding (get-imported-binding name))
	 (uid-val (shared-binding-ref binding)))
    
    (define (indeed uid)
      (shared-binding-set! binding (enter-fixnum uid))
      (use-event-type-uid! uid)
      uid)

    (if (fixnum? uid-val)
	(begin
	  (let ((uid (extract-fixnum uid-val)))
	    (cond
	     ((< uid *number-of-event-types*)
	      (indeed uid))
	     ((add-external-event-types (+ 1 uid))
	      (indeed uid))
	     (else -1)))) ; out of memory
	(let ((uid (unused-event-type-uid)))
	  (if (= -1 uid)
	      uid
	      (indeed uid))))))

(define (s48-unregister-external-event-uid index)
 
  (define (lose/invalid)
    (write-error-string "trying to unregister invalid external event: ")
    (write-error-integer index)
    (write-error-newline)
    (unspecific)) 

  (if (>= index *number-of-event-types*)
      (lose/invalid))

  (let ((type (vector-ref *event-types* index)))
    (cond
     ((not (event-type-used? type))
      (lose/invalid))
     ((not (null-pointer? (event-type-next type)))
      (write-error-string "trying to unregister external event that is still in use : ")
      (write-error-integer index)
      (write-error-newline)
      (unspecific))
     (else
      (set-event-type-used?! type #f)))))

; Pending events

; This is intended to be called by the C code, but will generally
; need some sort of mutex protection there.

(define (s48-external-event-ready?/unsafe)
  (not (null-pointer? *pending-event-types-ready*)))

(define (s48-external-event-pending?/unsafe)
  (if (s48-external-event-ready?/unsafe)
      (begin
	(set! *pending-event-types-ready* (event-type-next *pending-event-types-ready*))
	#t)
      #f))

(define (s48-note-external-event!/unsafe index)

  (define (lose)
    (write-error-string "invalid external event: ")
    (write-error-integer index)
    (write-error-newline)
    (unspecific))

  (if (>= index *number-of-event-types*)
      (lose))

  (let ((type (vector-ref *event-types* index)))
    (cond
     ((not (event-type-used? type)) (lose))
     ((or (not (null-pointer? (event-type-next type))) ; already queued
	  (eq? type *pending-event-types-head*)  ; first and only
	  (eq? type *pending-event-types-tail*)) ; last
      (unspecific))
     ((null-pointer? *pending-event-types-head*)
      (set! *pending-event-types-head* type)
      (set! *pending-event-types-tail* type)
      (set! *pending-event-types-ready* type))
     (else
      (set-event-type-next! *pending-event-types-tail* type)
      (set! *pending-event-types-tail* type)))))

; returns a uid and a boolean indicating whether more events are
; pending afterwards
(define (s48-dequeue-external-event!/unsafe)
  (let* ((type *pending-event-types-head*)
	 (next (event-type-next type)))
    (set! *pending-event-types-head* next)
    (set-event-type-next! type (null-pointer))
    (if (null-pointer? next)
	(set! *pending-event-types-tail* (null-pointer)))
    (values (event-type-uid type)
	    (s48-external-event-ready?/unsafe))))
