; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; The barest skeleton of a test suite.
; Mostly it makes sure that many of the external packages load without
; error.

; ,exec ,load debug/check.scm
; (done)

(load-package 'testing)

(config '(run 
	  (define-structure bar (export)
	    (open scheme testing))))

(in 'bar '(bench off))
(in 'bar '(run (define (foo) (cadr '(a b)))))
(in 'bar '(run (define cadr list)))
(in 'bar '(run (test "non-bench" equal? '((a b)) (foo))))

(in 'bar '(bench on))
(in 'bar '(run (define (foo) (car '(a b)))))
(in 'bar '(run (define car list)))
(in 'bar '(run (test "bench" equal? 'a (foo))))

(config '(run
	  (define-structure test1 (export x)
	    (open scheme testing)
	    (begin (define x 10)
		   (define (z) x)))))
(config '(run 
	  (define-structure test2 (export)
	    (open scheme test1 testing)
	    (begin (define (z) x)))))
(config '(run
	  (define-structure test3 (export)
	    (open scheme test1 testing)
	    (begin (define (z) x)))))
(load-package 'test2)
(load-package 'test3)
(in 'test3 '(run (define x 20)))
(in 'test3 '(open test2))
(in 'test2 '(run (test "shadowing" = 10 (z))))
(in 'test3 '(run (test "shadowing" = 20 (z))))
(in 'test1 '(run (test "shadowing" = 10 (z))))

(config '(run 
(define-structure foo (export)
  (open scheme testing
	assembler
	queues
	random
	sort
	big-scheme
	arrays
	dump/restore
	search-trees
	threads
	placeholders
	locks
	interrupts
	sicp)
  (begin

(test "* 1" = 6 (* 1 2 3))
(test "* 2" = (* 214760876 10) 2147608760)
(test "* 3" = (* 47123 46039) 2169495797)
(test "apply" equal? '(1 2 3 4) (apply list 1 2 '(3 4)))
(test "char<->integer" eq? #\a (integer->char (char->integer #\a)))
(test "lap" equal? #f ((lap #f (false) (return))))
(let ((q (make-queue)))
  (enqueue! q 'a)
  (test "q" eq? 'a (dequeue! q)))
(test "random" <= 0 ((make-random 7)))
(test "sort" equal? '(1 2 3 3) (sort-list '(2 3 1 3) <))
(test "bigbit" = (expt 2 100) (arithmetic-shift 1 100))
(test "format" string=? "x(1 2)" (format #f "x~s" '(1 2)))
(test "fancy input ports" equal? '(3 5)
      (let ((p1 (make-tracking-input-port (make-string-input-port "


    2 3"))))
	(read p1)
	(list (current-row p1) (current-column p1))))
(test "fancy output ports" equal? '(1 4 "8
   9")
      (let* ((r 0)
	     (c 0)
	     (s (call-with-string-output-port
		 (lambda (out)
		   (let ((out (make-tracking-output-port out)))
		     (write 8 out)
		     (fresh-line out)
		     (fresh-line out)
		     (display "   9" out)
		     (set! r (current-row out))
		     (set! c (current-column out))
		     (close-output-port out))))))
	(list r c s)))
(test "write-one-line" string=? "(1 2 3 4 5"
      (call-with-string-output-port
       (lambda (out)
	 (write-one-line out
			 10
			 (lambda (out)
			   (display '(1 2 3 4 5 6) out))))))
(test "destructure" eq? 'a (destructure (((x (y) z) '(b (a) c))) y))
(test "array" eq? 'a
      (let ((a (make-array 'b 3 4)))
	(array-set! a 'a 1 2)
	(array-ref a 1 2)))
(test "R4RS delay" = 3 (letrec ((p (delay (if c 3 (begin (set! c #t)
							 (+ (force p) 1)))))
				(c #f))
			 (force p)))
(test "receive" equal? '(a b) (receive stuff (values 'a 'b) stuff))
(let ((z '(a "b" 3 #t)))
  (test "dump" equal? z
	(let ((q (make-queue)))
	  (dump z (lambda (c) (enqueue! q c)) -1)
	  (restore (lambda () (dequeue! q))))))
(let ((r 0)
      (l1 (make-lock))
      (l2 (make-lock))
      (ph (make-placeholder)))
  (let ((f (lambda (i lock)
	     (spawn (lambda ()
		      (let ((v (placeholder-value ph)))
			(with-interrupts-inhibited
			 (lambda ()
			   (set! r (+ i v r))))
			(release-lock lock)))))))
    (obtain-lock l1)
    (obtain-lock l2)
    (f 1 l1)
    (f 2 l2)
    (placeholder-set! ph 10)
    (obtain-lock l1)
    (obtain-lock l2)
    (test "placeholders" = r 23)))
(test "explode" equal? 'ab3 (implode (explode 'ab3)))
(test "get/put" equal? 'a (begin (put 'foo 'prop 'a)
				 (get 'foo 'prop)))
(test "search-trees" eq? 'a
      (let ((t (make-search-tree = <)))
	(search-tree-set! t 3 'b)
	(search-tree-set! t 4 'a)
	(search-tree-set! t 5 'c)
	(search-tree-ref t 4)))

))))

(load-package 'foo)

(load-package 'floatnums)

(in 'foo '(run (let* ((one (exact->inexact 1))
		      (three (exact->inexact 3))
		      (third (/ one three))
		      (xthird (inexact->exact third)))
		 (test "float" eq? #f (= 1/3 xthird))
		 (test "exact<->inexact" = third (exact->inexact xthird)))))


; All done.

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))
