; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; New, more efficient SLEEP  1/23/92

; Earlier, simpler (and probably better) version:
;(define (sleep n)
;  (let ((until (+ (time) n)))
;    (with-interrupts-inhibited
;      (lambda ()
;        (let loop ()
;          (if (>= (time) until)
;              #t
;              (begin (dispatch)
;                     (loop))))))))

; NYI: If there are no dozers to awake, and no runnable threads, and
; we're running under time sharing, we really ought to be polite and
; relinquish the processor to other processes by doing an appropriate
; system call (on unix, this means pause, sleep, or select).

(define (sleep n)
  (let ((cv (make-condvar)))
    (with-lock dozers-lock
      (lambda ()
	(set! *dozers*
	      (insert (cons (+ (time) n) cv)
		      *dozers*
		      (lambda (frob1 frob2)
			(< (car frob1) (car frob2)))))
	(if (not *wakeup-service*)
	    (set! *wakeup-service* (spawn wakeup-service 'wakeup-service)))))
    (condvar-ref cv)))

(define dozers-lock (make-lock))

(define *dozers* '())  ;List of (wakeup-time . condvar)


; Wakeup service

(define *wakeup-service* #f)

(define (wakeup-service)
  (dynamic-wind
     relinquish-timeslice
     (lambda ()
       (let loop ()
	 (obtain-lock dozers-lock)
	 (if (not (null? *dozers*))
	     (begin (wake-up-some-threads)
		    (release-lock dozers-lock)
		    (relinquish-timeslice)
		    (loop)))))
     (lambda ()
       ;; If wakeup service gets killed, propagate kill to the threads
       ;; it was going to wake up, so their unwind forms can run.
       (for-each (lambda (dozer)
		   (kill-condvar (cdr dozer)))
		 *dozers*)
       (set! *dozers* '())		;in case of kill-thread
       (set! *wakeup-service* #f)
       (if (eq? (lock-owner dozers-lock) (current-thread))
	   (release-lock dozers-lock)))))

(define (wake-up-some-threads)
  (if (null? *dozers*)
      #f
      (if (< (time) (car (car *dozers*)))
	  #f
	  (let ((cv (cdr (car *dozers*))))
	    (set! *dozers* (cdr *dozers*))
	    (condvar-set! cv #t)
	    (wake-up-some-threads)))))

(define (insert x l <)
  (cond ((null? l) (list x))
	((< x (car l)) (cons x l))
	(else (cons (car l) (insert x (cdr l) <)))))

; Real time in seconds since some arbitrary origin.

(define (time)
  (primitive-time time-option/real-time #f))
(define primitive-time (structure-ref primitives time))

(define time-option/real-time (enum time-option real-time))



(define (read-char-with-timeout port t)
  (with-interrupts-inhibited
    (lambda ()
      (let ((deadline (+ (time) t)))
	(let loop ()
	  (cond ((char-ready? port)
		 (read-char port))
		((< (time) deadline)
		 (dispatch)
		 (loop))
		(else 'timeout)))))))
