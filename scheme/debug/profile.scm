; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This was a fun hack, but I didn't get much useful information out of
; it -- a profiler that only samples at points allowed by the VM's
; interrupt mechanism doesn't tell you what you want to know.  The
; only information available at that point is the continuation; what
; we really want to know is where the PC has been.  In particular, the
; only procedures that show up in the table at all are those that call
; other procedures.    JAR 12/92

'
(define-structure profiler (export profile)
  (open scheme-level-2 handle exception ;interrupts
	architecture continuation signals condition template
	table structure-refs debug-data sort
	clock) ;schedule-interrupt
  (files (misc profile)))

(define (profile thunk frequency)
  (let ((table (make-table template-uid))
	(dt (round (/ (expt 10 6) frequency))))
    (primitive-catch
      (lambda (k0)
	(let ((foo (continuation-template k0)))
	  (with-handler
	      (lambda (c punt)
		(if (and (interrupt? c)
			 (eq? (interrupt-type c) interrupt/alarm))
		    (primitive-catch
		      (lambda (k)
			(record-profile-information! k foo table)
			(schedule-interrupt dt)))
		    (punt)))
	    (lambda ()
	      (dynamic-wind (lambda () (schedule-interrupt dt -6))
			    thunk
			    (lambda () (schedule-interrupt 0 1))))))))
    table))

(define (record-profile-information! k k0-template table)
  (let ((k1 (continuation-cont (continuation-cont k))))
    (let ((z (get-counts table k1)))
      (set-car! z (+ (car z) 1))
      (set-cdr! z (+ (cdr z) 1)))
    (do ((k (continuation-parent k1) (continuation-parent k)))
	((or (not (continuation? k))
	     (eq? (continuation-template k) k0-template)))
      (let ((z (get-counts table k)))
	(set-cdr! z (+ (cdr z) 1))))))

(define (get-counts table k)
  (let ((info (template-info (continuation-template k))))
    (or (table-ref table info)
	(let ((z (cons 0 0)))
	  (table-set! table info z)
	  z))))

(define (template-uid info)
  (cond ((integer? info)
	 info)
	((debug-data? info)
	 (debug-data-uid info))
	(else 0)))  ;??

(define interrupt-type cadr)
(define interrupt/alarm (enum interrupt alarm))

(define (dump t)
  (let ((l '()))
    (table-walk (lambda (key count)
		  (let ((dd (if (integer? key)
				(table-ref debug-data-table key)
				key)))
		    (set! l (cons (cons count
					(if (debug-data? dd)
					    (debug-data-names dd)
					    `(? ,key)))
				  l))))
		t)
    (do ((l (sort-list l more-interesting?)
	    (cdr l))
	 (i 0 (+ i 1)))
	((or (null? l) (> i *prefix*)))
      (let* ((counts+names (car l))
	     (leaf-count (caar counts+names))
	     (total-count (cdar counts+names))
	     (names (cdr counts+names)))
	(display (pad-left total-count 6)) (display #\space)
	(display (pad-left leaf-count 6)) (display #\space)
	(write names)
	(newline)))))

(define (more-interesting? x y)
  (let ((c1 (cdar x))
	(c2 (cdar y)))
    (or (> c1 c2)
	(and (= c1 c2)
	     (> (caar x) (caar y))))))


(define *prefix* 60)
(define (pad-left s n)
  (let ((s (cond ((number? s) (number->string s))
		 ((symbol? s) (symbol->string s))
		 (else s))))
    (string-append (make-string (- n (string-length s)) #\space)
		   s)))
