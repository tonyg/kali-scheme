;;; Test suite for SRFI-19
;; tests by Will Fitzgerald, augmented by John Clements -- 2004-08-16
;; port to Scheme 48 by Emilio Lopes -- 2006-12-29

(define-syntax assert
  (syntax-rules ()
    ((_ op ...)
     (check (op ...) => #t))))

(define-syntax assert-equal?
  (syntax-rules ()
    ((_ op1 op2)
     (check op1 (=> equal?) op2))))

(define-syntax not-exception
  (syntax-rules ()
    ((_ expr)
     (guard (condition
             ((serious-condition? condition) #f)
             (else #t))
       expr
       #t))))

(define-syntax assert-not-exn
  (syntax-rules ()
    ((_ expr)
     (check (not-exception expr) => #t))))

(define-record-type :test-case
    (really-make-test-case description expression)
  (description test-case-description)
  (expression test-case-expression))

(define-syntax make-test-case
  (syntax-rules ()
    ((_ description expression)
     (really-make-test-case description (lambda () expression)))))

(define (run-test-case test-case)
  (format #t "** Running test: ~a~%" (test-case-description test-case))
  ((test-case-expression test-case)))

(define-record-type :test-suite
    (really-make-test-suite description test-cases)
  (description test-suite-description)
  (test-cases test-suite-cases))

(define (make-test-suite description . tests)
  (really-make-test-suite description tests))

(define (run-test-suite test-suite)
  (format #t "*** Running test suite: ~a~%" (test-suite-description test-suite))
  (check-reset!)
  (check-set-mode! 'report-failed)
  (for-each run-test-case (test-suite-cases test-suite))
  (check-report))

(define test/text-ui run-test-suite)

(define (run-tests)
  (run-test-suite srfi-19-test-suite))

(define srfi-19-test-suite
  (make-test-suite
   "Tests for SRFI 19"
   
   (make-test-case
    "Creating time structures"
    (assert-not-exn 
     (list (current-time 'time-tai)
           (current-time 'time-utc)
           (current-time 'time-monotonic)
           ;; currently not supported:
           ;; (current-time 'time-thread)
           ;; (current-time 'time-process)
           )))
   
   (make-test-case
    "Testing time resolutions"
    (assert-not-exn
     (list (time-resolution 'time-tai)
           (time-resolution 'time-utc)
           (time-resolution 'time-monotonic)
           (time-resolution 'time-thread)
           (time-resolution 'time-process))))
   
   (make-test-case 
    "Time comparisons (time=?, etc.)"
    (let ((t1 (make-time 'time-utc 0 1))
	  (t2 (make-time 'time-utc 0 1))
	  (t3 (make-time 'time-utc 0 2))
	  (t11 (make-time 'time-utc 1001 1))
	  (t12 (make-time 'time-utc 1001 1))
	  (t13 (make-time 'time-utc 1001 2)))
      (assert time=? t1 t2)
      (assert time>? t3 t2)
      (assert time<? t2 t3)
      (assert time>=? t1 t2)
      (assert time>=? t3 t2)
      (assert time<=? t1 t2)
      (assert time<=? t2 t3)
      (assert time=? t11 t12)
      (assert time>? t13 t12)
      (assert time<? t12 t13)
      (assert time>=? t11 t12)
      (assert time>=? t13 t12)
      (assert time<=? t11 t12)
      (assert time<=? t12 t13)))
   
   (make-test-case
    "Time difference"
    (let ((t1 (make-time 'time-utc 0 3000))
	  (t2 (make-time 'time-utc 0 1000))
	  (t3 (make-time 'time-duration 0 2000))
	  (t4 (make-time 'time-duration 0 -2000)))
      (assert time=? t3 (time-difference t1 t2))
      (assert time=? t4 (time-difference t2 t1))))
   
   (make-test-case
    "TAI-UTC Conversions"
    (begin
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
      (test-one-utc-tai-edge 0   0 0)         ;; at the epoch
      (test-one-utc-tai-edge 10   0 0)        ;; close to it ...
      (test-one-utc-tai-edge 1045789645 32 32) ;; about now ...
      ))
   
   (make-test-case
    "TAI-Date Conversions"
    (begin
      (assert tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 29)) 0)
              (make-date 0 58 59 23 31 12 1998 0))
      (assert tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 30)) 0)
              (make-date 0 59 59 23 31 12 1998 0))
      (assert tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 31)) 0)
              (make-date 0 60 59 23 31 12 1998 0))
      (assert tm:date= (time-tai->date (make-time time-tai 0 (+ 915148800 32)) 0)
              (make-date 0 0 0 0 1 1 1999 0))))
   
   
   (make-test-case 
    "Date-UTC Conversions"
    (begin
      (assert time=? (make-time time-utc 0 (- 915148800 2))
              (date->time-utc (make-date 0 58 59 23 31 12 1998 0)))
      (assert time=? (make-time time-utc 0 (- 915148800 1))
              (date->time-utc (make-date 0 59 59 23 31 12 1998 0)))
      ;; yes, I think this is acutally right.
      (assert time=? (make-time time-utc 0 (- 915148800 0))
              (date->time-utc (make-date 0 60 59 23 31 12 1998 0)))
      (assert time=? (make-time time-utc 0 (- 915148800 0))
              (date->time-utc (make-date 0 0 0 0 1 1 1999 0)))
      (assert time=? (make-time time-utc 0 (+ 915148800 1))
              (date->time-utc (make-date 0 1 0 0 1 1 1999 0)))))
   
   (make-test-case 
    "TZ Offset conversions"
    (let ((ct-utc (make-time time-utc 6320000 1045944859))
	  (ct-tai (make-time time-tai 6320000 1045944891))
	  (cd (make-date 6320000 19 14 15 22 2 2003 -18000)))
      (assert time=? ct-utc (date->time-utc cd))
      (assert time=? ct-tai (date->time-tai cd))))
   
   (make-test-case
    "date->string conversions"
    (begin
      (assert-equal? "~.Tue.Tuesday.Jun.June.Tue Jun 05 04:03:02-0002 2007.05.06/05/07. 5,02.000001,Jun.04"
                     ;; original, bogus: "~.Tue.Tuesday.Jun.June.Tue Jun 5 4:03:02-0200 2007.05.06/05/07. 5,2.000001,Jun.03"
                     (date->string (make-date 1000 2 3 4 5 6 2007 -120)
                                   "~~.~a.~A.~b.~B.~c.~d.~D.~e,~f,~h.~H"))))
   
   (make-test-case
    "date<->julian-day conversion"
    (begin (assert = 365 (- (date->julian-day (make-date 0 0 0 0 1 1 2004 0))
                            (date->julian-day (make-date 0 0 0 0 1 1 2003 0))))
           (let ((test-date (make-date 0 0 0 0 1 1 2003 -7200)))
             (assert tm:date= test-date (julian-day->date (date->julian-day test-date) -7200)))))
   
   (make-test-case
    "date->modified-julian-day conversion"
    (begin (assert = 365 (- (date->modified-julian-day (make-date 0 0 0 0 1 1 2004 0))
                            (date->modified-julian-day (make-date 0 0 0 0 1 1 2003 0))))
           (let ((test-date (make-date 0 0 0 0 1 1 2003 -7200)))
             (assert tm:date= test-date (modified-julian-day->date (date->modified-julian-day test-date) -7200)))))
   ))

(define (test-one-utc-tai-edge utc tai-diff tai-last-diff)
  (let* (;; right on the edge they should be the same
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
    
    (assert time=? utc-basic tai->utc-basic)
    (assert time=? tai-basic utc->tai-basic)
    (assert time=? utc-basic-1 tai->utc-basic-1)
    (assert time=? tai-basic-1 utc->tai-basic-1)
    (assert time=? utc-basic+1 tai->utc-basic+1)
    (assert time=? tai-basic+1 utc->tai-basic+1)
    (assert time=? utc-basic+2 tai->utc-basic+2)
    (assert time=? tai-basic+2 utc->tai-basic+2)))


(define (tm:date= d1 d2)
  (and (= (date-year d1) (date-year d2))
       (= (date-month d1) (date-month d2))
       (= (date-day d1) (date-day d2))
       (= (date-hour d1) (date-hour d2))
       (= (date-second d1) (date-second d2))
       (= (date-nanosecond d1) (date-nanosecond d2))
       (= (date-zone-offset d1) (date-zone-offset d2))))
