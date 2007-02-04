;;; Test suite for SRFI-19
;; tests by Will Fitzgerald, augmented by John Clements -- 2004-08-16
;; port to Scheme 48 by Emilio Lopes -- 2006-12-29

(define-test-suite srfi-19-tests)
 
(define-test-case creating-time-structures srfi-19-tests
  (check (current-time 'time-tai))
  (check (current-time 'time-utc))
  (check (current-time 'time-monotonic))
           ;; currently not supported:
           ;; (current-time 'time-thread)
           ;; (current-time 'time-process)
  )
   
(define-test-case testing-time-resolutions srfi-19-tests
  (check (time-resolution 'time-tai))
  (check (time-resolution 'time-utc))
  (check (time-resolution 'time-monotonic))
  (check (time-resolution 'time-thread))
  (check (time-resolution 'time-process)))

(define-test-case time-comparisons srfi-19-tests
  (let ((t1 (make-time 'time-utc 0 1))
	(t2 (make-time 'time-utc 0 1))
	(t3 (make-time 'time-utc 0 2))
	(t11 (make-time 'time-utc 1001 1))
	(t12 (make-time 'time-utc 1001 1))
	(t13 (make-time 'time-utc 1001 2)))
    (check (time=? t1 t2))
    (check (time>? t3 t2))
    (check (time<? t2 t3))
    (check (time>=? t1 t2))
    (check (time>=? t3 t2))
    (check (time<=? t1 t2))
    (check (time<=? t2 t3))
    (check (time=? t11 t12))
    (check (time>? t13 t12))
    (check (time<? t12 t13))
    (check (time>=? t11 t12))
    (check (time>=? t13 t12))
    (check (time<=? t11 t12))
    (check (time<=? t12 t13))))

(define-test-case time-difference srfi-19-tests
  (let ((t1 (make-time 'time-utc 0 3000))
	(t2 (make-time 'time-utc 0 1000))
	(t3 (make-time 'time-duration 0 2000))
	(t4 (make-time 'time-duration 0 -2000)))
    (check (time=? t3 (time-difference t1 t2)))
    (check (time=? t4 (time-difference t2 t1)))))

(define-test-case tai-utc-conversions srfi-19-tests
  (test-one-utc-tai-edge 915148800  32 31)
  (test-one-utc-tai-edge 867715200  31 30)
  (test-one-utc-tai-edge 820454400  30 29)
  (test-one-utc-tai-edge 773020800  29 28)
  (test-one-utc-tai-edge 741484800  28 27)
  (test-one-utc-tai-edge 709948800  27 26)
  (test-one-utc-tai-edge 662688000  26 25)
  (test-one-utc-tai-edge 631152000  25 24)
  (test-one-utc-tai-edge 567993600  24 23)
  (test-one-utc-tai-edge 489024000  23 22)
  (test-one-utc-tai-edge 425865600  22 21)
  (test-one-utc-tai-edge 394329600  21 20)
  (test-one-utc-tai-edge 362793600  20 19)
  (test-one-utc-tai-edge 315532800  19 18)
  (test-one-utc-tai-edge 283996800  18 17)
  (test-one-utc-tai-edge 252460800  17 16)
  (test-one-utc-tai-edge 220924800  16 15)
  (test-one-utc-tai-edge 189302400  15 14)
  (test-one-utc-tai-edge 157766400  14 13)
  (test-one-utc-tai-edge 126230400  13 12)
  (test-one-utc-tai-edge 94694400   12 11)
  (test-one-utc-tai-edge 78796800   11 10)
  (test-one-utc-tai-edge 63072000   10 0)
  (test-one-utc-tai-edge 0   0 0) ;; at the epoch
  (test-one-utc-tai-edge 10   0 0) ;; close to it ...
  (test-one-utc-tai-edge 1045789645 32 32) ;; about now ...
  )
   
(define-test-case tai-date-conversions srfi-19-tests
  (check (tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 29)) 0)
		   (make-date 0 58 59 23 31 12 1998 0)))
  (check (tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 30)) 0)
		   (make-date 0 59 59 23 31 12 1998 0)))
  (check (tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 31)) 0)
		   (make-date 0 60 59 23 31 12 1998 0)))
  (check (tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 32)) 0)
		   (make-date 0 0 0 0 1 1 1999 0))))
   
(define-test-case date-utc-conversions srfi-19-tests
  (check (time=? (make-time time-utc 0 (- 915148800 2))
		 (date->time-utc (make-date 0 58 59 23 31 12 1998 0))))
  (check (time=? (make-time time-utc 0 (- 915148800 1))
		 (date->time-utc (make-date 0 59 59 23 31 12 1998 0))))
  ;; yes, I think this is acutally right.
  (check (time=? (make-time time-utc 0 (- 915148800 0))
		 (date->time-utc (make-date 0 60 59 23 31 12 1998 0))))
  (check (time=? (make-time time-utc 0 (- 915148800 0))
		 (date->time-utc (make-date 0 0 0 0 1 1 1999 0))))
  (check (time=? (make-time time-utc 0 (+ 915148800 1))
		 (date->time-utc (make-date 0 1 0 0 1 1 1999 0)))))
   
(define-test-case tz-offset-conversions srfi-19-tests
  (let ((ct-utc (make-time time-utc 6320000 1045944859))
	(ct-tai (make-time time-tai 6320000 1045944891))
	(cd (make-date 6320000 19 14 15 22 2 2003 -18000)))
    (check (time=? ct-utc (date->time-utc cd)))
    (check (time=? ct-tai (date->time-tai cd)))))

(define-test-case date->string-conversions srfi-19-tests
  (check (date->string (make-date 1000 2 3 4 5 6 2007 -120)
		       "~~.~a.~A.~b.~B.~c.~d.~D.~e,~f,~h.~H")
	 =>
	 ;; original, bogus: "~.Tue.Tuesday.Jun.June.Tue Jun 5 4:03:02-0200 2007.05.06/05/07. 5,2.000001,Jun.03"
	 "~.Tue.Tuesday.Jun.June.Tue Jun 05 04:03:02-0002 2007.05.06/05/07. 5,02.000001,Jun.04"))
	 
(define-test-case date<->julian-day-conversion srfi-19-tests
  (check (- (date->julian-day (make-date 0 0 0 0 1 1 2004 0))
	    (date->julian-day (make-date 0 0 0 0 1 1 2003 0)))
	 => 365)
  (let ((test-date (make-date 0 0 0 0 1 1 2003 -7200)))
    (check (tm:date= test-date (julian-day->date (date->julian-day test-date) -7200)))))
   
(define-test-case date->modified-julian-day-conversion srfi-19-tests
  (check (- (date->modified-julian-day (make-date 0 0 0 0 1 1 2004 0))
	    (date->modified-julian-day (make-date 0 0 0 0 1 1 2003 0))) 
	 => 365)
  (let ((test-date (make-date 0 0 0 0 1 1 2003 -7200)))
    (check (tm:date= test-date (modified-julian-day->date (date->modified-julian-day test-date) -7200)))))

(define (test-one-utc-tai-edge utc tai-diff tai-last-diff)
  (let* ( ;; right on the edge they should be the same
	 (utc-basic (make-time 'time-utc 0 utc))
	 (tai-basic (make-time 'time-tai 0 (+ utc tai-diff)))
	 (utc->tai-basic (time-utc->time-tai utc-basic))
	 (tai->utc-basic (time-tai->time-utc tai-basic))
         
	 ;; a second before they should be the old diff
	 (utc-basic-1 (make-time 'time-utc 0 (- utc 1)))
	 (tai-basic-1 (make-time 'time-tai 0 (- (+ utc tai-last-diff) 1)))
	 (utc->tai-basic-1 (time-utc->time-tai utc-basic-1))
	 (tai->utc-basic-1 (time-tai->time-utc tai-basic-1))
         
	 ;; a second later they should be the new diff
	 (utc-basic+1 (make-time 'time-utc 0 (+ utc 1)))
	 (tai-basic+1 (make-time 'time-tai 0 (+ (+ utc tai-diff) 1)))
	 (utc->tai-basic+1 (time-utc->time-tai utc-basic+1))
	 (tai->utc-basic+1 (time-tai->time-utc tai-basic+1))
         
	 ;; ok, let's move the clock half a month or so plus half a second
	 (shy (* 15 24 60 60))
	 (hs (/ (expt 10 9) 2))
	 ;; a second later they should be the new diff
	 (utc-basic+2 (make-time 'time-utc hs (+ utc shy)))
	 (tai-basic+2 (make-time 'time-tai hs (+ (+ utc tai-diff) shy)))
	 (utc->tai-basic+2 (time-utc->time-tai utc-basic+2))
	 (tai->utc-basic+2 (time-tai->time-utc tai-basic+2)))
    
    (check (time=? utc-basic tai->utc-basic))
    (check (time=? tai-basic utc->tai-basic))
    (check (time=? utc-basic-1 tai->utc-basic-1))
    (check (time=? tai-basic-1 utc->tai-basic-1))
    (check (time=? utc-basic+1 tai->utc-basic+1))
    (check (time=? tai-basic+1 utc->tai-basic+1))
    (check (time=? utc-basic+2 tai->utc-basic+2))
    (check (time=? tai-basic+2 utc->tai-basic+2))))


(define (tm:date= d1 d2)
  (and (= (date-year d1) (date-year d2))
       (= (date-month d1) (date-month d2))
       (= (date-day d1) (date-day d2))
       (= (date-hour d1) (date-hour d2))
       (= (date-second d1) (date-second d2))
       (= (date-nanosecond d1) (date-nanosecond d2))
       (= (date-zone-offset d1) (date-zone-offset d2))))
